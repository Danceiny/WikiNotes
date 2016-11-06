---
date: 2016-08-29 08:50
status: public
title: Linux初步-1
---

# OS Loader:
grub: /boot/grub/
menu.lst -> ./grub.conf 内容一样

![](~/08-52-10.jpg)
# vmlinuz
/boot/
该目录下还有一个config文件，记录了各种详细信息，自动生成不可编辑。
最高票答案：Linux初始化init系统，RHEL 5为SysVinit，RHEL 6为Upstart，RHEL 7为Systemd，为更好地解释这个问题，以下选取RHEL 5.x进行说明。
(https://www.zhihu.com/question/20126189)
作者：echo1937
链接：https://www.zhihu.com/question/20126189/answer/18255079
来源：知乎
著作权归作者所有，转载请联系作者获得授权。

如流程图所示,RHEL的启动过程按步骤大致可以分为:
POST加电自检-->BIOS(Boot Sequence)-->加载对应引导上的MBR(bootloader) -->主引导设置加载其BootLoader-->Kernel初始化-->initrd—>/etc/init进程加载/etc/inittab

从加载BootLoader(RHEL下为Grub)开始,会发生以下事情.
1. 藉由Grub的管理,读取Kernel内核文件(/boot/vmlinuz),解压至主内存,利用内核的功能,进行第二次硬件检测(第一次发生在POST开机自检时)
2. 加载Initial RAM Disk(/boot/initrd),使之在内存中解压缩为根目录,kernel藉此完成驱动程序的加载,，最终释放虚拟文件系统，并挂载实际的根目录文件系统.
3. 在核心加载完毕,进行完硬件侦测与驱动程序加载后,内核会启动第一个进程/sbin/init, init进程将会读取/etc/inittab,在/etc/inittab中,大致规定了以下动作:
取得 runlevel 亦即默认运行等级的相关等级.
使用 /etc/rc.d/rc.sysinit 进行系统初始化
根据runlevel调用相关启动脚本,以启动相应的系统服务
其他一些系统设置,最终完成系统启动.

/sbin/init 最主要的功能就是准备软件运行的环境，包括系统的主机名称、网络配置、语系处理、文件系统格式及其他服务的启动等,而所有的动作都根据在/etc/inittab中的配置.

/etc/rc.d, 里面存放了rc.local, rc.sysinit, init.d, rcX.d (X包括0-6对应相对runlevel)

/etc/rc.d/init.d, 存放了各种系统服务的启动关闭的管理脚本,/etc/init.d是他的一个目录链接.

/etc/rc.d/rcX.d, 里面存放了一些脚本,脚本名字以K开头的,表示STOP动作,名字以S开头,表示Start动作,名称中的数字表示执行次序,数字越小表示越先执行.
/etc/rcX.d是他的目录链接,但事实上rcX.d下文件本身就是指向/etc/rc.d/init.d下脚本的链接文件.

/etc/rc.d/rc.local, 有一个链接文件,路径在/etc/rc.local,
系统根据runlevel启动完rcX.d中的脚本之后,会调用rc.local脚本,如果你有一个脚本命令不论在3和5都想开机启动,那么就添加与此,免去rc3.d和rc5.d分别增加启动脚本工作量.
# init
/etc/init/里面的文件好像没说个啥几把。
/etc/init.d/
/etc/rc.d/rc.sysinit: run once at boot time
[linux_surcsyinit](http://www.comptechdoc.org/os/linux/startupman/linux_surcsysinit.html)
![](~/09-11-04.jpg)

![](~/09-16-12.jpg)
# systemD
[IBM刘明-浅析 Linux 初始化 init 系统，第 3 部分: Systemd](http://www.ibm.com/developerworks/cn/linux/1407_liuming_init3/index.html)

# runlevel
```
$runlevel 
$who -r
```

![](~/09-33-17.jpg)
