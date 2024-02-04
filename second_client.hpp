// second_client.hpp

#ifndef SECOND_CLIENT_HPP
#define SECOND_CLIENT_HPP

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

class SecondClient : public Node {
    string servAddr;

    // crean an stub object dynamically 
    // so the second client won't retain data from the instantiation of previous clients 
    ClientStub clientStub;

public:
    SecondClient(string name) : Node(name) {
    }
    ~SecondClient() {
        stop();
    }

    void start();
    string getClientAddress();
    void setServerAddress(string addr);

};

#endif