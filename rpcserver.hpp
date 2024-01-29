#include "network.hpp"
#include "kvservice.hpp"

class RPCServer: public Node{

    public:
        RPCServer(string nodeName);
	    ~RPCServer(){};
        
};