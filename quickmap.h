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

template <class T>
class QuickMap {

    public:
        QuickMap() {}                       // Null constructor
        QuickMap(Key t_id, int size) {
            table_id = t_id;
            table = new T[size];
        }
        
        /*              DELETE FUNCTION IS BROKEN???            */
        static void operator delete(void *p) {
            delete[] ((QuickMap<T> *) p)->table;              // Free array memory
            free(p);
        }
  
        /*                  Overload array access                       */
        T& operator[](const Key id) throw (const char *) {
            int entry = hash(id);
            if (entry >= 0)
                return table[entry];        // Hash table quick lookup
            
            throw "Invalid Database Access";// Entry does not exist
        }

    protected:
        Key table_id;                       // Table this represents
        T *table;
        
        /*                  Quick (constant time) Hash Fxn              */
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
