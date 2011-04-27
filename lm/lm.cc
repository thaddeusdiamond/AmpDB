#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <map>
#include <sys/time.h>
#include "../circularbuffer.h"
#include "../loadgen/loadgen.h"
#include "../global.h"
#include "../remote.h"
#include "../generictxn.h"
#include "../message.h"
#include "../nonblock.h"

using namespace std;
#define DEQUE CircularBuffer
#define KEY int64_t
#define VAL int64_t

#define DEBUG 0
#define NPARTS 64

class Txn;
class Lock;

int PART;
map<int, Txn *> txns;
map<KEY, Lock *> locks;
DEQUE<Txn *> readytxns;
int partitionid;
pthread_t tpccthread;


DEQUE<Message> incomingmsgs;
DEQUE<Message> oldmessages;
DEQUE<GenericTxn> incomingtxns;
DEQUE< map<KEY, VAL> > incomingdbtxns;
DEQUE<GenericTxn *> outgoingdbtxns;

pthread_mutex_t olatch, ilatch;

Configuration *config;
RemoteConnection *connection;

double tim() { struct timeval tv; gettimeofday(&tv, NULL); return tv.tv_sec + tv.tv_usec/1e6; }

void ginits(int node_id, char *config_file) {
    setlinebuf(stdout);
    setlinebuf(stderr);
    std::ios::sync_with_stdio();
    config = new Configuration(node_id, config_file);
    connection = RemoteConnection::GetInstance(*config);
    srand(0);
    pthread_mutex_init(&olatch, NULL);
    pthread_mutex_init(&ilatch, NULL);
}


///////////// KEY /////////////
    
int16_t part(KEY k) {
    return (int16_t)(k >> 48);
}

int16_t table(KEY k) {
    return (int16_t)(k >> 32);
}

int32_t record(KEY k) {
    return (int32_t) k;
}

void flattenmap(map<KEY,VAL> *m, int64_t *size, int64_t **data) {
    *size = 2*m->size();
    *data = (int64_t *)malloc(*size * sizeof(int64_t));
    int offset = 0;
    for(map<KEY,VAL>::iterator i = m->begin(); i != m->end(); ++i) {
        (*data)[offset++] = i->first;
        (*data)[offset++] = i->second;
    }
}

void unflattenmap(map<KEY,VAL> *m, int size, int64_t *data) {
    for(int i = 0; i < size; i+=2) {
        if(!m->count(data[i])) {
            (*m)[data[i]] = data[i+1];
        }
    }
}


///////////// TXN /////////////

class Txn : public GenericTxn {
public:
    
    Txn(GenericTxn *gt) {
        txnid = gt->txnid;
        txnid_unordered = gt->txnid_unordered;
        source_mediator = gt->source_mediator;
        txntype = gt->txntype;
        isolationlevel = gt->isolationlevel;
        mp = gt->mp;
        
        rsetsize = gt->rsetsize;
        for (int i = 0; i < rsetsize; i++)
            rset[i] = gt->rset[i];
        wsetsize = gt->wsetsize;
        for (int i = 0; i < wsetsize; i++)
            wset[i] = gt->wset[i];
        argcount = gt->argcount;
        for (int i = 0; i < argcount; i++)
            args[i] = gt->args[i];
        
        readphasedone = false;
        lockwaits = 0;
        totalmessages = 0;
        
        if(mp) {
            for(int i = 0; i < gt->wsetsize; i++) {
                if(part(gt->wset[i]) == PART) {
                    //txn involves local writes, so count remote reads
                    bool readat[NPARTS] = { false };
                    for(int j = 0; j < gt->rsetsize; j++) {
                        int p = part(gt->wset[i]);
                        assert(0 <= p && p < NPARTS);
                        readat[p] = true;
                    }
                    for(int j = 0; j < NPARTS; j++) {
                        if(readat[j])
                            totalmessages++;
                    }
                    break;
                }
            }
        }
        messagewaits = totalmessages;
    }
    

    // this function is called once all locks have been acquired and the transaction
    // is ready to be sent to the storage layer for execution. run sends a command
    // to the storage layer starting the appropriate stored procedure running with the args supplied
    void run() {
//        cout << "RUNNING TXN: " << txnid;
        switch(txntype) {
            case NO_ID:
                if (mp)
                    type = "NOR";
                else
                    type = "NO";
                break;
                
            case PAY_ID:
                type = "PAY";
                break;
            
            case OS_ID:
            case DEL_ID:
            case SL_ID:
                return;
                break;
        }
        
//        cout << "\tTYPE: " << type << endl;
        pthread_mutex_lock(&olatch);
        outgoingdbtxns.enqueue(this);
        pthread_mutex_unlock(&olatch);
        
    }
    

    // this function is called (for multipartition txns only) after
    // all messages have been received
    void run2() {
        switch(txntype) {
            case NO_ID:
                if (mp && reads[0])
                    type = "NO";
                break;
                
            case OS_ID:
            case DEL_ID:
            case SL_ID:
            case PAY_ID:
                return;
                break;
        }
        
        pthread_mutex_lock(&olatch);
        outgoingdbtxns.enqueue(this);
        pthread_mutex_unlock(&olatch);
    }


//  GenericTxn class variables:
//  ---------------------------   
//  int64_t txnid;
//  int64_t txnid_unordered;
//  int64_t source_mediator;
//  int64_t txntype;
//  IsolationLevel isolationlevel; // SERIALIZABLE, SNAPSHOT, READ_COMMITTED, or READ_UNCOMMITTED
//  bool mp;
//  int64_t rsetsize;
//  int64_t wsetsize;
//  int64_t argcount;
//  int64_t rset[64];
//  int64_t wset[64];
//  int64_t args[64];


//  Txn class variables:
//  --------------------
    int totalmessages;      // number of (other) read partitions, or 0 if this partition performs no writes
    int lockwaits;          // number of lock acquisitions on which txn is currently blocked
    int messagewaits;       // number of messages still waiting to be received before txn can complete
    bool readphasedone;     // set to true after read phase is complete for multipartition txns
    map<KEY,VAL> reads;     // local copy of data state; initially empty
    
};


///////////// LOCK /////////////

class Lock {
public:

    Lock() {
        owner = NULL;
    }

    void lock(Txn *t) {
        if(owner == NULL) {
            owner = t;
        } else {
            requests.enqueue(t);
            t->lockwaits++;
        }
    }
    
    void unlock(Txn *t) {
        assert(owner == t);
        if(requests.size()) {
            owner = requests.dequeue();
            owner->lockwaits--;
            if(owner->lockwaits == 0)
                owner->run();
        } else {
            owner = NULL;
        }
    }

    Txn *owner;
    DEQUE<Txn *> requests;
};


///////////// LOCK MANAGER /////////////

// initialize and request locks for a new incoming transaction
void processNewTxn(GenericTxn gt) {
    // create txn obj
    Txn *t = new Txn(&gt);
    assert(txns.count(t->txnid) == 0);
    txns[t->txnid] = t;
    
    // request locks
    KEY r;
        
    for(int j = 0; j < t->wsetsize; j++) {
        if(part((r = t->wset[j])) == PART) {
            if(!locks.count(r))
                locks[r] = new Lock();
            locks[r]->lock(t);
        }
    }

    
    // start txn
    if(t->lockwaits == 0)
        t->run();
}

// process a new message, which is intermediate state from another partition's lock manager;
// return a ptr to the txn the message referred to; NULL if no such txn exists
Txn *processNewMsg(Message m) {
    if(!txns.count(m.txnid)) {
        // txn hasn't been initialized yet at this partition
        oldmessages.enqueue(m);
        return NULL;
    }
    Txn *t = txns[m.txnid];
    unflattenmap(&(t->reads), m.size, (int64_t *)(m.data));
    free(m.data);
    t->messagewaits--;
    return t;
}

// process transaction result returned from the storage layer
// (this function's calling signature may have to change)
void processCompletedTxn(Txn *t) {
    if(!t->mp || t->readphasedone) {
        // txn is done; release locks, clean up, and (TODO) send ack to client
        KEY r;

        for(int j = 0; j < t->wsetsize; j++) {
            if(part((r = t->wset[j])) == PART) {
                locks[r]->unlock(t);
                if(locks[r]->owner == NULL) {
                    delete locks[r];
                    locks.erase(r);
                }
            }
        }
        
        txns.erase(t->txnid);
        delete t;

    } else {
        // mp txn, just finished read phase
        t->readphasedone = true;

        // compute msg recips
        bool recips[NPARTS];
        for(int i = 0; i < t->wsetsize; i++) {
            int p = part(t->wset[i]);
            assert(p >= 0 && p < NPARTS);
            recips[p] = true;
        }
        
        // send messages to other partitions
        Message m;
        m.sender = PART;
        flattenmap(&(t->reads), &(m.size), (int64_t **) &(m.data));
        for(int i = 0; i < NPARTS; i++) {
            if(recips[i]) {
                m.recipient = i;
                connection->SendDBMsg((const Message &) m);
            }
        }
        free(m.data);
        
        // deliver any old messages
        int l = oldmessages.size();
        for(int i = 0; i < l; i++) {
            Message m = oldmessages.dequeue();
            if(m.txnid == t->txnid)
                processNewMsg(m);
            else
                oldmessages.enqueue(m);
        }
        
        if(t->messagewaits == 0)
            t->run2();
    }
}

void fillIncomingDBTxns() {
    map<KEY, VAL> newtxn;                           // Generic new txn
    KEY i, read_in;
    int c;
    
    i = read_in = 0;
    while (true) {
        if (kbhit() && (c = getch())) {
            if (!isdigit(c)) {
                newtxn[i++] = read_in;              // Whitespace, put in map
                if (c == '\n') {
                    incomingdbtxns.enqueue(newtxn); // Enqueue map
                    i = 0;                          // Reset loading point
                }
                read_in = 0;
            } else {
                read_in *= 10;                      // New digit for read_in
                read_in += c - '0';
            }
        } else
            break;
    }
}



void runtpcc() {
    // Thad: call tpc_c.cc main function from here, e.g.:
    //
          
          if(DEBUG) cout << "STARTED THREAD FOR DATABASE" << endl;
          pthread_create(&tpccthread, NULL, &tpcc_thread, &PART);
    //
    // use incomingdbtxns and outgoingdbtxns to communicate between, e.g. (example taken from line 154 above)
    //
    //      pthread_mutex_lock(&olatch);
    //      outgoingdbtxns.enqueue(this);
    //      pthread_mutex_unlock(&olatch);
}

int main(int argc, char **argv) {
    ginits(atoi(argv[2]), argv[1]);

    PART = atoi(argv[1]);
    
    if(DEBUG) cout << "PROGRAM STARTED" << endl << flush;
    runtpcc();
    
    int count = 0;
    int active = 0;
    while(true) {

        /* THE FOLLOWING CODE IS MEANT TO TEST DIRECTLY */
        
//        while(active < 1) {
//            t = generate(j++, 10);
//            incomingtxns.enqueue(*t);
//            active++;
//        }        

        // non-blocking input collection
        connection->FillIncomingMessages(&incomingmsgs);
        connection->FillIncomingTxns(&incomingtxns);

        while(incomingmsgs.size()) {
            Txn *t = processNewMsg(incomingmsgs.dequeue());
            if(t->messagewaits == 0)
                t->run2();
        }
        
        while(incomingtxns.size()) {
            processNewTxn(incomingtxns.dequeue());
        }

        pthread_mutex_lock(&ilatch);
        int s = incomingdbtxns.size();
        pthread_mutex_unlock(&ilatch);
        
        while(s > 0) {
            pthread_mutex_lock(&ilatch);
            map<KEY, VAL> dbtxn = incomingdbtxns.dequeue();
            pthread_mutex_unlock(&ilatch);
            
            Txn *t = txns[dbtxn[0]];                // Txn id
            if(DEBUG) ;cout << "NEW DB TXN INCOMING: " << dbtxn[0] << endl;
            t->reads[0] = dbtxn[1];                 // Populate reads
            processCompletedTxn(t);                 // Process completed db txn
            s--;
            active--;
            count++;
            
            TxnStatus status = COMMITTED;
            Message m(t->txnid_unordered, config->myNodeID,
                      t->source_mediator, sizeof(status), &status);
            connection->SendDBMsg(m);
            
        }

        double tm = tim()+0.01;
        if(tim() > tm) {
            cout << count << "\n" << flush;
            tm = tim() + 0.01;
            count = 0;
        }
    }
    
    free_database();
    exit(EXIT_FAILURE);                         // DB Server Interrupt
}


