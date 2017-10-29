---
date: 2016-05-14 19:36
status: public
title: _tmain与main的区别
---

_tmain:
　　1. Main是所有c或c++的程序执行的起点，_tmain是main为了支持unicode所使用的main的别名 ._tmain()不过是unicode版本的的main() .
　　2. _tmain需要一个返回值,而main默认为void.
　　3. _tmain的定义在<tchar.h>可以找到,如#define _tmain main，所以要加
　　#include <tchar.h>才能用。_tmain()是个宏,如果是UNICODE则他是wmain()否则他是main().
　　4. _tmain这个符号多见于VC++创建的控制台工程中，这个是为了保证移植unicode而加入的 （一般_t、_T、T()这些东西都是宏都和unicode有关系），对于使用非unicode字符集的工程来说，实际上和main没有差别（其实就算是使用unicode字符集也未必有多大的差别）。
　　5. 因此_tmain compile后仍为main，所以都可以执行.
　　main()是WINDOWS的控制台程序（32BIT）或DOS程序（16BIT）.
　　WinMain()是WINDOWS的GUI程序.
　　另外,wmain也是main的另一个别名,是为了支持二个字节的语言环境