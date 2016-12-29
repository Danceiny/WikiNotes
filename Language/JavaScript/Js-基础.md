---
date: 2016-06-03 00:18
status: public
title: Js-基础
---

JavaScript会忽略程序中标识（token）[2]之间的空格。多数情况下，JavaScript同样会忽略换行符（2.5节提到了一种意外情形）。由于可以在代码中随意使用空格和换行，因此可以采用整齐、一致的缩进来形成统一的编码风格，从而提高代码的可读性。

除了可以识别普通的空格符（\u0020），JavaScript还可以识别如下这些表示空格的字符：水平制表符（\u0009）、垂直制表符（\u000B）、换页符（\u000C）、不中断空白(\u00A0)、字节序标记(\uFEFF)，以及在Unicode中所有Zs类别的字符[3]。JavaScript将如下字符识别为行结束符：换行符（\u000A），回车符（\u000D），行分隔符（\u2028），段分隔符（\u2029）。回车符加换行符在一起被解析为一个单行结束符。
Unicode格式控制字符（Cf类[4]），比如“从右至左书写标记”（\u200F）和“从左至右书写标记”（\u200E）[5]，控制着文本的视觉显示，这对于一些非英语文本的正确显示来说是至关重要的，这些字符可以用在JavaScirpt的注释、字符串直接量和正则表达式直接量中，但不能用在标识符（比如，变量名）中。但有个例外，零宽连接符（\u200D）和零宽非连接符（\uFEFF）[6]是可以出现在标识符中的，但不能作为标识符的首字符。上文也提到了，字节序标记格式控制符（\uFEFF）被当成了空格来对待。

Unicode允许使用多种方法对同一个字符进行编码。比如，字符“é”可以使用Unicode字符\u00E9表示，也可以使用普通的ASCII字符e跟随一个语调符\u0301。在文本编辑器中，这两种编码的显示结果一模一样，但它们的二进制编码表示是不一样的，在计算机里也不相等。Unicode标准为所有字符定义了一个首选的编码格式，并给出了一个标准化的处理方式将文本转换为一种适合比较的标准格式，JavaScript会认为它正在解析的程序代码已经是这种标准格式，不会再对其标识符、字符串或正则表达式作标准化处理。

# 保留字

![](~/00-23-33.jpg)
全局变量和函数：

![](~/00-24-41.jpg)
# 可选分号

![](~/00-25-26.jpg)
JavaScript并不是在所有换行处都填补分号：只有在缺少了分号就无法正确解析代码的时候，JavaScript才会填补分号。

![](~/00-26-14.jpg)
# 数字
JavaScript不区分整数值和浮点数值。JavaScript中的所有数字均用浮点数值表示。JavaScript采用IEEE 754标准[2]定义的64位浮点格式表示数字，这意味着它能表示的最大值是±1.7976931348623157×10308，最小值是±5×10-324。

JS不一定支持八进制直接量。

当数字运算结果超过了JavaScript所能表示的数字上限（溢出），结果为一个特殊的无穷大（infinity）值，在JavaScript中以Infinity表示。同样地，当负数的值超过了JavaScript所能表示的负数范围，结果为负无穷大，在JavaScript中以-Infinity表示。
被零整除在JavaScript并不报错：它只是简单的返回无穷大（Infinity）或负无穷大（-Infinity）。但有一个例外，零除以零是没有意义的，这种整除运算结果也是一个非数字（not-a-number）值，用NaN表示。无穷大除以无穷大、给任意负数作开方运算或者算术运算符与不是数字或无法转换为数字的操作数一起使用时都将返回NaN。

JavaScript中的非数字值有一点特殊：它和任何值都不相等，包括自身。也就是说，没办法通过x==NaN来判断变量x是否是NaN。相反，应当使用x!=x来判断，当且仅当x为NaN的时候，表达式的结果才为true。函数isNaN()的作用与此类似，如果参数是NaN或者是一个非数字值（比如字符串和对象），则返回true。JavaScript中有一个类似的函数isFinite()，在参数不是NaN、Infinity或-Infinity的时候返回true。

负零值同样有些特殊，它和正零值是相等的（甚至使用JavaScript的严格相等测试来判断）。这意味着这两个值几乎一模一样，除了作为除数之外。

JavaScript采用了IEEE-754浮点数表示法（几乎所有现代编程语言所采用），这是一种二进制表示法，可以精确地表示分数，比如1/2、1/8和1/1024。遗憾的是，我们常用的分数（特别是在金融计算方面）都是十进制分数1/10、1/100等。二进制浮点数表示法并不能精确表示类似0.1这样简单的数字。
# 时间
```javascript:n
var then=new Date(2011,0,1);//2011年1月1日
var later=new Date(2011,0,1,17,10,30);//同一天,当地时间5:10:30pm,
var now=new Date();//当前日期和时间
var elapsed=now-then;//日期减法：计算时间间隔的毫秒数
later.getFullYear()//=＞2011
later.getMonth()//=＞0:从0开始计数的月份
later.getDate()//=＞1:从1开始计数的天数
later.getDay()//=＞5:得到星期几，0代表星期日，5代表星期一
later.getHours()//=＞当地时间17:5pm
later.getUTCHours()//使用UTC表示小时的时间，基于时区
```

# 字符串
```javascript:n
var s="hello,world"//定义一个字符串
s.charAt(0)//=＞"h":第一个字符
s.charAt(s.length-1)//=＞"d":最后一个字符
s.substring(1,4)//=＞"ell":第2～4个字符
s.slice(1,4)//=＞"ell":同上
s.slice(-3)//=＞"rld":最后三个字符
s.indexOf("l")//=＞2:字符l首次出现的位置
s.lastIndexOf("l")//=＞10:字符l最后一次出现的位置
s.indexOf("l",3)//=＞3:在位置3及之后首次出现字符l的位置
s.split(",")//=＞["hello","world"]分割成子串
s.replace("h","H")//=＞"Hello,world":全文字符替换
s.toUpperCase()//=＞"HELLO,WORLD"
```

在JavaScript中字符串是固定不变的，类似replace()和toUpperCase()的方法都返回新字符串，原字符串本身并没有发生改

在代码的最顶级——不在任何函数内的JavaScript代码——可以使用JavaScript关键字this来引用全局对象：

# 类型转换

![](~/17-24-55.jpg)
```
x+""//等价于String(x)
+x//等价于Number(x).也可以写成x-0
!!x//等价于Boolean(x).注意是双叹号
```
Number类定义的toString()方法可以接收表示转换基数（radix）[6]的可选参数，如果不指定此参数，转换规则将是基于十进制。同样，亦可以将数字转换为其他进制数（范围在2～36之间），例如：
```
var n=17;
binary_string=n.toString(2);//转换为"10001"
octal_string="0"+n.toString(8);//转换为"021"
hex_string="0x"+n.toString(16);//转换为"0x11"

var n=123456.789;
n.toFixed(0);//"123457"
n.toFixed(2);//"123456.79"
n.toFixed(5);//"123456.78900"
n.toExponential(1);//"1.2e+5"
n.toExponential(3);//"1.235e+5"
n.toPrecision(4);//"1.235e+5"
n.toPrecision(7);//"123456.8"
n.toPrecision(10);//"123456.7890"


parseInt("3 blind mice")//=＞3
parseFloat("3.14 meters")//=＞3.14
parseInt("-12.34")//=＞-12
parseInt("0xFF")//=＞255
parseInt("0xff")//=＞255
parseInt("-0XFF")//=＞-255
parseFloat(".1")//=＞0.1
parseInt("0.1")//=＞0
parseInt(".1")//=＞NaN:整数不能以"."开始
parseFloat("$72.47");//=＞NaN:数字不能以"$"开始
parseInt("11",2);//=＞3(1*2+1)
parseInt("ff",16);//=＞255(15*16+15)
parseInt("zz",36);//=＞1295(35*36+35)
parseInt("077",8);//=＞63(7*8+7)
parseInt("077",10);//=＞77(7*10+7)
```

**空数组会被转换为数字0**
对象转换为数字的细节解释了为什么空数组会被转换为数字0以及为什么具有单个元素的数组同样会转换成一个数字。数组继承了默认的valueOf()方法，这个方法返回一个对象而不是一个原始值，因此，数组到数字的转换则调用toString()方法。空数组转换成为空字符串，空字符串转换成为数字0。含有一个元素的数组转换为字符串的结果和这个元素转换字符串的结果一样。如果数组只包含一个数字元素，这个数字转换为字符串，再转换回数字。

# 属性访问
显然.identifier的写法更加简单，但需要注意的是，这种方式只适用于要访问的属性名称是合法的标识符，并且需要知道要访问的属性的名字。如果属性名称是一个保留字或者包含空格和标点符号，或是一个数字（对于数组来说），则必须使用方括号的写法。当属性名是通过运算得出的值而不是固定的值的时候，这时必须使用方括号写法（具体示例参照6.2.1节）。

## +运算符
```javascript:n
1+2+"blind mice";//=＞"3 blind mice"
1+(2+"blind mice");//=＞"12 blind mice"
``` 

## 无符号右移（＞＞＞）

运算符“＞＞＞”和运算符“＞＞”一样，只是左边的高位总是填补0，与原来的操作数符号无关，例如，-1＞＞4=-1，但是-1＞＞＞4=0x0FFFFFFF。

## instanceof运算符
```
var d=new Date();//通过Date()构造函数来创建一个新对象
d instanceof Date;//计算结果为true，d是由Date()创建的
d instanceof Object;//计算结果为true，所有的对象都是Object的实例
d instanceof Number;//计算结果为false，d不是一个Number对象
var a=[1,2,3];//通过数组直接量的写法创建一个数组
a instanceof Array;//计算结果为true，a是一个数组
a instanceof Object;//计算结果为true，所有的数组都是对象
a instanceof RegExp;//计算结果为false，数组不是正则表达式

```

## &&运算符
```
if(a==b)stop();//只有在a==b的时候才调用stop()
(a==b)＆＆stop();//同上
```

## ||逻辑或
```
//如果max_width已经定义了，直接使用它；否则在preferences对象中查找max_width
//如果没有定义它，则使用一个写死的常量
var max=max_width||preferences.max_width||500;
```

## eval()
于eval()最重要的是，它使用了调用它的变量作用域环境。也就是说，它查找变量的值和定义新变量和函数的操作和局部作用域中的代码完全一样。如果一个函数定义了一个局部变量x，然后调用eval("x")，它会返回局部变量的值。如果它调用eval("x=1")，它会改变局部变量的值。如果函数调用了eval("var y=3;")，它声明一个新的局部变量y。同样地，一个函数可以通过如下代码声明一个局部函数：

`eval("function f(){return x+1;}");`

## void运算符
＜a href="javascript:void window.open();"＞打开一个新窗口＜/a＞