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