/*  TPC-C library interface (with class definitions)
 *
 *  Thaddeus Diamond
 *  29 March 2011
 *
 */

#ifndef _TPCC_H_                // Avoid circular dependencies


#define _TPCC_H_

#include "../global.h"          // Std Library Headers 
#include "../generictxn.h"
#include "../quickmap.h"
#include "../remote.h"
#include "../loadgen/loadgen.h"
#include "../freeport_config.h"
#include <map>               
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <stdint.h>
#include <iostream>
#include <fstream>

#define Deque CircularBuffer    // Data structure aliases
#define Key int64_t
#define Val int64_t

using namespace std;

//////////////    WAREHOUSE CLASS DEFINITION    //////////////
class Warehouse {
    public:
        Warehouse() { exists = false; } // Null constructor
        Warehouse(Key w) {
            w_id = w;                   // Public constructor w/id
            exists = true;
        }
        
        /*                  ACCESSOR METHODS            */
        Key getID()         { return w_id; }

        /*                  PRINT METHODS               */
        void toString() {
            cout << "WAREHOUSE #" << getID() << ":" << endl;
            cout << "\tName:\t\t" << w_name << endl;
            cout << "\tStreet 1:\t" << w_street_1 << endl;
            cout << "\tStreet 2:\t" << w_street_2 << endl;
            cout << "\tCity:\t\t" << w_city << endl;
            cout << "\tState:\t\t" << w_state << endl;
            cout << "\tZip:\t\t" << w_zip << endl;
            cout << "\tTax:\t\t" << w_tax << endl;
            cout << "\tYTD:\t\t" << w_ytd << endl;
        }
        
        /*                  WAREHOUSE TABLE SCHEMA           */
        char w_name[11];        // Name of warehouse            (VAR LEN)
        char w_street_1[21];    // Street address 1             (VAR LEN)
        char w_street_2[21];    // Street address 2             (VAR LEN)
        char w_city[21];        // Warehouse city               (VAR LEN)
        char w_state[3];        // Warehouse state              (FIX LEN)
        char w_zip[10];         // Warehouse zip code           (FIX LEN)
        double w_tax;           // Taxes paid by warehouse      (SIGNED [4,4])
        double w_ytd;           // YTD earnings                 (SIGNED [12,2])
        bool exists;
    protected:
        Key  w_id;              // Primary key for warehouse
};

//////////////    DISTRICT CLASS DEFINITION    //////////////
class District {
    public:
        District() { exists = false; }  // Null constructor
        District(Key d, Key w) {
            d_id = d;                   // Public constructor, set primary key
            d_w_id = w;                 //      warehouse district key, and DNOID = 0
            d_next_o_id = 0;
            exists = true;
        }
        
        /*                  ACCESSOR METHODS            */
        Key getID()         { return d_id;   }
        Key getWarehouse()  { return d_w_id; }

        /*                  PRINT METHODS               */
        void toString() {
            cout << "DISTRICT #" << getID() << ":" << endl;
            cout << "\tWarehouse #" << getWarehouse() << endl;
            cout << "\tName:\t\t" << d_name << endl;
            cout << "\tStreet 1:\t" << d_street_1 << endl;
            cout << "\tStreet 2:\t" << d_street_2 << endl;
            cout << "\tCity:\t\t" << d_city << endl;
            cout << "\tState:\t\t" << d_state << endl;
            cout << "\tZip:\t\t" << d_zip << endl;
            cout << "\tTax:\t\t" << d_tax << endl;
            cout << "\tYTD:\t\t" << d_ytd << endl;
            cout << "\tNext Order ID:\t" << d_next_o_id << endl;
        }
        
        /*                  DISTRICT TABLE SCHEMA            */
        char d_name[11];        // Name of district             (VAR LEN)
        char d_street_1[21];    // Street address 1             (VAR LEN)
        char d_street_2[21];    // Street address 2             (VAR LEN)
        char d_city[21];        // District city                (VAR LEN)
        char d_state[3];        // District state               (FIX LEN)
        char d_zip[10];         // District zip code            (FIX LEN)
        double d_tax;           // Taxes paid by warehouse      (SIGNED [4,4])
        double d_ytd;           // YTD earnings                 (SIGNED [12,2])
        int d_next_o_id;        // Next available order #
        bool exists;
    protected:
        Key  d_id;              // Primary key for district
        Key  d_w_id;            // Primary (foreign) key [district by warehouse]
};

//////////////    CUSTOMER CLASS DEFINITION    //////////////
class Customer {
    public:
        Customer() { exists = false; }           // Null constructor
        Customer(Key c, Key d, Key w) {
            c_id = c;           // Public constructor w/id's. Set all three
            c_d_id = d;         //      for customer, district, and warehouse
            c_w_id = w;
            exists = true;
        }
        
        /*                  ACCESSOR METHODS            */
        Key getID()         { return c_id;   }
        Key getDistrict()   { return c_d_id; }
        Key getWarehouse()  { return c_w_id; }

        /*                  PRINT METHODS               */
        void toString() {
            cout << "CUSTOMER #" << getID() << ":" << endl;
            cout << "\tDistrict #" << getDistrict() << endl;
            cout << "\tWarehouse #" << getWarehouse() << endl;
            cout << "\tName:\t\t" << c_first << " " << c_middle << " " << c_last << endl;
            cout << "\tStreet 1:\t" << c_street_1 << endl;
            cout << "\tStreet 2:\t" << c_street_2 << endl;
            cout << "\tCity:\t\t" << c_city << endl;
            cout << "\tState:\t\t" << c_state << endl;
            cout << "\tZip:\t\t" << c_zip << endl;
            cout << "\tSince:\t\t" << c_since << endl;
            cout << "\tCredit:\t\t" << c_credit << endl;
            cout << "\tCredit Limit:\t" << c_credit_lim << endl;
            cout << "\tDiscount:\t\t" << c_discount << endl;
            cout << "\tBalance:\t\t" << c_balance << endl;
            cout << "\tYTD:\t\t" << c_ytd_payment << endl;
            cout << "\tPay Count:\t" << c_payment_cnt << endl;
            cout << "\tDelivery Count:\t" << c_delivery_cnt << endl;
            cout << "\tMiscellaneous:\t" << c_data << endl;
        }
        
        /*                  CUSTOMER TABLE SCHEMA            */
        char   c_first[17];     // Customer's first name        (VAR LEN)
        char   c_middle[3];     // Customer's middle initials   (FIX LEN)
        Key    c_last;          // Customer's last name         (VAR LEN)
        char   c_street_1[21];  // Street address 1             (VAR LEN)
        char   c_street_2[21];  // Street address 2             (VAR LEN)
        char   c_city[21];      // Customer city                (VAR LEN)
        char   c_state[3];      // Customer state               (FIX LEN)
        char   c_zip[10];       // Customer zip code            (FIX LEN)
        int    c_since;         // Time since customer enrolled (DATE AND TIME)
        char   c_credit[3];     // "GC" or "BC"                 (FIX LEN)
        double c_credit_lim;    // Customer's credit limit      (SIGNED [12,2])
        double c_discount;      // Customer's discount          (SIGNED [4,4])
        double c_balance;       // Remaining balance            (SIGNED [12,2])
        double c_ytd_payment;   // Total YTD payment            (SIGNED [12,2])
        int    c_payment_cnt;   // Payment count                (NUMERIC 4)
        int    c_delivery_cnt;  // Delivery count               (NUMERIC 4)
        char   c_data[501];     // Miscellaneous information    (VAR LEN)
        bool exists;
    protected:
        Key c_id;               // Primary customer key
        Key c_d_id;             // Primary (foreign) district key
        Key c_w_id;             // Primary (foreign) warehouse key
};
        
//////////////    NEW-ORDER CLASS DEFINITION    //////////////
class NewOrder {
    public:
        NewOrder() { exists = false; }           // Null constructor
        NewOrder(Key o, Key d, Key w) {
            no_o_id = o;        // Public constructor.  Set the order,
            no_d_id = d;        //      district and warehouse keys
            no_w_id = w;
            exists = true;
        }
        
        /*                  ACCESSOR METHODS            */
        Key getID()         { return no_o_id; }
        Key getDistrict()   { return no_d_id; }
        Key getWarehouse()  { return no_w_id; }
        bool exists;

        /*                  NEW-ORDER TABLE SCHEMA          */
    protected:
        Key no_o_id;            // Primary new order key
        Key no_d_id;            // Primary (foreign) district key
        Key no_w_id;            // Primary (foreign) warehouse key
};

//////////////    ORDER CLASS DEFINITION    //////////////
class Order {
    public:
        Order() { exists = false; }              // Null constructor
        Order(Key o, Key d, Key w, Key c) {
            o_id = o;           // Public constructor.  Set the order, district,
            o_d_id = d;         //      warehouse and customer keys
            o_w_id = w;
            o_c_id = c;
            exists = true;
        }

        /*                  ACCESSOR METHODS            */
        Key getID()         { return o_id;   }
        Key getDistrict()   { return o_d_id; }
        Key getWarehouse()  { return o_w_id; }
        Key getCustomer()   { return o_c_id; }

        /*                  ORDER TABLE SCHEMA          */
        int  o_entry_d;         // Order entry date             (DATE AND TIME)
        int  o_carrier_id;      // Order carrier id             (Unique IDs)
        int  o_ol_cnt;          // Order-Line count             (NUMERIC 2)
        bool o_all_local;       // Boolean true if all local    (NUMERIC 1)
        bool exists;
    protected:
        Key  o_id;              // Primary order key
        Key  o_d_id;            // Primary (foreign) district key
        Key  o_w_id;            // Primary (foreign) warehouse key
        Key  o_c_id;            // Primary (foreign) customer key
};

//////////////    ORDER-LINE CLASS DEFINITION    //////////////
class OrderLine {
    public:
        OrderLine() { exists = false; }          // Null constructor
        OrderLine(Key o, Key d, Key w, int n, Key i, Key q) {
            ol_o_id = o;        // Public constructor.  Set values for order-
            ol_d_id = d;        //      line, district, warehouse, #, item,
            ol_w_id = w;        //      supply warehouse (same as w), & quantity
            ol_number = n;
            ol_i_id = i;
            ol_supply_w_id = w;
            ol_quantity = q;
            exists = true;
        }

        /*                  ACCESSOR METHODS            */
        Key getID()         { return ol_o_id; }
        Key getDistrict()   { return ol_d_id; }
        Key getWarehouse()  { return ol_w_id; }
        Key getItem() 	    { return ol_i_id; }    
        Key getSupply()     { return ol_supply_w_id; }

        /*                  ORDER-LINE TABLE SCHEMA          */
        int  ol_number;         // Order-Line number            (15 unique IDs)
        int  ol_delivery_d;     // Delivery datetime            (DATE AND TIME)
        int  ol_quantity;       // Quantity in order-line       (NUMERIC 2)
        double ol_amount;       // Amount $ in the order-line   (SIGNED [6,2])
        char ol_dist_info[25];  // Information on district      (FIX LEN)
        Key  ol_supply_w_id;    // Primary (foreign) supply key
        bool exists;
    protected:
        Key  ol_o_id;           // Primary order-line key
        Key  ol_d_id;           // Primary (foreign) district key
        Key  ol_w_id;           // Primary (foreign) warehouse key
        Key  ol_i_id;           // Primary (foreign) item key
};

//////////////    ITEM CLASS DEFINITION    //////////////
class Item {
    public:
        Item() { exists = false; }      // Null constructor
        Item(Key i) {
            i_id = i;                   // Public constructor.  Set item id
            exists = true;
        }
        
        /*                  ACCESSOR METHODS            */
        Key getID()         { return i_id;    }
        
        /*                  PRINT METHODS               */
        void toString() {
            cout << "ITEM #" << getID() << ":" << endl;
            cout << "\tName:\t\t" << i_name << endl;
            cout << "\tPrice:\t\t" << i_price << endl;
            cout << "\tMiscellaneous:\t" << i_data << endl;
        }
        
        /*                  ITEM TABLE SCHEMA          */
        char   i_name[25];      // Item name                    (VAR LEN)
        double i_price;         // Item price                   (NUMERIC [5,2])
        char   i_data[51];      // Miscellaneous information    (VAR LEN)
        bool exists;
    protected:
        Key    i_id;            // Primary item key
};

//////////////    STOCK CLASS DEFINITION    //////////////
class Stock {
    public:
        Stock() { exists = false; } // Null constructor
        Stock(Key i, Key w) {
            s_i_id = i;             // Public constructor.  Set the stock item
            s_w_id = w;             //      and warehouse keys.  Randomize qty
            s_quantity = rand() % 256;
            s_ytd = s_order_cnt = s_remote_cnt = 0;
            exists = true;
        }
        
        /*                  ACCESSOR METHODS            */
        Key getID()         { return s_i_id; }
        Key getWarehouse()  { return s_w_id; }

        /*                  PRINT METHODS               */
        void toString() {
            cout << "STOCK #" << getID() << ":" << endl;
            cout << "\tWarehouse #" << getWarehouse() << endl;
            cout << "\tQuantity:\t" << s_quantity << endl;
            for (int i = 0; i < 10; i++)
                cout << "\tDistrict " << i + 1 << ":\t" << s_dist[i] << endl;
            cout << "\tYTD:\t\t" << s_ytd << endl;
            cout << "\tOrder Count:\t" << s_order_cnt << endl;
            cout << "\tRemote Count:\t" << s_remote_cnt << endl;
            cout << "\tMiscellaneous:\t" << s_data << endl;
        }
        
        /*                  STOCK TABLE SCHEMA          */
        int  s_quantity;        // Quantity in stock            (NUMERIC 4)
        char s_dist[10][25];    // District names               (FIX LEN)
                                //      must be discrete (see Clause 1.4.8)
        int  s_ytd;             // Year to date sold            (NUMERIC 8)
        int  s_order_cnt;       // Order count                  (NUMERIC 4)
        int  s_remote_cnt;      // Remote count                 (NUMERIC 4)
        char s_data[51];        // Miscellaneous info           (VAR LEN)
        bool exists;
    protected:
        Key  s_i_id;            // Primary stock key
        Key  s_w_id;            // Primary (foreign) warehouse key
};

//////////////    HISTORY CLASS DEFINITION    //////////////
class History {
    public:
        History() { exists = false; }            // Null constructor
        History(Key c, Key c_d, Key c_w, Key d, Key w) {
            h_c_id = c;         // Public constructor.  Set customer,
            h_c_d_id = c_d;     //      customer district, customer warehouse,
            h_c_w_id = c_w;     //      district and warehouse keys.
            h_d_id = d;
            h_w_id = w;
            exists = true;
        }
        
        /*                  ACCESSOR METHODS            */
        Key getCustomer()       { return h_c_id;   }
        Key getCustDistrict()   { return h_c_d_id; }
        Key getCustWarehouse()  { return h_c_w_id; }
        Key getDistrict()       { return h_d_id;   }
        Key getWarehouse()      { return h_w_id;   }
        
        /*                  HISTORY TABLE SCHEMA          */
        int    h_date;          // Date of history entry        (DATE AND TIME)
        double h_amount;        // Amount for history entry     (SIGNED [6,2])
        char   h_data[25];      // Miscellaneous info           (VAR LEN)
        bool exists;
    protected:
        Key  h_c_id;            // Primary (foreign) customer key
        Key  h_c_d_id;          // Primary (foreign) customer district key
        Key  h_c_w_id;          // Primary (foreign) customer warehouse key
        Key  h_d_id;            // Primary (foreign) district key
        Key  h_w_id;            // Primary (foreign) warehouse key
};


/* <---------------------- BEGIN FUNC DECS -----------------------> */

void createwarehouse(Key w_id);
void createdistrict (Key d_id, Key d_w_id);
void createcustomer (Key c_id, Key c_d_id, Key c_w_id);
void createstock    (Key s_id, Key s_w_id);
void createitem     (Key i_id);

bool warehouse_local(Key w_id);

void tpccinit();
void tpccdump();

// TODO: orderstatustxn, deliverytxn, stockleveltxn

    /*                      New Order                       */
int new_order(Key *args, Key txnid);
int processNewOrder(Key txnid, Key w_id, Key d_id, Key c_id, int ol_cnt, 
                     Key *ol_w, Key *ol_i, int *ol_q, int systime);
bool decrease_supply(Key w, Stock s, int qty);
bool check_reads(Key item);

    /*                     Payment Txn                      */
Key payment(Key *args);

/* <----------------------- END FUNC DECS ------------------------> */


#endif                                              // (#ifndef _TPCC_H_)
