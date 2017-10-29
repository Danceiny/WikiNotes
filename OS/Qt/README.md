---
date: 2016-02-29 18:10
status: public
title: PyQt4编程简介
---

# 关于本文
source:<PyQt4编程简介.pdf  作者：柴树杉>

该文档根据“Introduction to PyQt4”翻译。

# 开始
创建一个PyQt4一般需要：
1. 使用QtDesigner创建GUI界面；
2. 在属性编辑器中修改部件的名字；
3. 使用pyuic4工具生成一个python类；
4. 通过GUI对应类来运行程序；
5. 通过设置自己的slots来扩展功能；
6. 使用窗口部件的时候，可以从"PyQt's Classes"查询。

# 退出按钮
1. 打开QtDesigner，
2. 选择Widget类型，
3. 添加一个PushButton按钮，
4. 通过鼠标右键修改pushButton显示的内容，
5. 窗口建好之后，通过QtDesigner编辑一些Qt预定义的信号/槽。例如使用"close()"槽函数来关闭程序。首先切换到信号/槽边界模式，
6. 用鼠标移到pushButton区域，然后拖动，
7. 弹出一个信号/槽选择框，
8. 信号选择clicked()，槽选择close()。
9. 将窗口保存为test.ui文件。
10. 切换到test.ui所在的目录，输入命令：`pyuic4 test.ui > test_ui.py`
11. 下一步是创建一个test.py文件：
```python:n
import sys
from PyQt4 import QtCore, QtGui
from test_ui import Ui_Form

class MyForm(QtGui, QMainWindow):
	def __init__(self, parent=None):
		QtGui.QWidget.__init__(self, parent)
		self.ui = Ui_Form()	# Ui_Form是用pyuic4工具从Form窗口生成的对应python类的名字。
		self.ui.setupUi(self)

if __name__ == "__main__":
	app = QtGui.QApplication(sys.argv)
	myapp = MyForm()
	myapp.show()
	sys.exit(app.exec_())
```

12. 运行test.py，出现响应的窗口，点击按钮的时候退出程序。

# 简易的文本编辑器


---
date: 2016-02-29 18:10
status: public
title: Errors
---

我前几天准备学习Qt，今天写第一个Hello World，结果也是同样的can not open，然后百度一番都是说路径不能有中文，无果。
其实用到：C:\Users\***\AppData\Local\Temp这个目录，很明显是系统的临时文件目录。我就想到肯定在创建Hello World项目时哪里的设置有问题，果不其然，去工具->选项->构建和运行，不要使用jom代替nmake。OK，成功运行。


---
date: 2016-04-29 08:18
status: public
title: 2016-04-29-Qt的碎碎念
---

# API的健壮与安全
下面是两个不同层次的人编写的程序：
```c:n
char *strcpy(char dst[], const char src[])
{
	//使用数组的方式
	int i;
	for(i=0;src[i]!='\0';i++)
	dst[i] = src[i];
	dst[i] = '\0';
	return dst;
}

char *strcpy(char *dst, const char *src)
{
	//使用指针
	char *s = dst;
	while(*dst++ = *src++);
	return s;
}
```
# Qt程序开发框架

![](~/12-56-11.jpg)

Qt把下面这些特性添加到了C++中：
1. 一种关于无缝对象通信被称为信号/槽的非常强大的机制，可以直接设计属性；
2. 强大的事件和事件过滤器；
3. 根据上下文进行国际化的字符串翻译；
4. 完善的时间间隔驱动的计时器，使得在一个事件驱动的图形界面程序中很好地集成许多任务成为可能。
5. 以一种自然的方式组织对象所有权的分层次和可查询的对象树。

当参考对象被破坏时，被守护的指针QGuardedPtr可以自动地设置为无效。而C++指针在它的对象被破坏时变成了“摇摆指针”，这就是Qt区别于C++的地方。许多Qt的特性是基于QObject的继承，通过标准C++技术实现的。

其他例如，对象通信机制和虚拟属性系统，都需要Qt自己的元对象编译器提供的元对象系统。元对象系统是一个可使语言更加适用于真正的组件图形用户界面程序的C++扩展。

# 信号与槽
## 1. 回调
回调是指一个函数的指针。

回调有两个主要缺点：
1. 不是安全的类型，用户不能确定处理函数是否使用了正确的参数调用；
2. 回调和处理函数的联系紧密，处理函数必须明确要调用哪个回调，这就需要正确的选择。

## 2. 信号与槽
信号与槽用于对象间的通信，是Qt的一个中心特征。是一种可以替代回调的技术。

一个特定事件发生的时候，一个信号就被发射。槽就是一个可以被调用来处理特定信号的函数。

Qt的窗口部件有很多预定义的信号和槽，用户可以继承自定义。

回调的缺点就是信号与槽的优点。

信号与槽的连接方式有3种：
- 多信号对单一槽；
- 单信号对多槽；
- 信号对信号（此时第一个信号被发射时，第二个信号立即发射）

一个小的Qt类如下：
```C++:n
class foo: public QObject
	{
		Q_OBJECT
	public:
		foo();
		int value() const { reutrn val; }
	public slots:
	 	void setValue(int );
	signals:
		void valueChanged(int );
	private:
		int val;
	};
```
这个类通过发射一个信号valueChanged()，来通知外界它的状态发生了变化。其对象可以发信号给这个槽。

**所有包含信号与槽的类必须在程序中声明Q_OBJECT。**

下面是槽函数的一个可能实现：
```c++:n
void foo::setValue(int v)
{
	if(v != val){	// 避免循环连接时出现无休止循环
		val = v;
		emit valueChanged(v);//从对象中发射valueChanged信号
	}
}
```

下面是把两个对象连接在一起的一种方法：
```c++:n
foo a, b;
connect(&a, SIGNAL(valueChanged(int)), &b, SLOT(setValue(int)))；
b.setValue(11);	// b也可以说是信号源，但是没有接收器（槽）来处理它发出的信号
a.setValue(79);	// a是信号源，设值，会发射一个valueChanged()信号
b.value();
```

# 对象与对象树
对象树都是通过QObject组织起来的，当一个对象创建时，会自动被添加到父类的children()队列中，之后可通过调用children()函数来获得这个对象链表。

QShortcut是一个键盘的快捷键，被作为一个窗口的子部件来使用。一个用户关闭一个窗口时，快捷键就会被自动删除。

QWidget类是指在屏幕上显示的所有部件。

一个子类就是一个子部件。

可用调试函数进行调试：`QObject::dumpObjectTree()`  and  `QObject::dumpObjectInfo()`

构造与析构对象的规则如下：
>当对象在堆中建立，一个对象树就会按照它自己的方法来构造，之后采取同样的方法来删除。在删除一个对象前要检测是否有父类，如果有，将会自动地在对象中删除；（此处原文滥用第三人称代词，导致我无法准确理解，只能猜测）如果这个对象有子类，那么析构器也会自动删除子类。

# 对象属性
在一个类中声明一个属性，需要符合两个条件：

1.
>Q_PROPERTY()必须在声明属性前添加；

2.
>类必须是由QObject继承而来。

# 事件和事件过滤器
在Qt里，一个事件是继承QEvent的对象。事件通过调用QOject::event()，被发送到继承QObject的对象。事件发送即一个事件已经产生，由QEvent表达，QObject回应。多数事件针对QWidget及其子类。

有时并不存在一个特定事件函数，或特定的某个函数不能充分地满足需求。最常用的事件如按下Tab键。正常情况下，被QWidget看成是移动键盘焦点，但少数窗口部件需要自行解释。

```c++:n
bool MyClass:event(QEvent *e){
	if(e->type() == QEvent::KeyPress){
		QKeyEvent *ke = (QKeyEvent*)e;
		if(ke->key() == Key_Tab){
			//这是特定的Tab处理
			k->accept();
			return TRUE;
		}
	}
	else if(e->type() >= QEvent::User){
		QCustomEvent *c = (QCustomEvent*)e;
		//这是自定义事件处理
		return TRUE;
	}
	QWidget::event(e);
}
```

# 元对象编译系统
Qt的元对象系统是用来处理对象间通信的信号和槽，它运行消息类型和动态属性，包括：
- QObject类；
- 类声明私有段中的Q_OBJECT宏；
- 元对象编译器。

元对象编译器读取C++源文件时，如果发现其中有类的声明中又含有Q_OBJECT宏，元对象编译器就会给这个类生成另一个含有元对象代码的C++源文件，这个生成的源文件可以被该类的源文件所包含，或者一起编译和链接。

QObject中的元对象代码除了提供对象间通信的信号和槽意外，还可实现其他特征：
1. className()函数在运行时以字符串返回类的名称，不需要C++编译器中的本地运行类型信息（RTTI）的支持；
2. inherits()函数返回的对象是一个继承于QObject继承树种一个特定类的实例；
3. tr()和trUtf8()两个函数是用于国际化中的字符串翻译；
4. setProperty()和property()两个函数用来通过名称动态设置而获得对象属性。metaObject()函数返回这个类所关联的元对象。

使用QObject作为一个基类，而不适用Q_OBJECT宏和元对象代码是可以的。但是如果宏没有被使用，那么这个类声明的信号/槽以及其他特征描述都不会被调用。

根据元对象系统的观点，一个没有元代码的QOject的子类与它含有元对象代码的最近的父类相同。

所以强烈建议 **QObject的所有子类使用Q_OBJECT宏，而不管它们是否实际使用了信号、槽和属性。**


## 元对象编译器
元对象包括所有信号/槽函数的名称和指针。

用户可以检查一个对象是否继承了一个特定的类：
`if(widget->inherits("QButton")){...}`