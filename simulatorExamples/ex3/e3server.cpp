
#include "e3server.hpp"


E3Server::E3Server(string nodeName): Node(nodeName){
    // note this is the same as installng the service software on the server
    // it doesn't run yet until the node is started.

    cout << "Main: Server " << nodeName << " adding example 3 service" << endl;

    // c++ complains if you use make_shared as a parameter for a constructor
    shared_ptr<Service> E3Service =  make_shared<E3ServiceServer>(nodeName,weak_from_this());
    addService(E3Service);
}
