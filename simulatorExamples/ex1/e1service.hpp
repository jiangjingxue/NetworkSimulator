

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <map>
#include <atomic>

#include "dumpHex.hpp"
#include "network.hpp"

using namespace std;


class E1ServiceServer: public Service{

    // network management
    int sockfd;
    in_port_t PORT = 8080;
    static const int MAXMSG = 1400;
    uint8_t udpMessage[MAXMSG];

public:
    E1ServiceServer(string nodeName, weak_ptr<Node> p):Service(nodeName+".ex_1_service",p) {}
    ~E1ServiceServer(){
	stop();
    }
    void start();
};

#endif
