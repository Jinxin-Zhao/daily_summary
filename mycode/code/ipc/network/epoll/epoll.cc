#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>

#define IPADDRESS "127.0.0.1"
#define PORT      8787
#define MAXSIZE   1024
#define LISTENQ   5
#define FDSIZE    1000
#define EPOLLEVENTS 100

static int socket_bind(const char * ip,int port);

static void do_epoll(int listenfd);

static void handle_events(int epollfd,struct epoll_event * event,int num,int listenfd,char *buf);

static void handle_accept(int epollfd,int listenfd);

static void do_read(int epollfd,int fd,char *buf);

static void do_write(int epollfd,int fd,char * buf);

static void modify_event(int epollfd,int fd,int state);

static void delete_event(int epollfd,int fd,int state);


//realize
static int socket_bind(const char * ip,int port)
{
    int listenfd;
    struct sockaddr_in servaddr;
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd == -1)
    {
        perror("Socketr error");
        exit(1);
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&servaddr.sin_addr);
    servaddr.sin_port = htonl(port);
    if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
    {
        perror("bind error: ");
        exit(1);
    }
    return listenfd;
}

static void add_event(int epollfd,int fd,int state)
{
    struct epoll_event * ev = new epoll_event();
    ev->events = state;
    ev->data.fd = fd;
    //EPOLL_CTL_ADD :注册新的fd到epfd中
    //EPOLL_CTL_MOD: 修改已经注册的fd监听事件
    //EPOLL_CTL_DEL: 从epfd中删除一个fd
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,ev);
}

static void delete_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
}


static void modify_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
}

static void do_epoll(int listenfd)
{
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    int ret;
    char buf[MAXSIZE];
    memset(buf,0,MAXSIZE);
    epollfd = epoll_create(FDSIZE);
    add_event(epollfd,listenfd,EPOLLIN);
    while(1)
    {
        ret = epoll_wait(epollfd,events,EPOLLEVENTS,-1);
        handle_events(epollfd,events,ret,listenfd,buf);
    }
    close(epollfd);
}

static void handle_events(int epollfd,struct epoll_event * events,int num,int listenfd,char * buf)
{
    int i;
    int fd;
    for(i = 0;i < num;++i)
    {
        fd = events[i].data.fd;
        if((fd == listenfd) && (events[i].events & EPOLLIN))
        {
            handle_accept(epollfd,listenfd);
        }
        else if(events[i].events & EPOLLIN)
        {
            do_read(epollfd,fd,buf);
        }else if(events[i].events & EPOLLOUT)
        {
            do_write(epollfd,fd,buf);
        }
    }
}

static void handle_accept(int epollfd,int listenfd)
{
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen;
    clifd = accept(listenfd,(struct sockaddr*)&cliaddr,&cliaddrlen);
    if(clifd == -1)
    {
        perror("accept error;");
    }
    else
    {
        printf("accept a new client:%s,%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
        add_event(epollfd,clifd,EPOLLIN);
    }
}


static void do_read(int epollfd,int fd,char * buf)
{
    int nread;
    nread = read(fd,buf,MAXSIZE);
    if(nread == -1)
    {
        perror("read error:");
        close(fd);
        delete_event(epollfd,fd,EPOLLIN);
    }
    else if(nread == 0)
    {
        fprintf(stderr, "client close.\n");
        close(fd);
        delete_event(epollfd,fd,EPOLLIN);
    }
    else
    {
        printf("read message is: %s",buf);
        //修改描述符对应的事件，由读改为写
        modify_event(epollfd,fd,EPOLLOUT);
    }
}

static void do_write(int epollfd,int fd,char * buf)
{
    int nwrite;
    nwrite = write(fd,buf,strlen(buf));
    if(nwrite == -1)
    {
        perror("write error:");
        close(fd);
        delete_event(epollfd,fd,EPOLLOUT);
    }
    else
    {
        modify_event(epollfd,fd,EPOLLIN);
    }
    memset(buf,0,MAXSIZE);
}

int main()
{
    int listenfd;
    listenfd = socket_bind(IPADDRESS,PORT);
    listen(listenfd,LISTENQ);
    do_epoll(listenfd);
    return 0;
}
