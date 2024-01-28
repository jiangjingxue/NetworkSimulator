

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


class E3ServiceServer: public Service{

    // network management
    int sockfd;
    in_port_t PORT = 8080;
    static const int MAXMSG = 1400;
    uint8_t udpMessage[MAXMSG];
    uint8_t udpReply[MAXMSG];

public:
    E3ServiceServer(string name, weak_ptr<Node> p):Service(name + ".example_3_service",p) {}
    ~E3ServiceServer(){
	stop();
    }
    void start();
};

#endif
