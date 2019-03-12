# SourceInsight
alt+<
alt+>
# Beyond Compare
Windows序列号：
>H1bJTd2SauPv5Garuaq0Ig43uqq5NJOEw94wxdZTpU-pFB9GmyPk677gJ
vC1Ro6sbAvKR4pVwtxdCfuoZDb6hJ5bVQKqlfihJfSYZt-xVrVU27+0Ja
hFbqTmYskatMTgPyjvv99CF2Te8ec+Ys2SPxyZAF0YwOCNOWmsyqN5y9t
q2Kw2pjoiDs5gIH-uw5U49JzOB6otS7kThBJE-H9A76u4uUvR8DKb+VcB
rWu5qSJGEnbsXNfJdq5L2D8QgRdV-sXHp2A-7j1X2n4WIISvU1V9koIyS
NisHFBTcWJS0sC5BTFwrtfLEE9lEwz2bxHQpWJiu12ZeKpi+7oUSqebX+</pre>

(http://www.voidcn.com/blog/mandagod/article/p-4938654.html)
# Git
tag管理

http://blog.csdn.net/weixi6/article/details/7433322

# 正则表达式
分支条件用|，左边匹配了就不会匹配右边了，所以顺序不能调换。

下面两个正则不一样：(\1是指前面一个括号里匹配的文本，而不是括号中的正则)
`\b(\w+)\s+\1\b` == hello hello
`\b\w+\s+\w+\b` == hello world

难点：平衡组

* IPv4
([012]\d\d\.){3}\.(\d){1,3}

^((25[0-5])|2[0-4]\d|[01]?\d\d?\.){3}(25[0-5]|2[0-4]\d|[01]?\d\d?)$

* QQ
[1-9][0-9]{4,11}

* Email
\w+@\w+\.\w+

`\w+([-+.]\w+)*@\w+([-.]\w+([-.]\w+)*\.\w+([-.]\w+)*`
# shell
[重定向](https://www.ustack.com/blog/%E6%9C%89%E5%85%B3-shell-%E9%87%8D%E5%AE%9A%E5%90%91%E9%82%A3%E4%BA%9B%E4%BD%A0%E4%B8%8D%E7%9F%A5%E9%81%93%E7%9A%84%E6%95%85%E4%BA%8B/)

# TOP10问题
## TOP1 ----- 返回值处理
对返回值要检查后再使用。

未处理返回值，可能导致空指针方位、缺少回退处理（资源泄露等）。

可能执行失败的函数，不能直接将返回值赋出。

## TOP2 ------- 断言的使用
断言用于对程序运行过程中**不应该发生**的情况进行检查。
条件判断用于对程序运行过程中**可能发生**的情况进行处理。

1. 断言中不能含有业务逻辑，只能由逻辑表达式，因为Release版本断言无效。

2. 对于同一个文件中的函数，认为是同一个模块的内部函数，相互之间的调用要求输入参数由调用者保证。模块的内部函数建议使用断言检查参数的有效性。

3. 对于static或inline函数要求输入参数由调用者保证，该类函数建议使用断言检查参数的有效性。

## TOP3 -------- 系统资源使用
系统资源包括但不限于：动态内存、操作句柄（文件、接口、WAITLIST）、中断、信号量。
资源申请和释放应在同一层次。

对称的资源操作之间有异常退出会导致资源泄露。

资源申请应该在合法性和前提条件OK之后进行。
建议将申请的资源直接赋给间接变量，所有操作成功后再操作（赋值或挂接）数据结构，而不是首先赋给局部变量。

## TOP4 ------ 内存释放
链表中的数据，要先“摘链”再释放。

## TOP5 ------- 内存越界
## TOP6 ------- 空野指针
释放全局句柄资源后也应该清理句柄本身。
## TOP7 ------- 未初始化
## TOP8 ------- 代码冗余
结构体赋值可以整块内存区域赋。
## TOP9 ------- 编程接口
## TOP10 -----  资源型接口设计

## 被复杂化的需求
## 
函数不超过200行，参数不超过7个。循环嵌套不超过4层，![](index_files/4896359.png)

# 基本编程规范
![](index_files/6816218.png) 
![](index_files/6839296.png)![](index_files/6860890.png)
![](index_files/6788203.png)

一般函数的实现应该做到“最少信息即可调用”。![](index_files/7637218.png)
![](index_files/8109250.png)
![](index_files/8585625.png)
![](index_files/8543187.png)
![](index_files/8615828.png)

strlcpy比strncpy更安全，补充\0，且不修改整个目的缓冲区。**strlcpy（包括该系列函数）的第三个参数最好设置为目的缓冲区的空间大小(sizeof).**
![](index_files/8680281.png)

![](index_files/8809781.png)
![](index_files/8962125.png)

![](index_files/9283421.png)

`const CHAR * pcString = "";//const字符串指针一般初始化为空字符串而不是空指针。`

**字符串缓冲区在使用前，建议通过清零首字节来初始化。**
![](index_files/3121234.png)

**如果必须动态复制字符串数据，可以使用strdup接口。![](index_files/3200484.png)



申请动态内存必须判断返回值是否为空(malloc,calloc,realloc,kmalloc,kzalloc等).

![](index_files/24378312.png)![](index_files/24387359.png)

**内存释放后必须立即将指针置空。**

动态资源的申请释放尽量在同一模块/子模块完成。