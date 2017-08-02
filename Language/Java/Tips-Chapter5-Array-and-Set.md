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
2. 交集
`list1.retainAll(list2);`
retainAll方法会删除list1中没有出现在list2中的元素。

3. 差集
`list1.removeAll(list2);`

4. 无重复的并集
确保并集结果中只有一份交集（真正的并集）
```java
list2.removeAll(list1);
list1.addAll(list2);
```

错误方案：求出两个集合的并集，然后转为HashSet剔除重复元素
错误原因：会把集合A中原本就重复的元素也剔除掉


# 使用shuffle打乱列表
```java
List<String> tagClouds = new ArrayList<String>(tagCloudNum);

// version 1
for...
Collections.swap(tagClouds,i,randomPosition);
// version 2
Collections.shuffle(tagClouds);
```


# 减少HashMap中元素的数量

使用缓冲池操作数据时，大批量增删改查操作可能会让内存溢出。下面的模拟程序重现问题：
```java
public static void main(String[] args) {

    final Runtime rt = Runtime.getRuntime();
    //JVM终止前记录内存信息
    rt.addShutdownHook(new Thread(){
        @Override
        public void run(){
            StringBuffer sb = new StringBuffer();
            long heapMaxSize = rt.maxMemory()>>20;
            sb.append("最大可用内存："+heapMaxSize+"M\n");
            long total = rt.totalMemory()>>20;
            sb.append("堆内存大小："+total+"M\n");
            long free = rt.freeMemory()>>20;
            sb.append("空闲内存："+free+"M");
            System.out.println(sb);
        }
    });

    Map<String,String>map = new HashMap<String,String>();
    //放入近40万键值对
    for(int i=0;i<393217;i++){
        map.put("key"+i,"value"+i);
    }//内存溢出（事实上我再加3个0，在windows/cmd下要运行特别久才会报错

    List<String>list = new ArrayList<String>();
    for(int i-0;i<400000;i++){
        list.add("key"+i);
        list.add("value"+i);
    }//不会溢出
}
```
HashMap使用了Entry封装，比ArrayList多一层。

0.75倍数组长度时就要翻倍，此时虽有剩余内存，但是已不够翻倍。

# 集合中的哈希码不要重复
看看HashMap是如何查找Key值的：
```java
public static void main(String[] args) {
    int size = 10000;
    List<String>list = new ArrayList<String>(size);
    //init
    for(int i=0;i<size;i++){
        list.add("value"+i);
    }
    //record start time, ns as unit.
    long start = System.nanoTime();
    //start search
    list.contains("value"+(size-1));
    //record end time, ns as unit.
    long end = System.nanoTime();
    System.out.println("list执行时间："+(end-start)+"ns");
    //Map的查找时间
    Map<String,String>map = new HashMap<String,String>(size);
    for(int i=0;i<size;i++){
        map.put("key"+i,"value"+i);
    }
    start = System.nanoTime();
    map.containsKey("key"+(size-1));
    end = System.nanoTime();
    System.out.println("map执行时间："+(end-start)+"ns");
}
```
HashMap比ArrayList快很多倍。

```java
public boolean containsKey(Object key){
    return getEntry(key)!=null;
}

final Entry<K,V>getEntry(Object key){
    int hash = (key==null) ? 0 : hash(key.hashCode());
    for(Entry<K,V>e=table[indexFor(hash,table.length)]; e!=null; e=e.next){
        Object k;
        if (e.hash==hash && ((k=e.key)==key || (key!=null && key.equals(k) ))) {
            return e;
        }
    }
    return null;
}
```
table数组：
- 长度永远是2^N。
- 元素是Entry类型。
- 元素位置不连续。

HashMap如何插入元素？
```java
public V put(K key, V value){
    if(key==null)return putForNullKey(value);

    int hash = hash(key.hashCode());
    int i = indexFor(hash,table.length);
    for(Entry<K,V>e=table[i];e!=null;e=e.next){
        Object k;
        if (e.hash==hash && ((k=e.key)==key || (key!=null && key.equals(k) ))) {
            V oldValue = e.value;
            e.value = value;
            e.recordAccess(this);
            return oldValue;
        }
    }
    modCount++;
    addEntry(hash,key,value,i);
    return null;
}
```

如果新加入的键值对的hashCode唯一，则直接添加；若不唯一，替换冲突的，但是将所有哈希码相同的键值对组建成单向链表。


# 多线程使用Vector或HashTable
Vector是ArrayList的多线程版本，HashTable是HashMap的多线程版本。

当一个集合在被多个线程修改并访问时，就可能会出现ConcurrentModificatinException异常。

线程安全和线程同步是两码事。

# 非稳定排序使用List
SortedSet接口（TreeSet实现了该接口）只是定义了在给集合加入元素时对其进行排序，之后的修改概不负责。
因此TreeSet适用于不变量的集合数据排序。

Solution 1:
 `set = new TreeSet<Person>(new ArrayList<Person>(set));`
使用`TreeSet(SortedSet<E>s)`这个构造函数，由于是原set的浅拷贝，若里面有相同元素，则不会重新排序。

Solution 2:
换用List。


# 集合大家族
Java中的集合类实在是太丰富了，有常用的ArrayList、HashMap，也有不常用的Stack、Queue，有线程安全的Vector、HashTable，也有线程不安全的LinkedList、TreeMap，有阻塞式的ArrayBlockingQueue，也有非阻塞式的PriorityQueue等，整个集合家族非常庞大，而且也是错综复杂，可以划分为以下几类：

（1）List

实现List接口的集合主要有：ArrayList、LinkedList、Vector、Stack，其中ArrayList是一个动态数组，LinkedList是一个双向链表，Vector是一个线程安全的动态数组，Stack是一个对象栈，遵循先进后出的原则。
（2）Set

Set是不包含重复元素的集合，其主要的实现类有：EnumSet、HashSet、TreeSet，其中EnumSet是枚举类型的专用Set，所有元素都是枚举类型；HashSet是以哈希码决定其元素位置的Set，其原理与HashMap相似，它提供快速的插入和查找方法；TreeSet是一个自动排序的Set，它实现了SortedSet接口。

（3）Map

Map是一个大家族，它可以分为排序Map和非排序Map，排序Map主要是TreeMap类，它根据Key值进行自动排序；非排序Map主要包括：HashMap、HashTable、Properties、EnumMap等，其中Properties是HashTable的子类，它的主要用途是从Property文件中加载数据，并提供方便的读写操作；EnumMap则是要求其Key必须是某一个枚举类型。
Map中还有一个WeakHashMap类需要说明，它是一个采用弱键方式实现的Map类，它的特点是：WeakHashMap对象的存在并不会阻止垃圾回收器对键值对的回收，也就是说使用WeakHashMap装载数据不用担心内存溢出的问题，GC会自动删除不用的键值对，这是好事。但也存在一个严重问题：GC是静悄悄回收的（何时回收？God knows！），我们的程序无法知晓该动作，存在着重大的隐患。

（4）Queue

队列，它分为两类，一类是阻塞式队列，队列满了以后再插入元素则会抛出异常，主要包括：ArrayBlockingQueue、PriorityBlockingQueue、LinkedBlockingQueue，其中ArrayBlockingQueue是一个以数组方式实现的有界阻塞队列，PriorityBlockingQueue是依照优先级组建的队列，LinkedBlockingQueue是通过链表实现的阻塞队列；另一类是非阻塞队列，无边界的，只要内存允许，都可以持续追加元素，我们最经常使用的是PriorityQueue类。

还有一种队列，是双端队列，支持在头、尾两端插入和移除元素，它的主要实现类是：ArrayDeque、LinkedBlockingDeque、LinkedList。

（5）数组

数组与集合的最大区别就是数组能够容纳基本类型，而集合就不行，更重要的一点就是所有的集合底层存储的都是数组。

（6）工具类

数组的工具类是java.util.Arrays和java.lang.reflect.Array，集合的工具类是java.util.Collections，有了这两个工具类，操作数组和集合会易如反掌，得心应手。

（7）扩展类

集合类当然可以自行扩展了，想写一个自己的List？没问题，但最好的办法还是“拿来主义”，可以使用Apache的commons-collections扩展包，也可以使用Google的google-collections扩展包，这些足以应对我们的开发需要。

注意　commons-collections、google-collections是JDK之外的优秀数据集合工具包，使用拿来主义即可。
