### awk
+ 
### sed
+ delete sepecified rows：
```cpp
//bid class 
Class Field{
//bid1
int bid1;
//bid2;
int bid2;
//bid3;
int bid3;
//bid4
int bid4;
//bid5
int bid5;
};

]# sed -i 1~2d tmp.txt
//from row 1,delete a row every two rows;
//'-i' means it effects in tmp.txt directly
]# cat tmp.txt
Class Field{
int bid1;
int bid2;
int bid3;
int bid4;
int bid5;
};


]# sed Nd filename
//delete Nth row,N=1,2,...
]# sed 4,8d filename
//delete rows form 4th to 8th row
]# sed '$d' filename
//delete last row

]# sed /keywords/d filename
//delete all rows which contain 'keywords'



]# sed '/keywords/,$d' filename
//delete rows from which contain 'keywords' at the first time  to the end of the file 

//tmp.txt
]# cat tmp.txt
co
dd
1
co
dd 
2
co
dd
3
co
dd
4
co
dd
5

]# sed '/dd/,$d' tmp.txt
co  //from second row,all deleted

]# sed '/co/,+1d' tmp.txt
1
2
3
4
5
//delete row contains 'co' and the row behind co

```


###vim editor
+ clear content of file
    - 1. gg  //jump to the top of file
    - 2. dG  //clear all contents of file
    - 3. G$ //jump to the end of file
+ remove string like this:
    - d+w:  (127.9.8.7.000)---->(.9.8.7.000)
    - d+$: remove the content from current location to the end of this row

+ combo keys:
    - "C": remove content after current cursor and enter into 'insert' mode;
    - "b": jump to forehead of current keyword;
    - "A": jump to the end of current row and enter into 'insert' mode;

+ find & substitute
    - :s/vivian/sky/ 替换当前行第一个 vivian 为 sky

    - :s/vivian/sky/g 替换当前行所有 vivian 为 sky

    - :n,$s/vivian/sky/ 替换第 n 行开始到最后一行中每一行的第一个 vivian 为 sky

    - :n,$s/vivian/sky/g 替换第 n 行开始到最后一行中每一行所有 vivian 为 sky

    - n 为数字，若 n 为 .，表示从当前行开始到最后一行:%s/vivian/sky/（等同于 :g/vivian/s//sky/） 替换每一行的第一个 vivian 为 sky

    - :%s/vivian/sky/g（等同于 - - - -:g/vivian/s//sky/g） 替换每一行中所有 vivian 为 sky

    - 可以使用 # 作为分隔符，此时中间出现的 / 不会作为分隔符
       - :s#vivian/#sky/# 替换当前行第一个 vivian/ 为 sky/
+ 删除文本中的^M
    - · 使用命令：cat filename1 | tr -d “^V^M” > newfile；

    - · 使用命令：sed -e “s/^V^M//” filename > outputfilename。需要注意的是在1、2两种方法中，^V和^M指的是Ctrl+V和Ctrl+M。你必须要手工进行输入，而不是粘贴。
]# sed '/keywords/,+2d' filename
//delete the rows which contain keywords and the next 2 rows

]# sed '/^$/d' filename
//delete the blank row


###tar
+ param:[A c d r u x f v]
[note]: f must be the last
    - c: create file
ps -fu 的每一列属性
ps -fu 的每一列属性mation of source ps -fu 的每一列属性
ps -fu 的每一列属性
ps -fu 的每一列属性
ps -fu 的每一列属性les
ps -fu 的每一列属性to tar files
```
    tar -cvf test.tar file1 file2
    tar -tvf test.tar

    tar -rvf test.tar new_file

    ]# tar -xvf test.tar file1 ps 
    ]# tar -cvf - files/ | ssh ps -fu 的每一列属性amazon@172.16.2.211 "tar xv -C Documents/"

    #merge 2 tar files
    ]# tar -Af file1.tar file2.tar
    ]# tar -tvf file1.tar

    #if file in tar files has not changed,nothing will happen
    ]# tar -tvf  test.tar
        file1
        file2

    test.tar --delete file1 file2
    or ]# tar --delete --file test.tar file1 file2

    #exclude specified files from tar files
    ]# tar -cf test.tar * --exclude "*.txt"
    or you can put file1&file2 to a dir list,then:
    ]# tar -cf test.tar * -X list
```

### ps command
+ param:[A a au e N c]
```cpp
[root@localhost test6]# ps -l
F S UID PID PPID C PRI NI ADDR SZ WCHAN TTY TIME CMD
4 S 0 17398 17394 0 75 0 - 16543 wait pts/0 00:00:00 bash
4 R 0 17469 17398 0 77 0 - 15877 - pts/0 00:00:00 ps

各相关信息的意义：
F 代表这个程序的旗标 (flag)， 4 代表使用者为 super user
S 代表这个程序的状态 (STAT)，关于各 STAT 的意义将在内文介绍
UID 程序被该 UID 所拥有
PID 就是这个程序的 ID ！
PPID 则是其上级父程序的ID
C CPU 使用的资源百分比
PRI 这个是 Priority (优先执行序) 的缩写，详细后面介绍
NI 这个是 Nice 值，在下一小节我们会持续介绍
ADDR 这个是 kernel function，指出该程序在内存的那个部分。如果是个 running的程序，一般就是 "-"
SZ 使用掉的内存大小
WCHAN 目前这个程序是否正在运作当中，若为 - 表示正在运作
TTY 登入者的终端机位置
TIME 使用掉的 CPU 时间。
CMD 所下达的指令为何
在预设的情况下， ps 仅会列出与目前所在的 bash shell 有关的 PID 而已，所以， 当我使用 ps -l 的时候，只有三个 PID。

STAT：该程序目前的状态，主要的状态有
R ：该程序目前正在运作，或者是可被运作
S ：该程序目前正在睡眠当中 (可说是 idle 状态)，但可被某些讯号 (signal) 唤醒。
T ：该程序目前正在侦测或者是停止了
Z ：该程序应该已经终止，但是其父程序却无法正常的终止他，造成 zombie (疆尸) 程序的状态
//
D 不可中断 uninterruptible sleep (usually IO) 
R 运行 runnable (on run queue) 
S 中断 sleeping 
T 停止 traced or stopped 
Z 僵死 a defunct (”zombie”) process




3. 输出指定的字段
命令：ps -o pid,ppid,pgrp,session,tpgid,comm

[root@localhost test6]# ps -o pid,ppid,pgrp,session,tpgid,comm
PID PPID PGRP SESS TPGID COMMAND
17398 17394 17398 17398 17478 bash
17478 17398 17478 17398 17478 ps
```

### array && if else && while
```cpp
    #a blankspace between each words and no comma
    a=( "dir_1" "dir_2" "dir_3" )
    i=0
    while [ $i -lt ${#a[@]} ]ps -fu 的每一列属性
    do
        echo ${a[$i]}
        if [ -d ${a[i]} ];theps -fu 的每一列属性
            echo exist
        else
            mkdir ${a[i]}
        fi
        let i++
    done
    shell判断文件夹是否存在
 
#如果文件夹不存在，创建文件夹
if [ ! -d "/myfolder" ]; then
 mkdir /myfolder
fi
 
#shell判断文件,目录是否存在或者具有权限
 
 
folder="/var/www/"
file="/var/www/log"
 
# -x 参数判断 $folder 是否存在并且是否具有可执行权限
if [ ! -x "$folder"]; then
 mkdir "$folder"
fi
 
# -d 参数判断 $folder 是否存在
if [ ! -d "$folder"]; then
 mkdir "$folder"
fi
 
# -f 参数判断 $file 是否存在
if [ ! -f "$file" ]; then
 touch "$file"
fi
 
# -n 判断一个变量是否有值
if [ ! -n "$var" ]; then
 echo "$var is empty"
 exit 0
fi
 
# 判断两个变量是否相等
if [ "$var1" = "$var2" ]; then
 echo '$var1 eq $var2'
else
 echo '$var1 not eq $var2'
fi
    
```