#include <iostream>
#include "lg.h"

int main(int argc, char **argv) {
    int conflicts = 0;
    for(int i = 0; i < 1000000; i++) {
        GenericTxn *a = generate2(0);
        GenericTxn *b = generate2(0);
        for(int j = 0; j < 10; j++) {
            for(int k = 0; k < 10; k++) {
                if(a->wset[j] == b->wset[k]) {
                    conflicts++;
                    goto done;
                }
            }
        }
        done:;
    }
    cout << (double)conflicts / 10000 << "%" << endl << flush;
}
