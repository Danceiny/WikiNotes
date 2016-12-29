---
date: 2016-06-10 14:17
status: public
title: Linux_C-信号处理
---

# 信号的产生 
最常用的4个发出信号的系统函数是kill，raise,alarm,setitimer函数。
```C:n
#include <sys/types.h>;
#include <signal.h>;
#include <unistd.h>;
int kill(pid_t pid,int sig);
int raise(int sig);
unisigned int alarm(unsigned int seconds);
```
kill 系统调用负责向进程发送信号 sig.
如果 pid 是正数,那么向信号 sig 被发送到进程 pid.
如果 pid 等于 0,那么信号 sig 被发送到所以和 pid 进程在同一个进程组的进程。
如果 pid 等于-1,那么信号发给所有的进程表中的进程,除了最大的哪个进程号.
？？？如果 pid 由于-1,和 0 一样,只是发送进程组是-pid.

```c:n
#include<unisted.h>
main()
{
unsigned int i;
alarm(1);
for(i=0;1;i++)printf("I=%d",i);
}
```
alarm的缺省操作是结束进程，所以程序在1s后结束，这个程序可以比较系统性能差异。

# 信号操作
信号操作最常用的方法是信号屏蔽，用到下面的函数：
`#include<signal.h>`
* sigemptyset()
* sigfillset()
* sigaddset()
* sigdelset()
* sigismember()
* sigprocmask() 最关键
```c:n
#include <signal.h>;
#include <stdio.h>;
#include <math.h>;
#include <stdlib.h>;
int main(int argc,char **argv)
{
double y;
sigset_t intmask;
int i,repeat_factor;
if(argc!=2)
{
fprintf(stderr,"Usage：%s repeat_factor\n\a",argv[0]);
exit(1);
}
if((repeat_factor=atoi(argv[1]))<1)repeat_factor=10;
sigemptyset(&intmask); /* 将信号集合设置为空 */
sigaddset(&intmask,SIGINT); /* 加入中断 Ctrl+C 信号*/
while(1)
{
/*阻塞信号,我们不希望保存原来的集合所以参数为 NULL*/
sigprocmask(SIG_BLOCK,&intmask,NULL);
fprintf(stderr,"SIGINT signal blocked\n");
for(i=0;i<repeat_factor;i++)y=sin((double)i);
fprintf(stderr,"Blocked calculation is finished\n");
/* 取消阻塞 */
sigprocmask(SIG_UNBLOCK,&intmask,NULL);
fprintf(stderr,"SIGINT signal unblocked\n");
for(i=0;i<repeat_factor;i++)y=sin((double)i);
fprintf(stderr,"Unblocked calculation is finished\n");
}
exit(0);
}
```
信号屏蔽只是将信号加到阻塞信号集合中，并未丢弃。

若需对信号做出及时反映，例如当按下ctrl+c时，告诉用户该操作不好，请不要重试，而非毫无反应。此时可用到sigaction()函数。
```C:n
#include <signal.h>;
int sigaction(int signo,const struct sigaction *act,struct sigaction *oact);
//oact保存信息，一般设为null
struct sigaction {
void (*sa_handler)(int signo);
void (*sa_sigaction)(int siginfo_t *info,void *act);
sigset_t sa_mask;
int sa_flags;
void (*sa_restore)(void);
}
```

示例：
```C:n
#include <signal.h>;
#include <stdio.h>;
#include <string.h>;
#include <errno.h>;
#include <unistd.h>;
#define PROMPT "你想终止程序吗?"
char *prompt=PROMPT;
void ctrl_c_op(int signo)
{
write(STDERR_FILENO,prompt,strlen(prompt));
}
int main()
{
struct sigaction act;
act.sa_handler=ctrl_c_op;
sigemptyset(&act.sa_mask);
act.sa_flags=0;
if(sigaction(SIGINT,&act,NULL)<0)
{
fprintf(stderr,"Install Signal Action Error：%s\n\a",strerror(errno));
exit(1);
}
while(1);
}
```
# 其他信号函数
```C:n
#include<unistd.h>
#include<signal.h>
int pause(void);//挂起进程直到一个信号发生了
int sigsuspend(const sigset_t *sigmask); //挂起进程，只是在调用的时候用sigmask取代当前的信号阻塞集合

#include<sigsetjp>
int sigsetjmp(sigjmp_buf env,int val);
void siglongjmp(sigjmp_buf env,int val);
```

# 一个实例
```c:n
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#include<string.h>
#include<pwd.h>
#include<sys/types.h>
#include<sys/stat.h>

/*Linux的默认个人邮箱地址是/var/spool/mail/ */
#define MAIL_DIR "/var/spool/mail/"

//睡眠10s
#define SLEEP_TIME 10
#define MAX_FILENAME 255
unsigned char notifyflag = 1;
long get_file_size(const char* filename)
{
	struct stat buf;
	if(stat(filename,&buf)==-1){
		if(errno==ENOENT)return 0;
		else return -1;
	}
	return (long)buf.st_size;
}

void send_mail_notify(void)
{
	fprintf(stderr,"New mail has arrived\007\n");
}

void turn_on_notify(int signo)
{
	notifyflag = 1;
}

void turn_off_notify(int signo)
{
	notifyflag = 0;
}

int check_mail(const char* filename)
{
	long old_mail_size,new_mail_size;
	sigset_t blockset,emptyset;
	sigemptyset(&blockset);
	sigemptyset(&emptyset);
	
	sigaddset(&blockset,SIGUSR1);
	sigaddset(&blockset,SIGUSR2);
	old_mail_size = get_file_size(filename);
	if(old_mail_size<0)return 1;
	if(old_mail_size>0)send_mail_notify();
	sleep(SLEEP_TIME);
	while(1)
	{
		if(sigprocmask(SIG_BLOCK,&blockset,NULL)<0)return 1;
		while(notifyflag==0)sigsuspend(&emptyset);
		if(sigprocmask(SIG_SETMASK,&emptyset,NULL)<0)return 1;
		new_mail_size = get_file_size(filename);
		if(new_mail_size>old_mail_size)send_mail_notify();
		old_mail_size = new_mail_size;
		sleep(SLEEP_TIME);
	}
}

int main(void)
{
	char mailfile[MAX_FILENAME];
	struct sigaction newact;
	struct passwd *pw;
	if((pw=getpwuid(getuid()))==NULL)
	{
		fprintf(stderr,"Get Login Name Error: %s\n\a",strerror(errno));\
		exit(1);
	}
	strcpy(mailfile,MAIL_DIR);
	strcat(mailfile,pw->pw_name);
	newact.sa_handler = turn_on_notify();
	newact.sa_flags = 0;
	sigemptyset(&newact.sa_mask);
	sigaddset(&newact.sa_mask,SIGUSR1);
	sigaddset(&newact.sa_mask,SIGUSR2);
	if(sigaction(SIGUSR1,&newact,NULL)<0)
		fprintf(stderr,"Turn On Error: %s\n\a",strerror(errno));
	newact.sa_handler = turn_off_notify();
	if(sigaction(SIGUSR1,&newact,NULL)<0)
		fprintf(stderr,"Turn Off Error: %s\n\a",strerror(errno));
	check_mail(mailfile);
	exit(0);
}
```