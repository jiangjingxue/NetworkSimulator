#include "e3client.hpp"

using namespace std;
using namespace std::string_literals;

#define close mclose
void mclose(int fd);

const int MAXMSG = 1400;
in_port_t PORT = 8080;

void E3Client::start(){

    int n;
    int sockfd;
    socklen_t len;
    uint32_t blen = MAXMSG;
    uint8_t buffer[MAXMSG];
    struct sockaddr_in servaddr;
    struct sockaddr_in servaddrreply;

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

    // set socket to timeout in 1 second if reply not received
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv,sizeof(tv)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    // send the message to the server.
    // +1 to include the null byte in the message
    blen = snprintf((char*)buffer, MAXMSG,"Test 1 Message 1")+1;
    n = sendto(sockfd, (const char *)buffer, blen,
		MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    std::cout << "client sendto n = " << n << endl;

    len = sizeof(struct sockaddr_in);
    n = recvfrom(sockfd, (char *)buffer, MAXMSG,
		MSG_WAITALL, (struct sockaddr *) &servaddrreply, &len);
    if (n==-1) {
        if (errno == EWOULDBLOCK){
	    std::cout << "client timed out" << std::endl;
	} else {
	    perror("recvfrom");
	}
    } else {
	std::cout << "client recieved = " << n << std::endl;
	std::cout << HexDump{buffer,(uint32_t)n} << endl;
    }

    close(sockfd);
}

