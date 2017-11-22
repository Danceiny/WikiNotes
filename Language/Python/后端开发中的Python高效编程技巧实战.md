![](http://opkk27k9n.bkt.clouddn.com/17-8-4/85562503.jpg)

# Ch2
- 如何在列表、字典、集合中根据条件筛选数据？
	1. 初级方案：迭代，条件判断
	2. `filter(lambda x : x>=0, data)`
	3. 列表解析：`[x for x in data if x>=0]
	4. 字典解析：`{k:v for k,v in d.iteritems() if v>=0}`
	5. 集合解析：`{x for x in s if x>=0}`

- 如何为元组中的每个元素命名，提高程序可读性？
	1. 将元组的index定义为常量（枚举），如：`NAME=1;student[NAME]`
	2. 使用标准库中collections.namedtuple替代内置tuple。`Student = namedtuple('Student',['name','age','sex']); s = Student('Jim',16,'male'); s.name; isinstance(s,tuple) == True`。

- 如何统计序列中元素的出现频度？
	1. 对序列data，`result=dict.fromkeys(data);`。然后计数并更新result字典
	2. 使用collections.Counter。
	3. 对英文文本, `f=open('t.txt').read(); result = Counter(re.split('\W+',f);`

- 如何根据字典中的值的大小，对字典中的项排序？
	1. 使用内置函数sorted。利用zip将字典数据转化元组：`sorted(zip(d.values(),d.keys())` (zip(d.itervalues(),d.iterkeys()) 更省空间。
	2. 使用内置函数sorted。`sorted(d.items(),key=lambda x:x[1])`。

- 如何快速找到多个字典中的公共键？
	1. 利用字典的viewkeys函数得到键值的集合。`d1.viewkeys() & d2.viewkeys() & d3.viewkeys()` 
	2. 使用map函数得到所有字典的keys集合并通过reduce求交集。`map(dict.viewkeys,[d1,d2,d3]); reduce(lambda a,b: a&b, map(dict.viewkeys,[d1,d2,d3]))`

- 如何让字典保持有序？
	1. 使用collections.OrderedDict。会维护加入字典的键的次序（以遍历）。

- 如何实现用户的历史记录功能（最多n条）？
	1. 使用collections.deque，双端队列，程序退出前使用pickle将对象存入文件。`dq = deque([],n); dq.append(user_history);`

# ch3
- 如何实现可迭代对象和迭代器对象？
	1. iter()函数首先找`__iter__`接口，没有的话（比如str）就找`__getitem__`。![](http://opkk27k9n.bkt.clouddn.com/17-8-4/72216919.jpg)
	```python
	# coding=utf-8
	from collections import Iterator,Iterable
	import requests
	class WeatherIterator(Iterator):
		def __init__(self,cities):
			self.cities = cities
			self.index = 0
		def getWeather(self,city):
			r = requests.get(u'http://wthrcdn.etouch.cn/weather_mini?city=' + city)
			data = r.json()['data']['forecast'][0]
			return '%s: %s, %s ' % (city,data['low'],data['high'])
		def next(self):
			if self.index == len(self.cities):
				raise StopIteration
			city = self.cities[self.index]
			self.index += 1
			return self.getWeather(city)

	class WeatherIterable(Iterable):
		def __init__(self, cities):
			self.cities = cities
		def __iter__(self):
			return WeatherIterator(self.cities)
	for x in WeatherIterable([u'北京',u'上海',u'广州']):
		print x
	```
	2. 使用生成器函数实现可迭代对象。将该类的`__iter__`方法实现成生成器函数，每次yield返回一个素数。
	```python
	class PrimeNumbers:
		def __init__(self,start,end):
			self.start = start
			self.end = end
		def isPrimeNum(self,k):
			if k<2:
				return False
			for i in xrange(2,k):
				if k%i == 0:
					return False
			return True
		def __iter__(self):
			for k in xrange(self.start,self.end+1):
				if self.isPrimeNum(k):
					yield k
	# test case
	for x in PrimeNumbers(1,100):
		print x
	```
- 如何进行反向迭代以及如何实现反向迭代？
	1. `reversed(data)`。
	```python
	class FloatRange:
		def __init__(self,start,end,step=0.1):
			self.start = start
			self.end = end
			self.step = step
		def __iter__(self):
			t = self.start
			while t <= self.end:
				yield t
				t += self.step
		def __reversed__(self):
			t = self.end
			while t>=self.start:
				yield t
				t -= self.step
	# test case
	for x in FloatRange(1.0,4.0,0.5):
		print x
	for x in reversed(FloatRange(1.0,4.0,0.5)):
		print x
	```
- 如何对迭代器做切片操作？
	1. 对文本对象做切片可以么？`f.readlines()`方法可以，但是一次性读入对内存有压力。可以使用itertools.islice。`islice(f,100,300); #从100行到300行; islice(f,500); #从0行到500行； islice(f,100,None);# 从100行到最后一行;`。使用islice时会“消耗”原有的迭代器对象。

- 如何在一个for语句中迭代多个可迭代对象？
	1. 并行的zip方案。将多个可迭代对象合并，每次迭代返回一个元组。
	2. 串行的itertools.chain方案。将多个可迭代对象连接。


# ch4 字符串
- 如何拆分含有多种分隔符的字符串？
	1. 连续使用str.split()方法
	```python
	def mySplit(s,ds):
		res = [s]
		for d in ds:
			t = []
			map(lambda x: t.extend(x.split(d)),res)
			res = t
		return [x for x in res if x] #过滤空字符串
	s = 'ab;cd|efg|hi,,jkl|mn\topq;rst,uvw\txyz'
	print mySplit(s,';,|\t')
	```
	2. 使用正则表达式re。`re.split('[,;\t|]+',s)`。
- 如何判断字符串a是否以字符串b开头或结尾？
	1. `[name for name in os.listdir('.') if name.endswith(('.sh','.py'))]; `。 `os.stat('e.py').st_mode | stat.S_IXUSR`为该文件权限。修改：`os.chmod('e.py',os.stat('e.py').st_mode | os.stat.S_IXUSR)`。

- 如何调整字符串中文本的格式？
	比如log文件的日期格式。解决方案：使用正则re.sub()方法做字符串替换，利用正则表达式的捕获组捕获每个部分内容，在替换字符串中调整各个捕获组的顺序。
	`re.sub('(\d{4})-(\d{2})-(\d{2})',r'\2/\2/\1',log)`。起名：``re.sub('(?P<year>\d{4})-(?P<month>\d{2})-(?P<day>\d{2})',r'\g<month>/\g<day>/\g<year>',log)`.

- 字符串对齐
	Docstring: `s.ljust(width[,fillchar]) -> string`
	右对齐：s.rjust; 居中对齐：s.center。
	还可以用format：`format(s,'<20'); format(s,'>20'); format(s,'^20').

- 删除不需要的字符
	1. str.translate. `S.translate(table [,deletechars]) -> string`。table是映射表。`import string; table=string.maketrans('abc','xyz');`
	Return a copy of the string S, where all characters occurring in the optional argument deletechars are removed, and the remaining characters have been mapped through the given translation table, which must be a string of length 256 or None. If the table argument is None, no translation is applied and the operation simply removes the characters in deletechars.
	删除：`s.translate(None, '\t\r\n')`

# ch5 文件
- 在Py2和Py3中分别如何读取文本文件？
	字符串语义发生变化！ 

|Python2|Python3|six|
|---|---|---|
|str|bytes|six.binary_type|
|unicode|str|six.text_type|
|写入文件前对unicode编码，读入文件后对二进制字符串解码|open函数指定文本模式，encoding指定编码格式|


- 如何处理二进制文件？
	使用struct.unpack解析二进制数据。
	![](http://opkk27k9n.bkt.clouddn.com/17-8-5/37516391.jpg)
	![](http://opkk27k9n.bkt.clouddn.com/17-8-5/47524157.jpg)
	```python
	f = open('demo.wav','rb')
	info = f.read(44)
	import struct
	struct.unpack('<h','\x01\x02')# 把两个字节的'\x01\x02'解析成数据类型'h'，即对应short类型。<h是小段字节序，默认；>h是大段字节序。
	f.seek(0,2)
	n = (f.tell()-44)/2
	import array
	buf = array.array('h',(0 for _ in xrange(n)))
	f.seek(44)
	f.readinto(buf)
	
	for i in xrange(n):buf[i] /= 8
	
	fw = open('demo2.wav','wb')
	buf.tofile(f2)#没有写头部，是不能播放的
	fw.close()
	```
- 如何设置文件的缓冲？
	1. 全缓冲。open()的buffering参数设为大于1。
	2. 行缓冲。buffering参数设为1。写入'\n'就刷新缓冲区写入磁盘。
	3. 无缓冲。buffering=0.

- 如何将文件映射到内存？
	1. 访问某些二进制文件时，希望把文件映射到内存中实现随机访问。
	2. 嵌入式设备寄存器被编址到内存地址空间，可以映射/dev/mem某范围去访问寄存器。
	3. 多个进程映射同一文件可以实现进程通信。
	解决：使用mmap.mmap。
	```python
	f = open('.bin','r+b')
	fileno = f.fileno() #文件描述符
	m = mmap.mmap(fileno(),0,acess=mmap.ACCESS_WRITE)
	```
- 如何访问文件的状态？
	- os.stat(path) 跟随符号链接
	- os.lstat() 不跟随符号链接
	- os.fstat(fd) like stat()
	解析stat。`import stat`.判断是否是文件夹：`if stat.S_ISDIR(s.st_mode)`
	访问权限 `s.st_mode & stat.S_IXUSR`返回0则没有user的执行权限。
- 如何使用临时文件？
	临时文件：不用命名，关闭后自动删除。
	使用标准库中tempfile下的TemporaryFile,NamedTemporaryFile。
- 如何派生内置不可变类型并修改实例化行为？
	实现`__new__`，修改实例化行为。
	```python
	class IntTuple(tuple):
		def __new__(cls,iterable):
			g = (x for x in iterable if isinstance(x,int) and x>0)
			return super(IntTuple,cls).__new__(cls,g)
		def __init__(self,iterable):
			super(IntTuple,self).__init__(iterable)
	```
- 如何为创建大量实例节省内存？
	```python
	class Player:
		__slots__ = ['uid','name','stat','level']
		def __init__(self,...):
			self.uid = uid
			self.name = name
			self.stat = stat
			self.level = level
	```
	每个实例都少了`__dict__`和`__weakref__`的属性。放弃属性的动态绑定。

- 如何让对象支持上下文管理？
	实现上下文管理协议，需定义实例的`__enter__`,`__exit__`方法，分别在with开始和结束时被调用。`with A() as B`,A()调用`__enter__()`后返回值才是B。

- 如何创建可管理的对象属性？
	使用property。

- 如何让类支持比较操作？
	比较符号运算符重载需要实现`__lt__,__le__,__gt__`方法，使用functools的类装饰器total_ordering可以简化。

- 如何使用描述符对实例属性做类型检查？
	使用描述符来实现需要类型检查的属性：分别实现`__get__,__set__,__delete__`方法，在`__set__`内使用instance函数做类型检查。

- 如何在环状数据结构中管理内存？（循环引用导致的垃圾回收黑洞问题）
	解决：使用库weakref，可以创建一种能访问对象但不增加引用计数的对象。`a = A();a_wref = weakref.ref(a); a2 = a_wref(); a is a2 -> True`.
- 如何通过实例方法名字的字符串调用方法？
	1. 方法一：使用内置函数getattr，通过名字在实例上获取方法对象，然后调用；
	2. 方法二：使用标准库operator下的methodcaller函数调用。`s = 'abc123abc456'; s.find('abc',4) -> 6; methodcaller('find','abc',4)(s) -> 6;'
- 如何使用多线程？
	线程安全的queue。

- 如何在线程间进行事件通知？
	Threading.Event。两个线程同时持有该event对象。
	1. 等待事件一端调用wait，等待事件。
	2. 通知事件一端调用set，通知事件。

- 如何使用线程本地数据？
	threading.local函数可以创建线程本地数据空间，其下属性对每个线程独立存在。
- 如何使用线程池？
 对于Python3，使用concurrent.futures下的ThreadPoolExecutor。 
 	```python
 	executor = ThreadPoolExecutor(3)
 	def f(a,b):return a**b
 	submit = executor.submit(f,2,3)
 	submit.result() -> 8
 	# 传参：2&4， 3&5， 5&6, 三个线程。
 	executor.map(f,[2,3,5],[4,5,6])
- 如何使用多进程？
	GIL锁是线程锁，锁不住进程。
- 如何使用函数装饰器？
	例如fibonacci函数的缓存功能。
	```python
	def fibonacci(n,cache=None):
		if cache is None:
			cache = {}
		if n in cache:
			return cache[n]
		if n <= 1:
			return 1
		cache[n] = fibnacci(n-1,cache)+fibonacci(n-2,cache)
		return cache[n]

	def memo(func):
		cache = {}
		def wrap(*args):
			if args not in cache:
				cache[args] = func(*args)
			return cache[args]
		return wrap
	@memo
	def fibonacci(n):
		if n<= 1:
			return 1
		return fibonacci(n-1) + fibonacci(n-2)

	```

- 如何为被装饰的函数保存元数据？
	使用装饰器装饰后函数元数据改变了（变成了wrapper的）。
	`funcname.__closure__[0].cell_contents` 访问闭包元数据。
	使用标准库functools.wraps,update_wrapper, 可以将原函数的元数据绑定到wrapper装饰器上。
	`update_wrapper(wrapper,func,('__name__','__doc__'),('__dict__',));#合并元数据 return wrapper; `
	这两个元组可以使用默认参数 `WRAPPER_ASSIGNMENTS, WRAPPER_UPDATES`.
- 如何定义带参数的装饰器？
	```python
	from inspect import signature
	def typeassert(*ty_args, **ty_kwargs):
		def decorator(func):
			# func -> a,b
			# d = {'a':int, 'b':str}
			sig = signature(func)
			btypes = sig.bind_partial(*ty_args,**ty_kargs).arguments
			def wrapper(*args,**kwargs):
				for name,obj in sig.bind(*args,**kwargs).arguments.items():
					if name in btypes:
						if not isinstance(obj,btypes[name]):
							raise TypeError('"%s" must be "%s"' %(name,btypes[name]))
				return func(*args,**kwargs)
			return wrapper	
		return decorator

	@typeassert(int,str,list)
	def f(a,b,c):
		print(a,b,c)
	```
- 如何实现属性可修改的函数装饰器？
	
# 注意事项

1. range & xrange
py2 -> xrange
py3 -> range

2. timeit 测试性能
`timeit filter(lambda x: x>=0, data)`