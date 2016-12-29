---
date: 2016-07-23 15:39
status: public
title: Linux_C-消息管理
---

Linux的进程通信IPC

# 1. POSIX无名信号量
POSIX标准有名无名，Linux实现了无名信号量。
```c:n
/* POSIX标准无名信号量函数 */
#include <semaphore.h>
int sem_init(sem_t *sem,int pshared,unsigned int value);
int sem_destroy(sem_t *sem);
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_post(sem_t *sem);
int sem_getvalue(sem_t *sem);
```

# 2. System V信号量
```C:n
#include <sys/types.h>;
#include <sys/ipc.h>;
#include <sys/sem.h>;
key_t ftok(char *pathname,char proj);
int semget(key_t key,int nsems,int semflg);
int semctl(int semid,int semnum,int cmd,union semun arg);
int semop(int semid,struct sembuf *spos,int nspos);
struct sembuf {
short sem_num; /* 使用那一个信号 */
short sem_op; /* 进行什么操作 */
short sem_flg; /* 操作的标志 */
};
```

**实例：用标准错误输出代替打印机**
```c:n
#include<stdio.h>
#include<unisted.h>
#include<limits.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#define PERMS S_IRUSR|S_IWUSR 
void init_semaphore_struct(struct sembuf *sem,int semnum,int semop,int semflg)
{
	//初始化信号灯结构
	sem->sem_num = semnum;
	sem->sem_op = semop;
	sem->sem_flg = semflg;
}
int del_semaphore(int semid)
{
	//信号灯并不随程序结束而被删除，若未删除，可用ipcs命令查看到信号灯，ipcrm可以删除信号灯
	#if 1
	return semctl(semid,0,IPC_RMID);
	#endif
}
int main(int argc,char** argv)
{
	char buffer[MAX_CANON],*c;
	int i,n;
	int semid,semop_ret,status;
	pid_t childpid;
	struct sembuf semwait,semsignal;
	if((argc!=2)||((n=atoi(argv[1]))<1))
	{
		fprintf(stderr,"Usage: %s number\n\a",argv[0]);
		exit(1);
	}
	
	//使用IPC_PRIVATE表示由系统选择一个关键字来创建，信号灯初始值为0
	if((semid=semget(IPC_PRIVATE,1,PERMS))==-1)
	{
		fprintf(stderr,"[%d]: Increment Semaphore Error: %s\n\a",getpid(),strerror(errno));
		if(del_semaphore(semid==-1)
			fprintf(stderr,"[%d]: Destroy Semaphore Error: %s\n\a",getpid(),strerror(error));
		exit(1);
	}
	//创建一个进程链
	for(i=0;i<n;i++)
		if(childpid=fork())break;
	sprintf(buffer,"[i=%d]-->,[Process=%d]-->;[Parent=%d]-->;[Child=%d]\n",i,getpid(),getpid(),childpid);
	c=buffer;
	//这里要求资源，进入原子操作
	while(((semop_ret=semop(semid,&semwait,1))==-1)&&(errno==EINTR));//;多余的？
	if(semop_ret==-1)
	{
		fprintf(stderr,"[%d]: Decrement Semaphore Error: %s\n\a",getpid(),strerror(error));
		
	}
	else{
		while(*c!='\0')fputc(*c++,stderr);
		//原子操作完成，释放资源
		while(((semop_ret=semop(semid,&semsignal,1))==-1)&&(error==EINTR))
			if(semop_ret==-1)
				fprintf(stderr,"[%d]: Increment Semaphore Error: %s\n\a",getpid(),strerror(error));
	}
	
	//不能在其他进程反问信号灯的时候，删除了信号灯
	while((wait(&status)==-1)&&(error==EINTR))
	//信号灯只能被删除一次
	if(i==1)
		if(del_semaphore(semid)==-1)
			fprintf(stderr,"[%d]: Destroy Semaphore Error: %s\n\a",getpid(),strerror(errno));
			exit(0);
}
//信号灯的用途是保证资源在一个时刻只被一个进程所拥有
```

# 3. SystemV 消息队列
```c:n
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
int msgget(key_t key,int msgflg);
int msgsnd(int msgid,struct msgbuf *msgp,int msgsz,int msgflg);
int msgrcv(int msgid,struct msgbuf *msgp,int msgsz,
long msgtype,int msgflg);
int msgctl(Int msgid,int cmd,struct msqid_ds *buf);
struct msgbuf {
long msgtype; /* 消息类型 */
....... /* 其他数据类型 */
}
```

```c:n 
//服务端server.c 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<unisted.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/stat.h>
#include<sys/msg.h>
#define MSG_FILE "server.c"
#define BUFFER 255
#define PERM S_IRUSR|S_IWUSR
struct msgtype{
	long mtype;
	char buffer[BUFFER+1];
	};
int main()
{
	struct msgtype msg;
	key_t key;
	int msgid;
	if((key=frok(MSG_FILE,'a'))==-1)
	{
		fprintf(stderr,"Create Key Error: %s\a\n",strerror(errno));
		exit(1);
	}
	if((msgid=msgget(key,PERM|IPC_CREATE|IPC_EXCL))==-1)
	{
		fprintf(stderr,"Create Message Error: %s\a\n",strerror(errno));
		exit(1);
	}
	while(1)
	{
		msgrcv(msgid,&msg,sizeof(struct msgtype),1,0);
		fprintf(stderr,"Server Receive: %s\n",msg.buffer);
		msg.mtype = 2;
		msgsnd(msgid,&msg,sizeof(struct msgtype),0);
	}
	exit(0);
}


/*==============*/
//客户端client.c 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/stat.h>
#define MSG_FILE "server.c"
#define BUFFER 255
#define PERM S_IRUSR|S_IWUSR
struct msgtype{
	long mtype;
	char buffer[BUFFER+1];
	};
int main(int argc,char** argv)
{
	struct msgtype msg;
	key_t key;
	int msgid;
	if(argc!=2)
	{
		fprintf(stderr,"Usage: %s String\n\a",argv[0]);
		exit(1);
	}
	if((key=ftok(MAX_FILE,'a')==-1)
	{
		fprintf(stderr,"Create Key Error: %s\a\n",strerror(errno));
		exit(1);
	}
	if((msgid=msgget(key,PERM)==-1)
	{
		fprintf(stderr,"Create Message Error: %s\a\n",strerror(errno));
		exit(1);
	}
	msg.mtype=1;
	strncpy(msg.buffer,argv[1],BUFFER);
	msgsnd(msgid,&msg,sizeof(struct msgtype),0);
	memset(&msg,'\0',sizeof(struct msgtype));
	msgrcv(msgid,&msg,sizeof(struct msgtype),2,0);
	fprintf(stderr,"Client receive: %s\n",msg.buffer);
	exit(0);
}
//服务端创建的消息队列最后没有删除，需要使用ipcrm命令删除
```
# 4. SystemV共享内存
还有一种进程通信的方法是使用共享内存。 
```c:n
#include <sys/types.h>;
#include <sys/ipc.h>;
#include <sys/shm.h>;
int shmget(key_t key,int size,int shmflg);
void *shmat(int shmid,const void *shmaddr,int shmflg);
int shmdt(const void *shmaddr);
int shmctl(int shmid,int cmd,struct shmid_ds *buf);
```
在使用一个共享内存之前调用shmat得到共享内存的开始地址，使用结束后使用shmdt断开内存。

```c:n
/*该程序是父进程将参数写入到共享内存，然后子进程把内容读出来，最后要使用ipcrm释放资源的。先用ipcs找出ID然后ipcrm shm ID删除。*/
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unisted.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#define PERM S_IRUSR|S_IWUSR
int main(int argc,char **argv)
{
int shmid;
char *p_addr,*c_addr;
if(argc!=2)
{
fprintf(stderr,"Usage：%s\n\a",argv[0]);
exit(1);
}
if((shmid=shmget(IPC_PRIVATE,1024,PERM))==-1)
{
fprintf(stderr,"Create Share Memory Error：%s\n\a",strerror(errno));
exit(1);
}
if(fork())
{
p_addr=shmat(shmid,0,0);
memset(p_addr,'\0',1024);
strncpy(p_addr,argv[1],1024);
exit(0);
}
else
{
c_addr=shmat(shmid,0,0);
printf("Client get %s",c_addr);
exit(0);
}
}
```
# 相关笔记目录
## [基础知识-gcc\makefile](http://danceiny.farbox.com/post/c/linuxcao-zuo-xi-tong-xia-cyu-yan-bian-cheng-ru-men)
## [时间概念-时间测量](http://danceiny.farbox.com/post/c/linux_c-shi-jian-gai-nian)
## [消息管理-signal](http://danceiny.farbox.com/post/c/linux_c-xiao-xi-guan-li)
## [网络编程-Socket\IP\TCP\UDP](http://danceiny.farbox.com/post/c/linux_c-wang-luo-bian-cheng)
## [线程操作-文件夹备份](http://danceiny.farbox.com/post/c/linux_c-xian-cheng-cao-zuo)
## [开发工具-gcc/gdb](http://danceiny.farbox.com/post/c/linuxxia-ckai-fa-gong-ju)