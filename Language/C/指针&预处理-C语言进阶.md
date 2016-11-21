---
date: 2016-04-18 16:30
status: public
title: 指针&预处理-C语言进阶
---

# 不同类型的指针变量在内存中的存储区域分配
```
#include<stdio.h>
void main(int argc, char* argv[]){
    int a[10];
    printf("a的值为：\t%d\n",a);
    printf("&a的值为：\t%d\n",&a);
    printf("a+1的值为：\t%d\n",a+1);
    printf("&a+1的值为：\t%d\n",&a+1);
}
```

![](~/16-33-16.jpg)
&a就相当于指向一个int[10]的指针变量,`int a[10];`变形为`int* (&a)[10];`
则&a到&a+1的变化就是它指向的变量所占用的内存单元的大小：4Byte*10=40Byte.


# 求二维数组第i行的起始地址
`printf("%d\n",arr+i);`
`printf("%d\n",*(arr+i);`
`printf(%d\n",&arr[i]);`
`printf("%d\n",arr[i]);`
结果是一样的~~~~

# 联合（共用体）
```c:n
#include<stdio.h>
union uni{
char a;
int b;
int c;
};
void main(){
union uni y;
y.a = 0x1345;
y.b = 0xc4df;
y.c = 0xb548;
printf("%x\n",y.a);
printf("%x\n",y.b);
printf("%x\n",y.c);
}
```

![](~/18-08-00.jpg)
a显示48是因为a为char，显示低两位（占据低两位内存）。

**联合的大小是最大的那个变量的大小，其中所有变量共用同一块内存空间。**

# 位域
形如：
```
struct ab{
int a:4;
char b:6;
...
}ab;
```

`#pragma pack (2)` 指定对齐字节数（默认4字节对齐）

# 带参数的宏替换
不知道为何原书中这一节只说在Linux+gcc环境下。


```C:n
define min (x,y) ({typeof (x) _x=(x);typeof (y) _y=(y);
(void)(&_x==&_y);
_x<_y?_x:_y;})

define min (x,y) ({typeof (x) _x=(x);typeof (y) _y=(y);
(void)(&_x==&_y);
_x>_y?_x:_y;})
```

代码`(void)(&_x==&_y);`的功能相当于执行一个简单的判断操作，判断x和y的类型是否一致。
例如
```
#define print () ((void)(3))
print();
```
则print()好像不会被调用，因为接下来的代码都不会被调用。

为什么不用`typeof(x)==typeof(y)`而要用`typeof (x) _x=(x);typeof (y) _y=(y);`呢？
因为就像`char==int`一样这是不被允许的。

而执行`typeof (y) _x=(y);`是为了防止y为一个表达式的情况，如`y=i++`。

## 变参
`define print (...) printf(__VA_ARGS__)`

`define printf (tem,...) fprintf(stdout,tem,##__VA_ARGS__)`
这里用`##`是因为`##`前有一个逗号。不加`##`编译器会报错。

```
#include<stdio.h>
#define return_exam (p) if(!(p))\
{printf ("error:"#p"file_name: %s\tfunction_name: %s\tline: %d.\n",\
__FILE__,__func__,__LINE__);return 0;}

int print(){
return_exam(0);
}

int main(int argc,char* argv[]){
print();
printf("hello world!\n");
return 0;
}
```
Linux+gcc，运行结果如下：
```
error: 0 file_name: arg.c function_name: print line:9.
hello world!
```


p前面的#的作用就是对其后面的宏参数进行字符串化操作，就是替换后左右加双引号：`"#p"` ==> `""p""`

## 嵌套宏替换
逐层扩展。

# 常见宏错误
```
#define INT_P int*
INT_P p1,p2;
```
则p2是一个int型，而非int*~~~
Because，把`INT_P`替换为真正的`int*`则一目了然。

```
#define SUM(x,y) x+y

int s = SUM(x,y) * 10;
```
很不幸，s=x+y*10……
所以可以加个括号~


说多了都是宏扩展的错，一般都是加括号解决~~~

不过像a++这种，就得用上文中的方法解决啦。




# 条件编译
```
#if expression
...
#else
...
#endif
```
```
#ifdef 标识符
...
#else
...
#endif
```
```
#ifndef 标识符
...
#else
...
#endif
```
**expression不能用变量。（不能用sizeof等在编译时求值的操作符）**

# pragma指令
作用是设置编译器的状态或指示编译器完成一些特定动作。
1. `#pragma message ("消息")`
示例：
```
#define STR
#ifdef STR
#pragma message ("STR已定义")
#endif
```

编译时会打印消息，但是不会出现在程序最终运行结果中。

2. `#pragma once`
保证头文件只被编译一次。

3. `#pragma hdrstop`
表示编译头文件到此为止，后面的不再编译。

4. `#pragma pack ()`
字节对齐。参数为空则取消之前设定的。

`#pragma pack(push)`是保存当前默认的字节对齐方式，`#pragma pack (pop)`是恢复默认的字节对齐方式。

5. `#pragma warning ()`
`#pragma warning (disable: M N; enable J; once: H; error: K)`表示不显示M号和N号的警告信息，显示J号警告信息，H号警告信息只报告一次，把K号警告信息当作一个错误处理，也可以分开实现（以分号为界）。