## 使用 register 修饰符的注意点
register 变量必须是一个单个的值，并且其长度应小
于或等于整型的长度。 而且 register 变量可能不存放在内存中，所以不能用取址运算符“&”
来获取 register 变量的地址。

## `sizeof (int) *p`
32 位系统下：
```C
int*p = NULL;
sizeof(p)的值是多少？
sizeof(*p)呢？
inta[100];
sizeof (a) 的值是多少？
sizeof(a[100])呢？//请尤其注意本例。
sizeof(&a)呢？
sizeof(&a[0])呢？
intb[100];
void fun(intb[100])
{
sizeof(b);// sizeof (b) 的值是多少？
}
```


```c
intmain()
{
char a[1000];
inti;
for(i=0; i<1000; i++)
{
a[i] = -1-i;
}
printf("%d",strlen(a));//255
return 0;
}
```

### Questions
1），按照我们上面的解释，那-0 和+0 在内存里面分别怎么存储？
2），inti = -20;
unsigned j = 10;
i+j 的值为多少？为什么？
3）， 下面的代码有什么问题？
unsigned i ;
for (i=9;i>=0;i--)
{
printf("%u\n",i);
}


在 switch case 语句中能否使用 continue 关键字？为什么？


在多重循环中，如果有可能，应当将最长的循环放在最内层，最短的循环放
在最外层，以减少 CPU 跨切循环层的次数。

## const & define
const节省空间，避免不必要的内存分配，同时提高效率
constint*p; // p 可变，p 指向的对象不可变
intconst*p; // p 可变，p 指向的对象不可变
int*constp; // p 不可变，p 指向的对象可变
constint*constp; //指针 p 和 p 指向的对象都不可变

先忽略类型名（编译器解析的时候也是忽略类型名），我们看 const 离哪个近。“近水楼
台先得月”，离谁近就修饰谁。


### Q
constvolatile inti=10；这行代码有没有问题？如果没有，那 i 到底是什么
属性？

## 柔性数组
C99 中，结构中的最后一个元素允许是未知大小的数组，这就叫做柔性数组成员，但结
构中的柔性数组成员前面必须至少一个其他成员。柔性数组成员允许结构中包含一个大小可
变的数组。sizeof 返回的这种结构大小不包括柔性数组的内存。包含柔性数组成员的结构用
malloc ()函数进行内存的动态分配，并且分配的内存应该大于结构的大小，以适应柔性数组
的预期大小。

```
typedef structst_type
{
inti;
inta[0];
}type_a;
```
有些编译器会报错无法编译可以改成：
```
typedef structst_type
{
inti;
inta[];
}type_a;
```

## 如何用程序确认当前系统的存储模式（大端or小端）？
大端模式（Big_endian）：字数据的 高字节存储在 低地址中，而字数据的 低字节则存放
在 高地址中。
小端模式（Little_endian）：字数据的 高字节存储在 高地址中，而字数据的 低字节则存放
在 低地址中。

```C
int checkSystem()
{
    union check
    {
        int i;
        char ch;
    }c;
    c.i = 1;
    return(c.ch==1);    //大端返回0
}
```

### Q
A），枚举能做到事，#define 宏能不能都做到？如果能，那为什么还需要枚举？
B)，sizeof（ColorVal）的值为多少？为什么？
