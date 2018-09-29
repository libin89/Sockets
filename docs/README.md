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
* domain: IPv4(AF_INET), IPv6(AF_INET6) ...
* type: SOCK_STREAM, SOCK_DGRAM ...
* protocol: IPPROTO_TCP, IPPROTO_UDP ...
* return -1:fail, nonegative:success

> int close(int socket)
* success will return 0, fail return -1

## Specific address
### General address
`struct sockaddr {
    sa_family sa_family;  //Address family (e.g., AF_INET)
    char sa_data[14];  //Family-specific address information
};`
### IPv4 address
`struct in_addr {
    uint32_t s_addr;  //Internet address (32 bits)
};`
`struct sockaddr_in {
    sa_family_t sin_family; //Internet protocol (AF_INET)
    in_port_t sin_port;     //Address port (16 bits)
    struct in_addr sin_addr;//IPv4 address (32 bits)
    char sin_zero[8];       //Not used
};`
### IPv6 address
`srtuct in_addr {
    uint32_t s_addr[16];  //Internet address (128 bits)
};`
`struct sockaddr_in6 {
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
### 编写地址通用的代码（待更新）

## 超越基本的套接字编程
### 套接字选项
* getsockopt()和setsockopt()分别允许查询和设置套接字选项值
> int getsockopt(int socket, int level, int optName, void *optVal, socklen_t *optLen)
> int setsockopt(int socket, int level, int optName, const void *optVal, socklen_t optLen)
* 查看每个层级的_一些常用的选项，包括optVal指向的缓冲区的说明和数据类型（TCP_IP_Sockets编程page108）
### 信号
* 在TCP套接字上发送数据的任何程序都必须显示处理SIGPIPE以便保证健壮性。
* ---------------------------------------------------------------
*   类型                  触发事件                     默认行为
* SIGALRM              报警计时器到期                    终止
* SIGCHLD              子进程退出                       忽略
* SIGINT               中断字符（Ctrl+C）输入            终止
* SIGIO                套接字为I/O做好准备               忽略
* SIGPIPE              尝试写到关闭的套接字               终止
* ----------------------------------------------------------------
* 应用程序可以使用sigaction()更改特定信号的默认行为
> int sigaction(int whichSignal, const struct sigaction *newAction, struct sigaction *oldAction)
* 成功返回0，失败返回-1
`struct sigaction {`
`   void (*sa_handler)(int);  //Signal handler`
`   sigset_t sa_mask; //Signals to be blocked during handler execution`
`   int sa_flags; //Flags to modify default behavior`
`};`
* sa_mask被实现为一组布尔标志，其中每个标志用于一种信号。以下函数可以操纵这组标志：
> int sigemptyset(sigset_t *set)
> int sigfillset(sigset_t *set)
> int sigaddset(sigset_t *set, int whichSignal)
> int sigdelset(sigset_t *set, int whichSignal)
* 成功返回0，失败返回-1
* **认识到信号不会排队很重要--信号要么是挂起的，要么不是。如果在处理信号时把相同的信号递送多次，那么在处理程序完成原始执行之后它只会执行一次。**
* 信号的最重要的方面之一与套接字接口相关。
* 如果在套接字调用（如recv()或connect()）中阻塞程序时递送信号，并且指定了用于该信号的处理程序，那么一旦处理程序执行完成，套接字调用将返回-1，并且把errno设置为EINTR。因此，**用于捕获和处理信号的程序需要为从可能阻塞的系统调用返回的这些错误做好准备。**
* SIGPIPE的默认行为是终止程序，因此没有改变这种行为的服务器可能会被行为失常的客户所终止。**服务器总是应该处理SIGPIPE，使得他们可以检测到客户的消失，并收回用于给它提供服务的任何资源。**
### 非阻塞I/O
### 非阻塞套接字
> int fcntl(int socket, int command, ...)
### 异步I/O
* SIGIO
### 超时
* 实现超时的标准方法是在调用阻塞函数之前设置alarm
> unsigned int alarm(unsigned int secs)
* alarm()启动一个计时器，它在经过指定的秒数之后到期；alarm()为以前预定的任何报警返回剩余的秒数（或者如果没有预定报警，就返回0）.当计时器到期时，就把SIGALRM信号发送给进程，并且执行用于SIGALRM的处理函数（如果有的话）。
* example at page 120(TCP_IP_Sockets编程)

## 多任务处理
### 每个客户一个进程
> pid_t wait(int *statloc);
> pid_t waitpid(pid_t pid,int *statloc, int options);
* statloc指向终止进程的终止状态，如果不关心终止状态可指定为空指针
* pid有四种情况：
* 1.pid==-1 等待任意子进程
* 2.pid>0 等待进程ID与pid相等的子进程
* 3.pid==0 等待组ID等于调用进程组ID的任意子进程
* 4.pid<-1 等待组ID等于pid绝对值的任意子进程
* options控制waitpid的操作：
* 1,2是支持作业控制
* 1.WCONTINUED
* 2.WUNTRACED
* 3.WNOHANG  waitpid不阻塞
* 在子进程终止前，wait使其调用者阻塞，waitpid有一个选项可使调用者不阻塞。
* waitpid并不等待在其调用之后第一个终止的子进程，它有若干选项。换言之可以不阻塞。
* 事实上：
`pid_t wait(int *statloc)
{
    return waitpid(-1, statloc, 0);
}`
* **守护进程（deamon.c）**
* **时间与时区（TIME.md）**
### 每个客户一个线程
### 受限的多任务处理
### 多路复用
* 利用select()函数，程序可以指定一份要为挂起的I/O检查的描述符列表；select()会挂起程序，直到列表中的某个描述符准备好执行I/O并且返回哪些描述符已做好准备的指示为止。然后，程序可以在那个描述符上继续执行I/O，并且操作将不会阻塞。
> int select(int maxDescPlus1, fd_set *readDescs, fd_set *writeDescs,
    fd_set *execptionDescs, struct timeval *timeout)
* select()监视三个单独的描述符列表（注意：这些描述符可能引用常规的文件--比如终端输入--以及套接字）
* readDescs: 这个列表中的描述符用于检查即时的输入数据可用性；也就是说，调用recv()(或者recvfrom())将不会阻塞)。
* writeDescs: 这个列表中的描述符用于检查是否可以立即写数据；也就是说，调用send()(或者sendto())将不会阻塞)。
* exceptionDescs: 这个列表的描述符用于检查挂起的异常或者错误。（TCP套接字异常的一个例子，TCP套接字远端已经关闭，而数据仍然在信道中；在这种情况下，下一个读或写操作将会失败，并且会返回ECONNRESET）。
* 为任何描述符向量传递NULL将使得select()忽略I/O的类型。
* maxDescPlus1: 最大描述符值加1。
* timeout允许控制select()将为某件事情发生而等待多长时间。
* 如果经过了timeval结构中指定的时间之后，任何指定的描述符都还没有为I/O准备好，select()返回0。如果timeout时NULL，select()就没有超时限制，并且一直等待某个描述符做好准备为止。把tv_sec和tv_usec都置0将导致select()立即返回，从而允许轮询I/O描述符。
* 如果没有错误发生，select()返回I/O做好准备的描述符总个数。为指示描述符做好准备，select()会设置对应的位置。返回值-1指示select()中的错误。
* 系统提供给的宏，用于操作fd_set类型的实例：
> void FD_ZERO(fd_set *descriptorVector)
> void FD_CLR(int descriptor, fd_set *descriptorVector)
> void FD_SET(int descriptor, fd_set *descriptorVector)
> int FD_ISSET(int descriptor, fd_set *descriptorVector)
* FD_ZERO is used to clear descriptor list. 
* FD_CLR() and FD_SET() are used to delete and add descriptor respectively. 
* FD_ISSET()测试描述符在列表中的从属关系，如果给定的描述符在列表中，它就返回一个非0值；否则，就返回0.
* select()是一个功能强大的函数。它也可用于实现任何阻塞的I/O函数（例如，recv()，accept()）的超时版本，而无需使用报警。

## 揭秘
### 缓冲和TCP
* **使用TCP套接字时，不能假设写到连接一端的数据大小与从连接另一端读取的数据大小之间存在任何一致性。** 
* send() -> 底层SendQ缓冲 -> TCP协议 -> 底层RecvQ接收缓冲 -> recv() -> Delivered
* SendQ：在发送者的底层实现中缓冲的字节，它们已经写到输出流中，但还没有成功地传输到接收主机。
* RecvQ：在接收者的底层实现中缓冲的字节，等待递送到接收程序----即从输入流中读取。
* Delivered：接收者已经从输入流中读取的字节。
* 在接收端调用send()将向SendQ中追加字节。TCP协议负责移动字节---从SendQ中到RecvQ中。这种转移不受用户程序控制或者不能被它直接观察到，并且在块中发生，这些块的大小或多或少地独立于send()传入的缓冲区的大小，认识到这一点很重要。通过调用recv()把字节从RecvQ移到Delivered中；传输的块的大小依赖于RecvQ中的数据量以及提供给recv()的缓冲区的大小。
### 死锁风险
* 要仔细设计协议，以避免在两个方向上同时发送大量的数据。
### 关于性能
* TCP实现要求在发送用户数据之前按把它复制到SendQ中，这涉及到性能。
* SendQ和RecvQ缓冲区的大小会影响可以通过TCP连接实现的吞吐量（throughout）。
* 如果吞吐量时程序的一个重要性能指标，可以考虑使用SO_RCVBUF和SO_SNDBUF套接字选项更改发送和接收缓冲区的大小。
* NOTE：仅当程序需要的发送的数据量远远大于缓冲区大小时，才需要考虑这些情况。
* NOTE：在FILE流中包装TCP套接字将增加另一个缓冲阶段和额外的开销，从而可能会对吞吐量产生负面影响。
### TCP套接字的生存期
### 关闭TCP连接
* 关闭机制，允许应用程序在终止连接时不必担心可能仍在传输的数据会丢失。两个方向上的数据传输可以独立地终止。
* 工作方式：
1. 应用程序调用close()或者shutdown()指示完成数据发送。
1. 底层TCP首先将传输保留在SendQ中的数据（受到另一端RecvQ中的可用空间支配）
1. 然后向另一端发送一条关闭TCP的握手消息。该消息可以看作时流终止的标志。
1. 等待关闭消息的确认，一旦收到确认，该连接就变成半关闭状态（Half closed）。
1. 直到在连接的另一方向上收到了对称的握手消息后，连接才完全关闭。
* close()/shutdown()无需等待关闭握手完成即可返回，无法保证保留在SendQ中的数据确实到达另一端。
* 最佳的解决方案是设计一种应用协议，使得无论哪一端首先执行关闭，仅当它接收到关于数据已接收的应用程序级的保证后，它才这样做。
### 解多路复用揭秘

## 用C++进行套接字编程