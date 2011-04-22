#include "remote.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <limits>
#include <map>
#include <utility>
#include <vector>
#include "circularbuffer.h"

#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "config.h"
#include "generictxn.h"
#include "message.h"

// #define STATIC_TXN_FILE "txn"
// #define VERBOSE 1

#if EXTRA_TXN
#define REDUCE_NETWORK_TRAFFIC for(int i__ = 0; i__ <= EXTRA_TXN; ++i__)
#else
#define REDUCE_NETWORK_TRAFFIC if(0) ; else
#endif

#if VERBOSE
#define INFO(...) printf(__VA_ARGS__)
#else
#define INFO(...) (void)0
#endif

using std::make_pair;
using std::map;
using std::pair;
using std::vector;

int prepare_sock(const char hostname[], int port, bool mute);
int prepare_serversock(int port);
ssize_t SendFD(int fd, const void* buf, size_t count);

map<const Configuration*, RemoteConnection*> RemoteConnection::_instances;
const int error_socket = std::numeric_limits<int>::min();

RemoteConnection* RemoteConnection::GetInstance(const Configuration& config){
    map<const Configuration*, RemoteConnection*>::iterator it =
        _instances.find(&config);
    if(it == _instances.end())
        it = _instances.insert(std::make_pair(
                    &config, new RemoteConnection(config))).first;
    return it->second;
}

RemoteConnection::RemoteConnection(const Configuration& config)
    : _config(config), _server_sock_msgs(-1), _fd_set_valid(false),
      _read_buf(new int64_t[GenericTxn::FIXED_HEADER_SIZE]),
      _read_buf_size(GenericTxn::FIXED_HEADER_SIZE){
    map<int, Node*>::const_iterator it = config.allnodes.find(config.myNodeID);
    if(it == config.allnodes.end()){
        printf("myNodeID (%ld) is not listed as a node.\n", config.myNodeID);
        exit(0);
    }


    const Node* node = it->second;
    myNodeType = node->nodeType;
    switch(node->nodeType){
        case DB_NODE:
            InitializeDB(reinterpret_cast<const DBNode*>(node));
            break;
        case PREPROCESSOR_NODE:
            InitializePreprocessor(reinterpret_cast<const PreprocessorNode*>(node));
            break;
        case MEDIATOR_NODE:
            InitializeMediator(reinterpret_cast<const MediatorNode*>(node));
            break;
    }
}

#define ADD_DB(ID,REPLICA,PARTITION,FD)         \
    do{                                         \
        _fds[ID] = (FD);                        \
        _db_fds[(PARTITION)][(REPLICA)] = (FD); \
        _fd_set_valid = false;                  \
    }while(0)

#define ADD_PREPROC(ID,REPLICA,PARTITION,FD)         \
    do{                                              \
        _fds[(ID)] = (FD);                           \
        _preproc_fds[(PARTITION)][(REPLICA)] = (FD); \
        _fd_set_valid = false;                       \
    }while(0)

#define ADD_MEDIATOR(ID,PARTITION,FD)      \
    do{                                    \
        _fds[(ID)] = (FD);                 \
        _mediator_fds[(PARTITION)] = (FD); \
        _fd_set_valid = false;             \
    }while(0)

#define ADD_CLIENT(FD)            \
    do{                           \
        _client_fds.insert((FD)); \
        _fd_set_valid = false;    \
    }while(0)

void RemoteConnection::AddConnection(const Node* node, int fd){
    switch(node->nodeType){
    case DB_NODE: {
        const DBNode* the_node = reinterpret_cast<const DBNode*>(node);
        ADD_DB(the_node->nodeID, the_node->replicaID,
               the_node->partitionID, fd);
        break;
    }
    case PREPROCESSOR_NODE:
    if(myNodeType == DB_NODE){
        ADD_CLIENT(fd);
    }else{
        const PreprocessorNode* the_node =
            reinterpret_cast<const PreprocessorNode*>(node);
        ADD_PREPROC(the_node->nodeID, the_node->replicaID,
                    the_node->partitionID, fd);
    }
    break;
    case MEDIATOR_NODE: {
        const MediatorNode* the_node =
            reinterpret_cast<const MediatorNode*>(node);
        ADD_MEDIATOR(the_node->nodeID, the_node->partitionID,
                     fd);
        break;
    }
    }
}

void RemoteConnection::InitializeDB(const DBNode* node){
    _server_sock_txns = prepare_serversock(node->port_for_new_txns);
    _server_sock_msgs = prepare_serversock(node->port_for_txn_msgs);
    if(_server_sock_txns < 0 || _server_sock_msgs < 0)
        exit(0);

    INFO("Listening to port %d (fd = %d)\n",
           node->port_for_new_txns, _server_sock_txns);
    INFO("Listening to port %d (fd = %d)\n",
           node->port_for_txn_msgs, _server_sock_msgs);

    // Insert fd -1 for all DB nodes with nodeID lower than us:
    // we are responsable for connecting to them.
    for(map<int, Node*>::const_iterator it = _config.allnodes.begin();
        it != _config.allnodes.end() && it->second->nodeID < node->nodeID; ++it)
        if(it->second->nodeType == DB_NODE &&
           it->second->nodeID < node->nodeID)
            AddConnection(it->second, -1);

    TryReconnect();
}

void RemoteConnection::InitializePreprocessor(const PreprocessorNode* node){
    _server_sock_txns = prepare_serversock(node->port);
    if(_server_sock_txns < 0)
        exit(0);

    INFO("Listening to port %d (fd = %d)\n", node->port, _server_sock_txns);

    // Fill DB fd tables with -1 and Preproc fd tables with -1 we are in
    // replica 0.
    for(map<int, Node*>::const_iterator it = _config.allnodes.begin();
        it != _config.allnodes.end(); ++it)
        switch(it->second->nodeType){
        case DB_NODE: 
            AddConnection(it->second, -1);
            break;
        case PREPROCESSOR_NODE:
            if(node->replicaID == 0){
                const PreprocessorNode* the_node =
                    reinterpret_cast<const PreprocessorNode*>(it->second);
                if(the_node->replicaID > 0 &&
                   the_node->partitionID == node->partitionID)
                    AddConnection(the_node, -1);
                break;
            }
        case MEDIATOR_NODE:
            break;
        }
}

void RemoteConnection::InitializeMediator(const MediatorNode* node){
    _server_sock_txns = prepare_serversock(node->port);
    if(_server_sock_txns < 0)
        exit(0);

    INFO("Listening to port %d (fd = %d)\n", node->port, _server_sock_txns);

    // Fill DB and Preproc fd tables with -1
    for(map<int, Node*>::const_iterator it = _config.allnodes.begin();
        it != _config.allnodes.end(); ++it)
        if(it->second->nodeType == DB_NODE ||
           it->second->nodeType == PREPROCESSOR_NODE)
            AddConnection(it->second, -1);

    TryReconnect();
}

void RemoteConnection::TryReconnect(){
#define IN_PROGRESS(NODE_ID,FD) do{                            \
        _in_progress_nodes.insert(make_pair((NODE_ID), (FD))); \
        _in_progress_fds.insert(make_pair((FD), (NODE_ID)));   \
        _fd_set_valid = false;                                 \
    }while(0)
    for(map<int, int>::const_iterator it = _fds.begin();
        it != _fds.end(); ++it)
        if(it->second < 0 && it->second != error_socket){
            const Node* node = _config.allnodes.find(it->first)->second;
            if(_in_progress_nodes.find(node->nodeID) !=
               _in_progress_nodes.end())
                continue;

            switch(node->nodeType){
            case DB_NODE: {
                const DBNode* the_node = reinterpret_cast<const DBNode*>(node);
                int fd;
                if(myNodeType == DB_NODE){
                    fd = prepare_sock(the_node->host.c_str(),
                                      the_node->port_for_txn_msgs,
                                      false);
                    IN_PROGRESS(the_node->nodeID, fd);
                }else{
                    fd = prepare_sock(the_node->host.c_str(),
                                      the_node->port_for_new_txns,
                                      false);
                    IN_PROGRESS(the_node->nodeID, fd);
                }
                break;
            }
            case PREPROCESSOR_NODE: {
                const PreprocessorNode* the_node =
                    reinterpret_cast<const PreprocessorNode*>(node);
                int fd = prepare_sock(the_node->host.c_str(),
                                      the_node->port,
                                      false);
                IN_PROGRESS(the_node->nodeID, fd);
                break;
            }
            case MEDIATOR_NODE:
                break;
            }
        }
#undef IN_PROGRESS
}

void RemoteConnection::BuildFDSets(){
#define ADD(FD,FDSET,FDSETMAX) do{ \
    FD_SET((FD), &(FDSET));        \
    if((FD) > (FDSETMAX))          \
        (FDSETMAX) = (FD);         \
    }while(0)

    _in_progress_fd_max = 0;
    FD_ZERO(&_in_progress_fd_set);
    for(map<int, int>::const_reverse_iterator it = _in_progress_fds.rbegin();
        it != _in_progress_fds.rend(); ++it)
        ADD(it->first, _in_progress_fd_set, _in_progress_fd_max);
    ++_in_progress_fd_max;

    _txns_fd_max = 0;
    _msgs_fd_max = 0;
    FD_ZERO(&_txns_fd_set);
    FD_ZERO(&_msgs_fd_set);

    switch(myNodeType){
    case DB_NODE:
        ADD(_server_sock_txns, _txns_fd_set, _txns_fd_max);
        ADD(_server_sock_msgs, _msgs_fd_set, _msgs_fd_max);
        for(map<int, int>::const_iterator it = _mediator_fds.begin();
            it != _mediator_fds.end(); ++it)
            if(it->second >= 0)
                ADD(it->second, _txns_fd_set, _txns_fd_max);
        for(set<int>::const_iterator it = _client_fds.begin();
            it != _client_fds.end(); ++it)
            if(*it >= 0)
                ADD(*it, _txns_fd_set, _txns_fd_max);
        for(map<int, map<int, int> >::const_iterator it = _db_fds.begin();
            it != _db_fds.end(); ++it)
            for(map<int, int>::const_iterator jt = it->second.begin();
                jt != it->second.end(); ++jt)
                if(jt->second >= 0)
                    ADD(jt->second, _msgs_fd_set, _msgs_fd_max);
        break;

    case PREPROCESSOR_NODE:
        ADD(_server_sock_txns, _txns_fd_set, _txns_fd_max);
        for(map<int, int>::const_iterator it = _mediator_fds.begin();
            it != _mediator_fds.end(); ++it)
            if(it->second >= 0)
                ADD(it->second, _txns_fd_set, _txns_fd_max);
        break;

    case MEDIATOR_NODE:
        ADD(_server_sock_txns, _txns_fd_set, _txns_fd_max);
        for(set<int>::const_iterator it = _client_fds.begin();
            it != _client_fds.end(); ++it)
            if(*it >= 0)
                ADD(*it, _txns_fd_set, _txns_fd_max);
        for(map<int, map<int, int> >::const_iterator it = _preproc_fds.begin();
            it != _preproc_fds.end(); ++it)
            for(map<int, int>::const_iterator jt = it->second.begin();
                jt != it->second.end(); ++jt)
                if(jt->second >= 0)
                    ADD(jt->second, _msgs_fd_set, _msgs_fd_max);
        for(map<int, map<int, int> >::const_iterator it = _db_fds.begin();
            it != _db_fds.end(); ++it)
            for(map<int, int>::const_iterator jt = it->second.begin();
                jt != it->second.end(); ++jt)
                if(jt->second >= 0)
                    ADD(jt->second, _msgs_fd_set, _msgs_fd_max);
        break;
    }

    ++_txns_fd_max;
    ++_msgs_fd_max;
    _fd_set_valid = true;
#undef ADD
}

void RemoteConnection::AcceptingConnection(int server_sock){
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_len;
    int client_sock =
        accept(server_sock,
               reinterpret_cast<struct sockaddr*>(&cli_addr),
               &cli_addr_len);

    uint32_t ip = ntohl(cli_addr.sin_addr.s_addr);
    char buf[32];
    snprintf(buf, sizeof(buf), "%u.%u.%u.%u",
            ip >> 24, (ip >> 16) & 0xff, (ip >> 8) & 0xff, ip & 0xff);
    INFO("Accepting connection from %s (fd = %d)\n", buf, client_sock);

    if(myNodeType == MEDIATOR_NODE){
        ADD_CLIENT(client_sock);
        return;
    }

    int64_t node_id;
    read(client_sock, &node_id, sizeof(node_id));
    INFO("Header yields: nodeID = %ld\n", node_id);

    map<int, Node*>::const_iterator it = _config.allnodes.find(node_id);
    if(it == _config.allnodes.end()){
        printf("Unknown node\n");
        close(client_sock);
        return;
    }
    const Node* node = it->second;
    if(node->host.compare(buf) == 0 || ip == 0 || ip == 0x7F000001){
        // IPs match, from 0.0.0.0 or  127.0.0.1 (both mean localhost)
        AddConnection(node, client_sock);
    }else{
        printf("Node registered on different address \"%s\"\n",
               node->host.c_str());
        // close(client_sock);
    }
}

int RemoteConnection::ConnectionEstablished(fd_set* fds){
    // Type: [fd, nodeID]
    vector<pair<int, int> > erasing;
    int n = 0;
    for(map<int, int>::const_iterator it = _in_progress_fds.begin();
        it != _in_progress_fds.end(); ++it){
        int sock = it->first;
        if(FD_ISSET(sock, fds)){
            int error = 0;
            socklen_t len = sizeof(error);
            if(getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len) != 0 ||
               error != 0){
                close(sock);
                for(map<int, int>::const_iterator it = _fds.begin();
                    it != _fds.end(); ++it)
                    if(sock == it->second){
                        Disconnected(it->first);
                        break;
                    }
                /*
                AddConnection(_config.allnodes.find(it->second)->second,
                              -1); //error_socket);
                printf("Connection to node %d failed (fd = %d)\n",
                       it->second, sock);
                */
            }else{
                fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) & ~O_NONBLOCK);
                write(sock, &_config.myNodeID, sizeof(_config.myNodeID));
                AddConnection(_config.allnodes.find(it->second)->second, sock);
                INFO("Connection to node %d ready (fd = %d)\n",
                       it->second, sock);
            }
            erasing.push_back(*it);
            ++n;
        }
    }
    if(erasing.size() > 0){
        _fd_set_valid = false;
        for(vector<pair<int, int> >::const_iterator it = erasing.begin();
            it != erasing.end(); ++it){
            _in_progress_fds.erase(it->first);
            _in_progress_nodes.erase(it->second);
        }
    }
    return n;
}

void RemoteConnection::FillIncomingTxns(
        CircularBuffer<GenericTxn>* txns,
        int64_t wait_msec,
        CircularBuffer<int>* origin){
#ifdef STATIC_TXN_FILE
    {
        int fd = open(STATIC_TXN_FILE, O_RDONLY);
        GenericTxn txn;
        while(!ReadTxn(fd, &txn)){
            bool ok = false;
            for(int i = 0; i < txn.rsetsize; ++i)
                if(_config.partitionContainingRecord(txn.rset[i]) == _config.myNodeID)
                    ok = true;
            for(int i = 0; i < txn.wsetsize; ++i)
                if(_config.partitionContainingRecord(txn.wset[i]) == _config.myNodeID)
                    ok = true;
            if(ok)
                txns->push_back(txn);
        }
        close(fd);
        return;
    }
#endif  // defined(STATIC_TXN_FILE)

    static int retryCount;

    TryReconnect();
    txns->clear();

    CircularBuffer<int> tmp_origin;
    if(origin == 0)
        origin = &tmp_origin;

    if(!_fd_set_valid)
        BuildFDSets();

    timeval timeout = {
        /* tv_sec */  wait_msec / 1000000,
        /* tv_usec */ wait_msec % 1000000
    };
    timeval* timeout_p = &timeout;
    if(wait_msec < 0)
        timeout_p = NULL;
    fd_set readset = _txns_fd_set;
    fd_set writeset = _in_progress_fd_set;
    int actions = select(
        _txns_fd_max > _in_progress_fd_max ?
        _txns_fd_max : _in_progress_fd_max,
        &readset, &writeset, NULL, timeout_p);

    if(actions < 0){
        // INFO("Select(txn) error: %s\n", strerror(errno));
        if(errno == EBADF)
            _fd_set_valid = false;
        else if(errno != EINTR && ++retryCount > 3)
            exit(0);
        return;
    }else if(actions == 0)
        return;

    actions -= ConnectionEstablished(&writeset);

    if(actions == 0)
        return;
    if(FD_ISSET(_server_sock_txns, &readset)){
        AcceptingConnection(_server_sock_txns);

        if(--actions == 0)
            return;
    }

    GenericTxn txn;
    if(myNodeType == MEDIATOR_NODE){
        vector<int> erasing;
        for(set<int>::const_iterator it = _client_fds.begin();
            it != _client_fds.end() && actions; ++it){
            if(FD_ISSET(*it, &readset)){
                --actions;
                if(ReadTxn(*it, &txn)){
                    erasing.push_back(*it);
                }else{
                    REDUCE_NETWORK_TRAFFIC{
                        txns->push_back(txn);
                        origin->push_back(*it);
                    }
                }
            }
        }

        if(erasing.size() > 0){
            _fd_set_valid = false;
            for(vector<int>::const_iterator it = erasing.begin();
                it != erasing.end(); ++it){
                close(*it);
                _client_fds.erase(*it);
            }
        }
    }else{
        for(map<int, int>::const_iterator it = _fds.begin();
            it != _fds.end() && actions; ++it){
            if(it->second >= 0 && FD_ISSET(it->second, &readset)){
                --actions;

                int64_t n;
                int c, r = 0;
                while(r < sizeof(n) && (c = read(it->second, &n, sizeof(n) - r))
                        > 0)
                    r += c;
                if(r < sizeof(n)){
                    printf("Not receiving enough bytes from %d (%d, %d)\n",
                           it->first, r, c);
                    Disconnected(it->first);
                    continue;
                }
                for(int64_t i = 0; i < n; ++i){
                    if(ReadTxn(it->second, &txn)){
                        printf("Not receiving full txn from %d\n",
                               it->first);
                        Disconnected(it->first);
                        break;
                    }
                    REDUCE_NETWORK_TRAFFIC{
                        txns->push_back(txn);
                        origin->push_back(it->second);
                    }
                }  // next i
            }  // if(FD_ISSET(it->second, &readset))
        }  // next it

        if(myNodeType == DB_NODE && actions){
            vector<int> erasing;
            for(set<int>::const_iterator it = _client_fds.begin();
                    it != _client_fds.end() && actions; ++it){
                if(FD_ISSET(*it, &readset)){
                    --actions;

                    int64_t n;
                    int c, r = 0;
                    while(r < sizeof(n) && (c = read(*it, &n, sizeof(n) - r))
                            > 0)
                        r += c;
                    if(r < sizeof(n)){
                        printf("Not receiving enough bytes from fd %d (%d, %d)\n",
                               *it, r, c);
                        erasing.push_back(*it);
                        continue;
                    }
                    for(int64_t i = 0; i < n; ++i){
                        if(ReadTxn(*it, &txn)){
                            printf("Not receiving full txn from fd %d\n",
                                   *it);
                            erasing.push_back(*it);
                            break;
                        }
                        REDUCE_NETWORK_TRAFFIC{
                            txns->push_back(txn);
                            origin->push_back(*it);
                        }
                    }  // next i
                }
            }

            if(erasing.size() > 0){
                _fd_set_valid = false;
                for(vector<int>::const_iterator it = erasing.begin();
                        it != erasing.end(); ++it){
                    close(*it);
                    _client_fds.erase(*it);
                }
            }
        } // myNodeType == DB_NODE
    }

    /*for(CircularBuffer<GenericTxn>::iterator it = txns->begin();
        it != txns->end(); ++it)
        printf("txn => uid = %ld; id = %ld; om = %ld\n",
               it->txnid_unordered, it->txnid, it->source_mediator);*/
}

void RemoteConnection::FillIncomingMessages(CircularBuffer<Message>* msgs){
    static int retryCount;

    TryReconnect();
    msgs->clear();

    if(!_fd_set_valid)
        BuildFDSets();

    timeval timeout = {0, 0};  // returns immediately
    fd_set readset = _msgs_fd_set;
    fd_set writeset = _in_progress_fd_set;
    int actions = select(
        _msgs_fd_max > _in_progress_fd_max ?
        _msgs_fd_max : _in_progress_fd_max,
        &readset, &writeset, NULL, &timeout);

    if(actions < 0){
        // INFO("Select(msg) error: %s\n", strerror(errno));
        if(errno == EBADF)
            _fd_set_valid = false;
        else if(errno != EINTR && ++retryCount > 3)
            exit(0);
        return;
    }else if(actions == 0)
        return;

    actions -= ConnectionEstablished(&writeset);

    if(actions == 0)
        return;
    if(_server_sock_msgs >= 0 && FD_ISSET(_server_sock_msgs, &readset)){
        AcceptingConnection(_server_sock_msgs);

        if(--actions == 0)
            return;
    }

    Message msg(0, 0, 0, 0, 0);
    for(map<int, int>::const_iterator it = _fds.begin();
        it != _fds.end() && actions; ++it){
        if(it->second >= 0 && FD_ISSET(it->second, &readset)){
            --actions;

            if(ReadMsg(it->second, &msg)){
                printf("Not receiving full msg from %d\n",
                       it->first);
                Disconnected(it->first);
                break;
            }
            msgs->push_back(msg);
        }
    }
}

int RemoteConnection::ReadTxn(int fd, GenericTxn* txn){
    // Reliable read pattern
    int count = GenericTxn::FIXED_HEADER_SIZE * sizeof(int64_t);
    char* buf = reinterpret_cast<char*>(_read_buf);
    int c, n = 0;
    while(n < count && (c = read(fd, buf + n, count - n)) > 0)
        n += c;
    if(n < count)
        return -1;

    int64_t size = _read_buf[GenericTxn::RSETSIZE_IDX] +
                   _read_buf[GenericTxn::WSETSIZE_IDX] +
                   _read_buf[GenericTxn::ARGCOUNT_IDX];
    if(size + GenericTxn::FIXED_HEADER_SIZE > _read_buf_size){
        _read_buf_size = size + GenericTxn::FIXED_HEADER_SIZE;

        int64_t* t = new int64_t[_read_buf_size];
        std::copy(_read_buf, _read_buf + GenericTxn::FIXED_HEADER_SIZE, t);

        delete [] _read_buf;
        _read_buf = t;
    }

    // Reliable read pattern
    count = size * sizeof(int64_t);
    buf = reinterpret_cast<char*>(_read_buf + GenericTxn::FIXED_HEADER_SIZE);
    n = 0;
    while(n < count && (c = read(fd, buf + n, count - n)) > 0)
        n += c;
    if(n < count)
        return -1;

    txn->deserialize(_read_buf);
    return 0;
}

int RemoteConnection::ReadMsg(int fd, Message* msg){
    // Reliable read pattern
    int count = Message::FIXED_HEADER_SIZE * sizeof(int64_t);
    char* buf = reinterpret_cast<char*>(_read_buf);
    int c, n = 0;
    while(n < count && (c = read(fd, buf + n, count - n)) > 0)
        n += c;
    if(n < count){
        printf("DEAD at header (fd = %d) %d/%d\n", fd, n, count);
        return -1;
    }

    // number of int64_t's needed to hold the data
    int64_t size = (_read_buf[Message::DATASIZE_IDX] + sizeof(int64_t) - 1)
                   / sizeof(int64_t);
    if(size + Message::FIXED_HEADER_SIZE > _read_buf_size){
        _read_buf_size = size + Message::FIXED_HEADER_SIZE;

        int64_t* t = new int64_t[_read_buf_size];
        std::copy(_read_buf, _read_buf + Message::FIXED_HEADER_SIZE, t);

        delete [] _read_buf;
        _read_buf = t;
    }

    // Reliable read pattern
    count = _read_buf[Message::DATASIZE_IDX];
    buf = reinterpret_cast<char*>(_read_buf + Message::FIXED_HEADER_SIZE);
    n = 0;
    while(n < count && (c = read(fd, buf + n, count - n)) > 0)
        n += c;
    if(n < count){
        printf("DEAD at body (fd = %d) %d/%d\n", fd, n, count);
        return -1;
    }

    *msg = Message(_read_buf[0], _read_buf[1], _read_buf[2], _read_buf[3],
                   _read_buf + Message::FIXED_HEADER_SIZE);
    return 0;
}

void RemoteConnection::Disconnected(int node_id){
    const Node* node = _config.allnodes.find(node_id)->second;
    int fd = _fds[node_id];
    if(fd >= 0)
        close(fd);
    _fd_set_valid = false;

    INFO("node %d (fd = %d) is considered disconnected\n", node_id, fd);

    switch(node->nodeType){
    case DB_NODE: {
        const DBNode* the_node = reinterpret_cast<const DBNode*>(node);
        if(myNodeType == DB_NODE && _config.myNodeID < node_id){
            _fds.erase(node_id);
            _db_fds[the_node->partitionID].erase(the_node->replicaID);
        }else{
            ADD_DB(the_node->nodeID, the_node->replicaID,
                    the_node->partitionID, -1);
        }
        break;
    }
    case PREPROCESSOR_NODE: {
        const PreprocessorNode* the_node =
            reinterpret_cast<const PreprocessorNode*>(node);
        if(myNodeType == MEDIATOR_NODE)
            ADD_PREPROC(the_node->nodeID, the_node->replicaID,
                        the_node->partitionID, -1);
        else{
            _fds.erase(node_id);
            _preproc_fds[the_node->partitionID].erase(the_node->replicaID);
        }
        break;
    }
    case MEDIATOR_NODE: {
        const MediatorNode* the_node =
            reinterpret_cast<const MediatorNode*>(node);
        _fds.erase(node_id);
        _mediator_fds.erase(the_node->partitionID);
        break;
    }
    }
}

inline ssize_t SendFD(int fd, const void* buf, size_t count){
    if(fd < 0)
        return -1;

    // Reliable write pattern
    const char* buf_ = reinterpret_cast<const char*>(buf);
    int c;
    size_t n = 0;
    while(n < count && (c = write(fd, buf_ + n, count - n)) > 0)
        n += c;
    return n;
}

ssize_t RemoteConnection::Send(int node_id, const void* buf,
                                      size_t count) const{
    map<int, int>::const_iterator it = _fds.find(node_id);
    if(it == _fds.end() || it->second < 0)
        return -1;
    int fd = it->second;
    return SendFD(fd, buf, count);
}

ssize_t RemoteConnection::SendReplicasAndPartitions(
        const map<int, map<int, int> >& fd_map,
        int replica, int partition,
        const void* buf, size_t count){
    map<int, map<int, int> >::const_iterator it = fd_map.find(partition);
    if(it == fd_map.end())
        return -1;
    if(replica < 0){
        TryReconnect();
        ssize_t number_sent = std::numeric_limits<ssize_t>::max();
        const map<int, int>& fds = it->second;
        for(map<int, int>::const_iterator jt = fds.begin();
            jt != fds.end(); ++jt){
            ssize_t n = SendFD(jt->second, buf, count);
            if(n < number_sent)
                number_sent = n;
        }
        return number_sent;
    }else{
        map<int, int>::const_iterator jt = it->second.find(replica);
        if(jt == it->second.end())
            return -1;
        if(jt->second < 0)
            TryReconnect();
        return SendFD(jt->second, buf, count);
    }
}

ssize_t RemoteConnection::SendDB(int replica, int partition, const void* buf,
                                 size_t count){
    return SendReplicasAndPartitions(_db_fds, replica, partition, buf, count);
}

ssize_t RemoteConnection::SendDBMsg(const Message& message){
    map<int, int>::const_iterator it = _fds.find(message.recipient);
    if(it == _fds.end())
        return -1;
    if(it->second < 0)
        TryReconnect();
    int64_t buf[Message::FIXED_HEADER_SIZE] = {
        message.txnid,
        message.sender,
        message.recipient,
        message.size,
    };
    if(SendFD(it->second, buf, sizeof(buf)) < static_cast<ssize_t>(sizeof(buf)))
        return -1;
    return SendFD(it->second, message.data, message.size);
}

ssize_t RemoteConnection::SendPreprocessor(int replica, int partition,
                                           const void* buf, size_t count){
    return SendReplicasAndPartitions(_preproc_fds, replica, partition,
                                     buf, count);
}

ssize_t RemoteConnection::SendMediator(int partition, const void* buf,
                                       size_t count){
    map<int, int>::const_iterator it = _mediator_fds.find(partition);
    if(it == _mediator_fds.end())
        return -1;
    if(it->second < 0)
        TryReconnect();
    return SendFD(it->second, buf, count);
}

int prepare_sock(const char hostname[], int port, bool mute){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        if(!mute)
            printf("Socket creation failed: %s\n", strerror(errno));
        return -1;
    }

    if(fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0){
        if(!mute)
            printf("Socket flag setting failed: %s\n", strerror(errno));
        return -1;
    }

    struct hostent* host = gethostbyname(hostname);
    if(host == NULL){
        if(!mute)
            printf("Cannot resolve host %s: %s\n", hostname, strerror(errno));
        return -1;
    }

    struct sockaddr_in srv_addr;
    srv_addr.sin_family = AF_INET;
    memmove(&srv_addr.sin_addr, host->h_addr_list[0], host->h_length);
    srv_addr.sin_port = htons(port);

    if(connect(sock, reinterpret_cast<const sockaddr*>(&srv_addr),
                     sizeof(srv_addr)) < 0){
        if(errno == EINPROGRESS){
            if(!mute)
                INFO("Connection to %s:%d in progress (fd = %d)\n",
                       hostname, port, sock);
            return sock;
        }else{
            if(!mute)
                INFO("Connection to %s:%d refused: %s (fd = %d)\n",
                       hostname, port, strerror(errno), sock);
            return -1;
        }
    }
    return sock;
}

int prepare_serversock(int port){
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock < 0){
        printf("Socket creation failed: %s\n", strerror(errno));
        return -1;
    }

    struct sockaddr_in srv_addr;
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = INADDR_ANY; // binding on all interfaces
    srv_addr.sin_port = htons(port);

    if(bind(server_sock, reinterpret_cast<struct sockaddr*>(&srv_addr),
            sizeof(srv_addr)) < 0){
        printf("Socket binding failed: %s\n", strerror(errno));
        return -1;
    }
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, 0, 0);

    if(listen(server_sock, 1) < 0){
        printf("Socket listening failed: %s\n", strerror(errno));
        return -1;
    }
    return server_sock;
}
