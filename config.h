#ifndef _YALE_SCALE_CONFIG_H_
#define _YALE_SCALE_CONFIG_H_ 1

#include <stdint.h>

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

enum NodeType{
    DB_NODE, PREPROCESSOR_NODE, MEDIATOR_NODE,
};

struct Node {
    int nodeID;  // globally unique
    NodeType nodeType;
    string host;
    int cores;   // How many cores on this machine
};

struct DBNode : public Node {
    int replicaID;
    int partitionID;
    int port_for_new_txns;
    int port_for_txn_msgs;
};

struct PreprocessorNode : public Node {
    int replicaID;
    int partitionID;
    int port;
};

struct MediatorNode : public Node {
    int partitionID;
    int port;
};

class Configuration{
  public:
    explicit Configuration(int node_id);
    Configuration(int node_id, const string& filename);

    int partitionContainingRecord(int64_t x) const;

    int64_t myNodeID;
    int64_t wait_time; // in microsecond

    map<int, Node*> allnodes;
    map<int, MediatorNode*> mediators;
    map<int, vector<DBNode*> > dbreplicas;   // dbreplicas[x] returns a vector of all partitions within replica x
    map<int, vector<DBNode*> > dbpartitions; // dbpartitions[x] returns a vector of nodes that are partition x for some replica
    map<int, vector<PreprocessorNode*> > preprocessorreplicas;
    map<int, vector<PreprocessorNode*> > preprocessorpartitions;

    map<string, string> other_opt;

    virtual ~Configuration(){}

  protected:
    int ReadFromFile(const string& filename);
    virtual void ProcessConfigLine(char key[], char value[]);
};

#endif  // !defined(_YALE_SCALE_CONFIG_H_)
