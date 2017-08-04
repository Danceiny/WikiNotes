---
date: 2016-04-12 22:18
status: public
title: 2016-04-12-IPython
---

# Tab补全
只是查看，貌似不是真的补全
# ?自省
可以通配符：`np.*load*?`
# %run命令
所有文件可以通过%run命令当作python程序运行。

如果需要用到IPython命名空间内的变量，需要`%run -i`。

# 中断正在执行的代码
Ctrl+C

但是如果python代码已经调用了某个已编译的扩展模块时无效，只能等python解释器重新获得控制权，才能有效。

# 执行剪贴板中的代码
%paste和%cpaste两个魔术函数。

两个函数差不多，可以承载剪贴板中所有文本，并以整体形式执行。后者多出了一个用于粘贴代码的特殊提示符。

# IPython与编辑器、IDE之间的交互

# 键盘快捷键
|命令|说明|
|---|---|
|Ctrl+P或上箭头|后向搜索命令历史中以当前输入的文本开头的命令|
|Ctrl+N或下箭头|前向...|
|Ctrl+R|按行读取的反向历史搜索（部分匹配）|
|Ctrl+Shift+v|从剪贴板粘贴文本|
|Ctrl+A|将光标移动到行首|
|Ctrl+E|...行尾|
|Ctrl+K|删除光标开始至行尾的文本|
|Ctrl+U|清除当前行的所有文本|
|Ctrl+F|光标前移一个字符|
|Ctrl+b|..后移一个..|
|Ctrl+L|清屏|

# 异常和跟踪
有额外的上下文代码参考。
# 魔术命令 Magic Command
以%为前缀。

%timeit检测任意python语句的执行时间。

automagic技术可以通过%automagic打开或关闭，这样魔术命令就可以不加%前缀了（前提是没有同名变量）。

|常用的魔术命令|说明|
|---|---|
|%quickref|显示IPython的快速参考|
|%magic|显示所有魔术命令的详细文档|
|%debug|从最新的异常跟踪的底部进入交互式调试器|
|%hist|打印命令的输入历史|
|%pdb|在异常发生后自动进入调试器|
|%paste||
|%cpaste||
|%reset|删除解释器命名空间中的全部变量|
|%page OBJECT|通过分页器打印输出OBJECT|
|%run script.py|执行一个python脚本文件|
|%prun statement|通过cProfile执行statement，打印分析器的输出结果|
|%time statement|报告statement的执行时间|
|%timeit statement|多次执行statement以计算系综平均执行时间|
|%who、%who_Ls、%whos|显示interactive命名空间中定义的变量，信息级别/冗余度可变|
|%xdel variable|删除variable，清除其在IPython对象上的引用|

# 基于Qt的富GUI控制台
`ipython qtconsole`命令打开。

`ipython qtconsole --pylab=inline`好像也一样（据说为了集成matplotlib？）。

# 输入和输出变量
忘记把函数结果赋值给变量是一件令人郁闷的事情。

不过IPython中最近的两个输出结果分别保存在`_`和`__`中。

输入的文本被保存在名为`_iX`的变量中，其中X是输入行的行号。每个输入变量都有一个对应的输出变量`_X`。
