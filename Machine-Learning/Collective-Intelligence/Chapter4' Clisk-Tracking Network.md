---
date: 2016-03-19 23:59
status: public
title: 'Chapter4'' Clisk-Tracking Network'
---

# Learning from Clicks
在线应用的一个最大优势在于，可以持续收到以用户行为为表现形式的反馈信息。

下面介绍一种方法来记录用户点击查询结果的情况，病利用这一信息来改进搜索结果的排名。

为此须要构造一个人工神经网络，向其提供：
*查询条件中的单词，返回给用户的搜索结果，以及用户的点击决策*

然后再对其加以训练。一旦网络经过了许多不同查询的训练之后，我们就可以用来改进搜索结果排序，以更好地反映过去一段时间里的实际点击情况。

# Design of  a Click-Tracking Network
神经网络有许多种不同类型，都以一组节点（神经元）构成，并且彼此相连。
多层感知机(multilayer perceptron, MLP)) 网络，由多层神经元构成，其中第一层神经元接受输入，最后一层神经元给予输出。

神经网络可以有多个中间层，本例中只使用了一层。中间层与外界不能直接交互，又叫隐藏层。

本节还要展示如何利用一种反向传播(backpropagation)算法对网络进行训练。

# Setting Up the Database
须要将反映网络现状的信息存入数据库中。数据库中已经有一张涉及单词和URL的数据表，另外还需要一张代表隐藏层的数据表（称之为hiddennode），以及两张反映网络节点连接状况的表（一张从单词层到隐藏层，另一张连接隐藏层到输出层）。

新建一个nn.py，新建一个类searchnet.
```python:n
from math import tanh
from pysqlite2 import dbapi2 as sqlite

class searchnet:
    def __init__(self,dbname):
        self.con = sqlite.connect(dbname)
    def __del__(self):
        self.con.close()
    def maketables(self):
        self.con.execute('create table hiddennode(create_key)')
        self.con.execute('create table wordhidden(fromid,toid,strength)')
        self.con.execute('create table hiddenurl(fromid,toid,strength)')
        self.con.commit()
```

这些表还没有索引。如果存在效率问题，稍后可以添加索引。  
为了访问数据库，须要创建两个方法：getstrength()判断当前连接的强度。由于新连接只在必要时才会创建，因此若连接不存在时该方法将会返回一个默认值。（从单词层到隐藏层是-0.2，从隐藏层到URL是0）

```python:n
def getstrength(self,fromid,toid,layer):
    if layer == 0:  table = 'wordhidden'
    else:   table = 'hiddenurl'
    res = self.con.execute('select strength from %s where fromid=%d and toid=%d' % (table,fromid,toid)).fetchone()
    if res == None:
        if layer == 0:  return -0.2
        if layer == 1:  return 0
    return res[0]
```

此外还需要一个setstrength方法，来判断连接是否已存在，并利用新的强度值更新连接或创建连接。训练神经网络的代码需要用到这个函数。
```python:n
def setstrength(self,fromid,toid,layer,strength):
    if layer == 0: table = 'wordhidden'
    else: table = 'hiddenurl'
    res = self.con.execute('select rowid from %s where fromid=%d and toid=%d' % (table,fromid,toid)).fetchone()
    if res == None:
        self.con.execute('insert into %s (fromid,toid,strength) values (%d,%d,%f)' % (table,fromid,toid,strength))
    else:
        rowid = res[0]
        self.con.execute('update %s set strength=%f where rowid=%d' % (table,strength,rowid))
```

大多数时候所有节点都是预先建好的。但是在本例中，只在需要时建立新的隐藏节点会更高效，也更简单。

每传入一组以前未见过的单词组合，该函数就会在隐藏层中新建一个新的节点，随后为单词和隐藏节点之间、以及查询节点与由查询所返回的URL结果之间，这两个之间，建立起具有默认权重的连接。
```python:n
def generatehiddennode(self,wordids,urls):
    if len(wordids)>3: return None
    # 检查是否已为这组单词建好了一个节点
    createkey = '_'.join(sorted([str(wi) for wi in wordids]))
    res = self.con.execute("select rowid from hiddennode where create_key='%s'"%createkey).fetchone()
    #如果没有就建立
    if res == None:
        cur = self.con.execute("insert into hiddennode (create_key) values ('%s')"%createkey)
        hiddenid = cur.lastrowid
        #设置默认权重
        for wordid in wordsids:
            self.setstrength(wordid,hiddenid,0,1.0/len(wordids))
        for urlid in urls:
            self.setstrength(hiddenid,urlid,1,0.1)
        self.con.commit()
```


## nn.py
[nn.py](/Collective-Intelligence/_attachment/Chapter4 Searching and Ranking/nn.markdown)