---
date: 2016-01-26 00:21
status: public
title: 'Chapter4-Searching and Ranking'
---

本章介绍的**全文搜索引擎**,是最重要的集体智慧算法之一。Google的PageRank算法。
Information retrieval（信息检索）是一个巨大领域。
本章介绍如何**检索网页（crawl)** 、 **建立索引** 、**对网页进行搜索** 、 以及 **以多种不同方式对搜索的结果进行排名**。
# What's in a Search Engine?
建立搜索引擎的首要步骤是找到一种搜集文档的方法。
然后建立索引。须要建立一张大的表，表中包含了文档及所有不同单词的位置信息。
然后通过查询返回一个经过排序的文档列表。此处真正的奥妙之处在于结果的排列方式。
本章最后将建立一个神经网络，用以对查询结果进行排名。通过了解人们在得到搜索结果以后都点击了哪些链接，神经网络会将搜索过程与搜索结果关联起来。

为了运行示例，须要建立一个模块，取名为searchengine，其中包含两个类：
* 一个用于检索网页和创建数据库；
* 一个通过查询数据库进行全文搜索；
在示例中，用到了SQLite数据库。
首先新建searchengine.py文件，加入crawler类和相应方法签名。
```python:n
class crawler:
		# 初始化crawler类并传入数据库名称
		def __init__(self,dbname):
			pass

		def __del__(self):
			pass

		def dbcommit(self):
			pass

		# 辅助函数，用于获取条目的id,并且如果条目不存在，就将其加入数据库中
		def  getentryid(self,table,field,value,createnew=True):
			return None

		#为每个网页建立索引
		def addtoindex(self,url,soup):
			print 'Indexing %s ' % url

		# 从一个HTML网页中提取文字（不带标签的）
		def gettextonly(self,soup):
			return None

		# 根据任何非空白字符进行分词处理
		def separatewords(self,text):
			return None

		# 如果url已经建过索引，返回true
		def isindexed(self,url):
			return False

		# 添加一个关联两个网页的链接
		def addlinkref(self,urlFrom,urlTo,linkText):
			pass

		# 从一小组网页开始进行广度优先搜索，直至某一给定深度
		# 期间为网页建立索引
		def crawl(self,pages,depth=2):
			pass

		# 创建数据库表
		def createindextables(self):
			pass
```
# A Simple Crawler
在searchengine.py首部前面加入：
```python:n
import urllib2
from BeautifulSoup import *
from urlparse import urljoin
# 构建一个单词列表，被忽略的
ignorewords=set(['the','of','to','and','a','in','is','it'])
```
然后为crawler类中的函数填入代码：
```python:n
		def crawl(self,pages,depth=2):
			for i in range(depth):
				newpages=set()
				for page in pages:
					try:
						c=urllib2.urlopen(page)
					except:
						print "Could not open %s " % page
						continue
					soup=BeautifulSoup(c.read())
					self.addtoindex(page,soup)

					links = soup('a')
					for link in links:
						if('href' in dict(link.attr)):
							url=urljoin(page,link['href'])
							if url.find(" ' ") != -1: continue
							url = url.split('#')[0] # 去掉位置部分
							if url[0:4]=='http' and not self.isindexed(url):
								newpages.add(url)
							linkText=self.gettextonly(link)
							self.addlinkref(page,url,linkText)

					self.dbcommit()
				pages=newpages
```
该函数循环遍历网页列表，并针对每个网页调用addtoindex函数（暂未充实）。随后，该函数利用BeautifulSoup取到网页中的所有链接，并将这些链接加入到一个名为newpages的集合中。

上述函数也可以采用递归的形式定义。但是如果递归，每个链接都会触发函数的再次调用。但是，广度优先的搜索方式可以使代码的后续修改更为容易，我们可以一直持续进行检索，也可以将未经索引的网页列表保存起来，以备后续再行检索之用。同时也避免了栈溢出的风险。

试运行：
```python:n
>>import searchengine
>>pagelist=['http://kiwitobes.com/']
>>crawler=searchengine.crawler('')
>>crawler.crawl(pagelist)
```
# Building the Index
接下来为全文索引建立数据库。
索引对应一个列表，其中包含了所有不同的单词、这些单词所在的文档，以及单词在文档中出现的位置。

在searchengine.py的起始处import pysqlite2模块。
`from pysqlite2 import dbapi2 as sqlite`
并且修改`__init__`、`__del__`、`dbcommit`方法，以打开关闭数据库。
```python
def __init__(self,dbname):
	self.con = sqlite.connect(dbname)
def __del__(self):
	self.con.close()
def dbcommit(self):
	self.con.commit()
```

# Setting Up the Schema
准备数据库表。
1. urllist保存的是已经索引过的URL列表。
2. wordlist保存的是单词列表。
3. wordlocation保存的是单词在文档中所处位置的列表。
4. linkb保存了两个URL ID,指明从一张表到另一张表的链接关系。
5. linkwords表利用字段wordid和linkid记录了哪些单词与链接实际相关。

完整的数据库schema如下图所示。
 ![](~/schema.png)

 所有SQLite中的表默认都有一个名为rowid的字段，因此没必要显式地制定ID字段。
 建立一个函数将数据填入所有数据表中，将下列代码加入searchengine.py文件末尾(crawler类的一部分)。
 ```python
def createindextables(self):
	self.con.execute('create table urllist(url)')
	self.con.execute('create table wordlist(word)')
	self.con.execute('create table wordlocation(urlid,wordid,location)')
	self.con.execute('create table link(fromid integer,toid integer)')
	self.con.execute('create table linkwords(wordid,linkid)')
	self.con.execute('create index wordidx on wordlist(word)')
	self.con.execute('create index urlidx on urllist(url)')
	self.con.execute('create index wordurlidx on wordlocation(wordid)')
	self.con.execute('create index urltoidx on link(toid)')
	self.con.execute('create index urlfromidx on link(fromid)')
	self.dbcommit()
 ```

 该函数的用途是为即将用到的所有表建立scheme，并建立一些旨在加快搜索速度的索引。

下面在python会话中输入以下命令以建立一个数据库searchindex.db。
```
>> import searchengine
>> crawler = searchengine.crawler('searchindex.db')
>> crawler.createindextables()
```

之后还将在schema中加入一张表，以根据外部回指链接 **inbound link** 的计数情况评价度量值。

# Finding the Words on a page
对HTML文件的处理。
```python
def gettextonly(self,soup);
	v = soup.string
	if v == None:
		c = soup.contents
		resulttext = ' '
		for t in c:
			subtext = self.gettextonly(t)
			resulttext += subtext+'\n'
		return resulttext
	else:
		return v.strip()
```
该函数返回一个包含网页中所有文字的长字符串。
以向下递归的方式对HTML文档对象模型进行遍历，找出其中的文本节点。
为了便于之后某些度量的计算，此处应保留各章节的前后顺序。

接下来是separatewords函数，将字符串拆分成一组独立的单词，以便加入索引。

对于本章示例，将任何非字母或非数字的字符作为分隔符，对于英文单词的提取不会有问题，但是无法正确处理类似C++这样的词汇。
需要用到正则表达式改进之。
```python
def separatewords(self.text):
	splitter = re.compile('\\W*')
	return [s.lower() for s in splitter.split(text) if s!=' ']
```
> **词干提取算法Stemming Algorithm** 去除单词后缀。Porter Stemmer是一个有名的词干提取python实现 。

# Adding to the Index
接下来实现addtoindex方法。该方法调用前面所定义的两个函数，得到一个出现在网页中的单词的列表。然后，将网页及所有单词加入索引，在网页和单词之间建立关联，并保存单词在文档中出现的位置。对示例而言，单词位置location就是其在列表中的索引号i。
```python
def addtoindex(self,url,soup):
	if self.isindexed(url):return
	print 'Indexing' +url
	# 获取每个单词
	text = self.gettextonly(soup)
	words = self.separatewords(text)

	# 得到URL的id
	urlid = self.getentryid('urllist','url',url)

	# 将每个单词与该url关联
	for i in range(len(words)):
		word = words[i]
		if word in ignorewords: continue
		wordid = self.getentryid('wordlist','word',word)
		sefl.con.execute("insert int  wordlocation(urlid,wordid,location) \
			values(%d,%d,%d)" % (urlid,wordid,i))
```
其中还调用了getentryid函数。该函数返回某一条目的ID，如果条目不存在，则程序在数据库中新建一条记录并返回ID。
```python
def getentryid(self,table,field,value,createnew=True):
	cur = self.con.execute("select rowid from %s where %s='%s'" % (table,field,values))
	res = cur.fetchone()
	if res == None:
		cur = self.con.execute("insert into %s (%s) values ('%s')" % (table,field,value))
		return cur.lastrowid
	else:
		return res[0]
```
以及isindexed函数。该函数判断网页是否已存入数据库，如果存在则判断是否有单词与之关联。
```python
def isindexed(self,url):
	u = self.con.execute("select rowid from urllist where url='%s'" % url).fetchone()
	if u!=None:
		# 检查是否已被检索过
		v = self.con.execute('select * from wordlocation where urlid=%d' % u[0]).fetchone()
		if v!=None: return True
	return False
```
现在可以执行crawler，并在程序运行期间真正为网页建立索引。
```
>> reload(searchengine)
>> crawler=searchengine.crawler('searchindex.db')
>> pages = ['http://huangzhen.farbox.com']
>> crawler.crawl(pages)
```

发现没卵用。could not  parse page...（运行完整源代码情况下）

>目前的代码一次只能处理一个单词，而且以文档当初被加载的顺序返回文档。

# Querying
接下来开始准备搜索引擎的搜索部分。
首先在searchengine.py中新建一个用于搜索的类。
```python
class searcher:
	def __init__(self,dbname):
		self.con = sqlite.connect(dbname)

	def __del__(self):
		self.con.close()
```
wordlocation表为连接单词与数据表提供了一种简单方法。不过需要搜索引擎允许多词搜索。
建立一个查询函数，接受一个查询字符串作为参数，并将其拆分成多个单词，然后构造一个SQL查询，只查找那些包含所有不同单词的URL。
将该函数添加到searcher类定义中：
```python:n
def getmatchrows(self,q):
		# 构造查询的字符串
		fieldlist = 'w0.urlid'
		tablelist = ' '
		clauselist = ' '
		wordids = []
		# 根据空格拆分单词
		words = q.split(' ')
		tablenumber = 0
		for word in words:
		# 获取单词的ID
			wordrow = self.con.execute(
				"select rowid from wordlist where words = '%s'" % word).fetchone()
			if wordrow != None:
				wordrow = wordrow[0]
				wordids.append(wordid)
				if tablenumber > 0:
					tablelist += ','
					clauselist += ' and '
					clauselist += 'w%d.urlid=w%d.urlid and ' % (tablenumber-1,tablenumber)
				fieldlist += ',w%d.location ' % tablenumber
				tablelist += 'wordlocation w%d ' % tablenumber
				clauselist += 'w%d.wordid = %d' % (tablenumber,wordid)
				tablenumber += 1
		# 根据各个组分建立查询
		fullquery = 'select %s from %s where %s' % (fieldlist,tablelist,clauselist)		#此处fieldlist等三个应该都是长字符串，包含不止一组查询对象，多对多查询？
		cur  = self.con.execute(fullquery)
		rows = [row for row in cur]

		return rows,wordids
```
该函数所实现的只是为列表中的每个单词建立一个指向wordlocation表的引用，并根据对应的urlid将它们连结起来进行联合查询。

![](~/12-03-04.jpg)

举例来说，一个设计两个单词（分别对应ID10,17）的查询如下所示：
```sql
select w0.urlid, w0.location, w1.location
from wordlocation w0, wordlocation w1
where w0.urlid = w1.urlid
and w0.wordid = 10
and w1.wordid = 17
```
尝试调用：
```
> import searchengine
> e = searchengine.searcher('searchindex.db')
> e.getmatchrows('functional programming')
([(1,327,23), (1,327,162),.........])
```

可以注意到，根据单词位置的不同组合，此处每个URL ID会返回多次。
下面介绍几种对搜索结果进行排名的方法。
**基于内容的排名法Content-based ranking** 是根据网页的内容，利用某些可行的度量方式来对查询结果进行判断。
**外部回指链接排名法Inbound-link ranking** 则是利用站点的链接结构来决定查询结果中各项内容的重要程度。

# Content-Based Ranking
评价度量：
1.  **单词频度**
>位于查询条件中的单词在文档中出现的次数能有助于我们判断文档的相关程度。

2. **文档位置**
>文档的主题有可能会出现在靠近文档的开始处。

3. **单词距离**
>如果查询条件中有多个单词，则他们在文档中出现的位置应该靠得很近。

searcher类中的getscoredlist方法接受查询请求，将获取到的行集置于字典中，并以格式化列表的形式显示输出。

```python:n
def getscoredlist(self,rows,wordids):
	totalscores=dict([(row[0],0) for row in rows])

	# This is where we'll put our scoring functions later
	weights=[]

	for (weight,scores) in weights:
		for url in totalscores:
			totalscores[url]+=weight*scores[url]

	return totalscores

def geturlname(self,id):
	return self.con.execute(
		"select url from urllist where rowid=%d" % id).fetchone()[0]

def query(self,q):
	rows,wordids=self.getmatchrows(q)
	scores=self.getscoredlist(rows,wordids)
	rankedscores=sorted([(score,url) for (url,score) in scores.items(0)],reverse=1)
	for (score,urlid) in rankedscores[0:10]:
		print '%f\t%s' % (score,self.geturlname(urlid)))
```

query方法现在还没有对结果进行任何评价。
加入评价函数后，即可添加对weights列表（query结果）的调用。

# Normalization Function
归一化函数接受一个包含ID和评价值的字典，并返回一个带有相同ID，而评价值介于[0,1]的新字典。
函数根据每个评价值与最佳结果的接近程度（最佳结果的对应值为1），对其做相应的缩放处理。

```python:n
def normalizescores(self,scores,smallIsBetter=0):
	vsmall = 0.00001	# Avoid division by zero errors
	if smallIsBetter:
		minscore = min(scores.values())
		return dict([(u,float(minscore)/max(vsmall,1)) for (u,1) in scores.items()])
	else:
		maxscore=max(scores.values())
		if maxscore == 0: maxscore=vsmall
		return dict([(u,float(c)/maxscore) for (u,c) in scores.items()])
```

# Word Frequency
这种方法以 **单词** 频度作为度量手段，根据查询条件中的单词在网页中出现的次数对网页进行评价。假如要搜索"python"，更希望得到一个内容中多词提到该单词的有关"python"语言的网页。

单词频度函数如下所示，可以将其加入searcher类中：
```python
def frequencyscore(self,rows):
	counts = dict([(row[0],0) for row in rows])
	for row in rows: counts[row[0]]+=1
	return self.normalizescores(counts)
```
函数对每个单词出现的次数进行了计数，作为评价值，并归一化处理。

为了在返回结果中使用频度评价算法，修改getscoredlist中的weights一行为：
`weights = [(1.0,self.frequencyscore(rows))]`

# Document Location
另一个判断网页与查询条件相关程度的简单度量方法，是搜索单词在网页中的位置。通常，如果一个网页与待搜索的单词相关，该单词更可能出现在靠近网页开始处的位置，亦或是标题中。
目前，网页已建立了索引，单词在文档中的位置已记录，网页的标题位于列表中的第一项。
```python
def locationscore(self,rows):
	locations = dict([(row[0],100000) for row in rows])
	for row in rows:
		loc = sum(row[1:])
		if loc < locations[row[0]]:	locations[row[0]] = loc
	return self.normalizescores(locations,smallIsBetter=1)
```
**行集中的每一行的第一项是URL ID，后面紧跟的是所有各待查单词的位置信息，形如(1,34,543,...),即(UrlID,Location1,Location2...)。每个ID可以出现多次，每次对应的是不同的位置组合。针对每一行，该方法计算所有单词的位置之和，并将这一结果与迄今为止的最佳结果进行对比判断。**

将weights一行修改如下：`weights = [(1.0,self.locationscore(rows))]`
即可在解释器中再查询一次查看效果。

*取决于搜索者的意图，上述各种搜索结果都是有效的；而且对于一组特定的文档与应用而言，为了给出最佳结果，不同的加权组合也是必要的。*

例如修改weights一行如下：
`weights = [(1.0,self.frequencyscore(rows)),	(1.5,self.locationscore(rows))]`

# Word Distance
查询中包含多个单词时，寻找单词彼此间距很近的网页是很有意义的。

distancescore函数类似于locationscore：
```python:n
def distancescore(self,rows):
    # 如果只有一个单词，则得分一样
    if len(rows[0])<2: return dict([(row[0],1.0) for row in rows])
    # 初始化字典，并填入一个很大的数
    mindistance = dict([(row[0],1000000) for row in rows])

    for row in rows:
        dist = sum([abs(row[i]-row[i-1]) for i in range(2,len(row))])
        if dist < mindistance[row[0]]:    
            mindistance[row[0]]=dist
    return self.normalizescores(mindistance,smallIsBetter=1)
```
此处的主要区别在于，当函数循环遍历单词的位置时，会计算出每个位置与上一个位置间的差距。由于查询返回每一种距离组合，因此函数最终会找出总距离的最小值。

# Using Inbound Links
外部回指链接。

前面所讨论的评价度量，都是基于网页内容。

利用之前所编写的爬虫程序，可以获取到与链接有关的所有重要信息。
对于每一个遇到的链接，links表中记录了与其源和目的相对应的URLID，而且linkwords表还记录了单词与链接的关联。

# Simple Count
处理外部回指链接最简单的做法，就是在每个网页上统计链接的总数，以此作为度量。这有点像科研论文的被引用数指标。

```python:n
def inboundlinkscore(self,rows):
    uniqueurls = set([row[0] for row in rows])
    inboundcount = dict([(u,self.con.excute(\
        'select count(*) from link where toid=%d' % u).fetchone()[0]) for u in uniqueurls])
    return self.normalizescores(inboundcount)
```
这里的算法对每个外部回指链接都给予了同样的权重。

接下来在计算排名的过程中，将令来自热门网页的链接拥有更高的权重值。
为了将相关性与排名结合起来，需要结合外部回指链接与此前介绍过的任意一种度量方法。
# PageRank算法
>理论上，PageRank计算的是某个人在任意次链接点击之后到达某一网页的可能性。
>如果某个网页拥有来自其他热门网页的外部回指链接越多，人们无意间到达该网页的可能性也就越大。
>大多数人在浏览一段时间后会停止点击。为了反映这个情况，PageRank使用了一个值为0.85的阻尼因子，用以指示用户持续点击每个网页中链接的概率为85%。

对链接A,为了得到A的PageRank值，我们将指向A的每个网页的PageRank(PR)值除以这些网页的链接总数，然后乘以阻尼因子0.85，再加上一个0.15的最小值。公式如下：
`PR(A) = 0.15 + 0.85 * ( PR(B)/links(B) + PR(C)/links(C) + ...)`

看公式，问题出来了，有一个PR的嵌套啊。

这只有在知道了指向同一网页的所有其他网页的评价值后，我们才能计算出该网页的评价值。
那么如何对一组还没有PageRank值的网页进行PageRank计算呢？

为所有的PageRank都设置一个任意的初始值。然后反复计算，迭代若干次。

迭代次数要视网页的数量而定。

因为PageRank的计算是一项耗时的工作，而其计算结果又不随查询的变化而变化。因此可以建立一个函数，预先为每个URL计算好PageRank值，并将计算结果存入数据表中。加入到crawler类中。
```python:n
def calculatepagerank(self,iterations=20):
    # 清除当前的PageRank表
    self.con.execute('drop table if exists pagerank')
    self.con.execute('create table pagerank(urlid primary key,score)')
    self.con.execute('insert into pagerank select rowid, 1.0 from urllist')
    self.dbcommit(0

    for i in range(iterations):
        print "Iteration %d" % (i)
        for (urlid,) in self.con.execute('select rowid from urllist'):
            pr = 0.15
            # 循环遍历指向当前网页的所有其他网页
            for (linker,) in self.con.execute('select distinct fromid from link where toid=%d' % urlid):
            # 得到链接源对应网页的PageRank值
            linkingpr=self.con.execute('select score from pagerank where urlid=%d' % linker).fetchone()[0]
            # 根据链接源，求得总的链接数
            linkingcount=self.con.execute('select count(*) from link where fromid=%d' % linker).fetchone()[0]
            pr+=0.85*(linkingpr/linkingcount)
        self.con.execute('update pagerank set score=%f where urlid=%d' % (pr,urlid))
    self.commit()        
```            
运行函数：
```
import searchengine
crawler=searchengine.crawler('searchengine.db')
crawler.calculatepagerank()
```
若要知道示例数据集中哪个网页的PageRank值最高，可以直接查询数据库。
```
cur=cawler.con.execute('select * from pagerank order by score desc')
for i in range(3): print cur.next()
...
e.geturlname(438)# e=searchengine.searcher('searchengine.db')
...
```
现在我们已经拥有了一张包含PageRank评价值的数据表，那么只需创建一个函数，将评价值从数据库中取出归一化处理，即可利用这些数据。
```python:n
def pagerankscore(self,rows):
   pageranks = dict([row[0],self.con.execute('select score from pagerank where urlid=%d' % row[0]).fetchone()[0]) for row in rows])
   maxrank = max(pageranks.values())
   normalizedscores=dict([(u,float(l)/maxrank) for (u,l) in pageranks.items()])
   return normalizedscores
```

再次修改weights权值表，加入PageRank算法。
```python:n
weights = [(1.0,self.locationscore(rows)),
					(1.0,self.frequencyscore(rows)),
					(1.0,self.pagerankscore(rows))]
```
对于返回更高层次和更大众化的网页结果而言，PageRank是一种有效的度量方法。

# Using the Link Text
利用链接文本，是根据指向某一网页的链接文本来决定网页的相关程度。

大多数时候，相比被链接的网页自身所提供的信息而言，从指向该网页的链接中所得到的信息更有价值。
**因为针对其所指向的网页，网站的开发者们倾向于提供一些解释其内容的简短描述。**

根据链接的文本对网页进行评价的方法接受一个额外的参数——一个单词ID的列表。这个列表是在执行查询时得到的。
```python:n
def linktextscore(self,rows,wordids):
	linkscores = dict([(row[0],0) for row in rows])
	for wordid in wordids:
		cur = self.con.execute('select link.fromid,link.toid from linkwords,link where wordid=%d and linkwords.linkid=link.rowid' % wordid)
		for (fromid,toid) in cur:
			if toid in linkscores:
				pr = self.co.execute('select score from pagerank where urlid=%d' % fromid).fetchone()[0]
				linkscores[toid] += pr
	maxscore = max(linkscores.values())
	normalizedscores = dict([(u,float(l)/maxscore) for (u,l) in linkscores.items()])
	return normalizedscores
```
循环遍历wordids中所有单词，查找包含这些单词的链接。如果链接的目标地址与搜索结果中的某条数据匹配，则链接源对应的PageRank值被加入评价值。

一个网页，如果拥有大量来自其他 *重要网页* 的链接指向，且这些网页又满足查询条件，则该网页评价值较高。

所用到的度量方法，以及赋予的权重系数，很大程度上取决于正在试图构建的应用。


# source code
## searchengine.py
[searchengine.py](/Collective-Intelligence/_attachment/Chapter4 Searching and Ranking/searchengine.markdown)
