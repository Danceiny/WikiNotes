# Chapter 2
1. javap
2. javac -g:vars 输出相应信息
javac -g:vars,lines


# Chapter 3
看一段程序到底加载了什么类：
`java -XX:+TraceClassLoading`

跟踪类的卸载：`-XX:+TraceClassUnloading`

JIT优化后的本地代码存放在CodeCache中，`-XX:+Use CodeCache Flushing`清理，`-XX:ReservedCodeCacheSize=64m`修改（默认48）。

同名类出现在不同的jar包中，可用`XXX.getClass().getResource("").getPath()`。
`getResource(String)`传参为package名称时会找到jar包内部，空字符串则不会。

## 字节码增强
1. 内存中获取原始字节码，通过开源API来修改其byte[]数组，得到新的byte[]数组。
2. 将新的byte[]数组写到PermGen区域，也就是加载它或替换原来的Class字节码（也可在进程外调用完成）。

开源API：ASM javassist BCEL SERP CGLib
