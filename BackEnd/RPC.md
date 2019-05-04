>>>本文整理自公司的前端同学发起的《网络探秘》课程中RPC一章的备课资料，该课程的宗旨是回答**从浏览器输入url到展示页面中间都发生了什么**。本文对RPC的介绍，比较偏重协议本身，知识点比较分散，较少涉及RPC框架、微服务等内容。

## 前言

大家好，我是来自爬虫技术组的黄振，花名弹簧，更多人叫振子，今天给大家讲的题目是RPC。

在座的有些参加了微服务课程的同学应该知道，微服务那边已经讲过RPC，我听下来的感受就是，**RPC的本质是socket加动态代理**。不懂的同学不用着急，我们网络课程里的RPC，侧重点会有一些不一样。

我们先来看我今天预备讲的目录。总而言之呢，我们会一起来探索一下RPC的前世今生，以及它在我们这门课的主题——”从浏览器输入url到展示页面中间都发生了什么？”，我称之为“url宇宙”，是一个怎样的存在。

我先讲前言部分。之所以叫前言，是因为想回顾一下前面几节课。可以说，**DNS解决了域名到ip的映射问题，http/https/http2解决了数据的端到端传输问题**，也就是说，有了这几个协议，浏览器可以从某台远端的服务器拿到一些数据。我现在假设在计算机网络这门学科、这个领域中，大家都知道OSI七层模型，也知道TCP/IP四层模型。那么大家可以思考一下，我们学过的这几个协议，是哪一层的协议？我们是如何在不知道底层协议如何工作的基础上，知道这些协议是能work的？——这一点很重要，对RPC来讲也是。

![1556933023324](assets/1556933023324.png)

我们经常讲某某协议工作在第几层，今天讲RPC，当然也希望知道它是工作在第几层的。在揭晓答案之前，我想先介绍一下我用Google Trends做的一点微小的工作。


首先我通过某种众所周知的工具知道了一件事情，那就是：RPC全称叫Remote Procedure Call，直译过来就是远程过程调用。然后我想知道，当人们在谈论RPC的时候，到底在关心着什么，于是乎我想到了Google Trends，我搜索了近5年全中国在Google上搜索“远程过程调用”这一主题的“相关主题”，也就是人们在搜索的RPC的同时还搜索了什么，最终我得到了这页PPT。

![1556933068685](assets/1556933068685.png)

值得一提的是，我把时间放宽到自2004年以来，SOAP进入了这份名单，而我估计在做大部分人根本没用过这个东西。由此推测，RPC的发展还是有一定的历史阶段的。



回到TCP/IP的四层网络模型，”显而易见"，RPC是工作在应用层的协议。这一点和前面课程讲到的DNS、HTTP等是一样的。但是我们知道应用层本身也是博大精深的，内部也会有一些上下层级关系，这一点情况比较复杂，后面会讲到。

![1556933100470](assets/1556933100470.png)



## RPC的由来


我们现在知道，RPC是应用层协议，也就是说它有一定的“应用”场景。下面我想从一个具体的应用来谈RPC的由来。


假设，有个云盘应用，网址是"http://pan.baixing.com" ，产品说了，如果是该应用的注册用户，那么进入该网站后首先看到的是用户的主目录。翻译成技术语言，这不就像"ls /home/lurenjia"命令的输出一样嘛。

又假设我们现在有一台1核1G内存20G硬盘的云主机。现在我们尝试来做一点高级的开发工作——写点伪代码，画个PPT架构图。

![1556933177897](assets/1556933177897.png)

现在请大家脑补一下我设计的架构。我在这台服务器上跑个mysql-server，并且给每个注册用户分配一个文件目录，然后用我们熟悉的web框架，比如flask，laravel，eggjs这样的，写个controller连mysql做用户模块，写个controller做**ls**，写个foreach渲染的html。三下五除二，这个云盘应用模型就呼之欲出了——

```
def index('/home', request) {
    files = os.execute('ls /home/' + request.user.name)
    for file in files:
    	echo '<li>' + file.name '</li>'
}
```
So easy。我把这个阶段的架构叫做“web应用和文件存储在一台服务器上”。

![1556933261132](assets/1556933261132.png)

然而随着用户文件数量的增长，我们很快发现这个20G硬盘要满了。

怎么办？



加机器呗。这次我们先加一台和前面那台CPU和内存都差不多、但是硬盘大一倍的机器。

怎么**安排**这两台服务器呢？
1. 把现有的应用部署到新机器上，老机器做开发机。
2. 把现有的应用部署到两台机器上，再用nginx做个负载均衡。
3. 新建一个应用，专门用来处理文件，部署到新机器上；原有的应用基本不变，但是调用**ls**的本地调用，改为调用新应用提供的API。



看起来第三个方案比较好，怎么实现呢？像下面这样吗？

```
def index('/home', request) {
	resp = requests.get('http://file.pan.baixing.com', 
						params={"cmd": 'ls /home/' + request.user.name})
    file_dict_list = json.loads(resp.content)
    for file_dict in file_dict_list:
    	echo '<li>' + file_dict['name'] '</li>'
}
```

这个方案的问题是什么？

1. 要尤为关心网络异常
2. 数据结构变了



![1556933341389](assets/1556933341389.png)



怎么解决？这份伪代码其实是python风格的，python作为一个动态语言，有一个很灵活的东西，叫“猴子补丁”，大概就是长这个样子了。

```
def response_to_files(resp):
	files = []
	file_dict_list = json.loads(resp.content)
	for file_dict in file_dict_list:
		file = new File(name = file_dict['name'])
		files.append(file)
	return files
	
def execute_patch(cmd):
	resp = requests.get('http://file.pan.baixing.com', 
						params={"cmd": 'ls /home/' + request.user.name})
    return response_to_files(resp)
    
os.execute = execute_patch

def index('/home', request) {
    files = os.execute('ls /home/' + request.user.name)
    for file in files:
    	echo '<li>' + file.name 
}
```

是不是很像一个本地调用了？


遇到的现实开发问题是解决了，但是有更高追求的我们会想要思考，这里面有哪些共性问题？

比如说，

- 如何规定远程调用的语法？
- 如何传递参数？
- 如何表示数据？
- 如何知道一个服务端都实现了哪些远程调用？从哪个端口可以访问这个远程调用？
- 发生了错误、重传、丢包、性能等问题怎么办？

那我们刚刚的那个方案，对这几个问题，背后隐藏了什么样的回应呢？

- 我们采用了RESTful的API设计
- 我们使用HTTP协议中的headers，path，query，body等来传递参数
- 我们使用JSON表示数据
- 我们对网络错误等抛出异常
- 我们使用API文档来进行约定

还记得吗，我们一开始看到的Google Trends，RESTful，HTTP，JSON，API都是和RPC相关性很高的词。

可以说，**这5个问题，就是RPC协议试图要解决的问题**。



## Sun RPC
俗话说，以史为镜可以明得失。我们还是先来看一看RPC的历史。

对于RPC的解释，可以找到一个有点历史感的陈述：

>RPC 的全称是 Remote Procedure Call，是一种进程间通信方式。 它允许程序调用另一个地址空间（通常是共享网络的另一台机器上）的过程或函数，而不用程序员显式编码这个远程调用的细节。

这句话讲得很明白，但不够具体，对吧？毕竟是个术语嘛。


通常认为，
>RPC 这个概念术语在上世纪 80 年代由 Bruce Jay Nelson 提出。

Nelson这位大牛呢，写了一篇大名鼎鼎的论文，奈何年代久远，pdf模糊得不得了，我就放弃了。

关于RPC的历史呢，我在网上找了三张图，有点时间线的感觉，给大家分享一下。


第一张图，那时候我还没出生。这里面大部分东西估计大家也不太清楚，但是细看的话，其实Nelson和施乐的名字在图中是在一起的。

![1556933535123](assets/1556933535123.png)



第二张图，开始出现一些大家比较熟悉、或者耳闻过的一些名词，比如Java RMI，REST，Thrift。

![1556933573438](assets/1556933573438.png)



第三张图，可能慢慢地大家没那么了解了。从前后端分离的视角看，问题域会比较偏后端一些。

![1556933595480](assets/1556933595480.png)



名词花样百出，不过我们还是要返璞归真，先来看看最初的RFC1050是怎么解释RPC的。

这篇RFC的作者是Sun公司，我挑了一段有关`RPC protocol`的，有点长，但是翻译成人话就是：C/S架构，Socket通信。

我们再看Nelson的那篇论文（其实是看别人的总结），里面论述了实现RPC协议的一种范式，后来也成为了RPC框架的一种标准范式。

![1556933657943](assets/1556933657943.png)


这里补充说一点，其实RPC协议和TCP/IP协议一样，都是先有工业实现，然后才贡献回标准组织成为业界标准的。这个好处就是协议非常接地气。

我们细看这个图，左边是caller，调用者，分为三部分，User/User-stub/RPCRuntime；右边是callee，被调用者，也是三部分，RPCRuntime/Server-stub/Server，两边高度对称，中间通过网络包进行沟通。

我们前面实现的那个网盘，最后调用远程ls就像调用本地一样方便。这其实体现了RPC的宏伟目标。

>让构建分布式计算（应用）更容易，
>在提供强大的远程调用能力时
>不损失本地调用的语义简洁性。

怎么实现这个宏伟目标呢？还是先学习下先人的经验。

先简单介绍下NFS，网络文件系统，也就是Sun公司的RPC的来头，可以看下图：

![1556933685669](assets/1556933685669.png)

它是怎么解决前面提到的那五个问题的呢？

对于前面三个问题，归纳为语义问题，NFS做了一套RPC框架，也就是Stub 层，来处理双方约定好的语法、语义、封装、解封装（其中数据编解码采用了XDR，一种数据描述与编码标准）。

第四个问题呢，称之为传输问题，这类问题呢其实好像没法解决，只能优化。我挑一个方面讲。比如这个NFS，不难猜想，里面含有大量对远程文件系统的读写操作，大家都知道磁盘是很慢的，这就有很大的性能问题了，可能我在一个挂载了NFS的电脑上发起了一个写磁盘操作，结果它一直在转圈，关键是这个操作可能还让我后台的下载任务给pending住了，这肯定接受不了？对这样的问题，NFS是怎么处理的呢？

这里就牵扯到一个RPC的分类问题，同步RPC，异步RPC。NFS对于读写操作，基本都是走异步RPC的，也就是会先放到一个本地缓存，再异步去做真正的读写。但是其他的NFS操作，基本上都还是传统的同步RFC调用。

![1556933755647](assets/1556933755647.png)

再看第五个问题。这个问题在NFS这里变复杂了，它不能简单地用一个API端点（endpoint）约定来做，因为服务端可能在不同的进程中实现了多个远程调用，而这些进程监听的端口不一样，也不可能再各自去占用一个“众所周知”的端口。这就有点像是需要“服务发现”了。NFS的解决方案是portmapper，也就是每个RPC进程启动时先去portmapper注册一下自己的端口号，而这个portmapper监听一个众所周知的端口号，这样客户端只需要问portmapper就行了。

![1556933777468](assets/1556933777468.png)

NFS里的这个RPC，以及RFC1050中所谈到的RPC，通常管它叫Sun RPC。它有什么缺点呢？

吐槽来自两方面：
- 协议死板，不够灵活，升级简直比升天还难（因为参数的个数、位置都规定得死死的，想要给服务端加一个参数，之前的客户端是没法和新的服务端沟通的）
- 面向过程，而现在早就是面向对象的天下了

我认为这个根源问题在于，二进制啊，不是给人看的。

现在网速比当年快了多少个数量级，CPU也强了不知哪里去了，二进制那点空间还有省的必要吗？
像我这么想的人很多，于是就出现了文本RPC，一开始是XML，然后是JSON，他们都有一个特征：

>>“天然跨语言，自成文档”。

真是美妙。这二者的竞争，结果大家也看到了，JSON和它背后的RESTful赢了。

既然文本类RPC有这么好的优点，那还有什么缺点呢？
缺点就是，相同的信息量，文本自然会比二进制占用更多的字节数，从网络传输的角度讲，对带宽和时延是不友好的。特别是现在微服务兴起，原来的本地调用变成了走网络请求的跨服务调用，而且这种调用非常频繁，对性能其实是非常敏感的。

因此，现在很多二进制的RPC仍然占有很大的比重，比如我们镐京里用的Thrift。

这一节最后，我们看一张图，这个图很好地解释了RPC的层次，以及RPC框架需要做什么。

![1556933894864](assets/1556933894864.png)

需要说明的是，RFC中有句话：

> RPC协议本身与**客户端服务端的绑定以及交互**是独立的。

换句话说就是RPC协议本身与传输问题无关，但是RPC框架需要考虑。前面说了RPC协议与HTTP协议同属应用层协议，但是应用层协议本身也可能是分层的，显而易见的是，**RPC框架可以采用HTTP协议来做网络传输协议，也就是说RPC协议可以工作于HTTP协议的上层**。

## gRPC
这一节我们看gRPC。
首先我们明确一点，**gRPC是实现了RPC协议的一种框架，并且是跨语言的RPC框架，并不是go语言的专属rpc**。另外，gRPC是基于HTTP2的，也就是说，gRPC对于传输问题给出的解决方案是HTTP2。因此，我们今天重点就是***看一下gRPC里的HTTP2，是怎么一回事***。

我们回到一开始的那个网盘应用。假设产品说要做一个离线下载功能，下载什么呢，抖音用户上传的视频。翻译成技术语言就是，给你一个抖音用户ID，把该用户的抖音视频异步下载到网盘中。

我做了一些调研，要获取一个抖音用户的视频列表，需要请求抖音的一个API，这个API需要一个签名。签名怎么解决呢？有人整理了一个js的签名算法，输入一个抖音uid，输出一个签名字符串。

那么问题来了，前面的伪代码是python写的，python怎么调这个js代码呢？
还是两种方式：

- 装一个node，让python来调node，让node来运行js，比如`signature = os.execute('node sign.js --userid=' + user_id)`
- 使用跨语言RPC框架，把js封装成一个服务端，python这边作为客户端

![1556933928618](assets/1556933928618.png)

跟后者比较类似的，有一个大家比较熟悉的做法，就是写API，JSON来JSON去，很好办的。

不过今天要换一种符合主题的方式，gRPC。

先看一下gRPC官网自我介绍一下的一个图，我的第一反应是跨语言这个优点表现得挺明显的。
![1556933949494](assets/1556933949494.png)



图里提到了两个东西：Proto Request和Proto Response，有一个Proto Response后面还加了括号s括号。

都是挺细节的东西，我第一次看到这个图压根不会注意。但里面就蕴含了类似服务端给客户端异步回调这样的“请求响应”模式。

下面我们来看看这个Proto Request和Response在http2里是什么样的。

```
Request → Request-Headers *Length-Prefixed-Message EOS


HEADERS (flags = END_HEADERS)

:method = POST
:scheme = http
:path = /google.pubsub.v2.PublisherService/CreateTopic
:authority = pubsub.googleapis.com
grpc-timeout = 1S
content-type = application/grpc+proto
grpc-encoding = gzip
authorization = Bearer y235.wef315yfh138vh31hv93hv8h3v

DATA (flags = END_STREAM)
<Length-Prefixed Message>
```

信息量有点大，这个 `*Length-Prefixed-Message`稍微解释一下，中文文档翻译成了“界定的消息”，其实也就是http2中的END_STREAM标识所界定的一个完整的消息，这个消息的最前面是消息长度，帧类型是DATA。还有就是path头大家可以看到是一个service内的方法，也就是被远程调用的目标函数。大家可以回忆一下，RESTful中的path头代表什么含义？



再看response。
```
Response → (Response-Headers *Length-Prefixed-Message Trailers) / Trailers-Only
HEADERS (flags = END_HEADERS)
:status = 200
grpc-encoding = gzip
content-type = application/grpc+proto

DATA
<Length-Prefixed Message>

HEADERS (flags = END_STREAM, END_HEADERS)
grpc-status = 0 # OK
trace-proto-bin = jher831yy13JHy3hc
```
顾名思义的trailers就是尾部，具体是个啥呢？其实就是两个头：
- grpc-status
- grpc-message

可以看官方文档的解释，比较清楚：
```
Response → (Response-Headers *Length-Prefixed-Message Trailers) / Trailers-Only
Response-Headers → HTTP-Status [Message-Encoding] [Message-Accept-Encoding] Content-Type *Custom-Metadata
Trailers-Only → HTTP-Status Content-Type Trailers
Trailers → Status [Status-Message] *Custom-Metadata
HTTP-Status → ":status 200"
Status → "grpc-status" 1*DIGIT ; 0-9
Status-Message → "grpc-message" Percent-Encoded
Percent-Encoded → 1*(Percent-Byte-Unencoded / Percent-Byte-Encoded)
Percent-Byte-Unencoded → 1*( x20-x24 / x26-x7E ) ; space and VCHAR, except %
Percent-Byte-Encoded → "%" 2HEXDIGIT ; 0-9 A-F
```

这里涉及很多名词，但是还有三个比较重要的需要介绍一下，Stream ID， Data Frames和Errors。这三个东西是http2的，在gRPC中，它们是这样被使用的：

- Stream ID
>gRPC中每个调用都需要分配一个内部id，实际上就是http2中stream的id。而stream的id，只能保证在一个http2会话中唯一，如果一个进程处理多个http2会话，就不能保证唯一性了。

- Data Frames
>http2数据帧的边界与Length-Prefixed-Message的边界没有关联，也就是说后者可能横跨多个数据帧。

- Errors
>gRPC的运行时错误，会作为一个Trailer，在一个HEADERS帧中发给客户端。如果遇到RST_STREAM帧，则需要将http对应的错误码翻译成grpc的错误码。



下面我们践行一下实践出真知，亲自动手抓包试试。除了一份代码，还需要准备两个工具。

- wireshark
- BloomRPC （RPC的图形化客户端）

wireshark需要配置一下，就可以筛选http2的包（可以参考<<https://github.com/elixir-grpc/grpc/wiki/How-to-capture-HTTP2-packages-using-Wireshark>>）。

代码的话，除了gRPC官方给的示例，我承接前文所述，提供

- 服务端(nodejs)：<https://github.com/Danceiny/micro-node-douyin>
- 客户端(java spring)：https://github.com/Danceiny/micro-server-douyin

简便起见（无须启动spring项目），用BloomRPC导入proto文件，输入一下ip+端口号，就可以执行调用了。

wireshark筛选一下http2，可以看到一次rpc调用大概会有5到6个报文。

![1556934382744](assets/1556934382744.png)

这里可以看到很多WINDOW_UPDATE帧，提一个问题：
>对于数据量较大的gRPC传输，可以怎么优化？
>
>> 降低WINDOW_UPDATE帧出现的概率是一个基本思路，可以增大设置窗口的大小来做。



如果改一下这个service的代码，让它做一个除0的除法，看一下报文，会比较容易理解Trailers实际上是什么内容。

![1556934524959](assets/1556934524959.png)




官方的示例给了更多的例子，包括RPC调用的各种姿势，比如
-  单向RPC
>客户端发送一个请求给服务端，从服务端获取一个应答，就像一次普通的函数调用。

- 服务端流式RPC
>客户端发送一个请求给服务端，可获取一个数据流用来读取一系列消息。客户端从返回的数据流里一直读取直到没有更多消息为止。

- 客户端流式RPC
>客户端用提供的一个数据流写入并发送一系列消息给服务端。一旦客户端完成消息写入，就等待服务端读取这些消息并返回应答。

- 双向流式RPC
>两边都可以分别通过一个读写数据流来发送一系列消息。这两个数据流操作是相互独立的，所以客户端和服务端能按其希望的任意顺序读写，例如：服务端可以在写应答前等待所有的客户端消息，或者它可以先读一个消息再写一个消息，或者是读写相结合的其他方式。每个数据流里消息的顺序会被保持。

有兴趣的可以继续探索，今天就不展开了。

最后讲讲gRPC默认使用的protobuf。protobuf经常拿来和json作对比，通常认为它是一种更高效的、更现代的序列化方案，比json更为紧凑。为什么紧凑呢？查资料看到：
>protobuf采用Zigzag编码，该编码使用了Varint技术。Varint 是一种紧凑的表示数字的方法。它用一个或多个字节来表示一个数字，值越小的数字使用越少的字节数。

从序列化之后的消息体积来看，比json和xml都要小，看图差不多小了一半多。

![1556934612011](assets/1556934612011.png)

上面讲Varint，它对越小的数字，使用越少的字节数来表示，这个方案能奏效的原理，其实来自于哈弗曼编码。因为哈弗曼编码论证了一个结论：压缩编码应满足“高概率的码字字长应不长于低概率的码字字长”，而protobuf做出了一个我们都深以为然的假设：实际上高频使用的数字，其实都挺小的，一个long型64比特，大部分都浪费了。
那么也请大家思考一个问题，如果要传输的整数是大整数巨多，那protobuf还高效吗？答案比较显而易见，但是也很显然的是，这样的场景出现的概率比较小。

关于protobuf，再给大家分享一篇文章，叫[《Protobuf有没有比JSON快5倍》]()。这篇文章的一个结论是：protobuf在处理整数时优势明显，而json在对象绑定时其实并不弱。

![1556934661178](assets/1556934661178.png)

最后简单提一下gRPC的一些不完美之处。

![1556934721455](assets/1556934721455.png)

以上。



##  参考资料
- <https://github.com/Danceiny/WikiNotes/blob/master/BackEnd/RPC_tutorial.ipynb>
- [深入了解 gRPC：协议](https://www.jianshu.com/p/48ad37e8b4ed)
- [深入浅出RPC原理](https://ketao1989.github.io/2016/12/10/rpc-theory-in-action/)
- [RPC 的概念模型与实现解析](https://segmentfault.com/a/1190000005178084)
- [一个爬虫工程师的技术博客：gRPC](http://maqiangthunder.github.io/2016/07/04/%E6%9D%82/gRPC/)
- [HTTP/2 and gRPC — The Next Generation of Microservices Interactions](https://render.githubusercontent.com/view/ipynb?commit=e6497145f0a56bac346c842caf5f88eba36532a1&enc_url=68747470733a2f2f7261772e67697468756275736572636f6e74656e742e636f6d2f44616e6365696e792f57696b694e6f7465732f653634393731343566306135366261633334366338343263616635663838656261333635333261312f4261636b456e642f5250435f7475746f7269616c2e6970796e62&nwo=Danceiny%2FWikiNotes&path=BackEnd%2FRPC_tutorial.ipynb&repository_id=72998147&repository_type=Repository)
- [HTTP2详解](https://juejin.im/post/5b88a4f56fb9a01a0b31a67e)
- [dubbo协议](http://dubbo.apache.org/zh-cn/docs/user/references/protocol/dubbo.html)
- [HTTP/2 幕后原理](https://www.ibm.com/developerworks/cn/web/wa-http2-under-the-hood/index.html)
- [抓包gRPC的细节及分析](https://jingwei.link/2018/10/02/grpc-wireshark-analysis.html)
- [HPACK: Header Compression for HTTP/2](https://httpwg.org/specs/rfc7541.html)
- [http-parser](https://github.com/nodejs/http-parser)
- [gRPC的HTTP2实现](https://ninokop.github.io/2018/06/18/gRPC%E7%9A%84HTTP2%E5%AE%9E%E7%8E%B0/)
- [思考gRPC ：为什么是HTTP/2](http://hengyunabc.github.io/thinking-about-grpc-http2/)
- [HTTP2 协议上的 gRPC](https://doc.oschina.net/grpc?t=58011) (原文[gRPC over HTTP2](https://github.com/grpc/grpc/blob/master/doc/PROTOCOL-HTTP2.md)
- [Protobuf 有没有比 JSON 快 5 倍？](https://www.infoq.cn/article/json-is-5-times-faster-than-protobuf)
- [Protobuf 的优势在哪里？](http://ngudream.com/2017/08/15/java-protobuf-superior/)
- [Introducing gRPC Support with NGINX 1.13.10](https://www.nginx.com/blog/nginx-1-13-10-grpc/)
- [grpc实现一个较复杂的聊天室](https://my.oschina.net/tuxpy/blog/1645030)
- [NFS 文件系统源代码剖析](https://www.ibm.com/developerworks/cn/linux/l-cn-nfs/index.html)
- [晁岳攀---基于go的 rpc框架实践](http://www.10tiao.com/html/528/201809/2653371218/1.html)