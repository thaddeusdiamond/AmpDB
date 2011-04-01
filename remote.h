#ifndef _YALE_SCALE_REMOTE_H_
#define _YALE_SCALE_REMOTE_H_ 1

#include <stdint.h>
#include <unistd.h>
#include <sys/select.h>

#include <deque>
#include <map>
#include <set>
#include "circularbuffer.h"

#include "config.h"

using std::deque;
using std::map;
using std::set;
using std::pair;

class GenericTxn;
class Message;

class RemoteConnection{
  public:
    ssize_t Send(int node_id, const void* buf, size_t count) const;

    ssize_t SendDB(int replica, int partition, const void* buf, size_t count);
    ssize_t SendDBMsg(const Message& message);

    ssize_t SendPreprocessor(int replica, int partition, const void* buf,
                             size_t count);
    ssize_t SendMediator(int partition, const void* buf, size_t count);

    void FillIncomingTxns(
            CircularBuffer<GenericTxn>* txns,
            int64_t wait_msec = 0LL,
            CircularBuffer<int>* origin = 0);
    void FillIncomingMessages(CircularBuffer<Message>* msgs);
    
    static RemoteConnection* GetInstance(const Configuration& config);

  private:
    explicit RemoteConnection(const Configuration& config);
    RemoteConnection(const RemoteConnection&);
    RemoteConnection& operator=(const RemoteConnection&);

    void InitializeDB(const DBNode* node);
    void InitializePreprocessor(const PreprocessorNode* node);
    void InitializeMediator(const MediatorNode* node);

    void TryReconnect();
    void BuildFDSets();
    void AcceptingConnection(int server_sock);
    void AddConnection(const Node* node, int fd);

    // process pending outgoing connections
    int ConnectionEstablished(fd_set* fds);
    void Disconnected(int node_id);

    inline ssize_t SendReplicasAndPartitions(
        const map<int, map<int, int> >& fd_map,
        int replica, int partition,
        const void* buf, size_t count);
    int ReadTxn(int fd, GenericTxn* txn);
    int ReadMsg(int fd, Message* msg);

    const Configuration& _config;

    // Type: nodeID -> fd
    map<int, int> _fds;

    // Connect to port_for_new_txns for preprocessors and mediators,
    // to port_for_txn_msgs for dbs. Type: partition -> (replica -> fd)
    map<int, map<int, int> > _db_fds;

    // Type: partition -> (replica -> fd)
    map<int, map<int, int> > _preproc_fds;

    // Type: partition -> fd
    map<int, int> _mediator_fds;

    // Mediator nodes: clients
    // DB nodes: preprocessors
    set<int> _client_fds;

    // Type: fd -> nodeID
    map<int, int> _in_progress_fds;
    // Type: nodeID -> fd
    map<int, int> _in_progress_nodes;

    int _server_sock_txns;
    int _server_sock_msgs;  // only used on DB nodes

    NodeType myNodeType;

    bool _fd_set_valid;
    int  _txns_fd_max;
    int  _msgs_fd_max;  // only used on DB and mediator nodes
    int  _in_progress_fd_max;
    fd_set _txns_fd_set;
    fd_set _msgs_fd_set;  // only used on DB and mediator nodes
    fd_set _in_progress_fd_set;

    int64_t* _read_buf;
    int _read_buf_size;

    static map<const Configuration*, RemoteConnection*> _instances;
};

#endif  // !defined(_YALE_SCALE_REMOTE_H_)
