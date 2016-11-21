---
date: 2016-06-10 13:27
status: public
title: Linux_C-时间概念
---

# 时间表示
time()函数返回从1970年1月1日0点以来的秒数，存储在time_t结构中。
ctime()函数将秒数转化为一个字符串，长度固定为26，形如`Thu Dec 7 14：58：59 2000`.
# 时间测量
```C:n
#include <sys/time.h>;
int gettimeofday(struct timeval *tv,struct timezone *tz);
strut timeval {
long tv_sec; /* 秒数 */
long tv_usec; /* 微秒数 */
};

//gettimeofday 将时间保存在结构 tv 之中.tz 一般我们使用 NULL 来代替.
#include <sys/time.h<
#include <stdio.h<
#include <math.h<
void function()
{
unsigned int i,j;
double y;
for(i=0;i<1000;i++)
for(j=0;j<1000;j++)
y=sin((double)i);
}
main()
{
struct timeval tpstart,tpend;
float timeuse;
gettimeofday(&tpstart,NULL);
function();
gettimeofday(&tpend,NULL);
timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+
tpend.tv_usec-tpstart.tv_usec;
timeuse/=1000000;
printf("Used Time：%f\n",timeuse);
exit(0);
}
```
# 计时器使用
Linux为每个进程提供3个内部间隔计时器。
* ITIMER_REAL：减少实际时间，到时的时候发出SIGALRM信号。
ITIMER_VIRTUAL：减少有效时间（进程执行时间），产生SIGVTALRM信号。
* ITIMER_PROF：减少进程的有效时间和系统时间（为进程调度花费的时间）。这个经常和上面一个使用用来计算系统内核时间和用户时间，产生SIGPROF信号。

```C:n
#include <sys/time.h>;
int getitimer(int which,struct itimerval *value);
int setitimer(int which,struct itimerval *newval,
struct itimerval *oldval);
struct itimerval {
struct timeval it_interval;
struct timeval it_value;
}
```

这个程序每执行两秒中之后会输出一个提示:
```C:n
#include <sys/time.h>;
#include <stdio.h>;
#include <unistd.h>;
#include <signal.h>;
#include <string.h>;
#define PROMPT "时间已经过去了两秒钟\n\a"
char *prompt=PROMPT;
unsigned int len;
void prompt_info(int signo)
{
write(STDERR_FILENO,prompt,len);
}
void init_sigaction(void)
{
struct sigaction act;
act.sa_handler=prompt_info;
act.sa_flags=0;
sigemptyset(&act.sa_mask);
sigaction(SIGPROF,&act,NULL);
}
void init_time()
{
struct itimerval value;
value.it_value.tv_sec=2;
value.it_value.tv_usec=0;
value.it_interval=value.it_value;
setitimer(ITIMER_PROF,&value,NULL);
}
int main()
{
len=strlen(prompt);
init_sigaction();
init_time();
while(1);
exit(0);
}
```
# 相关笔记目录
## [基础知识-gcc\makefile](http://danceiny.farbox.com/post/c/linuxcao-zuo-xi-tong-xia-cyu-yan-bian-cheng-ru-men)
## [时间概念-时间测量](http://danceiny.farbox.com/post/c/linux_c-shi-jian-gai-nian)
## [消息管理-signal](http://danceiny.farbox.com/post/c/linux_c-xiao-xi-guan-li)
## [网络编程-Socket\IP\TCP\UDP](http://danceiny.farbox.com/post/c/linux_c-wang-luo-bian-cheng)
## [线程操作-文件夹备份](http://danceiny.farbox.com/post/c/linux_c-xian-cheng-cao-zuo)
## [开发工具-gcc/gdb](http://danceiny.farbox.com/post/c/linuxxia-ckai-fa-gong-ju)