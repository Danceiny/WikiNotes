# 流程
1. 环境配置 & 基础知识
- 正则表达式
- 深度优先广度优先遍历算法
- url去重策略

2. scrapy爬取真实数据
- spider
- item
- item loader
- pipeline
- feed export
- CrawlSpider

3. scrapy突破反爬技术
- 图片验证码
- ip访问频率限制
- user-agent随机切换

4. scrapy进阶
- scrapy原理
- 基于scrapy的中间件开发
![](http://opkk27k9n.bkt.clouddn.com/17-8-12/79037700.jpg)

**User-agent的处理**：
写到downloadmiddleware里，成为全局的，不用写到每个spider里去。覆盖`class RandomUserAgentMiddleware(object): dclassmethod def from_crawler`方法。BTW,classmethod是绑定类到类方法，而普通的是绑定方法到类的实例，staticmethod就只是方法。推荐开源的fake-useragent.

**ip代理**：

- 动态网站的抓取处理
- 将selenium和phantomjs集成到scrapy中
- scrapy log配置
- email发送
- scrapy信号	

5. scrapy redis 分布式爬虫
- 集成
request队列集中管理（单机爬虫是放在内存里） + 去重集中管理

6. elasticsearch django实现搜索引擎