#include "rpcclient.hpp"

using namespace std;
using namespace std::string_literals;

#define close mclose

const int MAXMSG = 1400;
in_port_t PORT = 8080;

void RPCClient::start(){

    int n;
    int sockfd;
    socklen_t len;
    uint32_t blen = MAXMSG;
    uint8_t buffer[MAXMSG];	 // to serialize into
    struct sockaddr_in servaddr;

    // Server information
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    inet_aton("10.0.0.2", &servaddr.sin_addr);

    // Open a socket to send the info.
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
	perror("socket creation failed");
	exit(EXIT_FAILURE);
    }

    // send the 1st message to the server.
    // +1 to include the null byte at the end of the string
    blen = snprintf((char*)buffer, MAXMSG,"Test 1 Message 1")+1;
    n = sendto(sockfd, (const char *)buffer, blen,
		MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    std::cout << "client sendto n = " << n << endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // send the 2nd message to the server.
    blen = snprintf((char*)buffer, MAXMSG,"Test 1 Message 2")+1;
    n = sendto(sockfd, (const char *)buffer, blen,
		MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    std::cout << "client sendto n = " << n << endl;


    std::this_thread::sleep_for(std::chrono::milliseconds(50));


    close(sockfd);
}