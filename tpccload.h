#ifndef _TPCCLOAD_H_
#define _TPCCLOAD_H_

#include "global.h"
#include "generictxn.h"
#include "config.h"

void generatetpcctxn(GenericTxn *t, int ndbs) {
    int x = rand() % 100;
    t->isolationlevel = SERIALIZABLE;
    t->rsetsize = 0;
    t->wset[0] = rand() % ndbs;
    t->argcount = 0;
    for(int i = 1; i < 16; i++)
        t->wset[i] = t->wset[0];
    if(x < 49) { // payment txn
        t->wsetsize = 2;
    } else { // new order txn
	t->wsetsize = 16;
        if(rand() % 100 < 10 && ndbs > 1) { //mp
            do {
                t->wset[1] = rand() % ndbs;
            } while(t->wset[1] == t->wset[0]);
        }
    }
}

#endif // (#ifndef _TPCC_H_)
