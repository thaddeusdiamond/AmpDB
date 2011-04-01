/*  Implementation of TPC-C main run library
 *
 *  Thaddeus Diamond
 *  29 March 2011
 *
 *  Usage: ./TPC-C [Partition #]
 *
 */

#include "tpc_c.h"                              // tpc_c library
        
int Part;                                       // Machine partition #
int WarehouseMask = 0;

map<Key, Warehouse>   w_table;                  // Database representation (.h)
map<Key, District>    d_table;                  //      tables are arrays
map<Key, Customer>    c_table;
map<Key, NewOrder>    no_table;
map<Key, Order>       o_table;
map<Key, OrderLine>   ol_table;
map<Key, Stock>       s_table;
map<Key, Item>        i_table;

/* <---------------------- BEGIN RAND FUNCS -----------------------> */

void fill_with_random(int start, int length, char *string) {
    for (int i = start; i < length; i++)
        string[i] = rand() % 26 + 'A';          // Randomized, null-term string
    string[length] = '\0';
}

void fill_with_nums(int start, int length, char *string) {
    for(int i = start; i < length;  i++) 
        string[i] = rand() % 10 + '0';
}

bool warehouse_local(Key w_id) {
    return (WarehouseMask & (1 << w_id));
}

/* <----------------------- END RAND FUNCS -----------------------> */


/* tpccinit(): Initialize random database to perform work on */
void tpccinit() {
    Key w, d, c, s, i;                          // New keys
    srand( time(NULL) );
    for(w = Part * MAXW; w < (Part + 1) * MAXW; w++) {
        createwarehouse(w);
        WarehouseMask = WarehouseMask | (1 << w);
        for(d = w * DPERW; d < (w + 1) * DPERW; d++) {
            createdistrict(d, w);
            for(c = d * CPERD; c < (d + 1) * CPERD; c++)
                createcustomer(c, d, w);
        }
        for(s = w * ITEMS; s < (w + 1) * ITEMS; s++)
            createstock(s, w);
    }
    
    srand(100);                                 // Seed random deterministically
    for(i = 0; i < ITEMS; i++)
        createitem(i);
}

/* createwarehouse(key): Insert random warehouse into DB */
void createwarehouse(Key w_id) {
    Warehouse w(w_id);
    
    fill_with_random(0, 10, w.w_name);          // Random, null-term name
    
    fill_with_nums(0, 3, w.w_street_1);         // ## RANDOM
    w.w_street_1[3] = ' ';                      //      (format for addresses)
    fill_with_random(4, 20, w.w_street_1);
    
    for(int i = 0; i < 20; i++) w.w_street_2[i] = ' ';
    w.w_street_2[20] = '\0';
    
    fill_with_random(0, 20, w.w_city);          // Random city/state/zip
    fill_with_random(0, 2, w.w_state);
    fill_with_nums(0, 9, w.w_zip);
    w.w_zip[9] = '\0';
    
    /* <---------        INITIAL DEFAULTS -----------> */
    w.w_tax = 0.05;
    w.w_ytd = 0;
    /* <--------         END DEFAULTS    ------------> */
    
    w_table[w_id] = w;
}

/* createdistrict(keys...): Insert random district into DB */
void createdistrict(Key d_id, Key d_w_id) {
    District d(d_id, d_w_id);
    
    fill_with_random(0, 10, d.d_name);          // Random, null-term name
    
    fill_with_nums(0, 3, d.d_street_1);         // ## RANDOM
    d.d_street_1[3] = ' ';                      //      (format for addresses)
    fill_with_random(4, 20, d.d_street_1);
    
    for(int i = 0; i < 20; i++) d.d_street_2[i] = ' ';
    d.d_street_2[20] = '\0';
    
    fill_with_random(0, 20, d.d_city);          // Random city/state/zip
    fill_with_random(0, 2, d.d_state);
    fill_with_nums(0, 9, d.d_zip);
    d.d_zip[9] = '\0';
    
    /* <---------        INITIAL DEFAULTS -----------> */
    d.d_tax = 0.05;
    d.d_ytd = 0;
    d.d_next_o_id = 1;
    /* <--------         END DEFAULTS    ------------> */
    
    d_table[d_id] = d;
}

/* createcustomer(keys...): Insert random customer into DB */
void createcustomer(Key c_id, Key c_d_id, Key c_w_id) {
    Customer c(c_id, c_d_id, c_w_id);
    
    fill_with_random(0, 16, c.c_first);         // Randomized first, middle
    fill_with_random(0, 2, c.c_middle);         //      last
    fill_with_random(0, 16, c.c_last);
    
    fill_with_nums(0, 3, c.c_street_1);         // ## RANDOM
    c.c_street_1[3] = ' ';                      //      (format for addresses)
    fill_with_random(4, 20, c.c_street_1);
    
    for(int i = 0; i < 20; i++) c.c_street_2[i] = ' ';
    c.c_street_2[20] = '\0';
    
    fill_with_random(0, 20, c.c_city);          // Random city/state/zip
    fill_with_random(0, 2, c.c_state);
    fill_with_nums(0, 9, c.c_zip);
    c.c_zip[9] = '\0';
    
    /* <---------        INITIAL DEFAULTS -----------> */
    c.c_since = 0;
    (c.c_credit[0] = 'G') && (c.c_credit[1] = 'C') && (c.c_credit[2] = '\0');
    c.c_credit_lim = 0.01;
    
    c.c_discount = 0.5;
    c.c_balance = 0;
    
    c.c_ytd_payment = 0;
    c.c_payment_cnt = 0;
    c.c_delivery_cnt = 0;
    
    fill_with_random(0, 50, c.c_data);
    /* <--------         END DEFAULTS    ------------> */
    
    c_table[c_id] = c;
}

/* createstock(keys...): Insert random stock into DB */
void createstock(Key s_id, Key s_w_id) {
    Stock s(s_id, s_w_id);
    
    s.s_quantity = rand() % 100 + 100;          // Random quantity in stock
    
    for (int i = 0; i < 10; i++)
        fill_with_random(0, 24, s.s_dist[i]);   // DO I RANDOMIZE DISTRICTS??
   
    /* <---------        INITIAL DEFAULTS -----------> */
    s.s_ytd = 0;
    s.s_order_cnt = 0;
    s.s_remote_cnt = 0;
    /* <--------         END DEFAULTS    ------------> */
    
    fill_with_random(0, 50, s.s_data);
    
    s_table[s_id] = s;
}

/* createitem(key): Insert random item into DB */
void createitem(Key i_id) {
    Item i(i_id);
    
    fill_with_random(0, 24, i.i_name);          // Random name, price and data
    i.i_price = (double) (rand() % 100);
    fill_with_random(0, 50, i.i_data);
    
    i_table[i_id] = i;
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " [PARTITION #]" << endl;
        return false;                           // Illegal # args
    }
    
    Part = atoi(argv[1]);
    cout << "Initializing benchmarks, please wait..." << endl;
    tpccinit();
    cout << "Please enter transaction arguments now:" << endl;
    
    int argcount;                               // # args, type of txn, id
    string type;
    Key txnid;      
    
    while (true) {        
        cin >> type;                            // Read in variables
        cin >> txnid;
        cin >> argcount;

        Key args[argcount];                     // Static args array
        for (int i = 0; i < argcount; i++)
            cin >> args[i];
            
        /*     NEWORDER TXN:        READ PHASE                  */
        if (!type.compare("NOR")) {
            bool reads_okay = true;
            for (int i = 20; i < 20 + args[3]; i++) {
                if (!check_reads(args[i]))
                    reads_okay = false;         // One variable not okay
            }
            cout << reads_okay << endl;
        
        /*     NEWORDER TXN:        WRITE PHASE                 */
        } else if (!type.compare("NO")) {
            // Process New Order
            cout << new_order(args, txnid) << endl; 
    
        
        /*     PAYMENT TXN:         READ PHASE                 */
        } else if (!type.compare("PAYR")) {
        
        /*     PAYMENT TXN:         WRITE PHASE                 */
        } else if (!type.compare("PAY")) {
        
        }
    }
    
    exit(EXIT_FAILURE);                         // DB Server Interrupt
}
