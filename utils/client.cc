#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <cmath>
#include <ctime>
#include <csignal>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../global.h"
#include "../generictxn.h"
#include "../config.h"
#include "../message.h"
#include "../tpccload.h"
#include "../loadgen/loadgen.h"

using namespace std;

#define AS_CLIENT
// #define AS_MEDIATOR 100  // mediator nodeID

#define MICRO_MEAN 60
#define MICRO_STD  10

int prepare_sock(const char hostname[], int port, bool mute);
void send_txn(int sock, const GenericTxn& txn);
void receive_result(int sock, Message* msg);
void get_intr(int sig);

int nexttxnid = 1;
int benchmarking = 0;
int nDone = 0;
timespec starting_time;

void Usage(char* argv[]){
    printf("Usage: %s [-m m] config-file number-of-txns\n", argv[0]);
    printf("     | %s [-m m] config-file -b number-of-simultaneous-txns\n", argv[0]);
    printf("     | %s [-m m] config-file -d partitionid\n", argv[0]);
}

int main(int argc, char* argv[]){
    if(argc < 3){
        Usage(argv);
        return -1;
    }

    setlinebuf(stdout);
    setlinebuf(stderr);

    int mediator = 0;
    if(strcmp(argv[1], "-m") == 0){
        mediator = atoi(argv[2]);
        argv += 2;
    }

    Configuration config(-1, string(argv[1]));
    
#ifdef AS_MEDIATOR
    PreprocessorNode* dest = config.preprocessorreplicas[0][0];
#elif defined(AS_CLIENT)
    MediatorNode* dest = config.mediators[mediator];
#endif
    int sock = prepare_sock(dest->host.c_str(), dest->port, false);

#ifdef AS_MEDIATOR
    int64_t node_id = AS_MEDIATOR;
    write(sock, &node_id, sizeof(node_id));  // header
#endif

    GenericTxn *txn;
    
    if(!strcmp(argv[2],"-d")) {
        if(!argv[3]) {
            Usage(argv);
            return -1;
        }
        prepare_txn_DEBUG(&config, &txn, atoi(argv[3]));
        send_txn(sock, txn);
        return 0;
    }
    
    if(!strcmp(argv[2],"-b")) {
        if(!argv[3]) {
            Usage(argv);
            return -1;
        }
        int n = atoi(argv[3]);
        for(int i = 0; i < n; i++) {
            // prepare_txn_MICRO(&config, &txn);
            generatetpcctxn(&txn, config.dbpartitions.size());
            send_txn(sock, txn);
        }

        signal(SIGINT, &get_intr);
        printf("Press ^C to start benchmarking\n");
        while(1){
            Message msg(0, 0, 0, 0, 0);
            receive_result(sock, &msg);
            if(msg.size != sizeof(TxnStatus) ||
                    *reinterpret_cast<TxnStatus *>(msg.data) != COMMITTED){
                printf("Response to %ld failed\n", msg.txnid);
                break;
            }
            free(msg.data);
            ++nDone;

            // prepare_txn_MICRO(&config, &txn);
            txn = generate();
            send_txn(sock, txn);
            delete txn;
        }
        return 0;
    }
    
    int n = atoi(argv[2]);
    for(int i = 0; i < n; i++) {
        prepare_txn_MICRO(&config, &txn);
        send_txn(sock, txn);

        Message msg(0, 0, 0, 0, 0);
        receive_result(sock, &msg);
        if(msg.size == sizeof(TxnStatus) && *reinterpret_cast<TxnStatus *>(msg.data) == COMMITTED)
            printf("Committed txn %ld\n", msg.txnid);

        free(msg.data);
    }
    return 0;
}

int prepare_sock(const char hostname[], int port, bool mute){
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        if(!mute)
            printf("Socket creation failed: %s\n", strerror(errno));
        return -1;
    }

    struct hostent* host = gethostbyname(hostname);
    if(host == NULL){
        if(!mute)
            printf("Cannot resolve host %s: %s\n", hostname, strerror(errno));
        return -1;
    }

    struct sockaddr_in srv_addr;
    srv_addr.sin_family = AF_INET;
    memmove(&srv_addr.sin_addr, host->h_addr_list[0], host->h_length);
    srv_addr.sin_port = htons(port);

    if(connect(sock, reinterpret_cast<const sockaddr*>(&srv_addr),
                     sizeof(srv_addr)) < 0){
        if(!mute)
            printf("Connection to %s refused: %s\n", hostname, strerror(errno));
        return -1;
    }
    return sock;
}



//void prepare_txn_MICRO(GenericTxn* t){
//    t->txnid = nexttxnid++;
//    t->txnid_unordered = t->txnid;
//    t->source_mediator = 0;
//    t->txntype = 0;
//    t->isolationlevel = SERIALIZABLE;
//    t->rsetsize = 5;
//    t->wsetsize = 5;
//    t->argcount = t->rsetsize + t->wsetsize;
//    t->rset = (int64_t *)malloc(        t->rsetsize         * sizeof(int64_t));
//    t->wset = (int64_t *)malloc(        t->wsetsize         * sizeof(int64_t));
//    t->args = (int64_t *)malloc((t->rsetsize + t->wsetsize) * sizeof(int64_t));
//    for(int i = 0; i < t->rsetsize; i++) t->rset[i] = t->args[              i] = gaussian_cell();
//    for(int i = 0; i < t->wsetsize; i++) t->wset[i] = t->args[t->rsetsize + i] = gaussian_cell();
//}

void send_txn(int sock, const GenericTxn& txn){
    int64_t length;
#if defined(AS_MEDIATOR)
    char* buf = reinterpret_cast<char*>(
        malloc((1 + GenericTxn::FIXED_HEADER_SIZE +
                txn.rsetsize + txn.wsetsize + txn.argcount) * sizeof(int64_t)));
    *reinterpret_cast<int64_t*>(buf) = 1;
    txn.serialize_to(buf + sizeof(int64_t), &length);
    length += sizeof(int64_t);
#elif defined(AS_CLIENT)
    char* buf = reinterpret_cast<char*>(txn.serialize(&length));
#endif  // defined(AS_CLIENT)

    int c, n = 0;
    while(n < length && (c = write(sock, buf + n, length - n)) >= 0)
        n += c;

    free(buf);
}

void receive_result(int sock, Message* msg){
    char* buf = reinterpret_cast<char*>(&msg->txnid);
    int length = Message::FIXED_HEADER_SIZE * sizeof(int64_t);
    int c, n = 0;
    while(n < length && (c = read(sock, buf + n, length - n)) > 0)
        n += c;

    msg->data = malloc(msg->size);
    buf = reinterpret_cast<char*>(msg->data);
    length = msg->size;
    n = 0;
    while(n < length && (c = read(sock, buf + n, length - n)) > 0)
        n += c;
}

void get_intr(int sig){
    if(!benchmarking){
        benchmarking = 1;
        nDone = 0;
        printf("Press ^C again to stop\n");
        clock_gettime(CLOCK_MONOTONIC, &starting_time);
    }else{
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        long diff_sec = now.tv_sec - starting_time.tv_sec;
        long diff_nsec = now.tv_nsec - starting_time.tv_nsec;
        if(diff_nsec < 0){
            diff_nsec += 1000000000;
            --diff_sec;
        }
        printf("Finished %d transactions in %ld.%09ld seconds\n",
               nDone, diff_sec, diff_nsec);
        exit(0);
    }
}
