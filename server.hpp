// server.hpp

#include <gdbm.h>

#include "network.hpp"
#include "kv_service.hpp"

class Server: public Node{
    string dbname;
    shared_ptr<KVServiceServer> KVService;
    shared_ptr<Service> key_value_service;

public:
    Server(string nodeName);
    ~Server(){}
    void setDatabaseName(string dbName);

};

