//test host/network byte order
#include <stdio.h>
#include <arpa/inet.h>

// int main()
// {
//     unsigned int x = 0x12345678;
//     unsigned char * p = (unsigned char*)&x;
//     printf("%0x %0x %0x %0x\n",p[0],p[1],p[2],p[3]); //x86 host small btye

//     unsigned int y = htonl(x);
//     p = (unsigned char*)&y;
//     //netbyte order big
//     printf("%0x %0x %0x %0x\n",p[0],p[1],p[2],p[3]);

//     unsigned long addr = inet_addr("192.168.0.100");
//     //the addr is netbtye order ,switch to host order below
//     printf("addr=%u\n",ntohl(addr));

//     struct in_addr ipaddr;
//     ipaddr.s_addr = addr;
//     printf("%s\n",inet_ntoa(ipaddr));

//     return 0;
// }
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m)                 \
            do                      \
            {                       \
                perror(m);          \
                exit(EXIT_FAILURE); \
            }while(0)               \

int main()
{
    int listenfd = 0;
    if( (listenfd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP) )< 0)
        ERR_EXIT("socket");
    //listenfd = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(5188);  //port (need to switch to net order)

    servaddr.sin_addr.s_addr   = htonl(INADDR_ANY);  //the INADDR_ANY is all ,so you can also ignore htonl
    //servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //inet_aton("127.0.0.1",&servaddr.sin_addr);


//REUSERADDR
    int on = 1; //1 denotes 开启
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on) ) < 0 )
    {
        ERR_EXIT("setsockopt");
    }
    if( bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr) ) < 0)
    {
        ERR_EXIT("bind");
    }

    if(listen(listenfd,SOMAXCONN) < 0)
    {
        ERR_EXIT("listen");
    }
    //after calling listen ,the sock from 主动socket-->被动套接字,而主动套接字是发起连接的socket

    struct sockaddr_in peeraddr;
    socklen_t peerlen = sizeof(peeraddr);
    int conn = 0;
    if( (conn = accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen) ) < 0)
    {
        ERR_EXIT("accept");
    }
    printf("ip=%s port=%d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
    //conn is called 已连接套接字(主动)
    char recvbuf[1024];
    while(1)
    {
        memset(recvbuf,0,sizeof(recvbuf));
        int ret = read(conn,recvbuf,sizeof(recvbuf));
        fputs(recvbuf,stdout);
        write(conn,recvbuf,ret);
    }

    close(conn);
    close(listenfd);
    return 0;
}
