#ifndef _YALE_SCALE_GENERICTXN_H_
#define _YALE_SCALE_GENERICTXN_H_ 1

#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

enum IsolationLevel{
    SERIALIZABLE = 0,
    SNAPSHOT,
    READ_COMMITTED,
    READ_UNCOMMITTED,
};

enum TransactionType{
    NO_ID = 0,
    PAY_ID,
    OS_ID,
    DEL_ID,
    SL_ID,
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
        txntype  = (TransactionType) s[3];
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
        s[3] = (int64_t) txntype;
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
    
    void toString() {
        cout << "TXNID: " << txnid << endl;
        cout << "\ttxnid_unordered\t" << txnid_unordered << endl;
        cout << "\tsource_mediator\t" << source_mediator << endl;
        cout << "\ttxntype\t\t" << txntype << endl;
        cout << "\tisolationlevel\t" << isolationlevel << endl;
        cout << "\tmultipartition\t" << mp << endl;
        
        cout << "\trsetsize\t\t" << rsetsize << endl;
        for (int i = 0; i < rsetsize; i++)
            cout << "\t\trset #" << i << ": " << rset[i] << endl;
        cout << "\twsetsize\t\t" << wsetsize << endl;
        for (int i = 0; i < wsetsize; i++)
            cout << "\t\twset #" << i << ": " << wset[i] << endl;
        cout << "\targcount\t\t" << argcount << endl;
        cout << "\t\tw_id: " << args[0] << endl;
        cout << "\t\td_id: " << args[1] << endl;
        cout << "\t\tc_id: " << args[2] << endl;
        cout << "\t\tol_cnt: " << args[3] << endl;
        cout << "\t\tsystime: " << args[4] << endl;
        for (int i = 0; i < (argcount - 5) / 3; i++) {
            cout << "\t\tware #" << i << ": " << args[i + 5] << endl;
            cout << "\t\titem #" << i << ": " << args[i + 20] << endl;
            cout << "\t\tqty #" << i << ": " << args[i + 35] << endl;
        }
    }
   
    int64_t txnid;
    int64_t txnid_unordered;
    int64_t source_mediator;
    TransactionType txntype;
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
