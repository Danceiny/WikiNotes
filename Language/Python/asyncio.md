asyncio在Python2的移植版本：

http://trollius.readthedocs.org/



- https://docs.python.org/3/library/asyncio-dev.html
- http://asyncio.org/
- https://github.com/aio-libs
- http://tornado.readthedocs.org/en/latest/asyncio.html

## Websockets

- http://aaugustin.github.io/websockets/
- http://autobahn.ws/python/

```python
def handle(id):
    subject = get_subject_from_db(id)
    buyinfo = get_buyinfo(id)
    change = process(subject, buyinfo)
    notify_change(change)
    flush_cache(id)
    
    
import asyncio
async def handle(id):
    subject = asyncio.ensure_future(get_subject_from_db(id))
    buyinfo = asyncio.ensure_future(get_buyinfo(id))
    results = await asyncio.gather(subject, buyinfo)
    change = await process(results)
    await notify_change(change) 
    loop.call_soon(flush_cache, id)
```

1. 包装成Future对象。上面使用了ensure_future来做，上篇也说过，也可以用loop.create_task。如果你看的是老文章可能会出现asyncio.async这种用法，它现在已经被弃用了。如果你已经非常熟悉，你也可以直接使用asyncio.Task(get_subject_from_db(id))这样的方式。
2. 回调。上面用到了call_soon这种回调。除此之外还有如下两种：
   1. loop.call_later(delay, func, *args)。延迟delay秒之后再执行。
   2. loop.call_at(when, func, *args)。 某个时刻才执行。



http://www.dongwm.com/archives/%E4%BD%BF%E7%94%A8Python%E8%BF%9B%E8%A1%8C%E5%B9%B6%E5%8F%91%E7%BC%96%E7%A8%8B-asyncio%E7%AF%87-%E4%B8%89/\





> 在这个脚本的线程版中，save_flag函数会阻塞download_one函数的线程，但是阻塞的只是众多工作线程中的一个。阻塞型I/O调用在背后会释放GIL，因此另一个线程可以继续。但是在flags2_aysncio.py脚本中，save_flag函数阻塞了客户代码与asyncio事件循环共用的唯一线程，因此保存文件时整个应用程序都会冻结。

这个问题的解决办法：使用事件循环对象的run_in_executor方法。



如果需要协调异步请求，而不只是发起完全独立的请求，协程较之回调的好处会显得显而易见。



Javascript的回调地狱：嵌套匿名函数

Js： 所有本地的上下文都会丢失，需要依靠闭包

```js
api_call1(request1, function (response1){
  // 第一步
  var request2 = step1(response1);
  
  api_call2(request2, function(response2){
    //第二步
    var requst3 = step2(response2);
    api_call3(request3, function(response3){
      //第三步
      step3(response3);
    })
  })
})
```



Python：

````python
@asyncio.coroutine
def three_stages(request1):
    response1 = yield from api_call1(request1)
    # 第一步
    request2 = step1(repsonse1)
    response2 = yield from api_call2(request2)
    # 第二步
    request3 = step2(repsonse1)
    response3 = yield from api_call3(request2)
    # 第三步
    step3(response3)
loop.create_task(three_stages(request1)) #必须显式调度执行
````



关于异常：在基于回调的API中，解决方法是为每个异步调用注册两个回调，一个处理正常，一个处理异常。