### apt-get
+ Q1: 使用Ubuntu打开终端时,输入带有sudo apt-get 命令行是回报下面错误:

    ```
    E: 无法获得锁 /var/lib/dpkg/lock - open (11: 资源暂时不可用)
    E: 无法锁定管理目录(/var/lib/dpkg/)，是否有其他进程正占用它？
    ```

    - reason:
    ```
    在ubuntu系统用带有apt-get 命令行的时候，如果在未完成任务的情况下将终端中断，此时 apt-get进程可能没有结束。
    当重新开机再次运行带有apt-get命令行的时候，可能会发生上面的错误.
    ```

    - solution:
    ```
    在终端输入下面命令强制解锁:
    sudo rm /var/cache/apt/archives/lock
    sudo rm /var/lib/dpkg/lock
    ```
+ Q2: gzip: stdout: No space left on device
E: mkinitramfs failure cpio 141 gzip 1
update-initramfs: failed for /boot/initrd.img-4.13.0-37-generic with 1.
dpkg：处理 initramfs-tools (–configure)时出错：
子进程 已安装 post-installation 脚本 返回了错误号 1
在处理时有错误发生：
initramfs-tools
E: Sub-process /usr/bin/dpkg returned an error code (1) 
    - solution:
    1 查看系统内安装的所有内核：
    dpkg --get-selections | grep linux
    2 查看系统当前使用的内核：
    uname -r
    3 删除多余的内核：
    sudo apt-get remove linux-image-<版本号>
    不要随便删除其他的内核，只删除linux-image-<版本号>类型的内核

### openssh in ubuntu
+ install 
    ]# sudo apt-get install openssh-server
+ check if it's been installed
    ]# sudo ps -e | grep ssh
+ start ssh
    ]# sudo service ssh start
+ modify config to avoid logging next time
    ]# sudo vi /etc/ssh/sshd_config
    find line PermitRootLogin prohibit-password,set yes
    ]# sudo service ssh restart
+ use xshell to login 