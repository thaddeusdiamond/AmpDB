/*  Payment Transaction Implementation
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
extern QuickMap<History>    h_table;

extern QuickMap< DBIndex<Key> >   c_last_index;

/* payment(Keys ...): 
 *  This is the main payment function and is the only one accessed directly
 */ 
Key payment(Key *args, Key prev_c_id) {
    Key w_id, d_id, c_id;                       // Warehouse, district keys
    int h_amount, h_date, i, j;
    bool last;
    
    w_id = args[0];                             //  Get keys from argument
    d_id = args[1];    
    h_amount = args[2];
    h_date = args[3];
    last = args[4];
        
    /*                 Warehouse Processed Through Was Local            */
    if (warehouse_local(w_id)) {  
        Warehouse w = w_table[w_id];            // Perform reads from the
        w.w_ytd += h_amount;                    //      databases, then write
        District  d = d_table[d_id];
        d.d_ytd += h_amount;
    }
    
    /*                  Secondary Key-ing Support                       */
    if (last) {                             // LAST NAME SECONDARY LOOKUP
        c_id = c_last_index[args[5]].key;
        if (c_id != prev_c_id)
            return c_id;                    // SEND BACK TO PREPROCESSOR
    } else                                  // PRIMARY KEY LOOKUP
        c_id = args[5];
    
    /*                  Customer is Local To Partition                  */
    if (Part == (c_id >> 48)) {
        Customer  c = c_table[c_id];            // Retrieve customer
        c.c_balance -= h_amount;                //      and write update info
        c.c_ytd_payment += h_amount;
        c.c_payment_cnt++;
        if (!strcmp(c.c_credit, "BC"))          // Overwrite cdata when bad cred
            sprintf(c.c_data, "%s%s%s%s%s%s%s", c_id, c.getDistrict(),
                    c.getWarehouse(), d_id, w_id, h_amount, c.c_data);
        
        History h(c_id, c.getDistrict(), c.getWarehouse(), d_id, w_id);
        //sprintf(h.h_data, "%s    %s", w.w_name, d.d_name);
        h.h_date = h_date;
        
        char h_id[129]; sprintf(h_id, "%lld%d", h.getCustomer(), h_date);
        //h_table[h_id] = h;                      // Insert new history row
    }
    
    return true;
}
