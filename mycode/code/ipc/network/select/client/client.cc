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

//read n bytes from a descriptor
ssize_t readn(int fd,void * vptr,size_t n)
{
    size_t nleft;
    ssize_t nread;
    char *ptr = vptr;
    nleft = n;

    while(nleft > 0)
    {
        if( (nread = read(fd,ptr,nleft)) < 0)
        {
            if(errno == EINTR)
                nread = 0;
            else
                return -1;
        }
        else if(nread == 0)
        {
            break;   //EOF
        }
        nleft -= nread;
        vptr += nread;
    }
    return (n - nleft);
}

//attention the second parameter is const
ssize_t writen(int fd,const void *vptr,size_t n)
{
    size_t nleft = n;
    ssize_t nwritten = 0;
    const char * ptr = vptr;
    while(nleft > 0)
    {
        if( (nwritten = write(fd,ptr,nleft)) <= 0)
        {
            if(nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}


//readline:use recv_peek function
ssize_t readline(int sockfd,void * buf,size_t maxline)
{
    int ret;
    int nread;tg
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

void echo_cli(int sock)
{
    char sendbuf[1024] = {0};
    char recvbuf[1024] = {0};

    fd_set rset;
    FD_ZERO(&rset);
    int nready;
    int maxfd;
    int fd_stdin = fileno(stdin);
    if(fd_stdin > sock)
        maxfd = fd_stdin;
    else
        maxfd = sock;
    while(1)
    {
        FD_SET(fd_stdin,&rset);
        FD_SET(sock,&rset);
        nready = select(maxfd+1,&rset,NULL,NULL,NULL);
        if(nready == -1)
        {
            ERR_EXIT("select");
        }
        //超时
        if(nready == 0)
            continue;
        //normal situation
         if(FD_ISSET(sock,&rset))
         {
            int ret = readline(sock,recvbuf,sizeof(recvbuf));
            if(ret == -1)
                ERR_EXIT("readline");
            else if(ret == 0)
            {
                printf("server closed\n");
                break;
            }
            fputs(recvbuf,stdout);
            memset(sendbuf,0,sizeof(sendbuf));
            memset(recvbuf,0,sizeof(recvbuf));
         }
         if(FD_ISSET(fd_stdin,&rset))
         {
            if(fgets(sendbuf,sizeof(sendbuf),stdin) == NULL)
                break;
            writen(sock,sendbuf,strlen(sendbuf));
         }
    }
    close(sock);
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

    struct  sockaddr_in localaddr;
    socklen_t addrlen = sizeof(localaddr);
    if(getsockname(sock,(struct sockaddr *)&localaddr,&addrlen) < 0)
        ERR_EXIT("getsockname");
    printf("ip=%s port=%d\n",inet_ntoa(localaddr.sin_addr),ntohs(localaddr.sin_port));
    echo_cli(sock);

    return 0;
}
