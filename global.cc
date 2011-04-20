#include "global.h"
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

double tim() { struct timeval tv; gettimeofday(&tv, NULL); return tv.tv_sec + tv.tv_usec/1e6; }
void spin(double dur) {
    double start = tim();
    while(tim() < start + dur)
        ;
}

double gaussian(double s) {
    double x1, x2, w, z;
    do {
        x1 = 2.0 * RANF() - 1.0;
        x2 = 2.0 * RANF() - 1.0;
        w = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );
    return s * x1 * sqrt( (-2.0 * log( w ) ) / w );
    z = 0.5 + s * x1 * sqrt( (-2.0 * log( w ) ) / w );
    if (z < 0 || z >= 1) return RANF();
    else return z;
}

