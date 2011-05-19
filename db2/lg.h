#include <stdlib.h>
#include "../global.h"
#include "../config.h"
#include "../generictxn.h"
#include <assert.h>

#define MP 10
#define CONTENTION 0.5

GenericTxn *generate2(int partitions) {
    assert(partitions > 0);    
    bool mp = rand() % 100 > MP;
    if(partitions < 2) mp = 0;
    
    int64_t part1,part2;
    part1 = part2 = rand() % partitions;
    part2 = 1 - part1;
    if(mp && partitions > 2) {
        p2:
        part2 = rand() % partitions;
        if(part1==part2) goto p2;
    }    
    int64_t dbsize;
    if(CONTENTION == 0.1) dbsize = 100000;
    if(CONTENTION == 0.2) dbsize =  50000;
    if(CONTENTION == 0.5) dbsize =  20000;
    if(CONTENTION == 1)   dbsize =  10000;
    if(CONTENTION == 2)   dbsize =   5000;
    if(CONTENTION == 5)   dbsize =   2000;
    if(CONTENTION == 10)  dbsize =   1000;

    GenericTxn *t = new GenericTxn();
    t->wsetsize = 10;
    t->rsetsize = 0;
    t->argcount = 20;
    for(int i = 0; i < 10; i++) {
        top:
        int64_t item = rand() % dbsize;
        for(int j = 0; j < i; j++)
            if(item == t->wset[j])
                goto top;

        if(mp) item = item | (((i%2) ? part1 : part2) << 48);
        else   item = item | (         part1          << 48);

        t->wset[i] = item;
        t->args[i] = item;
        t->args[10+i] = rand() % 10; // quantity
    }
    
    return t;
}


