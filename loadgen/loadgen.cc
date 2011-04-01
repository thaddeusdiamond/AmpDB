/*  Implementation of A Load Generator for Transactions
 *
 *  Thaddeus Diamond
 *  29 March 2011
 *
 *  Usage: ???
 *
 *  TIMING:  SHOULD THAT GO IN THIS SECTION???
 *  IsolationLevel???? Do we have keys for txntype???
 *
 *  Breakdown -- 45% new-order, 43% transaction, 4% order-status, 
 *                  4% delivery, 4% stock-level
 */
 
#include "loadgen.h"

int Transaction = 1;
int TxnSize;
int NumParts;

GenericTxn *generate_txn(bool valid, bool mp, bool last, Key type) {
    Key txn_buffer[200];
    txn_buffer[0] = txn_buffer_id[1] = Transaction++;   // txnid(_unordered)
    txn_buffer[2] = 0;                                  // source_mediator
    txn_buffer[3] = type;                               // txn_type
    txn_buffer[4] = SERIALIZABLE;                       // isolation_level
    txn_buffer[5] = 0;                                  // rsetsize
    txn_buffer[6] = TxnSize;                            // wsetsize
    txn_buffer[7] = 0;                                  // argcount
    
    index = 8;
    for (int i = 5; i < 8; i++) {                       // Set read set,
        for (int j = 0; j < txn_buffer[i]; j++) {       //      write set, args
            txn_buffer[j + index++] = ;
        }
    }
    
    GenericTxn *t = new GenericTxn(txn_buffer);
    
    if (valid);                                         // Set item to random #
    else;                                               // Set item to -1
        
    if (mp)     t->mp = true;                           // Multi-partition
    else        t->mp = false;                          // Single partition
    
    return t;
}

GenericTxn *generate() {
    srand( time(NULL) );                                // Seed random
    float x = ((float) rand()) / RAND_MAX;              // Pick random #
    
    /*          PROBABILISTIC CHOICE OF TRANSACTION PARAMS      */
    if (x < NO_PER * NO_INV * NO_MP)                    // Invalid, mp N-O
        return generate_txn(false, true, false, NO_ID);
 
   else if (x < NO_PER * NO_INV)                        // Invalid, sp N-O
        return generate_txn(false, false, false, NO_ID);
    
    else if (x < NO_PER * (NO_INV + (1 - NO_INV) * NO_MP))
        return generate_txn(true, true, false, NO_ID);  // Valid, mp N-O
    
    else if (x < NO_PER)
        return generate_txn(true, false, false, NO_ID); // Valid, sp N-O
    
    else if (x < NO_PER + PAY_PER * PAY_MP * PAY_LAST)
        return generate_txn(true, true, true, PAY_ID);  // Cust select, mp PAY
    
    else if (x < NO_PER + PAY_PER * PAY_MP)
        return generate_txn(true, true, false, PAY_ID); // No cust, mp PAY
 
    else if (x < NO_PER + PAY_PER * (PAY_MP + (1 - PAY_MP) * PAY_LAST))
        return generate_txn(true, false, true, PAY_ID); // Cust select, sp PAY
    
    else if (x < NO_PER + PAY_PER)
        return generate_txn(true, false, false, PAY_ID);// No cust, mp PAY
    
    else if (x < NO_PER + PAY_PER + OS_PER * OS_LAST)
        return generate_txn(true, false, true, OS_ID);  // Cust select, sp O-S

    else if (x < NO_PER + PAY_PER + OS_PER)
        return generate_txn(true, false, false, OS_ID); // No cust, sp O-S

    else if (x < NO_PER + PAY_PER + OS_PER + DEL_PER)
        return generate_txn(true, false, false, DEL_ID);// sp DELIVERY

    else
        return generate_txn(true, false, false, SL_ID); // sp STOCK-LEVEL
}




