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

**基本设置**
port 6379   #默认6379
logfile ""  #默认 /dev/null

**Redis内存**
```conf
内存大小对应关系
# 1k => 1000 bytes
# 1kb => 1024 bytes
# 1m => 1000000 bytes
# 1mb => 1024*1024 bytes
# 1g => 1000000000 bytes
# 1gb => 1024*1024*1024 bytes
#设置Redis占用100mb的大小
maxmemory 100mb
#如果内存满了就需要按照如下相应算法进行删除过期的/最老的
#volatile-lru 根据LRU算法移除设置了过期的key
#allkeys-lru 根据LRU算法移除任何key(包含那些未设置过期时间的key)
#volatile-random/allkeys->random 使用随机算法而不是LRU进行删除
#volatile-ttl 根据Time-To-Live移除即将过期的key
#noeviction 永不过期，而是报错
maxmemory-policy volatile-lru
#Redis并不是真正的LRU/TTL，而是基于采样进行移除的，即如采样10个数据移除其中最老的/即将过期的
maxmemory-samples 10
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
Twemproxy 是一个 Redis/Memcached 代理中间件，可以实现诸如分片逻辑、HashTag、减少连接数等功能；尤其在有大量应用服务器的场景下 Twemproxy 的角色就凸显了，能有效减少连接数。

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


### Redis主从
Redis 本身是单线程的，因此我们可以设置每个实例在 6-8GB 之间，通过启动更多的实例提高吞吐量。如 128GB 的我们可以开启 8GB * 10 个实例，充分利用多核 CPU。

实际项目时，为了提高吞吐量，我们使用主从策略，即数据写到主 Redis，读的时候从从 Redis上读，这样可以通过挂载更多的从来提高吞吐量。而且可以通过主从机制，在叶子节点开启持久化方式防止数据丢失。

```conf
#在配置文件中挂载主从，不推荐这种方式，我们实际应用时Redis可能是会宕机的
slaveof masterIP masterPort
#从是否只读，默认yes
slave-read-only yes
#当从失去与主的连接或者复制正在进行时，从是响应客户端（可能返回过期的数据）还是返回“SYNC with master in progress”错误，默认yes响应客户端
slave-serve-stale-data yes
#从库按照默认10s的周期向主库发送PING测试连通性
repl-ping-slave-period 10
#设置复制超时时间（SYNC期间批量I/O传输、PING的超时时间），确保此值大于repl-ping-slave-period
#repl-timeout 60
#当从断开与主的连接时的复制缓存区，仅当第一个从断开时创建一个，缓存区越大从断开的时间可以持续越长
# repl-backlog-size 1mb
#当从与主断开持续多久时清空复制缓存区，此时从就需要全量复制了，如果设置为0将永不清空
# repl-backlog-ttl 3600
#slave客户端缓存区，如果缓存区超过256mb将直接断开与从的连接，如果持续60秒超过64mb也会断开与从的连接
client-output-buffer-limit slave 256mb 64mb 60
此处需要根据实际情况设置client-output-buffer-limit slave和 repl-backlog-size；比如如果网络环境不好，从与主经常断开
```


设置主从：
```
cd /usr/servers/redis-2.8.19
cp redis.conf redis_6660.conf
cp redis.conf redis_6661.conf
vim redis_6660.conf
vim redis_6661.conf
```
改端口后启动：
```
nohup /usr/servers/redis-2.8.19/src/redis-server/usr/servers/redis-2.8.19/redis_6660.conf &
nohup /usr/servers/redis-2.8.19/src/redis-server/usr/servers/redis-2.8.19/redis_6661.conf &
```

进入Slave,`/usr/servers/redis-2.8.19/src/redis-cli -p 6661`
```
127.0.0.1:6661> slaveof 127.0.0.1 6660
OK
127.0.0.1:6661> info replication
# Replication
role:slave
master_host:127.0.0.1
master_port:6660
master_link_status:up
master_last_io_seconds_ago:3
master_sync_in_progress:0
slave_repl_offset:57
slave_priority:100
slave_read_only:1
connected_slaves:0
master_repl_offset:0
repl_backlog_active:0
repl_backlog_size:1048576
repl_backlog_first_byte_offset:0
repl_backlog_histlen:0
```

进入Master,`/usr/servers/redis-2.8.19# /usr/servers/redis-2.8.19/src/redis-cli -p 6660`.
在Master `127.0.0.1:6660>set i 1`，再进入Slave，可以看到`127.0.0.1:6661>get i`得到"1"。


#### [Redis持久化](http://redis.readthedocs.org/en/latest/topic/persistence.html)
**可以用一个Slave专门给Redis来做数据持久化存储。**

- RDB持久化 `save seconds changes #即seconds秒变更changes次则保存`
- AOF持久化(append only file) 
>即把每一个用户操作的命令保存下来，这样就会存在好多重复的命令导致恢复时间过长，那么可以通过相应的配置定期进行 AOF 重写来减少重复


#### [Redis动态调整配置]


## [lua-resty-mysql](https://github.com/openresty/lua-resty-mysql)
```lua
local function close_db(db)
if not db then
return
end
db:close()
end
local mysql = require("resty.mysql")
--创建实例
local db, err = mysql:new()
if not db then
ngx.say("new mysql error : ", err)
return
end
--设置超时时间(毫秒)
db:set_timeout(1000)
local props = {
host = "127.0.0.1",
port = 3306,
database = "mysql",
user = "root",
password = "123456"
}
local res, err, errno, sqlstate = db:connect(props)
if not res then
ngx.say("connect to mysql error : ", err, " , errno : ", errno, " , sqlstate : ", sqlstate)
return close_db(db)
end
--删除表
local drop_table_sql = "drop table if exists test"
res, err, errno, sqlstate = db:query(drop_table_sql)
if not res then
ngx.say("drop table error : ", err, " , errno : ", errno, " , sqlstate : ", sqlstate)
return close_db(db)
end
--创建表
local create_table_sql = "create table test(id int primary key auto_increment, ch varchar(100))"
res, err, errno, sqlstate = db:query(create_table_sql)
if not res then
ngx.say("create table error : ", err, " , errno : ", errno, " , sqlstate : ", sqlstate)
return close_db(db)
end
--插入
local insert_sql = "insert into test (ch) values('hello')"
res, err, errno, sqlstate = db:query(insert_sql)
if not res then
ngx.say("insert error : ", err, " , errno : ", errno, " , sqlstate : ", sqlstate)
return close_db(db)
end
res, err, errno, sqlstate = db:query(insert_sql)
ngx.say("insert rows : ", res.affected_rows, " , id : ", res.insert_id, "<br/>")
--更新
local update_sql = "update test set ch = 'hello2' where id =" .. res.insert_id
res, err, errno, sqlstate = db:query(update_sql)
if not res then
ngx.say("update error : ", err, " , errno : ", errno, " , sqlstate : ", sqlstate)
return close_db(db)
end
ngx.say("update rows : ", res.affected_rows, "<br/>")
--查询
local select_sql = "select id, ch from test"
res, err, errno, sqlstate = db:query(select_sql)
if not res then
ngx.say("select error : ", err, " , errno : ", errno, " , sqlstate : ", sqlstate)
return close_db(db)
end
for i, row in ipairs(res) do
for name, value in pairs(row) do
ngx.say("select row ", i, " : ", name, " = ", value, "<br/>")
end
end
ngx.say("<br/>")
--防止sql注入
local ch_param = ngx.req.get_uri_args()["ch"] or ''
--使用ngx.quote_sql_str防止sql注入
local query_sql = "select id, ch from test where ch = " .. ngx.quote_sql_str(ch_param)
res, err, errno, sqlstate = db:query(query_sql)
if not res then
ngx.say("select error : ", err, " , errno : ", errno, " , sqlstate : ", sqlstate)
return close_db(db)
end
for i, row in ipairs(res) do
for name, value in pairs(row) do
ngx.say("select row ", i, " : ", name, " = ", value, "<br/>")
end
end
--删除
local delete_sql = "delete from test"
res, err, errno, sqlstate = db:query(delete_sql)
if not res then
ngx.say("delete error : ", err, " , errno : ", errno, " , sqlstate : ", sqlstate)
return close_db(db)
end
ngx.say("delete rows : ", res.affected_rows, "<br/>")
close_db(db)
```


add/update/delete的响应：
```lua
{
insert_id = 0,
server_status = 2,
warning_count = 1,
affected_rows = 32,
message = nil
}
```

## [lua-resty-http](https://github.com/pintsized/lua-resty-http)
OpenResty默认没有提供http客户端。

在 nginx.conf 中的 http 部分添加如下指令来做 DNS 解析：`resolver 8.8.8.8;`

## [ngx.location.capture]
ngx.location.capture 也可以用来完成 http 请求，但是它只能请求到相对于当前 nginx 服务器的路径，不能使用之前的绝对路径进行访问，但是我们可以配合 nginx upstream 实现我们想要的功能。

nginx.conf中的http部分添加：
```conf
upstream backend {
server s.taobao.com;
keepalive 100;
}
```
即我们将请求 upstream 到 backend；另外记得一定要添加之前的 DNS 解析器。

example.conf
```conf
location ~ /proxy/(.*) {
internal;
proxy_pass http://backend/$1$is_args$args;
}
```

lua
```lua
local resp = ngx.location.capture("/proxy/search", {
method = ngx.HTTP_GET,
args = {q = "hello"}
})
if not resp then
ngx.say("request error :", err)
return
end
ngx.log(ngx.ERR, tostring(resp.status))
--获取状态码
ngx.status = resp.status
--获取响应头
for k, v in pairs(resp.header) do
if k ~= "Transfer-Encoding" and k ~= "Connection" then
ngx.header[k] = v
end
end
--响应体
if resp.body then
ngx.say(resp.body)
end
```

我们通过 upstream+ngx.location.capture 方式虽然麻烦点，但是得到更好的性能和upstream 的连接池、负载均衡、故障转移、proxy cache 等特性。

不过因为继承在当前请求的请求头，所以可能会存在一些问题，比较常见的就是 gzip 压缩问题，ngx.location.capture 不会解压缩后端服务器的 GZIP 内容，解决办法可以参考https://github.com/openresty/lua-nginx-module/issues/12；因为我们大部分这种http 调用的都是内部服务，因此完全可以在 proxy location 中添加proxy_pass_request_headers off; 来不传递请求头。

## JSON库
cjson、dkjson。其中 cjson的语法严格（比如 unicode \u0020\u7eaf ），要求符合规范否则会解析失败（如 \u002），而 dkjson 相对宽松

## 编码转换
lua-iconv [http://ittner.github.io/lua-iconv/](http://ittner.github.io/lua-iconv/)

## 模板渲染
 lua-resty-template
• {(include_file)}：包含另一个模板文件；
• {* var *}：变量输出；
• {{ var }}：变量转义输出；
• {% code %}：代码片段；
• {# comment #}：注释；
• {-raw-}：中间的内容不会解析，作为纯文本输出


