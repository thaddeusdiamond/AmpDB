#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <cstring>
#include <stdint.h>

#include <ext/hash_map>

using __gnu_cxx::hash_map;

class Message {
public:
    static const int FIXED_HEADER_SIZE = 4;
    static const int DATASIZE_IDX = 3;

    Message(){}

    Message(int64_t i, int64_t snd, int64_t rec, int64_t sz, void *dt)
        : txnid(i), sender(snd), recipient(rec), size(sz) {
        data = malloc(sz);
        memcpy(data, dt, sz);
    }
    int64_t txnid;
    int64_t sender;
    int64_t recipient;
    int64_t size;
    void* data;
};




#endif // (#ifndef _MESSAGE_H_)
