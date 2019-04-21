//client
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

int main()
{
    int sock = 0;
    if( (sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP) )< 0)
        ERR_EXIT("socket");
    //listenfd = socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(5188);  //port (need to switch to net order)

    servaddr.sin_addr.s_addr   = inet_addr("127.0.0.1");

    if( connect(sock,(struct sockaddr*)&servaddr,sizeof(servaddr) ) < 0)
    {
        ERR_EXIT("connet");
    }

    char sendbuf[1024] = {0};
    char recvbuf[1024] = {0};
    while(fgets(sendbuf,sizeof(sendbuf),stdin) != NULL)
    {
        writen(sock,sendbuf,strlen(sendbuf));

        int ret = readline(sock,recvbuf,sizeof(recvbuf));
        if(ret == -1)
            ERR_EXIT("readline");
        else if(ret == 0)
        {
            printf("client close\n");
            break;
        }
        fputs(recvbuf,stdout);
        memset(sendbuf,0,sizeof(sendbuf));
        memset(recvbuf,0,sizeof(recvbuf));
    }

    close(sock);

    return 0;
}
