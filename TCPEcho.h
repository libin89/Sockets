#ifndef _TCP_ECHO_H
#define _TCP_ECHO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IP_ADDRESS_SIZE 129
#define SERVER_IP_ADDRESS "192.168.1.5"
#define SERVER_IP_PORT 8090
#define BUFSIZE 1800

typedef struct {
    in_port_t servPort;  //server port
    int32_t buf_size;  // buffer size
    int32_t timeout;  // timeout
    char *data_buf; // data buffer
    char *servIP;   //server ip address(ipv6 or ipv4)
}tcp_connection_t;

int32_t initializeTCPData(void);
int32_t deinitializeTCPData(void);



#endif