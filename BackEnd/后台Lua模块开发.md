
使用 require 第一次导入模块后，所有 Nginx 进程全局共享模块的数据和代码，每个 Worker 进程需要时会得到此模块的一个副本（Copy-On-Write），即模块可以认为是每 Worker 进程共享而不是每 Nginx Server 共享；另外注意之前我们使用 init_by_lua 中初始化的全局变量是每请求复制一个；如果想在多个 Worker 进程间共享数据可以使用 ngx.shared.DICT 或如 Redis 之类的存储。

```conf
#lua模块路径，其中”;;”表示默认搜索路径，默认到/usr/servers/nginx下找
lua_package_path "/usr/example/lualib/?.lua;;"; #lua 模块
lua_package_cpath "/usr/example/lualib/?.so;;"; #c模块
```

开发时将所有数据做成局部变量/局部函数；通过 _M 导出要暴露的函数，实现模块化封装。


## [lua-resty-redis](https://github.com/openresty/lua-resty-r
edis)
基于 cosocket API 的 ngx_lua 提供的 Lua redis 客户端，通过它可以完成 Redis 的操作。

启动redis后，
```lua
local function close_redis(red)
if not red then
return
end

-- 可复用版本（tcp握手挥手消耗太大，考虑使用连接池）
local function close_redis(red)
    if not red then
        return
    end
    --释放连接(连接池实现)
    local pool_max_idle_time = 10000 --毫秒
    local pool_size = 100 --连接池大小
    local ok, err = red:set_keepalive(pool_max_idle_time, pool_size)
    -- 此处假设调用 red:set_keepalive()，连接池大小通过 nginx.conf 中 http 部分的如下指令定义：
    -- #默认连接池大小，默认 30 lua_socket_pool_size 30; #默认超时时间,默认 60s lua_socket_keepalive_timeout 60s;

    if not ok then
    ngx.say("set keepalive error : ", err)
    end
end

local ok, err = red:close()
if not ok then
ngx.say("close redis error : ", err)
end
end
local redis = require("resty.redis")
--创建实例
local red = redis:new()
--设置超时（毫秒）
red:set_timeout(1000)
--建立连接
local ip = "127.0.0.1"
local port = 6660
local ok, err = red:connect(ip, port)
if not ok then
ngx.say("connect to redis error : ", err)
return close_redis(red)
end
ok, err = red:set("msg", "hello world")
if not ok then
ngx.say("set msg error : ", err)
return close_redis(red)
end
--调用API获取数据
local resp, err = red:get("msg")
if not resp then
ngx.say("get msg error : ", err)
return close_reedis(red)
end
--得到的数据为空处理
if resp == ngx.null then    --不是lua里的nil
resp = '' --比如默认值
end
ngx.say("msg : ", resp)
close_redis(red)
--调用API进行处理
```

1. 连接池是每 Worker 进程的，而不是每 Server 的；
2. 当连接超过最大连接池大小时，会按照 LRU 算法回收空闲连接为新连接使用；
3. 连接池中的空闲连接出现异常时会自动被移除；
4. 连接池是通过 ip 和 port 标识的，即相同的 ip 和 port 会使用同一个连接池（即使是不同类型的客户端如 Redis、Memcached）；
5. 连接池第一次 set_keepalive 时连接池大小就确定下了，不会再变更；
6. cosocket 的连接池 http://wiki.nginx.org/HttpLuaModule#tcpsock:setkeepaliv