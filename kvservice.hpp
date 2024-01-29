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


class KeyValueServiceServer: public Service{
public:
    KeyValueServiceServer(string nodeName, weak_ptr<Node> p):Service(nodeName + ".key_value_service",p) { }
    ~KeyValueServiceServer(){
	stop();
    }
    void start();
    void setFilename(string name);

private:
    // network management
    int sockfd;
    in_port_t PORT = 8080;
    static const int MAXMSG = 1400;
    uint8_t udpMessage[MAXMSG];
    uint8_t udpReply[MAXMSG];

    // gdbm database
    string gdbm_filename_;

};

#endif