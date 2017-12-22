## 自我介绍
“半吊子”后端程序员。学过N门语言。包括但不限于C,C++,HTML,CSS,Javascript,PHP,C#,Python,Java,Scala,Lua,Objective-C,Verilog, Go,Lisp,Shell,Markdown。
项目七零八落：
做过视觉方面的，
- 自动化学院机器人实验室的双目立体相机预研项目，Dian团队与释码大华公司合作的虹膜算法组的项目。
  做过创业公司方面的，
- 魅果科技的马上赚钱APP客户端（不想打杂，被老板嫌弃了）
- 魅果科技的承包项目中国电子商务师协会官网后台（第一个非demo的后端项目）
- 慕姝（上海）信息科技有限公司的RussellCloud项目后端，从0.5到1.0的后端开发。公司和Dian团队达成合作，成立了项目组。

## Python简介

import this

## Python高级特性

https://github.com/Danceiny/WikiNotes/blob/master/Language/Python/%E5%90%8E%E7%AB%AF%E5%BC%80%E5%8F%91%E4%B8%AD%E7%9A%84Python%E9%AB%98%E6%95%88%E7%BC%96%E7%A8%8B%E6%8A%80%E5%B7%A7%E5%AE%9E%E6%88%98.md

https://github.com/Danceiny/WikiNotes/blob/master/Language/Python/%E9%AB%98%E8%B4%A8%E9%87%8F%E4%BB%A3%E7%A0%81%E7%9A%8491%E4%B8%AA%E5%BB%BA%E8%AE%AE.md

### 迭代器 && 生成器

### 线程 && 进程 && 协程 && 并发
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

  ​

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



### 单例

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

    ​

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

  ​

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


  ## config celery
  celery_queues = (
      Queue('file', exchange='file', routing_key='task.user.file.#'),
      Queue('cpu', exchange='user', routing_key='task.user.compute.cpu'),
      Queue('gpu', exchange='user', routing_key='task.user.compute.gpu'),
      Queue('sys', exchange='sys', routing_key='task.sys.#'),
  )

  # timezone = 'Asia/Shanghai'
  CELERY_TASK_ALWAYS_EAGER = False

  ```

  ​

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

#### MySQL数据库

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

      ​

  - go

  - kafka

    - 分布式消息

  - Image-ci

    - 持续集成

- russell-cloud-web

  - vue



参考：[RussellCloud面试](./RussellCloud_Python研发工程师实习面经.md)