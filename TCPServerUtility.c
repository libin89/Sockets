#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Practial.h"

void HandleTCPClient(int clntSocket)
{
    memset(tcpConnection.data_buf, '\0', sizeof(tcpConnection.data_buf)); //sizeof(data_buf)==buf_size?

    //Receive message from client
    ssize_t numBytesRcvd = recv(clntSocket, tcpConnection.data_buf, tcpConnection.buf_size, 0);
    if (numBytesRcvd < 0)
        DieWithSystemMessage("recv() failed");
    
    //Send received string and receive again until end of stream
    while (numBytesRcvd > 0) {  //0 indicates end of stream
        //Echo message back to client
        ssize_t numBytesSent = send(clntSocket, tcpConnection.data_buf, numBytesRcvd, 0);
        if (numBytesSent < 0)
            DieWithSystemMessage("send() failed");
        else if (numBytesSent != numBytesRcvd)
            DieWithUserMessage("send()", "sent unexpected number of bytes");
        
        //See if there is more data to receive
        numBytesRcvd = recv(clntSocket, tcpConnection.data_buf, tcpConnection.buf_size, 0);
        if (numBytesRcvd < 0)
            DieWithSystemMessage("recv() failed");
    }

    close(clntSocket);  //Close client socket
}