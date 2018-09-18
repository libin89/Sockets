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

## Create and destroy socket

> int socket(int domain, int type, int protocol)
domain: IPv4(AF_INET), IPv6(AF_INET6) ...
type: SOCK_STREAM, SOCK_DGRAM ...
protocol: IPPROTO_TCP, IPPROTO_UDP ...
return -1:fail, nonegative:success

> int close(int socket)
success will return 0, fail return -1

## Specific address
### General address
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
### General address storage
`struct sockaddr_storage {
    sa_family_t
    ...
    //Padding and fields to get correct length and alignment
    ...
};`
### Binary/string address translation
* inet_pton() function(pton=printable to numeric)
> int inet_pton(int addressFamily, const char *src, void *dst)
* translation success return 1, src is formated to invalid address return 0, invalid address family return -1
* inet_ntop() function(ntop=numeric to printable)
> const char *inet_ntop(int addressFamily, const void *src, char *dst, socklen_t dstBytes)
* dstBytes: INET_ADDRSTRLEN(IPv4), INET6_ADDRSTRLEN(IPv6) ...
* translation success return non-NULL and printable string addr is saved in dst, otherwise contain NULL
### Get socket associated address
> int getpeername(int socket, struct sockaddr *remoteAddress, socketlen_t *addressLength)
> int getsockname(int socket, struct sockAddr *localAddress, socketlen_t *addressLength)
* socket: socket identifier we want to get address info
* remoteAddress/localAddress: point towards address structure containing address info, always be converted to socketaddr *. If don't know IP protocol version, we should use sockaddr_storage to recevice result.
* addressLength: input/output parameter
### connection socket
> int connect(int socket, const struct sockaddr *foreignAddress, socklen_t addressLength)
* addressLength: usually, sizeof(struct sockaddr_in) or sizeof(struct sockaddr_in6)
### Bind to address
> int bind(int socket, struct sockaddr *localAddress, socklen_t addressSize)
* success return 0, fail return -1.
### Process incoming connection
> int listen(int socket, int queueLimit)
* queueLimit: incoming connection maximum
* success return 0, otherwise return -1.
> int accept(int socket, struct sockaddr *clientAddress, socklen_t *addressLength)
* accept will block until having a connection request reach.
* if success, clientAddress will be set to point to sockaddr structure and addressLength is real address size.
* for example:
`struct sockaddr_storage address;
 socklen_t addrLength = sizeof(address);
 int newConnection = accept(sock, &address, &addressLength);`
 * if success, return a new socket identifier connecting to client. if fail, return -1.
 ### Communication
 * **After connection, the difference of client and server will disappear in fact.**
 > ssize_t send(int socket, const void *msg, size_t msgLength, int flags)
 > ssize_t recv(int socket, void *rcvBuffer, size_t bufferLength, int flags)
 * msgLength: the number of bytes will be sent
 * send() default behavior: block until sent all of data
 * recv() default behavior: block until received a few bytes(at least one byte)
 * bufferLength：cache length given, is received maximum number of bytes one time.
 * flags: set to 0 indicates default behavior
 * send() and recv() are used to return sent or recived number of bytes, fail return -1.
 * **TCP是一种字节流协议，因此不会保留send()边界。在接收者调用recv()一次所读取的字节数不一定由调用send()一次所写的字节数确定。如果利用3000字节调用send()，可能需要多次调用recv()来获取全部3000字节，即使给每个recv()调用传递5000字节的缓冲区也是如此。如果利用100字节调用send()四次，可能通过调用一次recv()一次来接收全部400字节。**
 * **在编写TCP套接字应用程序时一个常见的错误涉及如下假定：如果利用一个send()写入所有的数据，则可以利用一个recv()读取所有这些数据。**
 ### Use IPv6

 ## 关于名称和地址族
 ### 将名字映射到数字
 * 名称服务并不是使TCP/IP工作所必需的，名称只是提供一个间接层次。
 ### 访问名称服务
 > int getaddrinfo(const char *hostStr, const char *serviceStr, const  struct addrinfo *hints,
                struct addrinfo **results)
* 前两个参数指向null终止的字符串，分别表示主机名称或地址以及服务器名称或端口号
* hints描述要返回的信息种类
* result是struct addrinfo指针的位置，其中存储一个指向包含结果的链表的指针
* 解析成功（0），不成功（非0错误代码）
> void freeaddrinfo(struct addrinfo *addrList)
> const char *gai_strerror(int errorCode)
* getaddrinfo()创建结果的动态分配的链表，必须在调用者完成链表后释放它。
* 万一getaddrinfo()返回一个非0（错误）值，把它传递给gai_strerror()将生成一个字符串，它描述了错误的是什么。
`struct addrinfo {
    int ai_flags;   //Flags to control info resolution
    int ai_family;  //Family: AF_INET, AF_INET6, AF_UNSPEC
    int ai_socktype;//Socket type: SOCK_STREAM, SOCK_DGRAM
    int ai_protocol;//Protocol: 0(default) or IPPROTO_XXX
    socklen_t ai_addrlen; //Length of socket address ai_addr
    struct sockaddr *ai_addr; //Socket address for socket
    char *ai_canonname;  //Canonical name
    struct addrinfo *ai_next; //Next addrinfo in linked list
};`
* addrinfo各字段详细信息（暂略）
### 编写地址通用的代码（代码已同步更新）