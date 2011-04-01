


class Node {
public:
    int nodeID; //globally unique
    string host;
};

class DBNode : public Node {
public:
    int replicaID;
    int partitionID;
    int port_for_new_txns;
    int port_for_txn_msgs;
};

class PreprocessorNode : public Node {
public:
    int replicaID;
    int partitionID;
    int port;
};

class MediatorNode : public Node {
public:
    int port;
};


map<int,Node *> allnodes;
vector<MediatorNode *> mediators;
map<int,vector<DBNode *> > dbreplicas;
map<int,vector<DBNode *> > dbpartitions;
map<int,vector<PreprocessorNode *> > preprocessorreplicas;
map<int,vector<PreprocessorNode *> > preprocessorpartitions;
