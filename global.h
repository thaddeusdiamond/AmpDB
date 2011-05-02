#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define RANF() ((double)rand()/(1.0+(double)RAND_MAX))
double gaussian(double s);

double tim();
void spin(double dur);

enum TxnStatus {
    NEW = 0,
    ACTIVE,
    READY,
    NETWORK,
    BLOCKED,
    ABORTED,
    COMMITTED,
};

/* <---------------------- BEGIN TPC GLOBAL ----------------------> */

/////   MAXIMUM LIMITS    /////
#define MAXW  2
#define DPERW 10
#define MAXD  MAXW * DPERW
#define CPERD 3000
#define MAXC  MAXD * CPERD
#define NOPERW 9000
#define MAXNO MAXW * NOPERW
#define OPERW 30000
#define MAXO  MAXW * OPERW
#define OLPERW 300000
#define MAXOL MAXW * OLPERW
#define ITEMS 100000
#define MAXS  MAXW * ITEMS
#define HPERW 30000
#define MAXH MAXW * HPERW
#define HISTORY 1000000


/////   TABLE LISTINGS [UNIQUE IDENTIFIERS]    /////
#define  W_TABLE_ID   (Key) 0
#define  D_TABLE_ID   (Key) 1
#define  C_TABLE_ID   (Key) 2
#define  NO_TABLE_ID  (Key) 3
#define  O_TABLE_ID   (Key) 4
#define  OL_TABLE_ID  (Key) 5
#define  S_TABLE_ID   (Key) 6
#define  I_TABLE_ID   (Key) 7
#define  H_TABLE_ID   (Key) 8


/////   DEFINE TRANSACTION TYPES    /////
#define NO_TXN 0
#define NOR_TXN 1

/////   DEFINE TRANSACTION CODES    /////
#define TXN_SUCCESS 0
#define TXN_FAILURE 1
#define TXN_REDO 2

/* <----------------------- END TPC GLOBAL -----------------------> */


#endif // (#ifndef _GLOBAL_H_)
