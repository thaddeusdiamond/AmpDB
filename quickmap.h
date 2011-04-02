/*  QuickMap custom class
 *
 *  Thaddeus Diamond
 *  1 April 2011
 *
 */

#ifndef BASE_QUICKMAP_H__
#define BASE_QUICKMAP_H__

#include "global.h"          // Std Library Headers 
#include <stdint.h>
#include <cstdlib>
#include <iostream>
#include <new>

#define Key int64_t

using namespace std;

/*                  Customized Secondary Index Class (FAST)             */
template <class T>
class DBIndex {
    public:
        DBIndex() { exists = false;}        // Null constructor
        DBIndex(T i, Key k) {
            key = k;                        // Initialize (index, key) pair
            index = i;
            exists = true;
        }
        
        void toString() {
            cout << "INDEX: " << index << "\t\tKEY: " << key << endl;
        }
        
        T index;                            // (index, pair) instance
        Key key;
        bool exists;
};

/*                  Quick, Low Overhead Map (DB) Implementation         */
template <class T>
class QuickMap {

    public:
        QuickMap() {}                       // Null constructor
        QuickMap(Key t_id, int size) {
            table_id = t_id;                // Initialize table_id, table itself
            table = new T[size];            //      and table size
            table_size = size;
            
            for (int i = 0; i < size; i++)
                table[i] = T();
        }
        
        /*                Overloaded Delete Operator                    */
        static void operator delete(void *p) {
            delete[] ((QuickMap<T> *) p)->table;
            free(p);                        // Free array memory and object
        }
  
        /*                  Overload Array Access                       */
        T& operator[](const Key id) throw (const char *) {
            int entry = hash(id);
            if (entry >= 0)
                return table[entry];        // Hash table quick lookup
            
            throw "Invalid Database Access";// Entry does not exist
        }
        
        /*T& operator[](const char *key) throw (const char *) {
            int loc;
            while ((loc = hash(key)) < 0)
                rehash();
            return table[loc];              // Hash table quick lookup 
        }  <------- ERROR!!!! */
        
        DBIndex<char *>& operator[](const char *sec_key) throw (const char *) {
            return table[hash(sec_key)];    // Hash table quick lookup
        }

    protected:
        Key table_id;                       // Table this represents
        T *table;                           // Table itself
        int table_size;                     // Table size
        
        /*                Ability to add to end of table                */
        void rehash() {
            table_size *= 2;                //  Reallocate space
            T *newTable = new T[table_size];
            memcpy(newTable, table, table_size * sizeof(T));
            
            delete[] table;                 //  Garbage collection
            table = newTable;
        }
        
        // Calculates a numeric hash value for string <b>
        int calcHash(char *s) {
	        int sum = 0;			        // Running sum to return
            for (sum = 0; *s; s++)
		        sum = (sum<<3) ^ *s;	    // Bit shift operation
		    return abs(sum % table_size);	// Return key value
        }
        
        /*                  Quick (constant time) Hash Fxn              */
        int hash(char *key) {
            int start = calcHash(key);      // Find first index
            int i = 0;
            while(i++ < table_size && table[start].exists && 
                    strcmp(table[start].index, key) != 0)
                start++;
                
            if (i == table_size + 1)        // Table needs to be rehashed
                return -1;
            return start;                   // First null or 
        }
        
        int hash(Key id) {
            Key table;                      // Get key table
            table = (int16_t) (id >> 32);
            
            if (table == table_id)          // Our table is valid for key
                return table_conversion((int16_t) (id >> 32), (int32_t) id);
            else
                return -1;                  // Key not in this table
        }
        
        int table_conversion(int table, int id) {
            /*              CONTEXT SWITCH FOR TABLES               */
            switch (table) {
                case W_TABLE_ID:            // WAREHOUSE LOOKUP
                    return id % MAXW;       //      modulo max warehouses
                    break;
                    
                case D_TABLE_ID:            // DISTRICT LOOKUP
                    return id % MAXD;       //      modulo max districts
                    break;
                
                case C_TABLE_ID:            // CUSTOMER LOOKUP
                    return id % MAXC;       //      modulo Max Warehouses
                    break;
                
                case NO_TABLE_ID:           // NEWORDER LOOKUP
                    return id % MAXNO;      //      modulo max new orders
                    break;
                
                case O_TABLE_ID:            // ORDER LOOKUP
                    return id % MAXO;       //      modulo max orders
                    break;
                
                case OL_TABLE_ID:           // ORDERLINE LOOKUP
                    return id % MAXOL;      //      modulo max orderlines
                    break;
                
                case S_TABLE_ID:            // STOCK LOOKUP
                    return id % MAXS;       //      modulo max stock #
                    break;
                    
                case I_TABLE_ID:            // ITEM LOOKUP
                    return id % ITEMS;      //      modulo # items
                    break;
            }
        }
};

#endif                                      // (BASE_QUICKMAP_H__)
