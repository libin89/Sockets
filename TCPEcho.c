#include "TCPEcho.h"
#include "Practical.h"

tcp_connection_t *tcpConnection = NULL;

int32_t initializeTCPData(void)
{
    tcpConnection = (tcp_connection_t *)malloc(sizeof(tcp_connection_t));
    if(!tcpConnection) {
        printErr("Error malloc:%s\n",strerror(errno));
        return ERROR;
    }
    tcpConnection.servIP = (char *)malloc(IP_ADDRESS_SIZE * sizeof(char));
    if(!tcpConnection.servIP) {
        printErr("Error malloc:%s\n",strerror(errno));
        free(tcpConnection);
        return ERROR;
    }
    memset(tcpConnection.servIP, '\0', IP_ADDRESS_SIZE);
    strcpy(tcpConnection.servIP, SERVER_IP_ADDRESS);
    tcpConnection.servPort = SERVER_IP_PORT;
    tcpConnection.buf_size = BUFSIZE;
    tcpConnection.timeout = 1; //seconds
    tcpConnection.data_buf = (char *)malloc(tcpConnection.buf_size * sizeof(char));
    if(!tcpConnection.servIP) {
        printErr("Error malloc:%s\n",strerror(errno));
        free(tcpConnection.servIP);
        free(tcpConnection);
        return ERROR;
    }
    memset(tcpConnection.data_buf, '\0', tcpConnection.buf_size);

    return OK;
}

int32_t deinitializeTCPData(void)
{
    free(tcpConnection.data_buf);
    free(tcpConnection.servIP);
    free(tcpConnection);

    return OK;
}

int main(int argc, char **argv)
{
    int32_t skip = 1;

    if(argc < 2) {
        printErr("Parameter(s): <server|client>\n");
    }
    if(initializeTCPData()) {
        printErr("initializeTCPData fail.\n")
        return ERROR;
    }

    if(skip >= argc)
        break;
    if(!strcasecmp(argv[skip], "-v")) {
        printInfo("%s: Version 1.0\n");
        return OK;
    } else if (!strcasecmp(argv[skip], "-h")) {
        printInfo("Usage: %s [-v] [-h] [--server|client]\n", argv[0]);
        return OK;
    } else if (!strcasecmp(argv[skip], "--server")) {
        main_server();
    } else if (!strcasecmp(argv[skip], "--client")) {
        main_client();
    }

    deinitializeTCPData();
	return OK;
}