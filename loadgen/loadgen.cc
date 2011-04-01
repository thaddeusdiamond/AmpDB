/*  Implementation of A Load Generator for Transactions
 *
 *  Thaddeus Diamond
 *  29 March 2011
 *
 *  Usage: NONE [Link Object]
 *
 *
 *  Breakdown -- 45% new-order, 43% transaction, 4% order-status, 
 *                  4% delivery, 4% stock-level
 */

#include "loadgen.h"

int Transaction;
int NumWarehouses;

GenericTxn *generate_txn(bool valid, bool mp, bool last, Key type) {
    srand( time(NULL) );                                // Seed the rand()
    
    int ol_cnt;                                         // Orderline count
    ol_cnt = 5 + rand() % 10;                           //      5 - 15
    
    Key txn_buffer[200];
    txn_buffer[0] = txn_buffer[1] = Transaction;        // txnid(_unordered)
    txn_buffer[2] = 0;                                  // source_mediator
    txn_buffer[3] = type;                               // txn_type
    txn_buffer[4] = SERIALIZABLE;                       // isolation_level
    txn_buffer[5] = 2 + (mp*ol_cnt) + (valid*ol_cnt);   // rsetsize
    txn_buffer[6] = 1 + (valid*ol_cnt);                 // wsetsize
    txn_buffer[7] = 35 + ol_cnt;                        // argcount
    
    switch (type) {
        
        /*              NEW ORDER TXN                               */
        case NO_ID:
            int wset_index, rset_index, argc_index;     // wset, rset, argc
            rset_index = 8;
            wset_index = rset_index + txn_buffer[5]; 
            argc_index = wset_index + txn_buffer[6];
            
            int w, ware, d, c, i;                       // Randomized warehouse,
            w = rand() % NumWarehouses;                 //      district, cust,
            d = rand() % DPERW + w * DPERW;             //      id's and corr.
            c = rand() % CPERD + d * CPERD;             //      partition
            
            /*                      Set key bits                    */
            Key w_id, d_id, c_id, i_id, part;
            part = (w / MAXW);
            w_id = w | (part << 48) | (W_TABLE_ID << 32);
            d_id = d | (part << 48) | (D_TABLE_ID << 32);
            c_id = c | (part << 48) | (C_TABLE_ID << 32);

            cout << w_id << '\t' << d_id << '\t' << c_id << endl;
            

            /*                   Set read/write sets                */
            txn_buffer[argc_index++] = txn_buffer[rset_index++] = w_id;
            txn_buffer[argc_index++] = txn_buffer[wset_index++] = d_id;
            txn_buffer[argc_index++] = txn_buffer[rset_index++] = c_id;

            txn_buffer[argc_index++] = ol_cnt;
            txn_buffer[argc_index++] = time(NULL);
            
            /*                  Set orderlines                      */
            for(int j = 0; j < ol_cnt; j++) {
                if (mp) {                               //      MULTIPARTITION
                    ware = rand() % NumWarehouses;
                    while (!j && ware / MAXW == part)             //          Insurance
                        ware = rand() % NumWarehouses;
                    
                    w = ware;
                    part = w / MAXW;
                    w_id = w | (part << 48) | (W_TABLE_ID << 32);
                    
                    txn_buffer[argc_index] = w_id;                  
                    txn_buffer[rset_index++] = w_id;
                } else                                  //      SINGLE PARTITION
                    txn_buffer[argc_index] = w_id;
                
                if (valid) {                            //      VALID ITEM
                    i = rand() % ITEMS;
                    i_id = i | (part << 48) | (I_TABLE_ID << 32);
                    
                    txn_buffer[argc_index + 15] = i_id;
                    txn_buffer[rset_index++] = i_id;
                    txn_buffer[wset_index++] = (w * ITEMS + i) | (part << 48) | 
                                                    (S_TABLE_ID << 32);
                } else                                  //      INVALID ITEM
                    txn_buffer[argc_index + 15] = -1;
                                                        //      RANDOM QUANTITY
                txn_buffer[argc_index++ + 30] = (rand() % 10) + 1;
            }
            break;
    
    }
    
    GenericTxn *t = new GenericTxn(txn_buffer);
    
    if (mp)     t->mp = true;                           // Multi-partition
    else        t->mp = false;                          // Single partition
    
    return t;
}

GenericTxn *generate(int id, int w_ids) {
    srand( time(NULL) );                                // Seed random
    double x = RANF();                                  // Pick random #
    
    Transaction = id;
    NumWarehouses = w_ids;
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

/*int main() {
    ////////////////////////// DEBUG ////////////////////////////
    GenericTxn *t;
    t = generate(1, 2);
    
    t->toString();

    delete t;
} */
