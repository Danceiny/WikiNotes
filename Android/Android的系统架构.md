- Android的整体架构和特征

1. 应用层

2. 框架层
>框架层由多个系统服务共同组成（包括地理信息服务、电源管理服务等），所有服务寄宿在系统核心进程(System Core Process)中。运行时每个服务占据一个独立的线程，通过IPC发送消息和传输数据。

3. 运行时
>Dalvik是Java虚拟机，但是没有沿用Java Bytecode作为一次编译的中间文件，而是应用了新的二进制码格式文件.dex。

4. 核心类库
>例如opengl，sqlite。

5. 硬件抽象层

6. Linux内核


- Android的主要功能模块及其特点
- Android的设计思想