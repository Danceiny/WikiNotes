## shell
`file /usr/bin/* |grep 'shell script'`学习shell编程最好的方法是参考阅读这些linux系统自带的脚本文件。

cp file22.{txt,backup} # 拷贝file22.txt到file22.backup中(大括号中不许有空白）

whatis, apropos, whereis, whereis -b, head/tail -n, less打开多个文件时:N跳到下一个文件, 

mkdir -p donotexistDIR/dir

mv -b afileordir bfileordir //备份

cp -p a/b/c d //拷贝整个目录结构

## 1. Linux 技巧：让进程在后台可靠运行的几种方法

https://www.ibm.com/developerworks/cn/linux/l-cn-nohup/index.html

### 2. 使用 shell 脚本对 Linux 系统和进程资源进行监控

1. https://www.ibm.com/developerworks/cn/linux/l-cn-shell-monitoring/index.html

## ; && || 操作符的区别

| cmd1 ; cmd2    | cmd1 和 cmd2 **都会** 被执行       |
| -------------- | ---------------------------- |
| cmd1 && cmd2   | 如果 cmd1 执行 **成功** 则执行 cmd2   |
| cmd1 \|\| cmd2 | 如果 cmd1 执行 **失败** 则执行 cmd22. |



- 使用netstat来看看系统的监听端口

```
# netstat -antp       //所有连接、数字显示主机、端口、TCP连接、监听的程序

# netstat -anup       //所有连接、数字显示主机、端口、UDP连接、监听的程序

# netstat -s          //统计所有（开机至今的）连接数据，包括tcp、udp等

# netstat -st         //统计所有tcp连接数据

# netstat -su         //统计所有udp连接数据

# netstat -su         //粗略统计连接数据
```
允许8098端口的tcp：
```
iptables -t filter -A INPUT -p tcp --dport 8098 -j ACCEPT
```
/备份amanda 

｀du -sh /home｀查询磁盘空间

lsof -i:PORT 列出端口占用 

1.查看主机名
hostname

2.修改主机名(重启后无效)
hostname hadoop

3.修改主机名(重启后永久生效)
vi /ect/sysconfig/network

4.修改IP(重启后无效)
ifconfig eth0 192.168.12.22

5.修改IP(重启后永久生效)
vi /etc/sysconfig/network-scripts/ifcfg-eth0

6.查看系统信息
uname -a
uname -r

7.查看ID命令
id -u
id -g

8.日期
date
date +%Y-%m-%d
date +%T
date +%Y-%m-%d" "%T

9.日历
cal 2012

10.查看文件信息
file filename

11.挂载硬盘
mount
umount
加载windows共享
mount -t cifs //192.168.1.100/tools /mnt

12.查看文件大小
du -h
du -ah

13.查看分区
df -h

14.ssh
ssh hadoop@192.168.1.1

15.关机
shutdown -h now /init 0
shutdown -r now /reboot



添加一个tom用户，设置它属于users组，并添加注释信息
分步完成：useradd tom

          usermod -g users tom
          usermod -c "hr tom" tom
一步完成：useradd -g users -c "hr tom" tom

设置tom用户的密码
passwd tom

修改tom用户的登陆名为tomcat
usermod -l tomcat tom

将tomcat添加到sys和root组中
usermod -G sys,root tomcat

查看tomcat的组信息
groups tomcat

添加一个jerry用户并设置密码
useradd jerry
passwd jerry

添加一个交america的组
groupadd america

将jerry添加到america组中
usermod -g america jerry

将tomcat用户从root组和sys组删除
gpasswd -d tomcat root
gpasswd -d tomcat sys

将america组名修改为am
groupmod -n am america



创建a.txt和b.txt文件，将他们设为其拥有者和所在组可写入，但其他以外的人则不可写入:
chmod ug+w,o-w a.txt b.txt

创建c.txt文件所有人都可以写和执行
chmod a=wx c.txt 或chmod 666 c.txt

将/itcast目录下的所有文件与子目录皆设为任何人可读取
chmod -R a+r /itcast

将/itcast目录下的所有文件与子目录的拥有者设为root，用户拥有组为users
chown -R root:users /itcast

将当前目录下的所有文件与子目录的用户皆设为itcast，组设为users
chown -R itcast:users *

1.查看文件夹属性

ls -ld test

2.文件夹的rwx

--x:可以cd进去

r-x:可以cd进去并ls

-wx:可以cd进去并touch，rm自己的文件，并且可以vi其他用户的文件

-wt:可以cd进去并touch，rm自己的文件

ls -ld /tmp

drwxrwxrwt的权限值是1777(sticky)



1.新建一个文件

touch a.txt

\> b.txt

2.错误重定向:2>

find /etc -name zhaoxing.txt 2> error.txt

3.将正确或错误的信息都输入到log.txt中

find /etc -name passwd > /tmp/log.txt 2>&1 

find /etc -name passwd &> /tmp/log.txt

4.追加>>

5.将小写转为大写（输入重定向）

tr "a-z" "A-Z" < /etc/passwd

6.自动创建文件

cat > log.txt << EXIT

\> ccc

\> ddd

\> EXI

7.查看/etc下的文件有多少个？

ls -l /etc/ | grep '^d' | wc -l

8.查看/etc下的文件有多少个，并将文件详情输入到result.txt中

ls -l /etc/ | grep '^d' | tee result.txt | wc -l



1.查看用户最近登录情况

last

lastlog

2.查看硬盘使用情况

df

3.查看文件大小

du

4.查看内存使用情况

free

5.查看文件系统

/proc

6.查看日志

ls /var/log/

7.查看系统报错日志

tail /var/log/messages

8.查看进程

top

9.结束进程

kill 1234

kill -9 4333



1.cut截取以:分割保留第七段

grep hadoop /etc/passwd | cut -d: -f7

2.排序

du | sort -n 

3.查询不包含hadoop的

grep -v hadoop /etc/passwd

4.正则表达包含hadoop

grep 'hadoop' /etc/passwd

5.正则表达(点代表任意一个字符)

grep 'h.*p' /etc/passwd

6.正则表达以hadoop开头

grep '^hadoop' /etc/passwd

7.正则表达以hadoop结尾

grep 'hadoop$' /etc/passwd

规则：

.  : 任意一个字符

a* : 任意多个a(零个或多个a)

a? : 零个或一个a

a+ : 一个或多个a

.* : 任意多个任意字符

\. : 转义.

\<h.*p\> ：以h开头，p结尾的一个单词

o\{2\} : o重复两次

grep '^i.\{18\}n$' /usr/share/dict/words

查找不是以#开头的行

grep -v '^#' a.txt | grep -v '^$' 

以h或r开头的

grep '^[hr]' /etc/passwd

不是以h和r开头的

grep '^[^hr]' /etc/passwd

不是以h到r开头的

grep '^[^h-r]' /etc/passwd





1.gzip压缩

gzip a.txt

2.解压

gunzip a.txt.gz

gzip -d a.txt.gz

3.bzip2压缩

bzip2 a

4.解压

bunzip2 a.bz2

bzip2 -d a.bz2

5.将当前目录的文件打包

tar -cvf bak.tar .

将/etc/password追加文件到bak.tar中(r)

tar -rvf bak.tar /etc/password

6.解压

tar -xvf bak.tar

7.打包并压缩gzip

tar -zcvf a.tar.gz

8.解压缩

tar -zxvf a.tar.gz

解压到/usr/下

tar -zxvf a.tar.gz -C /usr

9.查看压缩包内容

tar -ztvf a.tar.gz

zip/unzip

10.打包并压缩成bz2

tar -jcvf a.tar.bz2

11.解压bz2

tar -jxvf a.tar.bz2



1.查找可执行的命令：

which ls

2.查找可执行的命令和帮助的位置：

whereis ls

3.查找文件(需要更新库:updatedb)

locate hadoop.txt

4.从某个文件夹开始查找

find / -name "hadooop*"

find / -name "hadooop*" -ls

5.查找并删除

find / -name "hadooop*" -ok rm {} \;

find / -name "hadooop*" -exec rm {} \;

6.查找用户为hadoop的文件

find /usr -user hadoop -ls

7.查找用户为hadoop并且(-a)拥有组为root的文件

find /usr -user hadoop -a -group root -ls

8.查找用户为hadoop或者(-o)拥有组为root并且是文件夹类型的文件

find /usr -user hadoop -o -group root -a -type d

9.查找权限为777的文件

find / -perm -777 -type d -ls

10.显示命令历史

history

11.grep

grep hadoop /etc/password


文件名-> inode -> 文件的二进制流
硬链接,两个文件名指向同一个inode.为了保证所有的硬链接移除后才彻底删除文件,每个inode使用一个link count. 
因为inode编号在自己文件系统之外无意义,故不能跨文件系统建立硬链接.符号链接解决该问题,但开销更大.

名称 : date
使用权限 : 所有使用者
使用方式 :
date [-u] [-d datestr] [-s datestr] [--utc] [--universal]
[--date=datestr] [--set=datestr] [--help] [--version] [+FORMAT] [MMDDhhmm[[CC]YY][.ss]]
说明 :
date 可以用来显示或设定系统的日期与时间，在显示方面，使用者可以设定欲显示的格式，格式设定为一个加号后接数个标记，其中可用的标记列表如下
:
时间方面 :
% : 印出 %
%n : 下一行
%t : 跳格
%H : 小时(00..23)
%I : 小时(01..12)
%k : 小时(0..23)
%l : 小时(1..12)
%M : 分钟(00..59)
%p : 显示本地 AM 或 PM
%r : 直接显示时间 (12 小时制，格式为 hh:mm:ss [AP]M)
%s : 从 1970 年 1 月 1 日 00:00:00 UTC 到目前为止的秒数
%S : 秒(00..61)
%T : 直接显示时间 (24 小时制)
%X : 相当于 %H:%M:%S
%Z : 显示时区
日期方面 :
%a : 星期几 (Sun..Sat)
%A : 星期几 (Sunday..Saturday)
%b : 月份 (Jan..Dec)
%B : 月份 (January..December)
%c : 直接显示日期与时间
%d : 日 (01..31)
%D : 直接显示日期 (mm/dd/yy)
%h : 同 %b
%j : 一年中的第几天 (001..366)
%m : 月份 (01..12)
%U : 一年中的第几周 (00..53) (以 Sunday 为一周的第一天的情形)
%w : 一周中的第几天 (0..6)
%W : 一年中的第几周 (00..53) (以 Monday 为一周的第一天的情形)
%x : 直接显示日期 (mm/dd/yy)
%y : 年份的最后两位数字 (00.99)
%Y : 完整年份 (0000..9999)
若是不以加号作为开头，则表示要设定时间，而时间格式为 MMDDhhmm[[CC]YY][.ss]，其中 MM
为月份，DD 为日，hh 为小时，mm 为分钟，CC 为年份前两位数字，YY 为年份后两位数字，ss 为秒数
参数 :
-d datestr : 显示 datestr 中所设定的时间 (非系统时间)
--help : 显示辅助讯息
-s datestr : 将系统时间设为 datestr 中所设定的时间
-u : 显示目前的格林威治时间
--version : 显示版本编号
例子 :
显示时间后跳行，再显示目前日期 :
date +%T%n%D
显示月份与日数 :
date +%B %d
显示日期与设定时间(12:34:56) :
date --date 12:34:56
注意 :
当你不希望出现无意义的 0 时(比如说 1999/03/07)，则可以在标记中插入 - 符号，比如说 date
+%-H:%-M:%-S 会把时分秒中无意义的 0 给去掉，像是原本的 08:09:04 会变为 8:9:4。另外，只有取得权限者(比如说
root)才能设定系统时间。
当你以 root 身分更改了系统时间之后，请记得以 clock -w 来将系统时间写入 CMOS 中，这样下次重新开机时系统时间才会持续抱持最新的正确值。
