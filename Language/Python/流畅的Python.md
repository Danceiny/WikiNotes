## 列表推导中的变量泄漏问题

```python
x = "my precious"
dummy = [x for x in 'ABC']
x # 'C'!
```



## collections

```python
import collections
Card = collections.namedtuple('Card', ['rank', 'suit'])

# cls attr
Card._fields
# cls method
Card._make()
# instance method
Card._asdict()

```

## 内置序列类型概览

- 容器序列

  - 引用，存放不同的数据类型


  - list 
  - tuple
  - collections.deque

- 扁平序列（值，连续内存空间）

  - str
  - bytes
  - bytesarray
  - Memoryview
  - array.array

- 可变序列

  - `__setitem__`,`__delitem__`,`insert`,`append`,`reverse`,`extend`,`pop`,`remove`,`__iadd__`


  - list
  - bytearray
  - array.array
  - collections.deque
  - memoryview

- 不可变序列

  - tuple
  - str
  - bytes

- ​

## pythontutor.com

## bisect 排序

bisect模块包含两个主要函数：

- bisect
  - https://docs.python.org/3/library/bisect.html
  - `import bisect`
  - `bisect.bisect == bisect.bisect_right`（别名），和`bisect_left`的区别就是插入到相等元素的左边还是右边。这对于类似`1 == 1.0`这样元素相等但实际并不相同的数据类型来说很重要。
  - 可在很长的有序序列中作为index的替代，用来更快地查找一个元素的位置
  - bisect(seq, target) 在seq中搜索target的位置，该位置满足：把target插入这个位置之后，seq还能保持升序。可以先调用该函数找到插入位置：`index = bisect.bisect(seq, target)`，然后`seq.insert(index, target)`，也可以用`insort()`一步到位，而且后者速度更快。
- insort



### 不用列表

- 存放1000万个浮点数，用array：array存储的不是float对象，而是字节表述。
- 如果需要频繁对序列做先进先出的操作，用deque（双端队列）。

```python
from array import array
from random import random
floats = array('d', (random() for i in range(10**7)))
fp = open('floats.bin', 'wb')
floats.tofile(fp)
fp.close()
floats2 = array('d')
fp = open('floats.bin', 'rb')
floats2.fromfile(fp, 10**7)
fp.close()
```



### 内存视图

https://stackoverflow.com/questions/4845418/when-should-a-memoryview-be-used/

- python2可能出现：TypeError: cannot make memory view because object does not have the buffer interface  用Py3！

```python
import array
numbers = array.array('h', [-2, -1, 0, 1, 2])   # 'h', 2 bytes
memv = memoryview(numbers)
l = len(memv)
print(l)    # 5

memv_oct = memv.cast('B')   # 'B', 1 byte
print(len(memv_oct))
memv_oct.tolist()

memv_oct[5] = 4 # [5] => 0 => 0x00_00 => 0x04_00 ([7] => 1 => 0x00_01)

print(numbers)  # got [-2, -1, 1024, 1, 2], 1024 <= 0x04_00
```



### 检查对象x能够迭代

- **从Python3.4开始，最准确的方法是调用 `iter(a)`函数**，不可迭代则会抛出`TypeError`异常。这个方法比`isintance(x, abc.Iterable)`更准确，因为`iter(x)`会考虑遗留的`__getitem__`方法。
- abc.Iterator类：https://hg.python.org/cpython/file/3.4/Lib/_collections_abc.py#l93

### 可迭代的集合 与 迭代器对象 之间的关系

标准的迭代器模式：

```python
import re
import reprlib	# py3.5+

RE_WORD = re.compile("\w+")


class SentenceIterator(object):
    # 实现可迭代协议
    def __init__(self, words):
        self.words = words
        self.index = 0
    def __next__(self):
        try:
            word = self.words[self.index]
        except IndexError:
            raise StopIteration()
        self.index += 1
        return word
    def __iter__(self):
        return self



class Sentence:
    def __init__(self, text):
        self.text = text
        self.words = RE_WORD.findall(text)

    def __repr__(self):
        return 'Sentence(%s)' % reprlib.repr(self.text)

    def __iter__(self):
        return SentenceIterator(self.words)
```

迭代器可以迭代，但是可迭代的对象不是迭代器。

可迭代的对象一定不能是自身的迭代器。即，可迭代的对象必须实现`__iter__`方法，但不能实现`__next__`方法。



### 标准库中的生成器函数

- 过滤
  - itertools
    - takewhile(predicate, it)
      - predicate返回真值时产出对应的元素，然后立即停止不再检查。
    - compress(it, selector_it)
      - 并行处理两个可迭代的对象。如果selector_it中的元素是真值，产出it中对应的元素
    - dropwhile(predicate, it)
      - 处理it，跳过predicate的计算结果为真值的元素，然后产出剩下的各个元素
    - filterfalse(predicate, it)
      - 和filter()作用类似逻辑相反。
    - islice(it, stop) 或 islice(it, start, stop, step=1)
      - 产出it的切片，it可以是任何可迭代的对象，惰性操作。
  - built-in
    - filter(predicate, it)
      - 把it的各个元素传给predicate，返回真值则产出对应的元素。如果predicate的None，则只产出真值元素
- 映射
  - itertools
    - accumulate(it, [func])
      - 产出累积的总和。如果提供了func，name把前两个元素传给func，再把计算结果和下一个元素传给它，以此类推，最后产出结果
    - starmap(func, it)
      - 把it中的各个元素传给func，产出结果；输入的可迭代对象应该产出可迭代的元素iit，然后以`func(*iit)`这种形式调用func。
  - Built-in
    - enumerate(iteratable, start=0)
      - 产出由两个元素组成的元组，结构如`(index, item)`，其中index从start开始计数，item从iterable中获取。
    - map(func, it1, [it2,…,itN])
      - 把it中的各个元素传给func，产出结果；如果传入N个可迭代的对象，则func必须能接受N个参数，并且要并行处理各个可迭代的对象
- 合并
  - itertools
    - chain(it1, … , itN)
    - chain.from_iterable(it)
      - 产出it生成的各个可迭代对象的元素，一个接一个，无缝连接在一起；it应该产出可迭代元素，例如可迭代的对象列表。
    - product(it1, …, itN, repeat=1)
      - 计算笛卡尔积，分别获取元素合并成由N个元素组成的元组；repeat指明重复处理多少次输入的可迭代对象。
    - zip_longest(it1, …, itN, fillvalue=None)
      - 和內建的zip差不多，但是要等到最长的可迭代对象到头才停止，空缺的值用fillvalue填充。
  - built-in
    - zip(it1, …, itN)
      - 并行从输入的各个可迭代对象中获取元素，产出由N个元素组成的元组，只要有一个可迭代对象到头了，就默默停止
- 扩展
  - itertools
    - combinations(it, out_len)
    - combinations_with_replacement(it, out_len)
    - count(start=0, step=1)
    - cycle(it)
    - permutations(it, out_len=None)
    - repeat(item, [times])
- 重排
  - itertools
    - groupby(it, key=None)
    - tee(it, n=2)
  - builtin
    - reversed(seq)

##### 可迭代的规约函数

| 模块        | 函数                           | 说明                                       |
| --------- | ---------------------------- | ---------------------------------------- |
| builtin   | all(it)                      | 所有都为真，才为真                                |
| builtin   | any(it)                      |                                          |
| builtin   | max(it, [key=,], [default=]) | 可迭代对象为空则返回default                        |
| builtin   | min(it, [key=,], [default=]) |                                          |
| functools | reduce(func, it, [initial])  | 把前两个元素传给func，然后把计算结果和第三个元素传给func，以此类推，提供了initital则将其作为第一个元素传入。 |
| builtin   | sum(it, start=0)             | 总和加上start                                |

###### iter的高阶玩法

`iter_obj = iter(seq_func, 1)` 其中1是一个“哨符”，可调用的对象返回该值时，触发StopIteration异常，而不产出哨符。一旦耗尽，这个生成的迭代器对象就消耗完了，需要重新构建。

```python
with open('data.txt') as fp:
    for line in iter(fp.readline, '\n'):
        process_line(line)
```

##### 把生成器当做协程

Python2.5`.send()`方法，实现了客户代码和生成器之间双向交换数据，而`__next__()`方法只允许客户代码从生成器中获取数据。



```python
def f():
    def do_yield(n):
        yield n
    x = 0
    while True:
        x += 1
        ### 死循环！
        do_yield(n)
        
### 不能通过简单的函数调用把职责委托给另一个生成器函数
def f():
    def do_yield(n):
        yield n
    x = 0
    while True:
        x += 1
        yield from do_yield(x)
```

#### else

- for

  - 仅当for循环运行完毕时（即未被break语句中止）才运行else块

- while

  - 仅当while循环因为条件为假值而退出时（即未被break中止）才运行

- try

  - 没有异常抛出时

  ​

- LBYL 调用函数等之前显式测试前提条件。可能线程不安全。（检查和行事的空档，有条件竞争）--

- EAFP 和LBYL相反

  ## 上下文管理器

  - ```python
    class LookingGlass:
        def __enter__(self):
            import sys
            self.original_write = sys.stdout.write
            sys.stdout.write = self.reverse_write
            return 'JSJFDJF'
        def reverse_write(self, text):
            self.original_write(text[::-1])
        def __exit__(self, exc_type, exc_value, traceback):
            import sys
            sys.stdout.write = self.original_write
            if exc_type is ZeroDivitionError:
                print("Please DO NOT divide by 0!")
                return True # must return True, else raise
    ```

  - https://docs.python.org/3/library/sqlite3.html#using-the-connection-as-a-context-manager

  - https://docs.python.org/3/library/threading.html#using-locks-conditions-and-semaphores-in-the-with-statement

  - https://docs.python.org/3/library/unittest.mock.html#patch

  ### contextlib模块

  - closing

  - suppress

    - 构建临时忽略指定异常的上下文管理器

  - @contextmanager

    - 把简单的生成器函数变成上下文管理器，就不用新建类来实现管理器协议

    - 默认压制异常

    - ```python
      import contextlib
      @contextlib.contextmanager
      def looking_glass():
          import sys
          original_write = sys.stdout.write
          def reverse_write(text):
              origial_write(text[::-1])
          sys.stdout.write = reverse_write
          yield 'JDJFLSDJ'	# 这个值会绑定到with语句中as子句后面的目标变量上，执行with块中的代码时，这个函数会在这一点暂停
          sys.stdout.write = original_write
          
      ## 这里有个严重bug，如果with块中抛出异常，looking_glass函数的yield表达式会再次抛出该异常，但是这里没有处理错误的代码，所以look_glass函数会中止，也就不会恢复stdout的写。可以把yield语句用try-catch-finally.
      ```

    - ```python
      # 原地重写文件
      import csv
      # inplace源码
      # http://www.zopatista.com/python/2013/11/26/inplace-file-rewriting/

      # python3
      with inplace(csvfilename, 'r', newline='') as (infh, outfh):
          reader = csv.reader(infh)
          writer = csv.writer(outfh)
          for row in reader:
              row += ['new', 'columns']
              writer.writerow(row)
              
      ```

    - ​

  - ContextDecorator

  - ExitStack

    - 这个上下文管理器能进入多个上下文管理器
    - with块结束时，ExitStack按照后进先出的顺序调用栈中各个上下文管理器的`__exit__`方法。
    - 场景：同时打开任意一个文件列表中的所有文件

  - ​

- yield from （Python3.3+）

  - 不只是语法糖

> 组合学·生成器

```python
list(itertools.combinations('ABC', 2))# 元素不相同，顺序不重要
list(itertools.combinations_with_replacement('ABC', 2))#元素可相同，顺序不重要
list(itertools.permutations('ABC', 2))#元素不相同，顺序重要
list(itertools.product('ABC', repeat=2))#笛卡尔积
```



#### 影响属性处理方式的特殊属性

- `__class__`
- `__dict__`
- `__slots__`
  - 一个字符串组成的元组
  - 如果该元组中没有`__dict__`，则该类的实例没有`__dict__`属性。

#### 处理属性的特殊方法

- `__delattr__(self, name)`
- `__getattr__(self, name)`
  - 仅当获取指定的属性失败，搜索过 obj, Class和超类之后调用
- `__getattribute__(self, name)`
  - 尝试获取指定的属性时总会调用这个方法