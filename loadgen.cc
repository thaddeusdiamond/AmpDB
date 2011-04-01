#include "generictxn.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <assert.h>

using namespace std;

int main(int argc, char *argv[]) {
    if(argc < 7){
        printf("Usage: %s partsize partcount txnsize txncount percentmp outfile\n", argv[0]);
        return -1;
    }
    
    int partsize = atoi(argv[1]);
    int partcount = atoi(argv[2]); 
    int txnsize = atoi(argv[3]);
    int txncount = atoi(argv[4]);
    int percentmp = atoi(argv[5]);
    FILE *f = fopen(argv[6],"w");
    
    
    int mp, p, p2;
    
    GenericTxn t;
    for(int i = 1; i <= txncount; i++) {
        
        t.txnid_unordered = t.txnid = i;
        t.source_mediator = 0;
        t.isolationlevel = SERIALIZABLE;
        t.txntype = 0;
        t.argcount = 0;
        t.rsetsize = 0;
        t.wsetsize = txnsize;
        
        mp = rand() % 100 < percentmp;
        p = rand() % partcount;
        do { p2 = rand() % partcount; } while (p2 == p);
        
        cout << i << " (";
        if(mp) cout << "m";
        else   cout << p;
        cout << "): ";
        
        for(int j = 0; j < txnsize; j++) {
            dengo:
            t.wset[j] = (rand() % partsize) * partcount + ((mp && j%2)?p2:p);
            for(int k = 0; k < j; k++)
                if(t.wset[j] == t.wset[k])
                    goto dengo;
            cout << t.wset[j] << " ";
        }
        cout << "\n";
        
        int64_t length;
        void *s = t.serialize(&length);
        fwrite(s, length, 1, f);
    }
    
    
    
    return 0;
}

