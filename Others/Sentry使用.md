## 目标：错误日志报警

## 手段：自建sentry-server

## 步骤

1. `git clone https://github.com/getsentry/onpremise.git`
2. customize
   1. ​
   2. REPOSITORY=registry.cn-shanghai.aliyuncs.com/russellcloud make build push
   3. Sentry.conf.py

```python
# This file is just Python, with a touch of Django which means
# you can inherit and tweak settings to your hearts content.

# For Docker, the following environment variables are supported:
#  SENTRY_POSTGRES_HOST
#  SENTRY_POSTGRES_PORT
#  SENTRY_DB_NAME
#  SENTRY_DB_USER
#  SENTRY_DB_PASSWORD
#  SENTRY_RABBITMQ_HOST
#  SENTRY_RABBITMQ_USERNAME
#  SENTRY_RABBITMQ_PASSWORD
#  SENTRY_RABBITMQ_VHOST
#  SENTRY_REDIS_HOST
#  SENTRY_REDIS_PASSWORD
#  SENTRY_REDIS_PORT
#  SENTRY_REDIS_DB
#  SENTRY_MEMCACHED_HOST
#  SENTRY_MEMCACHED_PORT
#  SENTRY_FILESTORE_DIR
#  SENTRY_SERVER_EMAIL
#  SENTRY_EMAIL_HOST
#  SENTRY_EMAIL_PORT
#  SENTRY_EMAIL_USER
#  SENTRY_EMAIL_PASSWORD
#  SENTRY_EMAIL_USE_TLS
#  SENTRY_ENABLE_EMAIL_REPLIES
#  SENTRY_SMTP_HOSTNAME
#  SENTRY_MAILGUN_API_KEY
#  SENTRY_SINGLE_ORGANIZATION
#  SENTRY_SECRET_KEY
#  GITHUB_APP_ID
#  GITHUB_API_SECRET
#  BITBUCKET_CONSUMER_KEY
#  BITBUCKET_CONSUMER_SECRET
```

```
docker-compose run --rm web config generate-secret-key
vim docker-compose.yml # SENTRY_SECRET_KEY
docker-compose run --rm web upgrade
```

```
Successfully built 5717e2b06d85

WARNING: Image for service web was built because it did not already exist. To rebuild this image you must use docker-compose build or docker-compose up --build.

5_7lxpou#c2**ky1cs_j-p45bzco55540oa7=9l)8iv1hg
```



3. 启动所有服务

   `docker-compose up -d`

4. http://ipaddress:9000



## 权限

  Sentry目前用户类型有四种： 超级管理员， 管理员，普通用户和System agents. 超级用户只能通过命令行来创建，其他用户可以自己注册或由其他用户邀请注册加入，然后由超级管理员或管理员分配项目和权限。为了更好支持团队协助以及信息安全，新版本Sentry(5.4.2)经过了重新设计，重新设计后的Sentry以Team为中心组织权限。所谓Team就是一个团队，一些用户组织在一起对某些项目有操作权限的组织。一个项目只能属于一个Team, 一个用户却可以属于多个Team, 并可在不同Team中扮演不同角色， 如用户A在Team X是管理员而在Team Y中是System agents. Sentry对用户角色的指定只能到Team级别，不能到Project级别， 所以将某个用户加入到某个Team之后，这个用户就对所有所有属于这个Team下所有project有了相同的权限。

- 超级管理员： 能创建各种用户， team和project只能由超级管理员创建。项目的一些设置比如改变Owner, 数据公开可见与否（设为public的数据可以通过url不登陆也能查看）以及客户端domain限制的设定。另外还有管理项目的api key(客户端只有得到此api key才能向Sentry发送消息)的权限等等。
- 管理员： 能创建用户， team和项目设定中除改变owner之外的权限， 可以对项目中具体数据做resolve, bookmark, public/public和remove操作。
- 普通用户： 无Team界面，只能对项目中具体数据做resolve, bookmark, public/unpublic和remove操作。
- System agents: 无Team界面，只能对项目中具体数据做bookmark, unpublic和remove操作。