```python
class IOLoop(Configurable):
    """A level-triggered I/O loop.

    We use ``epoll`` (Linux) or ``kqueue`` (BSD and Mac OS X) if they
    are available, or else we fall back on select(). If you are
    implementing a system that needs to handle thousands of
    simultaneous connections, you should use a system that supports
    either ``epoll`` or ``kqueue``.

    Example usage for a simple TCP server:

    .. testcode::

        import errno
        import functools
        import tornado.ioloop
        import socket

        def connection_ready(sock, fd, events):
            while True:
                try:
                    connection, address = sock.accept()
                except socket.error as e:
                    if e.args[0] not in (errno.EWOULDBLOCK, errno.EAGAIN):
                        raise
                    return
                connection.setblocking(0)
                handle_connection(connection, address)

        if __name__ == '__main__':
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            sock.setblocking(0)
            sock.bind(("", port))
            sock.listen(128)

            io_loop = tornado.ioloop.IOLoop.current()
            callback = functools.partial(connection_ready, sock)
            io_loop.add_handler(sock.fileno(), callback, io_loop.READ)
            io_loop.start()

    .. testoutput::
       :hide:

    By default, a newly-constructed `IOLoop` becomes the thread's current
    `IOLoop`, unless there already is a current `IOLoop`. This behavior
    can be controlled with the ``make_current`` argument to the `IOLoop`
    constructor: if ``make_current=True``, the new `IOLoop` will always
    try to become current and it raises an error if there is already a
    current instance. If ``make_current=False``, the new `IOLoop` will
    not try to become current.

    In general, an `IOLoop` cannot survive a fork or be shared across
    processes in any way. When multiple processes are being used, each
    process should create its own `IOLoop`, which also implies that
    any objects which depend on the `IOLoop` (such as
    `.AsyncHTTPClient`) must also be created in the child processes.
    As a guideline, anything that starts processes (including the
    `tornado.process` and `multiprocessing` modules) should do so as
    early as possible, ideally the first thing the application does
    after loading its configuration in ``main()``.
```

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
