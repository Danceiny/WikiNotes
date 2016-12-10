1. 用户认证
========

Flask的认证扩展
---------------

-   Flask-Login：管理已登录用户的用户会话。
-   Werkzeug：计算密码散列值并进行核对。
-   itsdangerous：生成并核对加密安全令牌。

> 对生成安全密码散列值的过程感兴趣，“Salted Password Hashing - Doing it Right”（计算加盐密码散列值的正确方法，[https://crackstation.net/hashing-security.htm] 这篇文章值得一读）。

创建认证蓝本
------------

对于不同的程序功能，我们要使用不同的蓝本，这是保持代码整齐有序的好方法。

app/auth/views.py：蓝本中的路由和视图函数

```python
from flask import render_template
from . import auth
@auth.route('/login')
def login():
	return render_template('auth/login.html')
```

# 使用Flask-Login认证用户
`pip install flask-login`

要想使用 Flask-Login 扩展，程序的 User 模型必须实现几个方法：

|方　　法 |说　　明|
|-----|------|
|is_authenticated() |如果用户已经登录，必须返回 True ，否则返回 False|
|is_active() |如果允许用户登录，必须返回 True ，否则返回 False 。如果要禁用账户，可以返回 False|
|is_anonymous() |对普通用户必须返回 False|
|get_id() |必须返回用户的唯一标识符，使用 Unicode 编码字符串|

>还有一种更简单的替代方案。Flask-Login 提供了一个 UserMixin 类，其中包含这些方法的默认实现，且能满足大多数需求。


**app/__init__.py：初始化 Flask-Login**
```python
from flask.ext.login import LoginManager
login_manager = LoginManager()
login_manager.session_protection = 'strong'
login_manager.login_view = 'auth.login'
def create_app(config_name):
	# ...
	login_manager.init_app(app)
	# ...
```

>LoginManager 对象的 session_protection 属性可以设为 None 、 'basic' 或 'strong' ，以提供不同的安全等级防止用户会话遭篡改。设为 'strong' 时，Flask-Login 会记录客户端 IP地址和浏览器的用户代理信息，如果发现异动就登出用户。 


## 注册令牌
发送邮件确认注册。


## 管理账户
修改密码（git checkout 8f）、重设密码（8g）、修改电子邮件（8h）。


# 2. 用户角色

