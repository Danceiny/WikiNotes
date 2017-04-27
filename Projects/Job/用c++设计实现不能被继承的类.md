# 题目
C#中有sealed，Java中有final。C++中没有类似的关键字，需要自主实现。


## 常规解法：把构造函数设为私有函数

问题：只能得到位于堆上的实例，得不到位于栈上的实例。

## 新奇解法：利用虚拟继承

```cpp
template <typename T>class MakeSealed
{
    friend T;   //VS可行，GCC不可行
private:
    MakeSealed(){}
    ~MakeSealed(){}
};

class SealedClass2:virtual public MakeSealed<SealedClass2>
{
public:
    SealedClass2(){}
    ~SealedClass2(){}
};
```