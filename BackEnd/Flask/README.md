1. [简书教程 有源码](http://www.jianshu.com/p/6b5eeff43360)
2. 国人翻译的 (http://docs.jinkan.org/docs/flask/index.html)
3. https://flask-script.readthedocs.io/en/latest/

4. FlaskBB是用Flask框架实现的一个轻量级的论坛社区软件，代码托管在GitHub上。
    * http://blog.kamidox.com/flaskbb-notes-1.html#_1
    * http://blog.kamidox.com/flaskbb-notes-2.html#_1

# 认证
## 扩展：Flask-HTTPAuth
For:
> 因为Restful API不保存状态，无法依赖Cookie及Session来保存用户信息，自然也无法使用Flask-Login扩展来实现用户认证。


# 邮件
1. http://xautlmx.github.io/2016-05-13-Flask%E2%80%94%E2%80%94%E7%94%B5%E5%AD%90%E9%82%AE%E4%BB%B6.html


2. http://www.pythondoc.com/flask-mega-tutorial/email.html

3.  Flask服务器部署 http://wiki.jikexueyuan.com/project/flask-guide/deploy-ways.html


# 入门
# virtualenv
`easy_install virtualenv`即可。

`virtualenv <ENVNAME>`创建名为ENVNAME的虚拟环境。

激活：

1. Win
`ENVNAME\Scripts\activate.bat`

2. Linux/Mac
`source ENVNAME/bin/activate`

然后pip安装flask。

# （动态）路由
```python:n
from flask import Flask
app = Flask(__name__)

@app.route('/')
def index():
	return '<h1>Hello World!</h1>'
	
@app.route('/user/<name>')
def user(name):
	return '<h1>Hello, %s!</h1>' % name

if __name__ == '__main__':
	app.run(debug=True)
```

浏览器打开 `127.0.0.1:5000/user/danceiny`即可见。

# 请求上下文


# 请求钩子
* before_first_request：注册一个函数，在处理第一个请求之前运行。
* before_request ：注册一个函数，在每次请求之前运行。
* after_request ：注册一个函数，如果没有未处理的异常抛出，在每次请求之后运行。
* teardown_request ：注册一个函数，即使有未处理的异常抛出，也在每次请求之后运行。

请求钩子函数和视图函数之间共享数据一般使用上下文全局变量g。

# flask-script
pip安装之。

```python
from flask_script import Manager


```


# 主机访问控制
```python
(venv) $ python hello.py runserver --host 0.0.0.0
* Running on http://0.0.0.0:5000/

```
现在，web服务器接收0.0.0.0这个网络中所有主机的访问，而不是服务器本机。


# 模板
模板是一个包含响应文本的文件，其中用占位变量表示的动态部分的具体值只在请求的上下文中才能知道。

渲染，即使用真实值替换变量得到响应字符串的过程。

Flask使用Jinja2引擎渲染模板。

过滤器：`Hello, {{ name|capitalize }}`(name将大写显示）

控制结构：
```
{% if user %}
	Hello, {{ user }}!
{% else %}
	Hello, Stranger!
{% endif %}

<ul>
	{% for comment in comments %}
		<li>{{ comment }}</li>
	{% endfor %}
</ul>
```

# 使用Flask-Bootstrap
pip安装。

初始化flask-bootstrap：
```python
from flask.ext.bootstrap import Bootstrap
#...
bootstrap = Bootstrap(app)
```
自定义错误界面：继承

# 使用Flask-Moment本地化日期和时间
pip安装。
http://momentjs.com/docs/#/displaying/
初始化：
```python
from flask.ext.moment import Moment
moment = Moment(app)
```

templates/base.html:引入moment
```
{% block scripts %}
{{ super() }}
{{ moment.include_moment() }}
{% endblock %}
```


# 表单：Flask-WTF
（http://pythonhosted.org/Flask-WTF/）

### WTForms支持的HTML标准字段：
StringField 文本字段
TextAreaField 多行文本字段
PasswordField 密码文本字段
HiddenField 隐藏文本字段
DateField 文本字段，值为 datetime.date 格式
DateTimeField 文本字段，值为 datetime.datetime 格式
IntegerField 文本字段，值为整数
DecimalField 文本字段，值为 decimal.Decimal
FloatField 文本字段，值为浮点数
BooleanField 复选框，值为 True 和 False
RadioField 一组单选框
SelectField 下拉列表
SelectMultipleField 下拉列表，可选择多个值
FileField 文件上传字段
SubmitField 表单提交按钮
FormField 把表单作为字段嵌入另一个表单
FieldList 一组指定类型的字段

### WTForms內建的验证函数：
Email 验证电子邮件地址
EqualTo 比较两个字段的值；常用于要求输入两次密码进行确认的情况
IPAddress 验证 IPv4 网络地址
Length 验证输入字符串的长度
NumberRange 验证输入的值在数字范围内
Optional 无输入值时跳过其他验证函数
Required 确保字段中有数据
Regexp 使用正则表达式验证输入值
URL 验证 URL
AnyOf 确保输入值在可选值列表中
NoneOf 确保输入值不在可选值列表中
# 重定向和用户会话


# 集成了Flask的sql数据库框架
使用数据库框架 [Flask-SQLAlchemy](http://pythonhosted.org/
Flask-SQLAlchemy/)，其包装了[SQLAlchemy](http://www.sqlalchemy.org/).

`pip install flask-sqlalchemy`.

数据库使用URL指定：
MySQL mysql://username:password@hostname/database
Postgres postgresql://username:password@hostname/database
SQLite（Unix） sqlite:////absolute/path/to/database
SQLite（Windows） sqlite:///c:/absolute/path/to/database

程序使用的数据库 URL 必须保存到 Flask 配置对象的 SQLALCHEMY_DATABASE_URI 键中。配置对象中还有一个很有用的选项，即 SQLALCHEMY_COMMIT_ON_TEARDOWN 键，将其设为 True时，每次请求结束后都会自动提交数据库中的变动。

|类型名| Python类型| 说　　明
|----|----|---|
|Integer	|int |普通整数，一般是 32 位|
|SmallInteger |int 取值范围小的整数，一般是 16 位|
|BigInteger| int 或 long |不限制精度的整数|

Float
float 浮点数
Numeric
decimal.Decimal 定点数
String
str 变长字符串
Text
str 变长字符串，对较长或不限长度的字符串做了优化
Unicode
unicode 变长 Unicode 字符串
UnicodeText
unicode 变长 Unicode 字符串，对较长或不限长度的字符串做了优化
Boolean
bool 布尔值
Date
datetime.date 日期
Time
datetime.time 时间
DateTime
datetime.datetime 日期和时间
Interval
datetime.timedelta 时间间隔
Enum
str 一组字符串
PickleType 任何 Python 对象 自动使用 Pickle 序列化
LargeBinary
str 二进制文件


### 常用的SQLAlchemy关系选项：
|选项名 |说　　明|
|---|-----|
backref |在关系的另一个模型中添加反向引用
primaryjoin |明确指定两个模型之间使用的联结条件。只在模棱两可的关系中需要指定
lazy |指定如何加载相关记录。可选值有 select （首次访问时按需加载）、 immediate （源对象加载后就加载）、 joined （加载记录，但使用联结）、 subquery （立即加载，但使用子查询），
noload |（永不加载）和 dynamic （不加载记录，但提供加载记录的查询）
uselist |如果设为 Fales ，不使用列表，而使用标量值
order_by |指定关系中记录的排序方式
secondary |指定多对多关系中关系表的名字
secondaryjoin |SQLAlchemy 无法自行决定时，指定多对多关系中的二级联结条件


### 最常使用的SQLAlchemy查询执行函数
方　法 |说　　明
----|---
all() |以列表形式返回查询的所有结果
first() |返回查询的第一个结果，如果没有结果，则返回 None
first_or_404() |返回查询的第一个结果，如果没有结果，则终止请求，返回 404 错误响应
get() |返回指定主键对应的行，如果没有对应的行，则返回 None
get_or_404() |返回指定主键对应的行，如果没找到指定的主键，则终止请求，返回 404 错误响应
count() |返回查询结果的数量
paginate()| 返回一个 Paginate 对象，它包含指定范围内的结果


### 集成Python shell
避免在shell会话中重复导入，可以配置使得flask-script的shell命令自动导入特定的对象。

```python
from flask.ext.script import Shell
def make_shell_context():
	return dict(app=app, db=db, User=User, Role=Role)
manager.add_command("shell", Shell(make_context=make_shell_context))
```

### 使用Flask-Migrate实现数据库迁移
使用数据库迁移框架是一种比较好的更新表的方法。

SQLAlchemy 的主力开发人员编写了一个迁移框架，称为 Alembic（https://alembic.readthedocs.org/en/latest/index.html）。除了直接使用 Alembic 之外，Flask 程序还可使用 Flask-Migrate（http://flask-migrate.readthedocs.org/en/latest/）扩展。这个扩展对 Alembic 做了轻量级包装，并
集成到 Flask-Script 中，所有操作都通过 Flask-Script 命令完成。

为了导出数据库迁移命令，Flask-Migrate 提供了一个 MigrateCommand 类，可附加到 Flask-
Script 的 manager 对象上。

在维护数据库迁移之前，要使用 init 子命令创建迁移仓库：
`(venv) $ python hello.py db init`


# 电子邮件
`pip install flask-mail`	
Flask-Mail SMTP服务器的配置：

配　　置 默认值 说　　明
MAIL_SERVER
localhost 电子邮件服务器的主机名或 IP 地址
MAIL_PORT 25 电子邮件服务器的端口
MAIL_USE_TLS False 启用传输层安全（Transport Layer Security，TLS）协议
MAIL_USE_SSL False 启用安全套接层（Secure Sockets Layer，SSL）协议
MAIL_USERNAME None 邮件账户的用户名
MAIL_PASSWORD None 邮件账户的密码

## 配置 Flask-Mail 使用 Gmail

```
import os
# ...
app.config['MAIL_SERVER'] = 'smtp.googlemail.com'
app.config['MAIL_PORT'] = 587
app.config['MAIL_USE_TLS'] = True
app.config['MAIL_USERNAME'] = os.environ.get('MAIL_USERNAME')
app.config['MAIL_PASSWORD'] = os.environ.get('MAIL_PASSWORD')
```

# 组织大型程序的结构

|-flasky
	|-app/
		|-templates/
		|-static/
		|-main/
			|-__init__.py
			|-errors.py
			|-forms.py
			|-views.py
		|-__init__.py
		|-email.py
		|-models.py
	|-migrations/
	|-tests/
		|-__init__.py
		|-test*.py
	|-venv/
	|-requirements.txt
	|-config.py
	|-manage.py
	
	
开发、测试和生产环境要使用不同的数据库，这样才不会彼此影响。

config.py：程序的配置

```python
import os
basedir = os.path.abspath(os.path.dirname(__file__))

class Config:
	SECRET_KEY = os.environ.get('SECRET_KEY') or 'hard to guess string'
	SQLALCHEMY_COMMIT_ON_TEARDOWN = True
	FLASKY_MAIL_SUBJECT_PREFIX = '[Flasky]'
	FLASKY_MAIL_SENDER = 'Flasky Admin <flasky@example.com>'
	FLASKY_ADMIN = os.environ.get('FLASKY_ADMIN')
	
	@staticmethod
	def init_app(app):
		pass
class DevelopmentConfig(Config):
	DEBUG = True
	MAIL_SERVER = 'smtp.googlemail.com'
	MAIL_PORT = 587
	MAIL_USE_TLS = True
	MAIL_USERNAME = os.environ.get('MAIL_USERNAME')
	MAIL_PASSWORD = os.environ.get('MAIL_PASSWORD')
	SQLALCHEMY_DATABASE_URI = os.environ.get('DEV_DATABASE_URL') or \
	'sqlite:///' + os.path.join(basedir, 'data-dev.sqlite')

class TestingConfig(Config):
	TESTING = True
	SQLALCHEMY_DATABASE_URI = os.environ.get('TEST_DATABASE_URL') or \
	'sqlite:///' + os.path.join(basedir, 'data-test.sqlite')
class ProductionConfig(Config):
	SQLALCHEMY_DATABASE_URI = os.environ.get('DATABASE_URL') or \
	'sqlite:///' + os.path.join(basedir, 'data.sqlite')
	config = {
	'development': DevelopmentConfig,
	'testing': TestingConfig,
	'production': ProductionConfig,
	'default': DevelopmentConfig
	}
```

## 工厂函数和蓝本
app/__init__.py
```python
from flask import Flask, render_template
from flask.ext.bootstrap import Bootstrap
from flask.ext.mail import Mail
from flask.ext.moment import Moment
from flask.ext.sqlalchemy import SQLAlchemy
from config import config
bootstrap = Bootstrap()
mail = Mail()
moment = Moment()
db = SQLAlchemy()

def create_app(config_name):
	app = Flask(__name__)
	app.config.from_object(config[config_name])
	config[config_name].init_app(app)
	
	bootstrap.init_app(app)
	mail.init_app(app)
	moment.init_app(app)
	db.init_app(app)
	
	# 附加路由和自定义的错误页面
	
	return app
```

Blueprint解决的问题：
>转换成程序工厂函数的操作让定义路由变复杂了。在单脚本程序中，程序实例存在于全
局作用域中，路由可以直接使用 app.route 修饰器定义。但现在程序在运行时创建，只
有调用 create_app() 之后才能使用 app.route 修饰器，这时定义路由就太晚了。和路由
一样，自定义的错误页面处理程序也面临相同的困难，因为错误页面处理程序使用 app.
errorhandler 修饰器定义。

## 需求文件
`(venv) $ pip freeze >requirements.txt`
如需创建这个虚拟环境的完全副本，可以创建一个新的虚拟环境，再：
`(venv) $ pip install -r requirements.txt`
