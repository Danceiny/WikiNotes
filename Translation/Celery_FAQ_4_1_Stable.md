---
date: 2017-11-25 01:41:16
status: public
title: Celery常见问题
keywords: 
- RabbitMQ
- Redis
- Celery
- Python
- 中间件
- 消息队列
tags: 

categories: 翻译
---

# 译文出处
http://docs.celeryproject.org/en/latest/faq.html

This document describes the current stable version of Celery (4.1).

本文长期更新地址： [Celery4.1常见问题](http://blog.cannot.cc/Celery_FAQ_4_1_Stable.html)

**术语翻译对照**

|英文|中文|
|---|---|
|celery|celery|
|worker|worker|
|queue|队列|
|message|消息|
|task|任务|

## 常规
### 应该用Celery来处理什么样的事情？
答案：[Queue everything and delight everyone](https://decafbad.com/blog/2008/07/04/queue-everything-and-delight-everyone/) ([我的译文参见]())解释了为什么你会需要在web的上下文中使用队列。

这里是一些普遍的使用案例：
- 在后台运行。例如For example, to finish the web request as soon as possible, then update the users page incrementally. This gives the user the impression of good performance and “snappiness”, even though the real work might actually take some time.
- 在web请求结束后运行
- 通过异步执行和重试，确保一些事情完成了
- 制定周期任务

以及一定程度上：
- 分布式计算
- 并行执行

## 误解
### Celery真的有50000行代码吗？
答案：没有。这个和类似的庞大数字在各种场合经常被报道。
- 核心：7141行代码
- 测试：14209行
- 后端，贡献，兼容性代码：9032行

代码行数不是有用的度量标准，因此，即便Celery真的有50K行代码，你也不能从这个数字中得到任何结论。

### Celery有很多依赖吗？
一个普遍的批评是说Celery使用了太多的依赖。这种担忧背后的原理很难想象，特别是考虑到代码复用在现代软件开发中已经作为成熟的解决复杂性的方式，而且在使用诸如pip和PyPI包管理工具后引入依赖的开销非常低——安装和维护依赖的麻烦已经成为过去式了。

一路上，Celery替换了一些依赖，现在的依赖列表如下：
#### **celery**
- kombu
Kombu是Celery生态系统的一部分，是用来发送和接收messages的库。也是使得Celery支持如此多不同的message brokers的库。Kombu也被用在OpenStack项目中，和其他许多项目中，验证了将其从Celery基础代码中分割出来的选择是有效的。

- billiard
billiard是Python多进程模块的一个分叉fork，包含了许多性能和稳定性改善。有一天这些改善最终将会被合并到Python中。

billiard也被用来处理没有多进程模块的老版本python的兼容问题。

- pytz
提供时区定义和相关工具。
#### kombu
Kombu依赖于下面的包：
- amqp

纯Python实现的amqp客户端。AMQP作为默认broker是很自然的依赖。

>>Note:
为了解决流行的配置选择的依赖，Celery定义了许多“bundle”包（捆绑安装^_^）。[详见](http://docs.celeryproject.org/en/latest/getting-started/introduction.html#bundles)

### Celery是heavy-weight，很重的吗？
Celery在内存足迹（memory footprint）和性能上造成了非常轻微的开销。但是请注意，默认配置并未在时间或空间上进行优化，[优化](http://docs.celeryproject.org/en/latest/userguide/optimizing.html#guide-optimizing)。

### Celery依赖于pickle（序列化库）吗？
答案：不，Celery可以支持任何序列化策略。
我们內建支持JSON、YAML、Pickle和msgpack。每个任务都和一种content type挂钩，因此你甚至可以一个任务用pickle，另一个用JSON。默认的序列化支持是pickle，但是从4.0版本起，是JSON。如果你需要发送复杂的Python对象作为任务参数，你可以使用pickle作为序列化格式，但是需要注意[Notes in Serializers](http://docs.celeryproject.org/en/latest/userguide/security.html#security-serializers)。

如果你需要和其他语言通信，你应该使用合适于任务的序列化格式，通常意味着不能用pickle了。
你可以设置一个全局的默认序列化器serializer，默认的serializer用于特定的任务，或者发送单个任务 instance的时候决定用什么serializer。

### Celery只支持Django吗？
答案：不。

### 我必须使用AMQP/RabbitMQ吗？
答案：不，尽管使用RabbitMQ是推荐的，你也可以使用Redis，SQS，或者Qpid。更多参见[broker](http://docs.celeryproject.org/en/latest/getting-started/brokers/index.html#brokers)

Redis作为broker表现不如AMQP，但是RabbitMQ作为broker，Redis作为结果存储的组合方式很常用。如果你有严格的可靠性要求，最好使用RabbitMQ，或者其他AMQP broker。一些transports也用轮询（polling），因此他们可能会消耗更多的资源。但是，如果你因为某些原因不能使用AMQP，可以放心使用这些替代品，在大部分场景下都能工作良好，而且以上不是为Celery量身定制的。如果你之前使用Redis/database作为队列也工作得很好，那现在也能。你一直可以到需要的时候再升级。


### Celery是多语言的吗？
答案：是。
worker是用Python实现的。如果某门语言有AMQP客户端，那用这门语言创建一个worker不需要做太多事情。一个Celery的worker只是一个连接broker来处理messages的程序。

而且，有另一种方式来做到语言独立，就是用REST的任务，这样你的任务就不是函数而是url了。有这个信息，你甚至可以创建一个简单的web服务器开启代码预加载。简单地暴露一个表现一个操作的端点endpoint，再创建一个任务，这个任务只是将一个HTTP请求表现给那个端点。


## 解决问题
### MySQL抛出死锁错误，怎么办？
答案：MySQL有默认的隔离级别设置为REPEATABLE-READ（可重复读），如果你并不真正需要它，可以设置为READ-COMMITTED（读提交）。你可以通过在my.cnf中加入：
```conf
[mysqld]
transaction-isolation = READ-COMMMITTED
```
更多有关InnoDB的事务模型，参见[MySQL - The InnoDB Transaction Model and Locking ](https://dev.mysql.com/doc/refman/5.1/en/innodb-transaction-model.html).

### worker什么都不做，hanging挂起了
答案：参见MySQL死锁，或者 Task.delay

### 任务结果返回不可靠
答案：如果你使用数据库后端存储结果，特别是MySQL，可能是死锁。参见上上个问题。

### 为什么`Task.delay/apply*`这些调用之后worker只是挂起？
答案： 一些AMQP客户端有一个bug，如果当前用户无法认证、密码不匹配或者用户没有访问指定虚拟主机的权限，就会挂起。检查broker的日志（RabbitMQ的在/var/log/rabbitmq/rabbit.log），通常会有消息描述原因。

### 兼容FreeBSD系统吗？
答案：看情况。
When using the RabbitMQ (AMQP) and Redis transports it should work out of the box.

For other transports the compatibility prefork pool is used and requires a working POSIX semaphore implementation, this is enabled in FreeBSD by default since FreeBSD 8.x. For older version of FreeBSD, you have to enable POSIX semaphores in the kernel and manually recompile billiard.

Luckily, Viktor Petersson has written a tutorial to get you started with Celery on FreeBSD here: http://www.playingwithwire.com/2009/10/how-to-get-celeryd-to-work-on-freebsd/

### 遇到了完整性错误(IntegrityError)：Duplicate Key errors，什么原因？
答案：MySQL死锁。

### 我的任务为什么没有被处理？
答案：用RabbitMQ的话，你可以通过运行如下命令看有多少个消费者当前在接收任务：
```bash
$ rabbitmqctl list_queues -p <myvhost> name messages consumers
Listing queues ...
celery     2891    2
```
以上输出表明任务-队列里有2891条messages在等待被处理，而且有两个消费者正在处理他们。

队列从未被清空的一个原因可能是你有一个过期的worker进程劫持了这些messages。如果这个worker没有被正确地杀掉，就有可能发生这种情况。

当一个message被一个worker接收到了，这个worker在标记该message被处理前会等待被应答。这个worker不会重发message给另一个消费者，直到该消费者被正确地关闭。
如果你遇到这个问题，你必须手动杀掉所有的worker并重启：
```bash
$ pkill 'celery worker'

$ # - If you don't have pkill use:
$ # ps auxww | grep 'celery worker' | awk '{print $2}' | xargs kill
```
你可能必须等一会儿，知道所有的worker都结束了正在执行的任务。如果仍然长时间挂起，你可以强制杀掉：
```bash
$ pkill -9 'celery worker'

$ # - If you don't have pkill use:
$ # ps auxww | grep 'celery worker' | awk '{print $2}' | xargs kill -9
```

### 我的任务为什么不会运行？
答案：可能有语法错误导致任务模块没有被导入。
你可以看看通过手动执行任务，Celery能不能运行该任务：
```
>>> from myapp.tasks import MyPeriodicTask
>>> MyPeriodicTask.delay()
```
观察worker的日志文件，是否可以找到该任务，或者有没有其他错误发生。

### 我的定时任务为什么不会运行？
答案：参见上一个问题。

### 我怎么清理所有的等待中任务？
答案：你可以使用`celery purge`命令来清理所有的已配置的任务队列。
```bash
$ celery -A proj purge
```
或者在代码中：
```
>>> from proj.celery import app
>>> app.control.purge()
1753
```
如果你只是想清理特定队列中的消息，你必须使用AMQP API，或者`celery amqp`的功能：
```bash
$ celery -A proj amqp queue.purge <queue name>
```
1753是被清理的消息数。

你也可以开启`--purge`选项去启动worker，worker启动的时候就会清理消息。

### 我清理了message，但是队列队列中仍然有消息残留？
答案：只要任务真的被执行了，任务就被应答（从队列中移除）了。在worker接收到一个任务之后，在真正被执行前需要一点时间，特别是如果有大量任务已经在等待执行。没有被应答的消息，会被worker保持，直到消息关闭和broker（AMQP服务器）的连接。当连接关闭时（比如，因为worker停止了），任务会被broker重发给下一个可用的worker（或者在worker重启后又发给它），因此正确地清理等待任务的队列需要停掉所有的worker，然后再用`celery.control.purge`清理任务。

## 结果
### 如果我有一个id指向一个任务，怎么得到任务结果？
答案：用`task.AsyncResult`。
```
>>> result = my_task.AsyncResult(task_id)
>>> result.get()
```
这会用任务的当前结果后端(result backend)返回一个[AsyncResult](http://docs.celeryproject.org/en/latest/reference/celery.result.html#celery.result.AsyncResult)的实例。
如果你需要指定一个自定义的结果后端，或者你想使用当前应用的默认后端，你可以使用`app.AsyncResult`:
```
>>> result = app.AsyncResult(task_id)
>>> result.get()
```

## 安全
### 使用pickle不是有安全隐患吗？
答案：事实上，自从Celery4.0起，默认的序列化器是现在的JSON，就确保了人们有意识地选择序列化器并且意识到了这一担忧。
防范未认证授权的worker、数据库和其他传输pickled数据的服务接入是必要的。
注意到这不仅仅是你应该意识到有关Celery的问题，例如Django也使用pickle作为其缓存客户端。
对任务消息，你可以设置`task_serializer`为`json`或者`yaml`，而不是pickle。
类似地可以设置`result_serializer`。

### message是否可以加密？
答案：一些AMQPworker支持使用SSL（包括RabbitMQ），你可以通过`broker_use_ssl`开启这一功能。
给消息加入额外的加密和安全性也是可能的，如果你有需求，应该联系邮件列表。

### 以root用户运行worker是安全的吗？
答案：不是！
我们现在还没有发觉任何安全问题，但是认为安全问题不存在就太天真了，因此推荐以非特权用户运行Celery服务（celery woker， celery beat， celeryev等）。

## Brokers
### 为什么RabbitMQ崩溃了？
答案：RabbitMQ如果用光内存就会崩溃。未来版本的RabbitMQ会修复这一个问题。https://www.rabbitmq.com/faq.html#node-runs-out-of-memory

>>注意：
这已经不再是问题，RabbitMQ2.0+包含了一个新的固件，对内存不足错误是容忍的。因此推荐RabbitMQ2.1+版本配合Celery使用。如果你还在使用老版本，而且还遇到崩溃问题，赶紧升级吧。

Celery的错误配置最终也会导致老版本RabbitMQ的崩溃。即便不崩溃，也会消耗大量的资源，因此意识到这一普遍陷阱很重要。

- 事件Events
加上`-E`选项运行worker将会在worker内部事件发生时发送消息。
事件应该只在你有一个活跃的监控器消费事件的时候才被开启，否则你需要定期清理事件队列。

- AMQP 后端结果
在使用AMQP结果后端运行的时候，每个任务结果都会作为消息发送。如果你不收集collect这些结果，他们会积累，RabbitMQ最终会耗尽内存。
结果后端现在被弃用了，所以你不应该再使用。如果你需要多个消费者访问结果，可以用RPC后端来做rpc风格的调用，或者一个持久化的后端。
默认情况下结果在一天后失效。可以通过配置`result_expires`来降低这个有效期。

如果你不需要任务结果，确保你设置了`ignore_result`选项。
```python
@app.task(ignore_result=True)
def mytask():
    pass

class MyTask(Task):
    ignore_result = True
```

### Celery可以和ActiveMQ/STOMP一起使用吗？
答案：不能。

### 不使用AMQP broker时，哪些特征不支持了？
不完全列表：
- 远程控制命令（仅由Redis支持）
- 事件监控在所有的虚拟传输中可能不会工作
- header和fanout（扇出，Redis支持） exchange types


## 任务
### 调用tasks时如何复用连接？
答案：查看`broker_pool_limit`设置。v2.5+就默认开启连接池了。

### 子进程中sudo反回了None
有一个sudo的配置选项来使得不经过tty运行sudo的处理是非法的。
```
Defaults requiretty
```
如果你在`/etc/sudoers`文件中有这项配置，那worker作为守护进程跑的时候，任务将不能调用sudo。如果你想开启，你需要移除上面那一行。[http://timelordz.com/wiki/Apache_Sudo_Commands](http://timelordz.com/wiki/Apache_Sudo_Commands)


### 为什么workers不能处理任务的时候还能将任务从队列中删除？
答案：worker拒绝未知任务、错误编码的消息、不包含正确域field的消息（按照任务消息协议）。
如果不拒绝这些，会导致重复传送，引发死循环。
最近版本的RabbitMQ有能力配置一个dead-letter队列来交换，所以那些被拒的消息就被转移到了那里。

### 我可以通过任务名称调用任务吗？
答案：是的，用`app.send_task`。
你也可以在任何语言中使用AMQP客户端通过名字调用一个任务：
```
>>> app.send_task('tasks.add', args=[2, 2], kwargs={})
<AsyncResult: 373550e8-b9a0-4666-bc61-ace01fa4f91d>
```

### 我能设置当前任务的id吗？
答案：是的，当前id以及更多内容在任务请求里都是可用的。
```python
@app.task(bind=True)
def mytask(self):
    cache.set(self.request.id, "Running")
```
[Task Request](http://docs.celeryproject.org/en/latest/userguide/tasks.html#task-request-info)
如果你没有任务实例的引用，你可以使用`app.current_task`：
```
>>> app.current_task.request.id
```
但是需要注意，这可能是任何任务，一个被worker执行的任务，或者一个直接被任务调用的任务，或者一个急切eager调用的任务。（此处原文：But note that this will be any task, be it one executed by the worker, or a task called directly by that task, or a task called eagerly.）

可以用`current_worker_task`得到特定的当前被执行的任务：
```
>>> app.current_worker_task.request.id
```

需要注意`current_task`, 和 `current_worker_task` 可能是None。


### 我如何指定一个自定义的task_id？
答案：能。
```
>>> task.apply_async(args, kwargs, task_id='…')
```

### 任务上可以使用装饰器吗？
答案：能，但需要注意 [Basics的侧边栏](http://docs.celeryproject.org/en/latest/userguide/tasks.html#task-basics)

### 我能使用自然数作为task ids吗？
答案：能。但是确保其是唯一的，因为两个相同id的任务的行为是未定义的。

### 我能指定，一旦另一个task结束，马上运行一个task吗？
答案：能。你可以在一个任务里面安全地启动一个任务。一个常用的模式是给任务加上回调：
```python
from celery.utils.log import get_task_logger

logger = get_task_logger(__name__)

@app.task
def add(x, y):
    return x + y

@app.task(ignore_result=True)
def log_result(result):
    logger.info("log_result got: %r", result)
```
调用：
```
>>> (add.s(2, 2) | log_result.s()).delay()
```
获取更多信息：[Canvas: Designing Work-flows](http://docs.celeryproject.org/en/latest/userguide/canvas.html)

### 我能取消任务的执行吗？
答案：能。用`result.revoke()`:
```
>>> result = add.apply_async(args=[2, 2], countdown=120)
>>> result.revoke()
```
或者，只有任务id时：
```
>>> from proj.celery import app
>>> app.control.revoke(task_id)
```
后者也支持传入任务id列表作为参数。

### 为什么我的远程控制命令被所有的workers接收到了？
答案：为了接收到广播的远程控制命令，每一个worker节点基于其节点名创建了一个唯一的队列名。如果你有超过一个worker的主机名相同，控制命令将会在他们间循环接收。
为解决这个问题，你可以用`-n`参数显式地为每个worker设置节点名：
```
$ celery -A proj worker -n worker1@%h
$ celery -A proj worker -n worker2@%h
```
这里`%h`扩展成当前主机名。

### 我能发送一些任务到限定的一些服务器上吗？
答案：是的。你可以使用不同的消息路由拓扑，将任务路由到一个或多个worker上，而且一个worker实例可以绑定到多个队列。
[Routing Tasks](http://docs.celeryproject.org/en/latest/userguide/routing.html)

### 我能禁掉任务的预取prefetching吗？
答案：可能！AMQP的属于prefetch令人疑惑，因为它只被用来描述任务预取限制(task prefetching limit)。没有涉及实际的预取。
禁掉预取限制是可能的，但是那意味着worker会消费尽可能快地消费尽可能多的任务。
[一个有关预取限制的讨论，和worker的配置设定:同一时间只预定一个任务](http://docs.celeryproject.org/en/latest/userguide/optimizing.html#optimizing-prefetch-limit)

### 我可以在运行时改变周期任务的间隔时间吗？
答案：可以。你可以使用Django的数据库调度器，或者你可以创建一个新的调度子类，覆写`is_due()`:
```python
from celery.schedules import schedule

class my_schedule(schedule):

    def is_due(self, last_run_at):
        return run_now, next_time_to_check
```
### Celery支持task优先级吗？
答案：是的， RabbitMQv3.5.0+就支持优先级，Redis传输仿真实现了优先级支持。
你也可以通过将高优先级任务路由到不同的worker中，从而把工作优先级排好。在真实世界中，这通常比每一个消息的优先级更为奏效。你可以使用速率限制（rate limiting）和单条消息优先级（per message priorities）的组合来实现响应式的系统。

### 我应该使用重试retry还是acks_late？
答案：看情况。用一个或者另一个都不是必要的，你可能想要使用两个。
`Task.retry`用来重试任务，这是可以用`try`语句catch到可预知的错误（expected errors）的。AMQP事务不是用来处理这些错误的：如果任务引发了异常，仍然会被应答！

如果某些原因worker在执行中挂掉了，你需要任务再次执行时，可以使用`acks_late`设置。没人知道worker挂掉了，这很重要，如果知道worker挂掉，通常有不可恢复的错误，需要人工介入（worker或者任务代码的bug）。
理想情况下，你可以安全地重试任何失败的任务，但是有少数情况例外，假设有如下任务：
```python
def process_upload(filename, tmpfile):
    # Increment a file count stored in a database
    increment_file_counter()
    add_file_metadata_to_db(filename, tmpfile)
    copy_file_to_destination(filename, tmpfile)
```
如果它在拷贝文件时挂掉了，我们会知道这里有个未完成状态存在。这不是个严格的教学场景，但你大概可以想象一些更为灾难性的场景。目前为止，编程较少的话则可靠性更弱。默认值是好的，需要它并且知道它们在干什么的用户仍然能开启`acks_late`（未来希望使用手动应答）。
此外，`Task.retry`在AMQP事务中有不可用的特性：在重试中延迟，最大重试次数等。
因此，可以对Python中的错误使用重试，如果你的任务是幂等的而且要求可靠性级别，结合`acks_late`一起使用。

### 我可以计划让tasks在特定的时间执行吗？
答案：可以。使用`Task.apply_async()`的eta参数。[周期任务](http://docs.celeryproject.org/en/latest/userguide/periodic-tasks.html#guide-beat)

### 我可以安全地关闭worker吗？
答案：是的，使用`TERM`信号。这会告诉worker去结束所有当前执行的作业，然后尽可能快地关闭。只要完全关闭，实验性的传输中也不应该会有任务丢失。
你决不应该通过`KILL`信号(`kill -9`)来停止worker，除非你试过几次`TERM`等了几分钟看有没有关闭。

另外，确保你只是杀掉了worker的主进程，而不是它的任何子进程。如果你知道关闭worker所依赖的一个子进程正在执行一个任务，你可以给这个特定的子进程指定一个杀死信号（kill signal），这也意味着任务会被设定一个WorkerLostError状态，因此这个任务不会再执行了。

如果你安装了`setproctitle`模块，指定进程类型很容易。
```bash
$ pip install setproctitle
```
安装这个库，你可以看到在ps命令的列表中看到进程类型，但是worker必须重启才能生效。

[停止worker](http://docs.celeryproject.org/en/latest/userguide/workers.html#worker-stopping)

### 我可以在平台的后端运行worker吗？
Answer: Yes, please see [Daemonization[(http://docs.celeryproject.org/en/latest/userguide/daemonizing.html#daemonizing.

## Django
### django-celery-beat创建的数据库表有什么目的？
用到数据库后端定时器（database-backend schedule）的时候，从PeriodicTask的数据模型中取出周期任务计划有一些其他的辅助表 (IntervalSchedule, CrontabSchedule, PeriodicTasks)。

### django-celery-results创建的数据库表有什么目的？
Django的数据库结果后端扩展需要两个额外的数据模型：TaskResult and GroupResult.

## Windows
### Celery支持Windows吗？
答案：不。4.x版本以上就不支持Windows了。


# 欢迎扫码加群交流
![](http://opkk27k9n.bkt.clouddn.com/17-11-26/19293813.jpg)

#### 常见问题目录
- 常规
	- 应该用Celery来处理什么样的事情？
- 一些误解
	- Celery真的有50000行代码吗？
	- Celery有很多依赖吗？
		- celery
		- kombu
	- Celery是heavy-weight，很重的吗？
	- Celery依赖于pickle（序列化库）吗？
	- Celery只支持Django吗？
	- 我必须使用AMQP/RabbitMQ吗？
	- Celery是多语言的吗？
- 解决问题
	- MySQL抛出死锁错误，怎么办？
	- worker什么都不做，hanging挂起了
	- 任务结果返回不可靠
	- 为什么`task.delay/apply*`这些调用之后worker只是挂起？
	- 兼容FreeBSD系统吗？
	- 遇到了完整性错误(IntegrityError)：Duplicate Key errors，什么原因？
	- 我的任务为什么没有被处理？
	- 我的任务为什么不会运行？
	- 我的定时任务为什么不会运行？
	- 我应该清理所有的等待中任务吗？
	- 我怎么清理所有的等待中任务？
	- 我清理了消息，但是队列中仍然有消息残留？
- 结果
	- 如果我有一个id指向一个任务，怎么得到任务结果？
- 安全
	- 使用pickle不是有安全隐患吗？
	- message是否可以加密？
	- 以root用户运行worker是安全的吗？
- Brokers
	- 为什么RabbitMQ崩溃了？
	- Celery可以和ActiveMQ/STOMP一起使用吗？
	- 不使用AMQP broker时，哪些特征不支持了？
- Tasks
	- 调用任务时如何复用连接？
	- 子进程中sudo反回了None
	- 为什么workers不能处理任务的时候还能将任务从队列中删除？
	- 我可以通过任务名称调用任务吗？
	- 我如何获得当前任务的id？
	- 我如何指定一个自定义的任务_id？
	- 任务上可以使用装饰器吗？
	- 我能使用自然数作为任务 ids吗？
	- 我能指定，一旦另一个任务结束，马上运行一个任务吗？
	- 我能取消任务的执行吗？
	- 为什么我的远程控制命令被所有的workers接收到了？
	- 我能发送一些任务到限定的一些服务器上吗？
	- 我能禁掉任务的预取prefetching吗？
	- Celery支持任务优先级吗？
	- 我应该使用重试retry还是acks_late？
	- 我可以计划让任务在特定的时间执行吗？
	- 我可以安全地关闭worker吗？
	- 我可以在平台的后端运行worker吗？
- Django
	- django-celery-beat创建的数据库表有什么目的？
	- django-celery-results创建的数据库表有什么目的？
- Windows
	- Celery支持Windows吗？