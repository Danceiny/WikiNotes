# 用偶判断，不用奇判断
`-1 % 2 != 1`

因为Java中的%取余运算：
```Java
public static int remainder(int dividend,int divisor){
    return dividend - dividend/divisor*divisor;
}
```
所以`-1 % 2 == -1`
# 用整数类型处理货币
浮点数不精确。

解决之道：
1. 使用BigDecimal类
2. 使用整型（先放再缩）

# 避免隐式类型转换

# 单元测试中的边界测试
如果一个方法接收的是int类型的参数，那0、正最大、负最小三个值是一定要测试的。
即便前端有用过JavaScript实现校验，但是只能限制普通用户，因此后台还是要有边界检查。

# 四舍五入
-10 == Math.round(-10.5) != Math.round(10.5) == 11

**银行家舍入近似算法(Banker's Round)**
舍去位的数值小于5时直接舍去，大于等于6时进位后舍去，等于5时分两种情况：
1. 5后面还有其他非0数字，则进位后舍去；
2. 5后面是0，则根据5前一位数的奇偶性判断是否进位，奇数进偶数舍。

直接使用RoundingMode类提供的Round模式即可。
`BigDecimal i = d.multiply(r).setScale(2,RoundingMode.HALF_EVEN);`

其他舍入模式还有：
- ROUND_UP
>只要舍弃位非0即进位

- ROUND_DOWN
>不存在进位

- ROUND_CEILING
> 如果是正数，类似ROUND_UP；
> 如果是负数，类似ROUND_DOWN。
> Math.round的默认模式

- ROUND_FLOOR
> 如果是负数，类似ROUND_UP；
> 如果是正数，类似ROUND_DOWN。

- HALF_UP
> 最最经典的四舍五入模式

- HALF_DOWN
> 5舍去，其他同四舍五入。

- HALF_EVEN
> 银行家舍弃算法。


# 提防包装类型的null值
```java
List<Integer> list = new ArrayList<Integer>();
list.add(null); // will throw NullPointerException
```
包装类型参与运算时，要做null值校验。

# 谨慎包装类型的大小比较

# 优先使用整型池
```Java
public static void main(String[] args) {
    Scanner input = new Scanner(System.in);
    while(input.hadNextInt()){
        int ii = input.nextInt();
        System.out.println("\n===="+ii+"的相等判断=====");
        Integer i = new Integer(ii);
        Integer j = new Integer(ii);
        System.out.println("new 产生的对象："+(i==j));

        i = ii;
        j = ii;
        System.out.println("基本类型转换的对象"+(i==j));

        i = Integer.valueOf(ii);
        j = Integer.valueOf(ii);
        System.out.println("valueOf产生的对象："+(i==j));
    }
}
```

输入三个数字127\128\555,结果依次为：
127 new=>false, basic=>true, valueOf=>true
128 new=>false, basic=>false, valueOf=>false
555 new=>false, basic=>false, valueOf=>false

1. new=>false,因为==比较的是引用，肯定不等嘛；
2. 装箱生成的对象（装箱动作通过valueOf实现），其实现代码如下：
```java
public static Integer valueOf(int i){
    final int offset = 128;
    if(i>=-128 && i<=127){
        //must cache
        return IntegerCache.cache[i+offset];
    }
    return new Integer(i);
}
```
显然，-128到127之间的int类型，直接从cache数组中获取。那缓存中又是什么呢？
```java
//整型池：节省内存，提高性能
static final Integer cache[] = new Integer[-(-128) + 127 + 1];  //????
static {
    for(int i=0; i<cache.length; i++)
        cache[i] = new Ineger(i-128);
}

```


# 优先选择基本类型
自动装箱可能会引起严重问题。


# 不要随便设置随机种子
随机数和种子之间的关系遵从以下两个规则：
1. 种子不同，产生不同的随机数；
2. 种子相同，即使实例不同也产生相同的随机数。

`new Random(1000)`显式地设置了随机种子为1000.

Java中有两种方法可以获得不同的随机数：
- 通过java.util.Random类获得随机数的原理和Math.random方法相同，


**上一章：Java开发中的通用方法和准则**
**下一章：类、对象及方法**
