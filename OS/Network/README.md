# TCP

![](./~img/TCP状态转换图.png)


# TCP/UDP区别
- http://feinibuke.blog.51cto.com/1724260/340272

# 
![](http://opkk27k9n.bkt.clouddn.com/17-8-5/60377616.jpg)


  应用层：www，SMTP，DNS，FTP 
  
  传输层：解释数据 
  
  网路层：定位IP地址、确定连接路径 
  
  链路层：与硬件驱动程序对话 


与网络地址相应，对在此网络中的主机，TCP／IP使用网络掩码。在上面的例子中，掩
码是255.0.0.0，其含义是这样：假设18.0.0.3想和18.110.75对话，那么，将这两个地址相互异或，再与掩码取“与“（&），结果是零 ，说明这两个地址可以直接对话；相反，如果要与19.1.1.1对话,那么运算之后不为零,说明必须使用间接方式才能到达。

以太网使用一种载波监听／碰撞检测的方式工作，因为线路上跑的是电气讯号，所
以以太网卡通过监视电缆电位来判断是否有某个设备在传输数据。如果网卡接到一个发
送任务，它首先判断线路上是否有数据传输，如果有，它等待一会：如果没有，它就以
一个特定的数据包大小（帧）将数据发送到线路上。



|协议名 |端口| 协议类型 |说明| 
|---|---|---|---|
  
|daytime| 13 |TCP/UDP| 日期/时间服务 |
  
  ftp 21 TCP 文件传输服务 |
  ftp-data 20 TCP ftp数据 
  
  ssh 22 TCP/UDP Security Shell 
  
  telnet 23 TCP 远程登录 
  
  smtp 25 TCP 电子邮件 
  
  time 37 TCP/UDP 时间服务 
  
  domain 53 TCP/UDP DNS域名服务 
  
  gopher 70 TCP/UDP Gopher服务 
  
  http 80 TCP HTTP传输协议 
  
  pop-3 110 TCP 邮局协议3 
  
  sunrpc 111 TCP/UDP 端口映像程序 
  
  auth 113 TCP 用户身份认证 
  
  nntp 119 TCP Usenet新闻传输 
  
  netbios-ns 137 TCP/UDP netbios名字服务 
  
  netbios-dgm 138 TCP/UDP netbios数据处理 
  
  netbios-ssn 139 TCP/UDP netbios会话 
  
  imap2 143 TCP/UDP IMAP邮件管理 
  
  irc 194 TCP/UDP Internet Chat 
  
  https 443 TCP/UDP 使用SSL的http 



| 信号ID |名称 |含义 |是否可以捕获 |是否可以阻塞| 
|---|---|---|---|---| 
| 1 |SIGHUP| 挂起 |是| 是 |
| 2 |SIGINT |中断 |是 |是 |
| 3 |SIGQUIT| 退出 |是 |是| 
| 9 |SIGKILL| 杀死进程| 否| 否| 
| 15|SIGTERM|软件终止 |是 |是| 