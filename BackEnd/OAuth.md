# OAuth 2.0 
## 原理

## 应用场景

## 流程

## 授权模式
- 授权码模式 authorization code
- 简化模式 implicit （跳过授权码步骤）
- 密码模式 resource owner password credentials
- 客户端模式

# 授权码模式流程
1. 客户登录login页面(/client/login)，login页面将重定向至oauth页面(/oauth)，并通过url参数提供`response_type=code&client_id=%d&redirect_uri=%s`.在oauth页面，弹窗输入用户名和密码，oauth提供了用户授权提交授权服务器，授权服务器将用户重定向至重定向过来的uri(/client/passport)，并附上授权码。passport页面重定向至`    uri = 'http://localhost:5000/oauth?grant_type=authorization_code&code=%s&redirect_uri=%s&client_id=%s' % (code, redirect_uri, client_id)`。oauth页面再：
```python
if auth_code.get(int(request.args.get('code'))) == request.args.get('redirect_uri'): #判断重定向来源是否可信
    return gen_token(request.args.get('client_id'))
```
具体实现代码可见：[oauth2.0_implement.py](https://github.com/Danceiny/WikiNotes/blob/master/Others/oauth2.0_implement.py)