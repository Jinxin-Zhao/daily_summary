# 版本发布

## 版本发布流程
* 变更收集->静态代码检查->代码封板->集成测试->版本号更新->发布测试->仿真并行上线->版本发布

## 变更收集
* 迭代开始时，在gitlab上建立对应于本次迭代版本的Milestone
* 封板前5天开始进行变更收集（建议根据gitlab中Merged的MR来统计收集）
* 分类：[缺陷修复] [新业务] [优化] [工具变更] [API变更] [配置文件变更]
* 检查变更是否已登记jira，jira记录信息是否完善

## 静态代码检查
* 收集本版本内所有变更的文件列表，使用TscanCode工具进行静态代码检查
* 主要关注"致命""严重""警告"级别的错误

## 测试
版本负责人需在迭代期间持续关注如下测试任务，出现问题及时解决
1. new_lua_business_test 集成测试（提交测试）
    * 向business分支提交MR后执行
    * 封板后执行
2. NGTS-MR-Msg-Check 集成测试
    * 检查提交信息是否规范
3. ngts_regression_tradetest 回归测试
    * 每日定时执行
4. ngts_arch_test 架构测试
    * 每日定时执行
5. 反演测试 （待建立）
6. API接口测试
    * 测试新增接口功能（暂时手动）
7. 覆盖率测试 （待建立）
8. ngts_release_business 发布测试
    * 向business_release分支提交merge request后执行

## 版本号更新
代码封板后由版本负责人更新系统版本号，需修改的内容如下：
1. 修改NGTS仓库系统版本
    * source/version/VersionValue.h
    * target/cprun.sh
    * target/mall
2. 修改API版本（如有需要）
    * source/ffexuserapi/FtdcUserApiMacro.h.tpl
3. 修改CI仓库版本（master分支）
    * scripts/version_release/release.ini
    * scripts/phase1.sh

## 版本打包
1. 确认各项测试通过、版本无问题后，可执行Jenkins中的ngts_release_version任务进行版本打包，打包后的版本位于：
jenkins_release@172.31.197.1:~/artifact/releaseversion${number}
2. windows版api需独立编译，详见《windows版api编译说明》
3. 为定稿版本在gitlab上打上tag，并填入完整的发布信息，主要内容如下
    * 发布位置
    * 发布清单
    * 发布内容
    * 缺陷修复
    * 优化
    * 新业务
    * 配置文件修改
    * 工具修改
4. 若为增量发布则，解压待发布的压缩包并与前版本比对、发布信息比对

## 仿真并行上线
仿真并行上线除替换交易系统二进制文件外，还需注意：
* 是否有配置文件修改（config、ini文件）
* 是否有工具变更（slog_replay、flowtransfer、flowadapter等）
* 是否有脚本修改（反演脚本、结果比对脚本等）

# windows版API编译说明

## 编译环境
通过远程桌面登录
* 虚拟机名：TSD-NGTS_API
* 虚拟机IP：172.31.195.47
* 用户名/密码：cffex/cffEx2016

## 虚机配置
* 操作系统： Windows 7 64位
* 内存：16GB
* 编译器：Visual Studio 2012
 
## 工具
1. amake
    amake工具位于C:\build_tool\amake\amake.bat
    相关环境变量已设置
2. pump
    pump工具位于C:\build_tool\pump\bin\pump.exe
    相关环境变量已设置

## API编译步骤
以编译TraderApi为例：
以管理员权限进入“VS2012 开发人员命令提示”
1. 进入source\openssl目录，将package.des.win32重命名为package.des
2. 进入C:\NGTS\target\ffextraderapidll目录，依次执行：
    amake.bat ffextraderapidll_win32.prj
    nmake pump
    nmake


# 开发流程

## 分支模型
交易系统[NGTS](http://172.31.112.162:85/eTechTSD/NGTS)项目只维护如下三个分支：
* `master`分支：用于发布正式的生产版本
* `release`分支：用于发布提测版本
* `feature/develop`分支：用于合并大家提交的功能点

## Merge requests
交易系统[NGTS](http://172.31.112.162:85/eTechTSD/NGTS)项目的所有代码、测试和文档通过`merge request (MR)`的形式进行演进。

以`merge request (MR)`方式进行工作的主要流程如下所示：

1. 从交易系统[NGTS](http://172.31.112.162:85/eTechTSD/NGTS)项目 `Fork`代码库到您的私人空间中
2. 从(`feature/develop`)最新的分支`创建新分支`用于添加本次修改代码
3. 拉取您`私人空间`中的代码库到本地
4. 修改或增加代码、文档、测试用例
5. 在本地测试通过，在`本地仓库提交代码`
6. `推送`本地修改到您私人空间中的NGTS项目库，如有琐碎的提交，请在推送前使用`git squash`来合并
7. 从您私人空间中的NGTS项目库向交易系统[NGTS](http://172.31.112.162:85/eTechTSD/NGTS)项目发起`merge request (MR)`

> `注意：gitlab中私人空间fork出的代码库并不会与NGTS项目库自动同步`
这意味着您需要定期将私人库与NGTS库手动同步。
`因为有此需求，所以建议大家不要直接在feature/develop分支上开发，而是保留此分支用于追踪NGTS库中feature/develop分支`。

## 操作示例

下面假设需要进行保证金业务的修改，计划在`feature/margin`分支上进行开发，`feature/develop`保持干净用于追踪NGTS库中feature/develop分支。
具体步骤如下：

### A.开发前准备工作 (如添加过无需重复此步骤)
1. 从交易系统[NGTS](http://172.31.112.162:85/eTechTSD/NGTS)项目 `Fork`代码库到您的私人空间中
   假设私人仓库地址为：http://172.31.112.162:85/guiyang/NGTS.git
2. clone私人仓库到本地
```
git clone http://172.31.112.162:85/guiyang/NGTS.git
```
3. 添加交易系统[NGTS]库作为自己的远程仓库,命名为`NGTS`。
```
git remote add NGTS http://172.31.112.162:85/eTechTSD/NGTS.git
```

### B.同步NGTS项目仓库与私人仓库
1. 拉取远程仓库NGTS内容到`feature/develop`分支
```
git checkout feature/develop
git pull NGTS feature/develop
```
2. 将最新的`feature/develop`分支推送到私人仓库
```
git push origin feature/develop:feature/develop
```

### C.开发与测试
1. 从`feature/develop`分支新建`feature/margin`分支用于开发新功能
```
git checkout feature/develop
git checkout -b feature/margin
```
2. 添加代码、测试用例、文档
3. 提交更改
   `git commit`
   
### D.重新同步`feature/develop`分支并推送`feature/margin`分支
1. 拉取远程仓库NGTS内容到`feature/develop`分支
```
git checkout feature/develop
git pull NGTS feature/develop
```
2. 将`feature/margin`分支的修改应用到`feature/develop`分支，有变基和合并两种方式可供选择
```
变基方式
git checkout feature/margin
git rebase NGTS/feature/develop
合并方式
git checkout feature/margin
git merge NGTS/feature/develop
```
3. 推送`feature/margin`分支到私人仓库`feature/margin`分支*注意：此处不是推送到`feature/develop`分支*
```
git push origin feature/margin:feature/margin
```
4. 从[GitLab](http://172.31.112.162:85)中发起私人仓库`feature/margin`分支到[NGTS](http://172.31.112.162:85/eTechTSD/NGTS)项目`feature/develop`分支的`merge request (MR)`
5. 待`merge request (MR)`合并后同步本地仓库`feature/develop`分支。
```
git pull NGTS feature/develop
```
6. 待`merge request (MR)`合并后可根据需要删除私人仓库`feature/margin`分支。

# 问题跟踪
交易系统使用![JIRA][logo] `JIRA`来记录问题与缺陷，所有已知问题都记录在[JIRA-NGTS](http://172.31.112.120:8081/issues/?jql=project=NGTS)。
[logo]: http://172.31.112.120:8081/download/resources/com.metainf.jira.plugin.emailissue/images/jeti-icon.png

## 报告问题
报告问题之前请确认问题未在[JIRA-NGTS](http://172.31.112.120:8081/issues/?jql=project=NGTS)中报告过。
报告问题应当提供相应版本的SHA提交编号，最好附上相应的用例帮助复现该问题。