#ifndef _YALE_SCALE_GENERICTXN_H_
#define _YALE_SCALE_GENERICTXN_H_ 1

#include <cstdlib>
#include <cstring>

enum IsolationLevel{
    SERIALIZABLE = 0,
    SNAPSHOT,
    READ_COMMITTED,
    READ_UNCOMMITTED,
};

class GenericTxn {
public:
    static const int FIXED_HEADER_SIZE = 8;  // unit: int64_t
    static const int RSETSIZE_IDX = 5;
    static const int WSETSIZE_IDX = 6;
    static const int ARGCOUNT_IDX = 7;

    GenericTxn(){}
    GenericTxn(void *buf) {
        deserialize(buf);
    }

    void deserialize(void *buf){
        int64_t *s = (int64_t *)buf;
        txnid    = s[0];
        txnid_unordered = s[1];
        source_mediator = s[2];
        txntype  = s[3];
        isolationlevel = (IsolationLevel) s[4];
        rsetsize = s[5];
        wsetsize = s[6];
        argcount = s[7];

        int index = 8;
        memset(rset, 0, 64*sizeof(int64_t));
        memset(wset, 0, 64*sizeof(int64_t));
        memset(args, 0, 64*sizeof(int64_t));
        for(int i = 0; i < rsetsize; i++) rset[i] = s[index++];
        for(int i = 0; i < wsetsize; i++) wset[i] = s[index++];
        for(int i = 0; i < argcount; i++) args[i] = s[index++];
    }

    void *serialize_to(void* s_, int64_t* length) const{
        *length = (FIXED_HEADER_SIZE + rsetsize + wsetsize + argcount) *
                   sizeof(int64_t);
        int64_t* s = reinterpret_cast<int64_t*>(s_);
        s[0] = txnid;
        s[1] = txnid_unordered;
        s[2] = source_mediator;
        s[3] = txntype;
        s[4] = (int64_t) isolationlevel;
        s[5] = rsetsize;
        s[6] = wsetsize;
        s[7] = argcount;

        int index = 8;
        for(int i = 0; i < rsetsize; i++) s[index++] = rset[i];
        for(int i = 0; i < wsetsize; i++) s[index++] = wset[i];
        for(int i = 0; i < argcount; i++) s[index++] = args[i];
        return (void *)s;
    }
   
    inline void *serialize(int64_t* length) const{
        *length = (FIXED_HEADER_SIZE + rsetsize + wsetsize + argcount) *
                  sizeof(int64_t);
        return serialize_to(malloc(*length), length);
    }
   
    int64_t txnid;
    int64_t txnid_unordered;
    int64_t source_mediator;
    int64_t txntype;
    IsolationLevel isolationlevel;
    bool mp;
    int64_t rsetsize;
    int64_t wsetsize;
    int64_t argcount;
    int64_t rset[64];
    int64_t wset[64];
    int64_t args[64];
};

#endif  // !define(_YALE_SCALE_GENERICTXN_H_)
