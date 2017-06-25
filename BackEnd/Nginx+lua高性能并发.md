首先我们选择使用 OpenResty，其是由 Nginx 核心加很多第三方模块组成，其最大的亮点是默认集成了 Lua 开发环境，使得 Nginx 可以作为一个 Web Server 使用。借助于 Nginx 的事件驱动模型和非阻塞 IO，可以实现高性能的 Web 应用程序。而且 OpenResty 提供了大量组件如 Mysql、Redis、Memcached 等等，使在 Nginx上开发Web 应用更方便更简单。目前在京东如实时价格、秒杀、动态服务、单品页、列表页等都在使用Nginx+Lua 架构，其他公司如淘宝、去哪儿网等.

## 安装环境
- 创建目录 /usr/servers，以后我们把所有软件安装在此目录；
- 安装依赖（我的环境是 ubuntu，可以使用如下命令安装，其他的可以参考 openresty 安装
步骤
- 下载 `ngx_openresty-1.7.7.2.tar.gz` 并解压（ngx_openresty-1.7.7.2/bundle 目录里存放着 nginx 核心和很多第三方模块，比如有我们需要的 Lua 和 LuaJ
IT。)
- 安装 `LuaJIT`
- 下载 `ngx_cache_purge` 模块，该模块用于清理 nginx 缓存
- 下载 `nginx_upstream_check_module` 模块，该模块用于 ustream 健康检查
- 安装 `ngx_openresty`

>luajit 类似于 java 的 jit，即即时编译，lua 是一种解释语言，通过 luajit 可以
即时编译 lua 代码到机器代码，得到很好的性能；


## 配置环境
- 编辑 nginx.conf 配置文件，在 http 部分添加如下配置
```conf
#lua模块路径，多个之间”;”分隔，其中”;;”表示默认搜索路径，默认到/usr/servers/nginx下找
lua_package_path "/usr/servers/lualib/?.lua;;"; #lua 模块
lua_package_cpath "/usr/servers/lualib/?.so;;"; #c模块
```
在nginx.conf同级目录下新建lua.conf文件，
```conf
#lua.conf
server {
listen 80;
server_name _;
}
```
然后到nginx.conf中http部分`include lua.conf`.

然后测试是否正常：`nginx -t`


## Helloworld
在lua.conf中添加：
```conf
location /lua {
default_type 'text/html';
content_by_lua 'ngx.say("hello world")';
}
```
然后测试配置是否正常：`nginx -t`

重启nginx：`nginx -s reload`
访问： `127.0.0.1/lua`，看到helloworld。


通过引入外部文件——lua.conf如下配置：
```conf
location /lua {
default_type 'text/html';
content_by_lua_file conf/lua/test.lua; #相对于nginx安装目录
}
```

### `lua_code_cache`
默认情况下 lua_code_cache 是开启的，即缓存 lua 代码，即每次 lua 代码变更必须reload nginx 才生效，如果在开发阶段可以通过 lua_code_cache off;关闭缓存，这样调试时每次修改 lua 代码不需要 reload nginx；但是正式环境一定记得开启缓存。(关闭缓存会有性能警告)


>tail -f /usr/servers/nginx/logs/error.log



## 项目构建
项目结构：
```
example
    example.conf ---该项目的nginx 配置文件
    lua ---我们自己的lua代码
        test.lua
    lualib ---lua依赖库/第三方依赖
        *.lua
        *.so
```

对应的nginx.conf配置文件如下：
```conf
#user nobody;
worker_processes 2;
error_log logs/error.log;
events {
worker_connections 1024;
}
http {
include mime.types;
default_type text/html;
#lua模块路径，其中”;;”表示默认搜索路径，默认到/usr/servers/nginx下找
lua_package_path "/usr/example/lualib/?.lua;;"; #lua 模块
lua_package_cpath "/usr/example/lualib/?.so;;"; #c模块
include /usr/example/example.conf;
}
```

example.conf如下：
```conf
server {
listen 80;
server_name _;
location /lua {
default_type 'text/html';
lua_code_cache off;
content_by_lua_file /usr/example/lua/test.lua;
}
}
```


## 开发入门

### Nginx基础知识
nginx 启动、关闭、重启
http://www.cnblogs.com/derekchen/archive/2011/02/17/1957209.html
agentzh 的 Nginx 教程
http://openresty.org/download/agentzh-nginx-tutorials-zhcn.html
Nginx+Lua 入门
http://17173ops.com/2013/11/01/17173-ngx-lua-manual.shtml
nginx 配置指令的执行顺序
http://zhongfox.github.io/blog/server/2013/05/15/nginx-exec-order/
nginx 与 lua 的执行顺序和步骤说明
http://www.mrhaoting.com/?p=157
Nginx 配置文件 nginx.conf 中文详解
http://www.ha97.com/5194.html
Tengine 的 Nginx 开发从入门到精通
http://tengine.taobao.org/book/
官方文档
http://wiki.nginx.org/Configuration


### Lua基础知识
Lua 简明教程
http://coolshell.cn/articles/10739.html
lua 在线 lua 学习教程
http://book.luaer.cn/
Lua 5.1 参考手册
http://www.codingnow.com/2000/download/lua_manual.html
Lua 5.3 参考手册
http://cloudwu.github.io/lua53doc/


### [Nginx Lua API](http://wiki.nginx.org/HttpLuaModule#Nginx_API_for_Lua)
example.conf:
```conf
location ~ /lua_request/(\d+)/(\d+) {
#设置nginx变量
set $a $1;
set $b $host;
default_type "text/html";
#nginx内容处理
content_by_lua_file /usr/example/lua/test_request.lua;
#内容体处理完成后调用
echo_after_body "ngx.var.b $b";
}
```

test_request.lua
```lua
--nginx变量
local var = ngx.var
ngx.say("ngx.var.a : ", var.a, "<br/>")
ngx.say("ngx.var.b : ", var.b, "<br/>")
ngx.say("ngx.var[2] : ", var[2], "<br/>")
ngx.var.b = 2;
ngx.say("<br/>")
--请求头
local headers = ngx.req.get_headers()
--获取请求头，默认只获取前100，如果想要获取所以可以调用ngx.req.get_headers(0)；获取带中划线的请求头时请使用如 headers.user_agent 这种方式；如果一个请求头有多个值，则返回的是 table；
ngx.say("headers begin", "<br/>")
ngx.say("Host : ", headers["Host"], "<br/>")
ngx.say("user-agent : ", headers["user-agent"], "<br/>")
ngx.say("user-agent : ", headers.user_agent, "<br/>")
for k,v in pairs(headers) do
    if type(v) == "table" then
        ngx.say(k, " : ", table.concat(v, ","), "<br/>")
    else
        ngx.say(k, " : ", v, "<br/>")
    end
end
ngx.say("headers end", "<br/>")
ngx.say("<br/>")
--get请求uri参数
ngx.say("uri args begin", "<br/>")
local uri_args = ngx.req.get_uri_args()
for k, v in pairs(uri_args) do
    if type(v) == "table" then
        ngx.say(k, " : ", table.concat(v, ", "), "<br/>")
    else
        ngx.say(k, ": ", v, "<br/>")
    end
end
ngx.say("uri args end", "<br/>")
ngx.say("<br/>")
--post请求参数
ngx.req.read_body()--必须提前调用 ngx.req.read_body() 来读取 body 体
ngx.say("post args begin", "<br/>")
local post_args = ngx.req.get_post_args()
for k, v in pairs(post_args) do
    if type(v) == "table" then
    ngx.say(k, " : ", table.concat(v, ", "), "<br/>")
    else
        ngx.say(k, ": ", v, "<br/>")
    end
end
ngx.say("post args end", "<br/>")
ngx.say("<br/>")
--请求的http协议版本
ngx.say("ngx.req.http_version : ", ngx.req.http_version(), "<br/>")
--请求方法
ngx.say("ngx.req.get_method : ", ngx.req.get_method(), "<br/>")
--原始的请求头内容
ngx.say("ngx.req.raw_header : ", ngx.req.raw_header(), "<br/>")
--请求的body内容体
ngx.say("ngx.req.get_body_data() : ", ngx.req.get_body_data(), "<br/>")
ngx.say("<br/>")
```

测试：`wget --post-data 'a=1&b=2' 'http://127.0.0.1/lua_request/1/2?a=3&b=4' -O -`


### [Nginx全局内存]( http://wiki.nginx.org/HttpLuaModule#ngx.shared.DICT)
Nginx 是一个 Master 进程多个 Worker进程的工作方式，因此我们可能需要在多个 Worker 进程中共享数据，那么此时就可以使用 ngx.shared.DICT来实现全局内存共享。

1. 首先在nginx.conf的http部分分配内存大小
```conf
#共享全局变量，在所有worker间共享
lua_shared_dict shared_data 1m;
```

2. example.conf配置到的对应lua如下：
```lua
--1、获取全局共享内存变量
local shared_data = ngx.shared.shared_data
--2、获取字典值
local i = shared_data:get("i")
if not i then
i = 1
--3、惰性赋值
shared_data:set("i", i)
ngx.say("lazy set i ", i, "<br/>")
end
--递增
i = shared_data:incr("i", 1)
ngx.say("i=", i, "<br/>")
```


### [Nginx Lua 模块指令]( http://wiki.nginx.org/HttpLuaModule#Directives)
Nginx 共11个处理阶段，而相应的处理阶段是可以做插入式处理，即可插拔式架构；另外指令可以在 http、server、server if、location、location if 几个范围进行配置：

#### `init_by_lua`
每次 Nginx 重新加载配置时执行，可以用它来完成一些耗时模块的加载，或者初始化一些全局配置；在 Master进程创建 Worker 进程时，此指令中加载的全局变量会进行 Copy-OnWrite，即会复制到所有全局变量到 Worker 进程。

nginx.conf配置：
```conf
#共享全局变量，在所有worker间共享
lua_shared_dict shared_data 1m;
init_by_lua_file /usr/example/lua/init.lua;
```

init.lua:
```lua
--初始化耗时的模块
local redis = require 'resty.redis'
local cjson = require 'cjson'
--全局变量，不推荐
count = 1
--共享全局内存
local shared_data = ngx.shared.shared_data
shared_data:set("count", 1)
```

#### `init_worker_by_lua`
用于启动一些定时任务，比如心跳检查，定时拉取服务器配置等等；此处的任务是跟 Worker 进程数量有关系的，比如有2个 Worker 进程那么就会启动两个完全一样的定时任务。

ngx.timer.at：延时调用相应的回调方法；ngx.timer.at(秒单位延时，回调函数，回调函数的参数列表)；可以将延时设置为0即得到一个立即执行的任务，任务不会在当前请求中执行不会阻塞当前请求，而是在一个轻量级线程中执行.
```conf
• lua_max_pending_timers 1024; #最大等待任务数
• lua_max_running_timers 256; #最大同时运行任务数
```

#### `set_by_lua` `set_by_lua_file`
设置nginx变量。
```conf
set_by_lua_file $num /usr/example/lua/test_set_1.lua;
echo $num;
```

实际工作时经常涉及到网站改版，有时候需要新老并存，或者切一部分流量到新版,首先在 example.conf 中使用 map 指令来映射 host 到指定 nginx 变量，方便我们测试：
```conf
############ 测试时使用的动态请求
map $host $item_dynamic {
default "0";
item2014.jd.com "1";
}
```
绑定 hosts
• 192.168.1.2 item.jd.com ;
• 192.168.1.2 item2014.jd.com ;

想访问 item2014.jd.com 时访问新版，那么我们可以简单的使用如:
```conf
if ($item_dynamic = "1") {
proxy_pass http://new;
}
proxy_pass http://old;
```

但是我们想把商品编号为 8 位(比如品类为图书的)没有改版完成，需要按照相应规则跳转到老版，但是其他的到新版；虽然使用 if 指令能实现，但是比较麻烦
```conf
set jump "0";
if($item_dynamic = "1") {
set $jump "1";
}
if(uri ~ "^/6[0-9]{7}.html") {
set $jump "${jump}2";
}
\#非强制访问新版，且访问指定范围的商品
if (jump == "02") {
proxy_pass http://old;
}
proxy_pass http://new;
```

所以就可以借助lua：
```conf
set_by_lua $to_book '
local ngx_match = ngx.re.match
local var = ngx.var
local skuId = var.skuId
local r = var.item_dynamic ~= "1" and ngx.re.match(skuId, "^[0-9]{8}$")
if r then return "1" else return "0" end;
';
set_by_lua $to_mvd '
local ngx_match = ngx.re.match
local var = ngx.var
local skuId = var.skuId
local r = var.item_dynamic ~= "1" and ngx.re.match(skuId, "^[0-9]{9}$")
if r then return "1" else return "0" end;
';
#自营图书
if ($to_book) {
proxy_pass http://127.0.0.1/old_book/$skuId.html;
}
#自营音像
if ($to_mvd) {
proxy_pass http://127.0.0.1/old_mvd/$skuId.html;
}
#默认
proxy_pass http://127.0.0.1/proxy/$skuId.html;
```


#### `rewrite_by_lua` `rewrite_by_lua_file`
执行内部 URL 重写或者外部重定向，典型的如伪静态化的 URL 重写。其默认执行在 rewrite 处理阶段的最后。
lua:
```lua
if ngx.req.get_uri_args()["jump"] == "1" then
    return ngx.redirect("http://www.jd.com?jump=1", 302)
end
```
当我们请求 http://192.168.1.2/lua_rewrite_1 时发现没有跳转，而请求 http://192.168.1.2/lua_rewrite_1?jump=1 时发现跳转到京东首页了。 此处需要301/302跳转根据自己需求定义.

lua2:
```lua
if ngx.req.get_uri_args()["jump"] == "1" then
    ngx.req.set_uri("/lua_rewrite_3", false);
    ngx.req.set_uri("/lua_rewrite_4", false);
    ngx.req.set_uri_args({a = 1, b = 2});
end
```
此处两者都是 location 内部 url 重写，不会重新发起新的 location 匹配(false,若为true则发起新的location匹配)；
访问如 http://192.168.1.2/lua_rewrite_2?jump=0 时得到响应 rewrite2 uri : /lua_rewrite_2, a :


>注意，在使用 rewrite_by_lua 时，开启 rewrite_log on;后也看不到相应的 rewrite log。

#### `access_by_lua`
用于访问控制，比如我们只允许内网 ip 访问