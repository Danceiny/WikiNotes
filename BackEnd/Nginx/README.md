# 介绍

# 编译

# 信号控制
https://nginx.org/en/docs/control.html

- HUP 开启新进程重读配置文件，关掉旧进程。
- INT, TERM
- QUIT 等完成请求后再关闭
- USR1 日志分割
- USR2 nginx升级
- WINCH
kill -TYPE master-pid
# 整合PHP、Python等

# 虚拟主机配置

# 日志切割
定时任务做日志切割。

```bash
#!/bin/bash

LOGPATH=/usr/local/nginx/logs/access.log
BASEPATH=/data
bak = $BASEPATH/$(data -d yesterday +%Y%m%d).accesss.log
mkdir -p $BASEPATH/%(data -d yesterday +%Y%m)
mv $LOGPATH $bak
touch $LOGPATH
kill -USR1 `cat /usr/local/nginx/logs/nginx.pid`
```

`crontab e`
# gzip设置

# 浏览器缓存配置

# Rewrite规则

# memcached

# 连接数优化

# 反向代理
proxy
upstream
# 集群与负载均衡

socket连接数
more /proc/sys/net/core/somaxconn
改大。

加快tcp回收
cat /proc/sys/net/ipv4/tcp_tw_recycle
改成1。


# URL重写
location -> rewrite


# Nginx + Memcache
memcache_pass

# 自动列目录
```
location / {
  autoindex on;
 }
```
# 参考文档
- http://tengine.taobao.org/book/chapter_02.html#id1
