#ifndef _YALE_SCALE_PREPROCESSOR_CONFIG_H_
#define _YALE_SCALE_PREPROCESSOR_CONFIG_H_ 1

#include <string>
#include <vector>
#include <ext/hash_map>

#include "../config.h"

using std::string;
using std::vector;
using __gnu_cxx::hash_map;

namespace Preproc{
enum SchedulerType{
    ECHO, RANDOM, DENSITY,
};

class Configuration : public ::Configuration{
  public:
    explicit Configuration(int node_id);
    Configuration(int node_id, const string& filename);

    int instances;
    int instance_number;

    size_t batch_size;
    string binlog_filename;
    SchedulerType scheduler_type;

  protected:
    virtual void ProcessConfigLine(char key[], char value[]);
};
}

#endif  // !defined(_YALE_SCALE_PREPROCESSOR_CONFIG_H_)
