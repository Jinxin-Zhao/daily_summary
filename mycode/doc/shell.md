### awk
+ 

###vim editor
+ remove string like this:
    - d+w:  (127.9.8.7.000)---->(.98.7.000)
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

###tar
+ param:[A c d r u x f v]
[note]: f must be the last
    - c: create file
    - f: specify  filename
    - v: verbose,show more infomation of source files
    - x: exact
    - t: show info of tar
    - A: merge series of tar files
    - r: add more source files to tar files
```
    tar -cvf test.tar file1 file2
    tar -tvf test.tar

    tar -rvf test.tar new_file

    #extract all files to current path
    ]# tar -xvf test.tar
    #extract all files to specified path
    ]# tar -xvf test.tar -C /path/to/extract_dir/
    #extract specified files to current path
    ]# tar -xvf test.tar file1 file3

    ]# tar -cvf - files/ | ssh amazon@172.16.2.211 "tar xv -C Documents/"

    #merge 2 tar files
    ]# tar -Af file1.tar file2.tar
    ]# tar -tvf file1.tar

    #if file in tar files has not changed,nothing will happen
    ]# tar -tvf  test.tar
        file1
        file2
    ]# tar -uf test.tar file1


    #delete files from tar files
    ]# tar -f test.tar --delete file1 file2
    or ]# tar --delete --file test.tar file1 file2

    #exclude specified files from tar files
    ]# tar -cf test.tar * --exclude "*.txt"
    or you can put file1&file2 to a dir list,then:
    ]# tar -cf test.tar * -X list
```
### array && if else && while
```
    #a blankspace between each words and no comma
    a=( "dir_1" "dir_2" "dir_3" )
    i=0
    while [ $i -lt ${#a[@]} ]
    do
        echo ${a[$i]}
        if [ -d ${a[i]} ];then
            echo exist
        else
            mkdir ${a[i]}
        fi
        let i++
    done

#######

#shell判断文件夹是否存在
 
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