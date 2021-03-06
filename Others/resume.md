## 个人简介
- 黄振    
- 1996年出生 2018年本科毕业 会一点前端的后端
  - 优点：学习、适应、抗压能力强
- 缺点：没那么聪明、不太能长期加班、没有什么特长
- https://github.com/Danceiny
- phone: 18602170534  email:danceiny@gmail.com   wechat:tanceiny

## 求职意向
prefer

- AI/云计算相关领域的后端研发

- 上海 

- python/go/java

**对其他岗位或城市持积极开放态度^_^**

## 教育背景

|时间|学校|专业|学位|
|---|---|---|---|
|2014.9~2018.6| 华中科技大学| 电子信息工程 基于项目的信息类专业教育实验班(2+2)| 工科学士|

## 主要技术背景

| 技术栈 | 语言       | 主要框架                       | 掌握情况                                   |
| ------ | ---------- | ------------------------------ | ------------------------------------------ |
| 后端   | Java       | spring                         | 基本熟练掌握互联网常见业务场景下的开发使用 |
| 后端   | Python     | flask, tornado, celery, scrapy | 对python语言本身非常熟悉                   |
| 后端   | Php        | laravel                        | 基本熟练掌握互联网常见业务场景下的开发使用 |
| 后端   | Golang     | gin, beego                     | 学习中，已能够独立完成企业级项目           |
| 后端   | Javascript | eggjs, nuxt, koa               | 基本熟练掌握互联网常见业务场景下的开发使用 |
| 前端   | Javascript | vue                            | 能够完成较为复杂的后台管理系统             |

## 工作经历

|时间|公司|职位|内容|
|---|---|---|---|
|2018.7~至今|百姓网股份有限公司|后端开发工程师|参与“百姓店铺”、“百姓逛逛”等多条业务线的前后端开发;参与推进公司的中台Java服务化转型战略;为公司在教育方向开展新业务提供技术支持。主要完成的工作有:作为主要贡献者完成了百姓店铺PHP后端服务的Java化;作为核心开发者，使用spring框架开发了中台基础服务“字典服务”和“媒体内容服务”，使用nuxt框架开发了整站配置化工具“牧野系统管理后台”。目前，正在使用python的scrapy框架开发内部的爬虫系统。主要工作亮点：在公司举办的新兵营中担任班长；工作8个月后成功晋升。|

## 开源项目

| 名称          | 是否fork | 技术栈         | 地址                                      | 简介                                                         |
| ------------- | -------- | -------------- | ----------------------------------------- | ------------------------------------------------------------ |
| goelery       | 是       | golang         | https://github.com/Danceiny/gocelery      | Celery Distributed Task Queue in Go。原项目已不维护。实现了celery的第二版消息协议，并优化了程序启动代码。 |
| parser_engine | 否       | python，scrapy | https://github.com/Danceiny/parser_engine | A parser engine born for scrapy。配置文件驱动的自动解析器，支持JSON和HTML两种格式的响应，旨在让scrapy项目中的响应解析可配置化。 |
| go.fastjson   | 否       | golang         | https://github.com/Danceiny/go.fastjson   | fastjson in Golang。基于jsoniter开发，为从java迁过来写golang的选手提供与fastjson类似的json处理调用方式。 |



## 项目经历

|时间|项目名称|项目组|角色|内容|
|---|---|---|---|---|
|2019.02~至今|百姓网爬虫系统|教育平台研发组|核心开发者|使用scrapy框架和scrapy_redis插件开发分布式爬虫（由parser_engine提供响应解析功能）；采用scrapyd部署爬虫；采用eggjs框架开发爬虫系统的管理后台，即提供爬虫种子，并作为scrapyd的客户端。该系统由多个单应用构成，目前已落地抖音APP、快手APP、大众点评网站、天润融通网站等数据源。|
|2018.12~2019.01|百姓网主站重构|百姓网“拆镐京”项目组|核心开发者|对百姓网整站进行微服务化拆分，牧野系统包含极为核心的底层可配置化数据服务，以及基于此的后台管理系统。牧野系统的底层服务采用spring体系开发，支持高并发的层级数据查询，已在线上8核6pod环境下跑出了最高14000的QPS，并较好地兼容了旧数据，且使用简洁的ID防火墙策略有效降低了查询负载和可能的URL参数攻击；后台管理系统采用Nuxt前后端同构解决方案开发。整个牧野系统中两个核心代码仓库，个人独立完成了所有核心代码，总代码贡献量超过2/3。|
|2018.10~2018.11|百姓网媒体内容服务|百姓网中台研发组|核心开发者|基于云服务屏蔽了不同底层云服务商的差异性，为内部业务赋能音视频内容的存储、转码等。 该服务采用spring体系开发，对资源文件信息分库分表存储。结合腾讯云、七牛云等任务流的概念，设计了可配置的、兼容多家云服务商的任务处理流程，极大简化了业务方的调用。|
|2017.08~2018.05|[RussellCloud深度学习云平台（russellcloud.com）](https://russellcloud.com/)|上海慕姝信息科技有限公司|核心开发者|主要产品为RussellCloud(https://russellcloud.com)，公司前后进行了两轮融资，目前已停止服务。作为主要后端参与开发了RussellCloud项目，项目依托于阿里云的集群服务，主站项目russell-cloud使用 flask+celery+mysql+redis，具有用户管理、任务调度等职能；文件服务器russell-fs-server使用tornado+websocket，负责文件上传下载以及云端任务日志流；终端命令行工具russell-cli同样使用python实现；此外还承担了各种深度学习环境Docker镜像的制作。|
|2017.05~2017.08|[电商协会官网（cecctm.com）](https://www.cecctm.com/)|武汉魅果科技有限公司|            | 参与中国电子商务师协会官方网站开发，主要负责网站后台管理系统的全栈式开发。采用的技术栈为前端VueJS（iVIew+Element）+后端Python- Flask。 |
|2016.09~2017.03|[马上赚钱APP（imeigoo.cn）](https://www.imeigoo.cn/)|武汉魅果科技有限公司|项目组成员|参与百万用户级的“马上赚钱”主应用以及“小星星看图学英语”等插件应用的iOS+Android客户端开发。主要负责内容：“马上赚钱”app的第三方广告平台接入。|
|2016.11~2016.12|[虹膜图像的模糊抖动研究]|华中科技大学Dian团队虹膜实验室|项目组成员|为北京释码大华公司的虹膜识别技术提供图像模糊判断以及去抖方案。主要负责相关论文算法（例如Triple A）的效果验证等。|
|2015.12~2016.04|Bumble2双目立体相机预研|华中科技大学自动化学院的多功能机器人协同实验室|项目组成员|搭建Bumble 2双目立体相机在Windows下的配套开发环境。主要实现了手势识别与双目测距。|

