#ifndef _YALE_SCALE_MEDIATOR_SERVER_H_
#define _YALE_SCALE_MEDIATOR_SERVER_H_ 1

#include <stdint.h>

#include <map>
#include <vector>
#include <ext/hash_map>
#include <ext/hash_set>

#include "../message.h"

using std::map;
using std::vector;
using __gnu_cxx::hash_map;
using __gnu_cxx::hash_set;

class Configuration;
class GenericTxn;
class RemoteConnection;

namespace Mediator {
class MediatorServer{
  public:
    MediatorServer(Configuration& config);

    int StartServer();

  private:
    MediatorServer(const MediatorServer&);
    MediatorServer& operator=(const MediatorServer&);

    int ProcessTxn(const GenericTxn& txn);
    void ProcessMsg(const Message& msg);
    void ProcessResult(const Message& msg);
    void AddDurableIds(const Message& msg);
    void SendMsg(const Message& msg);

    Configuration& _config;
    RemoteConnection& _remote;
    int64_t _next_txnid;
    int _next_partition;
    int _result_count;

    // Type: txnid_unordered -> fd
    hash_map<int64_t, int> _txn_origin;

    // Type: txnid_unordered -> txn response
    hash_map<int, Message> _pending_msgs;

    // Type: txnid_unordered
    hash_set<int64_t> _durable;
};
}

#endif  // !defined(_YALE_SCALE_MEDIATOR_SERVER_H_)
