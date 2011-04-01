#ifndef _GLOBAL_H_
#define _GLOBAL_H_

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


#endif // (#ifndef _GLOBAL_H_)
