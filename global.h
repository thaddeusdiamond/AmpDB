#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define WRITE   true
#define READ    false

#define DBSIZE      1000000
#define STACK_SIZE  16384
#define MAX_THREADS 32
#define TIMEOUT_THRESHOLD 1e-3

#define DETERM  true

#define RANF() ((double)rand()/(1.0+(double)RAND_MAX))
double gaussian(double s);

double tm();
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

enum TxnType {
    MICROTXN = 0,
    DEBUGTXN,
    NEWORDER,
    PAYMENT,
    ORDERSTATUS,
    DELIVERY,
    STOCKLEVEL,
    CUSTOMERLOOKUP,
};

#define MICROSIZE 100

#endif // (#ifndef _GLOBAL_H_)
