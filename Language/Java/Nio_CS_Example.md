# Server

```java
package cn.itcast_02_nio;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.nio.ByteBuffer;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;

public class TransferToServer {

	public static void main(String[] args) throws IOException {
		// 创建socket channel
		ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
		ServerSocket ss = serverSocketChannel.socket();
		ss.setReuseAddress(true);// 地址重用
		ss.bind(new InetSocketAddress("localhost", 9026));// 绑定地址
		System.out.println("监听端口 : "
				+ new InetSocketAddress("localhost", 9026).toString());

		// 分配一个新的字节缓冲区
		ByteBuffer dst = ByteBuffer.allocate(4096);
		// 读取数据
		while (true) {
			SocketChannel channle = serverSocketChannel.accept();// 接收数据
			System.out.println("Accepted : " + channle);
			channle.configureBlocking(true);// 设置阻塞，接不到就停
			int nread = 0;
			while (nread != -1) {
				try {
					nread = channle.read(dst);// 往缓冲区里读
					byte[] array = dst.array();//将数据转换为array
					//打印
					String string = new String(array, 0, dst.position());
					System.out.print(string);
					dst.clear();
				} catch (IOException e) {
					e.printStackTrace();
					nread = -1;
				}
			}
		}
	}
}
```



## Client

```java
package cn.itcast_02_nio;

import java.io.FileInputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.channels.FileChannel;
import java.nio.channels.SocketChannel;

public class TransferToClient {

	@SuppressWarnings("resource")
	public static void main(String[] args) throws IOException {
		long start = System.currentTimeMillis();
		// 打开socket的nio管道
		SocketChannel sc = SocketChannel.open();
		sc.connect(new InetSocketAddress("localhost", 9026));// 绑定相应的ip和端口
		sc.configureBlocking(true);// 设置阻塞
		// 将文件放到channel中
		FileChannel fc = new FileInputStream("C:/sss.txt").getChannel();// 打开文件管道
		//做好标记量
		long size = fc.size();
		int pos = 0;
		int offset = 4096;
		long curnset = 0;
		long counts = 0;
		//循环写
		while (pos<size) {
			curnset = fc.transferTo(pos, 4096, sc);// 把文件直接读取到socket chanel中，返回文件大小
			pos+=offset;
			counts+=curnset;
		}
		//关闭
		fc.close();
		sc.close();
		//打印传输字节数
		System.out.println(counts);
		// 打印时间
		System.out.println("bytes send--" + counts + " and totaltime--"
						+ (System.currentTimeMillis() - start));
	}
}
```





## 

