#include "kvservice.hpp"

#ifdef __APPLE__
#define MSG_CONFIRM 0
#endif

using namespace std;
using namespace string_literals;

#define close mclose
void mclose(int fd);

void KeyValueServiceServer::start(){

    cerr << "in KeyValueServiceServer::start" << endl;
    cerr << "gdbm filename" << gdbm_filename_ << endl;

    // define two socket address, one for the server, another for the client
    struct sockaddr_in servaddr, cliaddr;

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // clear variables before initializing
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // fill socket infos
    servaddr.sin_family    = AF_INET;        // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;   // whatever interface is available
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (::bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // MAIN LOOP TO IMPLEMENT THE KEY VALUE SERVICE
    socklen_t len;
    int n;
    char clientStrBuffer[20];

    while(alive){ 
	cerr << "waiting for call from client" << endl;

    // wait for a mesage from a client
    len = sizeof(cliaddr); 
	n = recvfrom(sockfd, (uint8_t *)udpMessage, MAXMSG, 
		    MSG_WAITALL, ( struct sockaddr *) &cliaddr,
		    &len);
    cerr << "server received " << n << " bytes." << endl;

    // Display the client address
    const char * clientstr = inet_ntop(AF_INET,&(cliaddr.sin_addr), clientStrBuffer,20);
    if (clientstr != nullptr){
	    cerr << "  from address " << clientstr << endl;
    }

    // Print the recevied message
    std::cerr << HexDump{udpMessage,(uint32_t)n} << endl;

    } // endwhile

    // close the socket
    close(sockfd);
 
}

void KeyValueServiceServer::setFilename(string name){
    gdbm_filename_ = name;
}