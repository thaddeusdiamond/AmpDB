/*  Loadgen Library
 *
 *  Thaddeus Diamond
 *  29 March 2011
 *
 */
 
#ifndef _LOADGEN_H_                 // Avoid circular dependencies


#define _LOADGEN_H_

#include <stdlib.h>
#include "../db/tpc_c.h"

/* <---------------------- BEGIN TPC GLOBAL -----------------------> */

/////    TRANSACTION PERCENTAGES    /////
#define NO_PER   0.57//0.45
#define NO_INV   0.01
#define NO_MP    0.01
#define PAY_PER  0.43
#define PAY_MP   0.15
#define PAY_LAST 0.6
#define OS_PER   0//0.04
#define OS_LAST  0.6
#define DEL_PER 0//0.04
#define SL_PER  0//0.04

/* <----------------------- END TPC GLOBAL -----------------------> */


/* <---------------------- BEGIN FUNC DECS -----------------------> */

GenericTxn *generate(int id);
GenericTxn *generate_txn(bool valid, bool mp, bool last, Key type);

/* <----------------------- END FUNC DECS ------------------------> */

#endif                              // (#ifndef _LOADGEN_H_)
