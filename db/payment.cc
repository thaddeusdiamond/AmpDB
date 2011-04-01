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

extern QuickMap< DBIndex<char *> >   c_last_index;

/* payment(Keys ...): 
 *  This is the main payment function and is the only one accessed directly
 */ 
int payment(Key *args, Key id) {

}
