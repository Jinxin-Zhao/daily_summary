<!-- TOC -->

- [1. awk](#1-awk)
- [2. sed](#2-sed)
- [3. vim editor](#3-vim-editor)
- [4. ps command](#4-ps-command)
- [5. shell grammar](#5-shell-grammar)
- [6. grep & xargs](#6-grep-xargs)

<!-- TOC -->

### 1. awk
+ 
### 2. sed
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
    //delete rows from which contain 'keywords' at  the first time  to the end of the file 

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


### 3. vim editor
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


### 4. ps command
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

### 5. shell grammar
+ shell grammar:
    ```cpp
    # a blankspace between each words and no comma
        a=( "dir_1" "dir_2" "dir_3" )
        i=0
        while [ $i -lt ${#a[@]} ]ps -fu 的每一列属性
        do
            echo ${a[$i]}(#3-vim-editor)
            if [ -d ${a[i(#3-vim-editor)一列属性
                echo exis(#3-vim-editor)
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

### 6. grep & xargs
+ grep
    ```cpp
          -E: 开启扩展（Extend）的正则表达式;
          -i: 忽略大小写(ignore case);
          -v: 反过来（invert），只打印没有匹配的，而匹配的反而不打印;
          -n: 显示行号;
          -w ：被匹配的文本只能是单词，而不能是单词中的某一部分，如文本中有liker，而我搜寻的只是like，就可以使用-w选项来避免匹配liker;
          -c ：显示总共有多少行被匹配到了，而不是显示被匹配到的内容，注意如果同时使用-cv选项是显示有多少行没有被匹配到;
          -o ：只显示被模式匹配到的字符串;
          --color :将匹配到的内容以颜色高亮显示;
          -A  n：显示匹配到的字符串所在的行及其后n行，after;
          -B  n：显示匹配到的字符串所在的行及其前n行，before;
          -C  n：显示匹配到的字符串所在的行及其前后各n行，context

    //grep -v
    ]# cat test.log | grep "login"|grep -v "deviceType"
    //找出test.log中包含login信息的,且没有deviceType这个字段的。
    
    //grep -i
    ]# cat test.log | grep -i "ASf"
    //不区分大小写

    // \|
    ]# grep "abc\|xyz" testfile 
    //表示过滤包含abc或xyz的行

    grep -l pattern files ：只列出匹配的文件名，
    grep -L pattern files ：列出不匹配的文件名，
    grep -w pattern files ：只匹配整个单词，而不是字符串的一部分(如匹配’magic’，而不是’magical’)，
    grep -C number pattern files ：匹配的上下文分别显示[number]行，
    grep pattern1 | pattern2 files ：显示匹配 pattern1 或 pattern2 的行;
    ```
+ xargs
    - xargs与管道有什么不同呢，这是两个很容易混淆的东西，我们来看下面的例子弄清楚为什么需要xargs：

    ```cpp
    ]# echo '--help' | cat 
    输出：
    --help

    echo '--help' | xargs cat 
    //相当于执行 cat --help,xargs 将--help作为cat的命令行参数
    ```
    - xargs的参数选项
    ```cpp
    -d //我们可以使用 -d 命令指定分隔符
    //默认情况下xargs将其标准输入中的内容以空白(包括空格、Tab、回车换行等)分割成多个之后当作命令行参数传递给其后面的命令，并运行之;
    ]# echo '11@22@33' | xargs -d '@' echo  
    //输出11 22 33

    -p //使用该选项之后xargs并不会马上执行其后面的命令，而是输出即将要执行的完整的命令(包括命令以及传递给命令的命令行参数)，询问是否执行，输入 y 才继续执行，否则不执行。
    ]# echo '11@22@33' | xargs -p -d '@'  echo 
    输出:
    echo 11 22 33
    ?...y      ==>这里询问是否执行命令 echo 11 22 33 输入y并回车，则显示执行结果，否则不执行
    11 22 33   ==>执行结果

    -n //该选项表示将xargs生成的命令行参数，每次传递几个参数给其后面的命令执行，例如如果xargs从标准输入中读入内容，然后以分隔符分割之后生成的命令行参数有10个，使用 -n 3 之后表示一次传递给xargs后面的命令是3个参数，因为一共有10个参数，所以要执行4次，才能将参数用完。例如：
    ]# echo '11@22@33@44@55@66@77@88@99@00' | xargs -d '@' -n 3 echo 
    输出结果：
    11 22 33
    44 55 66
    77 88 99
    00
    等价于：
    echo 11 22 33
    echo 44 55 66
    echo 77 88 99
    echo 00
    实际上运行了4次，每次传递3个参数，最后还剩一个，就直接传递一个参数。

    -E //该选项指定一个字符串，当xargs解析出多个命令行参数的时候，如果搜索到-e指定的命令行参数，则只会将-e指定的命令行参数之前的参数(不包括-e指定的这个参数)传递给xargs后面的命令
    ]# echo '11 22 33' | xargs -E '33' echo 
    输出：
    11 22
    [attention]: -E只有在xargs不指定-d的时候有效，如果指定了-d则不起作用，而不管-d指定的是什么字符，空格也不行。

    -0 //表示以 '\0' 为分隔符，一般与find结合使用
    ]# find . -name "*.txt"
    输出：
    ./2.txt
    ./3.txt
    ./1.txt     => 默认情况下find的输出结果是每条记录后面加上换行，也就是每条记录是一个新行

    ]# find . -name "*.txt" -print0
    输出：
    ./2.txt./3.txt./1.txt     => 加上 -print0 参数表示  find输出的每条结果后面加上 '\0' 而不是换行

    ]# find . -name "*.txt" -print0 | xargs -0 echo 
    输出：
    ./2.txt ./3.txt ./1.txt

    find . -name "*.txt" -print0 | xargs -d '\0' echo 
    输出：
    ./2.txt ./3.txt ./1.txt
    ```
