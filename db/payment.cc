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

extern QuickMap< DBIndex<char *> >   c_last_index;

/* payment(Keys ...): 
 *  This is the main payment function and is the only one accessed directly
 */ 
int payment(Key *args, Key id) {
    Key w_id, d_id, c_id;                       // Warehouse, district keys
    int h_amount, h_date, i, j;
    bool last;
    
    (w_id = args[0]) && (d_id = args[1]) &&     //  Get keys from argument
        (h_amount = args[2]) && (h_date = args[3]) &&
        (last = args[4]);
    
    /*                  NEED TO DO SEND BACK TO MEDIATOR!!!!            */
    /*                  SEPARATE WAREHOUSE LOCALITY AND CUSTOMER PART   */
    if (last)                                   // LAST NAME SECONDARY LOOKUP
        c_id = c_last_index[args[5]].key;
    else                                        // PRIMARY KEY LOOKUP
        c_id = args[5];
    
    Customer  c = c_table[c_id];                // Retrieve customer
    c.c_balance -= h_amount;                    //      and write update info
    c.c_ytd_payment += h_amount;
    c.c_payment_cnt++;
    
    if (!strcmp(c.c_credit, "BC"))              // Overwrite cdata when bad cred
        sprintf(c.c_data, "%s%s%s%s%s%s%s", c_id, c.getDistrict(),
                c.getWarehouse(), d_id, w_id, h_amount, c.c_data);
    
    //if (warehouse_local(c.getWarehouse())) {   
        Warehouse w = w_table[w_id];            // Perform reads from the
        w.w_ytd += h_amount;                    //      databases, then write
        District  d = d_table[d_id];
        d.d_ytd += h_amount;
    //}
    
    History h(c_id, c.getDistrict(), c.getWarehouse(), d_id, w_id);
    sprintf(h.h_data, "%s    %s", w.w_name, d.d_name);
    
    h_table.add(h);                             // Insert new history row
}
