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
# 注意事项

1. range & xrange
py2 -> xrange
py3 -> range

2. timeit 测试性能
`timeit filter(lambda x: x>=0, data)`