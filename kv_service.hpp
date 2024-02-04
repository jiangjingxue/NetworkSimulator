// kv_service.hpp

#ifndef _KV_SERVICE_HPP_
#define _KV_SERVICE_HPP_

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <map>
#include <atomic>
#include <gdbm.h>
#include <chrono>

#include "network.hpp"
#include "kv.pb.h"

using namespace std;


class KVServiceServer: public Service{
    // network management
    int sockfd;
    in_port_t PORT = 8080;
    static const int MAXMSG = 1400;
    uint8_t udpMessage[MAXMSG];
    uint8_t udpReply[MAXMSG];

    string dbname;
    GDBM_FILE db;
    int RPCFunction;
    bool putStatus;
    struct kvGetResult{
        bool status;
        unique_ptr<uint8_t[]> value;
        uint16_t vlen;
    };
    kvGetResult returnResult;

public:
    KVServiceServer(string nodeName, weak_ptr<Node> p) : Service(nodeName + ".kv_service", p) {
        // Initialize db to nullptr in the constructor
        db = nullptr;
    }
    ~KVServiceServer(){
        stop();
    }

    void start();
    void setDatabaseName(string dbName);
    uint64_t handleRPC(uint8_t* serialized_message, int n);
    bool kvPut(int32_t key, unique_ptr<uint8_t[]> &value, uint16_t vlen);
    kvGetResult kvGet(int32_t key);

};

#endif

