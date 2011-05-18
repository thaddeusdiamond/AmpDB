#include "config.h"

#include <cstdio>
#include <cstring>
#include <limits>

namespace Preproc{
Configuration::Configuration(int node_id)
    : ::Configuration(node_id),
      instances(1), instance_number(0),
      batch_size(8192),
      binlog_filename("binlog"),
      scheduler_type(ECHO){}

Configuration::Configuration(int node_id, const string& filename)
    : ::Configuration(node_id),
      instances(1), instance_number(0),
      batch_size(8192),
      binlog_filename("binlog"),
      scheduler_type(ECHO){
    ::Configuration::ReadFromFile(filename);
}

void Configuration::ProcessConfigLine(char key[], char value[]){
    if(strcmp(key, "batch_size") == 0){
        batch_size = atoi(value);
    }else if(strcmp(key, "scheduler") == 0){
        if(strcmp(value, "echo") == 0)
            scheduler_type = ECHO;
        else if(strcmp(value, "random") == 0)
            scheduler_type = RANDOM;
        else if(strcmp(value, "density") == 0)
            scheduler_type = DENSITY;
        else
            printf("Unkown scheduler type: %s\n", value);
    }else if(strcmp(key, "binlog") == 0){
        binlog_filename = value;
    // Multiple instances on single machine disabled
    // }else if(strcmp(key, "instances") == 0){
    //     instances = atoll(value);
    }else
        ::Configuration::ProcessConfigLine(key, value);
}
}
