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
