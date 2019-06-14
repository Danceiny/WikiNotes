# Cream of Python
**语言**
- [高质量代码的91个建议](./高质量代码的91个建议.md)
- [后端开发中的Python高效编程技巧实战](./后端开发中的Python高效编程技巧实战.md)

**库**
- [Flask]()
- [Django](./Django.md)
- [Numpy](./numpy.md)
- [urllib](./urllib.md)
- [requests](./requests.md)

## Pythoo风格规范
https://zh-google-styleguide.readthedocs.io/en/latest/google-python-styleguide/python_style_rules/

## 不得不知
Python2的字典实现

```
arguments: string object
return: hash
function: string_hash:
	if hash cached:
		return it
	set len to string's length
	initialize var p pointing to 1st char of string object
	set x to value pointed by p left shift 7 bits
	while len >= 0:
		set var x to (1000003 * x) xor value pointed by p
		increment pointer p
	set x to x xor length of string object
	cached x as the hash so we don't need to calculate it again
	return x as the hash
	
```

如果用长度为x的数组存储键值对，则用x-1作为掩码计算数组中slot的索引（`hash('a') & (x-1)`）。这样索引计算非常快（O(l)），但是容易冲突（键不够连续情况下）。可以用链表来存储，这样时间复杂度将会上升。



开放地址法：添加键值对要更久，但查找仍然O(1)。