[TOC]
#linux api
normal interfaces of linux kernel
##signal function
```
</usr/include/signal.h>
#define SIG_ERR ((__sighandler_t)-1)
#define SIG_DFL ((__sighandler_t)0)
#define SIG_IGN (__sighandler_t)1)

//程序终止(interrupt)信号, 在用户键入INTR字符(通常是Ctrl-C)时发出，用于通知前台进程组终止进程。
#define SIGINT  2
//1、double free/free 没有初始化的地址或者错误的地址
//2、堆越界
//3、assert
#define SIGABRT 6
//往一个已经接收到FIN的套接中写是允许的，接收到的FIN仅仅代表对方不再发送数据。并不能代表我不能发送数据给对方。
往一个FIN结束的进程中写(write)，对方会发送一个RST字段过来，TCP重置。如果再调用write就会产生SIGPIPE信号
#define SIGPIPE 13

typedef void (*__sighandler_t) (int)
signal(int __sig,__sighandler_t __handler)  

```
+ signal第一个参数是要捕捉的信号（查看信号：kill -l，9号SIGKILL信号不能被捕捉）;
第二个参数表示我们要对信号进行的处理方式。 
+ signal to ignore in main function:
    usage:
    - senario 1:
    ```
    int main(){
    #ifdef UNIX
        signal(SIGABRT,SIG_IGN);
        signal(SIGPIPE,SIG_IGN);
        signal(SIGINT,SIG_IGN);
    #endif
    ......
    return 0;
    }
    ```
    当执行程序时，陷入死循环，此时按下Ctrl+c进程并不会停止，因为我们对Ctrl+c产生的2号SIGINT信号采取了忽略处理，若要停止进程可用Ctrl+\（SIGQUIT）;
    - senario 2:
    ```
    int main(){
    #ifdef UNIX
        signal(SIGABRT,SIG_DFL);
        signal(SIGPIPE,SIG_DFL);
        signal(SIGINT,SIG_DFL);
    #endif
    ......
    return 0;
    }
    ```
    当执行程序时，同样是死循环，此时按下Ctrl+c进程停止，因为我们对2号信号采取默认动作处理，系统默认2号信号终止进程。 
    -senario 3:
     ```
     void  handler(int signo)//自定义一个函数处理信号
     {
       printf("catch a signal:%d\n:",signo);
     }
    inint getopt(int argc,char * const argv[ ],const char * optstring);t main(){
    #iint getopt(int argc,char * const argv[ ],const char * optstring);fdef UNIX
      int getopt(int argc,char * const argv[ ],const char * optstring);  signal(SIGABRT,handler);
        signal(SIGPIPE,handler);
        signal(SIGINT,handler);
    #endif
    ......
    return 0;
    }
    ```
    当执行程序时，按下Ctrl+c，进程并不会停止，而是去执行自定义的函数handler(),若要停止，按下Ctrl+\。
##getopt funtion
```
<unistd.h>
int getopt(int argc,char * const argv[ ],const char * optstring);
```
+ usage:
```
int main(int argc,int **argv){
    int c = 0;
    while((c = getopt(argc-1,argv+1,"s:d:r:t:m")) != EOF){
        switch(c){
            case ('s'):
            ......
            break;
            case ('d'):
            ......
            break;
            .....

        }
    }
}

]# exe -s arg
```

+ "s:d:r:t:m"参数分析:
    - 一个冒号就表示这个选项后面必须带有参数（没有带参数会报错哦）,但是这个参数可以和选项连在一起写，也可以用空格隔开，比如-a123 和-a   123（中间有空格） 都表示123是-a的参数;
    - 两个冒号的就表示这个选项的参数是可选的，即可以有参数，也可以没有参数，但要注意有参数时，参数与选项之间不能有空格（有空格会报错的哦），这一点和一个冒号时是有区别的。



////////
Ctrl + C 是强制中断程序的执行,进程已经终止。 Ctrl + C 发送 SIGINT信号 参考：linux信号
 
Ctrl + Z 的是将任务中止（暂停的意思）,但是此任务并没有结束,他仍然在进程中他只是维持挂起的状态,用户可以使用fg/bg操作继续前台或后台的任务,fg命令重新启动前台被中断的任务,bg命令把被中断的任务放在后台执行.  参考：bash工作管理
 
例如:当你vi一个文件是,如果需要用shell执行别的操作,但是你又不打算关闭vi,因为你得存盘推出,你可以简单的按下ctrl+z,shell会将vi进程挂起~,当你结束了那个shell操作之后,你可以用fg命令继续vi你的文件。
 
Ctrl + D 不是发送信号，而是表示一个特殊的二进制值，表示 EOF。
注：在shell中，ctrl-d表示推出当前shell.    参考cat用法





