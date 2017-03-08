# 推荐使用String直接量赋值
`new String("zhongguo")`这样的是不检查字符串池的。

# 正确使用String、StringBuffer、StringBuilder
String类不可更改；
StringBuffer和String一样在内存中保存一个有序的字符序列，但是其对象的值可变。
StringBuilder和StringBuffer基本相同，不同点是后者是线程安全的，前者是线程不安全的。因而，线程安全的StringBuffer性能要差很多。

1. 使用String类的场景
字符串不经常变化的场景：常量的声明、少量的变量运算等；

2. 使用StringBuffer的场景
频繁进行字符串的运算，并且运行在多线程的环境中，如XML解析、HTTP参数解析和封装等。

3. 使用StringBuilder类的场景
频繁进行字符串的运算，并且运行在单线程环境中，如SQL语句拼装、JSON封装等。

# 自由选择字符串拼接方法
`+ concat StringBuilder/StringBuffer的append方法`
可以通过看源码分析其性能差异。

# 推荐在复杂字符串操作中使用正则表达式
简单操作中就不要用了。

# 强烈建议使用UTF编码
UTF是UNICODE的存储和传输格式，是为了解决UNICODE的高位占用冗余空间而产生的。
