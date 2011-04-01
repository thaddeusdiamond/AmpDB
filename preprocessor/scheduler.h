#ifndef _YALE_SCALE_PREPROCESSOR_SCHEDULER_H_
#define _YALE_SCALE_PREPROCESSOR_SCHEDULER_H_ 1

#include <stdint.h>

#include <map>
#include <vector>
#include <ext/hash_map>
#include <ext/hash_set>

using std::map;
using std::vector;
using __gnu_cxx::hash_map;
using __gnu_cxx::hash_set;

namespace Preproc{
class Configuration;
class TxnInfo;

class Scheduler{
  public:
    static Scheduler* GetInstance(const Configuration& config);

    virtual void Schedule(
            const hash_map<int64_t, hash_set<int64_t> >& conflicts,
            vector<TxnInfo>* txns,
            vector<TxnInfo*>* ordered);

    static double ExpectedDuration(
            const Configuration& config,
            const hash_map<int64_t, hash_set<int64_t> >& conflicts,
            const vector<TxnInfo*>& txns);

  protected:
    explicit Scheduler(const Configuration& config);
    virtual ~Scheduler();

    int64_t GetSerialNumber(int to_use);

    const Configuration& _config;

  private:
    Scheduler(const Scheduler&);
    Scheduler& operator=(const Scheduler&);

    static map<const Configuration*, Scheduler*> _instances;

    uint64_t _sn_time_mask;
    uint64_t _sn_instance_val;
    uint64_t _sn_serial_mask;

    int64_t _latest_masked_time;
    int64_t _latest_sn;
};
}
#endif  // !defined(_YALE_SCALE_PREPROCESSOR_SCHEDULER_H_)
