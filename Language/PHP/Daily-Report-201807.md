## 2018-07-04

DONE

- 36分钟完成默写miniChaoge
- 本地安装php开发环境
- 了解laravel及php项目基本架构

TODO

- 了解laravel各常用组件的基本功能和用法，并尝试实现一些测试代码
- 熟悉laravel以及php“前后端共存”的开发方式
- 阅读haojing的wiki文档

THINKING

- 生产环境的php环境以及php框架比较复杂，初学者还是要抽丝剥茧，只关注当前需要关心的最核心的部分。比如需要了解laravel的路由处理方式，可以暂不care版本、部署、数据库、artisan，也不care中间件，即可清晰地看到laravel的web/api分离设计，以及view的作用位置；然后再来了解和路由相关的中间件，可以看到laravel的中间件归属于http内核，以”中间件名称-中间件类“的键值对的形式存在，且支持分组，以及自定义中间件的两个入参 “上文”和"下文"和其他额外参数。
- 尝试在一个middleware文件中创建两个中间件类（同一名称空间），在Kernel.php中似乎是成功配置了，但实际使用仍然报错，报错信息 ”Class App\Http\Middleware\mwdemo_with_param2 does not exist“，看调用栈是反射时异常报错。这种写法不太规范，在网上没有找到有关解答，但还是想知道原理是什么？



## 2018-07-05

DONE

- 过了一遍laravel的文档和《Laravel框架关键技术解析-陈昊》。
- 第一次给BXApplets提了pr

TODO

- 继续熟悉文档和php的高级特性
- 给BXApplets画一个框架思维导图，以熟悉项目

THINKING

- 纯看文档很容易睡着，看一段时间可以直接看代码、敲代码来提神。
- 这次通过结合看Laravel文档和代码以及实际跑代码，对一些设计模式有了直观的理解，比纯粹看设计模式效果要好一些。



## 2018-07-06

DONE

- 熟悉weapp业务。
- 熟悉mongodb和mysql表结构。试写了一个api接口查询mongodb，熟悉mongodb及在laravel中的增删改查基本用法。
- 第一次听技术分享。

TODO

- 参与 店铺搜索&后台菜单 的需求开发

THINKING

- 技术分享是Java Spring主题，涉及Bean和AOP，联想到最近学习laravel的IOC容器，有非常多相通之处。自己之前还是更多关注计算机体系架构方面，对语言生态层面的一些思想、概念和实现非常生疏，感觉二者之间话语体系也有所不同，有机会要买本相关书籍学习一下，追求融会贯通。



## 2018-07-09

DONE

- 通过laravel配置文件配置桌面端和手机端的菜单栏

TODO

- 店铺搜索的API接口

THINKING

- 今天进入实际编码阶段，在着手blade模板和js代码的融合时感觉非常棘手，对于blade和js都缺乏把握性，因此充满了uncertainty，调试很慢。希望可以慢慢习惯这种编码方式，练好js。
- 因为代码规范的问题耽误了一些时间，有一些编码习惯和IDE使用习惯需要适应。
- 感觉在php的controller里面硬编码的各种参数非常多，而且掺杂了非常多业务逻辑，需要多一点注释比较易读。



## 2018-07-10

DONE

- 店铺搜索API和搜索热词API，实现模糊搜索、搜索结果缓存。

TODO

- 和前端对接上述API并完善
- 学习zeptojs，尝试解决菜单全覆盖task的前端显示和跳转问题

THINKING

- 要有挑战前端的勇气

## 2018-07-11

DONE

- 菜单全覆盖task的h5页面菜单点击对话框以及其他代码优化

TODO

- 桌面端（manage）页面的对话框不显示，待解决
- 对话框的样式

THINKING

- 暂无



- showConfirmDialog 弹窗为什么不显示
- 弹窗显示二维码
- php注解：php-doc
- Java注解： 内置
  - @SafeVarargs
  - @Functionallneterface java8
  - 元注解
    - @Target： ElementType 常用
    - @Rentention： RetentionPolicy 常用
    - @Documented

## 2018-07-12

DONE

- 基本完成菜单全覆盖的task的开发，已提交pr

TODO

- 上述task的测试和修复跟进
- 店铺搜索接口的前端跟进

THINKING

- 今天的开发主要集中在前端部分，尤其是涉及很多css，之前写前端的水平基本停留在简单调用现成的vue组件和库函数，现在的小程序项目对我来说约等于原生js，碰到了很多问题，通过请教组内伙伴，我学习了很多调试开发技巧。相对于后端代码，感觉前端代码的繁杂特性更加需要团队约束和配合，更加需要避免重复造轮子，尽量追求组件化。

## 2018-07-13

DONE

- 提测菜单全覆盖，修复若干bug
- 根据pr反馈重构店铺搜索API

TODO

- 菜单全覆盖task的一个ui问题
- 菜单全覆盖task的bug修复确认

THINKING

- 今天的pr反馈意见非常重要，受益匪浅。同时第一次收到测试反馈，条目清晰，需要向测试学习，尽量实践TDD。



## 2018-07-16

DONE

- 菜单全覆盖task的成果演示及相关bug和冲突修复
- 接触/guandu
- 拆分“添加微信个人号”task的后端实现需求，

TODO

- 完成“添加微信个人号”task的后端开发和接口自测

THINKING

- bug返修情况有点严重，须加强
- 代码方面的团队沟通须加强
- 多分支同步开发，对于一些共用逻辑的修改，需要尤其慎重，须提前确定方案并知会相关开发人员，以便减少合并冲突



## 2018-07-17

DONE

- 菜单全覆盖task的UI问题（新的设计规范）跟进和修复，以及菜单显示、跳转逻辑的调整
- 实现微信号和微信二维码（uri）的修改API，加入了判别是否为二维码图片的处理

TODO

- 跟yp确定微信个人号task的前后端联合解决方案
- 联调店铺搜索
- 跟进微信个人号运营后台task

THINKING

- 和设计沟通重在罗列“点”，需要熟悉对方的专业沟通语言，不能遗漏
- 和产品沟通重在罗列“线”，知其逻辑和路径，这样相对而言改代码会比较快



## 2018-07-18

DONE

- AOP面向切面编程技术分享
- 菜单全覆盖的bug修复和UI细化
- 商户引导用户添加微信个人号运营后台，完成度90%
- 联调店铺搜索

TODO

- 联调微信个人号，完善运行后台

THINKING

- 又开始写Vue了，既熟悉又陌生，好处是代码干净了不少，基本通过看现有代码上手，不过现有的还是有些看不懂，一些关键的地方以及和后端的接口约定缺少注释（看代码还是比看文档效率低）。

## 2018-07-19

DONE

- 店铺搜索API的bug：前缀匹配和后缀匹配，状态上线筛选；以及打点
- 初识haojing

TODO

- 把haojing跑起来（解决类找不到）
- 在haojing中实现一个用户广告查询的API，支持筛选。

THIKING

- 看haojing的代码还是感觉比较吃力，一个是ayana的非常多的类装载和容器应用实现（和laravel有非常多相似之处，但似乎更轻量级），一个是数据库方面graph的映射关系还没有理得很清楚，一个是haojing的localservice和remoteService的关系，一个是haojing没有跑起来所以还不知道如何调试。



## 2018-07-20

DONE

- 店铺搜索接口bug确认（userSite&site混淆脏数据）
- 运行haojing，写了一个用户ad查询demo

TODO

- 微信号和微信二维码上传（包括审核后台）前后端联调
- 学习探索IDL

THINKING

- 今天大部分时间用来继续调试昨天haojing没跑起来的问题，主要问题集中在类加载不到，通过haojing代码及其依赖的ayana、cube、composer中打log，整个调试过程看了非常多的代码，虽然最后仍然没解决（最最后是copy整份可以运行的haojing代码跑起来的），但是对haojing的项目代码架构熟悉了不少。
- haojing代码读后感：巨大的软件系统，时间跨度大，开发人员多，因为性能优化而带来较多的复杂度（一是设计上的，二是编码上的，三是业务强相关的）。有很多封装做得很棒，但是有些老代码仍然在用，给理解造成了一定困难。wiki上有不少新人上手haojing的文档，但是时效性和完成度不够，周末有空可以自己写写。

## 2018-07-23

DONE

- 微信二维码后台审核界面细节完善
- 微信二维码上传的后端接口完善，实现内容检测和“抠图”。

TODO

- 微信客服消息开发
- 第二次搬家会议
- IDL学习

THINKING

- 小程序开发对微信的依赖非常重，有必要从开发者层面深入了解微信生态。

## 2018-07-24

DONE

- 需求评审会，确认两个task的重难点，一个依赖于IDL的小程序导流组件，一个依赖于神策的商户店铺数据展示。
- 设置微信号的后端API重构，抽出二维码处理部分做成公用库函数，注释掉controller里面的处理（暂时砍掉二维码）。
- 尝试修改IDL中的组件，对IDL的运作过程有了基础的了解。

TODO

- 周三上午找神策沟通平台小程序访问数据问题
- IDL培训，几个关注点（排名分先后）：数据流；数据格式问题的定义和使用；后端context与前端widget关系；vuex的角色和应用；ide与manage/editor的关系。

THINKING

- IDL中定义了一些完整的组件，但由于业务需求需要修改，重写一个相似度很高的组件显然不划算，直接修改则破坏了组件的自洽性，对于这种情况需要一个可配置程度更高的高级组件。

## 2018-07-25

DONE

- IDL培训
- 店铺数据接通神策API

TODO

- debug神策API调用
- 店铺数据缓存机制设计

THINKING

- 第一次接触神策，BI系统，和数据库体验有像也有不像，总体来说对数据的量级把握不够，实时查询不实际的情况下又需要近实时的数据，对缓存是个挑战。感觉神策从设计还是更偏向于BI人员和产品等，系统的API不太友好（500的server error非常难以排查）。
- IDL复杂程度有点超过想象，涉及到的面又很广（听尚霖讲过后对于在php的render侧有所了解，但是之后的过程还是一头雾水，原因之一也是对小程序和前端本身不够了解）。没有参与设计和开发且缺乏足够好的文档的话，或许能根据代码按图索骥改一些东西，但遇到问题仍然很难排查。

## 2018-07-26

DONE

- 修复店铺搜索bug（并进一步重构搜索方法）：共享版和专项版未筛选，搜索结果中出现两个相同店铺。
- 打通神策API，近七天的pv和分享数据已完成“查询神策”、”缓存“和”格式化“的完整数据链条。

TODO

- 神策API方面还有近七日客户列表（未测试，需要更换测试数据），以及pv，uv，share的总量（未开发）。

THKING

- 神策API有点坑，这次算是踩过一遍坑了。
- 对redis缓存的数据结构设计方法多样，有些优劣评价涉及redis底层实现而从可行性上无从判断，最好是能有代码实际运行的性能状况反馈。

## 2018-07-27

DONE

- 清洗线上店铺搜索相关site+userSite的name脏数据

- 神策API重构，所有统计数据到位。

TODO

- 神策：数据准确性检查
- 神策：缓存检查，包括失效期，刷新体验等。
- 神策：最近客户列表需求再确认
- 神策：前后端联调

THINKING

- 在确认需求实现方案后，首先考虑代码可复用性，然后再开始编码。

## 2018-07-30

DONE

- 复盘店铺搜索

- 店铺数据，和cc确认这一版不上最近访客头像（缺少打点数据），和邹峰对接数据接口。
- userSite和site的name同步修改问题
- 优惠券的显示（及其icon）
- 跑bxappletes-javaapi

TODO

- 店铺数据后端收尾，准备联调送测
- 尽快上手java，熟悉spring

THINKING

- 在学校的项目组待的两年施行KPI考核，到毕业时转成OKR，但已没有机会体验，现在需要认真审视制定自己的第一个OKR。两个关键方向，对个人是技术层面的成长和工作经验的增长，对团队和公司应该是对团队实力和公司业绩的促进。
- 尝试给店铺数据的代码写了测试，体验不错，对于一些显而易见的约定类bug（比如调用结果的数据结构错误）可以减少debug时间，以后争取测试先行，直接实践TDD。



## 2018-07-31

DONE

- 点击复制微信号的编辑器预览显示
- 店铺数据的后端数据格式修改
- 熟悉javaapi项目的ApiController用法，了解thrift用法
- 梳理商品管理java迁移的第一步迁移方案：product（及sku）相关的三个php的api调用（增加、修改、删除，由js调用）首先部分（和user、site无关的部分）迁移到java。

TODO

- 实现上述“第一步迁移方案”，重点在跑通调用链。

THINKING

- haojing用到的thrift似乎和apache官方版本有所不同，做了哪些深度定制吗？
- 坑：thrift官方的安装教程一个都用不了，还是brew install一步到位，以后不能迷信官方文档了。
- 项目迁移方案，在初期增加各种调用，性能开销是迁移过程代价？



## 2018-08-01

DONE

- spring restful api 获取请求参数
- 通过自定义converter把mysql的json映射到spring jpa data
- 实现商品的删除接口

TODO

- mysql库软删除的实现（可以参照mongo的softdeleteRepository实现一个基础类）
- 商品增删改的三个javaapi实现，以及php调用打通

THINKING

- spring restful api 获取请求参数的难点主要是复杂请求体的映射方案：自定义类（太繁琐复杂），JSON（有些类型转换非常蛋疼还没解决），指定map类型（目前感觉比较方便，可能一些复杂类型和JSON差不多蛋疼）。这个难点是动态类型语言所没有的，以后要慢慢习惯。
- 数据库类型到编程语言类型的映射是数据库的一个重难点和坑槽，而且感觉充满了不优雅。在网上找mysql的json的映射方案，大部分都说msyql的json比较新，spring目前没有支持，需要手动convert，没有去找官方文档，感觉这种东西应该spring要跟进才对啊。
- 本来想增删改查这几个接口应该很快，结果还是把大部分时间投入到了spring的数据库对接这一层。之前学习spring的进度有点慢，今天调试一度有点抓瞎。后来看到说是spring理念认为 约定 胜于 xx，这个约定还是要多看文档。最后，虽然进度有点慢，但是耗在调试里的时间也还是很有收获。
- 今天的实现一开始让service返回po对象，后面处理异常的时候，感觉service调用结果还是应该要自带错误信息，因此让service直接返回RestfulResult了，这样在endpoint中的service调用也足够简洁。不知道这种方式是不是可以？



## 2018-08-02

DONE

- Review店铺数据后端代码，提pr
- spring jpa的softdelete机制实现
- 商品修改接口实现
- 其他零散工作：
  - mysql表的entity：product, sku, product_tag, product_tag_value, sku_tag_value
  - 部分实现entity对应的dao
  - 实现针对修改entity的接口RequestBody映射对象类

TODO

- 继续商品的增删改查java开发，以及php调用适配

THINKING

- 目前实现的spring jpa的softdelete机制，是每个PO继承一个包含deleted_at的字段，然后在这个PO上配置一些namedquery之类的注解，那每个继承的类（数据库表）都要写一遍类似的注解代码，不知道有什么方式可以直接继承（减少此类重复但不相同的代码）？
- sql syntax error太坑，关键字重合是关键考察因素，在设计数据表结构时就应该避免！
- 在jpa的保存操作中，保存一个实体即是更新数据库记录的所有字段。这个设计略坑。需要给entity配置DynamicUpdate注解，并且关注所有可能为null的情形（注解好用，这个null就比较麻烦）。
- java写起来还是感觉繁琐，即便有框架加成，不过类型确定心里感觉比较踏实，后期debug时间应该比较少。

## 2018-08-03

DONE

- 店铺名称修改不同步的bug跟进，确认是偶发数据库插入错误，暂不处理，后期视频率而定
- 实现javaapi的product和sku的一对多关联
- 实现product的查询接口，优化product的增加和修改接口

TODO

- javaapi的VO层实现，目标是对齐php侧view层所需数据
- php-javaapi联调

THINKING

- postman的缓存是大坑，甚至上面的url复制到浏览器会有编码问题，出现奇异bug的时候需要特别注意。
- 原有的BXAppletes项目，在view层的php上做了太多事情（调用了过多php对象方法），这增加了java化的工作量。显然，view层若只是渲染纯格式化数据就会简单许多。

## 2018-08-06

DONE

- 完成商品模块php侧联调，准备送测
- 整理了商品模块的java侧代码，主要内容有：删除了不必要的注解、加上服务错误码响应、部分函数重构和bug修复。
- 店铺数据须上线测试，先隐藏菜单入口
- 开始开发微信消息推送”我要开店“

TODO

- 打通微信客服消息被动回复

THINKING

- 从laradoc的php容器访问宿主机网络，有一定技巧，还需要更加了解docker网络细节
- 本地环境、QA环境、线上环境差距较大，目前来看还没有一套行之有效且相对高效的解决方案来规避环境不一致所导致的各种qa甚至开发问题，平时可以多加总结。

## 2018-08-07

DONE

- 实现微信公众号客服消息（发现和需求不对应……）
- 店铺搜索返回数据条数限制为100条（临时解决数据量过大问题）

TODO

- 用我自己的小程序测试，打通小程序客服消息

THINKING

- 微信的文档写得略烂，请求体也不直接给出http request的细节，导致开发者这边一定程度上得靠猜靠试……
- 今天主要工作都是在做公众号的客服消息，做完后发现需求文档里的客服消息其实是小程序客服消息，崩溃了一会儿，跟yp，zoufeng他们讨论后发现还要改idl，且线下环境没法发布，这个客服消息也没法用。。因为小程序端只需要加一个简单的入口，主要工作还是在后端，因此我决定自己弄一个简易小程序，先把后端调通。有了白天公众号客服消息的工作经验，应该弄小程序比较快吧（这里吐槽下，小程序的客服消息和公众号的客服消息，微信post过来的请求格式都不统一……）



## 2018-08-08

DONE

- 打通小程序客服消息，跟cc商议调整“我要开店”客服流程。
- 修复店铺数据部分用户404bug
- 店铺搜索热词缓存条数限制
- 听了其林的风控技术分享，主要topic有爬虫和刷单的风控。

TODO

- 商品模块java化java侧单元测试，提测
- 小程序客服后端接口上线对接确认

THINKING

- 暂无

## 2018-08-09

DONE

- 小程序客服消息服务器接口配置确认
- 商品模块java化重构部分代码，基本完成单元测试，提pr

TODO

- 根据pr反馈重构
- 尝试用mockit重写单元测试

THINKING

- 今天大部分时间在追java单元测试的一个序列化失败问题，首先为了排查onetomany这类复杂关系的影响，我简化了之前写的onetomany，删除了manytoone，发现问题并不是这个，商品的更新操作依然报错序列化异常（同样的调用，通过api走就不会报错，这让我感觉非常诡异）。进一步排查各个更新的属性，发现是一个自定义了Converter的JSONArray出问题了。尝试换用其他数据结构接收这个在mysql里按json存的数组，也非常不方便。这个问题最后也没解决，但考虑到不影响正常运行，暂时不管了，有空再研究一下。

## 2018-08-12

DONE

- 修复bug：店铺数据最近7天数据空，已合到qa
- 百姓店铺中推荐店铺的逻辑调整预开发，包括商品模块java侧的接口调整，预计总体进度30%+。
- 商品模块javaapi增加商品名称多关键字查询支持

TODO

- 需求评审，拆分task，跟崔老师等说明交易等服务的新接口需求

THINKING

- 店铺数据的bug排查下来，一个空判断写错。空判断的逻辑总是语言相关的，以后要多注意。
- 想学习一下【微服务的交叉调用最佳实践】。比如店铺内商品搜索这个需求，现有的javaapi已经基本能够实现搜索需求，那我还需要写新的后端接口代码吗？需求里一条敏感词屏蔽，显然javaapi又得再弄一个thrift调用，或者直接用php里现成的？
- 商品模块java化送测拖得有点久，现在直接看新需求改提过pr的代码了，但愿顺利……

## 2018-08-13

DONE

- 下线小程序的后端支持，siteInfo接口返回数据增加status字段
- 商品模块java化java侧提测（合到qa）
- 推荐店铺开发及自测，php侧进度80%，外部接口依赖pending。
- 开始继续推进商品模块java化：php侧重构接口请求，下架商品，c端商品列表，预计两个工作日内完成。

TODO

- 继续推进商品模块java化，包括店铺内商品搜索。
- 拿到推荐店铺所需的所有外部接口规范，完成开发和mock数据测试。

THINKING

-  商品java化的重构，参考了trade，还不错，以后多多学习。不过现在javaapi和trade服务的调用方式和接口还不太统一，如果统一的话又可以抽取一些公用方法，现在和trade有很多代码是雷同的。

## 2018-08-14

DONE

- 桌面端加上消息中心菜单入口
- 商品模块java化完成php侧和java侧部分重构，其中backstage中的商品操作（包括下架商品）等已完成java化。

TODO

- 沟通外部pending状况

THINKING

- 工作中接触不到运维，和测试信息不同步，影响效率，还是要多多同步信息。
- java化迁移还是得加速，不然php侧又堆积了很多新业务新代码，这样的话java化越慢，浪费的人力成本就越高……

## 2018-08-15

DONE

- 基于mock接口数据（线下接口数据基本是空的）基本完成推荐店铺逻辑调整，包括javaapi商品模块的一些调整和新增统计接口
- 修复了javaapi的几个接口bug

TODO

- 跟进完成javaapi商品模块测试

THINKING

- 第一次用了php的生成器，对推荐店铺的评分任务应该有性能上的帮助。不过现在店铺数量和订单数量较少，现在写好的代码应该够用了，后面可以考虑尝试用异步http请求来做java服务的调用。做这块的时候感觉user和userSite这样的强关系型的数据实体，用mongo来做还是不太适合，然后一些site，siteData，userSite的历史遗留问题，让人感觉比较别扭（期待有一天彻底解决啊，命名令人迷醉）。

## 2018-08-16

DONE

- 针对店铺数据的空数据时前端报错问题，对神策数据处理模块做了一次重构，基本解决。（但测试那边提的部分用户、首次打开时服务器500的问题未能查明原因）
- 店铺推荐的部分siteDataId和userSiteId查询使用redis缓存
- 参加java的string主题技术分享

TODO

- 推进商品模块java化业务测试（包括java侧和php侧）

THINKING

- 如何推进自动化测试？

## 2018-08-21
DONE 
- 商品模块在profilecontroller中的java化以及参数对齐bug修复

TODO

- 小程序客服消息测试
- 评价服务接入前期准备

THINKING

- 一直对小程序的线下测试感到疑惑不解，之前想的用自己的小程序来做线下测试的方案，折腾一番后还是夭折了。顺便吐槽一下现在项目里魔鬼数字和字符串满天飞，有空需要统一梳理改造一下。
- 并行多个需求的时候容易慌乱中出错，以后注意。
- 商品模块java化过程中在php这边的代码感觉还是非常丑陋，一方面php的controller，blade和js这三个的耦合度过高，不太熟悉代码的人很难进行统一的改造（改其中一个就必须得改其他几个，既麻烦也容易出错），基本上只能改动从数据库拉数据这一层。平时工作中在这方面还是要多加思考，坚决避免此类问题。

## 2018-08-22
DONE
- javaapi商品模块查询性能优化（一对多设置优化，一次http请求查询从2500ms左右优化到800ms）
- php里加上商品权限判断，判断商品是否为用户所有
  ProductContext + DataFetcher 中的商品模块java化开发

TODO

- 小程序客服消息测试（拖了好久了……）
- ProductContext + DataFetcher 中的商品模块java化测试

THINKING

- 性能问题源自小米说backstage页面加载需要8s，我的local环境这个页面实质上会502，之前把请求size从50条降到10条即解决，但是今天测试发现，50条情况下，走javaapi查询的耗时大概在2.5~3.0s以内，虽然耗时较长，但并不是主要瓶颈。检查发现是php这边的查询过于粗放，优化后耗时降低三四十倍，java调用成为主要耗时来源。
- hibernate不太好用，面向对象的优势在不想仔细读文档的情况下可以说是累赘，其次性能也不好。想起之前有用过一些自造的orm（python里的）直接把一行映射到一个dict就完事了，sql几乎裸写（自己封装函数生成sql），这样可控性很高而且用起来也非常方便。
## 2018-08-23

DONE

- backstage 商品模块空数据bug修复
- 小程序客服消息测试完成
- 评价服务接入task拆分
  - UserProfile表结构设计
    - id 业务id（评论服务可用），long
    - openid 小程序授权之后获取的用户openid, varchar(32)
    - unionid varchar(32)
    - headimgurl 小程序授权之后获取的用户头像地址, varchat(256)
    - nickname varchar(32)
  - 商品评论接口设计
    - Base URL: **/api/bxapplets/javaapi/v1/comment**
    - 创建商品评论  **POST /api/bxapplets/javaapi/v1/product/comment**
    - 查询用户评论 **GET /api/bxapplets/javaapi/v1/user/comment**
    - 查询商品评论 **GET /api/bxapplets/javaapi/v1/product/{productId}/comment**
  - javaapi的httpclient util

TODO

- 评价服务接口数据对齐，表设计对齐，以及完成接口设计
- 评价服务开发

THINKING

- 微服务架构下哪个服务做哪些事，调用链如何设计以求最合理，是个值得思考的问题（现实问题就是，面对一个评价服务，BXApplets（php）需要用，现阶段评价是在商品和用户（此用户与php里的user还不太一样）下面的，商品在javaapi，是php直接打评价服务呢，还是php打javaapi，javaapi再打评价服务？显然后者更合理也更合乎java化迁移的初衷，不过显然调用链变长，不可控因素也变多了。

## 2018-08-24

DONE

- 在javaapi上完成开发user的id-mapping（新建一张mysql表）
- php里对javaapi调用的重构，补充评论相关接口

TODO

- 在两个工作日内完成评论服务接入的后端开发，然后在一个工作日内完成评论服务接入接口联调测试，最后在一到两个工作日内完成前端联调

THINKING

- 模块间依赖导致有些纠结的地方，评论服务已经独立，我作为服务使用者，评论附属于商品和用户，我是把评论分别放到商品下面和用户下面，还是独立于商品和用户，让商品和用户来调用呢？对于javaapi和php这两个主体，考虑到可扩展性，需要分别考虑吗……

## 2018-08-27

DONE

- 协助富帅修复店铺内商品搜索的bug
- 继续接入评论服务，给前端写接口文档。

TODO

- 测试一下php里的商品java化
- 评论服务的回调接口开发

THINKING



## 2018-08-28

DONE

- javaapi接入redis，增加商品已售数量查询支持
- 店铺评分bug跟踪修复：查明是javaapi统计接口性能太差导致超时，已改进；php侧增加超时异常处理。
- 继续开发评论服务，增加删除（下架）接口，开发进度60%。

TODO

- 完成评论服务申请，开始javaapi一侧的联调：增加、查询、下架以及删除，以及php侧的联调：评论审核回调以及调javaapi

THINKING

- 联表查询尽量自定义，原生sql掌控性好啊。彻底弃用onetomany吧。



## 2018-08-29

DONE

- javaapi 去关联化改造，商品数量统计接口优化，评价服务联调
- manage后台editor页面商品设置改调javaapi完成自测

TODO

- 评价服务问题汇总
  - 删除评论：
    - 情形一：没有报错，显示删除条数为0，但实际未删除（查询评论可以查到）。
    - 情形二：没有报错，显示删除条数大于0。（删除评论id为1的，显示条数为3）
  - 查询用户所有评论：改变用户id等参数，响应结果永远不变（共计5条，id依次为91~95）

THINKING

- 暂无

## 2018-08-30

DONE

- 评价服务跟进
  - 确认：评价状态的影响，根据运营后台接口需求，委托服务方开发新的查询接口
  - 开发：评价审核回调发送模板消息、短信；接入服务方新的查询接口；用户的订单中商品是否被评论相关接口

- javaapi商品相关优化提测

- 和尚霖确认前后端开发分工事项

TODO

- 根据与尚霖的沟通结果，开发发表评论和后台查询评论的两个controller接口，并测试今明两天开发的新代码
- 与交易服务方就订单状态与评价相关的事宜进行沟通确认，并完成相关开发

THINKING

- 评价服务接入做下来比预想的要复杂很多，一开始只是以为作为一个外部api的接入，感觉工作量不是很大，重心都放在java开发的一些常用工具使用上面。越到后面越发现与非常多业务关联密切，甚而需要反馈评价服务方有针对性地提供一些接口，理论上来说，这些工作我还是应该更提前去完成，即在动手写代码之前考虑所有的业务情况，对现有的不明确的地方尽早找pm确认。

## 2018-08-31

DONE

- 完成评价服务php接口代码开发，准备联调
- 参加java集合技术分享

TODO

- 评价服务联调

THINKING

- 学过语言比较多了，每门语言的基本、常用数据结构相关的原理和函数林林总总，可以规范整理一下，有一个对照，方便做语言切换的时候心中有数，避免语言切换开销。

## 2018-09-03

DONE

- 本地测试评价服务回调接口，各项处理基本跑通

TODO

- 评价联调送测

THINKING

- 暂无

## 2018-09-04

DONE

- 店铺推荐：修复一个店铺筛选bug和一个分页bug；增加测试号过滤和关键词过滤。
- 修改订单状态流转与评价相关的逻辑；联调创建、查询评价；发现评价服务一个排序bug，已反馈。

TODO

- 评价联调backstage部分

THINKING

- 店铺推荐的店铺筛选bug是一个for循环的递进写错了，回顾排查过程非常坎坷，分析主要原因有三：程序无致命bug，无错误日志；程序依赖太多，一开始的排查重点在各种依赖上面；变量名词不达意，主观上马虎大意。以后提交代码前一定要上个洗手间再来一行一行读代码！

## 2018-09-05

DONE

- 粗略过了一遍通道服务的文档和代码
- 评价服务javaapi代码bug修复：2个；完成联调

TODO

- 评价送测修bug
- 深入了解通道服务

THINKING

- 有空总结一个方法，处理下述情形：java里面字段非常多的类，初始化时避免遗漏初始化某些字段。



## 2018-09-06

DONE

- 评价送测
- 阅读了通道服务rule，po等部分的代码，以及haojing发送短信相关调用
- 商品数量统计接口性能排查：线上mysql数据库添加索引

TODO

- 继续商品数量统计接口性能排查
- 继续熟悉通道服务
- 跟进评价测试结果

THINKING

- 之前用laravel的migration来创建索引，线上执行过migrate操作显示成功，实际检查发现未生效，遂直接在线上mysql数据库添加了product.site_id, product.created_at, sku.product_id三个索引，接口调用耗时降低了约400~500ms；实际排查发现瓶颈不在数据库查询（数据库查询时间在0.0025s的级别），而实际接口调用耗时稳定在3.8s左右。这超出了之前的预料范围，由于线下数据库无对等数据可供查询，所以线下接口速度比较快也无法反映出问题。接口中使用了hashmap结构，线上接口稳定返回一个capacity约等于50的结构，而代码中没有显示设定initCapacity值，默认为16，显然需要重新分配。但是这似乎并不能解释3.8s的耗时是怎么来的……

1. 手机端商户后台商品详情页
2. 店铺装修页面商品列表
3. 小程序内部商品详情页



线上测试：

- 不使用having

```
select product.site_id, product.created_at, count(product.id) from product
            where product.deleted_at is null
            and product.created_at >  '2018-08-16 17:16:38.111111'
            and (select sum(sku.stock)>0 from sku
            where sku.product_id=product.id
            group by sku.product_id)
group by product.site_id;
```

0.0028 + 0.0022+0.0023



### Detailed profile

| Order | State[![文档](http://weapp.baixing.com.cn:7100/themes/dot.gif)](http://weapp.baixing.com.cn:7100/url.php?url=https%3A%2F%2Fdev.mysql.com%2Fdoc%2Frefman%2F5.7%2Fen%2Fgeneral-thread-states.html) | 时间   |
| ----- | ------------------------------------------------------------ | ------ |
| 1     | Statistics                                                   | 13 µs  |
| 2     | Preparing                                                    | 12 µs  |
| 3     | Sorting Result                                               | 2 µs   |
| 4     | Executing                                                    | 2 µs   |
| 5     | Sending Data                                                 | 91 µs  |
| 6     | Executing                                                    | 2 µs   |
| 7     | Sending Data                                                 | 74 µs  |
| 8     | Executing                                                    | 3 µs   |
| 9     | Sending Data                                                 | 14 µs  |
| 10    | Executing                                                    | 1 µs   |
| 11    | Sending Data                                                 | 81 µs  |
| 12    | Executing                                                    | 1 µs   |
| 13    | Sending Data                                                 | 26 µs  |
| 14    | Executing                                                    | 1 µs   |
| 15    | Sending Data                                                 | 34 µs  |
| 16    | Executing                                                    | 1 µs   |
| 17    | Sending Data                                                 | 85 µs  |
| 18    | Executing                                                    | 1 µs   |
| 19    | Sending Data                                                 | 80 µs  |
| 20    | Executing                                                    | 1 µs   |
| 21    | Sending Data                                                 | 34 µs  |
| 22    | Executing                                                    | 1 µs   |
| 23    | Sending Data                                                 | 80 µs  |
| 24    | Executing                                                    | 1 µs   |
| 25    | Sending Data                                                 | 27 µs  |
| 26    | Executing                                                    | 3 µs   |
| 27    | Sending Data                                                 | 13 µs  |
| 28    | Executing                                                    | 1 µs   |
| 29    | Sending Data                                                 | 8 µs   |
| 30    | Executing                                                    | 1 µs   |
| 31    | Sending Data                                                 | 46 µs  |
| 32    | Executing                                                    | 2 µs   |
| 33    | Sending Data                                                 | 54 µs  |
| 34    | Executing                                                    | 1 µs   |
| 35    | Sending Data                                                 | 9 µs   |
| 36    | Executing                                                    | 1 µs   |
| 37    | Sending Data                                                 | 24 µs  |
| 38    | Executing                                                    | 1 µs   |
| 39    | Sending Data                                                 | 13 µs  |
| 40    | Executing                                                    | 1 µs   |
| 41    | Sending Data                                                 | 77 µs  |
| 42    | Executing                                                    | 1 µs   |
| 43    | Sending Data                                                 | 30 µs  |
| 44    | Executing                                                    | 3 µs   |
| 45    | Sending Data                                                 | 27 µs  |
| 46    | Executing                                                    | 1 µs   |
| 47    | Sending Data                                                 | 159 µs |
| 48    | Executing                                                    | 1 µs   |
| 49    | Sending Data                                                 | 9 µs   |
| 50    | Executing                                                    | 1 µs   |
| 51    | Sending Data                                                 | 22 µs  |
| 52    | Executing                                                    | 1 µs   |
| 53    | Sending Data                                                 | 8 µs   |
| 54    | Executing                                                    | 1 µs   |
| 55    | Sending Data                                                 | 9 µs   |
| 56    | Executing                                                    | 1 µs   |
| 57    | Sending Data                                                 | 6 µs   |
| 58    | Executing                                                    | 1 µs   |
| 59    | Sending Data                                                 | 7 µs   |
| 60    | Executing                                                    | 1 µs   |
| 61    | Sending Data                                                 | 17 µs  |
| 62    | Executing                                                    | 1 µs   |
| 63    | Sending Data                                                 | 8 µs   |
| 64    | Executing                                                    | 1 µs   |
| 65    | Sending Data                                                 | 7 µs   |
| 66    | Executing                                                    | 1 µs   |
| 67    | Sending Data                                                 | 7 µs   |
| 68    | Executing                                                    | 1 µs   |
| 69    | Sending Data                                                 | 7 µs   |
| 70    | Executing                                                    | 1 µs   |
| 71    | Sending Data                                                 | 194 µs |
| 72    | Executing                                                    | 1 µs   |
| 73    | Sending Data                                                 | 8 µs   |
| 74    | Executing                                                    | 1 µs   |
| 75    | Sending Data                                                 | 7 µs   |
| 76    | Executing                                                    | 1 µs   |
| 77    | Sending Data                                                 | 7 µs   |
| 78    | Executing                                                    | 1 µs   |
| 79    | Sending Data                                                 | 26 µs  |
| 80    | Executing                                                    | 1 µs   |
| 81    | Sending Data                                                 | 30 µs  |
| 82    | Executing                                                    | 4 µs   |
| 83    | Sending Data                                                 | 104 µs |
| 84    | Executing                                                    | 1 µs   |
| 85    | Sending Data                                                 | 26 µs  |
| 86    | Executing                                                    | 1 µs   |
| 87    | Sending Data                                                 | 8 µs   |
| 88    | Executing                                                    | 1 µs   |
| 89    | Sending Data                                                 | 8 µs   |
| 90    | Executing                                                    | 1 µs   |
| 91    | Sending Data                                                 | 8 µs   |
| 92    | Executing                                                    | 1 µs   |
| 93    | Sending Data                                                 | 7 µs   |
| 94    | Executing                                                    | 1 µs   |
| 95    | Sending Data                                                 | 11 µs  |
| 96    | End                                                          | 2 µs   |
| 97    | Query End                                                    | 6 µs   |
| 98    | Closing Tables                                               | 4 µs   |
| 99    | Freeing Items                                                | 47 µs  |
| 100   | Cleaning Up                                                  | 12 µs  |

### Summary by state

| State[![文档](http://weapp.baixing.com.cn:7100/themes/dot.gif)](http://weapp.baixing.com.cn:7100/url.php?url=https%3A%2F%2Fdev.mysql.com%2Fdoc%2Frefman%2F5.7%2Fen%2Fgeneral-thread-states.html) | Total Time | % Time | Calls | ø Time |
| ------------------------------------------------------------ | ---------- | ------ | ----- | ------ |
| Statistics                                                   | 13 µs      | 0.73%  | 1     | 13 µs  |
| Preparing                                                    | 12 µs      | 0.67%  | 1     | 12 µs  |
| Sorting Result                                               | 2 µs       | 0.11%  | 1     | 2 µs   |
| Executing                                                    | 2 µs       | 0.11%  | 1     | 2 µs   |
| Sending Data                                                 | 91 µs      | 5.08%  | 1     | 91 µs  |
| End                                                          | 2 µs       | 0.11%  | 1     | 2 µs   |
| Query End                                                    | 6 µs       | 0.33%  | 1     | 6 µs   |
| Closing Tables                                               | 4 µs       | 0.22%  | 1     | 4 µs   |
| Freeing Items                                                | 47 µs      | 2.62%  | 1     | 47 µs  |
| Cleaning Up                                                  | 12 µs      | 0.67%  | 1     | 12 µs  |



2. 使用having

```
select product.site_id, product.created_at, count(product.id) from product
            where product.deleted_at is null
            and product.created_at >  '2018-08-16 17:16:38.111111'
            and exists (select sku.id from sku
            where sku.product_id=product.id
            group by sku.product_id
            having sum(sku.stock) > 0)
group by product.site_id;
```

0.0025+0.0023+0.0023





## 2018-09-10 

DONE

- 评价服务bugfix：系统默认好评；模板消息跳转小程序；评论时间的时区问题；backstage的评论列表排序问题。
- 逛逛本地发现页的文案修改

TODO

- 投入到通道服务开发

THINKING

- 进公司两月以来第一次向haojing提代码。感觉对微信相关的封装过度了，不然这次就没有必要提交代码，因为只是多支持一个微信接口参数而已啊。

## 2018-09-11

DONE

- 昨天的评论服务bugfix：一是时间问题，发现评论服务方返回的时间戳就有问题，我方无法修复，已反馈；二是微信模板消息又挂了，同时商品详情页也时长会挂，查看日志发现是javaapi连接redis的异常，初步尝试升级spring-xxx-redis的版本。
- 分类信息的营收业务接入通道服务，对于新手礼、返券通知、一卡通过期提醒三个具体业务，在线上通道服务创建了对应模板，并完成相关开发

TODO

- 逛逛本地发现页测试（被delay了）

THINKING

- 暂无

## 2018-09-12

DONE

- 继续跟进评论的bug：针对dev环境，评论审核不通过（包括下架）用户未收到短信通知（staging的smslog），原因是BXApplets项目这边的短信暂不支持smslog；有图评价收不到微信模板消息，原因是评论服务的回调不稳定（debug）；评论的时区问题，已确认修复；商品详情页偶发性挂掉的问题，今天未复现。
- 镐京的营收业务短信部分重构，更改短信模板。

TODO

- dev环境修改商品的内容审核thrift调用挂了，本地未复现，dev稳定复现，暂不清楚原因，急需排查。

THINKING

- 修改商品的thrift调用bug非常迷……匪夷所思
- 商品详情页的bug（javaapi的redis连接报错）在升级spring-xxx-redis版本、以及把redis访问从forEach挪到普通for循环里之后，几乎不可复现，基本上可以说是解决了问题，但没有找到原因。有空还是要好好探索一下。
- 今天碰了一下教育培训新项目的事情，按照志超的想法，我们先做一些项目的基础建设，包括一些环境搭建、接入服务的事情。对于第三方服务（包括云服务商和自建的中台服务），如何能在已知项目行业背景、但业务方向不明确、产品需求还没有的前提下，做好一个快速接入的准备工作，值得探究。从我已有的经验来看，每次接入这种第三方服务，做一些与具体业务和产品需求无关的基础性工作（比如设置token和url这种事情）总是不可避免的，理论上这些工作应当可以提前做，但是通常因为业务逻辑的耦合特性，导致这种工作几乎没有提前开展的必要（写业务的时候很有可能要推倒重来）。希望可以以提高开发效率为目标，慢慢摸索经验。
- 对音视频图片等有兴趣，这次以新项目为契机，要探讨出一些常用、通用经验。

## 2018-09-13~2018-09-14

DONE

- 解决了商品发布、修改的线上bug
- 上线了javaapi的评价功能，完成所有上线前测试

TODO

- 评价上线验收
- 视频解决方案调研

THINKING

- 昨天早上追商品发布、修改的bug，初步定位是thrift模板不一致，同步模板相关代码后，仍然报错。由于我最近改动过镐京那边的thrift，而商品也涉及thrift调用，一直怀疑是这个原因。下午因为这个线上问题还发现了javaapi的一些bug，最终揪出来9天前的javaapi一次上线其实根本没有成功，掩盖了一些bug。今天从thrift调用的服务端和客户端着手，在发布系统界面上看到12号以后质量模块有多处更新上线，而最早的一条报错信息就是12号，根据这个信息找到西瓜在线下做了联调，总算是解决了这个幽灵问题。西瓜反馈我说应该要catch住调用报错，不能百分百相信这个服务。这是一个教训。另外，javaapi的线下线上环境不一致的问题，也引发了很多连锁反应，而我在以下几个方面是存在一些疏漏和不足的：一是对spring time和consule不熟，几乎处于无知状态，不会配置不懂部署；二是对线上线下日志系统无知，都不知道在哪里可以看日志；三是上代码的时候太不小心了，bug往往出在合代码的时候……除了质量审核这个thrift的bug，接入评论服务的过程中，同样存在类似的问题。反思下来还是对情况复杂度预估不足，以后还是要把这些情况分析以文档的形式留存下来，出问题了也方便check和debug。



## 2018-09-17

DONE

- 修复微信公众号模板消息的小程序跳转bug；联调评价的模板消息跳转小程序。
- 初步调研又拍云和腾讯云

TODO

- 音视频解决方案碰头会

THINKING

- 最近debug常常需要看dev或者online环境的laravel日志文件，整个操作流程还是非常麻烦，所以抽空写了一个小服务器，登录到这些服务器拉取日志在浏览器里显示，虽然还不完善，但是感觉非常便利。
- 基础设施产品选型需要考虑的因素非常多，对我现阶段来说还是很有难度。