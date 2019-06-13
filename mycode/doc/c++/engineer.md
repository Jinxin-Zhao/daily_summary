<!-- TOC -->

[1. Timer](#1-Timer)
[2. getUseFlag](#2-getUseFlag)

<!-- TOC -->


# 1. Timer
## simple timer
+ sleep:
    - 其中sleep精度是1秒，usleep精度是1微妙，具体代码就不写了。使用这种方法缺点比较明显，在Linux系统中，sleep类函数不能保证精度，尤其在系统负载比较大时，sleep一般都会有超时现象。
+ SIGALRM+alarm():
    - 这种方式的精度能达到1秒，其中利用了*nix系统的信号量机制，首先注册信号量SIGALRM处理函数，调用alarm()，设置定时长度，代码如下：
```cpp
#include <stdio.h>  
#include <signal.h>  
  
void timer(int sig)  
{  
        if(SIGALRM == sig)  
        {  
                printf("timer\n");  
                alarm(1);       //we contimue set the timer  
        }  
  
        return ;  
}  
  
int main()  
{  
        signal(SIGALRM, timer); //relate the signal and function  
  
        alarm(1);       //trigger the timer  
  
        getchar();  
  
        return 0;  
}  
缺点： 无法实现低于1秒的精度
```


+ RTC机制
    - 利用系统硬件提供的Real Time Clock机制，通过读取RTC硬件/dev/rtc,通过iocl()设置RTC频率：
```cpp
#include <stdio.h>  
#include <linux/rtc.h>  
#include <sys/ioctl.h>  
#include <sys/time.h>  
#include <sys/types.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <errno.h>  
#include <stdlib.h>  
  
int main(int argc, char* argv[])  
{  
        unsigned long i = 0;  
        unsigned long data = 0;  
        int retval = 0;  
        int fd = open ("/dev/rtc", O_RDONLY);  
  
        if(fd < 0)  
        {  
                perror("open");  
                exit(errno);  
        }  
  
        /*Set the freq as 4Hz*/  
        if(ioctl(fd, RTC_IRQP_SET, 1) < 0)  
        {  
                perror("ioctl(RTC_IRQP_SET)");  
                close(fd);  
                exit(errno);  
        }  
        /* Enable periodic interrupts */  
        if(ioctl(fd, RTC_PIE_ON, 0) < 0)  
        {  
                perror("ioctl(RTC_PIE_ON)");  
                close(fd);  
                exit(errno);  
        }  
  
        for(i = 0; i < 100; i++)  
        {  
                if(read(fd, &data, sizeof(unsigned long)) < 0)  
                {  
                        perror("read");  
                        close(fd);  
                        exit(errno);  
  
                }  
                printf("timer\n");  
        }  
        /* Disable periodic interrupts */  
        ioctl(fd, RTC_PIE_OFF, 0);  
        close(fd);  
  
        return 0;  
}  
```

+ slect()
    - 通过使用select()，来设置定时器；原理利用select()方法的第5个参数，第一个参数设置为0，三个文件描述符集都设置为NULL，第5个参数为时间结构体:
```cpp
#include <sys/time.h>  
#include <sys/select.h>  
#include <time.h>  
#include <stdio.h>  
  
/*seconds: the seconds; mseconds: the micro seconds*/  
void setTimer(int seconds, int mseconds)  
{  
        struct timeval temp;  
  
        temp.tv_sec = seconds;  
        temp.tv_usec = mseconds;  
  
        select(0, NULL, NULL, NULL, &temp);  
        printf("timer\n");  
  
        return ;  
}  
  
int main()  
{  
        int i;  
  
        for(i = 0 ; i < 100; i++)  
                setTimer(1, 0);  
  
        return 0;  
}  
```


## timer in project
+ 在开发高性能服务器中，定时器总是不可或缺的。 常见的定时器实现三种，分别是：
**排序链表，最小堆，时间轮。** 最小堆:在定时器数量不多时可以使用;
目前公司用的框架中的定时器是基于简单时间轮的，但是为了支持大范围的时间，每个齿轮的所维护的链表为有序链表，每次插入时先mod出spoke，再从头遍历链表以便将定时器插入到合适位置， 所以本质上还是基于有序链表的。时间复杂度并未减少。



# 2. getUseFlag
## 
```cpp

#define R_INSERT 0
#define R_UPDATE 1
#define R_CANCEL 2
#define R_USELESS 3

#define INSERT (1<<R_INSERT)
#define UPDATE (1<<R_UPDATE)
#define CANCEL (1<<R_CANCEL)

int getUse(){
    int result = 0;
    result |= R_INSERT;
    result |= R_UPDATE;
    result |= R_CANCEL;
    return result;
}

int use = getUse();
for(int i= 0; i < R_USELESS;i++){
    if(use & (1<<i)){
        //means this rule is in use;
        rules[i].push_back(thisRuleUser);
    }
}

```