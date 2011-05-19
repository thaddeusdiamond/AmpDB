#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "../global.h"
#include "../config.h"

using namespace std;

int PART;

typedef struct {
    double price;
    int quantity;
} Stock;

typedef struct {
    int name;
    double balance;
} Customer;

Stock stock[100000];
Customer customer[30000];
int cnameindex[1000000];

void dbinit() {
    // create stock table
    for(int i = 0; i < 100000; i++) {
        stock[i].price = 10 + 1.1 * ((double)(i%10));
        stock[i].quantity = 1000;
    }
    
    // clear cust name index
    for(int i = 0; i < 1000000; i++) {
        cnameindex[i] = 0;
    }
    
    // create customer table + cust name index
    for(int i = 0; i < 30000; i++) {
        int n = rand() % 1000000;
        cnameindex[n] = i;
        customer[i].name = n;
        customer[i].balance = 1000000;
    }
}

void neworder() {
    int id;
    cin >> id;
    
    int p[10]; // partition
    int i[10]; // item id
    int q[10]; // quantity

    for(int j = 0; j < 10; j++) {
        cin >> p[j];
        cin >> i[j];
        cin >> q[j];
        
        if(p[j] == PART) {
            stock[i[j]].quantity -= q[j];
            if(stock[i[j]].quantity < 0)
                stock[i[j]].quantity += 91;
        }
    }

    // ack success
    cout << id << endl;
}

void payment1(){
    int id, cname;
    cin >> id;
    cin >> cname;
    
    // return cust id
    cout << id << " " << cnameindex[cname] << endl;
}

void payment2(){
    int id, cid;
    cin >> id;
    cin >> cid;
    
    double amount;
    cin >> amount;
    
    customer[cid].balance -= amount;
    
    // ack success
    cout << id << " " << 1 << endl;
}

int main(int argc, char **argv) {
    dbinit();
    PART = atoi(argv[1]);
    
    int type;
    
    while(true) {
        cin >> type;
        if(type == 0) neworder();
        if(type == 1) payment1();
        if(type == 2) payment2();
    }
    
    return 0;
}




