gevent是一个基于libev的并发库。它为各种并发和网络相关的任务提供了整洁的API。

当主程序(main program)收到一个SIGQUIT信号时，不能成功做yield操作的 Greenlet可能会令意外地挂起程序的执行。这导致了所谓的僵尸进程， 它需要在Python解释器之外被kill掉。

对此，一个通用的处理模式就是在主程序中监听SIGQUIT信号，在程序退出 调用gevent.shutdown。
`gevent.signal(signal.SIGQUIT, gevent.shutdown)`