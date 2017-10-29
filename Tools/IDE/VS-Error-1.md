---
date: 2016-02-29 18:10
status: public
title: VS-Error-1
---

# Problems
## **File Load**
 some bytes have been replaced with the unicode substitution character while loading file ...

 --------------------------- Microsoft Visual Studio---------------------------
 File Load Some bytes have been replaced with the Unicode substitution character while loading file BasicNetworkStream.cs with Chinese Simplified (GB2312) encoding. Saving the file will not preserve the original file contents.
 --------------------------- OK---------------------------

source code:	FlyCapture下的Examples。

* stackoverflow <http://stackoverflow.com/questions/4305923/file-load-some-bytes-have-been-replaced-with-the-unicode-substitution-character>


## TRACKER : 错误 TRK0005: 未能找到:“CL.exe”。
 我先是安装了VS2010Express版本，后又安装了Ultimate版本，然后卸载Express版本，遂出错。

 可能是两个版本共用一些文件，卸载导致缺失。

 记得VS2013,2015有修复卸载选项，但是好像根本发现不了有VS2010Ultimate这个程序安装（在系统自带的程序卸载列表和IObit Uninstaller里面都看不到）。

 所以干脆重装好了。

## 链接器工具错误LNK1123

<https://msdn.microsoft.com/zh-cn/library/7dz62kfh.aspx>

<https://msdn.microsoft.com/zh-cn/library/7dz62kfh(v=vs.100).aspx>

<http://stackoverflow.com/questions/10888391/error-link-fatal-error-lnk1123-failure-during-conversion-to-coff-file-inval>


## Command line error D8016
VS2010报错：
>1	error D8016: '/clr' and '/MT' command-line options are incompatible	D:\Program Files\Point Grey Research\FlyCapture2\src\FlyCapture2GUI\cl


source from: <https://social.msdn.microsoft.com/Forums/vstudio/en-US/94d5b8e2-2d2d-4680-ab8d-0a3b0562412a/how-to-solve-command-line-error-d8016?forum=vclanguage>
The answer:
1.
>the /clr option isn't compatible with the /MT (multi threaded). Check this page to see the restrictions when using the /clr:
http://msdn2.microsoft.com/en-us/library/ffkc918h.aspx

To solve the problem, go to your project properties->C/C++->Code Generation->Runtime Library->Multi-threaded Debug DLL (/MDd)
(or Multi-threaded DLL (/MD) for release configuration)

2.
>Try doing what the compiler says, adding the `#define _AFXDLL` or you can use the preprocessor options in project properties->c/c++->preprocessor->preprocessor definitions
add a ; if you need to separate from another definition and add `_AFXDLL`

As for your other question about the terms:
- CRT - C Run-Time libraries, the libraries that contain the information about the C standard functions

- DLL - Dynamic Linked Library is a library that instead of being "merged" with the executable like a static library (.lib files), it stays in a separate file that is loaded when needed by your application.

- AFXDLL - This is shared version of the MFC libraries.

This kind of terms/acronyms can be found easily in google/wikipedia and other resources if you don't want to wait for a response here ;)


**FIXED!**

BTW:
**/clr Restrictions** <https://msdn.microsoft.com/en-us/library/ffkc918h.aspx>

# error LNK2019
报错：
>错误	3	error LNK2019: 无法解析的外部符号 `_CrtDbgReportW`，该符号在函数 `"public: char const & __cdecl std::_String_const_iterator<char,struct std::char_traits<char>,class std::allocator<char> >::operator*(void)const " (??D?$_String_const_iterator@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QEBAAEBDXZ)` 中被引用	D:\Program Files\Point Grey Research\FlyCapture2\src\FlyCapture2GUI\FlyCapture2GUI.obj

>错误	1	error LNK2028: 无法解析的标记(0A00032C) "extern "C" `int __cdecl _CrtDbgReportW(int,wchar_t const *,int,wchar_t const *,wchar_t const *,...)" (?_CrtDbgReportW@@$$J0YAHHPEB_WH00ZZ)`，该标记在函数 "`public: void __cdecl std::_String_const_iterator<char,struct std::char_traits<char>,class std::allocator<char> >::_Compat(class std::_String_const_iterator<char,struct std::char_traits<char>,class std::allocator<char> > const &)const " (?_Compat@?$_String_const_iterator@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$$FQEBAXAEBV12@@Z)` 中被引用	D:\Program Files\Point Grey Research\FlyCapture2\src\FlyCapture2GUI\AssemblyResolver.obj


# error 4996
>'fopen': This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation, use `_CRT_SECURE_NO_WARNINGS. See online help for details.`

项目属性页，C/C++，预处理器，预处理器定义，编辑添加_CRT_SECURE_NO_WARNINGS

**FIXED~**


---
date: 2016-03-05 23:01
status: public
title: 2016-03-05
---

# 误删Microsoft Help Viewer 2.x` 解决办法
1. 在ios镜像安装文件中找到 packages\Help 路径。

2. 打开控制台命令，cd到这个路径下

3. 在控制台中键入以下命令进行安装

>msiexec /i help3_vs_net.msi vs_setup=1

亲测可以。

在这里看到的：<http://social.msdn.microsoft.com/Forums/zh-CN/0b96225c-d195-4f94-914e-a52bde2b39f4/vs-2012hlpviewer?forum=visualstudio2012zhcn>

有个网友给出下面的答案：
>解压到C盘根目录运行run this
>基本原理与linlin tong的答案相同
链接: <http://pan.baidu.com/s/1gdLbBGn> 密码: 6asf


---
date: 2016-04-04 22:20
status: public
title: 2016-04-04
---

# fatal error C1189: #error : ＂No Target Architecture＂ 解决办法一
最后调整了以下include文件的顺序，问题解决了。
<http://www.ithao123.cn/content-183865.html>