// client.hpp

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>
#include <atomic>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "network.hpp"

#include "client_stub.hpp"
#include "kv.pb.h"

using namespace std;

#define close mclose
void mclose(int fd);

class Client : public Node {
    string servAddr;

    // crean an stub object dynamically 
    // so the second client won't retain data from the instantiation of previous clients 
    ClientStub clientStub;

public:
    Client(string name) : Node(name) {
    }
    ~Client() {
        stop();
    }

    void start();
    string getClientAddress();
    void setServerAddress(string addr);

};

#endif

