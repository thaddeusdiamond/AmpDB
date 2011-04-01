#include <cstdio>
#include <cstdlib>

#include "../config.h"
#include "server.h"

int main(int argc, char* argv[]){
    if(argc < 3){
        printf("Usage: %s config_file node_id\n", argv[0]);
        return -1;
    }

    setlinebuf(stdout);
    setlinebuf(stderr);
    Configuration config(atoi(argv[2]), argv[1]);
    Mediator::MediatorServer server(config);
    if(config.allnodes[config.myNodeID]->nodeType != MEDIATOR_NODE){
        printf("Our node is not mediator in config file\n");
        return -1;
    }
    server.StartServer();
    return 0;
}
