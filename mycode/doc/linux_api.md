<!-- TOC -->

- [signal function](#signal-function)
- [getopt function](#getopt-function)
- [malloc](#malloc)
- [process communication](#process-communication)
  - [pipe](#pipe)
  - [name pipe（有名管道）](#name-pipe有名管道)
- [interrupt](#interrupt)
    - [hard interrupt](hard-interrupt)
    - [soft interrupt](soft-interrupt)
- [semaphore](#semaphore)
<!-- TOC -->

###SystemV_IPC vs POSIX_IPC 
####History
+ UNIX两大贡献者贝尔实验室和BSD，在进程之间通信侧重不同，前者基于内核对进程之间的通信手段进行了改进，形成了“System V IPC”，而后者则是基于网络形成了套接字.
+ 而POSIX则是IEEE制定的标准，目的是为运行在不同操作系统上的软件提供统一的接口，实现者则是不同的操作系统内核开发人员。
#### Efficiency
+ 在信号量这种常用的同步互斥手段方面，POSIX在无竞争条件下是不会陷入内核的，而SYSTEM V则是无论何时都要陷入内核，因此性能稍差。
#### Redundancy
+ POSIX的sem_wait函数成功获取信号量后，进程如果意外终止，将无法释放信号量，而System V则提供了SEM_UNDO选项来解决这个问题。因此，相比而言，后者更加可靠

# disk partition
+ command 'fdisk -l'
  ```shell
	Disk /dev/vda: 100 GiB, 107374182400 bytes, 209715200 sectors
	Units: sectors of 1 * 512 = 512 bytes
	Sector size (logical/physical): 512 bytes / 512 bytes
	I/O size (minimum/optimal): 512 bytes / 512 bytes
	Disklabel type: dos
	Disk identifier: 0xf4bb72a5

	Device     Boot Start       End   Sectors  Size Id Type
	/dev/vda1        2048 209715166 209713119  100G 83 Linux

	Disk /dev/vdb: 3.91 TiB, 4294967296000 bytes, 8388608000 sectors
	Units: sectors of 1 * 512 = 512 bytes
	Sector size (logical/physical): 512 bytes / 512 bytes
	I/O size (minimum/optimal): 512 bytes / 512 bytes
	Disklabel type: gpt
	Disk identifier: 83B6FD8D-BF73-45C6-AE49-265E027737AA

	Device     Start        End    Sectors  Size Type
	/dev/vdb1   2048 8388605951 8388603904  3.9T Linux filesystem
  ```
+ normal partition by using fdisk command
  ```shell
	]$ sudo fdisk /dev/vdb
  	Welcome to fdisk (util-linux 2.37.2).
	Changes will remain in memory only, until you decide to write them.
	Be careful before using the write command.

	Command (m for help): n
	Partition number (1-128, default 1): 1
	First sector (34-8388607966, default 2048):
	Last sector, +/-sectors or +/-size{K,M,G,T,P} (2048-8388607966, default 8388607966): +3T

	Created a new partition 1 of type 'Linux filesystem' and of size 3 TiB.
	Partition #1 contains a ext4 signature.

	Do you want to remove the signature? [Y]es/[N]o: Y

	The signature will be removed by a write command.

	Command (m for help): wq
	The partition table has been altered.
	Calling ioctl() to re-read partition table.
	Syncing disks.
	#################################################################################################
  	]$ sudo fdisk /dev/vdb
  	Welcome to fdisk (util-linux 2.37.2).
	Changes will remain in memory only, until you decide to write them.
	Be careful before using the write command.

	Command (m for help): n
	Partition number (2-128, default 2): 2
	First sector (6442452992-8388607966, default 6442452992):
	Last sector, +/-sectors or +/-size{K,M,G,T,P} (6442452992-8388607966, default 8388607966):

	Created a new partition 2 of type 'Linux filesystem' and of size 928 GiB.

	Command (m for help): wq
	The partition table has been altered.
	Calling ioctl() to re-read partition table.
	Syncing disks.
	#################################################################################################
  	]$ sudo fdisk -l
  	Disk /dev/vda: 100 GiB, 107374182400 bytes, 209715200 sectors
	Units: sectors of 1 * 512 = 512 bytes
	Sector size (logical/physical): 512 bytes / 512 bytes
	I/O size (minimum/optimal): 512 bytes / 512 bytes
	Disklabel type: dos
	Disk identifier: 0xf4bb72a5

	Device     Boot Start       End   Sectors  Size Id Type
	/dev/vda1        2048 209715166 209713119  100G 83 Linux


	Disk /dev/vdb: 3.91 TiB, 4294967296000 bytes, 8388608000 sectors
	Units: sectors of 1 * 512 = 512 bytes
	Sector size (logical/physical): 512 bytes / 512 bytes
	I/O size (minimum/optimal): 512 bytes / 512 bytes
	Disklabel type: gpt
	Disk identifier: C392134C-B132-406D-A449-6D13175269FE

	Device          Start        End    Sectors  Size Type
	/dev/vdb1        2048 6442452991 6442450944    3T Linux filesystem
	/dev/vdb2  6442452992 8388607966 1946154975  928G Linux filesystem
  	#################################################################################################
  	]$ sudo mkfs.ext4 /dev/vdb1
  	]$ sudo mkfs.ext4 /dev/vbd2
	]$ lsblk -f # check the filesystem format of each disk block
  	]$ sudo mount /dev/vdb1 /mnt/first
  	]$ sudo mount /dev/vdb2 /mnt/second
  ```
+ If you want to create a disk whose volume > 2T, you'd better use parted tool following the next steps:
	- providing we have a new disk dev '/dev/vdb' (volume = 3.91TiB > 2TiB), run command [$ sudo parted /dev/vdb]
 	- (parted) mklabel gpt (creating gpt partition tables)
  	- (parted) mkpart primary ext4 0% 100%  #create a new partition 3.91TiB
   	- (parted) quit
    	- sudo mkfs.ext4 /dev/vdb # creating file system
     	- sudo mount -t ext4 /dev/vdb1 /opt # mount the disk
        - df -h # check if the operation mount is executed successfully


# signal function
+ how the signal comes out:
    it can:
    - be sent to process B by process A;
    - be sent to processes by kernel;
    such as 'SIGCHLD' signal,it's sent by kernel to parent process;
+ special signal:
    + 'SIGKILL' & 'SIGSTOP' can't be ignored;
+ normal interface of linux:
    ```cpp
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
    ```cpp
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
    ```cpp
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
    - senario 3:
     ```cpp
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

# getopt function
```cpp
    //<unistd.h>
    int getopt(int argc,char * const argv[ ],const char *  optstring);
```
+ usage:
```cpp
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


# malloc
```cpp
void * malloc(size_t size);
//return: if success "p to memory address" else null

// 这两个函数的作用主要是扩展heap的上界brk。第一个函数的参数为设置的新的brk上界地址，如果成功返回0，失败返回-1。第二个函数的参数为需要申请的内存的大小，然后返回heap新的上界brk地址。如果sbrk的参数为0，则返回的为原来的brk地址。
int brk(const void * addr)
void * sbrk (intptr_t incr)

//mmap函数第一种用法是映射磁盘文件到内存中（前面讲进程通信的时候讲过）；而malloc使用的mmap函数的第二种用法，即匿名映射，匿名映射不映射磁盘文件，而是向映射区申请一块内存。
//这里要注意的是fd参数，fd为映射的文件描述符，如果是匿名映射，可以设为-1
//当申请小内存的时，malloc使用sbrk分配内存；当申请大内存时，使用mmap函数申请内存；但是这只是分配了虚拟内存，还没有映射到物理内存，当访问申请的内存时，才会因为缺页异常，内核分配物理内存。
void * mmap(void * addr, size_t length, int prot, int flags, int fd, off_t offset)
int munmap(void * addr, size_t length)
```

+ malloc函数返回一个指针，指向大小至少为size字节的内存块，这个块会为可能包含在这个块内的任何数据对象类型做对齐。（这个会根据系统位数返回相应规格的地址）.
如果malloc遇到问题（例如，程序要求的内存块比可用的虚拟内存还要大），那么它就返回null.并设置errno。malloc不初始化它返回的内存。

+ 由于brk/sbrk/mmap属于系统调用，如果每次申请内存，都调用这三个函数中的一个，那么每次都要产生系统调用开销（即cpu从用户态切换到内核态的上下文切换，这里要保存用户态数据，等会还要切换回用户态），这是非常影响性能的；其次，这样申请的内存容易产生碎片，因为堆是从低地址到高地址，如果低地址的内存没有被释放，高地址的内存就不能被回收。
+ 鉴于此，malloc采用的是内存池的实现方式，malloc内存池实现方式更类似于STL分配器和memcached的内存池，先申请一大块内存，然后将内存分成不同大小的内存块，然后用户申请内存时，直接从内存池中选择一块相近的内存块即可。
+ malloc将内存分成了大小不同的chunk，然后通过bins来组织起来。malloc将相似大小的chunk（图中可以看出同一链表上的chunk大小差不多）用双向链表链接起来，这样一个链表被称为一个bin。malloc一共维护了128个bin，并使用一个数组来存储这些bin。数组中第一个为unsorted bin,数组从2开始编号，前64个bin为small bins，同一个small bin中的chunk具有相同的大小，两个相邻的small bin中的chunk大小相差8bytes。small bins后面的bin被称作large bins。large bins中的每一个bin分别包含了一个给定范围内的chunk，其中的chunk按大小序排列。large bin的每个bin相差64字节。
+ malloc除了有unsorted bin，small bin,large bin三个bin之外，还有一个fast bin。一般的情况是,程序在运行时会经常需要申请和释放一些较小的内存空间。当分配器合并了相邻的几个小的 chunk 之后,也许马上就会有另一个小块内存的请求,这样分配器又需要从大的空闲内存中切分出一块,这样无疑是比较低效的,故而,malloc 中在分配过程中引入了 fast bins,不大于 max_fast(默认值为 64B)的 chunk 被释放后,首先会被放到 fast bins中,fast bins 中的 chunk 并不改变它的使用标志 P。这样也就无法将它们合并,当需要给用户分配的 chunk 小于或等于 max_fast 时,malloc 首先会在 fast bins 中查找相应的空闲块,然后才会去查找 bins 中的空闲 chunk。在某个特定的时候,malloc 会遍历 fast bins 中的 chunk,将相邻的空闲 chunk 进行合并,并将合并后的 chunk 加入 unsorted bin 中,然后再将 usorted bin 里的 chunk 加入 bins 中。
unsorted bin 的队列使用 bins 数组的第一个,如果被用户释放的 chunk 大于 max_fast,或者 fast bins 中的空闲 chunk 合并后,这些 chunk 首先会被放到 unsorted bin 队列中,在进行 malloc 操作的时候,如果在 fast bins 中没有找到合适的 chunk,则malloc 会先在 unsorted bin 中查找合适的空闲 chunk,然后才查找 bins。如果 unsorted bin 不能满足分配要求。 malloc便会将 unsorted bin 中的 chunk 加入 bins 中。然后再从 bins 中继续进行查找和分配过程。从这个过程可以看出来,unsorted bin 可以看做是 bins 的一个缓冲区,增加它只是为了加快分配的速度。
一开始时，brk和start_brk是相等的，这时实际heap大小为0；如果第一次用户请求的内存大小小于mmap分配阈值，则malloc会申请(chunk_size+128kb) align 4kb大小的空间作为初始的heap。初始化heap之后，第二次申请的内存如果还是小于mmap分配阈值时，malloc会先查找fast bins,如果不能找到匹配的chunk，则查找small bins。若还是不行，合并fast bins,把chunk 加入到unsorted bin，在unsorted bin中查找，若还是不行，把unsorted bin中的chunk全加入large bins中，并查找large bins。在fast bins和small bins中查找都需要精确匹配，而在large bins中查找时，则遵循"smalest-first,best-fit"的原则，不需要精确匹配。

# process communication
## pipe
+ 管道的容量大小为65536 。也就是64K.
+ 1. 通信过程中，别的进程先结束后，当前进程读端口关闭后，向管道内写数据时，write() 所在进程会（收到 SIGPIPE 信号）退出，收到 SIGPIPE 默认动作为中断当前进程。
	```cpp
	if( pid == 0 ){ // 子进程
		//close(fd_pipe[0]);
		_exit(0);
	}else if( pid > 0 ){// 父进程
		wait(NULL);	// 等待子进程结束，回收其资源
		close(fd_pipe[0]); // 当前进程读端口关闭
		char buf[50] = "12345";
		// 当前进程读端口关闭
		// write()会收到 SIGPIPE 信号，默认动作为中断当前进程
		write(fd_pipe[1], buf, strlen(buf));
		while(1);	// 阻塞
	}

	```
+ 2. 调用 write() 函数向管道里写数据，当缓冲区已满时 write() 也会阻塞。
	```cpp
	if( pid == 0 ){ // 子进程
		while(1){
			write(fd_pipe[1], buf, sizeof(buf));
			i++;
			printf("i ======== %d\n", i);
		}
		_exit(0);
	}else if( pid > 0){// 父进程
		wait(NULL);	// 等待子进程结束，回收其资源
	}		
	// i 一直print到64
	```
## name pipe（有名管道）
+ 有名管道通过如下指令创建:
	```shell
	]$ mkfifo myfifo
	```
+ 我们要注意这个文件的存在形式：它只是一个节点，没有真正的文件大小，这个节点用来标记内核当中的一个pipe。 所以，说白了，fifo还是使用的pipe。只不过没有了血缘关系的要求。


////////
Ctrl + C 是强制中断程序的执行,进程已经终止。 Ctrl + C 发送 SIGINT信号 参考：linux信号
 
Ctrl + Z 的是将任务中止（暂停的意思）,但是此任务并没有结束,他仍然在进程中他只是维持挂起的状态,用户可以使用fg/bg操作继续前台或后台的任务,fg命令重新启动前台被中断的任务,bg命令把被中断的任务放在后台执行.  参考：bash工作管理
 
例如:当你vi一个文件是,如果需要用shell执行别的操作,但是你又不打算关闭vi,因为你得存盘推出,你可以简单的按下ctrl+z,shell会将vi进程挂起~,当你结束了那个shell操作之后,你可以用fg命令继续vi你的文件。
 
Ctrl + D 不是发送信号，而是表示一个特殊的二进制值，表示 EOF。
注：在shell中，ctrl-d表示推出当前shell.    参考cat用法


# interrupt
+ 从本质上来讲，中断是一种电信号，当设备有某种事件发生时，它就会产生中断，通过总线把电信号发送给中断控制器。
如果中断的线是激活的，中断控制器就把电信号发送给处理器的某个特定引脚。处理器于是立即停止自己正在做的事，跳到中断处理程序的入口点，进行中断处理。

## hard interrupt
+ 由与系统相连的外设(比如网卡、硬盘)自动产生的。主要是用来通知操作系统系统外设状态的变化。比如当网卡收到数据包的时候，就会发出一个中断。我们通常所说的中断指的是硬中断(hardirq)。


## soft interrupt
+ 为了满足实时系统的要求，中断处理应该是越快越好。linux为了实现这个特点，当中断发生的时候，硬中断处理那些短时间就可以完成的工作，而将那些处理事件比较长的工作，放到中断之后来完成，也就是软中断(softirq)来完成。


Linux下硬中断是可以嵌套的，但是没有优先级的概念，也就是说任何一个新的中断都可以打断正在执行的中断，但同种中断除外。软中断不能嵌套，但相同类型的软中断可以在不同CPU上并行执行。
int是软中断指令。
中断向量表是中断号和中断处理函数地址的对应表。
int n - 触发软中断n。相应的中断处理函数的地址为：中断向量表地址 + 4 * n。
软中断是执行中断指令产生的，而硬中断是由外设引发的。
硬中断的中断号是由中断控制器提供的，软中断的中断号由指令直接指出，无需使用中断控制器。
硬中断是可屏蔽的，软中断不可屏蔽。
硬中断处理程序要确保它能快速地完成任务，这样程序执行时才不会等待较长时间，称为上半部。
软中断处理硬中断未完成的工作，是一种推后执行的机制，属于下半部。


# semaphore
+ 由于信号量只能进行两种操作等待和发送信号，即P(sv)和V(sv),他们的行为是这样的:
    - P(sv): 如果sv的值大于0，就给他减1；如果sv值为0，就挂起该进程的执行；
    - V(sv): 如果有其他进程因等待sv而被挂起，就让她恢复运行，如果没有进程因等待sv而挂起就给他加1；
+ linux提供两种信号量:
    - 内核信号量，由内核控制路径使用；
    - 用户态进程使用的信号量，这种信号量分为POSIX和SYSTEM V信号量；
    POSIX信号量分为：
    有名信号量：其值保存在文件中，所以它可以用于线程也可以用于进程间同步；
    无名信号量：其值保存在内存中；

    - POSIX信号量和SYSTEM V信号量对比:
        - 对于POSIX来说，信号量是个非负整数，常用于线程间同步。而SYSTEM V信号量则是一个或多个信号量的集合，它对应的是一个信号量的结构体，这个结构体是为SYSTEM V IPC服务的，信号量只不过是这个结构体的一部分，常用于进程间同步。
        - POSIX信号量的引用头文件是<semaphore.h>,SYSTEM V信号量的头文件是<sys/sem.h>。
        - 从使用角度来说，System V信号量是复杂的，而POSIX信号量比较简单。
## 内核信号量
Linux内核的信号量在概念和原理上与用户态的SystemV的IPC机制信号量是一样的，但它绝不可能在内核之外使用，他是一种睡眠锁。
它与自旋锁的差异：由于争用信号量的进程在等待锁重新变为可用时会睡眠，所以信号量适用于锁会被长时间持有的情况；不能同时占用信号量和自旋锁，因为自旋锁不可以睡眠而信号量锁可以睡眠。相对而来说信号量比较简单，它不会禁止内核抢占，持有信号量的代码可以被抢占。
信号量还有一个特征，就是它允许多个持有者，而自旋锁在任何时候只能允许一个持有者。
当然我们经常遇到也是只有一个持有者，这种信号量叫二值信号量或者叫互斥信号量。允许有多个持有者的信号量叫计数信号量，在初始化时要说明最多允许有多少个持有者（Count值）
信号量在创建时需要设置一个初始值，表示同时可以有几个任务可以访问该信号量保护的共享资源，初始值为1就变成互斥锁（Mutex），即同时只能有一个任务可以访问信号量保护的共享资源。
当任务访问完被信号量保护的共享资源后，必须释放信号量，释放信号量通过把信号量的值加1实现，如果信号量的值为非正数，表明有任务等待当前信号量，因此它也唤醒所有等待该信号量的任务。
- 内核信号量构成:
内核信号量类似于自旋锁，因为当锁关闭着时，它不允许内核控制路径继续进行,当内核控制路径试图获取内核信号量锁保护的忙资源时，相应的进程就被挂起.只有在资源被释放时，进程才再次变为可运行。只有可以睡眠的函数才能获取内核信号量；中断处理程序和可延迟函数都不能使用内核信号量。
内核信号量是struct semaphore类型的对象，在内核源码中位于include\linux\semaphore.h文件
```cpp
struct semaphore {
    atomic_t count; // 信号量的值，>0表示资源空闲，等于0表示资源忙，但没有进程等待这个保护的资源；<0表示资源不可用，且至少有一个进程等待资源。
    int sleepers; //标志，表示是否有一些进程在信号量上睡眠
    wait_queue_head_t wait; //存放等待队列链表的地址，当前等待资源的所有睡眠进程都会放在这个链表中。
}
// 内核信号量中的等待队列：
// 当某任务由于没有某种条件没有得到满足时，它就被挂到等待队列中睡眠。当条件得到满足时，该任务就被移出等待队列，此时并不意味着该任务就被马上执行，因为它又被移进工作队列中等待CPU资源，在适当的时机被调度。
// 内核信号量是在内部使用等待队列的，也就是说该等待队列对用户是隐藏的，无须用户干涉

// 该宏声明一个互斥锁name，但把它的初始值设置为1
#define DECLARE_MUTEX(name)     \
        struct semaphore name = __SEMAPHORE_INITIALIZER(name, 1)

// 该函用于数初始化设置信号量的初值，它设置信号量sem的值为val。
void sema_init (struct semaphore *sem, int val);

// 该函数用于初始化一个互斥锁，即它把信号量sem的值设置为1。
#define init_MUTEX(sem)         sema_init(sem, 1)

// 该函数也用于初始化一个互斥锁，但它把信号量sem的值设置为0，即一开始就处在已锁状态。
#define init_MUTEX_LOCKED(sem)  sema_init(sem, 0)
// 注意：对于信号量的初始化函数Linux最新版本存在变化，如init_MUTEX和init_MUTEX_LOCKED等初始化函数目前新的内核中已经没有或者更换了了名字等
// 因此建议以后在编程中遇到需要使用信号量的时候尽量采用sema_init(struct semaphore *sem, int val)函数，因为这个函数就目前为止从未发生变化。
```

+ 内核获取信号量-申请内核信号量所保护的资源
```cpp
// 该函数用于获得信号量sem，它会导致睡眠，因此不能在中断上下文（包括IRQ上下文和softirq上下文）使用该函数。该函数将把sem的值减1，如果信号量sem的值非负，就直接返回，否则调用者将被挂起，直到别的任务释放该信号量才能继续运行。
void down(struct semaphore * sem);

// 该函数功能与down类似，不同之处为，down不会被信号（signal）打断，但down_interruptible能被信号（比如Ctrl+C）打断，因此该函数有返回值来区分是正常返回还是被信号中断，如果返回0，表示获得信号量正常返回，如果被信号打断，返回-EINTR
int down_interruptible(struct semaphore *sem);

// 该函数试着获得信号量sem，如果能够立刻获得，它就获得该信号量并返回0，否则，表示不能获得信号量sem，返回值为非0值。因此，它不会导致调用者睡眠，可以在中断上下文使用。
int down_trylock(struct semaphore * sem);
```

+ 内核释放内核信号量所保护的资源
```cpp
// 该函数释放信号量sem，即把sem的值加1，如果sem的值为非正数，表明有任务等待该信号量，因此唤醒这些等待者。
void up(struct semaphore * sem);
```

+ 在驱动程序中，当多个线程同时访问相同的资源时(驱动中的全局变量时一种典型的共享资源)可能会引发“竞态“，因此我们必须对共享资源进行并发控制。Linux内核中
解决并发控制的最常用方法是自旋锁与信号量（绝大多数时候作为互斥锁使用）。
```cpp
ssize_t globalvar_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
　//获得信号量
　if (down_interruptible(&sem))
　{
　　return - ERESTARTSYS;
　}
　//将用户空间的数据复制到内核空间的global_var
　if (copy_from_user(&global_var, buf, sizeof(int)))
　{
　　up(&sem);
　　return - EFAULT;
　}
　//释放信号量
　up(&sem);
　return sizeof(int);
}
```

+ 读-写信号量
如果一个读写信号量当前没有被写者拥有并且也没有写者等待读者释放信号量，那么任何读者都可以成功获得该读写信号量；
否则，读者必须被挂起直到写者释放该信号量。如果一个读写信号量当前没有被读者或写者拥有并且也没有写者等待该信号量，那么一个写者可以成功获得该读写信号量，否则写者将被挂起，直到没有任何访问者。因此，写者是排他性的，独占性的。
    - 读写信号量有两种实现：
        - 一种是通用的，不依赖于硬件架构，因此，增加新的架构不需要重新实现它，但缺点是性能低，获得和释放读写信号量的开销大
        - 另一种是架构相关的，因此性能高，获取和释放读写信号量的开销小，但增加新的架构需要重新实现。在内核配置时，可以通过选项去控制使用哪一种实现。

+ 读写信号量相关API:
```cpp
// 该宏声明一个读写信号量name并对其进行初始化。
DECLARE_RWSEM(name)

// 该函数对读写信号量sem进行初始化
void init_rwsem(struct rw_semaphore * sem);

// 读者调用该函数来得到读写信号量sem。该函数会导致调用者睡眠，因此只能在进程上下文使用。
void down_read(struct rw_semaphore * sem);

// 该函数类似于down_read，只是它不会导致调用者睡眠。它尽力得到读写信号量sem，如果能够立即得到，它就得到该读写信号量，并且返回1，否则表示不能立刻得到该信号量，返回0。因此，它也可以在中断上下文使用。
int down_read_trylock(struct rw_semaphore * sem);

// 写者使用该函数来得到读写信号量sem，它也会导致调用者睡眠，因此只能在进程上下文使用。
void down_write(struct rw_semaphore * sem);

//
int down_write_trylock(struct rw_semaphore *sem);

// 读者使用该函数释放读写信号量sem。它与down_read或down_read_trylock配对使用。如果down_read_trylock返回0，不需要调用up_read来释放读写信号量，因为根本就没有获得信号量。
void up_read(struct rw_semaphore *sem);

// 写者调用该函数释放信号量sem。它与down_write或down_write_trylock配对使用。如果down_write_trylock返回0，不需要调用up_write，因为返回0表示没有获得该读写信号量。
void up_write(struct rw_semaphore *sem);

// 该函数用于把写者降级为读者，这有时是必要的。因为写者是排他性的，因此在写者保持读写信号量期间，任何读者或写者都将无法访问该读写信号量保护的共享资源，对于那些当前条件下不需要写访问的写者，降级为读者将，使得等待访问的读者能够立刻访问，从而增加了并发性，提高了效率。
void downgrade_write(struct rw_semaphore *sem);
```

## POSIX信号量
### 无名信号量
无名信号量的创建就像声明一般的变量一样简单，例如：sem_t sem_id。然后再初始化该无名信号量，之后就可以放心使用了。
无名信号量常用于多线程间的同步，同时也用于相关进程间的同步。也就是说，无名信号量必须是多个进程（线程）的共享变量，无名信号量要保护的变量也必须是多个进程（线程）的共享变量，这两个条件是缺一不可的。
```cpp
// pshared = 0: 用于同一多线程的同步;
// pshared > 0: 用于多个相关进程间的同步(由fork产生的);
int sem_init(sem_t * sem, int pshared, unsigned int value);

// 取回信号量sem的当前值，把该值保存到sval中
// 若有1个或更多的线程或进程调用sem_wait阻塞在该信号量上，该函数返回两种值:
// 返回0 【linux选取的一种返回策略】
// 返回阻塞在该信号量上的进程或线程数目
int sem_getvalue(sem_t * sem, int * sval);

// 申请资源 sem_wait(or sem_trywait)相当于P操作
// 操作是原子的，若sem>0,那么它减1并立即返回
// 若sem==0, 则睡眠直到sem>0,此时立即减1，然后返回。
int sem_wait(sem_t * sem); // 阻塞函数

// 非阻塞函数,和sem_wait一样，除了sem==0时不睡眠，而是返回一个错误EAGAIN
int sem_trywait(sem_t * sem);

// V操作, 把指定的信号量sem值+1，唤醒正在等待该信号量的任意线程
// 在这些函数中，只有sem_post是信号安全的函数，它是可重入函数
int sem_post(sem_t * sem);

```
+ 无名信号量在线程间的同步:
```cpp
#include <iostream>
#include <thread>
#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;

int number;
sem_t sem_id, sem_id2;


int main() {
    number = 1;
    sem_init(&sem_id, 0, 1); // 空闲
    sem_init(&sem_id2, 0, 0); // 忙

    // sem_id先是空闲，所以t_1先进入，然后在number++之后，释放了sem_id2,使得sem_id2>0
    // 此时线程t_2被唤醒，申请到sem_id2
    thread t_1([]()->void * {
        sem_wait(&sem_id);
        printf("thread_one have the semaphore\n");
        number++;

        printf("thread_one : number = %d\n", number);

        sem_post(&sem_id2);

        return NULL;});

    thread t_2([] () {
        sem_wait(&sem_id2);
        printf("thread_two have the semaphore \n");

        number--;
        printf("thread_two : number = %d\n", number);

        sem_post(&sem_id);

        return NULL;
    });

    t_1.join();
    t_2.join();
    return 0;
}

```

+ 无名信号量进程间同步:
```cpp
#include <iostream>
#include <thread>
#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
using namespace std;

int number;
sem_t sem_id, sem_id2;

void test_process_sem() {
    cout << "start to test process semaphore" << endl;
    int nloop = 10, zero = 0;
    sem_t sem_id;

    // open a file and map it into memory
    int fd = open("./log.txt", O_RDWR | O_CREAT, S_IRWXU);
    write(fd, &zero, sizeof(int));
    int * ptr = (int *)mmap(nullptr, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    // create, initialize semaphore(POSIX interface)
    if (sem_init(&sem_id, 1, 1) < 0) {
        perror("semaphore initialization");
        exit(0);
    }
    auto pid = fork();
    if (pid == 0) {
        // child process
        cout << "enter in child process" << endl;
        for (auto i = 0; i < nloop; ++i) {
            sem_wait(&sem_id);
            cout << "child: " << (*ptr)++ << endl;
            sem_post(&sem_id);
        }
        exit(0);
    } else if (pid == -1) {
        perror("fork error");
        exit(-1);
    } else {
        // parent process
        for (auto i = 0; i < nloop; ++i) {
            sem_wait(&sem_id);
            cout << "parent: " << (*ptr)++ << endl;
            sem_post(&sem_id);
        }
        exit(0);
    }
}

int main() {
    test_process_sem();
}
```
### 有名信号量
+ 有名信号量的特点是把信号量的值保存在文件中。既可以用于线程，也可以用于相关进程间，甚至是不相关进程。
由于有名信号量的值是保存在文件中的，所以对于相关进程来说，子进程是继承了父进程的文件描述符，那么子进程所继承的文件描述符所指向的文件是和父进程一样的，当然文件里面保存的有名信号量值就共享了。
+ 有名信号量在使用的时候，和无名信号量共享sem_wait和sem_post函数。区别就是初始化的时候有名信号量用sem_open代替sem_init,另外在结束时要像关闭文件一样去关闭这个有名信号量。
```cpp
// 打开一个已存在的有名信号量，或创建并初始化一个有名信号量。一个单一的调用就完成了信号量的创建、初始化和权限的设置
// name: 文件路径名， oflag: O_CREAT
// mode: 控制新信号量访问权限， value: 指定信号量初始化值
sem_t *sem_open(const char *name, int oflag, mode_t mode , int value);

// 注意：
// 这里的name不能写成/tmp/aaa.sem这样的格式，因为在linux下，sem都是创建在/dev/shm目录下。你可以将name写成“/mysem”或“mysem”，创建出来的文件都是“/dev/shm/sem.mysem”，千万不要写路径。也千万不要写“/tmp/mysem”之类的。
// 当oflag = O_CREAT时，若name指定的信号量不存在时，则会创建一个，而且后面的mode和value参数必须有效。若name指定的信号量已存在，则直接打开该信号量，同时忽略mode和value参数。
// 当oflag = O_CREAT|O_EXCL时，若name指定的信号量已存在，该函数会直接返回error。
```
+ 销毁信号量很重要，在做这个之前，要确定所有对这个有名信号量的引用都已经通过sem_close（）函数关闭了，然后只需在退出或是退出处理函数中调用sem_unlink()去删除系统中的信号量，注意如果有任何的处理器或是线程引用这个信号量，sem_unlink()函数不会起到任何的作用。必须是最后一个使用该信号量的进程执行sem_unlink才有效，因为每个信号量有一个引用计数器记录当前的打开次数，sem_unlink必须等待这个数为0时才能把name所指的信号灯从文件系统中删除。也就是要等待最后一个sem_close发生。

### 有名信号量在无相关进程间的同步
+ 如上所述有名信号量是位于共享内存区的，**那么它要保护的资源也必须是位于共享内存区，只有这样才能被无相关的进程所共享**。
在下面这个例子中，服务进程和客户进程都使用shmget和shmat来获取得一块共享内存资源。然后利用有名信号量来对这块共享内存资源进行互斥保护。
```cpp
// client
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHMSZ 27

char SEM_NAME[] = "sem_zjx";

int main() {
    key_t key = 1000;
    sem_t * sem_id = sem_open(SEM_NAME, 0, 0644, 0);
    if (sem_id == SEM_FAILED) {
        perror("reader::unable to open sem");
        sem_close(sem_id);
        exit(-1);
    }
    // create the shared memory segment with this key
    int shmid = shmget(key, SHMSZ, 0666);
    if (shmid < 0) {
        perror("reader::failure in shmget");
        exit(-1);
    }

    // attach this segment to virtual memory
    char * shm = (char*)shmat(shmid, nullptr, 0);
    // start reading
    char * s = shm;
    for (s = shm; *s != '\0'; s++) {
        sem_wait(sem_id);
        putchar(*s);
        sem_post(sem_id);
    }
    //
    *shm = '*';
    sem_close(sem_id);
    shmctl(shmid, IPC_RMID, 0);
    return EXIT_SUCCESS;
}
```
服务端：
```cpp
#include <iostream>
#include <thread>
#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/ipc.h>
using namespace std;

#define SHMSZ 27

char SEM_NAME[] = "sem_zjx";

int main() {
    // name the shared memory segment
    key_t key = 1000;
    // create & initialize semaphore
    sem_t * sem_id = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    if (sem_id == SEM_FAILED) {
        perror("unable to create semaphore");
        sem_unlink(SEM_NAME);
        exit(-1);
    }
    // create the shared memory segment
    int shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("failure in shmget");
        exit(-1);
    }

    // attach this segment to virtual memory
    char * shm = (char*)shmat(shmid, nullptr, 0);
    char * s = shm;
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        sem_wait(sem_id);
        *s++ = ch;
        sem_post(sem_id);
    }
    // the below loop could be replaced by binary semaphore
    while (*shm != '*') {
        sleep(1);
    }
    sem_close(sem_id);
    sem_unlink(SEM_NAME);
    shmctl(shmid, IPC_RMID, 0);
    return EXIT_SUCCESS;
}
```

## SystemV信号量
这是信号量值的集合，而不是单个信号量。相关的信号量操作函数由<sys/ipc.h>引用。System V 信号量在内核中维护，其中包括二值信号量 、计数信号量、计数信号量集。
内核为每个信号量集维护一个信号量结构体:
```cpp
struct semid_ds
{
    struct ipc_perm sem_perm; /* 信号量集的操作许可权限 */
    struct sem *sem_base; /* 某个信号量sem结构数组的指针，当前信号量集中的每个信号量对应其中一个数组元素 */
    ushort sem_nsems; /* sem_base 数组的个数 */
    time_t sem_otime; /* 最后一次成功修改信号量数组的时间 */
    time_t sem_ctime; /* 成功创建时间 */
};

struct sem {
ushort semval; /* 信号量的当前值 */
short sempid; /* 最后一次返回该信号量的进程ID 号 */
ushort semncnt; /* 等待semval大于当前值的进程个数 */
ushort semzcnt; /* 等待semval变成0的进程个数 */
};
```
