# 跨平台开发不同系统换行符统一问题(QT为例)
+ 在各自平台执行命令:
```shell
# 提交时CRLF→LF，检出时LF→CRLF。保护Windows本地环境
]$ git config --global core.autocrlf true

```
在windows环境下最好加上.gitattributes文件，它能在项目级别强制规则，不依赖个人设置，确保代码库统一使用LF。对于Windows批处理等特定文件，可以例外使用CRLF。内容如下:
```shell
# 核心规则：让Git自动判断文本文件，并统一为LF换行符
* text=auto eol=lf

# 明确一些Qt项目常见的文本文件类型
*.pri text
*.pro text
*.cpp text
*.h text
*.qml text
*.ui text

# 如果项目中有Windows批处理文件，为其保留CRLF
*.bat text eol=crlf
*.cmd text eol=crlf

# 排除二进制文件，防止Git误处理
*.png binary
*.jpg binary
*.zip binary
```
然后将这个文件提交（commit）并推送（push）到远程仓库。

然后同步设置macos/linux环境：
```shell
# 提交时CRLF→LF，检出时不转换。保护macOS/Linux环境并确保仓库纯净
]$ git config --global core.autocrlf input
```
并拉取包含.gitattributes文件的最新项目代码。

补充建议：
如果问题已经存在：如果仓库中已经混用了换行符，可以在配置好.gitattributes后，执行以下命令进行一次彻底清理：
```shell
git add --renormalize .
git commit -m "统一换行符"
```
关于Qt源码文件编码的额外提示：除了换行符，跨平台Qt开发时，源码文件的字符编码也建议统一为UTF-8 with BOM，这可以更好地避免在Windows（如MSVC编译器）和macOS上出现中文等特殊字符的乱码问题。



<h2>git操作流程</h2>

在提交本地文件到版本库时，需要对提交的内容进行界定，原则上只提交项目中的代码、构建脚本等相关内容，不应提交编译过程产生的文件或者是运行程序等

现假设有(A-Group/开发者A)仓库A，(开发者B)进行Fork产生仓库为B

<h5>Step 1.Fork远程仓库</h5>

登录GitLab,找到目标仓库A进行Fork操作(该步操作将目标仓库Fork到自己的用户下)

点击后将跳转到用户(组)的选择，选择自己账户等待Fork完成,在完成后将自动跳转至B仓库。

<h5>Step 2.Clone远程仓库</h5>

Clone之前Fork的远程B仓库到本地：

``` shell
 # 这里使用ssh克隆B仓库到本地
git clone -b master git@172.30.241.39:B-developer/B-repository.git

```

<h5>Step 3.添加A仓库地址到本地remote</h5>

``` shell
# 将项目A的git地址添加到本地remote
git remote add upstream git@172.30.241.39:A-Group/A-repository.git  
```

<h5>Step 4.Checkout新分支</h5>

``` shell
 # new-dev为新分支名称，应与A中的目标分支名保持一致
git checkout -b 分支名称
```

<h5>Step 5.A仓库同步到B仓库</h5>

``` shell
# pull远程A仓库中的分支到本地
git pull upstream 分支名称
# 将pull下来的A仓库push到B仓库中，以完成同步
git push origin 分支名称
```
以上Step4和Step5两步可以合并，命令如下：

``` shell
git checkout -b 分支名称 upstream/分支名称

```

<h5>Step 6.更新本地B仓库分支</h5>

前提:本地B仓库的开发分支内容滞后于A仓库对应的分支

``` shell
# 获取A最近更新
git fetch upstream
# 切换至你需要更新B的分支
git checkout B的分支
# 将A的更新合并到B的分支
git merge upstream/B的分支
```

<h5>Step 7.Commit到B分支</h5>

这里需要对git add命令作简要说明：

序号|命令|用途|备注
:-:|:-|:-|:-
1|git add $$ |添加指定文件到暂存区|$$为文件名
2|git add .|监控modified、new文件提交到暂存区，不包含：delete|
3|git add -u|监控tracked file，提交到暂存区|git add --update
4|git add -A|包括以上两个功能的合集|git add --all

介绍一下commit操作过程中相关命令：

序号|命令|用途
:-:|:-|:-
1|git status|查看文件的状态，包括：new、modified、deleted
2|git reset|重置上一次操作
3|git commit|提交到仓库

* 新增文件commit操作
``` shell
touch halo.cc          # 创建新文件halo.cc
git add halo.cc        # 提交到暂存区
git status             # 查看文件状态
git commit -m "commit halo.cc"  # 提交文件到本地仓库，务必添加提交信息
```
* 修改文件commit操作
``` shell
git add -u             # modified文件提交到暂存区
git status             # 检查文件状态
git commit -m "modified halo.cc"   # 提交修改的文件到本地仓库
```
* 删除文件commit操作
``` shell
git rm halo.cc         # 删除版本库中指定文件
git commit -m "rm halo.cc"   # 提交删除
```
<font color="orange">注:</font>若不小心使用linux的rm命令删除了文件，可通过git checkout -- (<font color="green">文件名</font>)找回。

<h5>Step 8.Push到远端B仓库</h5>

commit后就可以推送当前分支到远端仓库。

<font color="orange">示例中local_branch应与remote_branch保持一致，这里以示区分</font>

* 本地新建开发分支local_branch,推送当前local_branch开发分支到远端开发分支(远端当前不存在该分支)
``` shell
git push origin local_branch:origin_branch
```
* 切换到local_branch分支，推送当前开发分支到已有远端开发分支
``` shell
git push
```
* 已有远程分支remote_branch，但未关联本地local_branch,且需要推送当前分支
``` shell
git push -u origin/remote_branch    
```

<h5>Step 8.发起Merge请求</h5>

在B仓库下完成相应功能开发后，发起Merge到A仓库的请求。

* 找到GitLab左侧的Merge Requests选项
* 配置好Source branch与Target branch

即可完成Merge请求
最终由A仓库管理者完成代码审核、合并等操作。

<font color="orange">附一：文件冲突解决</font>

例：冲突文件:"App.java"
冲突内容形式如下：
``` java
public static void main(String[] args) {
    <<<<<<<< HEAD
    String head = "I come from origin";
    ========
    String end = "I come from here";
    >>>>>>>> feature
}
```
Git 用<<<<<,====,>>>>>标记不同分支内容，此处如果采用命令行模式，那么需要开发人员手动处理冲突部分的内容

修改完成后请再次提交：
``` shell
git add App.java
git commit -m "[Shell.App] conflict fixed."
```

##### Step 9.git的revert版本回退
reset（不推荐）： 通过reset的方式，把head指针指向之前的某次提交，reset之后，后面的版本就找不到了；
    version_1 -> version_2 -> version_3
    reset之后，目标版本之后的版本就不见了（这里reset到了version_1）
    version_1

    1. 在gitlab上找到要恢复的版本号，如：139dcfaa558e3276b30b6b2e5cbbb9c00bbdca96
    2. 执行命令
    git reset --hard 139dcfaa558e3276b30b6b2e5cbbb9c00bbdca96
    3. push到对应的远程分支(如提交到develop分支)
    git push -f -u origin develop

    缺点：现在到服务器上看到的代码就已经被还原回去了。这种操作存在一个问题，服务器上的代码虽然被还原了，但假如有多个人在使用，他们本地的版本依然是比服务器上的版本高的，所以，别人再重新提交代码的话，你撤销的操作又会被重新，你上面的操作也就白操作了。解决办法是，让别人把本地的分支先删掉，然后重新从服务器上拉取分支



revert（推荐）:
 这种方式不会把版本往前回退，而是生成一个新的版本。所以，你只需要让别人更新一下代码就可以了，你之前操作的提交记录也会被保留下来。

 revert前：
    version_1 -> version_2 -> [version_3]
 revert后生成一个新版本：
    version_1 -> version_2 -> version_3 -> [version_4]

    1. 找到你之前的版本号（通过 git log）
    2. git revert -n [version no.]
    3. git commit -m xxx 
    4. git push origin [branch name]

# git 常用cmd
+ 如果想要一次性add所有修改过的文件，而不想在git add指令后一个接一个地新增文件，可以使用
```shell
# 运行git add .命令将会将当前目录及其所有子目录下的所有修改的文件添加到暂存区（Index），准备提交到版本控制系统。这包括新创建的文件、修改过的文件和已删除的文件。

]# git add . 

# 需要注意的是，git add .会添加所有修改的文件，包括未跟踪的文件。如果你只想添加已跟踪的文件，可以使用git add -u命令。
]# git add -u
```

## git 问题解决
+ **问题描述**： remote: Support for password authentication was removed on August 13, 2021 
解决方案：
    -  1.生成一个个人访问令牌：
在 GitHub 网站上，点击右上角的头像，选择 "Settings"（设置）。
在左侧菜单中，选择 "Developer settings"（开发者设置）。
在左侧菜单中，选择 "Personal access tokens"（个人访问令牌）。
点击 "Generate new token"（生成新令牌）按钮。
提供令牌的描述，勾选需要的权限范围（例如 repo、gist、admin:org 等）。
点击 "Generate token"（生成令牌）按钮。
复制生成的访问令牌，它将是你进行身份验证的凭证。
    - 2.更新 Git 仓库的远程 URL：
    打开你的本地 Git 仓库目录。
    运行以下命令更新远程 URL，将 <TOKEN> 替换为你生成的个人访问令牌：
    ```shell
    git remote set-url origin https://<USERNAME>:<TOKEN>@github.com/<OWNER>/<REPO>.git
    ```
    其中，<USERNAME> 是你的 GitHub 用户名，<OWNER> 是仓库的所有者，<REPO> 是仓库的名称。

+ **问题描述**：git pull 突然卡住
    - 问题分析：这个问题就是DNS解析被污染了，有两种可能：
        - DNS解析被运营商劫持了
        - 使用了VPN（概率比较大）
    - 解决方案：
    ```shell
    # ssh -v命令中的 -v 代表verbose
    ]$ ssh -vT git@github.com
    # output
    OpenSSH_8.0p1, OpenSSL 1.1.1k  FIPS 25 Mar 2021
    debug1: Reading configuration data /etc/ssh/ssh_config
    debug1: Reading configuration data /etc/ssh/ssh_config.d/05-redhat.conf
    debug1: Reading configuration data /etc/crypto-policies/back-ends/openssh.config
    debug1: configuration requests final Match pass
    debug1: re-parsing configuration
    debug1: Reading configuration data /etc/ssh/ssh_config
    debug1: Reading configuration data /etc/ssh/ssh_config.d/05-redhat.conf
    debug1: Reading configuration data /etc/crypto-policies/back-ends/openssh.config
    debug1: Connecting to github.com [20.205.243.166] port 22.
    debug1: connect to address 20.205.243.166 port 22: Connection refused
    ssh: connect to host github.com port 22: Connection refused
    ```
    上面的信息显示github的地址是20.205.243.166，显然是DNS解析出错了，可以通过网址(https://www.ipaddress.com)来查询，直接在框子中输入github.com点击搜索，直接会显示出IP地址，然后将此映射加入/etc/hosts文件。