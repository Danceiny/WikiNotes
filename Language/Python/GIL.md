标准库中所有执行阻塞型IO操作的函数，在等待操作系统返回结果时都会释放GIL，这意味着在Python语言这个层次上可以使用多线程，而IO密集型Python程序从中受益：一个Python线程等待网络响应时，阻塞型IO函数会释放GIL，再运行一个线程。





使用concurrent.futures模块绕开GIL

- 启动进程
- ​