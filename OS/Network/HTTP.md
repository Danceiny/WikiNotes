
1.HTTP协议概述
		HTTP协议用于定义客户端与web服务器通迅的格式。
		HTTP是hypertext transfer protocol（超文本传输协议）的简写，它是TCP/IP协议的一个应用层协议
		HTTP使用请求-响应的方式进行传输，一个请求对应一个响应，并且请求只能是由客户端发起的。
		HTTP协议的版本：HTTP/1.0、HTTP/1.1，其中1.0一次请求发起一次连接，响应过后连接自动断开。1.1里每次请求响应后连接将保持一段时间，这段时间内可以再执行请求响应。

2.HTTP请求
	2.1请求行
					GET /books/java.html HTTP/1.1
					请求方式 请求的资源名 所遵循的协议
		2.1.1请求方式：GET、POST，
					其中GET方式在请求资源的URL后跟“？参数名=参数值&参数名=。。。”方式传递参数，传输的数据内容最大为1K
					其中POST方式在请求实体中传输数据
					除了用Form表单明确用method指定用post方式提交数据以外，其他的方式都是GET提交方式

	2.2请求头
		Accept: text/html,image/*    客户端可以接受的数据类型
		Accept-Charset: ISO-8859-1	客户端接受数据需要使用的字符集编码
		Accept-Encoding: gzip,compress 客户端可以接受的数据压缩格式
		Accept-Language: en-us,zh-cn  可接受的语言环境
		Host: www.it315.org:80 想要访问的虚拟主机名
		If-Modified-Since: Tue, 11 Jul 2000 18:23:51 GMT 这是和缓存相关的一个头，带着缓存资源的最后获取时间
		Referer: http://www.it315.org/index.jsp 这个头表示当前的请求来自哪个链接，这个头和防盗链的功能相关
		User-Agent: Mozilla/4.0 (compatible; MSIE 5.5; Windows NT 5.0) 客户端的一些基本信息
		Cookie 会在后面讲会话技术的时候单讲
		Connection: close/Keep-Alive 指定是否继续保持连接
		Date: Tue, 11 Jul 2000 18:23:51 GMT 当前时间



	2.3实体内容


3.HTTP响应
	3.1状态行
		HTTP/1.1 200 OK
		格式： HTTP版本号　状态码　原因叙述<CRLF>
		状态码：
			200：请求处理成功
			302：请求重定向
			304、307：服务器通知浏览器使用缓存
			404：资源未找到
			500：服务器端错误

	3.2若干响应头
			Location: http://www.it315.org/index.jsp  配合302实现请求重定向
			Server:apache tomcat 服务器的基本信息
			Content-Encoding: gzip 服务器发送数据时使用的压缩格式
			Content-Length: 80 发送数据的大小
			Content-Language: zh-cn 发送的数据使用的语言环境
			Content-Type: text/html; charset=GB2312 当前所发送的数据的基本信息，（数据的类型，所使用的编码）
			Last-Modified: Tue, 11 Jul 2000 18:23:51 GMT 缓存相关的头
			Refresh: 1;url=http://www.it315.org 通知浏览器进行定时刷新，此值可以是一个数字指定多长时间以后刷新当前页面，这个数字之后也可以接一个分号后跟一个URL地址指定多长时间后刷新到哪个URL
			Content-Disposition: attachment;filename=aaa.zip 与下载相关的头
			Transfer-Encoding: chunked 传输类型，如果是此值是一个chunked说明当前的数据是一块一块传输的
			Set-Cookie:SS=Q0=5Lb_nQ; path=/search 和cookie相关的头，后面课程单讲
			ETag: W/"83794-1208174400000" 和缓存机制相关的头
			Expires: -1 指定资源缓存的时间，如果取值为0或-1浏览就不缓存资源
			Cache-Control: no-cache  缓存相关的头，如果为no-cache则通知浏览器不缓存
			Pragma: no-cache   缓存相关的头，如果为no-cache则不缓存
			以上三个头都是用来控制缓存的，是因为历史原因造成的，不同的浏览器认识不同的头，我们通常三个一起使用保证通用性。
			Connection: close/Keep-Alive   是否保持连接
			Date: Tue, 11 Jul 2000 18:23:51 GMT 当前时间

	3.3实体内容

# 状态码细节
- 301（永久）与302（临时）的区别： http://veryyoung.me/blog/2015/08/24/difference-between-301-and-302.html
>301适合永久重定向.比较常用的场景是做域名跳转。
>302用来做临时跳转
比如未登陆的用户访问用户中心重定向到登陆页面。访问404页面会自动重定向到首页