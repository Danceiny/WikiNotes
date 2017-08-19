# 介绍

# 编译

# 信号控制
https://nginx.org/en/docs/control.html

# 整合PHP、Python等

# 虚拟主机配置

# 日志切割

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

# 参考文档
- http://tengine.taobao.org/book/chapter_02.html#id1