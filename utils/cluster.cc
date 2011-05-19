#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <csignal>
#include <iostream>

#include <map>
#include <vector>
#include <sstream>

#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

#include "../config.h"

using std::map;
using std::vector;

enum{ CLIENT_NODE = MEDIATOR_NODE + 1 };

struct ClientNode : public Node {
    int mediatorPartition;
};

class ClusterConfiguration : public Configuration{
  public:
    ClusterConfiguration(int node_id, const string& filename)
        : Configuration(node_id), _hasClient(false){
        if(ReadFromFile(filename))
            exit(0);
    }

    bool HasClient() const{
        return _hasClient;
    }
  protected:
    virtual void ProcessConfigLine(char key[], char value[]);

    bool _hasClient;
};

bool CheckExecutables(char* argv[], const ClusterConfiguration& config);
void RenewPorts(ClusterConfiguration* config);
bool WriteConfig(char filename[], const ClusterConfiguration& config);
void Deploy(const ClusterConfiguration& config, char* argv[]);

const char strExec[] = "ssh";

// Redirects stdin from /dev/null (actually, prevents reading from stdin).
// This must be used when ssh is run in the background.
const char strOpt1[] = "-nT";
// strOpt2 = address
const char strOpt3fmt[] = "%s/%s %s/%s %d";
const char strQuiteOpt3fmt[] = "%s/%s %s/%s %d > /dev/null 2>&1";
const char strValgrindOpt3fmt[] = "valgrind %s/%s %s/%s %d";
// sprintf(strOpt3, strOpt3fmt, Cwd, per-exec, Cwd, config-file, node-id)
const char strClientOpt3fmt[] = "%s/%s -m %d %s/%s -b 5000";
// sprintf(strOpt3, strClientOpt3fmt, Cwd, per-exec, mediatorPartition,
//                                    Cwd, config-file)

char* Cwd;

int argBegin;
bool doValgrind;
bool doQuite;
bool transferInterrupt;
bool doTransferInterrupt;

// Type: fd -> nodeID
map<int, int> ChildernPipes;
vector<int> ChildrenPID;
volatile bool EndCluster;

int main(int argc, char* argv[]){
    for(argBegin = 1; argBegin < argc; ++argBegin){
        if(strcmp(argv[argBegin], "-v") == 0)
            doValgrind = true;
        else if(strcmp(argv[argBegin], "-q") == 0)
            doQuite = true;
        else
            break;
    }

    if(argc < argBegin + 4){
        printf("Usage: %s [-v|-q] config-file db-exec preproc-exec mediator-exec [client-exec]\n",
               argv[0]);
        return -1;
    }

    ClusterConfiguration config(-1, argv[1]);
    Cwd = getcwd(NULL, 0);

    if(CheckExecutables(argv, config)){
        printf("Executables' problem\n");
        return -1;
    }

    RenewPorts(&config);
    if(WriteConfig(argv[1], config)){
        printf("Cannot modify config file: %s\n", strerror(errno));
        return -1;
    }

    Deploy(config, argv);
}

void ClusterConfiguration::ProcessConfigLine(char key[], char value[]){
    if(strncmp(key, "node", 4) != 0 ||
       strncmp(value, "client:", 7) != 0){
        Configuration::ProcessConfigLine(key, value);
        return;
    }
    printf("client %s\n", key);
    int node_id = atoi(key + 4);
    char* tok;

    const char* type      =      strtok_r(value, ":", &tok);
    int         cores     = atoi(strtok_r(NULL, ":", &tok));
    int         replica   = atoi(strtok_r(NULL, ":", &tok));
    int         partition = atoi(strtok_r(NULL, ":", &tok));
    const char* host      =      strtok_r(NULL, ":", &tok);
    int         mediator  = atoi(strtok_r(NULL, ":", &tok));

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

    ClientNode* node = new ClientNode();
    node->nodeID = node_id;
    node->nodeType = static_cast<NodeType>(CLIENT_NODE);
    node->host   = ip;
    node->cores  = cores;
    node->mediatorPartition = mediator;

    allnodes.insert(std::pair<int, Node*>(node_id, node));
    _hasClient = true;
    (void) type;
}

bool CheckExecutable(char* exec){
    struct stat buf;
    if(stat(exec, &buf) < 0){
        printf("Cannot access %s: %s\n", exec, strerror(errno));
        return true;
    }
    if(!S_ISREG(buf.st_mode) || !(buf.st_mode & S_IXUSR)){
        printf("Cannot execute %s\n", exec);
        return true;
    }
    return false;
}

bool CheckExecutables(char* argv[], const ClusterConfiguration& config){
    if(CheckExecutable(argv[argBegin + 1])) return true;
    if(CheckExecutable(argv[argBegin + 2])) return true;
    if(CheckExecutable(argv[argBegin + 3])) return true;
    if(config.HasClient() && CheckExecutable(argv[argBegin + 4])) return true;
    return false;
}

void RenewPorts(ClusterConfiguration* config){
    int port_max = 0;
    for(map<int, Node*>::const_iterator it = config->allnodes.begin();
        it != config->allnodes.end(); ++it)
        switch(static_cast<int>(it->second->nodeType)){
            case DB_NODE: {
                DBNode* node = reinterpret_cast<DBNode*>(it->second);
                if(node->port_for_new_txns > port_max)
                    port_max = node->port_for_new_txns;
                if(node->port_for_txn_msgs > port_max)
                    port_max = node->port_for_txn_msgs;
                break;
            }
            case PREPROCESSOR_NODE: {
                PreprocessorNode* node =
                    reinterpret_cast<PreprocessorNode*>(it->second);
                if(node->port > port_max)
                    port_max = node->port;
                break;
            }
            case MEDIATOR_NODE: {
                MediatorNode* node =
                    reinterpret_cast<MediatorNode*>(it->second);
                if(node->port > port_max)
                    port_max = node->port;
                break;
            }
            case CLIENT_NODE:
                break;
        }

#define INC(P) do{ \
        ++P;       \
        if((P) > 20000) \
            P = 10000;  \
    }while(0)

    INC(port_max);
    for(map<int, Node*>::const_iterator it = config->allnodes.begin();
        it != config->allnodes.end(); ++it)
        switch(static_cast<int>(it->second->nodeType)){
            case DB_NODE: {
                DBNode* node = reinterpret_cast<DBNode*>(it->second);
                node->port_for_new_txns = port_max;
                INC(port_max);
                node->port_for_txn_msgs = port_max;
                INC(port_max);
                break;
            }
            case PREPROCESSOR_NODE: {
                PreprocessorNode* node =
                    reinterpret_cast<PreprocessorNode*>(it->second);
                node->port = port_max;
                INC(port_max);
                break;
            }
            case MEDIATOR_NODE: {
                MediatorNode* node =
                    reinterpret_cast<MediatorNode*>(it->second);
                node->port = port_max;
                INC(port_max);
                break;
            }
            case CLIENT_NODE:
                break;
        }
}

bool WriteConfig(char filename[], const ClusterConfiguration& config){
    FILE* fp = fopen(filename, "w");
    if(fp == NULL)
        return true;
    for(map<int, Node*>::const_iterator it = config.allnodes.begin();
        it != config.allnodes.end(); ++it){
        if(it->second->host.compare("localhost") == 0 ||
           it->second->host.compare("127.0.0.1") == 0)
            printf("WARNING: hostname localhost may not work accross network\n");
        switch(static_cast<int>(it->second->nodeType)){
            case DB_NODE: {
                DBNode* node = reinterpret_cast<DBNode*>(it->second);
                fprintf(fp, "node%d=db:%d:%d:%d:%s:%d:%d\n",
                        it->first, node->cores, node->replicaID,
                        node->partitionID, node->host.c_str(),
                        node->port_for_new_txns,
                        node->port_for_txn_msgs);
                break;
            }
            case PREPROCESSOR_NODE: {
                PreprocessorNode* node =
                    reinterpret_cast<PreprocessorNode*>(it->second);
                fprintf(fp, "node%d=preprocessor:%d:%d:%d:%s:%d\n",
                        it->first, node->cores, node->replicaID,
                        node->partitionID, node->host.c_str(),
                        node->port);
                break;
            }
            case MEDIATOR_NODE: {
                MediatorNode* node =
                    reinterpret_cast<MediatorNode*>(it->second);
                fprintf(fp, "node%d=mediator:%d:0:%d:%s:%d\n",
                        it->first, node->cores,
                        node->partitionID, node->host.c_str(),
                        node->port);
                break;
            }
            case CLIENT_NODE: {
                ClientNode* node = reinterpret_cast<ClientNode*>(it->second);
                fprintf(fp, "node%d=client:%d:0:0:%s:%d\n",
                        it->first, node->cores, node->host.c_str(),
                        node->mediatorPartition);
                break;
            }
        }
    }
    fprintf(fp, "wait_time=%ld\n", config.wait_time);
    for(map<string, string>::const_iterator it = config.other_opt.begin();
        it != config.other_opt.end(); ++it)
        if(it->first != "binlog" || it->second.compare(0, 1, "/") == 0)
            fprintf(fp, "%s=%s\n", it->first.c_str(), it->second.c_str());
        else
            fprintf(fp, "binlog=%s/%s\n", Cwd, it->second.c_str());
    if(config.other_opt.find("binlog") == config.other_opt.end())
        fprintf(fp, "binlog=%s/binlog\n", Cwd);
    fprintf(fp, "mode=%s\n",
            config.execution_mode == MODE_DETERMINISTIC ?
            "deterministic" : "traditional");
    fclose(fp);
    return false;
}

void DeployOne(int nodeID, const Node* node, char* argv[]){
    const char* strOpt2 = node->host.c_str();

    const char* exec = 0;
    switch(static_cast<int>(node->nodeType)){
        case DB_NODE:           exec = argv[argBegin + 1]; break;
        case PREPROCESSOR_NODE: exec = argv[argBegin + 2]; break;
        case MEDIATOR_NODE:     exec = argv[argBegin + 3]; break;
        case CLIENT_NODE:       exec = argv[argBegin + 4]; break;
        default: exec = ""; break;
    }
    char strOpt3[1024];
    if(static_cast<int>(node->nodeType) == CLIENT_NODE)
        snprintf(strOpt3, sizeof(strOpt3), strClientOpt3fmt,
                 Cwd, exec,
                 reinterpret_cast<const ClientNode*>(node)->mediatorPartition,
                 Cwd, argv[1]);
    else if(static_cast<int>(node->nodeType) == DB_NODE && doValgrind)
        snprintf(strOpt3, sizeof(strOpt3), strValgrindOpt3fmt,
                 Cwd, exec, Cwd, argv[1], nodeID);
    else if(doQuite)
        snprintf(strOpt3, sizeof(strOpt3), strQuiteOpt3fmt,
                 Cwd, exec, Cwd, argv[1], nodeID);
    else
        snprintf(strOpt3, sizeof(strOpt3), strOpt3fmt,
                 Cwd, exec, Cwd, argv[1], nodeID);

    int pipefd[2];
    pipe(pipefd);

    int pid = fork();
    if(pid == 0){
        setsid();
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        dup2(pipefd[1], 2);
        close(pipefd[1]);
        execlp("ssh", "ssh", strOpt1, strOpt2, strOpt3, NULL);
        printf("Node %d spawning failed\n", nodeID);
        exit(-1);
    }else if(pid < 0){
        printf("Node %d forking failed\n", nodeID);
    }else{
        ChildrenPID.push_back(pid);
        ChildernPipes.insert(std::pair<int, int>(pipefd[0], nodeID));
        close(pipefd[1]);
    }

    timespec to_sleep = { 0, 100000000 };  // 0.1 sec
    nanosleep(&to_sleep, NULL);
}

void Kill(const ClusterConfiguration& config, char* argv[], bool client_int){
    char exec[1024];

    const char* sigArg;
    if(client_int)
        sigArg = "-INT";
    else
        sigArg = "-TERM";

    for(map<int, Node*>::const_iterator it = config.allnodes.begin();
        it != config.allnodes.end(); ++it){
        Node* node = it->second;
        if(client_int && static_cast<int>(node->nodeType) != CLIENT_NODE)
            continue;

        switch(static_cast<int>(node->nodeType)){
            case DB_NODE:           snprintf(exec, sizeof(exec),"%s/%s",
                                             Cwd, argv[argBegin + 1]); break;
            case PREPROCESSOR_NODE: snprintf(exec, sizeof(exec),"%s/%s",
                                             Cwd, argv[argBegin + 2]); break;
            case MEDIATOR_NODE:     snprintf(exec, sizeof(exec),"%s/%s",
                                             Cwd, argv[argBegin + 3]); break;
            case CLIENT_NODE:       snprintf(exec, sizeof(exec),"%s/%s",
                                             Cwd, argv[argBegin + 4]); break;
            default: exec[0] = 0; break;
        }
        if(exec[0]){
            int pid = fork();
            if(pid == 0){
                execlp("ssh", "ssh", node->host.c_str(),
                       "killall", sigArg, exec, NULL);
                exit(-1);
            }
        }
    }
}

void TerminatingChildren(int sig);

void Deploy(const ClusterConfiguration& config, char* argv[]){
    for(map<int, Node*>::const_iterator it = config.allnodes.begin();
        it != config.allnodes.end(); ++it){
        if(it->second->nodeType == DB_NODE)
            DeployOne(it->first, it->second, argv);
    }
    for(map<int, Node*>::const_iterator it = config.allnodes.begin();
        it != config.allnodes.end(); ++it){
        if(it->second->nodeType == PREPROCESSOR_NODE &&
           reinterpret_cast<PreprocessorNode*>(it->second)->replicaID != 0)
            DeployOne(it->first, it->second, argv);
    }
    for(map<int, Node*>::const_iterator it = config.allnodes.begin();
        it != config.allnodes.end(); ++it){
        if(it->second->nodeType == PREPROCESSOR_NODE &&
           reinterpret_cast<PreprocessorNode*>(it->second)->replicaID == 0)
            DeployOne(it->first, it->second, argv);
    }
    string access;
    {
        std::ostringstream stream;
        stream << "Public access (host port):\n";
        for(map<int, Node*>::const_iterator it = config.allnodes.begin();
                it != config.allnodes.end(); ++it){
            if(it->second->nodeType == MEDIATOR_NODE){
                DeployOne(it->first, it->second, argv);
                stream << "  " << it->second->host << " "
                    << (reinterpret_cast<MediatorNode*>(it->second)->port)
                    << '\n';
            }
        }
        access = stream.str();
    }
    for(map<int, Node*>::const_iterator it = config.allnodes.begin();
        it != config.allnodes.end(); ++it){
        if(static_cast<int>(it->second->nodeType) == CLIENT_NODE)
            DeployOne(it->first, it->second, argv);
    }
#if VERBOSE
    int linecount = 0;
    printf("%s", access.c_str());
#endif

    EndCluster = false;
    signal(SIGINT, &TerminatingChildren);
    signal(SIGTERM, &TerminatingChildren);
    signal(SIGPIPE, &TerminatingChildren);

    int num_fd = ChildernPipes.size();
    int max_fd = 0;
    fd_set readset, fds;
    FD_ZERO(&fds);
    for(map<int, int>::const_iterator it = ChildernPipes.begin();
        it != ChildernPipes.end(); ++it){
        if(it->first > max_fd)
            max_fd = it->first;
        FD_SET(it->first, &fds);
    }
    ++max_fd;

    char buf[4096];
    while(num_fd > 0){
        if(EndCluster){
            Kill(config, argv, false);
            EndCluster = false;
        }

        readset = fds;
        int actions = select(max_fd, &readset, NULL, NULL, NULL);
        if(actions == -1){
            if(errno != EINTR)
                break;
            if(doTransferInterrupt){
                Kill(config, argv, true);
                doTransferInterrupt = false;
                transferInterrupt = false;
            }
            continue;
        }

        vector<int> erasing;
        for(map<int, int>::const_iterator it = ChildernPipes.begin();
            it != ChildernPipes.end(); ++it){
            if(FD_ISSET(it->first, &readset)){
                int n;
                if((n = read(it->first, buf, sizeof(buf))) <= 0){
                    erasing.push_back(it->first);
                }else{
                    buf[n] = 0;

                    if(config.HasClient() && !transferInterrupt &&
                       strncmp(buf, "Press ^C", 8) == 0){
                        transferInterrupt = true;
                    }

                    char* p = strtok(buf, "\n");
                    do{
                        printf("%02d: %s\n", it->second, p);
#if VERBOSE
                        if(++linecount == 20){
                            printf("%s", access.c_str());
                            linecount = 0;
                        }
#endif  // VERBOSE
                        p = strtok(NULL, "\n");
                    }while(p);
                }
            }
        }
        if(erasing.size() > 0){
            for(vector<int>::const_iterator it = erasing.begin();
                it != erasing.end(); ++it){
                ChildernPipes.erase(*it);
                FD_CLR(*it, &fds);
            }
            num_fd -= erasing.size();
        }
    }

    Kill(config, argv, false);

    timespec to_sleep = { 1, 0 };  // 1 sec
    nanosleep(&to_sleep, NULL);
    for(vector<int>::const_iterator it = ChildrenPID.begin();
        it != ChildrenPID.end(); ++it)
        kill(*it, SIGTERM);
}

void TerminatingChildren(int sig){
    if(sig == SIGINT && transferInterrupt){
        doTransferInterrupt = true;
        return;
    }
    EndCluster = true;
}

