###Terminal 下不同类型的文件显示不同的颜色
+ Terminal 默认的 shell 是 bash (提示符是 $)

####
1. 在 ~ 先建立一个文件  ~/.profile

2. 加入别名，显示不同文件的颜色


alias ls=”ls -G”
存盘， 退出Terminal, 重起Terminal. 能显示不同文件颜色了，但是不能更改颜色


3. .profile加入下面的两行：

export CLICOLOR=1
export LSCOLORS=gxfxaxdxcxegedabagacad

存盘， 退出Terminal, 重起Terminal.
更改LSCOLORS可以有不同的颜色效果。 



4.配置

那么应该怎样来配置成我喜欢的颜色呢？下面我们就来详细说一些这些配置。

~/.profile是bash shell中当前登录用户的配置文件。bash是“终端”中默认的shell。

alias ls=”ls -G”是给”ls -G”起了一个别名，当执行ls时，就相当于执行了ls -G。

CLICOLOR是用来设置是否进行颜色的显示。CLI是Command Line Interface的缩写。

LSCOLORS是用来设置当CLICOLOR被启用后，各种文件类型的颜色。LSCOLORS的值中每两个字母为一组，分别设置某个文件类型的文字颜色和背景颜色。LSCOLORS中一共11组颜色设置，共22个设置，按照先后顺序，先前景色，后背景色，分别对以下的文件类型进行设置：

directory
symbolic link
socket
pipe
executable
block special
character special
executable with setuid bit set
executable with setgid bit set
directory writable to others, with sticky bit
directory writable to others, without sticky bit
LSCOLORS中，字母代表的颜色如下：

a 黑色
b 红色
c 绿色
d 棕色
e 蓝色
f 洋红色
g 青色
h 浅灰色
A 黑色粗体
B 红色粗体
C 绿色粗体
D 棕色粗体
E 蓝色粗体
F 洋红色粗体
G 青色粗体
H 浅灰色粗体
x 系统默认颜色
所以，如果我们想把目录显示成红色，就可以把LSCOLORS设置为bxfxaxdxcxegedabagacad就可以了

////////////////////////////////////////////

LSCOLORS 的含义：

    LSCOLORS The value of this variable describes what color to use
    for which attribute when colors are enabled with
    CLICOLOR. This string is a concatenation of pairs of the
    format fb, where f is the foreground color and b is the
    background color.

    The color designators are as follows:

    a black
    b red
    c green
    d brown
    e blue
    f magenta
    g cyan
    h light grey
    A bold black, usually shows up as dark grey
    B bold red
    C bold green
    D bold brown, usually shows up as yellow
    E bold blue
    F bold magenta
    G bold cyan
    H bold light grey; looks like bright white
    x default foreground or background

    Note that the above are standard ANSI colors. The actual
    display may differ depending on the color capabilities of
    the terminal in use.

    The order of the attributes are as follows:

    1. directory
    2. symbolic link
    3. socket
    4. pipe
    5. executable
    6. block special
    7. character special
    8. executable with setuid bit set
    9. executable with setgid bit set
    10. directory writable to others, with sticky bit
    11. directory writable to others, without sticky
    bit

    The default is “exfxcxdxbxegedabagacad”, i.e. blue fore-
    ground and default background for regular directories,
    black foreground and red background for setuid executa-
    bles, etc.