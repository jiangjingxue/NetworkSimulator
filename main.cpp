#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <string>


#include "network.hpp"
#include "server.hpp"
#include "client.hpp"
#include "second_client.hpp"

extern std::map<std::thread::id,shared_ptr<Node>> nodes;
extern std::map<std::thread::id,string> names;


int main(int argc, char * argv[]){
    // handle command line arguments...
    int res = network_init(argc, argv);
    stringstream ss;

    // start all of the servers first. This will let them get up
    // and running before the client attempts to communicste
    cout << "Main: ************************************" << endl;
    cout << "Main: starting Server1" << endl;

    shared_ptr<Server> server1 = make_shared<Server>("Server1");
    server1 -> setAddress("10.0.0.2");
    server1->setDatabaseName("kv_dict1.db");
    server1 -> startServices();

    // wait for servers to get up and running...
    this_thread::sleep_for(std::chrono::milliseconds(50));

    cout << "Main: ************************************" << endl;
    cout << "Main: starting Server2" << endl;

    shared_ptr<Server> server2 = make_shared<Server>("Server2");
    server2 -> setAddress("10.0.0.4");
    server2->setDatabaseName("kv_dict2.db");
    server2 -> startServices();

    // wait for servers to get up and running...
    this_thread::sleep_for(std::chrono::milliseconds(50));

    cout << "Main: ************************************" << endl;
    cout << "Main: initialize Client1" << endl;
    shared_ptr<Client> client1 = make_shared<Client>("Client1");
    client1->setAddress("10.0.0.3");
    string client1_addr = client1->getClientAddress();
    cout << "Main: Client1 address: " << client1_addr << endl;
    client1->setServerAddress("10.0.0.2");

    cout << "Main: initialize Client2" << std::endl;
    shared_ptr<SecondClient> client2 = make_shared<SecondClient>("Client2");
    client2->setAddress("10.0.0.5");
    string client2_addr = client2->getClientAddress();
    cout << "Main: Client2 address: " << client2_addr << endl;
    client2->setServerAddress("10.0.0.4");

    cout << "Main: ************************************" << endl;
    cout << "Main: starting multiple clients" << endl;
    vector<shared_ptr<thread>> clientThreads;
    {
      // need a scope for the lock guard. 
      // if this doesn't work put it in a function
      lock_guard<std::mutex> guard(nodes_mutex);
      shared_ptr<thread> t = make_shared<thread>([client1,client2](){
          cout << "Main: ************************************" << endl;
          cout << "Main: Client1" << std::endl;
          client1 -> start();
          cout << "Main: ************************************" << endl;
          cout << "Main: Client2" << std::endl;
          client2 -> start();
      });
      clientThreads.push_back(t);
      nodes.insert(make_pair(t->get_id(), client1));
      names.insert(make_pair(t->get_id(),"client1"));
      nodes.insert(make_pair(t->get_id(), client2));
      names.insert(make_pair(t->get_id(),"client2"));

    }

    // wait for clients to finish
    cout << "Main: ************************************" << endl;
    cout << "Main: waiting for clients to finish" << endl;
    vector<shared_ptr<thread>>::iterator cl_thd;
    for (cl_thd = clientThreads.begin(); cl_thd != clientThreads.end(); cl_thd++){
      shared_ptr<thread> tmp = *cl_thd;
      tmp->join();
    }

    cout << "Main: ************************************" << endl;
    cout << "Main: calling stop services on servers" << endl;
    server1 -> stopServices();
    server2 -> stopServices();

    cout << "Main: ************************************" << endl;
    cout << "Main: waiting for threads to complete" << endl;
    // wait for all server threads
    server1 -> waitForServices();
    server2 -> waitForServices();

    // shut down the protobuf library
    cout << "Main: ************************************" << endl;
    cout << "Main: Shuting down the protobuf library" << endl;
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}

