## 自我介绍
- 弹簧振子
- Danceiny
  - danceiny@gmail.com
  - https://github.com/Danceiny
  - http://blog.cannot.cc

# 入门级（大一）

1. 工欲善其事必先利其器
   1. Pycharm
   2. 终端的折腾
      1. Windows选手：cmd，powershell，cmder，pycharm的terminal..
   3. python环境
      1. 推荐使用conda（miniconda即可）管理多版本（python2，python3）
      2. 了解pip
      3. 使用jupyter notebook， ipython
   4. 翻墙
   5. 学会使用Google
   6. 简单了解git，github
2. 计算机网络架构：从物理层，到浏览器（可通过知乎了解）
3. 熟悉语法 + 关键字
   1. 对比学习C语言
4. 熟悉內建函数(built-in functions)，熟悉常用的一些标准库函数
5. W3cschool 了解最基础的html+css+js
6. 廖雪峰教程简单过一遍
6. 动手搭建最小的投票网站（不一定要使用数据库噢~写文件也行！）
7. 调试！通过看终端输出来debug。
   1. 打断点，看变量状态
   2. print 变量，dir(变量)， type(变量)

# 上手级（大二）

1. 使用flask或django
2. 使用restful
3. 使用http（有各种花式玩法）
4. 使用sqlalchemy，mysql或mongodb
5. 使用redis
6. 使用git管理代码
7. 动手搭建高性能、可扩展、代码整洁、逻辑清晰的投票网站（用数据库~）

# 应用级（大三）

## Python哲学

import this

## Python高级特性

https://github.com/Danceiny/WikiNotes/blob/master/Language/Python/%E5%90%8E%E7%AB%AF%E5%BC%80%E5%8F%91%E4%B8%AD%E7%9A%84Python%E9%AB%98%E6%95%88%E7%BC%96%E7%A8%8B%E6%8A%80%E5%B7%A7%E5%AE%9E%E6%88%98.md

https://github.com/Danceiny/WikiNotes/blob/master/Language/Python/%E9%AB%98%E8%B4%A8%E9%87%8F%E4%BB%A3%E7%A0%81%E7%9A%8491%E4%B8%AA%E5%BB%BA%E8%AE%AE.md

### 迭代器 && 生成器

### 线程 && 进程 && 协程 && 并发

Java的nio。 CPU >> Disk

- 协程

  - 协程是一个线程执行，那怎么利用多核CPU呢？最简单的方法是多进程+协程，既充分利用多核，又充分发挥协程的高效率，可获得极高的性能。

    Python对协程的支持是通过generator实现的。

    ```python
    def consumer():
        r = ''
        while True:
            n = yield r
            if not n:
                return
            print('[CONSUMER] Consuming %s...' % n)
            r = '200 OK'

    def produce(c):
        c.send(None)
        n = 0
        while n < 5:
            n = n + 1
            print('[PRODUCER] Producing %s...' % n)
            r = c.send(n)
            print('[PRODUCER] Consumer return: %s' % r)
        c.close()

    c = consumer()
    produce(c)
    ```

    注意到`consumer`函数是一个`generator`，把一个`consumer`传入`produce`后：

    1. 首先调用`c.send(None)`启动生成器；
    2. 然后，一旦生产了东西，通过`c.send(n)`切换到`consumer`执行；
    3. `consumer`通过`yield`拿到消息，处理，又通过`yield`把结果传回；
    4. `produce`拿到`consumer`处理的结果，继续生产下一条消息；
    5. `produce`决定不生产了，通过`c.close()`关闭`consumer`，整个过程结束。

    整个流程无锁，由一个线程执行，`produce`和`consumer`协作完成任务，所以称为“协程”，而非线程的抢占式多任务。

- 异步IO

  ```python
  loop = get_event_loop()
  while True:
      event = loop.get_event()
      process_event(event)
  ```

用asyncio提供的@asyncio.coroutine可以把一个generator标记为coroutine类型，然后在coroutine内部用yield from调用另一个coroutine实现异步操作。

为了简化并更好地标识异步IO，从Python 3.5开始引入了新的语法async和await，可以让coroutine的代码更简洁易读。

请注意，async和await是针对coroutine的新语法，要使用新的语法，只需要做两步简单的替换：

把@asyncio.coroutine替换为async；
把yield from替换为await。

```python
async def hello():
    print("Hello world!")
    r = await asyncio.sleep(1)
    print("Hello again!")
```

```python
import asyncio

@asyncio.coroutine
def hello():
    print("Hello world!")
    # 异步调用asyncio.sleep(1):
    r = yield from asyncio.sleep(1)
    print("Hello again!")

# 获取EventLoop:
loop = asyncio.get_event_loop()
# 执行coroutine
loop.run_until_complete(hello())
loop.close()
```

`@asyncio.coroutine`把一个generator标记为coroutine类型，然后，我们就把这个`coroutine`扔到`EventLoop`中执行。

`hello()`会首先打印出`Hello world!`，然后，`yield from`语法可以让我们方便地调用另一个`generator`。由于`asyncio.sleep()`也是一个`coroutine`，所以线程不会等待`asyncio.sleep()`，而是直接中断并执行下一个消息循环。当`asyncio.sleep()`返回时，线程就可以从`yield from`拿到返回值（此处是`None`），然后接着执行下一行语句。

把`asyncio.sleep(1)`看成是一个耗时1秒的IO操作，在此期间，主线程并未等待，而是去执行`EventLoop`中其他可以执行的`coroutine`了，因此可以实现并发执行。



```
import asyncio

@asyncio.coroutine
def wget(host):
    print('wget %s...' % host)
    connect = asyncio.open_connection(host, 80)
    reader, writer = yield from connect
    header = 'GET / HTTP/1.0\r\nHost: %s\r\n\r\n' % host
    writer.write(header.encode('utf-8'))
    yield from writer.drain()
    while True:
        line = yield from reader.readline()
        if line == b'\r\n':
            break
        print('%s header > %s' % (host, line.decode('utf-8').rstrip()))
    # Ignore the body, close the socket
    writer.close()

loop = asyncio.get_event_loop()
tasks = [wget(host) for host in ['www.sina.com.cn', 'www.sohu.com', 'www.163.com']]
loop.run_until_complete(asyncio.wait(tasks))
loop.close()
```

## 设计模式

- 面向对象


- 装饰器（修饰模式）

  ```python
  def fibonacci(n,cache=None):
  	if cache is None:
  		cache = {}
  	if n in cache:
  		return cache[n]
  	if n <= 1:
  		return 1
  	cache[n] = fibnacci(n-1,cache)+fibonacci(n-2,cache)
  	return cache[n]

  def memo(func):
  	cache = {}
  	def wrap(*args):
  		if args not in cache:
  			cache[args] = func(*args)
  		return cache[args]
  	return wrap
  @memo
  def fibonacci(n):
  	if n<= 1:
  		return 1
  	return fibonacci(n-1) + fibonacci(n-2)

  def fib():
      a, b = 0, 1
      while True:
          yield a
          a, b = b, a + b
  ```


```python
def auth_error_handler(auth):
    '''
    auth = HTTPBasicAuth()
    :param auth:
    :return:
    '''
    def decorate(f):
        @wraps(f)
        def wrapper(*args):
            res = f(*args)
            res = make_response(res)
            # if res.status_code == 200:
            #     # if user didn't set status code, use 401
            #     res.status_code = 401
            if 'WWW-Authenticate' not in res.headers.keys():
                res.headers['WWW-Authenticate'] = auth.authenticate_header()
            return res
        auth.auth_error_callback = wrapper
        return wrapper
    return decorate

http_basic_auth = HTTPBasicAuth()
@auth_error_handler(http_basic_auth)
def Unauthorized_Handler():
    return jsonify({"code": 403, "data":"账号或密码错误，请重试"})

@check_api_cost_time
def func():
    pass

func()

def check_api_cost_time(method):
    @wraps(method)
    def _decorator(*args, **kwargs):
        try:
            start = time.time()
            ret = method(*args, **kwargs)
            end = time.time()
            # print(method.__name__ + " api cost time %f s" % (end - start))
            file_name = method.func_code.co_filename
            line_num = method.func_code.co_firstlineno
            if args is not None and len(args) > 0 and Is_Cls(args[0]):
                # 类（实例）方法
                name = "Function: {function_name}, @Class: {class_name}, @File: {file_name}, @Line: {line_num}"\
                    .format(function_name=method.__name__,
                            class_name=args[0].__class__.__name__,
                            file_name=file_name,
                            line_num=line_num)
            else:
                name = "Function: {function_name}, @File: {file_name}"\
                    .format(function_name=method.__name__,
                            file_name=file_name,
                            line_num=line_num)
            global_loggers.debug(name + " api cost time %f s" % (end-start))
            return ret
        except Exception as e:
            global_loggers.error(repr(traceback.format_exc()))
    return _decorator
```

- 单例

  - 锁
```python
    #!/usr/bin/env python
    #-*- coding:utf-8 -*-
    # https://github.com/reyoung/singleton
    __author__ = 'reyoung'
    class Singleton(object):
        """
        The Singleton class decorator.
        Like:
            from singleton.singleton import Singleton
    
            @Singleton
            class IntSingleton(object):
                def __init__(self):
                    pass
        Use IntSingleton.instance() get the instance
        """
    
        def __init__(self, cls):
            """
            :param cls: decorator class type
            """
            self.__cls = cls
            self.__instance = None
    
        def initialize(self, *args, **kwargs):
            """
            Initialize singleton object if it has not been initialized
            :param args: class init parameters
            :param kwargs: class init parameters
            """
            if not self.is_initialized():
                self.__instance = self.__cls(*args, **kwargs)
    
        def is_initialized(self):
            """
            :return: true if instance is initialized
            """
            return self.__instance is not None
    
        def instance(self):
            """
            Get singleton instance
            :return: instance object
            """
            if not self.is_initialized():
                self.initialize()
            return self.__instance
    
        def __call__(self, *args, **kwargs):
            """
            Disable new instance of original class
            :raise TypeError:
            """
            raise TypeError("Singletons must be access by instance")
    
        def __instancecheck__(self, inst):
            """
            Helper for isinstance check
            """
            return isinstance(inst, self.__cls)
    from threading import Lock
    class ThreadSafeSingleton(object):
        def __init__(self, cls):
            self.__cls = cls
            self.__instance = None
            self.__mutex = Lock()
    
        def is_initialized(self):
            self.__mutex.acquire()
            try:
                return self.__instance is not None
            finally:
                self.__mutex.release()
    
        def initialize(self, *args, **kwargs):
            self.__mutex.acquire()
            try:
                if self.__instance is None:
                    self.__instance = self.__cls(*args, **kwargs)
            finally:
                self.__mutex.release()
    
        def instance(self):
            self.__mutex.acquire()
            try:
                if self.__instance is None:
                    self.__instance = self.__cls()
                return self.__instance
            finally:
                self.__mutex.release()
    
        def __call__(self, *args, **kwargs):
            """
            Disable new instance of original class
            :raise TypeError:
            """
            raise TypeError("Singletons must be access by instance")
    
        def __instancecheck__(self, inst):
            """
            Helper for isinstance check
            """
            return isinstance(inst, self.__cls)
```
- 观察者模式

  - 消费者-订阅者

    ```python
    # coding: utf-8

    class Publisher(object):

        def __init__(self):
            self.observers = []
        def add(self, observer):
            if observer not in self.observers:
                self.observers.append(observer)
            else:
                print("Failed to add: {}".format(observer))
        def remove(self, observer):
            try:
                self.observers.remove(observer)
            except ValueError:
                print("Failed to remove: {}".format(observer))
        def notify(self):
            [o.notify(self) for o in self.observers]

    class DefaultFormatter(Publisher):
        def __init__(self, name):
            Publisher.__init__(self)
            self.name = name
            self._data = 0
        def __str__(self):
            return "{}: '{}' has data = {}".format(type(self).__name__, self.name, self._data)
        
        @property
        def data(self):
            return self._data
        
        @data.setter
        def data(self, new_value):
            try:
                self._data = int(new_value)
            except ValueError as e:
                print("Error: {}".format(e))
            else:
                self.notify()

    class HexFormatter(object):
        def notify(self, publisher):
            print("{}: '{}' has now hex data = {}".format(type(self).__name__,
            publisher.name, hex(publisher.data)))

    class BinaryFormatter(object):
        def notify(self, publisher):
            print("{}: '{}' has now bin data = {}".format(type(self).__name__,
            publisher.name, bin(publisher.data)))

    def main():
        df = DefaultFormatter("test1")
        print(df)
        
        print()
        hf = HexFormatter()
        df.add(hf)
        df.data = 3
        print()
        print(df)    
        
        print()
        bf = BinaryFormatter()
        df.add(bf)
        df.data = 21
        print(df)

    if __name__ == '__main__':
        main()
    ```

## 深入理解Flask

自己写的学习型项目：https://github.com/Danceiny/HackGirlfriend.git

- Flask-sqlalchemy

  - base_model

    ```python
    #!/usr/bin/env python
    # -*- coding: utf-8 -*-
    import uuid
    from marshmallow import Schema, fields, post_load
    from App.extension import db
    import datetime

    class BaseSchema(Schema):
        __abstract__  = True
        id = fields.Str()
        date_created = fields.DateTime(allow_none=True)
        date_modified = fields.DateTime(allow_none=True)
        is_deleted = fields.Int()
    BaseSchema.id=1
    class BaseModel(db.Model):
        """
        Base for all model classes
        """
        __abstract__ = True
        id = db.Column(db.String(32), primary_key=True, default=lambda: uuid.uuid4().hex)
        date_created = db.Column(db.DateTime, default=lambda: datetime.datetime.utcnow())
        date_modified = db.Column(db.DateTime, default=lambda: datetime.datetime.utcnow(),
                                  onupdate=lambda: datetime.datetime.utcnow())
        is_deleted = db.Column(db.Integer, default=0)

        def to_dict(self):
            return self.schema.dump(self).data

        def delete(self):
            self.is_deleted = 1
            db.session.commit()
            return True

        @classmethod
        def from_dict(cls, dct):
            return cls.schema.load(dct).data
    ```

  - CURD

    - create
    - update
    - read
    - delete

- restart.sh

  ```bash
  #!/usr/bin/env bash
  cd /root/russell-cloud
  rm logs/*_supervisor.log

  python database_manager.py db migrate
  python database_manager.py db upgrade

  #supervisord
  supervisorctl restart uwsgi
  supervisorctl restart celery_cpu
  supervisorctl restart celery_gpu
  supervisorctl restart celery_fork
  supervisorctl restart celery_sys
  ```

- flask特有的变量和函数

  - config

  - request

  - session

  - url_for()

  - g 线程安全，threadLocal

    ​

- blueprint

  ```python
  # 实例化一个blueprint
  Admin = Blueprint("Admin", __name__,template_folder='templates',static_folder='static',static_url_path='admin/static')

  @Admin.route('/',methods=['GET'],endpoint='admin_hello_world')
  def hello_world():
      return redirect('index')

  def configure_blueprints(app):
      app.secret_key = 'jikappj39822@$*hjj'

      # 初始化数据库模型
      DBModelFactory.instance()
      app.register_blueprint(Admin, url_prefix="/")   # 总管理
      app.register_blueprint(QQBot, url_prefix="/")   # QQ机器人
      app.register_blueprint(Speak, url_prefix="/")   # 语音聊天
      app.register_blueprint(Secret, url_prefix="/")  # Secret Android API

      # zuiwan
      app.register_blueprint(ZuiwanWechat, url_prefix="/")
      app.register_blueprint(ZuiwanUser, url_prefix="/")模板
  ```

- Template

  ```python
  class Path_View(MethodView):
      view_endpoint = 'path_view'
      def get(self, p=''):
          return self._get_file_view(p, os.path.join(self.root, p))

      def _get_file_view(self, rel_path, abs_path):
          pass
      def rendor(self, path):
          return render_template('index.html',
                                 path=path,
                                 contents=getattr(self, 'contents', None),
                                 total=getattr(self, 'total', None),
                                 hide_dotfile=self.hide_dotfile,
                                 view_endpoint=self.view_endpoint,
                                 user_name=getattr(self, 'user_name', None),
                                 item_name=getattr(self, 'item_name', None),
                                 version=getattr(self, 'version', None),
                                 token=getattr(self, 'token', None)) 
  class Data_File_View(Path_View):
      def get(self, user_name, dataset_name, version=None, relative_path=None):
          return self._get_file_view(relative_path, abs_path)
  class Experiment_File_View(Path_View):
      def get(self, user_name, project_name, experiment_id, relative_path=None):
          return self._get_file_view(relative_path, abs_path)
      
  flask_app.add_url_rule('/files/data/<path:p>/', view_func=path_view = Path_View.as_view('path_view'))

  flask_app.add_url_rule('/<string:user_name>/project/<string:project_name>/<int:version>/<path:relative_path>/',view_func=Project_File_View.as_view('project_file_view'))

  flask_app.add_url_rule('/<string:user_name>/project/<string:project_name>/<int:version>/',view_func=Project_File_View.as_view('project_root_file_view'))

  flask_app.add_url_rule('/<string:user_name>/project/<string:project_name>/task/<string:experiment_id>/<path:relative_path>/',view_func=Experiment_File_View.as_view("experiment_file_view"))
  ```

- celery

  ```python
  @celery_app.task(bind=True)
  def clone_project_task(self, project_dir, step='compress'):
      imz = InMemoryZip()
      num = 0
      for root, dirs, files in os.walk(project_dir):
          for name in files:
              filepath = os.path.join(root, name)
              imz.appendFile(filepath)
              num += 1
              size = os.path.getsize(filepath)
              self.update_state(state='Started', meta={'num': num, 'size': size})
      return imz, num

  ## apply async
  check_task = checkInstance.apply_async(args=[experiment.id, task_longest_limit],
                                         queue="cpu")
  ```


  ## config celery
  ```python
celery_queues = (Queue('file', exchange='file', routing_key='task.user.file.#'),
				 Queue('cpu', exchange='user', routing_key='task.user.compute.cpu'),
				 Queue('gpu', exchange='user', routing_key='task.user.compute.gpu'),
				 Queue('sys', exchange='sys', routing_key='task.sys.#'),)
timezone = 'Asia/Shanghai'
CELERY_TASK_ALWAYS_EAGER = False
 ```

```conf
[program:celery_sys]
  command=/usr/local/bin/celery -A App.aliyun_api worker -E -n worker_sys -Q sys -B --autoscale=2,1 -s /root/russell-cloud/celerybeat-schedule

  directory=/root/russell-cloud
  autostart=true
  autorestart=true
  loglevel=debug
  stdout_logfile=/root/russell-cloud/logs/celery_sys_supervisor.log
  stderr_logfile=/root/russell-cloud/logs/celery_sys_err_supervisor.log
```

- uwsgi

  ```conf
  [program:uwsgi]
  command=/usr/local/bin/uwsgi /root/russell-cloud/uwsgi.ini
  directory=/root/russell-cloud
  autostart=true
  autorestart=true
  loglevel=debug
  stdout_logfile=/root/russell-cloud/logs/uwsgi_supervisor.log
  stderr_logfile=/root/russell-cloud/logs/uwsgi_err_supervisor.log
  ```

## MySQL数据库

- 关系表设计范式
  - 第一范式
    - **数据冗余过大** 
    - 插入异常、。。。异常
  - 第二范式
    - **2NF在1NF的基础之上，消除了非主属性对于码的部分函数依赖**。
  - 反范式化设计
    - 实际场景（例如，读>>写）

- 索引

  - 内存 ps 磁盘 
  - 唯一性索引：索引列的所有值只能出现一次且必须唯一
  - 联合索引
    - 顺序
    - B-树
    - 假设有索引（firstname, lastname, age)，则下列搜索条件使用该联合索引：
      - firstname，lastname，age
      - firstname，lastname
      - firstname

- 外键

  - 如果我们试着CREATE一台Pc，它所使用的CPU的型号是Parts表中不存在的，那么MySQL会禁止这台PC被CREATE出来。

- 事务

  - 一组sql语句
  - 分布式场景的强一致性要求
  - 回滚 A给B转账100元

- 引擎

  - InnoDB
    - 默认
  - MyISAM
    - 不支持事务

- 主从

  - 容灾备份，分担读负载

- 集群

  - proxy

- Redis

  - k-v

  - http://www.runoob.com/redis/redis-tutorial.html

  - 《Redis设计与实现》

    ​

## RussellCloud架构
- russell-cloud

  - flask，celery，mvc，uwsgi，supervisor，nginx等

- russell-fs-server

  - tornado
    - 异步IO
  - socket
    - 文件流
  - mmap
    - 文件读写的内核空间复制省略，文件-用户进程空间的映射

- russell-cli

  - python-click库

- aliyun

  - Docker

    - dockerfile

    - dockerhub

    - docker-compose

    - ├── Dockerfile
      ├── Makefile
      ├── README.md
      ├── config.yml
      ├── docker-compose.yml
      ├── requirements.txt
      ├── restart.sh
      ├── sentry.conf.py
      └── start.py

      - start.py

      ```python
      import subprocess
      cmd = ["docker-compose", "run", "--rm", "web", "config", "generate-secret-key"]
      res = subprocess.check_output(cmd, stderr=subprocess.STDOUT)
      sk = res.strip()[-50:]

      filename = 'docker-compose.yml'
      f = open(filename, 'r')
      content = f.read()
      nc = content.replace("# SENTRY_SECRET_KEY: ''", "SENTRY_SECRET_KEY: {}".format(sk))
      f.close()

      f = open(filename, 'w')
      f.write(nc)
      f.close()
      ```

      - restart.sh

      ```bash
      docker-compose stop \
      	&& docker-compose rm \
      	&& docker-compose build
      docker-compose run --rm web upgrade
      docker-compose up -d
      ```

      https://cr.console.aliyun.com/?spm=5176.2020520152.210.d103.3d5bb184Bxc5qG#/dockerImage/45333/layers/1.4.0-gpu.cuda9cudnn7-py2_aws.18

  - go

  - kafka

    - 分布式消息

  - Image-ci

    - 持续集成

- russell-cloud-web

  - vue

## CodingHub

借鉴RussellCloud，个人编程学习中心。

- https://github.com/zuiwan/CodingHub 大后端主项目
- https://github.com/zuiwan/CodingHub-Web 网站
- https://github.com/zuiwan/CodingHub-CLI 终端命令行工具

如何贡献代码：
1. 在你感兴趣的项目中提issue说明基本情况，我会邀请你加入organization-team，请查收github绑定的邮箱接受邀请。
2. 加入贡献者团队后，建立一个专属于你的branch，这将由你维护。一些比较小的开发工作，在该branch上进行。
3. 对于一些比较大规格的改动，建议fork一份到你自己的仓库，测试完成后cross-fork来提交pull request。
4. 代码能力比较强的同学承担code-review和merge-pr的工作。

参考：[RussellCloud面试](./RussellCloud_Python研发工程师实习面经.md)