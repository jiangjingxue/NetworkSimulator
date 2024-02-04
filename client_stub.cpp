// client_stub.cpp

#include "client_stub.hpp"

const int MAXMSG = 1400;
in_port_t PORT = 8080;

ClientStub::ClientStub() {
    sockfd = -1; // Initialize sockfd to an invalid value
}

void ClientStub::shutdownNetwork() {
    mclose(sockfd);
}

void ClientStub::setServerAddress(string addr) {

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_aton(addr.c_str(), &servaddr.sin_addr);
    std::cout << "ClientStub: server address is set to " << addr << std::endl;
}

bool ClientStub::kvPut(int32_t key, std::unique_ptr<uint8_t[]>& value, uint16_t vlen) {

    // Autostarrt network
    if (sockfd == -1) {
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("socket creation failed");
            return false;
        }
    }

    // Initialize an RpcMessage object from .proto file
    RPC::RpcMessage rpc_message;

    // Set contents of RPC message header (magic number, version number, message id)
    rpc_message.mutable_kv_put_request()->mutable_header()->set_magic_number(42);
    rpc_message.mutable_kv_put_request()->mutable_header()->set_version(1);

    // Generate and set unique message ID
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    uint64_t sendID = static_cast<uint64_t>(millis);
    rpc_message.mutable_kv_put_request()->mutable_header()->set_message_id(sendID);

    // Set contents of put request
    rpc_message.mutable_kv_put_request()->set_key(key);
    rpc_message.mutable_kv_put_request()->mutable_value()->assign(value.get(), value.get() + vlen);
    rpc_message.mutable_kv_put_request()->set_vlen(vlen);

    std::string serialized_message;
    rpc_message.SerializeToString(&serialized_message);

    // Use the existing socket and servaddr to send data
    n = sendto(sockfd, serialized_message.data(), serialized_message.size(), MSG_CONFIRM, (const struct sockaddr*)&servaddr, sizeof(servaddr));
    std::cout << "ClientStub: client sent " << n << " bytes"<< std::endl;

    // Receive the response
    uint8_t buffer[MAXMSG];
    socklen_t length = sizeof(struct sockaddr_in);
    n = recvfrom(sockfd, (uint8_t *)buffer, MAXMSG, MSG_WAITALL, (struct sockaddr *) &servaddrreply.sin_addr, &length);

    // Parse the received data into an RpcMessage
    RPC::RpcMessage response_message;
    response_message.ParseFromArray(buffer, n);
    
    // Handle the response
    if (response_message.has_kv_put_response()) {
        const RPC::KvPutResponse& kv_put_response = response_message.kv_put_response();

        // Check to make sure message ID of response matches request
	uint64_t returnID = kv_put_response.header().message_id();
	if (returnID != sendID) {
	    cout << "ERROR: Message IDs do not match. Messages were lost" << endl;
	    return false;
	}
        bool putStatus = kv_put_response.status();
        return putStatus;
    } else {
        cerr << "Invalid response format" << endl;
        return false;
    }
}

kvGetResult ClientStub::kvGet(int32_t key) {

    // Autostart network
    if (sockfd == -1) {
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("socket creation failed");
            return {false, nullptr, 0};
        }
    }

    // Initialize an RpcMessage object for the GET request
    RPC::RpcMessage rpc_message;

    // Set contents of RPC message header for GET request
    rpc_message.mutable_kv_get_request()->mutable_header()->set_magic_number(42);
    rpc_message.mutable_kv_get_request()->mutable_header()->set_version(1);

    // Generate and set unique message ID
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    uint64_t sendID = static_cast<uint64_t>(millis);
    rpc_message.mutable_kv_get_request()->mutable_header()->set_message_id(sendID);

    // Set contents of GET request
    rpc_message.mutable_kv_get_request()->set_key(key);

    std::string serialized_message;
    rpc_message.SerializeToString(&serialized_message);

    // Send GET request to the server
    n = sendto(sockfd, serialized_message.data(), serialized_message.size(), MSG_CONFIRM, (const struct sockaddr*)&servaddr, sizeof(servaddr));
    std::cout << "ClientStub: client sent = " << n << " bytes" << std::endl;

    // Receive the response
    uint8_t buffer[MAXMSG];
    socklen_t length = sizeof(struct sockaddr_in);
    n = recvfrom(sockfd, (uint8_t*)buffer, MAXMSG, MSG_WAITALL, (struct sockaddr*)&servaddrreply, &length);
    std::cerr << "ClientStub: client received " << n << " bytes." << std::endl;

    // Parse the received data into an RpcMessage
    RPC::RpcMessage response_message;
    response_message.ParseFromArray(buffer, n);

    // Handle the response, checking the type of response message
    if (response_message.has_kv_get_response()) {
        const RPC::KvGetResponse& kv_get_response = response_message.kv_get_response();

        // Check to make sure message ID of response matches request
        uint64_t returnID = kv_get_response.header().message_id();
        if (returnID != sendID) {
            cout << "ERROR: Message IDs do not match. Messages were lost" << endl;
            return {false, nullptr, 0};
        }

        kvGetResult result;

        // Retrieve the value and vlen
        const std::string& serialized_value = kv_get_response.value();
        result.vlen = static_cast<uint16_t>(serialized_value.size());
        result.value = std::make_unique<uint8_t[]>(result.vlen);
        std::copy(serialized_value.begin(), serialized_value.end(), result.value.get());
        return result;
    } else {
        cerr << "Invalid response format" << endl;
        return {false, nullptr, 0};
    }
}

