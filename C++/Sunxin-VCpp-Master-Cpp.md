---
date: 2016-04-28 14:38
status: public
title: Sunxin-VCpp-Master-Cpp
---

# 输入输出流

|C++| C|
|----|----|
|cin |	scanf|
|cout|	printf|
|cerr| 	stderr|
|iostram.h|	stdio.h|
|endle|	\n|
|struct 可以有函数|不可以|
endl == end of line

# 类的继承访问特性
|基类的访问特性|类的继承特性|子类的访问特性|
|-------|------|---------|
|Public||Public|
|Protected|Public|Protected|
|Private||No access|
|Public||Protected|
|Protected|Protected|Protected|
|Private||No access|
|Public||Private|
|Protected|Private|Private|
|Private||No access|


# 作用域标识符`::`
使用`::`只能调用类中的静态成员函数。

静态成员函数中不能引用非静态中的成员变量。

因为非静态的成员变量还没分配内存空间。

而非静态可以引用静态的~

利用内存模型，分析哪些可以访问哪些。

静态成员变量需要在类外部做初始化。

**（如果不访问静态成员变量就可以不初始化了。如果访问而不初始化，则会链接错误，而非编译错误。）**

# 句柄 handle
操作对象。

# GCC