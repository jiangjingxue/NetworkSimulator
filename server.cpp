// server.cpp

#include "server.hpp"

Server::Server(string nodeName): Node(nodeName){
    
    cout << nodeName << ":" << " adding KV service" << endl;
    
    // create an object of the the key value service
    key_value_service = make_shared<KVServiceServer>(nodeName,weak_from_this());

    // c++ complains if you use make_shared as a parameter for a constructor
    addService(key_value_service);
}

void Server::setDatabaseName(string dbName){
    
    dbname = dbName;

    shared_ptr<KVServiceServer> key_value_service_server = dynamic_pointer_cast<KVServiceServer>(key_value_service);
    // Call setDatabaseName(dbName) from KVServiceServer class
    key_value_service_server->setDatabaseName(dbName);
}

