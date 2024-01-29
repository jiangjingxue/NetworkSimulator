#ifndef __CLIENT_HPP_
#define __CLIENT_HPP_
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <atomic>
#include <sys/socket.h>
#include <arpa/inet.h>

#ifdef __APPLE__
#define MSG_CONFIRM 0
#endif

#include "dumpHex.hpp"

#include "network.hpp"

using namespace std;

class RPCClient: public Node {

public:
    RPCClient(string name): Node(name){}
    ~RPCClient(){
        stop();
    }
    void start();
};


#endif