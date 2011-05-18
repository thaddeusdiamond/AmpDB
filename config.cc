#include "config.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

#include <netdb.h>
#include <netinet/in.h>

// #define VERBOSE 1

using std::string;

Configuration::Configuration(int node_id)
    : myNodeID(node_id), wait_time(1000), execution_mode(MODE_DETERMINISTIC){
}

Configuration::Configuration(int node_id, const string& filename)
    : myNodeID(node_id), wait_time(1000), execution_mode(MODE_DETERMINISTIC){
    if(ReadFromFile(filename))
        exit(0);
}

int Configuration::partitionContainingRecord(int64_t x) const{
    return (((uint64_t)x) >> 48) % dbpartitions.size();
}

int Configuration::ReadFromFile(const string& filename){
    char buf[1024];
    FILE* fp = fopen(filename.c_str(), "r");
    if(fp == NULL){
        printf("Cannot open config file %s\n", filename.c_str());
        return -1;
    }
    char* tok;
    while(fgets(buf, sizeof(buf), fp)){
        char* p = buf;
        while(isspace(*p))
            ++p;
        if(*p == '#' || *p == '\0')
            continue;
        char* key = strtok_r(p, "=\n", &tok);
        char* value = strtok_r(NULL, "=\n", &tok);
        ProcessConfigLine(key, value);
    }
    fclose(fp);
    return 0;
}

void Configuration::ProcessConfigLine(char key[], char value[]){
    if(strcmp(key, "wait_time") == 0){
        wait_time = atoll(value);
    }else if(strcmp(key, "mode") == 0){
        if(strcmp(value, "deterministic") == 0)
            execution_mode = MODE_DETERMINISTIC;
        else if(strcmp(value, "traditional") == 0)
            execution_mode = MODE_TRADITIONAL;
        else
            printf("Unknown execution mode: %s\n", value);
    }else if(strncmp(key, "node", 4) != 0){
#if VERBOSE
        printf("Unknown key in config file: %s\n", key);
#endif
        other_opt.insert(std::pair<string, string>(
                    key, value));
    }else{
        int node_id = atoi(key + 4);
        char* tok;

        const char* type      =      strtok_r(value, ":", &tok);
        int         cores     = atoi(strtok_r(NULL, ":", &tok));
        int         replica   = atoi(strtok_r(NULL, ":", &tok));
        int         partition = atoi(strtok_r(NULL, ":", &tok));
        const char* host      =      strtok_r(NULL, ":", &tok);
        int         port      = atoi(strtok_r(NULL, ":", &tok));

        string ip;
        {  // translate hostnames to IPs
            struct hostent* ent = gethostbyname(host);
            if(ent == NULL){
                ip = host;
            }else{
                uint32_t n;
                char buf[32];
                memmove(&n, ent->h_addr_list[0], ent->h_length);
                n = ntohl(n);
                snprintf(buf, sizeof(buf), "%u.%u.%u.%u",
                        n >> 24, (n >> 16) & 0xff,
                        (n >> 8) & 0xff, n & 0xff);
                ip = buf;
            }
        }

        if(cores < 1)     cores = 1;
        if(replica < 0)   replica = 0;
        if(partition < 0) partition = 0;

        Node* node = 0;
        if(strcmp(type, "db") == 0){
            int port2 = atoi(strtok_r(NULL, ":", &tok));
            DBNode* db = new DBNode();
            node = db;
            db->nodeID = node_id;
            db->nodeType = DB_NODE;
            db->host   = ip;
            db->cores  = cores;
            db->replicaID = replica;
            db->partitionID = partition;
            db->port_for_new_txns = port;
            db->port_for_txn_msgs = port2;
            dbreplicas[replica].push_back(db);
            dbpartitions[partition].push_back(db);
        }else if(strcmp(type, "preprocessor") == 0){
            PreprocessorNode* preproc = new PreprocessorNode();
            node = preproc;
            preproc->nodeID = node_id;
            preproc->nodeType = PREPROCESSOR_NODE;
            preproc->host   = ip;
            preproc->cores  = cores;
            preproc->replicaID = replica;
            preproc->partitionID = partition;
            preproc->port   = port;
            preprocessorreplicas[replica].push_back(preproc);
            preprocessorpartitions[partition].push_back(preproc);
        }else if(strcmp(type, "mediator") == 0){
            MediatorNode* mediator = new MediatorNode();
            node = mediator;
            mediator->nodeID = node_id;
            mediator->nodeType = MEDIATOR_NODE;
            mediator->host   = ip;
            mediator->cores  = cores;
            mediator->port   = port;
            mediator->partitionID = partition;
            mediators[partition] = mediator;
        }else{
#if VERBOSE
            printf("Unknown type of node: %s\n", type);
#endif
        }

        if(node)
            allnodes.insert(std::pair<int, Node*>(node_id, node));
    }
}
