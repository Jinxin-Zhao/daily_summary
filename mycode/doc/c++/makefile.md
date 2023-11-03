# How to write makefile
## target
  ```makefile
  target ...: prerequisites ...
    command
    ...
    ...
  ```
  **target**: 可以是一个object file(.o),也可以是一个执行文件，还可以是一个标签(label)。
  **prerequisites**: 生成该target所依赖的文件和/或target
  **command**: 该target要执行的命令(任意的shell命令)
  $$prerequisites中如果有一个以上的文件比target文件要新的话，command所定义的命令就会被执行$$

+ Example:
  ```makefile
  edit : main.o kbd.o command.o display.o \
        insert.o search.o files.o utils.o
    cc -o edit main.o kbd.o command.o display.o \
        insert.o search.o files.o utils.o

   main.o : main.c defs.h
       cc -c main.c
   kbd.o : kbd.c defs.h command.h
       cc -c kbd.c
   command.o : command.c defs.h command.h
       cc -c command.c
   display.o : display.c defs.h buffer.h
       cc -c display.c
   insert.o : insert.c defs.h buffer.h
       cc -c insert.c
   search.o : search.c defs.h buffer.h
       cc -c search.c
   files.o : files.c defs.h buffer.h command.h
       cc -c files.c
   utils.o : utils.c defs.h
       cc -c utils.c
   clean :
       rm edit main.o kbd.o command.o display.o \
        insert.o search.o files.o utils.o
  ```
  在这个makefile中，target包含: edit和(*.o)
  在定义好依赖关系后，后续的那一行定义了如何生成目标文件的操作系统命令，一定要以一个 Tab 键作为开头。
  如果edit文件不存在，或是edit所依赖的后面的 .o 文件的文件修改时间要比 edit 这个文件新，那么，他就会执行后面所定义的命令来生成 edit 这个文件
  如果 edit 所依赖的 .o 文件也不存在，那么make会在当前文件中找目标为 .o 文件的依赖性，如果找到则再根据那一个规则生成 .o 文件。
+ 为了makefile的易维护，在makefile中我们可以使用变量。makefile的变量也就是一个字符串，理解成C语言中的宏可能会更好.
  ```makefile
  OBJS = main.o kbd.o command.o display.o\
  insert.o search.o files.o utils.o

  edit: $(OBJS)
    cc -o edit $(OBJS)

  ...
  clean:
    rm edit $(OBJS)
  ```
+ make可以自动推导文件以及文件依赖关系后面的命令，于是我们就没必要去在每一个 .o 文件后都写上类似的命令，因为，我们的make会自动识别，并自己推导命令。
  只要make看到一个.o文件，它就会自动把.c文件加在依赖关系中，如果make找到一个 whatever.o ，那么 whatever.c 就会是 whatever.o 的依赖文件。并且 cc -c whatever.c 也会被推导出来。
  ```makefile
    objects = main.o kbd.o command.o display.o \
    insert.o search.o files.o utils.o

    edit : $(objects)
        cc -o edit $(objects)

    main.o : defs.h
    kbd.o : defs.h command.h
    command.o : defs.h command.h
    display.o : defs.h buffer.h
    insert.o : defs.h buffer.h
    search.o : defs.h buffer.h
    files.o : defs.h buffer.h command.h
    utils.o : defs.h

    .PHONY : clean
    clean :
        rm edit $(objects)
  ```
  **.PHONY**表示clean是一个伪目标文件
+ 一种奇怪的makefile文件写法(不推荐)
  ```makefile
  objects = main.o kbd.o command.o display.o \
    insert.o search.o files.o utils.o

    edit : $(objects)
        cc -o edit $(objects)

    $(objects) : defs.h
    kbd.o command.o files.o : command.h
    display.o insert.o search.o files.o :   buffer.h

    .PHONY : clean
    clean :
        rm edit $(objects)
  # 为了避免依赖重复的.h文件
  ```
  + 在rm命令前加一个小减号代表: 也许某些文件出现问题，但不要管，继续做后面的事。
## makefile规则
+ 显式规则。显式规则说明了如何生成一个或多个目标文件。这是由Makefile的书写者明显指出要生成的文件、文件的依赖文件和生成的命令。

+ 隐晦规则。由于我们的make有自动推导的功能，所以隐晦的规则可以让我们比较简略地书写 Makefile，这是由make所支持的。

+ 变量的定义。在Makefile中我们要定义一系列的变量，变量一般都是字符串，这个有点像你C语言中的宏，当Makefile被执行时，其中的变量都会被扩展到相应的引用位置上。

+ 文件指示。其包括了三个部分，一个是在一个Makefile中引用另一个Makefile，就像C语言中的include一样；另一个是指根据某些情况指定Makefile中的有效部分，就像C语言中的预编译#if一样；还有就是定义一个多行的命令。有关这一部分的内容，我会在后续的部分中讲述。

+ 注释。Makefile中只有行注释，和UNIX的Shell脚本一样，其注释是用 # 字符，这个就像C/C++中的 // 一样。如果你要在你的Makefile中使用 # 字符，可以用反斜杠进行转义，如： \# 。
  makefile中的命令必须要以Tab键开始。

### Makefile文件名
+ 默认的情况下，make命令会在当前目录下按顺序找寻文件名为“GNUmakefile”、“makefile”、“Makefile”的文件，找到了解释这个文件。在这三个文件名中，最好使用“Makefile”这个文件名，因为，这个文件名第一个字符为大写，这样有一种显目的感觉。
+ 当然，你可以使用别的文件名来书写Makefile，比如：“Make.Linux”，“Make.Solaris”，“Make.AIX”等，如果要指定特定的Makefile，你可以使用make的 -f 和 --file 参数，如： make -f Make.Linux 或 make --file Make.AIX。
### 引用其他的Makefile
+ 语法规则 "include <filename>", filename 可以是当前操作系统shell的文件模式(可以包含路径和通配符).
+ 在 include 前面可以有一些空字符，但是绝不能是 Tab 键开始。 include 和 <filename> 可以用一个或多个空格隔开。举个例子，你有这样几个Makefile： a.mk 、 b.mk 、 c.mk ，还有一个文件叫 foo.make ，以及一个变量 $(bar) ，其包含了 e.mk 和 f.mk ，那么，下面的语句：
  ```makefile
  include foo.make *.mk $(bar)
  # <==> expression below
  include foo.make a.mk b.mk c.mk e.mk f.mk
  ```
  make命令开始时，会找寻include所指出的其他Makefile，并把其内容安置在当前的位置。如果文件都没有指定绝对路径或是相对路径的话，make会在当前目录下首先寻找，如果当前目录下没有找到，那么make还会在下面的几个目录下找:
  1. 如果make执行时，有-I或--include-dir参数，那么make就会在这个参数指定的目录下寻找
  2. 如果目录<prefix>/include(一般是: /usr/local/bin或/usr/include)存在，make也会寻找
   如果有文件没有找到的话，make会生成一条警告信息，但不会马上出现致命错误。它会继续载入其它的文件，一旦完成makefile的读取，make会再重试这些没有找到，或是不能读取的文件，如果还是不行，make才会出现一条致命信息。如果你想让make不理那些无法读取的文件，而继续执行，你可以在include前加一个减号“-”。
   ```makefile
   -include <filename>
   ```
### 环境变量MAKEFILES
+ 如果你的当前环境中定义了环境变量 MAKEFILES ，那么，make会把这个变量中的值做一个类似于 include 的动作。这个变量中的值是其它的Makefile，用空格分隔。只是，它和 include 不同的是，从这个环境变量中引入的Makefile的“目标”不会起作用，如果环境变量中定义的文件发现错误，make也会不理。
    ```makefile
    # objects的值就是*.o, *.o并不会展开
    objects = *.o
    # 如果想要展开，可以像这样
    objects := *.o
    ```
#### 关键字
+ wildcard, patsubst
wildcard函数获取当前文件下所有匹配的文件
patsubst函数用于将文件模式进行替换。格式为"$(patsubst 原模式，目标模式，文件列表)"
    ```makefile
    # 列出一确定文件夹中的所有.c文件 wildcard用来获取工作目录下的.c文件列表
    objects := $(wildcard *.c)
    # 列出上述所有对应的.o文件
    $(patsubst %.c, %.o, $(wildcard *.c))
    
    #通过上述命令可以编译链接所有的.c和.o文件
    objects := $(patsubst %.c, %.o, $(wildcard *.c))
    foo : $(objects)
        cc -o foo $(objects)


    # example
    target5:
        @echo $(patsubst %.c, %.o, $(SOURCE_PWD))
    # 然后调用make target5之后就会输出目录下所有.c文件编译后对应的.o文件
    ```
#### 文件搜寻路径
##### VPATH
+ VPARTH
    ```makefile
    # 如果没有指明这个VPATH变量，make只会在当前目录中去寻找依赖文件和目标文件，如果定义了这个变量，make就会在当前目录找不到的情况下到所指定的目录中寻找
    # 以下指定了两个目录，src和../headers以：分开
    VPATH = src:../headers
    ```
##### vpath
+ vpath
    ```makefile
    # 为符合模式<pattern>的文件指定搜索目录<directories>
    vpath <pattern> <directories>
    # 清除符合模式<pattern>的文件的搜索目录
    vpath <pattern>
    # 清除所有已被设置好了的文件搜索目录
    vpath

    # Example
    # <pattern>中需要包含%字符
    # 在../headers目录下搜索所有以.h结尾的文件
    vpath %.h ../headers
    ```
##### .PHONY(伪目标)
+ .PHONY
    ```makefile
        .PHONY : clean
    ```
+ 只要有这个声明，不管是否有"clean"文件，要运行clean这个目标，只有"make clean"这样，于是整个过程可以这样写:
  ```makefile
  .PHONY : clean
  clean :
        rm *.o temp
  ```
+ 伪目标一般没有依赖的文件，但是我们也可以为伪目标指定所依赖的文件。伪目标同样可以作为“**默认目标**”，只要将它放在第一个。示例：如果想要一口气生成若干个可执行文件，但是想要简单执行make命令完成，所有的目标文件都写在一个Makefile中，那么可以使用.PHONY：
    ```makefile
    all : prog1 prog2 prog3
    .PHONY : all

    prog1 : prog1.o utils.o
        cc -o prog1 prog1.o utils.o

    prog2 : prog2.o
        cc -o prog2 prog2.o

    prog3 : prog3.o sort.o utils.o
        cc -o prog3 prog3.o sort.o utils.o       
    ```
    Makefile中的第一个目标会被作为其默认目标，我们声明一个all的伪目标，其依赖于其他三个目标。由于**默认目标**特性是总是被执行，但由于all是一个伪目标，伪目标只是一个标签不会生成文件，所以不会生成all文件。（其实这里的.PHONY : all不写一般情况也可以正确执行，这样make可以通过隐式规则推导。建议显示写出）
    ```makefile
    .PHONY : cleanall cleanobj cleandiff

    cleanall : cleanobj cleandiff
        rm program
    cleanobj :
        rm *.o
    cleandiff :
        rm *.diff
    ```
    可以执行"make cleanall"或者"make cleanob"或者"make cleandiff"
  
1. 承租期内租客在房屋内注意自身人生安全，房屋内发生一切人生安全问题与甲方无关；
2. 租客租期内转租内要经过房东同意，转租中介费由房客承担，且新客户须只能与房屋所有人签约。