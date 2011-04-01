#include <cstdio>
#include <cstdlib>
#include <csignal>

#include "config.h"
#include "server.h"

int main(int argc, char* argv[]){
    if(argc < 3){
        printf("Usage: %s config_file node_id\n", argv[0]);
        return -1;
    }

    setlinebuf(stdout);
    setlinebuf(stderr);

    signal(SIGINT, &exit);
    signal(SIGTERM, &exit);
    signal(SIGPIPE, &exit);

    Preproc::Configuration config(atoi(argv[2]), argv[1]);
    Preproc::PreprocessorServer server(config);
    server.StartServer();
    return 0;
}
