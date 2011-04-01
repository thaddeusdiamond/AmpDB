/*  Loadgen Library
 *
 *  Thaddeus Diamond
 *  29 March 2011
 *
 */
 
#ifndef _LOADGEN_H_                 // Avoid circular dependencies


#define _LOADGEN_H_

 
#include "../generictxn.h"          // Std Library Headers
#include <map>                  
#include <string>
#include <cstring>
#include <stdint.h>
#include <iostream>
#include <cstdlib>
 
/* <---------------------- BEGIN TPC GLOBAL -----------------------> */

/////    TRANSACTION TYPE KEYS      /////
#define NO_ID  0
#define PAY_ID 1
#define OS_ID  2
#define DEL_ID 3
#define SL_ID  4

/////    TRANSACTION PERCENTAGES    /////
#define NO_PER   0.45
#define NO_INV   0.01
#define NO_MP    0.01
#define PAY_PER  0.43
#define PAY_MP   0.15
#define PAY_LAST 0.6
#define OS_PER   0.04
#define OS_LAST  0.6
#define DEL_PER 0.04
#define SL_PER  0.04

/* <----------------------- END TPC GLOBAL -----------------------> */


/* <---------------------- BEGIN FUNC DECS -----------------------> */

GenericTxn *generate();
GenericTxn *generate_txn(bool valid, bool mp, bool last, 
                         Key type)

/* <----------------------- END FUNC DECS ------------------------> */

#endif                              // (#ifndef _LOADGEN_H_)
