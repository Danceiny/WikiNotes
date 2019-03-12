## 一个最最简单的图片浏览应用

### index.jsp
```jsp
<%@ page language="java" import="java.util.*" pageEncoding="ISO-8859-1"%>
<%
	String path = request.getContextPath();
	String basePath = request.getScheme() + "://"
			+ request.getServerName() + ":" + request.getServerPort()
			+ path + "/";
	String demopath = basePath + "demo";
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<base href="<%=basePath%>">
<title>My JSP 'index.jsp' starting page</title>
</head>
<body>
	<a href="<%=demopath%>"> demo </a>
</body>
</html>
```
图片和index.jsp位于同一级目录下。

### WEB-INF/web.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="2.5" 
	xmlns="http://java.sun.com/xml/ns/javaee" 
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
	http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
  <display-name></display-name>
  <servlet>
    <servlet-name>Demo</servlet-name>
    <servlet-class>cn.itcast.servlet.Demo</servlet-class>
  </servlet>

  <servlet-mapping>
    <servlet-name>Demo</servlet-name>
    <url-pattern>/demo</url-pattern>
  </servlet-mapping>	
  <welcome-file-list>
    <welcome-file>index.jsp</welcome-file>
  </welcome-file-list>
</web-app>
```

### demo.java

```java
import java.io.FileInputStream;
import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class Demo extends HttpServlet {

	public void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		FileInputStream fis = new FileInputStream(this.getServletContext()
				.getRealPath("/4.jpg"));

		byte[] b = new byte[1024];
		int len = -1;
		while ((len = fis.read(b)) != -1) {
			response.getOutputStream().write(b, 0, len);
		}

	}

	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		doGet(request, response);
	}

}
```