#include "scheduler.h"

#include <cstdio>
#include <cmath>
#include <ctime>

#include <algorithm>
#include <functional>
#include <limits>
#include <map>
#include <queue>
#include <vector>
#include <ext/hash_map>
#include <ext/hash_set>

#include "config.h"
#include "txninfo.h"

using std::map;
using std::priority_queue;
using std::vector;
using __gnu_cxx::hash_map;
using __gnu_cxx::hash_set;

namespace Preproc{
// Time-constrained randomized scheduling
class RandomizedScheduler : public Scheduler{
  public:
    RandomizedScheduler(const Configuration& config);

    virtual void Schedule(
            const hash_map<int64_t, hash_set<int64_t> >& conflicts,
            vector<TxnInfo>* txns,
            vector<TxnInfo*>* ordered);

  private:
    int64_t _execute_deadline;  // in nanosecond
};

class DensityScheduler : public Scheduler{
  public:
    DensityScheduler(const Configuration& config)
        : Scheduler(config){}

    virtual void Schedule(
            const hash_map<int64_t, hash_set<int64_t> >& conflicts,
            vector<TxnInfo>* txns,
            vector<TxnInfo*>* ordered);
};

map<const Configuration*, Scheduler*> Scheduler::_instances;

Scheduler* Scheduler::GetInstance(const Configuration& config){
    map<const Configuration*, Scheduler*>::iterator it =
        _instances.find(&config);
    if(it == _instances.end()){
        Scheduler* sched = 0;
        switch(config.scheduler_type){
            case ECHO:
                sched = new Scheduler(config);
                break;
            case RANDOM:
                sched = new RandomizedScheduler(config);
                break;
            case DENSITY:
                sched = new DensityScheduler(config);
                break;
            default:
                return 0;
        }
        it = _instances.insert(std::make_pair(
                    &config, sched)).first;
    }
    return it->second;
}

Scheduler::Scheduler(const Configuration& config)
    : _config(config), _logic_clock(0), _latest_masked_time(-1){
    // Count the bits to prepare the bit masks
    uint64_t t;
    int txn_bits = 1;
    t = 1;
    while(_config.batch_size > t){
        ++txn_bits;
        t <<= 1;
    }
    int instance_bits = 1;
    t = 1;
    while(static_cast<unsigned int>(_config.instances) > t){
        ++instance_bits;
        t <<= 1;
    }
    int partition_bits = 1;
    t = 1;
    while(_config.preprocessorpartitions.size() > t){
        ++partition_bits;
        t <<= 1;
    }

    const PreprocessorNode* node =
        reinterpret_cast<const PreprocessorNode*>(
            _config.allnodes.find(_config.myNodeID)->second);

    _sn_time_shift = txn_bits + instance_bits + partition_bits;
    _sn_time_mask = ~((1ULL << _sn_time_shift) - 1);
    _sn_instance_val =
        ((static_cast<unsigned int>(node->partitionID) << instance_bits) |
         static_cast<unsigned int>(_config.instance_number)) << txn_bits;
    _sn_serial_mask = (1ULL << txn_bits) - 1;
}

Scheduler::~Scheduler(){}

void Scheduler::Schedule(
        const hash_map<int64_t, hash_set<int64_t> >& conflicts,
        vector<TxnInfo>* txns,
        vector<TxnInfo*>* ordered){
    int64_t sn = GetSerialNumber(txns->size());
    for(vector<TxnInfo>::iterator it = txns->begin();
        it != txns->end(); ++it){
        it->txnid = sn++;
        ordered->push_back(&*it);
    }
}

typedef priority_queue<double, vector<double>, std::greater<double> >
    thread_head_t;

double Scheduler::ExpectedDuration(
        const Configuration& config,
        const hash_map<int64_t, hash_set<int64_t> >& conflicts,
        const vector<TxnInfo*>& txns){
    const vector<DBNode*>& replica0 = config.dbreplicas.begin()->second;

    vector<thread_head_t> partition_thread_head(replica0.size());
    for(vector<DBNode*>::const_iterator it = replica0.begin();
        it != replica0.end(); ++it){
        const int partitionID = (*it)->partitionID;
        const int cores = (*it)->cores;
        thread_head_t& thread_head = partition_thread_head[partitionID];
        for(int j = 0; j < cores; ++j)
            thread_head.push(0.0);
    }

    hash_map<int64_t, double> ending_time;  // if not assigned, 0.0 works
    for(vector<TxnInfo*>::const_iterator it = txns.begin();
        it != txns.end(); ++it){
        const TxnInfo& txn = **it;
        double machine_latest_head = 0.0;
        for(vector<int>::const_iterator jt = txn._slices.begin();
            jt != txn._slices.end(); ++jt){
            const double this_machine_head = partition_thread_head[*jt].top();
            if(this_machine_head > machine_latest_head)
                machine_latest_head = this_machine_head;
        }

        double conflict_latest_ending = 0.0;
        const hash_set<int64_t>& conflict = conflicts.find(txn.txnid_unordered)->second;
        for(hash_set<int64_t>::const_iterator jt = conflict.begin();
            jt != conflict.end(); ++jt){
            const double this_txn_ending = ending_time[*jt];
            if(this_txn_ending > conflict_latest_ending)
                conflict_latest_ending = this_txn_ending;
        }

        const double my_ending_time =
            (machine_latest_head > conflict_latest_ending ?
             machine_latest_head : conflict_latest_ending) +
            txn.ExpectedDuration();
        ending_time[txn.txnid_unordered] = my_ending_time;

        for(vector<int>::const_iterator jt = txn._slices.begin();
            jt != txn._slices.end(); ++jt){
            thread_head_t& thread_head = partition_thread_head[*jt];
            thread_head.pop();
            thread_head.push(my_ending_time);
        }
    }

    double finish_time = 0.0;
    for(vector<DBNode*>::const_iterator it = replica0.begin();
        it != replica0.end(); ++it){
        const int partitionID = (*it)->partitionID;
        thread_head_t& thread_head = partition_thread_head[partitionID];
        double t;
        do{
            t = thread_head.top();
            thread_head.pop();
        }while(!thread_head.empty());
        if(t > finish_time)
            finish_time = t;
    }
    return finish_time;
}

int64_t Scheduler::GetSerialNumber(int to_use){
    uint64_t t = Tick();
    uint64_t masked_time = (t << _sn_time_shift) & _sn_time_mask;
    uint64_t sn = (masked_time == _latest_masked_time) ?
        _latest_sn :
        masked_time | _sn_instance_val;
    _latest_masked_time = masked_time;
    _latest_sn = sn + to_use;
    return static_cast<int64_t>(sn);
}

RandomizedScheduler::RandomizedScheduler(const Configuration& config)
    : Scheduler(config){
    // wait_time is in microsecond, _execute_deadline is in nanosecond
    _execute_deadline = config.wait_time * 1000 / 2;
}

void RandomizedScheduler::Schedule(
        const hash_map<int64_t, hash_set<int64_t> >& conflicts,
        vector<TxnInfo>* txns,
        vector<TxnInfo*>* ordered){
    vector<TxnInfo>& txns_ = *txns;
    timespec deadline_time, cur_time;
    clock_gettime(CLOCK_MONOTONIC, &deadline_time);
    deadline_time.tv_nsec += _execute_deadline;
    if(deadline_time.tv_nsec > 1000000000){
        deadline_time.tv_sec += deadline_time.tv_nsec / 1000000000;
        deadline_time.tv_nsec %= 1000000000;
    }

    int n = txns_.size();
    double best_duration = std::numeric_limits<double>::max();
    vector<TxnInfo*> assignment;
    vector<double> durations;

    for(int i = 0; i < n; ++i)
        assignment.push_back(&txns_[i]);
    int trial_per_clock = n > 3000 ? 1 : 3000 / n;
    do{
        for(int i = 0; i < trial_per_clock; ++i){
            std::random_shuffle(assignment.begin(), assignment.end());
            double duration = ExpectedDuration(_config, conflicts, assignment);
            durations.push_back(duration);
            if(duration < best_duration){
                best_duration = duration;
                *ordered = assignment;
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &cur_time);
    }while(cur_time.tv_sec < deadline_time.tv_sec ||
           (cur_time.tv_sec == deadline_time.tv_sec &&
            cur_time.tv_nsec < deadline_time.tv_nsec));

    int64_t sn_start = GetSerialNumber(n);
    for(int i = 0; i < n; ++i)
        (*ordered)[i]->txnid = sn_start + i;

#if VERBOSE
    double sum = 0.0;
    double sqr_sum = 0.0;
    for(vector<double>::const_iterator it = durations.begin();
        it != durations.end(); ++it){
        sum += *it;
        sqr_sum += *it * *it;
    }
    int count = durations.size();
    sum /= count;
    sqr_sum /= count;  // now they are averages
    double std_dev = sqrt(sqr_sum - sum * sum);
#endif  // VERBOSE
}

void DensityScheduler::Schedule(
        const hash_map<int64_t, hash_set<int64_t> >& conflicts,
        vector<TxnInfo>* txns,
        vector<TxnInfo*>* ordered){
    hash_map<int64_t, double> t;
    hash_map<int64_t, int> degree;
    for(vector<TxnInfo>::const_iterator it = txns->begin();
        it != txns->end(); ++it){
        const int64_t txnid = it->txnid_unordered;
        t[txnid] = 0.0;
        degree[txnid] = conflicts.find(txnid)->second.size();
    }

    int64_t sn = GetSerialNumber(txns->size());
    ordered->clear();

    double time = 0.0;
    while(t.size() > 0){
        vector<TxnInfo*> no_conflict_txns;
        double min_t = std::numeric_limits<double>::max();
        int max_degree = -1;
        TxnInfo* next_txn = 0;
        bool has_earlier_than_time = false;
        for(vector<TxnInfo>::iterator it = txns->begin();
            it != txns->end(); ++it){
            hash_map<int64_t, double>::iterator jt =
                t.find(it->txnid_unordered);
            if(jt != t.end()){
                if(jt->second < time){
                    has_earlier_than_time = true;
                    min_t = jt->second;

                    int d = degree[it->txnid_unordered];
                    if(d > max_degree){
                        max_degree = d;
                        next_txn = &*it;
                    }
                }else if(jt->second < min_t){
                    min_t = jt->second;
                    next_txn = &*it;
                }
            }
        }

        const hash_set<int64_t>& neighbors =
            conflicts.find(next_txn->txnid_unordered)->second;
        for(hash_set<int64_t>::const_iterator it = neighbors.begin();
            it != neighbors.end(); ++it){
            hash_map<int64_t, double>::iterator jt = t.find(*it);
            if(jt != t.end() && jt->second < time){
                jt->second = time + next_txn->ExpectedDuration();
                --degree[*it];
            }
        }
        time += next_txn->ExpectedDuration() /
            _config.dbreplicas.find(0)->second[next_txn->_slices[0]]->cores;

        next_txn->txnid = sn++;
        t.erase(next_txn->txnid_unordered);
        ordered->push_back(next_txn);
    }
}
}
