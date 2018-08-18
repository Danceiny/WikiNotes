一、jsp基础
1.JSP概念
		Servlet是j2ee提供的动态资源开发技术，是以java的程序的形式进行开发，在java中书写HTML标签是一件十分头疼的事情，所以人们开发出了JSP，看起来像是HTML一样，但是通过服务器的编译最终可以生成Servlet
2.JSP的组成部分
	2.1模版元素
				直接书写在JSP中的HTML内容，看起来就像写HTML一样的方便，但是最终会在被翻译成Servlet的过程中 out.write()直接输出
	2.2脚本表达式
			<%= 表达式%> 接受的是一段java表达式，在JSP翻译到Servlet的过程中，将会计算表达式的值，利用out.write()输出出去
	2.3脚本片段
		<%  %>直接可以在脚本片段中书写java源代码，其中的源代码将会直接拷贝到翻译过来的servlet中的响应位置上。
	2.4JSP声明
		<%! %>在其中可以写java代码，其中的源代码会被拷贝到servlet中的service方法之外，可以利用它来为servlet增加成员方法、成员变量、静态代码块
	2.5JSP注释
		<%-- --%>被jsp注释包围起来的内容将不会被服务器翻译到servlet之中，要注意区分html注释和java注释的区别
		jsp注释不会被翻译到servlet中，会在翻译时遗弃
		java注释不会被编译到class文件中，会在编译时遗弃
		html注释将会当作模版元素，直接输出到浏览器中，浏览器将不会显示html注释中的内容
	2.6JSP指令
		2.6.1page指令
			用来通知翻译引擎，如果翻译当前的JSP
			[ language="java" ] 当前JSP使用的开发语言
			[ extends="package.class" ] 当前jsp翻译成servlet后要继承的类，注意此值必须是一个servlet的子类，一般情况下不要改
			[ import="{package.class | package.*}, ..." ] 导入需要使用到的包 java.lang.*;javax.servlet.*;javax.servlet.jsp.*;javax.servlet.http.*;
			[ session="true | false" ] 用来指定当前页面是否使用session，如果设置为true，则翻译过来的servlet中将会有对session对象的引用，于是可以直接在jsp中使用session隐式对象。但是这将导致一旦访问jsp就会调用request.getSession()方法，可能导致不必要的空间浪费。如果确定jsp中不需要session可以设为false
			[ buffer="none | 8kb | sizekb" ] out隐式对象所使用的缓冲区的大小
			[ autoFlush="true | false" ] out隐式对象是否自动刷新缓冲区，默认为true，不需要更改
			[ isThreadSafe="true | false" ] 翻译过来的servlet是否实现SingleThreadModel
			[ errorPage="relative_url" ] 如果页面出错，将要跳转到的页面，除了在jsp中使用此属性指定错误页面外也可以在web.xml中配置整个web应用的错误页面，如果两个都设置则jsp中的此属性起作用
			[ isErrorPage="true | false" ] 如果设置此属性为true,翻译过来的servlet中将含有Exception隐式对象,其中封装的就是上一个页面中抛出的异常对象
			[ contentType="mimeType [ ;charset=characterSet ]" | "text/html ; charset=ISO-8859-1" ] 和jsp乱码相关的指令,用来指定jsp输出时,设置的Content-Type响应头用来指定浏览器打开的编码
			[ pageEncoding="characterSet | ISO-8859-1" ] 服务器翻译jsp时使用的编码集.如果向防止jsp乱码,应该保证文件的保存编码和jsp翻译成servlet用的编码以及输出到浏览器后浏览器打开的编码一致.此属性一旦设置好,翻译引擎会间接帮我们设置content-type属性.
			[ isELIgnored="true | false" ] 当前页面是否使用el表达式,设置为false时表示启用el,j2ee4.0开始默认支持,j2ee4.0一下做开发时,如果要使用el表达式,需将此属性设置为fals

		2.6.2include指令
			<%@ incluede file=""%> 静态引入其他页面的内容
			*静态引入：在源文件级别进行合并，多个jsp生成一个servlet，最终由这一个servlet生成响应。推荐使用。
			*动态引入：在运行时将多个输出进行合并，多个jsp分别生成多个servlet，最终由这多个servlet生成响应，组成一个输出流，提供响应。执行效率没有静态引入高。
			
		2.6.3taglib指令
			<%@ taglib uri="" prefix=""%>用来引入标签库。
			uri指定被引入.tl文件的名称空间
			prefix 对该名称空间的一个缩写
	
	2.7九大隐式对象
			config
			application
			request
			response
			session
			out
			page
			pageContext
			Exception
		
		2.7.1out对象
			可以将他理解成response.getWriter()获得的PrintWriter.
			它自带一个缓冲区，其大小收page指令中的buffer的设定限制。当缓冲区满或缓冲区被关闭或当前jsp页面结束，则此缓冲区中的内容将被刷新到response.getWriter（）的缓冲区中。
		2.7.2PageContext对象
			（1）获取其它八大隐式对象，可以认为是一个入口对象。
			（2）获取其所有域中的数据
					pageContext操作所有域中属性的方法
					public java.lang.Object?getAttribute(java.lang.String?name,int?scope)
					public void setAttribute(java.lang.String?name, java.lang.Object?value,int?scope)
					public void?removeAttribute(java.lang.String?name,int?scope)
					
					pageContext中代表域的常量
					PageContext.APPLICATION_SCOPE
					PageContext.SESSION_SCOPE
					PageContext.REQUEST_SCOPE
					PageContext.PAGE_SCOPE 
					
					findAttribute方法,在四大域中搜寻属性，搜寻的顺序是page域、request域、session域、application域，从小域到大域开始搜索，如果搜索到就直接获取该值，如果所有域中都找不到，返回一个null(与el表达式不同，此处返回null，对网页是不友好的)
	
			（3）作为域对象使用
					作用的范围：真个jsp页面，是四大作用域中最小的一个。
					声明周期：当对jsp的请求开始时生成，当响应结束时销毁。
			
			（4）跳转到其他资源
						其身上提供了forward和include方法，简化重定向和转发的操作

========================================================================================================
二、标签：JSP既可以用来生成HTML页面，也可以直接书写java源码处理逻辑，这就导致了很多开发者在JSP出现初期，只用JSP做开发，这个JSP页面十分庞大、充满了java源码和HTML标签、许多百分号，逻辑结构混乱，不宜调试程序和页面美化。于是人们希望将java源码从JSP页面中尽量抽离，但是把所有java源码都抽走是不现实的，最基本的获取属性、简单的页面逻辑还是需要的，于是，sun公司就提供了JSP中的标签开发技术，以一个标签代表一种功能的java代码，是整个jsp看起来更像一个HTML，并且不丢失JSP进行逻辑处理的功能。
	1.JSP标签：由sun公司提供，属于jsp规范中的内容，不需要引入第三方标签库
		<jsp:inclue>用来替代request.getRequestDispatcher().include()
		<jsp:forward>用来替代request.getRequestDispatcher().forward()
		<jsp:param>配合前两个标签使用，可以在包含或转发时，带一些请求参数过去
		
		<jsp:useBean id="beanName" class="package.class"  scope="page|request|session|application"/>在指定域中搜寻名字为id设置值的bean，如果没有就在该域中创建一个
		<jsp:setProperty name="beanName" 
			{ 
				property="propertyName" value="{string | <%= expression %>}" |
				property="propertyName" [ param="parameterName" ] | 
				property= "*" 
			}/> 用于为bean的属性负值name指定bean的名字，property指定要设定的属性的名字，value指定设定的属性的值，param用来指定使用哪个请求参数设定该属性，property可以设置为*,用来将所有的请求参数自动设置懂啊bean对应的属性上
			
			<jsp:getProperty name="beanInstanceName" property="PropertyName" />用于获取属性的值输出到输出流中，其中name指定bean的名字，property指定bean上属性的名字


	2.el表达式：替代<%= %>脚本表达式，在j2ee1.4以前默认是不支持el，如果需要需要指定page指令[isELIgnored="true | false" ]为false，j2ee4.0后默认支持el
		2.1获得域中的属性
		${propName}在四个域中搜寻proName属性，输出该值到输出流中
		${pageScope/requestScope/sessionScope/applicationScope.proName}获取指定域中的属性
		${attr[0]}获取域中的集合的指定元素
		${list[0]}获取list中的指定位置元素
		${map.keyName}获取map中指定键的值
		${bean.propName}获取javaBean的属性，可以认为是调用了javaBean的getXXX方法，
		~最重要的一个应用场景：在写路径的时候最好不要把web应用名称写死，java中应该用request.getContextPath去获取。jsp中就用el获取：${pageContext.request.contextPth}
		~.和[]区别使用点的地方都可以用中括号，如果属性名是数字或包含特殊符号（.-）就必须使用中括号。例子：${map["first.name"]}
		2.2进行简单运算
			（1）算数运算：所有参与元算的元素都会被转成数字，如果不能转就报错，空元素参与运算当作没参与。
			（2）关系运算：
			（3）逻辑运算:
			（4）empty/not empty判断一个对象或集合或数组是否为空或长度为0		
			（5）二元表达式 name == null ? "张三" : name;
		2.3获取web开发常用对象
			pageContext:代表pageContext对象，注意和pageScope进行区分
			pageScope：代表page域，可以用来获取page域中的属性
			reqeustScope：代表reqeust域，可以用来获取reqeust域中的属性
			sessionScope：代表session域，可以用来获取session域中的属性
			applicationScope：代表application域，可以用来获取application域中的属性
			param 代表请求参数组成的map集合${param.userName}
			paramValues 代表请求参宿组成的map集合，但是此集合的value是String[]，用来获取一名多值的param
			header 获取请求头组成的map
			headerValues 获取请求头组成的map但是value是一个String[]，用来获取一名多值的head
			cookie 获取cookie组成的map对象，此map的值是一个cookie对象${cookie.cookieName.cookieValue}
			initParam 以map封装的web.xml中配置的整个web应用的初始化参数
		2.4调用java方法
			el表达式可以调用java中的静态方法,分如下步骤完成：
			(1)编写一个类，其中应该包含要使用el调用的静态方法
			(2)编写一个tld文件，描述该方法的调用，在创建tld文件时应选用2.0版本的jsp配置，指定名称空间uri和缩写prefix
			(3)在tld文件中配置方法信息
				<function>
					<name>encodeURL</name>el在调用时所使用的方法名
					<function-class>cn.itheima.util.EncodeURL</function-class>静态方法所在的类全路径名
					<function-signature>
						java.lang.String EncodURL( java.lang.String )//对该方法的描述：返回值类型 方法名(参数类型)
					</function-signature>
				</function>
			(4)在jsp中使用<%@ taglib uri="" prefix="ppp"%>引入tld文件
			(5)在jsp中使用${ppp:encodeURL("xxxx")}调用
	2.5SUN提供的EL函数库，基本上都是对字符串进行操作的函数，请参考张老师写的《第8章 标准标签库_0519.doc》
		
	3.JSTL标签库，在javaee4.0需要导入JSTL相关的jar包，在javaee5.0开始，默认已经包含了此jar包。还要需要用<%@ taglib%>指令引入标签库
		3.1介绍
			JavaServer Pages Standard Tag Library
	 		由JCP（Java Community Process）指定标准
			提供给 Java Web 开发人员一个标准通用的标签函数库
			和 EL 配合来取代传统直接在页面上嵌入 Java 程序（Scripting）的做法，以提高程序可读性、维护性和方便性
			一般我们使用JSTL1.1以上的版本，应为从这个版本开始支持EL表达式
			JSTL1.0默认不支持el表达式，不建议使用
		3.2JSTL标签库
			****核心标签库 (core)  --- c
			国际化标签 fmt
			数据库标签 sql --Servlet
			XML标签  xml
			JSTL函数(EL函数) el
		3.3JSTL核心标签库
		<c:out> 标签用于输出一段文本内容到pageContext对象当前保存的“out”对象中。
		<c:set>标签用于把某一个对象存在指定的域范围内，或者设置Web域中的java.util.Map类型的属性对象或JavaBean类型的属性对象的	属性。  
		<c:remove>标签用于删除各种Web域中的属性
		<c:catch>标签用于捕获嵌套在标签体中的内容抛出的异常，其语法格式如下：<c:catch [var="varName"]>nested actions</c:catch>
		<c:if test=“”>标签可以构造简单的“if-then”结构的条件表达式 
		<c:choose>标签用于指定多个条件选择的组合边界，它必须与<c:when>和<c:otherwise>标签一起使用。使用<c:choose>，<c:when>和<c:otherwise>三个标签，可以构造类似 “if-else if-else” 的复杂条件判断结构。
		<c:forEach>标签用于对一个集合对象中的元素进行循环迭代操作，或者按指定的次数重复迭代执行标签体中的内容。 
		<c:forTokens>用来浏览一字符串中所有的成员，其成员是由定义符号所分隔的
		<c:param>标签  	在JSP页面进行URL的相关操作时，经常要在URL地址后面附加一些参数。<c:param>标签可以嵌套在<c:import>、<c:url>或<c:redirect>标签内，为这些标签所使用的URL地址附加参数。
 		<c:import> 标签,实现include操作
 		<c:url>标签用于在JSP页面中构造一个URL地址，其主要目的是实现URL重写。URL重写就是将会话标识号以参数形式附加在URL地址后面 
		<c:redirect>标签用于实现请求重定向
=======================================================================================================================================================
三.自定义标签
	1.自定义标签技术出现的原因：虽然有第三方组织提供了很多标签，但是这些都是一些通用标签，开发中常常需要根据业务需求使用jsp页面，这个时候通用的标签就不够用了，我们需要自己去开发标签库。
	2.标签处理类生命周期
		2.1传统标签处理类的生命周期：传统标签处理类在标签被第一次使用时创建实例，从此驻留内存为后续请求提供服务，其中每次标签执行都会依次执行setPageContext(PageContext pc) 、setParent(Tag t) 、doStartTag() 、doEndTag() 、~在服务器停止之前调用release() 
		2.2简单标签的声明周期：setJspContext(JspContext pc) 、setParent(JspTag parent) 如果没有父标签就不调用、如果有属性就调用对应的setXXX方法如果有el表达式，则计算结果后传入、如果有标签体调用setJspBody(JspFragment jspBody)  、doTag() 执行标签体处理.每次访问该标签时都创建简单标签实例，每次使用后销毁该实例。


	3.传统标签的继承结构：
JspTag
		（1）传统标签
			Tag接口-----定义了一个标签处理类应具有的最基本的方法和属性（EVAL_BODY_INCLUDE dostart方法返回表示执行标签体，SKIP_BODY dostart方法用，跳过标签体。EVAL_PAGE 
用在doendtag里通知后续页面继续执行，SKIP_PAGE doendtag里通知后续页面不再执行）
			|
			|----IterationTag接口 (提供了doAfterBody() 在标签体执行过后立即执行，并提供EVAL_BODY_AGAIN 供doafterbody方法返回表示要重新执行标签体)
						|
						|----TagSupport类(提供了对pageContext的引用)
						|
						|--BodyTag接口（EVAL_BODY_BUFFERED 在doStartTag方法中返回通知标签处理器去缓存标签体bodyContent）
								|
								|----BodyTagSupprot类		（getBodyContent() 获取缓存对象bodyContent）
			
​		
		（2）简单标签（简单标签的标签体不能包含脚本内容，所以tld文件中配置body-content时不能配置为JSP）
			SimpleTag接口
			|
			|---SimpleTagSupport实现类（getJspContext() 获取代表jsp页面的pageContext，getJspBody() 代表标签体内容的JspFragment对象）
			
			JspFragment：invoke(Writer out) 此方法一经调用会将标签体输出到对应的流中。如果你直接给一个null默认输出到pageContext.getOut()中
			如果doTag（）方法抛出SkipPageException异常，标签后的jsp页面就不再执行。
	
		（3）自定义标签声明属性：在标签处理类中提供setXXX方法，并在tld文件中通过<attribute>标签声明属性的特性即可。
	
	4.自定义标签应该具有的最基本的功能
			实验1：控制jsp页面某一部分内容是否执行。<c:if>
			实验2：控制整个jsp页面是否执行。
			实验3：控制jsp页面内容重复执行。<c:forEach>
			实验4：修改jsp页面内容输出。<c:out> HTML转义
			实验5：获取属性
	5.tld文件配置自定义标签
	<tag>
		<name>demo10</name>-----定义标签的名字
		<tag-class>cn.itheima.simple.Demo10</tag-class>----标签处理类
		<body-content>scriptless</body-content>----标签体内容JSP表示任意jsp内容，empty代表只能为空、scriptless带表不能包含脚本内容、tagdependent代表其中的内容是用来给服务器用的，不是输出到浏览器的内容
		<attribute>
			<name>times</name>---属性的名字(名字必须有)
			<required>true</required>---是否是一个必须的属性
			<rtexprvalue>true</rtexprvalue>---是否支持el表达式
			<type>int</type>---属性的类型
		</attribute>
	</tag>