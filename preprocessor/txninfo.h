#ifndef _YALE_SCALE_PREPROCESSOR_TXNINFO_H_
#define _YALE_SCALE_PREPROCESSOR_TXNINFO_H_ 1

#include "../generictxn.h"

#include <vector>

using std::vector;

namespace Preproc{
class TxnInfo : public ::GenericTxn{
  public:
    TxnInfo(){}
    explicit TxnInfo(void *buf) : GenericTxn(buf){}

    // This is non-explicit to make STL happy
    TxnInfo(const ::GenericTxn& txn){
        *static_cast< ::GenericTxn*>(this) = txn;
    }

    double ExpectedDuration() const{
	return _slices.size() > 1 ? 10.0 : 1.0;
    }
    vector<int> _slices;
};
}

#endif  // !defined(_YALE_SCALE_PREPROCESSOR_TXNINFO_H_)
