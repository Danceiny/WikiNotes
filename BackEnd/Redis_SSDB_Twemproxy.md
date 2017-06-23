目前有如 SSDB、ARDB 等，还有如京东的 JIMDB，它们都支持 Redis 协议，可以支持 Redis 客户
端直接访问；而这些持久化存储大多数使用了如LevelDB、RocksDB、LMDB 持久化引擎来实现数据的持久化存储；

# Redis
* 后台启动 Redis 服务器
```shell
nohup /usr/servers/redis-2.8.19/src/redis-server /usr/servers/redis-2.8.19/redis.conf &
ps -aux | grep redis
```

* 进入客户端
/usr/servers/redis-2.8.19/src/redis-cli -p 6379
```
127.0.0.1:6379> set i 1
OK
127.0.0.1:6379> get i
"1"
```
# [SSDB服务器](https://github.com/ideawu/ssdbhttp://ssdb.io/docs/zh_cn/install.htm)
nohup /usr/servers/ssdb-1.8.0/ssdb-server /usr/servers/ssdb-1.8.0/ssdb.conf &
ps -aux | grep ssdb
/usr/servers/ssdb-1.8.0/tools/ssdb-cli -p 8888
/usr/servers/redis-2.8.19/src/redis-cli -p 888

因为 SSDB 支持 Redis 协议，所以用 Redis 客户端也可以访问
127.0.0.1:8888> set i 1
OK
127.0.0.1:8888> get i
"1"


# [Twemproxy](https://github.com/twitter/twemproxy)
```shell
cd /usr/servers
wget https://github.com/twitter/twemproxy/archive/v0.4.0.tar.gz
tar -xvf v0.4.0.tar.gz
cd twemproxy-0.4.0/
autoreconf -fvi
./configure && make
```
如上安装方式在有些服务器上可能在大量如mset时可能导致 Twemproxy 崩溃，需要使用如
CFLAGS="-O1" ./configure && make 或 CFLAGS="-O3 -fno-strict-aliasing" ./configure && make 安装。

配置：`vim /usr/servers/twemproxy-0.4.0/conf/nutcracker.yml`
```yml
server1:
listen: 127.0.0.1:1111
hash: fnv1a_64
distribution: ketama
redis: true
servers:
- 127.0.0.1:6379:1
```

启动：`/usr/servers/twemproxy-0.4.0/src/nutcracker -d -c /usr/servers/twemproxy-0.4.0/conf/nutcracker.yml`
-d 指定后台启动 -c 指定配置文件；此处我们指定了代理端口为 1111

ps -aux | grep nutcracker
/usr/servers/redis-2.8.19/src/redis-cli -p 1111
127.0.0.1:1111> set i 1
OK
127.0.0.1:1111> get i
"1"
```