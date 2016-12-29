---
date: 2016-07-25 16:13
status: public
title: Linux下C开发工具
---

# GCC选项
* `gcc -p -g test.c`
编译test.c时为prof命令建立剖析（profile）信息且把调试信息加入到可执行文件中。
* `gcc -pg test.c`
为gprof命令建立剖析信息。
* `gcc test.c`
不用任何选项编译时，会建立一个名为a.out的可执行文件。
* `gcc -o count count.c`
使用-o选项指定一个可执行文件名代替a.out。
* -c
仅把源代码编译为
目标代码，而跳过汇编和连接的步骤。缺省时的目标代码有一个.o的扩展名。

* -S
产生汇编语言文件后停止编译，产生的汇编语言文件的缺省扩展名为.s。

* -E
仅对输入文件进行预处理。缺省时的预处理器输出被送到标准输出而不是储存到文件中。

# 优化选项
GCC默认会用最少的时间完成编译并且使编译后的代码易于调试，而优化选项可以使GCC在耗费更多编译时间和牺牲易调试性的基础上产生更小更快的可执行文件。
* -O
对源代码进行基本优化。
* -O2
产生尽可能小和尽可能快的代码。其编译速度比-O更慢，但产生的代码执行速度会更快。

## 调试和解析选项
* -g
产生能被GNU调试器使用的调试信息以便调试程序。
**GCC可联用-g和-O。**
* -pg
在程序中加入额外代码，产生gprof用的剖析信息以显示程序耗时情况。

# 用gdb调试GCC程序
gdb功能：
1. 监视程序中变量的值
2. 设置断点
3. 逐行执行代码

## gdb基本命令
|命令|描述|
|----|----|
|file|装入将调试的可执行文件|
|kill|终止正在调试的程序|
|list|列出产生执行文件的源代码的一部分|
|next|执行一行源代码，但不进入函数内部|
|step|执行一行源代码，但进入函数内部|
|run|执行当前被调试的程序|
|quit|终止gdb|
|watch|监视一个变量值，不管何时被改变|
|print|显示表达式的值（可以给变量临时赋值，调用函数）|
|break|在代码中设置断点|
|make|不退出gdb就可重新产生可执行文件|
|shell|不退出gdb就可执行UNIX shell命令|

delete Num可删除断点等记录。
dis Num使不能断点等。
jump Num跳到Num执行
## gdb应用实例
被调试的程序hello，显示一个简单的问候，并反序列出。
```c:n 
#include <stdio.h>;
static void my_print (char *);
static void my_print2 (char *);
main ()
{
char my_string[] = "hello world!";
my_print (my_string);
my_print2 (my_string);
}
void my_print (char *string)
{
printf ("The string is %s ", string);
}
void my_print2 (char *string)
{
char *string2;
int size, i;
size = strlen (string);
string2 = (char *) malloc (size + 1);
for (i = 0; i < size; i++)
string2[size - i] = string;
string2[size+1] = '';
printf ("The string printed backward is %s ", string2);
}
```
编译：
`gcc -g -o hello hello.c`
执行时显示：
```
../hello
The string is hello world!
The string printed backward is ...（乱码）
```
显然my_print2函数没有正常工作。使用gdb调试。

`gdb hello`

>若在输入命令时忘了把要调试的程序作为参数传给gdb，则可以在gdb提示符下用file命令载入。

`run`运行hello。

反序打印失败，为找出bug，在my_print2函数的for语句后设一个断点。
具体做法是在gdb提示符下键入list命令，列出源代码，一次只会列出有限的几行，并且带有行号。于是可以找到需要打断点的那一行的位置。

`break 24`

gdb响应：Breakpoint 1 at 0x4016a3: file hello.c, line 24.

再次`run`，运行到断点处停下来了。此时该观察变量值了。

`watch string2[size-i]`

gdb响应：Hardware watchpoint 2: string2[size-i]

现在用next命令一步步执行for循环。

`next`

gdb会响应告知所watch的变量的Old value和New Value。

一直next，看到已经没有值分配给string2[0]了，而这时新串的第一个字符，因为malloc函数在分配内存时初始化为null字符，因此string2的第一个字符是空字符，所以打印string2时没有任何输出。。。

bug找到了，string2的偏移设置不对，应该是string2[size-1-i]=string,bi=0时，string2[size-1]才是string的尾字符，其后string[size]是'\0'。

如果程序产生了core文件，则可以用`gdb hello core`查看程序在何处出错。如果忘了给string2分配内存，很可能就会core dump。

# 其他的C编程工具
## xxgdb
gdb的基于X Window的图形界面。
## calls
## calltree
## cproto
## indent
美观的缩进~
## gprof
安装在/usr/bin目录下的一个程序。可以知道程序的哪一部分最耗时间。对于提升程序性能十分有用。
`gcc -pg -o hello hello.c` 
`gprof hello | more`

>gprof产生的剖析数据很大，最好把输出重定向到一个文件中。
# 相关笔记目录
## [基础知识-gcc\makefile](http://danceiny.farbox.com/post/c/linuxcao-zuo-xi-tong-xia-cyu-yan-bian-cheng-ru-men)
## [时间概念-时间测量](http://danceiny.farbox.com/post/c/linux_c-shi-jian-gai-nian)
## [消息管理-signal](http://danceiny.farbox.com/post/c/linux_c-xiao-xi-guan-li)
## [网络编程-Socket\IP\TCP\UDP](http://danceiny.farbox.com/post/c/linux_c-wang-luo-bian-cheng)
## [线程操作-文件夹备份](http://danceiny.farbox.com/post/c/linux_c-xian-cheng-cao-zuo)
## [开发工具-gcc/gdb](http://danceiny.farbox.com/post/c/linuxxia-ckai-fa-gong-ju)