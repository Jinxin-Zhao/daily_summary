###linux环境防火墙
】# systemctl status firewalld
查看防火墙状态，一般出现active和inactive

】# systemctl is-enabled firewalld
查看是否开机启动防火墙服务

】# systemctl stop firewalld
】# systemctl status firewalld
关闭防火墙

】# systemctl disable firewalld
】# systemctl is-enabled firewalld
禁用防火墙，设置开机不启动


###根据进程名字查找它所占用的端口
]# ps -ef | grep `process_name`
通过上面命令找到进程号pid
]# netstat -nap | grep `pid` 

（note: tcpdump review)