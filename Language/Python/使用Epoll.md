## Blocking Socket Programming Examples





## Asynchronous Socket Programming Examples with epoll

使用epoll的程序通常会执行以下步骤：

1. 创建一个epoll对象
2. 让epoll对象监听指定sockets上的指定事件
3. 告诉epoll对象自最后一次查询后哪些sockets上可能发生了指定事件
4. 在这些sockets上执行一些动作
5. 让epoll对象修改一列sockts和/或需要监听的事件
6. 重复3~5直至完成
7. 销毁epoll对象

```python
import socket
import select
EOL1 = b'\n\n'
EOL2 = b'\n\r\n'
response = b'HTTP/1.0 200 OK\r\nDate: Mon, 1 Jan 1996 01:01:01 GTM\n\n'
response += b'Content-Type: text/plain\r\nContent-Length: 12\r\n\r\n'
response += b'Hello, world!'

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
serversocket.bind(('0.0.0.0', 8080))
serversocket.listen(1)
serversocket.setblocking(0) #默认socket处于阻塞模式

epoll = select.epoll() # 创建一个epoll对象
#在serversocket上注册读事件，读事件发生在serversocket每次接受socket连接时
epoll.register(serversocket.fileno(), select.EPOLLIN)
try:
	#文件描述符与其对应的网络连接对象的映射
	connections = {}
	requests = {}
	responses = {}
	while True:
		# 查询epoll对象，看是否有感兴趣的事件发生
		# 参数1表明最多会等待一秒钟
		# 如果这次查询之前有感兴趣的事件发生，这次查询将会立即返回这些事
		events = epoll.poll(1)
		# 事件是一个(fileno, 事件code)的元组
		for fileno, event in events:
			# 如果serversocket上发生了读事件，那么意味着有一个有新连接
			if fileno == serversocket.fileno():
				connection, address = serversocket.accept()
				# 将新的socket设为非阻塞
				connection.setblocking(0)
				# 给新创建的socket注册读事件（EPOLLIN），表明将有数据请求
				epoll.register(connection.fileno(), select.EPOLLIN)
				connections[connection.fileno()] = connection

				# 收集各客户端来的请求
				requests[connection.fileno()] = b''
				responses[connection.fileno()] = response
			elif event & select.EPOLLIN:
				# 如果发生了读事件，从客户端读取数据
				requests[fileno] += connections[fileno].recv(1024)
				if EOL1 in requests[fileno] or EOL2 in requests[fileno]:
					# 一旦请求被完全接收了，注销这个socket的读事件，然后注销？
					# 表明响应即将开始
					# 当向客户端发送响应的时候，读事件发生
					epoll.modify(fileno, select.EPOLLOUT)
					# 打印出完整的请求
					# 结果表明： 尽管客户端请求交错发送，每一个客户端的请求仍然有序
					print('-'*40+'\n'+requests[fileno].decode('utf-8'))
			elif event & select.EPOLLOUT:
				# 如果发生了写事件，向客户端发送响应
				# 每次向客户端发送一定长度的响应内容，每次都更新余下待发送的
				byteswritten = connections[fileno].send(responses[fileno][:1024])
				responses[fileno] = responses[fileno][byteswritten:]
				# 响应已经发送完毕，一次请求/响应周期完成，不再监听该socket
				if len(responses[fileno]) == 0:
					epoll.modify(fileno, 0)
				# 告诉客户端，关闭连接
				connections[fileno].shutdown(socket.SHUT_RDWR)
			# HUP 事件表明客户端断开了连接
			elif event & select.EPOLLHUP:
				# 注销对断开客户端socket的事件监听
				epoll.unregister(fileno)
				# 关闭连接， 服务端关闭
				connections[fileno].close()
				del connections[fileno]
finally:
	epoll.unregister(serversocket.fileno())
	epoll.close()
	serversocket.close()
```

epoll有两种模式：http://man7.org/linux/man-pages/man7/epoll.7.html

- edge-triggered 边缘触发
  - fd准备好读写时，不会不停告诉你这个fd准备好了，会告诉一次，直到下一次`EAGAIN`。因此ET会更高效，尽管更复杂。
  - ET模式下，只有当被监视的fd读写状态发生改变时才会传递事件
    - 假设下述场景
      - 1. 假设pip读取端的文件描述符rfd已经注册到epoll上
        2. pipe写入端向pipe中写入了2KB数据（可读事件发生）
        3. 调用epoll_wait，rfd将会作为可读文件描述符返回（数据消费完毕）
        4. pipe读取端从rfd缓冲区读入了1kB数据（该读操作并未将缓冲区的数据消费完）
        5. 调用epoll_wait（可能无限期阻塞）
  - 使用ET模式的epoll建议使用下述处理流程：
    - 将文件描述符设为nonblocking
    - 只有当read(2) 和 write(2) 返回`EAGAIN`才开始等待事件。
- level-triggered 电平触发
  - fd准备好读写时，会不停告诉你这个fd准备好了





## Performance Considerations

### Listen Backlog Queue Size

listen()函数接受一个可选参数 Listen backlog queue size, backlog通常被用来指定队列能容下的链接的个数。



TCP建立连接的三次握手过程中，连接需要先经历SYN RECEIVED状态才能到达最终的ESTABLISHED状态，处于ESTABLISHED状态的连接才能被accept系统调用返回给应用。因此， TCP/IP协议栈通常有两种实现backlog queue的策略：

1. 使用一个队列，大小由backlog参数决定。这个队列会包含SYNRECEIVED 和ESTABLISHED两种状态的连接，只有处于ESTABLISHED状态的连接才会返回给用户程序中的accept系统调用。
2. 使用两个队列，一个SYN队列（未完成连接队列），和一个accept队列（连接完成队列）。accept系统调用只从accept队列中消耗连接。该策略下，backlog参数决定的是accept队列的大小。

linux使用了两个不同的队列，一个由系统设定指定大小的SYN队列，一个由应用程序指定大小的accept队列。



Linux的TCP实现中，当accept队列满时，服务器收到了处于三次握手阶段最后的ack数据包后，默认会忽略这个数据包。



