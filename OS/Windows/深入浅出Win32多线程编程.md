---
date: 2016-05-26 23:19
status: public
title: 深入浅出Win32多线程编程
---

# 基本概念
![](~/23-21-31.jpg)
多数Unix及类Unix是多进程单线程系统。
![](~/23-24-19.jpg)

# Win32进程

## 进程间通信（IPC）
* Clip Board
* Dynamic Data Exchange
* Component Object Model
* File Mapping
* Mail Slots
* Pipes
* Socket
* Remote Procedure Call
* WM_COPYDATA Message

## 获取进程信息
1. EnumProcesses()获取进程ID

![](~/23-32-39.jpg)

2. GetModuleFileNameExA()通过进程句柄获取进程文件名

![](~/23-35-42.jpg)


**遍历系统进程，获得进程列表：**
```C++:n
EnumProcesses(process_ids,sizeof(process_ids),&num_processes);
for(int i = 0; i<num_processes;i++){
    process[i] = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,0,process_ids[i]);
    if(GetModuleFileNameExA(process[i],NULL,File_name,sizeof(fileName)))
        cout<<fileName<<endl;
}
```


# Win32线程
32个优先级，0~31，31最高。1~15为可变优先级，绝大多数程序；16~31为实时优先级，监控类程序。

# Win32核心对象
1. WaitForSingleObject,等待对象的激活

![](~/23-54-26.jpg)

可作为其第一个参数的对象包括：
Change notification,Console input,Event,Job,Memory resource notification,Mutex,Process,Semaphore,Thread,Waitable timer.

若等待对象不可用，则线程挂起，直到可用。
对不同对象，该函数行为含义不同。
若是hThread对象，则判断一个进程是否结束；
若是hMutex对象，则判断是否能够进入临界区；
若是hProcess对象，则等待一个进程的结束。

与该函数对应的还有一个WaitForMultipleObjects()函数。

2. CloseHandle(),关闭对象。
可通过GetLastError()进一步获得关闭对象时的错误原因。