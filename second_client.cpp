// second_client.cpp

#include "second_client.hpp"

void SecondClient::start() {
    std::string stringValue = "A gaint pink elephant with magnificent tusks";
    std::cout <<  name << ": "<< stringValue << endl;

    std::unique_ptr<uint8_t[]> value(new uint8_t[stringValue.size()]);
    std::copy(stringValue.begin(), stringValue.end(), value.get());

    std::cout << name << ": " << "Content of the byte array to be stored in the database:";
    for (int i = 0; i < stringValue.size(); ++i) {
        std::cout << static_cast<int>(value[i]) << " ";
    }
    std::cout << std::endl;

    std::cout << name << ": -------------- Key Value Put Operation --------------" << std::endl;

    bool putStatus = clientStub.kvPut(44, value, stringValue.size());

    if (putStatus) {
        std::cout << name << ": " << "Successfully put key-value pair." << std::endl;
    } else {
        std::cerr << name << ": " << "Failed to put key-value pair." << std::endl;
    }
    
    this_thread::sleep_for(std::chrono::milliseconds(50));
    
    std::cout << name << ": -------------- Key Value Get Operation --------------" << std::endl;
    
    kvGetResult getResult = clientStub.kvGet(44);

    if (getResult.status) {
        std::cout << name << ": " << "Successfully retrieved value for key 44: ";
        for (uint16_t i = 0; i < getResult.vlen; ++i) {
            std::cout << getResult.value[i];
        }
        std::cout << std::endl;
        std::cout << name << ": " << "Content of the byte array retrieved from database:";
        for (int i = 0; i < getResult.vlen; ++i) {
            std::cout << static_cast<int>(getResult.value[i]) << " ";
        }
        std::cout << std::endl;
    } else {
        std::cerr << name << ": " << "Failed to retrieve value for key 44." << std::endl;
    }
    
    clientStub.shutdownNetwork();
    
}

string SecondClient::getClientAddress(){
    return addr_str;
}

void SecondClient::setServerAddress(string addr) {
    
    servAddr = addr;

    clientStub.setServerAddress(addr);
}