/*+
 *  File:   main.cpp
 *
 *  Purpose:
 *      This module is the start driver for several of the ELEC 477 assignments.
 *      It initializes the
-*/

#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <string>


#include "network.hpp"
#include "e3server.hpp"
#include "e3client.hpp"

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

    shared_ptr<Node> e3Server = make_shared<E3Server>("e3server");
    e3Server -> setAddress("10.0.0.2");
    e3Server -> startServices();

    // wait for servers to get up and running...
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    std::cout << "Main: ************************************" << std::endl;
    std::cout << "Main: init client" << std::endl;
    shared_ptr<Node> e3Client = make_shared<E3Client>("e3client");
    e3Client->setAddress("10.0.0.3");

    std::cout << "Main: ************************************" << std::endl;
    std::cout << "Main: starting client" << std::endl;
    shared_ptr<thread> t;
    {
        // need a scope for the lock guard. 
        // if this doesn't work put it in a function
	std::lock_guard<std::mutex> guard(nodes_mutex);
	t = make_shared<thread>([e3Client](){
	    try{
		e3Client -> start();
	    } catch (exitThread & e){ }
	});
	nodes.insert(make_pair(t->get_id(), e3Client));
	names.insert(make_pair(t->get_id(),"e3client"));

    }

    // wait for clients to finish
    std::cout << "Main: ************************************" << std::endl;
    std::cout << "Main: waiting for clients to finish" << std::endl;
    t->join();

    std::cout << "Main: ************************************" << std::endl;
    std::cout << "Main: calling stop services on server" << std::endl;
    e3Server -> stopServices();

    std::cout << "Main: ************************************" << std::endl;
    std::cout << "Main: waiting for threads to complete" << std::endl;
    // wait for all server threads
    e3Server -> waitForServices();
 


    //std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    return 0;
}

