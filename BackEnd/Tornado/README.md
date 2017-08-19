
## 异步装饰器
```python
@tornado.web.asynchronous
def get(self):
	client = tornado.httpclient.AsyncHTTPClient()
	client.fetch("http://cannot.cc/",callback=self.deal)
def deal(self,response):
	#  dosomething...
```


## Tornado的长轮询以及websocket
js不停地向服务器去请求（长轮询），对服务器的压力较小。h5实现了websocket。`tornado.websocket.WebSocketHandler`。 
tronado中实现二者的区别在于回调。

## epoll
```
# poll
while True:
	for i in stream:
		dosomething

# select
while True:
	select(stream)
	for i in stream:
		dosomething

# event poll
while True:
	active_stream = epoll_wait(stream)
	for i in active_stream:
		dosomething
```

```c
int epoll_create(int size)
int epoll_ctl()
int epoll_wait()
```

tornado 对事件的算法处理：堆，取出时间最短的回调。


# Tornado Server
将tornado作为服务器，而不是web框架，来部署DJando等web框架应用。

```python
_here = os.path.dirname(os.path.abspath(__file__))
sys.path.append(_here)
sys.path.append(os.path.join(_here, ".."))
sys.path.append(os.path.join(_here, "../contrib"))

os.environ['DJANGO_SETTINGS_MODULE"] = "settings"

def main(port):
	wsgi_app = tornado.wsgi.WSGIContainer(
		django.core.handlers.wsgi.WSGIHandler())
	tornado_app = tornado.web.Application(
		[('.*', tornado.web.FallbackHandler, dict(fallback=wsgi_app))]
		)
	server = tornado.httpserver.HTTPServer(tornado_app)
	server.listen(port)
	tornado.ioloop.IOLoop.instance.start()

main(int(sys.argv[1]))
```


## RequestHandler && Application 源码分析
