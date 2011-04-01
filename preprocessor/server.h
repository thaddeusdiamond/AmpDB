#ifndef _YALE_SCALE_PREPROCESSOR_SERVER_H_
#define _YALE_SCALE_PREPROCESSOR_SERVER_H_ 1

#include <vector>

#include "txninfo.h"

using std::vector;

class RemoteConnection;

namespace Preproc {
class Configuration;

class ServerStrategy{
  public:
    ServerStrategy(const Configuration& config,
                   RemoteConnection& remote);
    virtual ~ServerStrategy(){}

    virtual void RunServer() = 0;

  protected:
    void RotateBinlog();

    const Configuration& _config;
    RemoteConnection& _remote;
    int _binlog_count;
    int _binlog_fd;
};

class PreprocessorServer{
  public:
    explicit PreprocessorServer(Configuration& config);

    int StartServer();

  private:
    PreprocessorServer(const PreprocessorServer&);
    PreprocessorServer& operator=(const PreprocessorServer&);

    Configuration& _config;
    RemoteConnection& _remote;
    ServerStrategy* _strategy;
};
}

#endif  // !defined(_YALE_SCALE_PREPROCESSOR_SERVER_H_)
