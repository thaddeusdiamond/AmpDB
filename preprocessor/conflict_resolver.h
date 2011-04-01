#ifndef _YALE_SCALE_PREPROCESSOR_CONFLICT_RESOLVER_H_
#define _YALE_SCALE_PREPROCESSOR_CONFLICT_RESOLVER_H_ 1

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

class ConflictResolver{
  public:
    static ConflictResolver* GetInstance(const Configuration& config);

    // Fill in the server slices each txn beint64_ts to, and construct
    // the undirected conflict graph stored in conflicts.
    void FindConflicts(
            vector<TxnInfo>* txns,
            hash_map<int64_t, hash_set<int64_t> >* conflicts);

  private:
    ConflictResolver(const Configuration& config) : _config(config){}
    ConflictResolver(const ConflictResolver&);
    ConflictResolver& operator=(const ConflictResolver&);

    static map<const Configuration*, ConflictResolver*> _instances;

    const Configuration& _config;
};
}
#endif  // !defined(_YALE_SCALE_PREPROCESSOR_CONFLICT_RESOLVER_H_)
