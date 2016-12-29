---
date: 2016-07-24 04:43
status: public
title: Linux_C-线程操作
---

*前言*
>先介绍什么是线程.我们编写的程序大多数可以看成是单线程的.就是程序是按照一定的
顺序来执行.如果我们使用线程的话,程序就会在我们创建线成的地方分叉,变成两个"程
序"在执行.粗略的看来好象和子进程差不多的,其实不然.子进程是通过拷贝父进程的地
址空间来执行的.而线程是通过共享程序代码来执行的,讲的通俗一点就是线程的相同的
代码会被执行几次.使用线程的好处是可以节省资源,由于线程是通过共享代码的,所以没
有进程调度那么复杂。

#1. 线程的创建和使用
```c:n
#include <pthread.h>;
int pthread_create(pthread_t *thread,pthread_attr_t *attr,void *(*start_routine)(void *),void *arg);
void pthread_exit(void *retval);
int pthread_join(pthread *thread,void **thread_return);
```

start_routine函数指针是线程创建成功后开始执行的函数，void* arg是传给stat_routine的参数。

pthread_exit和exit函数结束线程，释放函数资源，最后阻塞，直到其他线程使用。
pthread_join函数等待它，然后将*retval的值传给**thread_return。**由于exit时释放了函数资源，因此retval不能指向函数的局部变量。**

```c:n
/*实现当前目录下的所有文件备份，备份后的后缀名为bak*/

#include<stdio.h>
#include<unisted.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<pthread.h>
#include<dirent.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/time.h>

#define BUFFER 512
struct copy_file{
int infile;
int outfile;
};

void *copy(void* arg)
{
	int infile,outfile;
	int bytes_read,bytes_write,*bytes_copy_p;
	char buffer[BUFFER],*buffer_p;
	struct copy_file *file = (struct copy_file*)arg;
	infile = file->infile;
	outfile = file->outfile;
	
	//线程退出时，所有变量空间被释放，故须自己分配内存
	if((bytes_copy_p=(int*)malloc(sizeof(int)))==NULL)pthread_exit(NULL);
	bytes_read = bytes_write = 0;
	//拷贝文件
	while((bytes_read = read(infile,buffer,BUFFER))!=0)
	{
		if((bytes_read==-1)&&(errno!=EINTR))break;
		else if(bytes_read>0)
		{
			buffer_p = buffer;
			while((bytes_write = write(outfile,buffer_p,bytes_read))!=0)
			{
				if((bytes_write==-1)&&(errno!=EINTR))break;
				else if(bytes_write==bytes_read)break;
				else if(bytes_write>0)
				{
					buffer_p += bytes_write;
					bytes_read = bytes_write;
				}
			}
			if(bytes_write==-1)break;
			*bytes_copy_p += bytes_read;
		}
	}
	close(infile);
	close(outfile);
	pthread_exit(bytes_copy_p);
}

int main(int argc,char** argv)
{
	pthread_t *thread;
	struct copy_file *file;
	int byte_copy,*byte_copy_p,num,i,j;
	char filename[BUFFER];
	struct dirent **namelist;
	struct stat filestat;
	//得到当前路径下面所有文件（包括目录）的个数
	if((num=scandir(".",&namelist,0,alphasort))<0)
	{
		fprintf(stderr,"Get File Num Error: %s\n\a",strerror(errno));
		exit(1);
	}
	//给线程分配空间
	if(((thread=(pthread_t*)malloc(sizeof(pthread_t)*num))==NULL)||((file=(struct copy_file*)malloc(sizeof(struct copy_file)*num))==NULL)
	{
		fprintf(stderr,"Out Of Memory!\n\a");
		exit(1);
	}
	
	for(i=0,j=0;i<num;i++){
		memset(filename,'\0',BUFFER);
		strcpy(filename,namelist->d_name);
		if(stat(filename,&filestat)==-1)
		{
			fprintf(stderr,"Get File Information: %s\n\a",strerror(errno));
			exit(1);
		}
		
		//忽略目录
		if(!S_ISREG(filestat.st_mode))continue;
		if((file[j].infile=open(filename,O_RDONLY))<0)
		{
			fprintf(stderr,"Open %s Error: %s\a\n",filename,strerror(errno));
			continue;
		}
		strcat(filename,".bak");
		if((file[j].outfile=open(filename,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR))<0)
		{
			fprintf(stderr,"Create %s Error: %s\n\a",filename,strerror(errno));
			continue;
		}
		
		//创建线程，进行文件拷贝
		if(pthread_create(&thread[j],NULL,copy,(void*)&file[j])!=0)
		fprintf(stderr,"Create Thread[%d] Error: %s\n\a",i,strerror(errno));
		
		j++;
		}
		
		
		byte_copy = 0;
		for(i=0;i<j;i++)
		{
			//等待线程结束
			if(pthread_join(thread,(void**)&byte_copy_p)!=0)
			fprintf(stderr,"Thread[%d] Join Error: %s\a\n",i,strerror(errno));
			else{
				if(bytes_copy_p == NULL)continue;
				printf("Thread[%d] Copy %d bytes\n\a",i,*byte_copy_p);
				byte_copy+=*byte_copy_p;
				//释放在copy函数中创建的内存
				free(byte_copy_p);
			}
		}
		printf("Total Copy Bytes %d\a\n",byte_copy);
		free(thread);
		free(file);
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