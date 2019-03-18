## 技术选型
MQ 可以使用如 Apache ActiveMQ； Worker/ 动态服务可以通过如 Java 技术实现； RPC 可以选择如 alibab
a Dubbo； KV 持久化存储可以选择 SSDB（如果使用 SSD 盘则可以选择 SSDB+RocksDB 引擎）或者 AR
DB（ LMDB 引擎版）； 缓存使用 Redis； SSDB/Redis 分片使用如 Twemproxy，这样不管使用 Java 还是
Nginx+Lua，它们都不关心分片逻辑； 前端模板拼装使用 Nginx+Lua； 数据集群数据存储的机器可以采用 RAI
D 技术或者主从模式防止单点故障； 因为数据变更不频繁，可以考虑 SSD 替代机械硬盘。


## 核心流程
1. 首先我们监听商品数据变更消息；
2. 接收到消息后，数据聚合 Worker 通过 RPC 调用相关系统获取所有要展示的数据，此处获取数据的来源可
能非常多而且响应速度完全受制于这些系统，可能耗时几百毫秒甚至上秒的时间；
3. 将数据聚合为 JSON 串存储到相关数据集群；
4. 前端 Nginx 通过 Lua 获取相关集群的数据进行展示；商品页需要获取基本信息+其他信息进行模板拼装，即
拼装模板仅需要两次调用（另外因为其他信息数据量少且对一致性要求不高，因此我们完全可以缓存到 Ngin
x 本地全局内存，这样可以减少远程调用提高性能）；当页面滚动到商品介绍页面时异步调用商品介绍服务
获取数据；
5. 如果从聚合的 SSDB 集群 /Redis 中获取不到相关数据；则回源到动态服务通过 RPC 调用相关系统获取所
有要展示的数据返回（此处可以做限流处理，因为如果大量请求过来的话可能导致服务雪崩，需要采取保护
措施），此处的逻辑和数据聚合 Worker 完全一样；然后发送 MQ 通知数据变更，这样下次访问时就可以从
聚合的 SSDB 集群 /Redis 中获取数据了。
基本流程如上所述，主要分为 Worker、动态服务、数据存储和前端展示；因为系统非常复杂，只介绍动态服务
和前端展示、数据存储架构；Worker 部分不做实现。

项目部署目录结构：
```
/usr/chapter7
    ssdb_basic_7770.conf
    ssdb_basic_7771.conf
    ssdb_basic_7772.conf
    ssdb_basic_7773.conf
    ssdb_desc_8880.conf
    ssdb_desc_8881.conf
    ssdb_desc_8882.conf
    ssdb_desc_8883.conf
    redis_other_6660.conf
    redis_other_6661.conf
    nginx_chapter7.conf
    nutcracker.yml
    nutcracker.init
    item.html
    header.html
    footer.html
    item.lua
    desc.lua
    lualib
        item.lua
        item
            common.lua
    webapp
WEB-INF
    lib
    classes
    web.xml
```


注意：ssdb配置文件使用 Tab 而不是空格做缩排。
启动：nohup /usr/servers/ssdb-1.8.0/ssdb-server /usr/chapter7/ssdb_basic_7770.conf &  

## lua-resty-cookie 获取cookie进行解析