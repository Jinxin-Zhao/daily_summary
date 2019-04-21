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
