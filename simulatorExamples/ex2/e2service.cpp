
#include "e2service.hpp"

#ifdef __APPLE__
#define MSG_CONFIRM 0
#endif

using namespace std;
using namespace string_literals;

#define close mclose
void mclose(int fd);

void E2ServiceServer::start(){

    cerr << "in E2ServiceServer::start" << endl;

    struct sockaddr_in servaddr, cliaddr;

    // get a socket to recieve messges
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // clear variables before initializing
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Port and Interfact information for binding
    // the socket
    servaddr.sin_family    = AF_INET;        // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;   // whatever interface is available
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (::bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    socklen_t len;
    int n;
    char clientStrBuffer[20];

    while(alive){ 
	cerr << "waiting for call from client" << endl;

        // wait for a mesage from a client
	len = sizeof(cliaddr);  //len is value/result
	n = recvfrom(sockfd, (uint8_t *)udpMessage, MAXMSG, 
		    MSG_WAITALL, ( struct sockaddr *) &cliaddr,
		    &len);
        cerr << "server received " << n << " bytes." << endl;
	const char * clientstr = inet_ntop(AF_INET,&(cliaddr.sin_addr), clientStrBuffer,20);
        if (clientstr != nullptr){
	    cerr << "  from address " << clientstr << endl;
        }
	std::cerr << HexDump{udpMessage,(uint32_t)n} << endl;

	// format reply

	uint32_t blen = snprintf((char*)udpReply, MAXMSG,"%s reply", udpMessage)+1;


	int servern = sendto(sockfd, udpReply, blen,
                MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
        cerr << "server sent " << servern << " bytes" << endl;

    }

    close(sockfd);
}

