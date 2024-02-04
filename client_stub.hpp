// client_stub.hpp

#ifndef CLIENT_STUB_HPP
#define CLIENT_STUB_HPP

#include <string>
#include <cstdint>
#include <memory>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "network.hpp"
#include "kv.pb.h"

using namespace std;

struct kvGetResult {
    bool status;
    unique_ptr<uint8_t[]> value;
    uint16_t vlen;
};

class ClientStub {
    struct sockaddr_in servaddr;
    struct sockaddr_in servaddrreply;
    int n;
    string servAddr;
    int sockfd;

public:
    ClientStub();
    ~ClientStub() {
    }
    void setServerAddress(string addr);
    bool kvPut(int32_t key, unique_ptr<uint8_t[]> &value, uint16_t vlen);
    kvGetResult kvGet(int32_t key);
    void shutdownNetwork();

};

#endif // CLIENT_STUB_HPP

