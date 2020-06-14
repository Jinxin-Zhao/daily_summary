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