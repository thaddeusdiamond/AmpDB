/*  New Order Transaction Implementation
 *  
 *  Thaddeus Diamond
 *  30 March 2011
 *
 */

#include "tpc_c.h"                              // tpc_c library

extern int Part;                                // Machine partition #

extern QuickMap<Warehouse>  w_table;
extern QuickMap<District>   d_table;
extern QuickMap<Customer>   c_table;
extern QuickMap<NewOrder>   no_table;
extern QuickMap<Order>      o_table;
extern QuickMap<OrderLine>  ol_table;
extern QuickMap<Stock>      s_table;
extern QuickMap<Item>       i_table;


/* new_order(Keys ...): 
 *  This is the main newOrder function and is the only one accessed directly
 */                    
int new_order(Key *args, Key id) {
    Key w_id, d_id, c_id, ol_w[15], ol_i[15];   // Warehouse, cust, district
    int ol_cnt, systime, ol_q[15];              //      keys, orderline count,
                                                //      systime and orderline
                                                //      arrays
                                                
    // Set warehouse, district, customer IDs; Set orderline count and systime
    w_id = args[0];
    d_id = args[1];
    c_id = args[2];
    ol_cnt = args[3];
    systime = args[4];
    
    for(int j = 0; j < ol_cnt; j++) {           // For each orderline
        ol_w[j] = args[5 + j];                  //      Set the warehouse, item,
        ol_i[j] = args[20 + j];                 //      and quantity
        ol_q[j] = (int) args[35 + j];
    }
    
    /*          CUSTOMER IN OUR WAREHOUSE               */
    if (warehouse_local(w_id))
        return processNewOrder(id, w_id, d_id, c_id, ol_cnt, 
                                   ol_w, ol_i, ol_q, systime);
                               
    /*          CUSTOMER IN ANOTHER WAREHOUSE           */
    for (int i = 0; i < ol_cnt; i++) {
        if (!check_reads(ol_i[i]))
            return TXN_FAILURE;                 // Validate item #
        decrease_supply(ol_w[i], s_table[ol_i[i]], ol_q[i]);
    }
    return true;
}

/* processNewOrder(Keys ...): 
 *  This is for when our warehouse needs to fully process the new order
 */
int processNewOrder(Key txnid, Key w_id, Key d_id, Key c_id, int ol_cnt, 
                     Key *ol_w, Key *ol_i, int *ol_q, int systime) {
    
    Warehouse w = w_table[w_id];                // Proper classes
    District d  = d_table[d_id];
    Customer c  = c_table[c_id];
    
    d.d_next_o_id++;                            // Increment district order cnt
    int o_id = txnid;                           // Use txn id for order id
    
    bool all_local = true;    
    for(int ol_number = 0; ol_number < ol_cnt; ol_number++) {
        if (!check_reads(ol_i[ol_number]))
            return TXN_FAILURE;
        
        OrderLine ol(o_id, d_id, w_id, ol_number, ol_i[ol_number], 
                     ol_q[ol_number]);          // OL Tables
        ol.ol_supply_w_id = ol_w[ol_number];
        if (ol_w[ol_number] != w_id)            // Foreign warehouse
            all_local = false;
        ol.ol_delivery_d = systime;
        
        Item i = i_table[ol.getItem()];         // Read correct item and stock
        Key s_id = ((ol_w[ol_number] << 32) >> 32) * ITEMS + ol.getItem();
        Stock s = s_table[s_id | ((ol_w[ol_number] >> 32) << 32)];
        decrease_supply(ol_w[ol_number], s, ol.ol_quantity);
        
        ol.ol_amount = (ol.ol_quantity * i.i_price)* 
                       (1 + w.w_tax + d.d_tax)* // Set the orderline amt
                       (1 - c.c_discount);      //      correctly (discounts)
        
        ol_table[o_id] = ol;                    // Write OL (stock implicit)
    }
    
    Order o(o_id, d_id, w_id, c_id);            // Add order to database
    o.o_entry_d = systime;
    o.o_carrier_id = -1;
    o.o_ol_cnt = ol_cnt;
    o.o_all_local = all_local;
    o_table[o_id] = o;                          // Write order
    
    NewOrder no(o_id, d_id, w_id);              // Add new order to database
    no_table[o_id] = no;                        // Write new order       

    return TXN_SUCCESS;
}

/* decrease_supply(Stock, Qty): 
 *  We do not have the customer in our warehouse, off site txn
 */
bool decrease_supply(Key w_id, Stock s, int qty) {
    if (warehouse_local(w_id)) {
        if(s.s_quantity > (qty + 10))
            s.s_quantity -= qty;                    // Update stock quantities
        else                              
            s.s_quantity -= qty - 91;
    }
    return true;
}

/* check_reads(Keys ...): 
 * Done before our write phase to ensure we want to go through with txn
 */
bool check_reads(Key i) {
    if (i >= ITEMS || i < 0)
        return false;
    return true;
}
