#include <cstdio>

#include "../config.h"
#include "../remote.h"
#include "../generictxn.h"
#include "../global.h"
#include "../message.h"
#include "../circularbuffer.h"

// #define VERBOSE 1

#if EXTRA_TXN
#define REDUCE_NETWORK_TRAFFIC if(++c % (EXTRA_TXN + 1) != 0) continue; else
#else
#define REDUCE_NETWORK_TRAFFIC if(0) ; else
#endif

void DumpTxn(const GenericTxn& txn, const Configuration& config){
    printf("|- ID %ld; orig %ld\n", txn.txnid, txn.txnid_unordered);
    printf("|- from %ld; type %d; iso %d\n",
           txn.source_mediator, txn.txntype, txn.isolationlevel);
    printf("|- rsetsize %ld; wsetsize %ld; argcount %ld\n",
           txn.rsetsize, txn.wsetsize, txn.argcount);

    for(int i = 0; i < txn.rsetsize; ++i){
        if(i % 4 == 0)
            printf("r-");
        printf(" %20ld", txn.rset[i]);
        if(i % 4 == 3)
            putchar('\n');
    }
    if(txn.rsetsize % 4 != 0)
        putchar('\n');
    for(int i = 0; i < txn.wsetsize; ++i){
        if(i % 4 == 0)
            printf("w-");
        printf(" %20ld", txn.wset[i]);
        if(i % 4 == 3)
            putchar('\n');
    }
    if(txn.wsetsize % 4 != 0)
        putchar('\n');
    for(int i = 0; i < txn.argcount; ++i){
        if(i % 4 == 0)
            printf("a-");
        printf(" %20ld", txn.args[i]);
        if(i % 4 == 3)
            putchar('\n');
    }
    if(txn.argcount % 4 != 0)
        putchar('\n');
    putchar('\n');
}

void DumpMsg(const Message& msg, const Configuration& config){
    printf("|- ID %ld; from %ld; to %ld\n",
           msg.txnid, msg.sender, msg.recipient);
    printf("|- size %ld\n", msg.size);

    const char* data = reinterpret_cast<const char*>(msg.data);
    int64_t limit = msg.size > 128 ? 128 : msg.size;
    for(int i = 0; i < limit; ++i){
        if(i % 32 == 0)
            printf("|%04x:", i);
        if(i % 2 == 0)
            putchar(' ');
        printf("%02x", data[i]);
        if(i % 32 == 31)
            putchar('\n');
    }
    if(limit % 32 != 0)
        putchar('\n');
    putchar('\n');
}

int main(int argc, char* argv[]){
    if(argc < 3){
        printf("Usage: %s config_file node_id\n", argv[0]);
        return -1;
    }
#if EXTRA_TXN
    int c = 0;
#endif
#if VERBOSE
    int count = 0;
#endif

    setlinebuf(stdout);
    setlinebuf(stderr);
    Configuration config(atoi(argv[2]), argv[1]);
    RemoteConnection* remote = RemoteConnection::GetInstance(config);
    while(1){
        CircularBuffer<GenericTxn> txns;
        remote->FillIncomingTxns(&txns, -1);
#if VERBOSE
            if(++count % 100 == 0){
                printf("Waiting (%d): got %lu\n", getpid(), txns.size());
            }
#endif
        if(txns.size() > 0){
            for(CircularBuffer<GenericTxn>::iterator it = txns.begin();
                it != txns.end(); ++it)
                REDUCE_NETWORK_TRAFFIC{
                    // DumpTxn(*it, config);

                    TxnStatus status = COMMITTED;
                    Message m(it->txnid_unordered, config.myNodeID,
                              it->source_mediator, sizeof(status), &status);
                    while(remote->SendDBMsg(m) < 0){
		    }
                    free(m.data);
                }
        }

        /*
        deque<Message> msgs;
        remote->FillIncomingMessages(&msgs);
        if(msgs.size() > 0){
            printf("%lu msg\n", msgs.size());
            for(deque<Message>::const_iterator it = msgs.begin();
                it != msgs.end(); ++it)
                DumpMsg(*it, config);
        }
        */
    }
    return 0;
}
