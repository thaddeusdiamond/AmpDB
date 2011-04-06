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
            //free(p);                        // Free array memory and object
        }
  
        /*                  Overload Array Access                       */
        T& operator[](const Key id) throw (const char *) {
            int entry = hash(id);
            if (entry >= 0)
                return table[entry];        // Hash table quick lookup
            
            throw "Invalid Database Access";// Entry does not exist
        }
        
        T& operator[](char *key) throw (const char *) {
            int loc;
            while ((loc = hash(key)) < 0)
                rehash();                   // History table rehash
            return table[loc];              // Hash table quick lookup 
        } 
        
        /*DBIndex<Key>& operator[](Key key) throw (const char *) {
            return table[sec_index(key)];   // Hash table quick lookup
        }*/

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
        
        /*          Calculates a numeric hash value for string <b>      */
        int calcHash(char *s) {
	        int sum = 0;			        // Running sum to return
            for (sum = 0; *s; s++)
		        sum = (sum<<3) ^ *s;	    // Bit shift operation
		    return abs(sum % table_size);	// Return key value
        }
        
        /*                  Quick (constant time) Hash Fxn              */
        int hash(Key id) {
            Key table;                      // Get key table
            int16_t key, table_id;
            table = (int16_t) (id >> 32);
            key = (int16_t) (id >> 48);
            table_id = (int16_t) (id >> 32);
            
            if (table == table_id)          // Our table is valid for key
                return table_conversion(key, table_id, (int32_t) id);
            else
                return -1;                  // Key not in this table
        }
        
        /*                      Hash for History Table              */
        int hash(char *key) {
            int start = calcHash(key);      // Find first index
            int i = 0;
            while(i++ < table_size && table[start].exists)
                start++;
                
            if (i == table_size + 1)        // Table needs to be rehashed
                return -1;
            return start;                   // First null or 
        }
        
        /*                      Secondary Key Lookup                */
        int sec_index(Key key) {
            int start = calcHash(key);      // Find first index
            int i = 0;
            while(i++ < table_size && table[start].exists && 
                    strcmp(table[start].index, key) != 0)
                start++;
                
            if (i == table_size + 1)        // Table needs to be rehashed
                return -1;
            return start;                   // First null or 
        }
        
        /*                  CONTEXT SWITCH FOR TABLES               */
        int table_conversion(int part, int table, int id) {
            switch (table) {
                case W_TABLE_ID:                // WAREHOUSE LOOKUP
                    return id - (part * MAXW);  //      modulo max warehouses
                    break;
                    
                case D_TABLE_ID:                // DISTRICT LOOKUP
                    return id - (part * MAXD);  //      modulo max districts
                    break;
                
                case C_TABLE_ID:                // CUSTOMER LOOKUP
                    return id - (part * MAXC);  //      modulo Max Warehouses
                    break;
                
                case NO_TABLE_ID:               // NEWORDER LOOKUP
                    return id - (part * MAXNO); //      modulo max new orders
                    break;
                
                case O_TABLE_ID:                // ORDER LOOKUP
                    return id - (part * MAXO);  //      modulo max orders
                    break;
                
                case OL_TABLE_ID:               // ORDERLINE LOOKUP
                    return id - (part * MAXOL); //      modulo max orderlines
                    break;
                
                case S_TABLE_ID:                // STOCK LOOKUP
                    return id - (part * MAXS);  //      modulo max stock #
                    break;
                    
                case I_TABLE_ID:                // ITEM LOOKUP
                    return id - (part * ITEMS); //      modulo # items
                    break;
            }
        }
};

#endif                                      // (BASE_QUICKMAP_H__)
