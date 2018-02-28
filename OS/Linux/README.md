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