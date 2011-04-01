#include "conflict_resolver.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <vector>
#include <ext/hash_map>
#include <ext/hash_set>

#include "txninfo.h"
#include "config.h"

using std::make_pair;
using std::map;
using std::set;
using std::pair;
using std::vector;
using __gnu_cxx::hash_map;
using __gnu_cxx::hash_multimap;
using __gnu_cxx::hash_set;

namespace Preproc{
struct Access{
    char type;  // 'r' or 'w'
    int64_t txnid;

    Access(char type_, int64_t txnid_) : type(type_), txnid(txnid_){}
};
typedef hash_multimap<int64_t, Access> AccessMap;

map<const Configuration*, ConflictResolver*> ConflictResolver::_instances;

ConflictResolver* ConflictResolver::GetInstance(const Configuration& config){
    map<const Configuration*, ConflictResolver*>::iterator it =
        _instances.find(&config);
    if(it == _instances.end())
        it = _instances.insert(make_pair(
                    &config, new ConflictResolver(config))).first;
    return it->second;
}

void ConflictResolver::FindConflicts(
        vector<TxnInfo>* txns,
        hash_map<int64_t, hash_set<int64_t> >* conflicts){
    hash_map<int64_t, hash_set<int64_t> >& conflicts_ = *conflicts;
    conflicts_.clear();

    const bool skipFind = (_config.scheduler_type == ECHO);

    AccessMap access;
    for(vector<TxnInfo>::iterator it = txns->begin();
        it != txns->end(); ++it){
        const int64_t txnid = it->txnid_unordered;
        set<int> slices;
        hash_set<int64_t>& conflict_with = conflicts_[txnid];

        for(int i = 0; i < it->rsetsize; ++i){
            int slice = _config.partitionContainingRecord(it->rset[i]);
            if(slice >= 0)
                slices.insert(slice);
            if(skipFind)
                continue;

            pair<AccessMap::const_iterator, AccessMap::const_iterator> p =
                access.equal_range(it->rset[i]);
            for(AccessMap::const_iterator jt = p.first; jt != p.second; ++jt)
                if(jt->second.type == 'w'){
                    conflict_with.insert(jt->second.txnid);
                    conflicts_[jt->second.txnid].insert(txnid);
                }
            access.insert(make_pair(it->rset[i], Access('r', txnid)));
        }
        for(int i = 0; i < it->wsetsize; ++i){
            int slice = _config.partitionContainingRecord(it->wset[i]);
            if(slice >= 0)
                slices.insert(slice);
            if(skipFind)
                continue;

            pair<AccessMap::const_iterator, AccessMap::const_iterator> p =
                access.equal_range(it->wset[i]);
            for(AccessMap::const_iterator jt = p.first; jt != p.second; ++jt)
                if(jt->second.txnid != txnid){
                    conflict_with.insert(jt->second.txnid);
                    conflicts_[jt->second.txnid].insert(txnid);
                }
            access.insert(make_pair(it->wset[i], Access('w', txnid)));
        }

        if(skipFind)
            conflict_with.erase(txnid);
        std::copy(slices.begin(), slices.end(),
                  std::back_inserter(it->_slices));
    }
}
}
