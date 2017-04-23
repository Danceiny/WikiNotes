# 题目
实现Singleton（单例）模式


## 不好的解法一：只适用于单线程环境

```c#
public sealed class Singleton1
{
    private Singleton1()
    {

    }
    private static Singleton1 instance = null;
    public static Singleton1 Instance{
        get
        {
            if (instance == null)
                instance = new Singleton1();
            return instance;
        }
    }
}
```

## 不好的解法二：可在多线程环境中工作，但效率不高

```c#
public sealed class Singleton2
{
    private Singleton2()
    {

    }
    private static readonly object syncObj = new Object();
    private static Singleton2 instance = null;
    public static Singleton2 Instance{
        get
        {
            lock(syncObj){
                if (instance == null)
                instance = new Singleton2();
            }
            return instance;
        }
    }
}
```

## 可行的解法：加同步锁前后 两次 判断实例是否已存在
```c#
public sealed class Singleton3
{
    private Singleton3()
    {

    }
    private static readonly object syncObj = new Object();
    private static Singleton3 instance = null;
    public static Singleton3 Instance{
        get
        {
            if(instance==null)
            {
                lock(syncObj){
                if (instance == null)
                    instance = new Singleton3();
                }
            }
            
            return instance;
        }
    }
}
```
缺点：实现复杂，容易出错。

## 更好的解法一：利用静态构造函数
```c#
public sealed class Singleton3
{
    private Singleton3()
    {

    }

    private static Singleton3 instance = new Singleton3();
    public static Singleton3 Instance{
        get
        {
            return instance;
        }
    }
}
```


## 更好的解法二：实现按需创建实例

```c#
public sealed class Singleton3
{
    private Singleton3()
    {

    }

    private static Singleton3 instance = new Singleton3();
    public static Singleton3 Instance{
        get
        {
            return Nested.instance;
        }
    }
    class Nested
    {
        static Nested{}
        internal static readonly Singleton3 instance = new Singleton3();
    }
}
```