#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <string>

#include "kv.pb.h"
#include "network.hpp"
#include "rpcserver.hpp"
#include "rpcclient.hpp"

extern std::map<std::thread::id,shared_ptr<Node>> nodes;
extern std::map<std::thread::id,string> names;

int main(int argc, char * argv[]){
    // handle command line arguments...
    int res = network_init(argc, argv);
    std::stringstream ss;

    // start all of the servers first. This will let them get up
    // and running before the client attempts to communicste
    std::cout << "Main: ************************************" << std::endl;
    std::cout << "Main: starting server" << std::endl;

    shared_ptr<Node> rpc_server = make_shared<RPCServer>("rpcserver");
    rpc_server -> setAddress("10.0.0.2");
    rpc_server -> startServices();

    // wait for servers to get up and running...
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    std::cout << "Main: ************************************" << std::endl;
    std::cout << "Main: init client" << std::endl;
    shared_ptr<Node> rpc_client = make_shared<RPCClient>("rpcclient");
    rpc_client->setAddress("10.0.0.3");

    std::cout << "Main: ************************************" << std::endl;
    std::cout << "Main: starting client" << std::endl;
    shared_ptr<thread> t;
    {
        // need a scope for the lock guard. 
        // if this doesn't work put it in a function
	std::lock_guard<std::mutex> guard(nodes_mutex);
	 t = make_shared<thread>([rpc_client](){
				try{
				  rpc_client -> start();
				} catch (exitThread & e){ }
			});
	nodes.insert(make_pair(t->get_id(), rpc_client));
	names.insert(make_pair(t->get_id(),"rpc_client"));
    }

    // wait for the client to finish
    std::cout << "Main: ************************************" << std::endl;
    std::cout << "Main: waiting for clients to finish" << std::endl;
    t->join();

    std::cout << "Main: ************************************" << std::endl;
    std::cout << "Main: calling stop services on server" << std::endl;
    rpc_server -> stopServices();

    std::cout << "Main: ************************************" << std::endl;
    std::cout << "Main: waiting for threads to complete" << std::endl;
    // wait for all server threads
    rpc_server -> waitForServices();
 
    return 0;
}