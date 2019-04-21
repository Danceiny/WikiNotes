## 2019-03-11

DONE

- 阅读scrapyd源码，了解scrapyd的scrapy爬虫调度、运行的实现原理。
- 熟悉eggjs框架，完成ScheduleService（负责向爬虫的队列扔url从而启动爬取），并编写了基于该service的定时任务。

TODO

- 爬虫系统功能实现：基于数据库存储、带状态同步更新的线索

THINKING

- 今天决定把daily重新捡起来
- scrapyd基于Twisted框架。twisted应该算是python里非常优异的底层框架了，tornado即基于twisted开发。scrapyd直接采用了twisted构建service，整个项目非常精巧。大致结构如下图。现在我们有定时启动爬虫的需求，显然scrapyd无法满足该需求，因为其使用的异步处理队列过于简单。有时间的话想在scrapyd的基础上实现该功能，也算是对twisted的深入学习了。

![image-20190311111825653](/Users/huangzhen/WikiNotes/imgs/image-20190311111825653.png)





## 2019-03-12

DONE

TODO

THINKING

- scrapyd作为一个scrapy爬虫部署工具实在是太不称职了😒，事实上感觉与scrapy相关的库做得并不完善，比如scrapy_redis仅仅支持redis的单实例连接，再比如基于scrapyd的web-ui，基本都是由个人开发，缺少维护，很难使用。





## 2019-03-18

DONE

- 监控报警会议
  - 新的需求：
    - 自动感知已修复
    - 前端异常
    - 如何接入普罗米修斯？流程化、标准(业务指标以外的，需要哪些点)
    - 跨语言栈的接入问题

TODO

THINKING





## 2019-03-22

DONE

- parser-engine v0.0.9 更新日志
  集成了clue相关的代码，在`parser_engine.clue`包，包含以下内容：

  - ClueSpider： 整理了原BaseSpider的内容，`from parser_engine.clue.spider import ClueSpider`
  - ClueItem：即原ClueItemm， `from parser_engine.clue.items import ClueItem`
  - ClueModel: 即原ClueModel, `from parser_engine.clue.models import ClueModel`
  - CluePersistentPipeline: 将clue持久化到mysql，`parser_engine.clue.pipelines.CluePersistentPipeline`
  - CluePipeline: 将clue扔进队列， `parser_engine.clue.pipelines.CluePipeline`，强烈建议将该pipeline置于CluePersistentPipeline之后

  一般来说，我们新的spider类，只需要继承ClueSpider即可，然后开启两个clue相关的pipeline，上面列出的其他事项已经不需要关心了。settings.py关于mysql的配置（上面的CluePersistentPipeline，将会把clue持久化到该配置指定的数据库中的clue表），建议格式如下：

  ```
  MYSQL = {
    "HOST": "",
    "USER": "",
    "PORT": 3306,
    "PASSWORD": "",
    "DATABASE": ""
  }
  ```

  需要yield一个ClueItem的时候，请务必指定item的`project`、`spider`这两个属性值，按照约定，PE根据这两个值确定相应的队列名。

  具体代码可以参考 `huoche` 项目的 `kachezhijia_spider.py`及`settings.py`。



本地可以跑一个 scrapydweb的服务（可以`pip install -U scrapydweb` 安装最新版，在任意目录下直接scrapydweb运行)，可以直接部署spider、运行spider、查看日志， 主要有以下自定义配置：

```
SCRAPYD_SERVERS = [
    '172.31.1.4:30217',
    '172.31.1.4:31429',
]

SCRAPY_PROJECTS_DIR = '/Users/huangzhen/baixing/processor/'
ENABLE_LOGPARSER = False
```



## 2019-03-28

DONE

- 在开发[优卡](<http://www.china2cv.com/>)
- 微服务第一讲：

TODO

THINKING