先贴测试代码：
public class HelloWorld 
{ 
public static void main(String[] argv){ 
    try{ 
System.out.println("中文");//1 
System.out.println("中文".getBytes());//2 
System.out.println("中文".getBytes("GB2312"));//3 
System.out.println("中文".getBytes("ISO8859_1"));//4 
  
System.out.println(new String("中文".getBytes()));//5 
System.out.println(new String("中文".getBytes(),"GB2312"));//6 
System.out.println(new String("中文".getBytes(),"ISO8859_1"));//7 
  
System.out.println(new String("中文".getBytes("GB2312")));//8 
System.out.println(new String("中文".getBytes("GB2312"),"GB2312"));//9 
System.out.println(new String("中文".getBytes("GB2312"),"ISO8859_1"));//10 
  
System.out.println(new String("中文".getBytes("ISO8859_1")));//11 
System.out.println(new String("中文".getBytes("ISO8859_1"),"GB2312"));//12 
System.out.println(new String("中文".getBytes("ISO8859_1"),"ISO8859_1"));//13 
} 
catch(Exception e){ 
e.printStackTrace(); 
} 
  } 
} 
![](http://opkk27k9n.bkt.clouddn.com/17-7-5/8336928.jpg)
 
javac 是以系统默认编码读入源文件，然后按unicode进行编码的，并且默认输入输出都是操作系统的默认编码。。