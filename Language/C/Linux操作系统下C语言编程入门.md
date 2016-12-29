---
date: 2016-06-08 20:46
status: public
title: Linux操作系统下C语言编程入门
---

# 笔记目录
## [基础知识-gcc\makefile](http://danceiny.farbox.com/post/c/linuxcao-zuo-xi-tong-xia-cyu-yan-bian-cheng-ru-men)
## [时间概念-时间测量](http://danceiny.farbox.com/post/c/linux_c-shi-jian-gai-nian)
## [消息管理-signal](http://danceiny.farbox.com/post/c/linux_c-xiao-xi-guan-li)
## [网络编程-Socket\IP\TCP\UDP](http://danceiny.farbox.com/post/c/linux_c-wang-luo-bian-cheng)
## [线程操作-文件夹备份](http://danceiny.farbox.com/post/c/linux_c-xian-cheng-cao-zuo)
## [开发工具-gcc/gdb](http://danceiny.farbox.com/post/c/linuxxia-ckai-fa-gong-ju)
# 基础知识
## 源程序的编译
`gcc -o hello hello.c`，生成一个hello的可执行文件，执行`./hello`就可以看到程序的运行结果了。-o选项表示我们要求编译器给我们输出的可执行文件名为hello而hello.c是我们的源程序文件。

gcc的-c选项表示只要求编译器输出目标代码而不必输出可执行文件。
-g选项表示要求编译器编译时提供可对程序进行调试的信息。

## Makefile的编写
程序源代码如下：
```
/* main.c */
#include "mytool1.h"
#include "mytool2.h"
int main(int argc,char **argv)
{
mytool1_print("hello");
mytool2_print("hello");
}
/* mytool1.h */
#ifndef _MYTOOL_1_H
#define _MYTOOL_1_H
void mytool1_print(char *print_str);
#endif

/* mytool1.c */
#include "mytool1.h"
void mytool1_print(char *print_str)
{
printf("This is mytool1 print %s\n",print_str);
}

/* mytool2.h */
#ifndef _MYTOOL_2_H
#define _MYTOOL_2_H
void mytool2_print(char *print_str);
#endif

/* mytool2.c */
#include "mytool2.h"
void mytool2_print(char *print_str)
{
printf("This is mytool2 print %s\n",print_str);
}
```
可以这样编译：
```
# 这是上面那个程序的 Makefile 文件
gcc -c main.c
gcc -c mytool1.c
gcc -c mytool2.c
gcc -o main main.o mytool1.o mytool2.o
```
不过用Makefile显然更符合工程学思想。
```
main：main.o mytool1.o mytool2.o
gcc -o main main.o mytool1.o mytool2.o
main.o：main.c mytool1.h mytool2.h
gcc -c main.c
mytool1.o：mytool1.c mytool1.h
gcc -c mytool1.c
mytool2.o：mytool2.c mytool2.h
gcc -c mytool2.c
```

### Makefile编写方法
最重要的是描述文件的依赖关系：
```
target: components
TAB rule
```
第一行表示依赖关系，第二行是规则（TAB表示TAB键）。
如上所示makefile文件的第二行，`main: main.o mytool1.o mytool2.o`表示目标(target)main的依赖对象(components)是main.o mytool1.o mytool2.o。

有用的变量：
1. $@
目标文件
2. $^
所有的依赖文件
3. $<
第一个依赖文件

```
# 这是简化后的 Makefile
main：main.o mytool1.o mytool2.o
gcc -o $@ $^
main.o：main.c mytool1.h mytool2.h
gcc -c $<
mytool1.o：mytool1.c mytool1.h
gcc -c $<
mytool2.o：mytool2.c mytool2.h
gcc -c $<
```


### 缺省规则
```
..c.o:
gcc -c $<
```
这个规则表示所有.o文件都是依赖于相应的.c文件的。
```
# 这是再一次简化后的 Makefile
main：main.o mytool1.o mytool2.o
gcc -o $@ $^
..c.o：
gcc -c $<
```

## 程序库的链接
用`gcc -o temp temp.c`编译如下程序时：
```
/* temp.c */
#include <math.h>;
int main(int argc,char **argv)
{
double value;
printf("Value：%f\n",value);
}
```
会出错:
```
/tmp/cc33Kydu.o： In function `main'：
/tmp/cc33Kydu.o(.text+0xe)： undefined reference to `log'
collect2： ld returned 1 exit status
```

虽然包含了正确的头文件，但是编译时仍然需要链接确定的库。为此要加上`-lm`选项。
有时候还要指定库的路径，此时需要用编译器的`-L`选项指定路径。例如：`-L/home/hoyt/mylib`。
系统缺省库路径/lib /usr/lib /usr/local/lib这三个路径下面的库可以不指定路径。

还有一个问题,有时候我们使用了某个函数,但是我们不知道库的名字,这个时候怎么办呢?很抱歉,对于这个问题我也不知道答案,我只有一个傻办法.首先,我到标准库路径下面去找
看看有没有和我用的函数相关的库,我就这样找到了线程(thread)函数的库文件(libpthread.a).
当然,如果找不到,只有一个笨方法.比如我要找 sin 这个函数所在的库. 就只好用 nm -o/lib/*.so|grep sin>;~/sin 命令,然后看~/sin 文件,到那里面去找了. 在 sin 文件当中,我会找到
这样的一行 libm-2.1.2.so：00009fa0 W sin 这样我就知道了 sin 在 libm-2.1.2.so 库里面,我用 -lm
选项就可以了(去掉前面的 lib 和后面的版本标志,就剩下 m 了所以是 -lm). 


## 程序的调试
最常用的调试软件gdb，图形化的xxgdb，编译时须加入-g选项。
## 头文件和系统
# 进程介绍
系统还对进程区分了不同的状态.将进程分为新建,运行,阻塞,就绪和完成五个状态. 新建表示进程正在被创建,运行是进程正在运行,阻塞是进程正在等待某一个事件发生,就绪是表示系统正在等待 CPU 来执行命令,而完成表示进程已经结束了系统正在回收资源. 
## 进程的标志
系统调用 getpid 可以得到进程的 ID,而 getppid 可以得到父进程(创建调用该函数进程的进程)的 ID.
```
#include <unistd>;
pid_t getpid(void);
pid_t getppid(void);
```
系统为了找到进程的用户名,还为进程和用户建立联系.这个用户称为进程的所有者.相应的每一个用户也有一个用户 ID.通过系统调用 getuid 可以得到进程的所有者的 ID.由于进程要用到一些资源,而 Linux 对系统资源是进行保护的,为了获取一定资源进程还有一个有效用户 ID.这个 ID 和系统的资源使用有关,涉及到进程的权限. 通过系统调用 geteuid 我们可以得到进程的有效用户 ID. 和用户 ID相对应进程还有一个组 ID 和有效组 ID 系统调用 getgid 和 getegid 可以分别得到组 ID 和有效组 ID.
```
#include <unistd>;
#include <sys/types.h>;
uid_t getuid(void);
uid_t geteuid(void);
gid_t getgid(void);
git_t getegid(void);
```
登录名等其他信息:getpwuid().
实例程序：
```
#include <unistd.h>;
#include <pwd.h>;
#include <sys/types.h>;
#include <stdio.h>;
int main(int argc,char **argv)
{
pid_t my_pid,parent_pid;
uid_t my_uid,my_euid;
gid_t my_gid,my_egid;
struct passwd *my_info;
my_pid=getpid();
parent_pid=getppid();
my_uid=getuid();
my_euid=geteuid();
my_gid=getgid();
my_egid=getegid();
my_info=getpwuid(my_uid);
printf("Process ID：%ld\n",my_pid);
printf("Parent ID：%ld\n",parent_pid);
printf("User ID：%ld\n",my_uid);
printf("Effective User ID：%ld\n",my_euid);
printf("Group ID：%ld\n",my_gid);
printf("Effective Group ID：%ld\n",my_egid)：
if(my_info)
{
printf("My Login Name：%s\n" ,my_info->;pw_name);
printf("My Password ：%s\n" ,my_info->;pw_passwd);
printf("My User ID ：%ld\n",my_info->;pw_uid);
printf("My Group ID ：%ld\n",my_info->;pw_gid);
printf("My Real Name：%s\n" ,my_info->;pw_gecos);
printf("My Home Dir ：%s\n", my_info->;pw_dir);
printf("My Work Shell：%s\n", my_info->;pw_shell);
}
}
```
## 进程的创建
```
#include<unisted.h>
pid_t fork();
```
当一个进程调用了 fork 以后,系统会创建一个子进程.这个子进程和父进程不同的地方只
有他的进程 ID 和父进程 ID,其他的都是一样.就象符进程克隆(clone)自己一样.当然创建两个一模一样的进程是没有意义的.**为了区分父进程和子进程,我们必须跟踪 fork 的返回值. **

父进程创建子进程后，子进程一般要执行不同的程序，用到exec族：
```
#include <unistd.h>;
int execl(const char *path,const char *arg,...);
int execlp(const char *file,const char *arg,...);
int execle(const char *path,const char *arg,...);
int execv(const char *path,char *const argv[]);
int execvp(const char *file,char *const argv[])：
```

实例：
```
#include <unistd.h>;
#include <sys/types.h>;
#include <sys/wait.h>;
#include <stdio.h>;
#include <errno.h>;
#include <math.h>;
void main(void)
{
pid_t child;
int status;
printf("This will demostrate how to get child status\n");
if((child=fork())==-1)
{
printf("Fork Error ：%s\n",strerror(errno));
exit(1);
}
else if(child==0)
{
int i;
printf("I am the child：%ld\n",getpid());
for(i=0;i<1000000;i++) sin(i);
i=5;
printf("I exit with %d\n",i);
exit(i);
}
while(((child=wait(&status))==-1)&(errno==EINTR));
if(child==-1)
printf("Wait Error：%s\n",strerror(errno));
else if(!status)
printf("Child %ld terminated normally return status is zero\n",
child);
else if(WIFEXITED(status))
printf("Child %ld terminated normally return status is %d\n",
child,WEXITSTATUS(status));
else if(WIFSIGNALED(status))
printf("Child %ld terminated due to signal %d znot caught\n",
child,WTERMSIG(status));
}
```
strerror函数返回一个指定的错误号的错误信息（字符串）。

##　守护进程的创建
后台进程的创建思想：首先父进程创建一个子进程，然后子进程杀死父进程。信号处理的所有工作由子进程处理。
```
#include <unistd.h>;
#include <sys/types.h>;
#include <sys/stat.h>;
#include <stdio.h>;
#include <errno.h>;
#include <fcntl.h>;
#include <signal.h>;
/* Linux 的默任个人的邮箱地址是 /var/spool/mail/用户的登录名 */
#define MAIL "/var/spool/mail/hoyt"
/* 睡眠 10 秒钟 */
#define SLEEP_TIME 10
main(void)
{
pid_t child;
if((child=fork())==-1)
{
printf("Fork Error：%s\n",strerror(errno));
exit(1);
}
else if(child>;0)
while(1);
if(kill(getppid(),SIGTERM)==-1)
{
printf("Kill Parent Error：%s\n",strerror(errno));
exit(1);
}
{
int mailfd;
while(1)
{
if((mailfd=open(MAIL,O_RDONLY))!=-1)
{
fprintf(stderr,"%s","\007");
close(mailfd);
}
sleep(SLEEP_TIME);
}
}
}
```

# 文件操作
## 文件的创建和读写
```
#include <fcntl.h>;
#include <unistd.h>;
#include <sys/types.h>;
#include <sys/stat.h>;
int open(const char *pathname,int flags);
int open(const char *pathname,int flags,mode_t mode);
int close(int fd);
```
flags可以是下面的组合（前三个互斥）：
O_RDONLY：以只读的方式打开文件.
O_WRONLY：以只写的方式打开文件.
O_RDWR：以读写的方式打开文件.
O_APPEND：以追加的方式打开文件.
O_CREAT：创建一个文件.
O_EXEC：如果使用了 O_CREAT 而且文件已经存在,就会发生一个错误.
O_NOBLOCK：以非阻塞的方式打开一个文件.
O_TRUNC：如果文件已经存在,则删除文件的内容.

如果用了O_CREATE标志，则需指定mode标志，用来表示文件的访问权限，mode可以是如下：
-----------------------------------------------------------------
S_IRUSR 用户可以读 S_IWUSR 用户可以写
S_IXUSR 用户可以执行 S_IRWXU 用户可以读写执行
-----------------------------------------------------------------
S_IRGRP 组可以读 S_IWGRP 组可以写
S_IXGRP 组可以执行 S_IRWXG 组可以读写执行
-----------------------------------------------------------------
S_IROTH 其他人可以读 S_IWOTH 其他人可以写
S_IXOTH 其他人可以执行 S_IRWXO 其他人可以读写执行
-----------------------------------------------------------------
S_ISUID 设置用户执行 ID S_ISGID 设置组的执行 ID

也可以用5位数字代表各个位的标志。00000，第一位表示设置用户ID，第二位表示设置组ID，第三位表示用户自己的权限位，第四位表示组的权限，第五位表示其他人的权限。
每个数字可以取1（执行权限），2（写权限），4（读权限），0（什么也没有）或者这几个值的和。

拷贝文件的实例：
```
#include <unistd.h>;
#include <fcntl.h>;
#include <stdio.h>;
#include <sys/types.h>;
#include <sys/stat.h>;
#include <errno.h>;
#include <string.h>;
#define BUFFER_SIZE 1024
int main(int argc,char **argv)
{
int from_fd,to_fd;
int bytes_read,bytes_write;
char buffer[BUFFER_SIZE];
char *ptr;
if(argc!=3)
{
fprintf(stderr,"Usage：%s fromfile tofile\n\a",argv[0]);
exit(1);
}
/* 打开源文件 */
if((from_fd=open(argv[1],O_RDONLY))==-1)
{
fprintf(stderr,"Open %s Error：%s\n",argv[1],strerror(errno));
exit(1);
}
/* 创建目的文件 */
if((to_fd=open(argv[2],O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))==-1)
{
fprintf(stderr,"Open %s Error：%s\n",argv[2],strerror(errno));
exit(1);
}
/* 以下代码是一个经典的拷贝文件的代码 */
while(bytes_read=read(from_fd,buffer,BUFFER_SIZE))
{
/* 一个致命的错误发生了 */
if((bytes_read==-1)&&(errno!=EINTR)) break;
else if(bytes_read>;0)
{
ptr=buffer;
while(bytes_write=write(to_fd,ptr,bytes_read))
{
/* 一个致命错误发生了 */
if((bytes_write==-1)&&(errno!=EINTR))break;
/* 写完了所有读的字节 */
else if(bytes_write==bytes_read) break;
/* 只写了一部分,继续写 */
else if(bytes_write>;0)
{
ptr+=bytes_write;
bytes_read-=bytes_write;
}
}
/* 写的时候发生的致命错误 */
if(bytes_write==-1)break;
}
}
close(from_fd);
close(to_fd);
exit(0);
}
```

## 文件的各个属性
判断文件是否可以进行某种操作，使用access函数。
```
#include <unistd.h>;
int access(const char *pathname,int mode);
```
mode可以是：
R_OK 文件可以读,W_OK 文件可以写,X_OK 文件可以执行,F_OK 文件存在.
有权限时函数返回0，有一个不符合即返回-1.
stat用来判断没有打开的文件，fstat用来判断打开的文件。
```
#include <sys/stat.h>;
#include <unistd.h>;
int stat(const char *file_name,struct stat *buf);
int fstat(int filedes,struct stat *buf);
struct stat {
dev_t st_dev; /* 设备 */
ino_t st_ino; /* 节点 */
mode_t st_mode; /* 模式 */
nlink_t st_nlink; /* 硬连接 */
uid_t st_uid; /* 用户 ID */
gid_t st_gid; /* 组 ID */
dev_t st_rdev; /* 设备类型 */
off_t st_off; /* 文件字节数 */
unsigned long st_blksize; /* 块大小 */
unsigned long st_blocks; /* 块数 */
time_t st_atime; /* 最后一次访问时间 */
time_t st_mtime; /* 最后一次修改时间 */
time_t st_ctime; /* 最后一次改变时间(指属性) */
};
```
**宏判断**
S_ISLNK(st_mode)：是否是一个连接.S_ISREG 是否是一个常规文件.S_ISDIR 是否是一个目
录 S_ISCHR 是否是一个字符设备.S_ISBLK 是否是一个块设备 S_ISFIFO 是否 是一个 FIFO
文件.S_ISSOCK 是否是一个 SOCKET 文件. 

## 目录文件的操作
getcwd()得到当前工作路径。
```
#include <dirent.h>;
#include <unistd.h>;
#include <fcntl.h>;
#include <sys/types.h>;
#include <sys/stat.h>;
int mkdir(const char *path,mode_t mode);
DIR *opendir(const char *path);
struct dirent *readdir(DIR *dir);
void rewinddir(DIR *dir);
off_t telldir(DIR *dir);
void seekdir(DIR *dir,off_t off);
int closedir(DIR *dir);
struct dirent {
long d_ino;
off_t d_off;
unsigned short d_reclen;
char d_name[NAME_MAX+1]; /* 文件名称 */
```
下面这个小程序，有一个参数，为文件名则输出这个文件的大小和最后修改的时间，如果是一个目录则输出这个目录下所有文件的大小和修改时间。
```C:n
#include <unistd.h>;
#include <stdio.h>;
#include <errno.h>;
#include <sys/types.h>;
#include <sys/stat.h>;
#include <dirent.h>;
#include <time.h>;
static int get_file_size_time(const char *filename)
{
struct stat statbuf;
if(stat(filename,&statbuf)==-1)
{
printf("Get stat on %s Error：%s\n",
filename,strerror(errno));
return(-1);
}
if(S_ISDIR(statbuf.st_mode))return(1);
if(S_ISREG(statbuf.st_mode))
printf("%s size：%ld bytes\tmodified at %s",
filename,statbuf.st_size,ctime(&statbuf.st_mtime));
return(0);
}
int main(int argc,char **argv)
{
DIR *dirp;
struct dirent *direntp;
int stats;
if(argc!=2)
{
printf("Usage：%s filename\n\a",argv[0]);
exit(1);
}
if(((stats=get_file_size_time(argv[1]))==0)||(stats==-1))exit(1);
if((dirp=opendir(argv[1]))==NULL)
{
printf("Open Directory %s Error：%s\n",
argv[1],strerror(errno));
exit(1);
}
while((direntp=readdir(dirp))!=NULL)
if(get_file_size_time(direntp-<d_name)==-1)break;
closedir(dirp);
exit(1);
}
```
## 管道文件
过滤和重定向程序（操作符）：more，cat，<>;|<<等等，都用到了管道这种特殊的文件。系统调用pipe可以创建一个管道。

`int pipe(int fildes[2]);`调用成功后可以访问文件描述符fildes[0],fildes[1],其中前者是用来读的文件描述符，后者是用来写的。
```C:n
#include <stdio.h>;
#include <stdlib.h>;
#include <unistd.h>;
#include <string.h>;
#include <errno.h>;
#include <sys/types.h>;
#include <sys/wait.h>;
#define BUFFER 255
int main(int argc,char **argv)
{
char buffer[BUFFER+1];
int fd[2];
if(argc!=2)
{
fprintf(stderr,"Usage：%s string\n\a",argv[0]);
exit(1);
}
if(pipe(fd)!=0)
{
fprintf(stderr,"Pipe Error：%s\n\a",strerror(errno));
exit(1);
}
if(fork()==0)
{
close(fd[0]);
printf("Child[%d] Write to pipe\n\a",getpid());
snprintf(buffer,BUFFER,"%s",argv[1]);
write(fd[1],buffer,strlen(buffer));
printf("Child[%d] Quit\n\a",getpid());
exit(0);
}
else
{
close(fd[1]);
printf("Parent[%d] Read from pipe\n\a",getpid());
memset(buffer,'\0',BUFFER+1);
read(fd[0],buffer,BUFFER);
printf("Parent[%d] Read：%s\n",getpid(),buffer);
exit(1);
}
}
```

`int dup2(int oldfd,newfd)`实现重定向操作。
```C:n
#include <unistd.h>;
#include <stdio.h>;
#include <errno.h>;
#include <fcntl.h>;
#include <string.h>;
#include <sys/types.h>;
#include <sys/stat.h>;
#define BUFFER_SIZE 1024
int main(int argc,char **argv)
{
int fd;
char buffer[BUFFER_SIZE];
if(argc!=2)
{
fprintf(stderr,"Usage：%s outfilename\n\a",argv[0]);
exit(1);
}
if((fd=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR))==-1)
{
fprintf(stderr,"Open %s Error：%s\n\a",argv[1],strerror(errno));
exit(1);
}
if(dup2(fd,STDOUT_FILENO)==-1)
{
fprintf(stderr,"Redirect Standard Out Error：%s\n\a",strerror(errno));
exit(1);
}
fprintf(stderr,"Now,please input string");
fprintf(stderr,"(To quit use CTRL+D)\n");
while(1)
{
fgets(buffer,BUFFER_SIZE,stdin);
if(feof(stdin))break;
write(STDOUT_FILENO,buffer,strlen(buffer));
}
exit(0);
}
```