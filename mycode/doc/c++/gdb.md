# How to use gdb

### Basics
+ First, you should add '-g' option to the compiler(without -O2 will be better)
  ```cpp
   > gdb ./exe_name

   or 
   > gdb
   > file ./exe_name


    GNU gdb (Ubuntu 8.1-0ubuntu3.2) 8.1.0.20180409-git
    Copyright (C) 2018 Free Software Foundation, Inc.
    License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
    This is free software: you are free to change and redistribute it.
    There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
    and "show warranty" for details.
    This GDB was configured as "x86_64-linux-gnu".
    Type "show configuration" for configuration details.
    For bug reporting instructions, please see:
    <http://www.gnu.org/software/gdb/bugs/>.
    Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.
    For help, type "help".
    Type "apropos word" to search for commands related to "word"...
    Reading symbols from ./example...done.
    (gdb) 
   ```

  ```cpp
    #include <iostream>
    #include <cstdio>
    using namespace std;
    int f(int x){
        int ans=1;
        for(int i=1;i<=x;i++) ans*=i;
        return ans;
    }

    int main(){
        int a;
        scanf("%d",&a);
        printf("%d\n",f(a));
        return 0;
    }
  ```


#### list(abbr 'l')
+ list  [row number] 
  - print codes of neighbour 10 row numbers at [row number]
+ list
  - print the next codes

    ```cpp
    (gdb) l 7
    2	#include <cstdio>
    3	using namespace std;
    4	int f(int x){
    5	    int ans=1;
    6	    for(int i=1;i<=x;i++) ans*=i;
    7	    return ans;
    8	}
    9	
    10	int main(){
    11	    int a;
    (gdb) l
    12	    scanf("%d",&a);
    13	    printf("%d\n",f(a));
    14	    return 0;
    15	}
    (gdb) 
    ```


#### break(abbr 'b')
+ break [function name]
+ break [row number]

    ```cpp
    (gdb) break main //main函数处设置断点
    Breakpoint 1 at 0x874: file example.cpp, line 10.
    (gdb) break 11 //在第11行处设置断点
    Breakpoint 2 at 0x883: file example.cpp, line 11.


    (gdb) b main
    Breakpoint 1 at 0x874: file example.cpp, line 10.
    (gdb) b 11
    Breakpoint 2 at 0x883: file example.cpp, line 11.


    (gdb) r
    Starting program: /home/acceptedzhs/example 

    Breakpoint 1, main () at example.cpp:10
    10	int main(){
    (gdb) c
    Continuing.

    Breakpoint 2, main () at example.cpp:12
    12	    scanf("%d",&a); 
    (gdb) 

    ```

#### next(abbr 'n') & step(abbr 's')
+ next: step to next row
+ step: step into the function

    ```cpp
    (gdb) b 13
    Breakpoint 1 at 0x89b: file example.cpp, line 13.
    (gdb) r
    Starting program: /home/acceptedzhs/example 
    10

    Breakpoint 1, main () at example.cpp:13
    13	    printf("%d\n",f(a));
    (gdb) n
    3628800
    14	    return 0;
    (gdb) [回车] //看到没，执行了上次的命令，即next
    15	}
    (gdb) 
    ```

#### print(abbv 'p') & display(abbv 'disp')
+ print: print the value of the variable
+ print the variable value on encountering the breakpoint or next cmd;

    ```cpp
    (gdb) b 13
    Breakpoint 1 at 0x89b: file example.cpp, line 13.
    (gdb) r
    Starting program: /home/acceptedzhs/example 
    9

    Breakpoint 1, main () at example.cpp:13
    13	    printf("%d\n",f(a));
    (gdb) p a
    $1 = 9
    (gdb) n
    362880 //输出9!
    14	    return 0; //这只会显示一次，下一步就不会再打印该变量值了
    (gdb) p f(2) //当然，调用函数也可以
    $2 = 2
    (gdb) 
    Copy
    (gdb) b f
    Breakpoint 1 at 0x841: file example.cpp, line 5.
    (gdb) r
    Starting program: /home/acceptedzhs/example 
    9

    Breakpoint 1, f (x=9) at example.cpp:5
    5	    int ans=1;
    (gdb) disp ans
    1: ans = 0
    (gdb) n
    6	    for(int i=1;i<=x;i++) ans*=i;
    1: ans = 1
    (gdb) 
    7	    return ans;
    1: ans = 362880 //每次停下来时，该变量都会显示
    (gdb) 
    8	}
    1: ans = 362880
    (gdb)
    有时，你可能会见到 <optimized out> 的提示。此时，请检查编译时是否开了优化（如 -O2 ）。

    ```

#### info(abbr 'i')
+ the types can be breakpoints,locals,display etc

    ```cpp
    (gdb) b 13
    Breakpoint 1 at 0x89b: file example.cpp, line 13.
    (gdb) r
    Starting program: /home/acceptedzhs/example 
    10 //程序的标准输入

    Breakpoint 1, main () at example.cpp:13
    13	    printf("%d\n",f(a));
    (gdb) i lo
    a = 10
    (gdb) i b
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep y   0x000055555555489b in main() at example.cpp:13
        breakpoint already hit 1 time
    (gdb) disp a
    1: a = 10
    (gdb) i display 
    Auto-display expressions now in effect:
    Num Enb Expression
    1:   y  a
    (gdb) 
    ```

#### disable(abbr 'dis') & delete(abbr 'd') & enable(abbr 'en')
+ example:

    ```cpp
    (gdb) b 12
    Breakpoint 1 at 0x883: file example.cpp, line 12.
    (gdb) info b
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep n   0x0000555555554883 in main() at example.cpp:12
    (gdb) dis b 1 //禁用1号断点
    (gdb) r
    Starting program: /home/acceptedzhs/example 
    10
    3628800 //不经过该断点了
    [Inferior 1 (process 2695) exited normally]
    (gdb) en b 1 //启用该断点
    (gdb) r
    Starting program: /home/acceptedzhs/example 

    Breakpoint 1, main () at example.cpp:12
    12	    scanf("%d",&a); //又经过该断点了
    (gdb) d breakpoints 1 //删除
    (gdb) r
    The program being debugged has been started already.
    Start it from the beginning? (y or n) y
    Starting program: /home/acceptedzhs/example 
    10
    3628800 //又不经过断点了
    [Inferior 1 (process 3516) exited normally] 
    (gdb) 
    ```


    | 命令	| 简写  | 作用 |
    | ---- | ---- | ---- |
    | file	   | fil	    |   载入可执行文件 
    | list	   |  l	    |   打印源代码
    | quit	   |  q	    |   退出gdb
    | break	   |  b	    |   设置断点
    | run	   |  r	    |   从头运行程序
    | continue |  c	    |   从当前位置继续运行程序
    | until	   |  u	    |   从当前位置继续运行，直到指定行号
    | next	   |  n	    |   单步执行
    | step	   |  s	    |   单步执行
    | print	   |  p	    |   打印一次值
    | display  | disp	|   设置某个变量/函数总是显示
    | info	   |  i	    |  打印相关类型的信息
    | disable  | dis	| 临时禁用某些东西
    | delete   | d	    | 删除某些东西
    | enable   | en	    | 启用某些东西
    | help	   |  h	    | 获取帮助


### Advanced

 ```cpp
 #include <iostream>
 #include <cstdio>
 using namespace std;
 int f(int n){
     if(n==0) return 1;
   return f(n-1)*n;
 }

 int main(){
    int n;
    scanf("%d",&n);
    printf("%d\n",f(n));
    return 0;
 }
 ```

 #### backtrace
 + look over the frame infomation
 + up/down [num]:  move frame to up/down, default value of num is 1
 + frame [num]: switch to the numth frame ( abbr 'f')

    ```cpp
    (gdb) bt
    #0  f (n=3) at example.cpp:6
    #1  0x000055555555519e in f (n=4) at example.cpp:6
    #2  0x000055555555519e in f (n=5) at example.cpp:6
    #3  0x000055555555519e in f (n=6) at example.cpp:6
    #4  0x000055555555519e in f (n=7) at example.cpp:6
    #5  0x000055555555519e in f (n=8) at example.cpp:6
    #6  0x000055555555519e in f (n=9) at example.cpp:6
    #7  0x000055555555519e in f (n=10) at example.cpp:6
    #8  0x00005555555551dd in main () at example.cpp:12
    (gdb)


    (gdb) up 1
    #1  0x000055555555519e in f (n=4) at example.cpp:6
    6           return f(n-1)*n;
    (gdb) down 1
    #0  f (n=3) at example.cpp:6
    6           return f(n-1)*n;
    (gdb) frame 5 //前面bt输出的结果中，第一项是序号，frame即切换到对应序号的帧
    #5  0x000055555555519e in f (n=8) at example.cpp:6
    6           return f(n-1)*n;
    (gdb) 
    ```

#### commands(abbr. 'comm')
+ run the appointed gdb command on triggering some(or multiple) breakpoint.
+ usage:
  - command [breakpoint 1] [breakpoint 2] ...
  
    ```cpp
    (gdb) b 6
    Breakpoint 1 at 0x1191: file example.cpp, line 6.
    (gdb) comm 1 
    Type commands for breakpoint(s) 1, one per line.
    End with a line saying just "end".
    >p "Command test" //指定到达1号断点时打印这段字符串
    >end //以end结束
    (gdb) r
    Starting program: /run/media/acceptedzhs/SimpleDisk/编程/洛谷/example 
    10

    Breakpoint 1, f (n=10) at example.cpp:6
    6           return f(n-1)*n;
    $1 = "Command test"
    (gdb) 
    ```

#### ignore(abbr. 'ig')
+ stop until the [num+1]th breakpoint
+ usage:
  - ignore [breakpoint][num]

    ```cpp
    (gdb) b 6
    Breakpoint 1 at 0x1191: file example.cpp, line 6.
    (gdb) ig 1 4
    Will ignore next 4 crossings of breakpoint 1.
    (gdb) r
    Starting program: /run/media/acceptedzhs/SimpleDisk/编程/洛谷/example 
    10

    Breakpoint 1, f (n=6) at example.cpp:6
    6           return f(n-1)*n;
    (gdb) //前面4次经过断点，分别为f(10)、f(9)、f(8)、f(7)，都跳过了
    //因此f(6)才触发
    ```
#### condition(abbr. 'cond')
+ stop until the condition is true on triggering the breakpoint
+ usage: 
  - condition [breakpoint][condition]
  
    ```cpp
    (gdb) b 6
    Breakpoint 1 at 0x1191: file example.cpp, line 6.
    (gdb) cond 1 n==5 //只有n等于5时才触发断点
    (gdb) r
    Starting program: /run/media/acceptedzhs/SimpleDisk/编程/洛谷/example 
    10

    Breakpoint 1, f (n=5) at example.cpp:6
    6           return f(n-1)*n;
    (gdb) 
    ```

#### various types of breakpoint
+ break(abbr 'b')
+ tbreak(abbr 'tb'): temporary breakpoint(once and for all)
+ hbreak(abbr 'hb'): hardware breakpoint
+ rbreak(abbr 'rb'): set breakpoint according to [regular expression]; usage: rbreak [regular expression](说明一下rbreak。举个例子，我程序里有两个函数，dfs1与dfs2。如果我运行 rbreak dfs* ，由于dfs1与dfs2均匹配，所以这两个函数均会被加上断点)

    ```cpp
    (gdb) tb f
    Temporary breakpoint 1 at 0x1184: file example.cpp, line 5.
    (gdb) r
    Starting program: /run/media/acceptedzhs/SimpleDisk/编程/洛谷/example 
    10 //这里是输入，10!=3628800

    Temporary breakpoint 1, f (n=10) at example.cpp:5
    5           if(n==0) return 1; //临时断点，只停了一次
    (gdb) c
    Continuing. //继续，就不会再停了
    3628800
    [Inferior 1 (process 31859) exited normally]
    (gdb) 
    ```

#### print/display output format
+ usage: print/[format][var1][var2]


    | fomat	alpha | discription  |
    | ---- | ---- |
    | x	   |  按十六进制格式显示变量     |
    | d	   |  按十进制格式显示变量	     |
    | u	   |  按十进制格式显示无符号整型  |
    | o	   |  按八进制格式显示变量	     |
    | t	   |  按二进制格式显示变量	     |
    | a    |  按十六进制格式显示变量      |
    | c	   |  按字符格式显示变量	    |
    | f	   |  按浮点数格式显示变量	    |


#### save
+ save the breakpoint infomation to file so as to reload them at the next time
+ usage: save breakpoints [filename]
         source [filename]

    ```cpp
    (gdb) b 5
    Breakpoint 1 at 0x1184: file example.cpp, line 5.
    (gdb) b 6
    Breakpoint 2 at 0x1191: file example.cpp, line 6.
    (gdb) b 7
    Breakpoint 3 at 0x11a2: file example.cpp, line 7.
    (gdb) save breakpoints 123 //保存断点信息
    Saved to file '123'.
    (gdb) q

    ---中间退出gdb，再重新进---

    (gdb) source 123 //从这个文件中引入断点信息
    Breakpoint 1 at 0x1184: file example.cpp, line 5.
    Breakpoint 2 at 0x1191: file example.cpp, line 6.
    Breakpoint 3 at 0x11a2: file example.cpp, line 7.
    (gdb) 
    ```

#### call
+ usage: call [call clause]
  ```cpp
  (gdb) call f(10)
  $1 = 3628800
  (gdb)
  ```

#### finish(abbr 'fin')
+ keep running till the current function return

    ```cpp
    Breakpoint 1, main() at example.cpp:10
    10    int n;
    (gdb) fin
    Run till exit from #0 main() at example.cpp:9 //main函数执行完毕，返回了0

    [Inferior 1 (process 32243) exited normally]
    (gdb) 
    ```

#### watchpoint(abbr 'wa')
+ the special type of breakpoint to watch the pointed varaiable
+ usage: watch/rwatch/awatch [var]
  - watch (abbr 'wa'): stop when the variable is written
  - rwatch (abbr 'rwa'): stop when the variable is read
  - awatch(abbr 'awa'): stop when the variable is read/written

    ```cpp
    (gdb) wa n
    Hardware watchpoint 2: n
    (gdb) c
    Continuing.
    10

    Hardware watchpoint 2: n

    Old value = 32767 //此处由于scanf读入修改了n的值，因此停下
    New value = 10
    0x00007ffff7ac43a9 in __vfscanf_internal () from /usr/lib/libc.so.6
    (gdb) 
    ```

#### checkpoint(abbr 'ch')
+ create a snapshot
+ we can roll back to the former checkpointb calling:
  - usage: restart [checkpoint no.]

    ```cpp
    Breakpoint 1, f (n=10) at example.cpp:5
    5           if(n==0) return 1;
    (gdb) ch //创建了编号为1的快照
    checkpoint 1: fork returned pid 32213.
    (gdb) c
    Continuing.

    Breakpoint 1, f (n=9) at example.cpp:5
    5           if(n==0) return 1;
    (gdb) restart 1 //恢复到编号为1的快照
    Switching to process 32213
    #0  f (n=10) at example.cpp:5
    5           if(n==0) return 1;
    (gdb) 
    ```

#### jump
+ usage: jump [num]
+ jump to the numth row compulsorily(only within a function)

    ```cpp
    (gdb) b 10
    Breakpoint 1 at 0x11bb: file example.cpp, line 11.
    (gdb) r
    Starting program: /run/media/acceptedzhs/SimpleDisk/编程/洛谷/example 

    Breakpoint 1, main () at example.cpp:11
    11          scanf("%d",&n);
    (gdb) jump 13
    Continuing at 0x5555555551f0.
    [Inferior 1 (process 32243) exited normally] //跳到了第13行，main函数已经结束了，因此直接退出程序
    (gdb) 
    ```

#### return
+ usage: return [argu]
+ make the current function return compulsorily
  
    ```cpp
    Breakpoint 1, f (n=10) at example.cpp:5
    5           if(n==0) return 1;
    (gdb) return 15
    Make f(int) return now? (y or n) y
    #0  0x00005555555551dd in main () at example.cpp:12
    12          printf("%d\n",f(n));
    (gdb) n
    15  //因为前面设定返回15，所以这里输出15
    ```
#### attach到已运行的进程
+ 方法一: gdb -q a pid
+ 方法二: gdb -q a进入后，然后执行attach pid
如果不想继续调试了可以用detach命令。