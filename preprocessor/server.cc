#include "server.h"

#include <cstdio>
#include <cstring>
#include <csignal>
#include <cerrno>

#include <algorithm>
#include <deque>
#include <iterator>
#include <map>
#include <set>
#include <vector>
#include <ext/hash_map>
#include <ext/hash_set>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../remote.h"
#include "../message.h"
#include "config.h"
#include "conflict_resolver.h"
#include "scheduler.h"
#include "txninfo.h"
#include "../circularbuffer.h"

#define DEQUE CircularBuffer

// #define VERBOSE 1

#if EXTRA_TXN
#define REDUCE_NETWORK_TRAFFIC if(++c % (EXTRA_TXN + 1) != 0) continue; else
#else
#define REDUCE_NETWORK_TRAFFIC if(0) ; else
#endif

using std::deque;
using std::map;
using std::set;
using std::vector;
using __gnu_cxx::hash_map;
using __gnu_cxx::hash_set;

namespace Preproc{
class MasterStrategy : public ServerStrategy{
  public:
    MasterStrategy(const Configuration& config, RemoteConnection& remote)
        : ServerStrategy(config, remote){}

    void RunServer();

  private:
    void Listen();
    void ProcessBatch();
    void SendBatch(const vector<TxnInfo*>& batch);
    void MakeDurable(const vector<TxnInfo*>& batch);
    void InformMediator(const vector<TxnInfo*>& batch);

    vector<TxnInfo> _batch;
};

class SlaveStrategy : public ServerStrategy {
  public:
    SlaveStrategy(const Configuration& config, RemoteConnection& remote)
        : ServerStrategy(config, remote){}

    void RunServer();
};

ServerStrategy::ServerStrategy(const Configuration& config,
                               RemoteConnection& remote)
    : _config(config), _remote(remote), _binlog_count(0), _binlog_fd(-1){
    RotateBinlog();
}

void ServerStrategy::RotateBinlog(){
    if(_binlog_fd >= 0)
        close(_binlog_fd);

    char fname[_config.binlog_filename.length() + 16];
    PreprocessorNode* node = reinterpret_cast<PreprocessorNode*>(
            _config.allnodes.find(_config.myNodeID)->second);
    snprintf(fname, sizeof(fname), "%s.%d.%d.%d.bin",
             _config.binlog_filename.c_str(),
             node->replicaID, node->partitionID, _binlog_count++);
    _binlog_fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC | O_SYNC | O_APPEND, 0666);
    if(_binlog_fd < 0)
        printf("WARNING: cannot open %s for binlog writing.\n"
               "         Will lose all transaction information.\n", fname);
}

PreprocessorServer::PreprocessorServer(Configuration& config)
    : _config(config),
      _remote(*RemoteConnection::GetInstance(config)){
    if(config.allnodes.find(config.myNodeID) == config.allnodes.end() ||
       config.allnodes[config.myNodeID]->nodeType != PREPROCESSOR_NODE){
        printf("Our node is not preprocessor in config file\n");
        exit(0);
    }
    if(reinterpret_cast<PreprocessorNode*>(config.allnodes[config.myNodeID])
       ->replicaID == 0)
        _strategy = new MasterStrategy(config, _remote);
    else
        _strategy = new SlaveStrategy(config, _remote);
}

int PreprocessorServer::StartServer(){
    if(_config.instances <= 1){
        _config.instance_number = 0;
        _strategy->RunServer();
        return 0;
    }else{
        bool failed = false;
        int* pids = new int[_config.instances];
        int nChildren = 0;
        for(int i = 0; i < _config.instances; ++i){
            pid_t pid = fork();
            if(pid < 0){
                failed = true;
                break;
            }else if(pid == 0){
                _config.instance_number = i;
                _strategy->RunServer();
                _exit(0);
            }else{
                pids[i] = pid;
                ++nChildren;
            }
        }

        if (failed) {
            for(int i = 0; i < nChildren; ++i)
                if(pids[i])
                    kill(pids[i], SIGTERM);
        }

        while(nChildren > 0){
            wait(NULL);
            --nChildren;
        }
        return failed ? -1 : 0;
    }
}

inline int64_t now(){
    timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    return static_cast<int64_t>(spec.tv_sec) * 1000000000 +
           spec.tv_nsec;
}

void MasterStrategy::RunServer(){
#if VERBOSE
    int count = 0;
#endif
    while(1){
        // NOTE: deadline in nanosecond from unkown timepoint
        //       time_left in microsecond
        int64_t deadline = now() + _config.wait_time * 1000;
        int64_t time_left = _config.wait_time;

        do{
            DEQUE<GenericTxn> txns;
            _remote.FillIncomingTxns(&txns, time_left);

#if VERBOSE
            if(++count % 100 == 0){
                printf("Waiting (%d): got %lu\n", getpid(), _batch.size());
            }
#endif

            int64_t current = now();
            // Convert nanosecond to microsecond
            time_left = (deadline - current) / 1000;

            std::copy(txns.begin(), txns.end(),
                      std::back_inserter(_batch));
        }while(time_left > 0 && _batch.size() < _config.batch_size);

#if VERBOSE
        if(_batch.size() > 0){
            printf("%d: got %lu\n", getpid(), _batch.size());
        }
#endif

        ProcessBatch();
    }
}

void MasterStrategy::ProcessBatch(){
    if(_batch.size() == 0)
        return;

    hash_map<int64_t, hash_set<int64_t> > conflicts;
    ConflictResolver::GetInstance(_config)->FindConflicts(&_batch, &conflicts);

    vector<TxnInfo*> ordered;
    Scheduler::GetInstance(_config)->Schedule(conflicts, &_batch, &ordered);

    SendBatch(ordered);
    MakeDurable(ordered);

    _batch.clear();
}

struct TxnsSize{
    int64_t num_int64;
    int64_t count;
    char* buf;
    char* p;
};

void MasterStrategy::SendBatch(const vector<TxnInfo*>& batch){
#if EXTRA_TXN
    int c = 0;
#endif

    map<int, TxnsSize> total_size;
    for(vector<TxnInfo*>::const_iterator it = batch.begin();
        it != batch.end(); ++it)
        REDUCE_NETWORK_TRAFFIC{
            for(vector<int>::const_iterator jt = (*it)->_slices.begin();
                    jt != (*it)->_slices.end(); ++jt){
                TxnsSize& size = total_size[*jt];
                if(size.num_int64 == 0)
                    size.num_int64 = 1; // Initial txn count takes one
                size.num_int64 += GenericTxn::FIXED_HEADER_SIZE +
                                  (*it)->rsetsize + (*it)->wsetsize +
                                  (*it)->argcount;
                ++size.count;
            }
        }

    for(map<int, TxnsSize>::iterator it = total_size.begin();
        it != total_size.end(); ++it){
        int total_bytes = it->second.num_int64 * sizeof(int64_t);
        int64_t total_count = it->second.count;
        char* buf = new char[total_bytes];
        *reinterpret_cast<int64_t*>(buf) = total_count;

        it->second.buf = buf;
        it->second.p = buf + sizeof(int64_t);
#if VERBOSE
        printf("%d <== %"PRId64" (%d)\n", it->first, total_count, total_bytes);
#endif
    }

    for(vector<TxnInfo*>::const_iterator it = batch.begin();
        it != batch.end(); ++it)
        REDUCE_NETWORK_TRAFFIC{
            for(vector<int>::const_iterator jt = (*it)->_slices.begin();
                    jt != (*it)->_slices.end(); ++jt){
                TxnsSize& size = total_size[*jt];

                int64_t length;
                (*it)->serialize_to(size.p, &length);
                size.p += length;
            }
        }

    for(map<int, TxnsSize>::iterator it = total_size.begin();
        it != total_size.end(); ++it){
	while(_remote.SendDB(
		    -1, it->first, it->second.buf,
		    it->second.num_int64 * sizeof(int64_t)) < 0){
	}
        delete [] it->second.buf;
    }
}

void MasterStrategy::MakeDurable(const vector<TxnInfo*>& batch){
    if(_binlog_fd < 0)
        return;

    int pid = fork();
    if(pid < 0){
        printf("Fork failed, doing synchronized write");
    }if(pid > 0){
        // parent returns
        int status;
        waitpid(pid, &status, 0); // avoid zombie
        return;
    }else{
        // Child setsid to give up terminal and do the second fork for init
        // to take over the grandchild
        setsid();
        pid = fork();
        if(pid > 0)
            _exit(0);
        // Only grandchild reaches here, or, if we are unfortunate, child if
        // fork failed.
    }

    int64_t txns_size = 0;
    for(vector<TxnInfo*>::const_iterator it = batch.begin();
        it != batch.end(); ++it)
        txns_size += GenericTxn::FIXED_HEADER_SIZE +
                     (*it)->rsetsize + (*it)->wsetsize + (*it)->argcount;

    char* buf = new char[txns_size * sizeof(int64_t)];
    char* p = buf;
    for(vector<TxnInfo*>::const_iterator it = batch.begin();
        it != batch.end(); ++it){
        int64_t length;
        (*it)->serialize_to(p, &length);
        p += length;
    }

    int count = txns_size * sizeof(int64_t);
    int c, n = 0;
    while(n < count && (c = write(_binlog_fd, buf + n, count - n)) > 0)
        n += c;
    fsync(_binlog_count);
    delete [] buf;

    // TODO: send out the order and wait for half of the replicas to response
    InformMediator(batch);

    if(pid == 0)
        _exit(0);
}

void MasterStrategy::InformMediator(const vector<TxnInfo*>& batch){
    // Size of the message to mediator saying: committed
    // TODO: build lists for different mediators
    int64_t committed_size = batch.size() + Message::FIXED_HEADER_SIZE;
    int64_t* buf = new int64_t[committed_size];
    buf[0] = 0;
    buf[1] = _config.myNodeID;
    buf[2] = 0;
    buf[3] = batch.size() * sizeof(int64_t);
    int i = 4;
    for(vector<TxnInfo*>::const_iterator it = batch.begin();
        it != batch.end(); ++it, ++i)
        buf[i] = (*it)->txnid_unordered;

    set<int> mediators;
    for(vector<TxnInfo*>::const_iterator it = batch.begin();
        it != batch.end(); ++it)
        mediators.insert((*it)->source_mediator);

    for(set<int>::const_iterator it = mediators.begin();
        it != mediators.end(); ++it)
        _remote.Send(*it, buf, committed_size * sizeof(int64_t));

    delete [] buf;
}

void SlaveStrategy::RunServer(){
}
}
