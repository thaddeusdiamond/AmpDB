/*  Implementation of TPC-C main run library
 *
 *  Thaddeus Diamond
 *  29 March 2011
 *
 *  Usage: ./TPC-C [Partition #]
 *
 */

#include "tpc_c.h"                              // tpc_c library

#define MAX_ARGS 64
       
Key Part;                                       // Machine partition #

QuickMap<Warehouse>  w_table(W_TABLE_ID, MAXW); // Database representation (.h)
QuickMap<District>   d_table(D_TABLE_ID, MAXD); //      tables are arrays
QuickMap<Customer>   c_table(C_TABLE_ID, MAXC);
QuickMap<NewOrder>   no_table(NO_TABLE_ID, MAXNO);
QuickMap<Order>      o_table(O_TABLE_ID, MAXO);
QuickMap<OrderLine>  ol_table(OL_TABLE_ID, MAXOL);
QuickMap<Stock>      s_table(S_TABLE_ID, MAXS);
QuickMap<Item>       i_table(I_TABLE_ID, ITEMS);
QuickMap<History>    h_table(H_TABLE_ID, HISTORY);

QuickMap< DBIndex<Key> >   c_last_index(C_TABLE_ID, MAXC);

Configuration *config;                          // Used in communicating
RemoteConnection *connection;                   //  w/mediator for 2nd lookup

ofstream log;

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
    return (w_id >> 48 == Part);
}

/* <----------------------- END RAND FUNCS -----------------------> */


/* tpccinit(): Initialize random database to perform work on */
void tpccinit() {
    int w, d, c, s, i;                          // New keys
    Key w_id, d_id, c_id, s_id;
    srand( time(NULL) );
    for(w = Part * MAXW; w < (Part + 1) * MAXW; w++) {
        w_id = w | (Part << 48) | (W_TABLE_ID << 32);
        createwarehouse(w_id);
        for(d = w * DPERW; d < (w + 1) * DPERW; d++) {
            d_id = d | (Part << 48) | (D_TABLE_ID << 32);
            createdistrict(d_id, w_id);
            for(c = d * CPERD; c < (d + 1) * CPERD; c++) {
                c_id = c | (Part << 48) | (C_TABLE_ID << 32);
                createcustomer(c_id, d_id, w_id);
            }
        }
        for(s = w * ITEMS; s < (w + 1) * ITEMS; s++) {
            s_id = s | (Part << 48) | (S_TABLE_ID << 32);
            createstock(s_id, w_id);
        }
    }
    
    srand(100);                                 // Seed random deterministically
    for(i = 0; i < ITEMS; i++)
        createitem(i | (Part << 48) | (I_TABLE_ID << 32));
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
    c.c_last = c_id;           
    
    /*                       SECONDARY INDEX                            */
    c_last_index[c.c_last] = DBIndex<Key>(c.c_last, c_id);
    
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
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " [CONFIG FILE] [PARTITION #] " << endl;
        //return false;                           // Illegal # args
        cerr << "Unspecified partition; assuming 0." << endl;
        Part = 0;
    } else Part = atoi(argv[2]);
    
//    cout << "Initializing benchmarks, please wait..." << endl;
    log.open("db.out");
    tpccinit();                                 // Initialize databases
    config = new Configuration(Part, argv[1]);
    //connection = RemoteConnection::GetInstance(*config);
    
    /*<---------------  BENCHMARKING TPC-C BY ITSELF  ------------->
    int j, sum, low, high;
    GenericTxn *t;
    sum = low = high = 0;
    for (int k = 0; k < 200; k++) {             // 200 Iterations
        time_t cur_time = time(NULL) + 1;       // Current Time
        for (j = 0; time(NULL) < cur_time; j++) {
	        t = generate(j, 10);                // Generate rand txn
	        if (t->txntype == NO_ID)
    	        new_order(t->args, j);          //      NEW ORDER
	        else
	            payment(t->args);               //      PAYMENT
	        delete t;
        }
        cout << "ROUND " << k << ": " << j << " transactions" << endl;
        sum += j;
        if (!low || j < low)
            low = j;
        if (j > high)
            high = j;
    }
    /*                      PRINT RESULTS                           
    cout << "Average over 200 iterations: " << (sum / 200.0) << endl;
    cout << "LOW: " << low << endl << "HIGH: " << high << endl;
    
    /*<-------------------  END BENCHMARK  ------------------------>
    
    cout << "Please enter transaction arguments now:" << endl;*/
     
    int argcount;                               // # args, type of txn, id
    string type;
    Key id;      
    
    while (true) {        
        cin >> type;                            // Read in variables
        cin >> id;
        cin >> argcount;
        
        Key args[MAX_ARGS];                     // Static args array
        for (int i = 0; i < argcount; i++)
            cin >> args[i];
        
        /*     NEWORDER TXN:        READ PHASE                  */
        if (!type.compare("NOR")) {
            bool reads_okay = true;
            for (int i = 20; i < 20 + args[3]; i++) {
                if (!check_reads(args[i]))
                    reads_okay = false;         // One variable not okay
            }
            cout << id << " " << reads_okay << endl;
        
        /*     NEWORDER TXN:        WRITE PHASE                 */
        } else if (!type.compare("NO")) {
            cout << id << " " << new_order(args, id) << endl; 
    
        /*     PAYMENT TXN:         READ PHASE                  
        } else if (!type.compare("PAYR")) {
        
        /*     PAYMENT TXN:         WRITE PHASE                 */
        } else if (!type.compare("PAY")) {
            cout << id << " " << payment(args) << endl; 
        } else {
            log << "ERROR!, UNKNOWN TYPE: " << type << endl;
        }
        log << "FINISHED PROCESSING TXN: " << id << endl;
    }
    log.close();
    
    /*                      FREE ALL MEMORY                     */
    delete &w_table;
    delete &d_table;
    delete &c_table;
    delete &no_table;
    delete &o_table;
    delete &ol_table;
    delete &s_table;
    delete &i_table;
    delete &h_table;
    delete &c_last_index;
    
    exit(EXIT_FAILURE);                         // DB Server Interrupt
}
