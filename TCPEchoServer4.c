#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

static const int MAXPENDING = 5; //Maximum outstanding connection requests

int main(int argc, char *argv[])
{

    if (argc != 2)  //Test for correct number of arguments
        DieWithUserMessage("Parameter(s", "<Server Port>");
    
    in_port_t servPort = atoi(argv[1]);  //First arg: local port

    //Create socket for incoming connections
    int servSock;  //Socket descriptor for server
#ifndef IPPROTO_IPV6
    if ((servSock = socket(AF_INENT, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithSystemMessage("socket() failed");
#else
    if ((servSock = socket(AF_INENT6, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithSystemMessage("socket() failed");
#endif
    
    //Construct local address structure
#ifndef IPPROTO_IPV6
    struct sockaddr_in servAddr;  //Local address
    memset(&servAddr, 0, sizeof(servAddr));  //Zero out structure
    servAddr.sin_family = AF_INET;  //IPv4 address family
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);  //Any incoming interface
    servAddr.sin_port = htons(servPort);  //Local port
#else
    struct sockaddr_in6 servAddr;  //Local address
    memset(&servAddr, 0, sizeof(servAddr));  //Zero out structure
    servAddr.sin6_family = AF_INET6;  //IPv6 address family
    servAddr.sin6_addr.s_addr = in6addr_any;  //Any incoming interface
    servAddr.sin6_port = htons(servPort);  //Local port
#endif

    //Bind to the local address
    if (bind(servSock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
        DieWithSystemMessage("bind() failed");
    
    //Mark the socket so it will listen for incoming connections
    if (listen(servSock, MAXPENDING) < 0)
        DieWithSystemMessage("listen() failed");

    for (;;) {  //Run forever
    #ifndef IPPROTO_IPV6
        struct sockaddr_in clntAddr;  //Client address
    #else
        struct sockaddr_in6 clntAddr;  //Client address
    #endif
        //Set length of client address structure (in-out parameter)
        socklen_t clntAddrLen = sizeof(clntAddr);

        //Wait for a client to connect
        int clntSock = accept(servSock, (struct sockaddr*)&clntAddr, &clntAddrLen);
        if (clntSock < 0)
            DieWithSystemMessage("accept() failed");
        
        //clntSock is connected to a client!
    #ifndef IPPROTO_IPV6
        char clntName[INET_ADDRSTRLEN];  //String to contain client address
        if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL)
            printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
        else
            puts("Unable to get client address");
    #else
        char clntName[INET6_ADDRSTRLEN];  //String to contain client address
        if (inet_ntop(AF_INET6, &clntAddr.sin6_addr.s6_addr, clntName, sizeof(clntName)) != NULL)
            printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin6_port));
        else
            puts("Unable to get client address");
    #endif
        HandleTCPClient(clntSock);
    }
    //NOT REACHED
}