- 协程的底层架构定义：https://www.python.org/dev/peps/pep-0342/
- Syntax for Delegating to a Subgenerator https://www.python.org/dev/peps/pep-0380/
- 生成器对象方法的官方文档：https://docs.python.org/3/reference/expressions.html#generator-iterator-methods

协程可以处于下面几个状态之一，当前状态可以使用inspect.getgeneratorstate(...)函数确定，该函数返回下面的字符串：

- 'GEN_CREATED'

  - 等待开始执行

- 'GEN_RUNNING' 

  - 解释器正在执行
  - 只有多线程应用中才能看到该状态

- 'GEN_SUSPENDED'

  - 在yield表达式处暂停
  - 仅当协程处于暂停状态才能调用send方法（send一个None没问题，这就和`next(gen_function_applyed)`效果一样）
  - 最先调用`next()`函数这一步，通常叫 “预激”(prime) x协程，即让协程向前执行到第一个yield表达式

- 'GEN_CLOSED'

  - 执行结束
  - ​

  ### 示例：使用协程计算移动平均值

  ```python
  def averager():
      total = 0.0
      count = 0
      average = None
      while True:
          term = yield average
          total += term
          count += 1
          average = total/count
  ```

  #### 预激协程的装饰器

  - yield from调用协程时会自动预激
  - asyncio.coroutine装饰器不会预激

  ```python
  from functools import wraps
  def coroutine(func):
      @wraps
      def primer(*args, **kwargs):
          gen = func(*args, **kwargs)
          next(gen)
          return gen
      return primer
  ```

  #### 终止协程和异常处理

  - `generator.throw(exc_type[, exc_value[, traceback]])`
  - `generator.close()`

  ```python
  class DemoException(Exception):
      '''Demo'''
      
  def demo_finally():
      print('-> coroutine started')
      try:
          while True:
              try:
                  x = yield
              except DemoException:
                  print('*** DemoException handled, continueing...')
              else:
                  print('-> coroutine received: {!r}'.format(x))
              finally:
                  print('-> coroutine ending')
  ```

  - ```python
    from collections import namedtuple
    Result = namedtuple('Result', 'count average')
    def averager():
        total = 0.0
        count = 0
        average = None
        while True:
            term = yield
            if term is None:
                break
            total += term
            count += 1
            average = total/count
        return Result(count, average)# Py3.3之前生成器返回值解释器会报语法错误
    ```

### yield from

引入yield from结构的目的是为了支持实现了`__next__`， `send`,`close`,`throw`方法的生成器。尽管也支持仅实现了`__next__`的生成器。

- 委派生成器
  - 包含yield from <iterable> 表达式的生成器函数
- 子生成器 subgenerator
  - 从yield from 表达式 `<iterable>`部分获取的生成器



用法摘要：

1. 使用yield from链接的多个协程最终必须由不是协程的调用方驱动，调用方显式或隐式（例如for循环）在最外层委派生成器上调用next()函数或者`.send()`方法。
2. 链条中最内层的子生成器必须是简单的生成器（只使用yield）或可迭代的对象。
3. 在asyncio包的API中使用yield from时，这两点都成立，还有以下细节：
   1. 协程  链条始终通过把最外层委派生成器传给asyncio包API中的某个函数

（如`loop.run_until_complete(…)`）驱动。

​	2.  协程链条最终通过yield from把职责委托给asyncio包中的某个协程函数或协程方法例如 `asyncio.sleep(..)`或者其他实现高层协议的协程，例`resp = yield from aiohttp.request('GET', url)`。