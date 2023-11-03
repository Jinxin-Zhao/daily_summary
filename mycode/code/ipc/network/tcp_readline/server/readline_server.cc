//test host/network byte order
#include <stdio.h>
#include <arpa/inet.h>

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


//recv&&read
ssize_t recv_peek(int sockfd,void * buf,size_t len)
{
    while(1)
    {
        //MSG_PEEK:可以接受数据到buf，但并不从buf中清除数据，而read读完会清除
        int ret = recv(sockfd,buf,len,MSG_PEEK);
        if(ret == -1 && errno == EINTR)
            continue;
        return ret;
    }
}

//readline:use recv_peek function
ssize_t readline(int sockfd,void * buf,size_t maxline)
{
    int ret;
    int nread;
    char * bufp = (char*)buf;
    int nleft = maxline;
    while(1)
    {
        ret = recv_peek(sockfd,bufp,nleft);
        if(ret < 0)
            return ret;
        //表示对方关闭了套接口
        else if(ret == 0)
            return ret;
        nread = ret;
        int i;
        for(i = 0;i < nread;i++)
        {
            if(bufp[i] == '\n')
            {
                //这个函数直接读完这些数据就把这些数据清掉了
                ret = readn(sockfd,bufp,i+1);
                if(ret != i+1)
                    exit(EXIT_FAILURE);
                return ret;
            }
        }
        if(nread > nleft)
        {
            exit(EXIT_FAILURE);
        }
        nleft -= nread;
        ret = readn(sockfd,bufp,nread);
        if(ret != nread)
        {
            exit(EXIT_FAILURE);
        }
        bufp += nread;
    }
    return -1;
}


void do_service(int conn)
{
    char recvbuf[1024];
    while(1)
    {
        memset(recvbuf,0,sizeof(recvbuf));
        int ret = readline(conn,recvbuf,1024);
        if(ret == -1)
        {
            ERR_EXIT("readline");
        }
        if(ret == 0)
        {
            //client closed
            printf("client closed\n");
            break;
        }
        fputs(recvbuf,stdout);
        write(conn,recvbuf,strlen(recvbuf));
    }

    close(conn);
}


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

    pid_t pid;
    while(1)
    {
        if( (conn = accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen) ) < 0)
        {
            ERR_EXIT("accept");
        }
    printf("ip=%s port=%d\n",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
    //conn is called 已连接套接字(主动)

    pid = fork();
    if(pid == -1)
    {
        ERR_EXIT("fork");
    }
    if(pid == 0)
    {
        //sub_process
        close(listenfd);
        do_service(conn);
        exit(EXIT_SUCCESS);
    }else
    {
        close(conn);
    }
 }

    close(conn);
    close(listenfd);
    return 0;
}
