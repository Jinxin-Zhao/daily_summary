### How to update gcc:
+ packages requested:
    - gcc-6.1.0.tar.bz2
    - gmp-4.3.2.tar.bz2
    - mpfr-2.4.2.tar.bz2
    - mpc-0.8.1.tar.gz
    - isl-0.16.tar.bz2(version must >= 0.15)  [http://isl.gforge.inria.fr]

+ compile these pacakges:[https://www.cnblogs.com/Hxinguan/p/5016305.html]
    - 安装gmp-4.3.2
        $ tar -jxvf gmp-5.0.1.tar.bz2
        $ cd gmp-5.0.1
        $ mkdir temp
        $ cd temp

        开始配置安装目录，进入temp目录后，输入以下命令：
        $ ../configure --prefix=/usr/local/gmp-4.3.2
        $ make
        $ make install

        这样gmp就安装完成了。安装mpfr和mpc过程也差不多，只是安装的时候得加入依赖项

    - 安装mpfr
　　    我这里选择的是3.1.2版本的mpfr。解压，建立临时编译目录temp,进入temp目录

        $ tar -zxvf mpfr-2.4.2.tar.gz 
        $ cd mpfr-2.4.2
        $ mkdir temp
        $ cd temp
        $ ../configure --prefix=/usr/local/mpfr-2.4.2 --with-gmp=/usr/local/gmp-4.3.2
        $ make
        $ make install

        其中--with=/usr/local/gmp-5.0.1就是依赖项， /usr/local/gmp-4.3.2是gmp的安装目录

    - 安装mpc
　　    我选择的是1.0.2版本mpc.解压，建立临时编译目录temp,进入temp目录.

        $ tar -zxvf mpc-0.8.1.tar.gz
        $ cd mpc-0.8.1
        $ mkdir temp
        $ cd temp
        $ ../configure --prefix=/usr/local/mpc-1.0.2 --with-gmp=/usr/local/gmp-4.3.2 --with-mpfr=/usr/local/mpfr-2.4.2
        $ make
        $ make install

        记得后面两项的依赖项，也就是你的gmp和mpfr的安装目录
    - 安装isl
        $ tar -zxvf isl-0.16.tar.gz
        $ cd isl-0.16
        $ mkdir temp
        $ cd temp
        $ ../configure --prefix=/usr/local/isl-0.16 --with-gmp=/usr/local/gmp-4.3.2 

+ compile gcc [http://www.netgull.com/gcc/releases ]
    - sudo ./configure --prefix=/usr/local/gcc-6.1.0 --enable-threads=posix --disable-checking --enable--long-long --enable-languages=c,c++,java --disable-multilib --with-gmp=/usr/local/gmp-4.3.2/ --with-gmp-include=/usr/local/gmp-4.3.2/include  --with-mpfr=/usr/local/mpfr-2.4.2/ --with-mpc=/usr/local/mpc-0.8.1/ --with-isl=/usr/local/isl-0.16

    - sudo make

    [error 1 you may encount]: error situation: configure: error: cannot compute suffix of object files: cannot compile
    [solution]: $ exportLD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/mpc-0.9/lib:/usr/local/gmp-5.0.1/lib:/usr/local/mpfr-3.1.0/lib

    [error 2]: /bin/ld: cannot find -lstdc++
                collect2: error: ld returned 1 exit status
    [solution]: sudo yum install glibc-static libstdc++-static


    $ source /etc/profile
    
    - sudo make install

+ compile c++ source code:
    - [error]: /usr/lib/libstdc++.so.6: version `CXXABI_1.3.9' not found
        [reason]: 问题是由于升级了gcc，却没有将升级后的gcc的动态库去替换老版本的gcc动态库所致。
        [solution]: strings /usr/lib64/libstdc++.so.6 | grep CXXABI
            结果如下：
                CXXABI_1.3
                CXXABI_1.3.1
                CXXABI_1.3.2
                CXXABI_1.3.3
                发现最高版本只有1.3.3，没有1.3.9的。说明出现这些问题，是因为升级gcc时，生成的动态库没有替换老版本gcc的动态库。
            - $ sudo cp /usr/local/gcc-6.1.0/lib64/libstdc++.so.6.0.22 /usr/lib64
            - $ sudo ln -s libstdc++.so.6.0.22 libstdc++.so
            - $ sudo ln -s libstdc++.so.6.0.22 libstdc++.so.6

+ SRAM related:
    - 现在我们知道了动态与静态函数库，也知道了当前的Linux大多是将函数库做成动态函数库，下面来讨论增加函数库读取性能的方法。我们知道，内存的访问速度是硬盘的好几倍，所以，如果将常用的动态函数库加载到内存中（高速缓存，cache），当软件套件要采用动态函数库时，就不需要重新从硬盘里读出，这样就可以提高动态函数库的读取速度。这个时候需要ldconfig与 /etc/ld.so.conf的帮助。

    将动态函数库加载到高速缓存（cache）中的过程如下：

    1. 首先，要在 /etc/ld.so.conf中写下“想要读入高速缓存中的动态函数库所在的目录”，注意，是目录而不是文件。

    2. 利用ldconfig执行文件将 /etc/ld.so.conf的数据读入高速缓存中。

    3. 同时在 /etc/ld.so.cache文件中记录数据。

    CPU的运算速度比主内存的读写速度要快得多，这就使得CPU在访问内存时要花很长时间来等待内存的操作，这种空等造成了系统整体性能的下降。为了解决这种速度上的不匹配问题，我们在CPU与主内存之间加入了比主内存要快的SRAM（Static Ram，静态存储器）。SRAM储存了主内存的映象，使CPU可以直接通过访问SRAM来完成数据的读写。由于SRAM的速度与CPU的速度相当，从而大大缩短了数据读写的等待时间，系统的整体速度也自然得到提高。 高速缓存即 Cache，就是指介于CPU与主内存之间的高速存储器（通常由静态存储器SRAM构成）。
    Cache的工作原理是基于程序访问的局部性。依据局部性原理，可以在主存和CPU通用寄存器之间设置一个高速的容量相对较小的存储器，把正在执行的指令地址附近的一部分指令或数据从主存调入这个存储器，供CPU在一段时间内使用。这对提高程序的运行速度有很大的作用。这个介于主存和CPU之间的高速小容量存储器称作高速缓冲存储器(Cache)。
　　 CPU对存储器进行数据请求时，通常先访问Cache。由于局部性原理不能保证所请求的数据百分之百地在Cache中，这里便存在一    个命中率。即CPU在任一时刻从Cache中可靠获取数据的几率。命中率越高，正确获取数据的可靠性就越大。