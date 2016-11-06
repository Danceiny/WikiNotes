---
date: 2016-02-29 18:10
status: public
title: Errors
---

我前几天准备学习Qt，今天写第一个Hello World，结果也是同样的can not open，然后百度一番都是说路径不能有中文，无果。
其实用到：C:\Users\***\AppData\Local\Temp这个目录，很明显是系统的临时文件目录。我就想到肯定在创建Hello World项目时哪里的设置有问题，果不其然，去工具->选项->构建和运行，不要使用jom代替nmake。OK，成功运行。