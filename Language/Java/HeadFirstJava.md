 
java开 MidiEvent，权限不足，需要用管理员权限打开cmd。
![](http://opkk27k9n.bkt.clouddn.com/17-7-5/75612335.jpg)


在class目录下，
创建一个manifest.txt文件，带有下面一行：
Main-Class: MyApp

jar -cvmf manifest.txt app1.jar *.class

执行
java -jar app1.jar

 
 

列出JAR内容：
jar -tf packEx.jar

解压JAR内容：
jar -xf packEx.jar


![](http://opkk27k9n.bkt.clouddn.com/17-7-5/11531743.jpg)
![](http://opkk27k9n.bkt.clouddn.com/17-7-5/30649382.jpg)
 ![](http://opkk27k9n.bkt.clouddn.com/17-7-5/59145846.jpg)
 ![](http://opkk27k9n.bkt.clouddn.com/17-7-5/74717804.jpg)
 
# 带有断言的执行
java -ea Test