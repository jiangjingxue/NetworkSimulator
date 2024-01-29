#include "rpcserver.hpp"

RPCServer::RPCServer(string nodeName) : Node(nodeName) {
    // debug message
    cout << "Main: Server " << nodeName << " adding key value service" << endl;

    // create an object of the the key value service

    shared_ptr<Service> key_value_service = make_shared<KeyValueServiceServer>(nodeName,weak_from_this());

    // set file name using dynamic casting (offer runtime checking)
    string name = "gdbm_placeholder";
    shared_ptr<KeyValueServiceServer> key_value_service_server = dynamic_pointer_cast<KeyValueServiceServer>(key_value_service);
    key_value_service_server->setFilename(name);
    
    // add the key value service to the RPC(Remote Procedure Call) Server
    // you may add more services to the RPC server as needed
    addService(key_value_service);
}
