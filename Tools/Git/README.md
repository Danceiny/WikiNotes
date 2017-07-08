http://think-like-a-git.net/

# 忽略文件权限
git config core.filemode false

签出其他历史版本前，要把文件还原到原始状态，最简单是用`git reset --hard`，会损坏本地更改。

经常需要从GitHub上下载修正和改进后的源码用于更新本地仓库：
`git fetch --all`

`git fetch --tags` 

`git reset --hard origin/master`

(git fetch 命令用于利用远程仓库更新本地仓库的*提交历史和标签*，不会改动真正的源文件，随后的`git reset`才是改动（更新）源文件。

查看两个版本之间的区别：
`git diff 2a 2b`

# 分支
http://wuxiaolong.me/2017/03/09/gitBranch/

查看远程所有分支：

$ git branch -r
  origin/master
列出所有本地分支和远程分支：

$ git branch -a
创建本地 dev1 分支

$ git checkout -b dev1 
Switched to a new branch 'dev1'
本地 master 分支默认就是远程 master 分支，上面命令在此基础上创建本地 dev1 分支，然后切换到 dev1 分支，相当于以下两条命令：


$ git branch dev1
$ git checkout dev1
想从远程分支 dev （远程有该分支）创建本地分支 dev1：


$ git checkout -b dev1 origin/dev
Switched to a new branch 'dev1'

# 文件超大
## 移除

```
git rm -- THE_TOO_BIG_FILE
git commit --amend -CHEAD
```
执行完这步后，这个大文件将会从你的commit记录里移除，并且以后commit都将不会再把它commit了，这时候就可以git push把本地代码push到github上了。


## Git Large File Storage
https://git-lfs.github.com/

>Git Large File Storage (LFS) replaces large files such as audio samples, videos, datasets, and graphics with text pointers inside Git, while storing the file contents on a remote server like GitHub.com or GitHub Enterprise.
