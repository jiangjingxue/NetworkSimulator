
#include "e2server.hpp"


E2Server::E2Server(string nodeName): Node(nodeName){
    // note this is the same as installng the service software on the server
    // it doesn't run yet until the node is started.

    cout << "Main: Server " << nodeName << " adding example 2 service" << endl;

    // c++ complains if you use make_shared as a parameter for a constructor
    shared_ptr<Service> E2Service = make_shared<E2ServiceServer>(nodeName,weak_from_this());
    addService(E2Service);
}
