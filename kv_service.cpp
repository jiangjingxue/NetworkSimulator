// kv_service.cpp

#include "kv_service.hpp"

#define close mclose
void mclose(int fd);


void KVServiceServer::start(){

    cerr << "in KVServiceServer::start" << endl;

    // Setup Network configuration

    struct sockaddr_in servaddr, cliaddr;

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("KVService: socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family    = AF_INET;        // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;   // whatever interface is available
    servaddr.sin_port = htons(PORT);

    if (::bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
    {
        perror("KVService: bind failed");
        exit(EXIT_FAILURE);
    }

    socklen_t len;
    int n;
    char clientStrBuffer[20];


    // Open GDBM file
    db = gdbm_open(dbname.c_str(), 0, GDBM_WRCREAT | GDBM_READER | GDBM_WRITER, 0666, nullptr);
    if (!db) {
        // Handle error opening/creating the database
	cerr << "KVService: Could not open/create database: " << gdbm_strerror(gdbm_errno) << endl;
	exit(1);
    }

    // Run main network loop
    cliaddr.sin_addr.s_addr = INADDR_ANY;
    while(alive){ 
        cerr << "KVService: waiting for call from client" << endl;

        // Clear udpMessage by filling it with zeros
        memset(udpMessage, 0, MAXMSG);

        // wait for a mesage from a client
        len = sizeof(cliaddr);  //len is value/result
        n = recvfrom(sockfd, (uint8_t *)udpMessage, MAXMSG, 
	    MSG_WAITALL, ( struct sockaddr *) &cliaddr,
	    &len);
        if (inet_ntop(AF_INET,&(cliaddr.sin_addr), clientStrBuffer,20) != nullptr){
            cout << "KVService: server received " << n << " bytes from address " << clientStrBuffer << endl;
        }

	// Call handleRPC() to decode udpMessage and process RPC
	uint64_t receiveID = handleRPC(udpMessage, n);
        int servern;

        if(RPCFunction == 0) {
	    // Respond to client with kv_put_response
	    RPC::RpcMessage rpc_message;

	    // Set contents of RPC message header (magic number, version number, message id)
	    rpc_message.mutable_kv_put_response()->mutable_header()->set_magic_number(42);
	    rpc_message.mutable_kv_put_response()->mutable_header()->set_version(1);

            // Set message id for response to match request
	    rpc_message.mutable_kv_put_response()->mutable_header()->set_message_id(receiveID);

	    // Set contents of put response
	    rpc_message.mutable_kv_put_response()->set_status(putStatus);
	    
	    // Serialize message to string to be sent over the network
	    string udpReply;
	    rpc_message.SerializeToString(&udpReply);

	    // Respond to client with kv_put_response message
	    servern = sendto(sockfd, udpReply.data(), udpReply.size(), MSG_CONFIRM, (const struct sockaddr*) &cliaddr, len);
	    if (servern == -1) {
	        perror("KVService: sendto failed");
	    }
	    cout << "KVService: server sent " << servern << " bytes" << endl;

        } else if(RPCFunction == 1) {
            // Respond to client with kv_get_response
            RPC::RpcMessage rpc_message;

            // Set contents of RPC message header (magic number, version number, message id)
            rpc_message.mutable_kv_get_response()->mutable_header()->set_magic_number(42);
            rpc_message.mutable_kv_get_response()->mutable_header()->set_version(1);

            // Set message id for response to match request
            rpc_message.mutable_kv_get_response()->mutable_header()->set_message_id(receiveID);
	    
	    // Set contents of get response
            rpc_message.mutable_kv_get_response()->set_status(returnResult.status);
	    rpc_message.mutable_kv_get_response()->mutable_value()->assign(returnResult.value.get(), returnResult.value.get() + returnResult.vlen);
	    rpc_message.mutable_kv_get_response()->set_vlen(returnResult.vlen);

	    // Serialize message to string to be sent over the network
            string udpReply;
            rpc_message.SerializeToString(&udpReply);

            servern = sendto(sockfd, udpReply.data(), udpReply.size(), MSG_CONFIRM, (const struct sockaddr*) &cliaddr, len);
            if (servern == -1) {
                perror("KVService: sendto failed");
            }
            cout << "KVService: server sent " << servern << " bytes" << endl;

	} else {
	    cerr << "KVService: RPC Message not recognized" << endl;
 	}
    }


    // Cleanup sockets, close gdbm file, and return
    close(sockfd);
    gdbm_close(db);
    return; // Implicit and not needed (since void function) but added for consistency with assignent doc

}

void KVServiceServer::setDatabaseName(string dbName){

    dbname = dbName;

}

uint64_t KVServiceServer::handleRPC(uint8_t* serialized_message, int n){
	    
    // Parse the received data into an RpcMessage
    RPC::RpcMessage rpc_message;
    
    uint64_t receiveID;

    if (rpc_message.ParseFromArray(serialized_message, n)) {
        // Successfully parsed the message
        if (rpc_message.has_kv_put_request()) {
            
	    RPCFunction = 0;

            const RPC::KvPutRequest& kv_put_request = rpc_message.kv_put_request();
            
            uint32_t magic_number = kv_put_request.header().magic_number();
            uint32_t version = kv_put_request.header().version();
            receiveID = kv_put_request.header().message_id();
            int32_t key = kv_put_request.key();
            uint16_t vlen = kv_put_request.vlen();
            const string& value_str = kv_put_request.value();
            unique_ptr<uint8_t[]> value(new uint8_t[vlen]);
            copy(value_str.begin(), value_str.begin() + vlen, value.get());

            putStatus = kvPut(key, value, vlen);

        } else if (rpc_message.has_kv_get_request()) {

	    RPCFunction = 1;
	    
	    const RPC::KvGetRequest& kv_get_request = rpc_message.kv_get_request();

	    uint32_t magic_number = kv_get_request.header().magic_number();
            uint32_t version = kv_get_request.header().version();
            receiveID = kv_get_request.header().message_id();
            int32_t key = kv_get_request.key();

            returnResult = kvGet(key);

        }
    } else {
        // Failed to parse the message
	cerr << "Failed to parse incoming message." << endl;
	return -1;
    }
    return receiveID;

}

bool KVServiceServer::kvPut(int32_t key, unique_ptr<uint8_t[]> &value, uint16_t vlen){

    datum gdbmKey, gdbmValue;
    gdbmKey.dptr = reinterpret_cast<char*>(&key);
    gdbmKey.dsize = sizeof(int32_t);
    gdbmValue.dptr = reinterpret_cast<char*>(value.get());
    gdbmValue.dsize = vlen;
    // Store the key-value pair in the database
    if (gdbm_store(db, gdbmKey, gdbmValue, GDBM_REPLACE) != 0) {
        return false;
    }
    return true;

}

KVServiceServer::kvGetResult KVServiceServer::kvGet(int32_t key){
    
    kvGetResult result;

    // Initialize the GDBM key structure
    datum gdbmKey, gdbmValue;
    gdbmKey.dptr = reinterpret_cast<char*>(&key);
    gdbmKey.dsize = sizeof(int32_t);

    // Lookup the key in the GDBM database
    gdbmValue = gdbm_fetch(db, gdbmKey);

    if (gdbmValue.dptr != nullptr) {
        // Key found, populate the result
        result.status = true;
        result.vlen = gdbmValue.dsize;
        result.value = make_unique<uint8_t[]>(result.vlen);
        copy(gdbmValue.dptr, gdbmValue.dptr + result.vlen, result.value.get());
    } else {
        // Key not found
        result.status = false;
        result.vlen = 0;
        result.value = nullptr;
    }

    // Free the memory allocated by GDBM for the result
    free(gdbmValue.dptr);

    return result;
    
}

