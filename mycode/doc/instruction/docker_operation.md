# docker
+ (https://vuepress.mirror.docker-practice.com/data_management/bind-mounts/#%E6%8C%82%E8%BD%BD%E4%B8%80%E4%B8%AA%E4%B8%BB%E6%9C%BA%E7%9B%AE%E5%BD%95%E4%BD%9C%E4%B8%BA%E6%95%B0%E6%8D%AE%E5%8D%B7)
+ 镜像（Image）和容器（Container）的关系，就像是面向对象程序设计中的 类 和 实例 一样，镜像是静态的定义，容器是镜像运行时的实体。容器可以被创建、启动、停止、删除、暂停等。
+ 按照 Docker 最佳实践的要求，容器不应该向其存储层内写入任何数据，容器存储层要保持无状态化。所有的文件写入操作，都应该使用 数据卷（Volume）、或者 绑定宿主目录，在这些位置的读写会跳过容器存储层，直接对宿主（或网络存储）发生读写，其性能和稳定性更高。
+ 镜像构建完成后，可以很容易的在当前宿主机上运行，但是，如果需要在其它服务器上使用这个镜像，我们就需要一个集中的存储、分发镜像的服务，Docker Registry 就是这样的服务。一个 Docker Registry 中可以包含多个 仓库（Repository）；每个仓库可以包含多个 标签（Tag）；每个标签对应一个镜像。通常，一个仓库会包含同一个软件不同版本的镜像，而标签就常用于对应该软件的各个版本。我们可以通过 <仓库名>:<标签> 的格式来指定具体是这个软件哪个版本的镜像。如果不给出标签，将以 latest 作为默认标签。以 Ubuntu 镜像 (opens new window)为例，ubuntu 是仓库的名字，其内包含有不同的版本标签，如，16.04, 18.04。我们可以通过 ubuntu:16.04，或者 ubuntu:18.04 来具体指定所需哪个版本的镜像。如果忽略了标签，比如 ubuntu，那将视为 ubuntu:latest。

## docker镜像编译
### 删除本地镜像
+ 用完整的镜像ID删除镜像
    ```shell
    # 列出image,这种方式列出的已经是短ID了
    ]$ docker image ls
    # 删除by 镜像ID
    ]$ docker image rm 501
    # 也可以用镜像名，即<仓库名>:<标签>
    ]$ docker image rm centos
    # Untagged: centos:latest
    #Untagged:centos@sha256:b2f9d1c0ff5f87a4743104d099a3d561002ac500db1b9bfa02a783a46e0d366c
    #Deleted:sha256:0584b3d2cf6d235ee310cf14b54667d889887b838d3f3d3033acd70fc3c48b8a
    #Deleted:sha256:97ca462ad9eeae25941546209454496e1d66749d53dfa2ee32bf1faabd239d38
    
    # 使用指令配合删除
    ]$ docker image rm $(docker image ls -q centos)
    # 删除所有在centos:7之前的镜像
    ]$ docker image rm $(docker image ls -q -f before=centos:7)
    ```
+ Untagged和Deleted，镜像的唯一标识是其ID和摘要，而一个镜像可以有多个标签，因此当我们使用上面命令删除镜像的时候实际上是在要求删除某个标签的镜像，首先需要做的就是将满足我们要求的所有镜像标签取消，所以看到Untagged信息；因此当我们删除了所指定的标签后，可能还有别的标签指向了这个镜像，这种情况Delete行为就不会发生。
+ 除了镜像依赖以外，还需要注意的是容器对镜像的依赖。如果有用这个镜像启动的容器存在（即使容器没有运行），那么同样不可以删除这个镜像。之前讲过，容器是以镜像为基础，再加一层容器存储层，组成这样的多层存储结构去运行的。因此该镜像如果被这个容器所依赖的，那么删除必然会导致故障。如果这些容器是不需要的，应该先将它们删除，然后再来删除镜像。


### 从Dockerfile构建镜像
+ FROM和RUN，FROM指定基础镜像；
+ Dockfile中每一个指令都会建立一层，RUN也不例外。每一个RUN行为都会新建立一层，在其上执行这些命令，执行结束后，commit这一层的修改构成新的镜像。
    ```shell
    FROM debian:stretch

    RUN apt-get update
    RUN apt-get install -y gcc libc6-dev make wget
    RUN wget -O redis.tar.gz "http://download.redis.io/releases/redis-5.0.3.tar.gz"
    RUN mkdir -p /usr/src/redis
    RUN tar -xzf redis.tar.gz -C /usr/src/redis --strip-components=1
    RUN make -C /usr/src/redis
    RUN make -C /usr/src/redis install
    ```
    而上面的这种写法，创建了 7 层镜像。这是完全没有意义的，而且很多运行时不需要的东西，都被装进了镜像里，比如编译环境、更新的软件包等等。结果就是产生非常臃肿、非常多层的镜像，不仅仅增加了构建部署的时间，也很容易出错。 这是很多初学 Docker 的人常犯的一个错误。
    Union FS 是有最大层数限制的，比如 AUFS，曾经是最大不得超过 42 层，现在是不得超过 127 层。
    应该这样写：
    ```shell
    FROM debian:stretch

    RUN set -x; buildDeps='gcc libc6-dev make wget' \
    && apt-get update \
    && apt-get install -y $buildDeps \
    && wget -O redis.tar.gz "http://download.redis.io/releases/redis-5.0.3.tar.gz" \
    && mkdir -p /usr/src/redis \
    && tar -xzf redis.tar.gz -C /usr/src/redis --strip-components=1 \
    && make -C /usr/src/redis \
    && make -C /usr/src/redis install \
    && rm -rf /var/lib/apt/lists/* \
    && rm redis.tar.gz \
    && rm -r /usr/src/redis \
    && apt-get purge -y --auto-remove $buildDeps
    ```
+ 在Dockerfile文件所在目录执行:
    ```shell
    ]$ docker build -t centos:8 .
    ```
    镜像构建上下文(Context),如果注意，会看到 docker build 命令最后有一个 .。. 表示当前目录，而 Dockerfile 就在当前目录，因此不少初学者以为这个路径是在指定 Dockerfile 所在路径，这么理解其实是不准确的。首先我们要理解 docker build 的工作原理。Docker 在运行时分为 Docker 引擎（也就是服务端守护进程）和客户端工具。Docker 的引擎提供了一组 REST API，被称为 Docker Remote API (opens new window)，而如 docker 命令这样的客户端工具，则是通过这组 API 与 Docker 引擎交互，从而完成各种功能。因此，虽然表面上我们好像是在本机执行各种 docker 功能，但实际上，一切都是使用的远程调用形式在服务端（Docker 引擎）完成。也因为这种 C/S 设计，让我们操作远程服务器的 Docker 引擎变得轻而易举。
    当我们进行镜像构建的时候，并非所有定制都会通过 RUN 指令完成，经常会需要将一些本地文件复制进镜像，比如通过 COPY 指令、ADD 指令等。而 docker build 命令构建镜像，其实并非在本地构建，而是在服务端，也就是 Docker 引擎中构建的。那么在这种客户端/服务端的架构中，如何才能让服务端获得本地文件呢？
    这就引入了上下文的概念。当构建的时候，用户会指定构建镜像上下文的路径，docker build 命令得知这个路径后，会将路径下的所有内容打包，然后上传给 Docker 引擎。这样 Docker 引擎收到这个上下文包后，展开就会获得构建镜像所需的一切文件。
    一般来说，应该会将 Dockerfile 置于一个空目录下，或者项目根目录下。如果该目录下没有所需文件，那么应该把所需文件复制一份过来。如果目录下有些东西确实不希望构建时传给 Docker 引擎，那么可以用 .gitignore 一样的语法写一个 .dockerignore，该文件是用于剔除不需要作为上下文传递给 Docker 引擎的。
    那么为什么会有人误以为 . 是指定 Dockerfile 所在目录呢？这是因为在默认情况下，如果不额外指定 Dockerfile 的话，会将上下文目录下的名为 Dockerfile 的文件作为 Dockerfile。
    这只是默认行为，实际上 Dockerfile 的文件名并不要求必须为 Dockerfile，而且并不要求必须位于上下文目录中，比如可以用 -f ../Dockerfile.php 参数指定某个文件作为 Dockerfile。
    当然，一般大家习惯性的会使用默认的文件名 Dockerfile，以及会将其置于镜像构建上下文目录中。
## docker容器
+ 查看、启动、删除容器
    ```shell
    # 列出运行状态的容器
    ]$ docker container ls
    # or
    ]$ docker ps
    ```
## docker数据管理
### 在容器中管理数据主要有两种方式:
    - 数据卷(Volumes)
    - 挂载主机目录(Bind mounts)
#### 数据卷
+ 数据卷可以提供给一个或多个容器使用，绕过UFS；可以在容器间共享和重用；对数据卷的修改会立马生效；对数据卷的更新不会影响镜像；数据卷默认会一直存在，即使容器被删除；
+ 注意：数据卷 的使用，类似于 Linux 下对目录或文件进行 mount，镜像中的被指定为挂载点的目录中的文件会复制到数据卷中（仅数据卷为空时会复制）。
    ```shell
    # 创建一个名为my_vol的卷
    ]$ docker volume create my_vol
    # 查看所有的数据卷
    ]$ docker volume ls
    # DRIVER     VOLUME NAME
    # local       my_vol

    # 查看指定数据卷的信息
    ]$ docker volume inspect my_vol

    # 删除数据卷,Docker 不会在容器被删除后自动删除 数据卷，并且也不存在垃圾回收这样的机制来处理没有任何容器引用的 数据卷。如果需要在删除容器的同时移除数据卷。可以在删除容器的时候使用 docker rm -v 这个命令。
    ]$ docker volume rm my_vol

    # 无主的数据卷可能会占据很多空间，要清理使用以下命令
    ]$ docker volume prune
    ```

#### 挂载主机目录、文件
+ 使用--mount标记可以指定挂载一个本地主机的目录到容器:
    ```shell
    # Docker 挂载主机目录的默认权限是 读写，用户也可以通过增加 readonly 指定为 只读

    ]$ docker run -it -v /source/path:/target/path centos:8 /bin/bash

    # example 
    ]$ docker run -it -v /Users/xina/workspace/docker_volume:/root/workspace centos:8 /bin/bash
    #]$ docker run -d -P --name new_container --mount type=bind,source=/tmp, target=/usr/tmp

    #在主机里使用以下命令查看容器信息
    ]$ docker inspect new_container
    ```
+ 使用--mount标记挂载文件
    ```shell
    # 这样可以记录在容器输入过的命令
    ]$ docker run -rm -it --mount type=bind,source=$HOME/.bash_history,target=/root/.bash_history ubuntu:18.04
    ```

## 容器端口映射
```shell 
    ]$ docker run -itd --name=centos-craft -p 127.0.0.1:80:80 centos:8
    ]$ docker exec -it centos-craft /bin/bash


    ]$  docker run -d -p 5022:22 --name centos_c --privileged=true -v /Users/xina/workspace/docker_volume:/root/workspace centos:8 /usr/sbin/init
    ]$ docker exec -it centos_c /bin/bash
```

###  配置ssh
+ （https://segmentfault.com/a/1190000020671797）
### 配置CLion远程
+ (https://zhuanlan.zhihu.com/p/429270402)
### how to use docker to install linux system in macos
+ make sure docker has been successfully installed in advance;
+ command needing to input:
    - docker search centos;
    - docker pull centos ( pull the latest version of centos by default);
    - docker images(list the iso files installed);
    - docker run -it -d --privileged --name centos_xin -p 8022:22 centos /usr/sbin/init (don't use port: 8888)
    (-name: define name of your container; -p: define port mappings: the former is virtual machine port,the latter is container port; -it: interaction model)
+ since the default linux system(centos etc...) just intalled is the simplest version by default; you need to intall the software you need by yourself; 
 such as:
    ```cpp
    sudo yum install gcc;
    sudo yum install gcc-c++;
    sudo yum install ncurses(if you type 'clear',warning: "bash     command not found");
    yum install openssh-server;
    systemctl restart/start/stop  sshd
    netstat -antp | grep sshd
    yum install initscripts -y(if service cmd not found)
    yum install net-tools(if netstat cmd not found)
    ```
+ then you need to save the operations you typed to virtual system in the container:
    - open a new tab terminal and execute the command below:
        - ]# docker ps;
        ouput like this:
        ```cpp
        CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS                  NAMES
        1a0f0c50f255        470671670cac        "/bin/bash"         11 hours ago        Up 11 hours         0.0.0.0:8088->80/tcp   xina_centos

        ```
        - ]# docker commit 1a0f0c50f255 470671670cac ( docker commit [CONTAINER ID] [IMAGE])
        - turn to the former tab(which runs the linux system) and type the command:
        ]# exit
+ by the way you can inspect the particular infomation by typing:
    - ]# docker inspect [id] ( docker inspect '1a')
    - ]# docker inspect -f '{{.Id}}' centos_xin //query full name of container
+ how to transfer files between docker container and host:
  - docker cp [host_dir_path] [fullname_of_container]:[container_dir_path]    //host --> container
  - docker cp [fullname_of_container]:[container_dir_path] [host_dir_path]     //container --> host
 
  - example: container --> host
    docker cp cc80438298a32bc5918a2aec891a653ec2f84597f364eb30f2ae69f7cec4b2ac:/opt/share/cmake-build-debug /Users/xina/workspace/remote_workspace/d_craft/

+ how to ssh the docker container:
    - ]# ssh root@0.0.0.0 -p 8022
  
+ problems you may encounter:
    我现在想要使用SSH连接到容器’ssh root @< IP address>‘但该命令给出以下错误：’操作超时’.
    进一步调查显示我无法ping< IP地址> – &GT; ‘请求icmp_seq 0的超时’
    solutions:
    您描述的场景是在“普通”Docker上使用的方法.
    由于Mac上的Docker是专门为Mac创建的,因此它已经过调整以使其更易于使用.因此,在Mac上无法以这种方式使用容器的IP地址.
    文档Getting Started with Docker for Mac指出：
    ```cpp
    Previous beta releases used docker as the hostname to build the URL.
    From this release forward, ports are exposed on the private IP
    addresses of the VM and forwarded to localhost with no other host name
    set. See also, Release Notes for Beta 9.
    ```
    因此,SSH到容器的正确方法是使用端口映射到SSH端口在Docker for Mac上将其旋转(22).例如
    docker run -d -p 2022:22 <Image Name>
    并且使用此命令启动SSH连接(N.B.它在指定的端口上使用’localhost’,而不必确定和使用容器的IP地址)：
    ssh -p 2022 root@localhost
注：不可能简单地将端口22映射到自身,即’-p 22:22′,因为这会导致以下错误(至少对我来说是这样的！)：
    ```cpp
    docker: Error response from daemon: driver failed programming external
    connectivity on endpoint pensive_wilson
    (2e832b82fc67d3e48864975c6eb02f6c099e34eee64b29634cfde286c41e00a7):
    Error starting userland proxy: Failed to bind: EADDRINUSE.
    ```


### in virtual machine, if there's no ip address:
    [root@localhost ~]# cd /etc/sysconfig/network-scripts/
    [root@localhost network-script]# vi ifcfg-ens33
    set ONBOOT=yes
    [root@localhost network-script]# systmctl restart NetWorkManager 
### in virtual machine, how to set ip fixed-address:
    + look for the MAC address: settings --> network adaptor --> advanced options
    + modify dhcpd.conf:
        on my system,this file is located in '/Library/Preferences/VMware Fusion/vmnet8',so edit the file(use sudo):
        ]# sudo vim /Library/Preferences/VMware Fusion/vmnet8/dhcpd.conf

        Now, after where it says "End of "DO NOT NODIFY SECTION"",enter the following lines:
        ```cpp
        host CentOS7x64 {
	        hardware ethernet 00:0C:29:CF:9E:C0;
            fixed-address 172.16.223.80
        }
        //[Crutial]: My VM's name is actually "CentOS 7 x64",so in the dhcpd.conf file you must refer to it with no spaces in the name "CentOS7x64";
        //[Crutial]: you must allocate an IP address that is outside the range defined in :

        subnet 172.16.223.0 netmask 255.255.255.0 {
	        range 172.16.223.128 172.16.223.254;
	        option broadcast-address 172.16.223.255;
	        option domain-name-servers 172.16.223.2;
	        option domain-name localdomain;
	        default-lease-time 1800;                # default is 30         minutes
	        max-lease-time 7200;                    # default is 2 hours
	        option netbios-name-servers 172.16.223.2;
	        option routers 172.16.223.2;
        }
        // so I can allocate any address under 172.16.223.128(which means 172.16.233.{1~127})
        ```
    + restart VMware Fusion
    + start the VM