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