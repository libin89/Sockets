***
### Edit format brief introduction

1. "# " means h1
1. "## " means h2
1. "### " means h3
1. "\*\*content\*\*" means bold
1. "\_content\_" means italic
1. "\`content\`" means code
1. "* " means unordered list
1. "1. " means ordered list
1. "> " means blockquote
1. "***" means horizontal rule
***

# Sockets

## IPv4 TCP Client

## IPv4 TCP Server

## create and destroy socket

***
int socket(int domain, int type, int protocol)
***
domain: IPv4(AF_INET), IPv6(AF_INET6) ...
type: SOCK_STREAM, SOCK_DGRAM ...
protocol: IPPROTO_TCP, IPPROTO_UDP ...
return -1:fail, nonegative:success

***
int close(int socket)
***
success will return 0, fail return -1

## specific address
### general address
`struct sockaddr {
    sa_family sa_family;  //Address family (e.g., AF_INET)
    char sa_data[14];  //Family-specific address information
};`
### IPv4 address
`struct in_addr {
    uint32_t s_addr;  //Internet address (32 bits)
};

struct sockaddr_in {
    sa_family_t sin_family; //Internet protocol (AF_INET)
    in_port_t sin_port;     //Address port (16 bits)
    struct in_addr sin_addr;//IPv4 address (32 bits)
    char sin_zero[8];       //Not used
};`
### IPv6 address
`srtuct in_addr {
    uint32_t s_addr[16];  //Internet address (128 bits)
};

struct sockaddr_in6 {
    sa_family_t sin6_family;   //Internet protocol (AF_INET6)
    in_port_t sin6_port;       //Address port (16 bits)
    uint32_t sin6_flowinfo;    //Flow information
    struct in6_addr sin6_addr; //IPv6 address (128 bits)
    uint32_t sin6_scope_id;    //Scope identifier
};`
### general address storage
`struct sockaddr_storage {
    sa_family_t
    ...
    //Padding and fields to get correct length and alignment
    ...
};`
### binary/string address translation
* inet_pton() function(pton=printable to numeric)
***
int inet_pton(int addressFamily, const char *src, void *dst)
***
translation success return 1, src is formated to invalid address return 0, invalid address family return -1
* inet_ntop() function(ntop=numeric to printable)
***
const char *inet_ntop(int addressFamily, const void *src, char *dst, socklen_t dstBytes)
***
dstBytes: INET_ADDRSTRLEN(IPv4), INET6_ADDRSTRLEN(IPv6) ...
translation success return non-NULL and printable string addr is saved in dst, otherwise contain NULL