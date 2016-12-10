---
date: 2016-01-30 01:44
status: public
title: pysqlite2
---

# pysqlite2
只支持python2
核心命令：`python2 setup.py install`

# windows
环境：64bit-windows7-ultimate, Visual Studio 2015
## 问题一：找不到vcvarsall.bat
python2默认选择的编译器是指向VS08的。
解决要点：（ **注意VS15对应数字（大版本号？）14，VS13对应12，VS12对应11等等。**

1. CMD执行`SET VS90COMNTOOLS=%VS140COMNTOOLS%`

2. 系统环境变量VS90COMNTOOLS指向 %VS你的版本COMNTOOLS%

3. 在“..python安装路径...\Lib\distutils目录下有个msvc9compiler.py找到243行  
 `toolskey = "VS%0.f0COMNTOOLS" % version`   的version直接手动替换为14。

 以上任选其一。


##  问题二  找不到头文件
exit status，找不到头文件。
 暂时不理，且看下文。

# Linux
环境：Ubuntu15.10kylin

同样编译不通过。exit status，找不到头文件。还有这个鬼`src/connection.h:33:21`。

Google了解到pysqlite2是链接python和sqlite3的模块。
好像我没安装过sqlite3。不过这个名字在python的包目录里面很眼熟啊。
不管，反正ubuntu装东西方便快速。
`sudo apt-get install sqlite3`
这下总有那个破头文件了吧。
然而……
问题没有得到解决。
继续google。
StackOverFlow～
看到一个
>There is no such headers, maybe You forgot to install them :) linux installs them seperatly with libsqlite3-dev

于是乎：`sudo apt-get install libsqlite3-dev`

Done! Perfect! I love this!
所以，下次切回windows把sqlite3全家桶给装了？