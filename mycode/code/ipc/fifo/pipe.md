+ Pipe:
    - Pipes behave FIFO(First in First out), Pipe behave like a queue data structure. 

    - properties:
        - 1. read & write: We can write 512 bytes(atomic if write<= 512 bytes) at a time but we can read only 1 byte at a time in a pipe.

+ FIFO(named pipe)
    - mkfifo命令创建一个FIFO特殊文件，是一个命名管道（可以用来做进程之间通信的桥梁）
    - 管道也是一种文件，一般是linux中的一个页大小，4k，管道数据一旦被读取就没了。

    - create pipe: ]# mkfifo test.pipe
    - case one :  1 vs 1
        - ]#  ls -l > test.pipe  [terminal 1]
        - ]#  cat < test.pipe   [terminal 2]
        当我们执行 ls -l > test.pipe 的时候，这里就开始阻塞了，只有当我们再第二个窗口执行 cat < test.pipe 的时候，第一个窗口的ls命令才执行完。
    - case two: [single input] vs [multireceiver]
        - ]# echo "zhaojx" > test.pipe [terminal 1]
        - ]# cat < test.pipe [terminal 2]
        - ]# cat < test.pipe [terminal 3]
        执行的结果是窗口2输出了 lzz，第一个窗没有输出，说明接收方无论多个只能获取一份水，只要被一个接收方接受到，另外一个就无法获取了（所以他不是订阅型的），通过持续往管道输入可以看到，接收方应该是基于抢占式的
    - case three: [multi-input] vs [single reveiver]
        -  #窗口1 
        ]#  cat < log.pipe
        - #窗口2 
        ]# python logpipe.py win1 > log.pipe
        - #窗口3 
        ]# python logpipe.py win2 > log.pipe

        - #输出
            win2
            win2
            win2
            win2
            win1
            win1
            其实这种情况，比较好理解，有个下水道大家都往里面倒水，最后汇总到一个口流出来，由于有缓冲区，所以并不会马上读取到结果。+ Pipe:
    - Pipes behave FIFO(First in First out), Pipe behave like a queue data structure. 

    - properties:
        - 1. read & write: We can write 512 bytes(atomic if write<= 512 bytes) at a time but we can read only 1 byte at a time in a pipe.