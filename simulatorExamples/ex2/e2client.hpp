
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

class E2Client: public Node {

public:
    E2Client(string name): Node(name){
    }
    ~E2Client(){
        stop();
    }
    void start();
};


#endif

