问:  为什么Runtime.exec("ls")没有任何输出?  
答:  
　　调用Runtime.exec方法将产生一个本地的进程,并返回一个Process子类的实例,该实例可用于控制进程或取得进程的相关信息. 由于调用Runtime.exec方法所创建的子进程没有自己的终端或控制台,因此该子进程的标准IO(如stdin,stdou,stderr)都通过Process.getOutputStream(),Process.getInputStream(), Process.getErrorStream()方法重定向给它的父进程了.用户需要用这些stream来向 子进程输入数据或获取子进程的输出. 所以正确执行Runtime.exec("ls")的例程如下:  
try  
{  
process = Runtime.getRuntime().exec (command);  
InputStreamReader ir=newInputStreamReader(process.getInputStream());  
LineNumberReader input = new LineNumberReader (ir);  
String line;  
while ((line = input.readLine ()) != null)  
System.out.println(line);  
}  
catch (java.io.IOException e){  
System.err.println ("IOException " + e.getMessage());  
}  