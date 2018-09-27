#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

int main_client(void)
{
	char *servIP = tcpConnection.servIP;  //server IP address(dotted quad)
	
	//server port(numeric).
	in_port_t servPort = tcpConnection.servPort;
	
	//Create a reliable, stream socket using TCP
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0)
		DieWithSystemMessage("socket() failed");
	
	//Construct the server address structure
	struct sockaddr_in servAddr;  //Server address
	memset(&servAddr, 0, sizeof(servAddr));  //Zero out structure
	servAddr.sin_family = AF_INET;  //IPv4 address family
	//Convert address
	int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
	if (rtnVal == 0)
		DieWithUserMessage("inet_pton() failed", "invalid address string");
	else if (rtnVal < 0)
		DieWithUserMessage("inet_pton() failed");
	servAddr.sin_port = htons(servPort);  //Server port
	
	//Establish the connection to the echo server
	if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
		DieWithSystemMessage("connect() failed");
	
	printDebug("Sent: ");
	//read echo string from stdin stream
	while(1) {
		fgets(tcpConnection.data_buf, tcpConnection.buf_size, stdin));
		if(ferror(tcpConnection.data_buf)) {
			printErr("Error get string!\n");
			//do something ...
		} else {
			size_t echoStringLen = strlen(tcpConnection.data_buf); //Determin input length
			printDebug("%s\n", tcpConnection.data_buf);
			//Send the string to the server
			ssize_t numBytes = send(sock, tcpConnection.data_buf, echoStringLen, 0);
			if (numBytes < 0)
				DieWithSystemMessage("send() failed");
			else if (numBytes != echoStringLen)
				DieWithUserMessage("send()", "sent unexpected number of bytes");
			printDebug("Sent: ");
		}

		//Receive the same string back from server
		unsigned int totalBytesRcvd = 0; //Count of total bytes received
		printDebug("Received: "); //Setup to print the echoed string
		while (totalBytesRcvd < echoStringLen) {
			memset(tcpConnection.data_buf, '\0', sizeof(tcpConnection.data_buf)); //sizeof(data_buf)==buf_size?
			/* Receive up to the buffer size (minus 1 to leave space for 
			a null terminator) bytes from the sender */
			numBytes = recv(sock, tcpConnection.data_buf, tcpConnection.buf_size, 0);
			if (numBytes < 0)
				DieWithSystemMessage("recv() failed");
			else if (numBytes == 0)
				DieWithUserMessage("recv()", "connection closed prematurely");
			totalBytesRcvd += numBytes; //Keep tally of total bytes
			tcpConnection.data_buf[numBytes] = '\0'; //Terminate the string!
			printDebug("%s\n", tcpConnection.data_buf); //Print the echo buffer
			
		}
	}
	
	close(sock);
}