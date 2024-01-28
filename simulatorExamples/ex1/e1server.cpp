
#include "e1server.hpp"


E1Server::E1Server(string nodeName): Node(nodeName){
    // note this is the same as installng the service software on the server
    // it doesn't run yet until the node is started.

    cout << "Main: Server " << nodeName << " adding example 1 service" << endl;

    shared_ptr<Service> E1Service = make_shared<E1ServiceServer>(nodeName,weak_from_this());
    addService(E1Service);
}
