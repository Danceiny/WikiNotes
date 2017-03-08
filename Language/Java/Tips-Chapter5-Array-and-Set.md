# 性能考虑，数组是首选
# 若有必要，使用变长数组
通过对数组扩容，“曲线救国”。
```Java
public static<T> T[] expandCapacity(T[] datas, int newLen)
{
    newLen = newLen < 0 ? 0 : newLen;
    return Arrays.copyOf(datas,newLen);
}
```
# 警惕数组的浅拷贝
数组中的元素没有实现Serializable接口。
解决方案就是遍历每一个元素，重新生成数组。

经典方案：List.toArray => Arrays.copyOf => Array.toList (注意浅拷贝)

# 在明确的场景下，为集合指定初始容量
对于ArrayList，明确的数量的1.5倍，定为初始容量。


# 适时选择多种最值算法
1. 自行实现，快速查找最大值
```java
public static int max(int[] data){
    int max = data[0];
    for(int i:data){
        max = max > i ? max : i;
    }
    return max;
}
```

2. 先排序，后取值
```java
public static int max(int[] data){
    Arrays.sort(data.clone());
    return data[data.length-1];
}
```
以上两种方法，在数组少于1万时基本没有差别。

3. 剔除重复数据的排序
```java
//取第二大
public static int getSecond(Integer[] data){
    List<Integer> dataList = Arrays.asList(data);
    TreeSet<Integer> ts = new TreeSet<Integer> (dataList);
    return ts.lower(ts.last());
}
```

# 避开基本类型数组转换列表陷阱
应该要这样：
```java
Integer[] data = {1,2,3,4,5};   //而不是 int[] data = {}.如果这样，陷阱就是会包成一个int[]类型放到list中，而不是多个int。
List list = Arrays.asList(data);
```

# asList方法产生的List对象不可更改
```java
List<Week> list = Arrays.asList(workDays);
list.add(Week.Sat);//编译通过，运行异常

List<String> names = Arrays.asList("张三"，"李四","王五");
//重大隐患：列表长度无法修改
```

# 不同的列表选择不同的遍历方法
```java
public static int average(List<Integer>list)
{
    int sum = 0;
    if(list instanceof RandomAccess)
    {
        //可以随机存取，则使用下标遍历
        for(int i=0,sizo=list.size();i<size;i++)
        {
            sum += list.get(i);
        }
    }
    else
    {
        //有序存取，使用foreach方式
        for(int i:list)
        {
            sum += i;
        }
    }
    //计算平均值
    return sum / list.size();
}
```

# 频繁插入和删除时使用LinkedList
## 插入
ArrayList插入效率很低。
LinkedList是一个双向列表。
## 删除
还是LinkedList胜。
## 修改
因为LinkedList是顺序存取的，定位元素必然是一个遍历过程，效率不如ArrayList。


add方法的写操作二者没有什么差别。

# 推荐使用subList处理局部列表
需求：一个列表有100个元素，要删除索引位置为20~30的元素。
实现：用subList取出一个子列表，然后清空。因为subList返回的是原始列表的一个视图，删除此视图中的所有元素，就会反映到原始字符串上。
`list.subList(20,30).clear();`

# 生成子列表后不要再操作原列表
对于子列表操作，因为视图动态生成，生成子列表后再操作原列表，必然会导致视图不稳定，最有效的方法就是通过Collections.unmodifiableList方法设置只读：
```java
List<String> list = new ArrayList<String>();
List<String> subList = list.subList(0,2);
list = Collections.unmodifiableList(list);
// read only
doReadSomething(list);
// read and write
doReadAndWriteSomething(subList)
```

# 使用Comparator进行排序
Java中给数据排序有两种实现方式：
1. 实现Comparable接口：必须实现compareTo方法
2. 实现Comparator接口：

Comparable接口可以作为实现类的默认排序法，Comparator接口则是一个类的扩展排序工具。

# 不推荐使用binarySearch对列表进行检索  
binarySearch底层实现使用了二分法查询，而二分法查询的一个首要前提是：数据集已经实现升序排列，否则其查找的值是不准确的。

binarySearch的二分法比indexOf的遍历算法性能上高很多。

# 集合中的元素compareTo和equals必须同步
indexOf通过equals方法判断，binarySearch通过compareTo方法判断。
equals是判断元素是否相等，compareTo是判断元素在排序中的位置是否相同。

# 集合运算时使用更优雅的方式
不用遍历不用遍历不用遍历！！！
1. 并集
```java
public static void main(String[] args){
    List<String> list1 = new ArrayList<String>();
    list1.add("A");
    list1.add("B");
    List<String> list2 = new ArrayList<String>();
    list2.add("C");
    list2.add("B");

    //并集
    list1.addAll(list2);
}
```
