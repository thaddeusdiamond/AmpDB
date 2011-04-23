#include "server.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <cerrno>

#include <algorithm>
#include <limits>
#include <map>
#include <set>
#include <utility>
#include <vector>
#include <deque>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../config.h"
#include "../generictxn.h"
#include "../remote.h"
#include "../message.h"
#include "../circularbuffer.h"

#define DEQUE CircularBuffer

// #define VERBOSE 1
#define GENERATE_TXN 5000  // pool size

// #define EXTRA_TXN 1  // generate EXTRA_TXN extra txns for each one from clients
#if EXTRA_TXN || GENERATE_TXN
#include "../loadgen/loadgen.h"
#define NUM_WAREHOUSE 10
#endif

const int RSETSIZE_IDX = 5;
const int WSETSIZE_IDX = 6;
const int RSET_OFFSET = 8;

using std::deque;
using std::make_pair;
using std::map;
using std::set;
using std::vector;

namespace Mediator{
static bool server_running;

int prepare_serversock(int port);

void stop_server(int sig){
    server_running = false;
}

MediatorServer::MediatorServer(Configuration& config)
    : _config(config),
      _remote(*RemoteConnection::GetInstance(config)),
      _next_partition(0){
    uint64_t t = 1;
    int partition_bits = 0;
    while(_config.mediators.size() > t){
        ++partition_bits;
        t <<= 1;
    }
    int64_t myPartitionID = reinterpret_cast<MediatorNode*>(
            _config.allnodes.find(_config.myNodeID)->second)->partitionID;
    _next_txnid = myPartitionID << (63 - partition_bits);
}

int MediatorServer::StartServer(){
#if VERBOSE
    int received = 0;
    int count = 0;
#endif
#if GENERATE_TXN
    int generated = 0;
    timespec starting_time;
    int active_txns = GENERATE_TXN;
    {
        map<string, string>::const_iterator it = 
            _config.other_opt.find("active_txns");
        if(it != _config.other_opt.end())
            active_txns = atoi(it->second.c_str());
    }
    clock_gettime(CLOCK_MONOTONIC, &starting_time);
#endif

    server_running = true;
    signal(SIGINT, &stop_server);
    signal(SIGTERM, &stop_server);
    signal(SIGPIPE, &stop_server);

    deque<GenericTxn> queued_txns;
    deque<int> queued_origin;
    _result_count = 0;
    while(server_running){
        deque<GenericTxn> txns;
        deque<int> origin;

#if ! GENERATE_TXN
        _remote.FillIncomingTxns(&txns, 0, &origin);
#else
        {
            int need = active_txns - _txn_origin.size() - queued_txns.size();
            if(need <= 0){
                timespec to_sleep = { 0, 1000000 };  // 1 ms
                nanosleep(&to_sleep, NULL);
            }else{
                generated += need;
                origin.resize(need, -1);
                for(int i = 0; i < need; ++i){
                    GenericTxn* txn = generate(0, NUM_WAREHOUSE);
                    txns.push_back(*txn);
                    delete txn;
                }
            }
        }
#endif  // GENERATE_TXN

#if VERBOSE
        received += txns.size();
        if(++count % 100 == 0){
            printf("Waiting: got %d\n", received);
            received = 0;
        }
#endif

        txns.insert(txns.end(), queued_txns.begin(), queued_txns.end());
        origin.insert(origin.end(),
                      queued_origin.begin(), queued_origin.end());

        queued_txns.clear();
        queued_origin.clear();

        deque<int>::const_iterator jt = origin.begin();
        for(deque<GenericTxn>::iterator it = txns.begin();
            it != txns.end(); ++it, ++jt){
            it->txnid_unordered = it->txnid = _next_txnid++;
            it->source_mediator = _config.myNodeID;
            if(ProcessTxn(*it) >= 0){
                _txn_origin.insert(make_pair(it->txnid_unordered, *jt));
            }else{
                queued_txns.push_back(*it);
                queued_origin.push_back(*jt);
                --_next_txnid;
            }
#if EXTRA_TXN
            for(int i = 0; i < EXTRA_TXN; ++i){
                GenericTxn extra_txn;
                generatetpcctxn(&extra_txn, _config.dbpartitions.size());
                extra_txn.txnid_unordered = extra_txn.txnid = _next_txnid++;
                _txn_origin.insert(make_pair(extra_txn.txnid_unordered, -1));
                ProcessTxn(extra_txn);
            }
#endif
        }

        DEQUE<Message> msgs;
        _remote.FillIncomingMessages(&msgs);

        for(DEQUE<Message>::iterator it = msgs.begin();
            it != msgs.end(); ++it)
            ProcessMsg(*it);
    }
#if GENERATE_TXN
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    long diff_sec = now.tv_sec - starting_time.tv_sec;
    long diff_nsec = now.tv_nsec - starting_time.tv_nsec;

    if(diff_nsec < 0){
        diff_nsec += 1000000000;
        --diff_sec;
    }
    double sec = diff_sec;
    sec *= 1000000000;
    sec += diff_nsec;
    sec /= 1000000000;

    printf("Finished %d transactions in %ld.%09ld seconds (gen %d)\n",
            _result_count, diff_sec, diff_nsec, generated);
    printf("throughput = %f txns/sec\n", _result_count / sec);
#endif  // GENERATE_TXN
    return 0;
}

int MediatorServer::ProcessTxn(const GenericTxn& txn){
#if EXTRA_TXN
    static int c = 0;
    if(++c % 4 != 0)
        return 0xdeadbeef;
#endif

    int result = -1;
    int64_t length;
    char* buf = reinterpret_cast<char*>(malloc(
        (1 + GenericTxn::FIXED_HEADER_SIZE +
         txn.rsetsize + txn.wsetsize + txn.argcount) *
        sizeof(int64_t)));
    *reinterpret_cast<int64_t*>(buf) = 1;
    txn.serialize_to(buf + sizeof(int64_t), &length);
    length += sizeof(int64_t);

    if(txn.isolationlevel == READ_UNCOMMITTED){
        // pure reading txn, no order needed
        set<int> slices;
        for(int i = 0; i < txn.rsetsize; ++i){
            int slice = _config.partitionContainingRecord(txn.rset[i]);
            if(slice >= 0)
                slices.insert(slice);
        }

        bool ok = true;
        for(set<int>::const_iterator it = slices.begin();
            it != slices.end(); ++it)
            if(_remote.SendDB(-1, *it, buf, length) < 0)
                ok = false;
        result = ok ? slices.size() : -1;
    }else{
        result = _remote.SendPreprocessor(0, _next_partition, buf, length);
        if(++_next_partition >= _config.preprocessorpartitions.size())
            _next_partition = 0;
    }

    free(buf);
    return result;
}

void MediatorServer::ProcessMsg(const Message& msg){
    map<int, Node*>::const_iterator it = _config.allnodes.find(msg.sender);
    if(it == _config.allnodes.end())
        printf("WARNING: unknown node %ld sent us a message of size %ld\n",
               msg.sender, msg.size);
    else switch(it->second->nodeType){
    case DB_NODE:
        // DB nodes give us computed result
        ProcessResult(msg);
        break;

    case PREPROCESSOR_NODE:
        // Preprocessor nodes give us the list of txnid_unordered for txns
        // which have been made durable.
        AddDurableIds(msg);
        break;

    default:
        printf("WARNING: node %ld is of type %d sent us a message of size %ld\n",
               msg.sender, it->second->nodeType, msg.size);
        break;
    }
}

void MediatorServer::ProcessResult(const Message& msg){
    if(_txn_origin.find(msg.txnid) == _txn_origin.end()){
        free(msg.data);
        return;
    }
    hash_set<int64_t>::iterator it = _durable.find(msg.txnid);
    if(it == _durable.end()){
        _pending_msgs.insert(make_pair(msg.txnid, msg));
    }else{
        _durable.erase(it);
        SendMsg(msg);
    }
}

void MediatorServer::AddDurableIds(const Message& msg){
    const int64_t* data = reinterpret_cast<const int64_t*>(msg.data);
    int nTxns = msg.size / sizeof(int64_t);
    for(int i = 0; i < nTxns; ++i){
        if(_txn_origin.find(data[i]) == _txn_origin.end())
            // We didn't give this txn out, people shouldn't give it to us.
            continue;
        hash_map<int, Message>::iterator it = _pending_msgs.find(data[i]);
        if(it != _pending_msgs.end()){
            _pending_msgs.erase(data[i]);
            SendMsg(it->second);
        }else{
            _durable.insert(data[i]);
        }
    }
    free(msg.data);
}

void MediatorServer::SendMsg(const Message& msg){
    hash_map<int64_t, int>::iterator it = _txn_origin.find(msg.txnid);
    const int fd = it->second;
    _txn_origin.erase(it);
    ++_result_count;

#if EXTRA_TXN || GENERATE_TXN
    if(fd == -1)
        return;
#endif

    const int count = 4 * sizeof(int64_t) + msg.size;
    char* buf = new char[count];
    int64_t* header = reinterpret_cast<int64_t*>(buf);
    header[0] = msg.txnid;
    header[1] = 0;
    header[2] = 0;
    header[3] = msg.size;
    memcpy(buf + 4 * sizeof(int64_t), msg.data, msg.size);

    // Reliable write pattern
    int c, n = 0;
    while(n < count && (c = write(fd, buf + n, count - n)) > 0)
        n += c;

    delete [] buf;
    free(msg.data);
}
}
