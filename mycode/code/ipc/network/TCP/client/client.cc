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
        write(sock,sendbuf,strlen(sendbuf));
        read(sock,recvbuf,sizeof(recvbuf));

        fputs(recvbuf,stdout);
        memset(sendbuf,0,sizeof(sendbuf));
        memset(recvbuf,0,sizeof(recvbuf));
    }

    close(sock);

    return 0;
}
