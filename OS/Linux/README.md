二、使用netstat来看看系统的监听端口

# netstat -antp       //所有连接、数字显示主机、端口、TCP连接、监听的程序

# netstat -anup       //所有连接、数字显示主机、端口、UDP连接、监听的程序

# netstat -s          //统计所有（开机至今的）连接数据，包括tcp、udp等

# netstat -st         //统计所有tcp连接数据

# netstat -su         //统计所有udp连接数据

# netstat -su         //粗略统计连接数据

允许8098端口的tcp：
iptables -t filter -A INPUT -p tcp --dport 8098 -j ACCEPT