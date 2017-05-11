签出其他历史版本前，要把文件还原到原始状态，最简单是用`git reset --hard`，会损坏本地更改。

经常需要从GitHub上下载修正和改进后的源码用于更新本地仓库：
`git fetch --all`

`git fetch --tags` 

`git reset --hard origin/master`

(git fetch 命令用于利用远程仓库更新本地仓库的*提交历史和标签*，不会改动真正的源文件，随后的`git reset`才是改动（更新）源文件。

查看两个版本之间的区别：
`git diff 2a 2b`