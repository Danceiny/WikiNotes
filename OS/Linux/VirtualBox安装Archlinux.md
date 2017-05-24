http://www.jianshu.com/p/b66d14dcaffe


虚拟机名称和类型
名称：随意
类型：Linux
版本：Arch Linux（64-bit）
内存大小：1024M
虚拟硬盘
大小：8G
格式：VDI
固定大小
下载Archlinux
版本：archlinux-2016.01.01-dual
将下载好的文件分配到虚拟机的光驱
安装

参考：官方wiki
命令lsblk查看设备情况，得到硬盘为：sda
创建分区表
打开设备：parted /dev/sda
创建MBR/msdos 分区表：(parted) mklabel msdos
创建Boot分区：
(parted) mkpart primary ext4 1M 100M
(parted) set 1 boot on
创建swap分区：(parted) mkpart primary linux-swap 100M 2048M
创建/分区：(parted) mkpart primary ext4 2048M 100%
格式化分区：
boot分区：mkfs.ext4 /dev/sda1
/分区：mkfs.ext4 /dev/sda3
swap分区：mkswap /dev/sda2
启用swap：
swapon /dev/sda2
挂载分区：
/分区：mount /dev/sda3 /mnt
boot分区：
mkdir /mnt/boot
mount /dev/sda1 /mnt/boot
增加国内镜像源
nano /etc/pacman.d/mirrorlist
Server = http://mirrors.ustc.edu.cn/archlinux/$repo/os/$arch
Server = http://mirrors.163.com/archlinux/$repo/os/$arch
Server = http://mirrors.sohu.com/archlinux/$repo/os/$arch

刷新：pacman -Syy
安装基本系统

安装基本系统
pacstrap -i /mnt base #base-devel 可不装
fstab
genfstab -U -p /mnt > /mnt/etc/fstab
使用命令cat /mnt/etc/fstab检查内容是否正确
切换目录chroot
arch-chroot /mnt /bin/bash
Locale
nano /etc/locale.gen
增加内容：
en_US.UTF-8 UTF-8
zh_CN.UTF-8 UTF-8
使最后用命令locale-gen生成。
创建locale.conf文件：
echo LANG=en_US.UTF-8 > /etc/locale.conf
nano搜索文本：按Ctrl+W，回车确定。用Alt+W定位到下一个匹配。
时间设置
选择时区：tzselect
设置关联文件：ln -s /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
设置root密码：passwd root
设置主机名：echo "Archlinux" > /etc/hostname
安装bootloader：
pacman -S grub #os-prober 识别硬盘上其他系统的工具
grub-install --recheck /dev/sda
生成配置文件：
grub-mkconfig -o /boot/grub/grub.cfg
配置网络
查看网卡名：ip link show
设置启动dhcp：systemctl enable dhcpcd@enp0s3.service
卸载分区并重启系统
推出chroot：exit
卸载分区：
umount -R /mnt/boot
umount -R /mnt
重启系统：reboot
配置

新增用户
新增用户及设置密码：
useradd -m mu
passwd mu
安装sudo
pacman -S sudo
增添sudo用户
nano /etc/sudoers
添加：mu ALL=(ALL) ALL
安装字体
中文字体：pacman -S wqy-zenhei wqy-microhei
等宽字体：
pacman -S ttf-dejavu
pacman -S adobe-source-code-pro-fonts
openssh安装
安装：pacman -S openssh
允许root登陆：
nano /etc/ssh/sshd_config
将PermitRootLogin改为yes，没有就添加
启动服务：systemctl start sshd
设置开机启动：systemctl enable sshd
解压软件：
zip：pacman -S p7zip
rar：pacman -S unrar
图形界面：pacman -S file-roller
Yaourt
添加源：nano /etc/pacman.conf
 [archlinuxfr]
 SigLevel = Never
 Server = http://repo-fr.archlinuxcn.org/$arch
安装：pacman -Sy yaourt
networkmanager
安装：pacman -S networkmanager
图形：pacman -S network-manager-applet xfce4-notifyd
PPTP：networkmanager-pptp
运行：systemctl start NetworkManager
开启启动：systemctl enable NetworkManager
图形界面的安装

安装显卡驱动：
pacman -S xf86-video-vesa
安装Xorg：
pacman -S xorg-server xorg-xinit xorg-utils xorg-server-utils
安装xfce4
安装：pacman -S xfce4 xfce4-goodies
启动参数设置：
复制配置文件到home目录： cp /etc/X11/xinit/xinitrc ~/.xinitrc
nano .xinitrc添加：exec startxfce4
启动桌面：startx
中文设置
nano .xinitrc
在exec前添加内容：
export LANG=zh_CN.UTF-8
export LANGUAGE=zh_CN:en_US
export LC_CTYPE=en_US.UTF-8
安装sogou拼音

安装fcitx：
pacman -S fcitx-im
fcitx图形界面配置：
pacman -S fcitx-configtool
下载sogou：
curl -L-O https://aur.archlinux.org/cgit/aur.git/snapshot/fcitx-sogoupinyin.tar.gz
解压包：
tar -xvzf fcitx-sogoupinyin.tar.gz
生成并安装软件包：
makepkg -sri
-s /--syncdeps 表示自动执行安装依赖关系。
-r /--rmdeps 会在编译后删除不需要的编译时依赖。
-i /--install 会安装软件包。
~/.xprofile 文件添加：
export GTK_IM_MODULE=fcitx
export QT_IM_MODULE=fcitx
export XMODIFIERS="@im=fcitx"
安装wps-office

先安装aur里面的libpng12
再安装aur里面的wps-office
问题

装好后没有网络
ip link show 看下网卡叫什么名字，然后用下面的命令
ip link set eth0 up
dhcpcd eth0
关闭tty1
ps aux | grep tty1 | grep bash
kill -9 tty1_pid
........
http://user.qzone.qq.com/1010841065/blog/1428768109