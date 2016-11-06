---
date: 2016-01-30 01:09
status: public
title: 2016-01-30
---

# sudo 无法解析主机

在/etc/hosts 里追加如下：
127.0.1.1   hostname

参见<http://www.debian.org/doc/manuals/debian-reference/ch05.en.html#s-net-dns>

The IP address 127.0.1.1 in the second line of this example may not be found on some other Unix-like systems. TheDebian Installer creates this entry for a system without a permanent IP address as a workaround for some software (e.g., GNOME) as documented in thebug #719621.

# sudo /etc/sudoer.d问题
1. 开机选择RecoveryMode启动（ubuntu高级模式）；

2. 在Menu中选择root；

3. 执行 `chmod 0440 /etc/sudoers`;

4. 执行`mount -o remount rw /`;

5. 执行`chmod 0440 /etc/sudoers`;

6. 执行`sudo passwd`并修改密码;

7. 执行`chown -R root:root /etc/sudoers.d`;

8. 执行`chmod u=rwx,g=rx,o=rx /etc/sudoers.d/`;

9. 执行`chmod u=r,g=r,o= /etc/sudoers.d/*`;

Over.