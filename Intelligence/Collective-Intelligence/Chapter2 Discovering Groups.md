---
date: 2016-01-10 21:15
status: public
title: Chapter2 Discovering Groups
---

# 数据聚类 data clustering
用于data-intensive的应用中。

## Supervised versus Unsupervised Learning 监督学习/无监督学习
Supervised Learning:样本输入，期望输出。包括：神经网络，决策树，向量支持机，贝叶斯过滤。

Unsupervied Learning的一个例子：聚类。聚类算法的目标是采集数据，从中找出不同群组。其他例子还包括Non-negative matrix factorization（负矩阵因式分解）和self-organizing maps（自组织映射）。

## Word Vectors
为聚类算法准备数据的常见做法：定义一组公共的数值型属性。

## Pigeonholing the Bloggers
数据集：某单词在各个用户的博客中出现的次数。 [blogdata.txt](http://huangzhen.farbox.com/post/collective-intelligence/blogdata)

## Counting the Words in a Feed
解析RSS订阅源XML文档。解析工具：[Universal Feed Parser]。

某些词（如"the"）过于高频而无实际效用，故应选择介于某个百分比范围内的单词，即定上下边界。     generatefeedvector.py 该py的主体代循环遍历订阅源并生成数据集。 遍历[feedlist.txt](/Collective-Intelligence/feedlist.txt)文件中的每一行，然后生成针对每个博客的单词统计，以及出现这些单词的博客数目(apcount)。然后建立单词列表，用于针对每个博客的单词计数。最后利用单词列表和博客列表建立一个文本文件。[blogdata.txt](/Collective-Intelligence/blogdata.txt)<br>** 本算法所依赖的一个RSS订阅源解析软件，以及博客文章数据集获取URL失效。数据集的文件格式为：包含一个以制表符分隔的表格，其中的每一列对应一个单词，每一行对应一个博客。**

## Hierarchical Clustering 分级聚类
分级聚类通过连续不断地将最为相似的群组两两合并，来构造出一个群组的层级结构。可用树状图(dendrogram)表现所得结果。

每次迭代过程中，分类聚集算法会计算每两个群组间的距离，并将距离最近的两个群组合并成一个新的群组。直到只剩一个群组为止。

紧密度closeness。本例中，皮尔逊相关度判断的是两组数据与某条直线的拟合程度，因此可以纠正"某些博客在总体上比其他博客包含更多的词汇"的问题。

在代码中，设立一个class存放聚类属性,以此来描述这棵层级树。

分级聚类算法以一组对应于原始数据项的聚类开始。函数主循环部分尝试每一组可能的配对并计算相关度，以此找出最佳配对。最佳配对合并为新聚类。新聚类所包含的数据，等于将两个旧聚类的数据求均值之后得到的结果。重复该过程。

```python
def hcluster(rows,distance=pearson)
```

因为每个聚类都指向构造该聚类的两个被合并的聚类，所以可以递归搜索由该函数最终返回的聚类，以重建所有的聚类及叶节点。

## Drawing the Dendrogram 绘制树状图
 需要知道给定聚类的总体高度。聚类是叶节点（即孤点），则高度为1，否则是所有分支高度之和。

```python
  def getheight(clust)
```

- 需要知道根节点的总体误差。线条的长度会根据每个节点的误差进行相应调整，所以需要根据总体误差生成一个scaling factor（在源代码生成的下图中表现为图片宽度的缩放比）。一个节点的误差深度等于其下所述的每个分支的最大可能误差。

  ```python
   def getdepth(clust)
  ```

- 关键部分在于节点，drawnode()函数。接受一个聚类，及其位置作为输入参数。函数取得子节点的高度，并计算出这些节点所在位置，然后用线条（一条垂直线和两条水平线）连接起来。水平线的长度由聚类中的误差情况（某聚类是一个bicluster类，类属性distance是描述聚合成该类的两个子类的相似度)决定。水平线越长，表明两聚类差距越大。

  ```python
  def drawnode(draw,clust,x,y,scaling,labels)
  ```

  ![](~/blogclust.jpeg)

  ```python:n
   from PIL import Image,ImageDraw
   def drawdendrogram(clust,labels,jpeg='clusters.jpg'):
     # height and width
     h=getheight(clust)*20
     w=1200
     depth=getdepth(clust)
     # width is fixed, so scale distances accordingly
     scaling=float(w-150)/depth
     # Create a new image with a white background
     img=Image.new('RGB',(w,h),(255,255,255))
     draw=ImageDraw.Draw(img)
     draw.line((0,h/2,10,h/2),fill=(255,0,0))    
     # Draw the first node
     drawnode(draw,clust,10,(h/2),scaling,labels)
     img.save(jpeg,'JPEG')
  ```

## Column Clustering
  同时在行和列上进行聚类。为提高函数复用性，可作行列转置。使单词变成行，每一行对应一组数字，指明该词在每篇博客中出现的次数。

```python
   def rotatematrix(data)
```

## K-Means Clustering
  分级聚类的缺点：计算量过大；树形视图不会真正将数据拆分成不同组。

  K-均值聚类完全不同于分级聚类。需要生成的聚类数量的预先确定的，算法依据数据的结构状况来确定聚类大小。

  K-均值聚类算法首先随机确定k个中心位置（位于空间中代表聚类中心的点），然后将各个数据项分配给最邻近的中心点。分配完成后，聚类中心会移到分配给该聚类的所有节点的平均位置处。这整个分配过程周而复始直到分配过程不再发生变化为止。

   如图：

  ![](~/20-38-37.jpg)

```python
 def kcluster(rows,distance=pearson,k=4):
```

```
确定每个点的最小值和最大值，并随机创建k个中心点：
```

```python
    # rows的每一行（即一个点），有个最小值和最大值。ranges是一个这样的二维数组
    ranges = [(min(row[i] for row in rows]),max(row[i] for row in rows])) for i in  range(len(rows[0]))]
    # 随机数要满足随机性（伪随机数？)有空要好好研究一下** 随机数 **。
    clusters = [[random.random()*(ranges[i][1]-ranges[i][0])+ranges[i][0] for i in range(len(rows[0])) ] for j in range(k)]
```

# Clusters of Preferences
## Getting and Preparing the Data
### Beautiful Soup
解析网页，构造结构化数据表达形式的函数库。 其描述Web页面的方式是构造一个soup对象。     举例：

```
    >>>import urllib2    
    >>>from BeautifulSoup import BeautifulSoup
    >>>c = urllib2.urlopen('http://...')
    >>>soup = BeautifulSoup(c.read())
    >>>links = soup('a')
    >>>links[10]
    <a href="..." title="...">...</a>
    >>>links[10]['href']
    ...
```

## Scraping the Zebo results
 [zebo.txt](/Collective-Intelligence/zebo.txt)  见downloadzebodata.py

## Defining a Distance Metric
皮尔逊相关度适合博客数据集。但对于zebo.txt这样的取值只有0和1两种的数据集，Tanimoto系数（Tanimoto coefficient)的度量方法更为适合。 Tanimito coefficient代表的是交集（只在两个集合中都出现的项）与并集（包含在所有出现于任一集合中的项）的比率。<br>下面给出代码：

```python:n
    def tanimoto(v1,v2):
        c1,c2,shr=0,0,0
        for i in range(len(v1)):
            if v1[i]!=0:c1+=1
            if v2[i]!=0:c2+=1
            if v1[i]!=0 and v2[i]!=0:shr+=1
        tani=float(shr)/(c1+c2-shr)
        return 1.0-tani
```

  如下图，聚类是人们所希望拥有的物品。从市场营销角度看，反映了消费者的群组性。 ![](~/clusters.jpg)

## Viewing Data in Two Dimensions
在本章中已经采用了一种程式化的数据可视化的表达方法，在二维空间中演示了聚类算法的运用。利用了不同物品间的图上距离来指示差异。

由于在大多数真实场景中，所要聚类的内容都不止含有两个数值，所以不可能按照前述方法采集数据并以二维形式绘制。

但是，为了清晰不同物品间的关系，将其绘在一张图中并以距离远近表示相似程度，是一种非常有效的做法。

### Muitidimensional Scaling 多维缩放技术
该算法根据每对数据项之间的差距情况，尝试绘制出一幅图来，图中各数据项之间的距离远近，对应于它们彼此间的差异程度。

为此，算法 **先要计算出所有项之间的目标距离** 。在博客数据集中，采用皮尔逊相关度对各数据项进行比较。如下表所示：


|  | A   | B   | C   | D |
|---|----|---|---|---|
|A | 0.0 | 0.2 | 0.8 | 0.7|
|B | 0.2 | 0.0 | 0.9 | 0.8|
|C | 0.8 | 0.9 | 0.0 | 0.1|

下一步，将所有数据项（本例中为博客）随机放置在二维图上。**所有数据项两两间的当前距离值由实际距离（差平方之和）计算求得。** （此处实际距离是指 **随机放置** 所致的距离？）
针对每两两构成的一对数据项，将它们的目标距离与当前距离进行比较，求出一个误差值。根据误差，按照比例将每个数据项的所在位置移近移远。

每一个节点的移动，都是所有其他节点施加在该节点的推或拉的综合效应。节点每移动一次，其当前距离和目标距离间的差距就会减少一些。这一过程不断重复，直到无法再通过移动节点来减少总体误差为止。

实现这一功能的函数接受一个数据向量作为参数，并返回一个只包含两列的向量，即数据项在二维图上的横纵坐标。
`def scaledown(data,distance=pearson,rate=0.01)`
执行：
```python
>>import clusters
>>blognames,words,data=clusters.readfile('blogdata.txt')
>>coords=clusters.scaledown(data)
>>clusters.draw2d(coords,blognames,jpeg='blog2d.jpg')
```
blog2d.jpg如下图。
![](~/blogs2d.jpg)
## Other Things to Cluster
根据数据项所包含的参数信息将其绘制在一个空间范围内。
在多维缩放的过程中，一些信息可能会丢失掉。
# Exercises
1. 利用delicious API 构造一个适合聚类的标签数据集 针对该数据集分别运行分类聚集算法和K-均值聚类算法。
2. 修改解析博客的代码，以实现针对每个文章条目（entries）而非整个博客的聚类 来自同一博客的不同条目能否聚类在一起？拥有相同日期信息的条目又如何？
3. 尝试使用实际距离（即毕达哥拉斯距离）对博客进行聚类 这样会对结果产生什么样的影响呢？
4. 找到**曼哈顿Manhattan距离**的定义 为其编写一个函数，看看它是如何影响Zebo数据集的结果的？
5. 请修改K-均值聚类函数 令其在返回聚类结果的同时，一并返回所有数据项彼此之间的距离总和，以及它们各自的中心点位置。
6. 完成第5个练习后，编写一个函数，令其选择不同的K值来运行K-均值聚类算法 看看总的距离值是如何随着聚类数的增加而改变的？当处于哪个位置的时候，聚类数的多少对最终结果的影响才会变得微乎其微？
7. 在两个维度上的多维缩放易于打印 不过这项技术也可以用于任意数量的维度。尝试修改代码，以实现在一个维度上的缩放（即所有点都在一条直线上）。再尝试令其支持三个维度。
# source code
## generatefeedvector.py

```python:n
# -*- coding: -*- utf-8
import feedparser
import re
    # 返回一个RSS订阅源的标题和包含单词计数情况的字典
def getwordcounts(url):
    # 解析订阅源
    d = feedparser.parse(url)
    # 数据集wc
    wc = {}
    # 循环遍历所有的文章条目
    for e in d.entries:
        if 'summary' in e: summary=e.summary
        else: summary=e.description
        # 提取一个单词列表
        words = getwords(e.title+' '+summary)
        for word in words:
            wc.setdefault(word,0)
            wc[word]+=1
    return d.feed.title,wc

def getwords(html):
    # 去除所有HTML标记
    txt = re.compile(r'<[^>]+>').sub('',html)
    # 利用所有非字母字符拆分出单词
    words = re.compile(r'[^A-Z^a-z]+').split(txt)
    # 转换成小写模式
    return [word.lower() for word in words if word!='']

apcount={}
wordcounts={}
    # feedlist.txt中的每一行对应一个url
feedlist = [line for line in file('feedlist.txt')]
for feedurl in feedlist:
    title,wc = getwordcounts(feedurl)
    wordcounts[title]=wc
    for word,count in wc.items():
        apcount.setdefault(word,0)
        if count>1:
            apcount[word]+=1

wordlist=[]
    # w,bc分别表示所采集数据集中的word及该word在（某篇文章？所以文章条目？）出现的次数
for w,bc in apcount.items():
    # frac表示在（某篇文章？所有文章条目？）该词出现的频率
    frac = float(bc)/len(feedlist)
    if frac>0.1 and frac<0.5: wordlist.append(w)
    # 利用上述单词列表和博客列表来建立一个文本文件，包含一个大的矩阵，记录着针对每个博客的所有单词的统计情况
    # 生成的文本文件blogdata.txt需要被cluster.py中的函数读取

out = file('blogdata.txt','w')
out.write('Blog')
for word in wordlist: out.write('\t%s' % word)
out.write('\n')
for blog,wc in wordcounts.items(0):
    out.write(blog)
    for word in wordlist:
        if word in wc: out.write('\t%d' % wc[word])
    else: out.write('\t0')
    out.write('\n')
```

## clusters.py

```python:n
def readfile(filename):
  lines=[line for line in file(filename)]

  # First line is the column titles
  colnames=lines[0].strip().split('\t')[1:]
  rownames=[]
  data=[]
  for line in lines[1:]:
    p=line.strip().split('\t')
    # First column in each row is the rowname
    rownames.append(p[0])
    # The data for this row is the remainder of the row
    data.append([float(x) for x in p[1:]])
  return rownames,colnames,data


from math import sqrt

def pearson(v1,v2):
  # Simple sums
  sum1=sum(v1)
  sum2=sum(v2)

  # Sums of the squares
  sum1Sq=sum([pow(v,2) for v in v1])
  sum2Sq=sum([pow(v,2) for v in v2])    

  # Sum of the products
  pSum=sum([v1[i]*v2[i] for i in range(len(v1))])

  # Calculate r (Pearson score)
  num=pSum-(sum1*sum2/len(v1))
  den=sqrt((sum1Sq-pow(sum1,2)/len(v1))*(sum2Sq-pow(sum2,2)/len(v1)))
  if den==0: return 0

  return 1.0-num/den

class bicluster:
  def __init__(self,vec,left=None,right=None,distance=0.0,id=None):
    self.left=left
    self.right=right
    self.vec=vec
    self.id=id
    self.distance=distance

def hcluster(rows,distance=pearson):
  distances={}
  currentclustid=-1

  # Clusters are initially just the rows
  clust=[bicluster(rows[i],id=i) for i in range(len(rows))]

  while len(clust)>1:
    lowestpair=(0,1)
    closest=distance(clust[0].vec,clust[1].vec)

    # loop through every pair looking for the smallest distance
    for i in range(len(clust)):
      for j in range(i+1,len(clust)):
        # distances is the cache of distance calculations
        if (clust[i].id,clust[j].id) not in distances:
          distances[(clust[i].id,clust[j].id)]=distance(clust[i].vec,clust[j].vec)

        d=distances[(clust[i].id,clust[j].id)]

        if d<closest:
          closest=d
          lowestpair=(i,j)

    # calculate the average of the two clusters
    mergevec=[
    (clust[lowestpair[0]].vec[i]+clust[lowestpair[1]].vec[i])/2.0
    for i in range(len(clust[0].vec))]

    # create the new cluster
    newcluster=bicluster(mergevec,left=clust[lowestpair[0]],
                         right=clust[lowestpair[1]],
                         distance=closest,id=currentclustid)

    # cluster ids that weren't in the original set are negative
    currentclustid-=1
    del clust[lowestpair[1]]
    del clust[lowestpair[0]]
    clust.append(newcluster)

  return clust[0]

def printclust(clust,labels=None,n=0):
  # indent to make a hierarchy layout
  for i in range(n): print ' ',
  if clust.id<0:
    # negative id means that this is branch
    print '-'
  else:
    # positive id means that this is an endpoint
    if labels==None: print clust.id
    else: print labels[clust.id]

  # now print the right and left branches
  if clust.left!=None: printclust(clust.left,labels=labels,n=n+1)
  if clust.right!=None: printclust(clust.right,labels=labels,n=n+1)

def getheight(clust):
  # Is this an endpoint? Then the height is just 1
  if clust.left==None and clust.right==None: return 1

  # Otherwise the height is the same of the heights of
  # each branch
  return getheight(clust.left)+getheight(clust.right)

def getdepth(clust):
  # The distance of an endpoint is 0.0
  if clust.left==None and clust.right==None: return 0

  # The distance of a branch is the greater of its two sides
  # plus its own distance
  return max(getdepth(clust.left),getdepth(clust.right))+clust.distance


def drawdendrogram(clust,labels,jpeg='clusters.jpg'):
  # height and width
  h=getheight(clust)*20
  w=1200
  depth=getdepth(clust)

  # width is fixed, so scale distances accordingly
  scaling=float(w-150)/depth

  # Create a new image with a white background
  img=Image.new('RGB',(w,h),(255,255,255))
  draw=ImageDraw.Draw(img)

  draw.line((0,h/2,10,h/2),fill=(255,0,0))    

  # Draw the first node
  drawnode(draw,clust,10,(h/2),scaling,labels)
  img.save(jpeg,'JPEG')

def drawnode(draw,clust,x,y,scaling,labels):
  if clust.id<0:
    h1=getheight(clust.left)*20
    h2=getheight(clust.right)*20
    top=y-(h1+h2)/2
    bottom=y+(h1+h2)/2
    # Line length
    ll=clust.distance*scaling
    # Vertical line from this cluster to children    
    draw.line((x,top+h1/2,x,bottom-h2/2),fill=(255,0,0))    

    # Horizontal line to left item
    draw.line((x,top+h1/2,x+ll,top+h1/2),fill=(255,0,0))    

    # Horizontal line to right item
    draw.line((x,bottom-h2/2,x+ll,bottom-h2/2),fill=(255,0,0))        

    # Call the function to draw the left and right nodes    
    drawnode(draw,clust.left,x+ll,top+h1/2,scaling,labels)
    drawnode(draw,clust.right,x+ll,bottom-h2/2,scaling,labels)
  else:   
    # If this is an endpoint, draw the item label
    draw.text((x+5,y-7),labels[clust.id],(0,0,0))

def rotatematrix(data):
  newdata=[]
  for i in range(len(data[0])):
    newrow=[data[j][i] for j in range(len(data))]
    newdata.append(newrow)
  return newdata

import random

def kcluster(rows,distance=pearson,k=4):
  # Determine the minimum and maximum values for each point
  ranges=[(min([row[i] for row in rows]),max([row[i] for row in rows]))
  for i in range(len(rows[0]))]

  # Create k randomly placed centroids
  clusters=[[random.random()*(ranges[i][1]-ranges[i][0])+ranges[i][0]
  for i in range(len(rows[0]))] for j in range(k)]

  lastmatches=None
  for t in range(100):
    print 'Iteration %d' % t
    bestmatches=[[] for i in range(k)]

    # Find which centroid is the closest for each row
    for j in range(len(rows)):
      row=rows[j]
      bestmatch=0
      for i in range(k):
        d=distance(clusters[i],row)
        if d<distance(clusters[bestmatch],row): bestmatch=i
      bestmatches[bestmatch].append(j)

    # If the results are the same as last time, this is complete
    if bestmatches==lastmatches: break
    lastmatches=bestmatches

    # Move the centroids to the average of their members
    for i in range(k):
      avgs=[0.0]*len(rows[0])
      if len(bestmatches[i])>0:
        for rowid in bestmatches[i]:
          for m in range(len(rows[rowid])):
            avgs[m]+=rows[rowid][m]
        for j in range(len(avgs)):
          avgs[j]/=len(bestmatches[i])
        clusters[i]=avgs

  return bestmatches

def tanamoto(v1,v2):
  c1,c2,shr=0,0,0

  for i in range(len(v1)):
    if v1[i]!=0: c1+=1 # in v1
    if v2[i]!=0: c2+=1 # in v2
    if v1[i]!=0 and v2[i]!=0: shr+=1 # in both

  return 1.0-(float(shr)/(c1+c2-shr))

def scaledown(data,distance=pearson,rate=0.01):
  n=len(data)

  # The real distances between every pair of items
  realdist=[[distance(data[i],data[j]) for j in range(n)]
             for i in range(0,n)]

  # Randomly initialize the starting points of the locations in 2D
  loc=[[random.random(),random.random()] for i in range(n)]
  fakedist=[[0.0 for j in range(n)] for i in range(n)]

  lasterror=None
  for m in range(0,1000):
    # Find projected distances
    for i in range(n):
      for j in range(n):
        fakedist[i][j]=sqrt(sum([pow(loc[i][x]-loc[j][x],2)
                                 for x in range(len(loc[i]))]))

    # Move points
    grad=[[0.0,0.0] for i in range(n)]

    totalerror=0
    for k in range(n):
      for j in range(n):
        if j==k: continue
        # The error is percent difference between the distances
        errorterm=(fakedist[j][k]-realdist[j][k])/realdist[j][k]

        # Each point needs to be moved away from or towards the other
        # point in proportion to how much error it has
        grad[k][0]+=((loc[k][0]-loc[j][0])/fakedist[j][k])*errorterm
        grad[k][1]+=((loc[k][1]-loc[j][1])/fakedist[j][k])*errorterm

        # Keep track of the total error
        totalerror+=abs(errorterm)
    print totalerror

    # If the answer got worse by moving the points, we are done
    if lasterror and lasterror < totalerror: break
    lasterror=totalerror

    # Move each of the points by the learning rate times the gradient
    for k in range(n):
      loc[k][0]-=rate*grad[k][0]
      loc[k][1]-=rate*grad[k][1]

  return loc

def draw2d(data,labels,jpeg='mds2d.jpg'):
  img=Image.new('RGB',(2000,2000),(255,255,255))
  draw=ImageDraw.Draw(img)
  for i in range(len(data)):
    x=(data[i][0]+0.5)*1000
    y=(data[i][1]+0.5)*1000
    draw.text((x,y),labels[i],(0,0,0))
  img.save(jpeg,'JPEG')
```

## downloadzebodata.py

```python:n
from BeautifulSoup import BeautifulSoup
import urllib2
import re
chare=re.compile(r'[!-\.&]')
itemowners={}

# Words to remove
dropwords=['a','new','some','more','my','own','the','many','other','another']

currentuser=0
for i in range(1,51):
  # URL for the want search page
  c=urllib2.urlopen(
  'http://member.zebo.com/Main?event_key=USERSEARCH&wiowiw=wiw&keyword=car&page=%d'
  % (i))
  soup=BeautifulSoup(c.read())
  for td in soup('td'):
    # Find table cells of bgverdanasmall class
    if ('class' in dict(td.attrs) and td['class']=='bgverdanasmall'):
      items=[re.sub(chare,'',str(a.contents[0]).lower()).strip() for a in td('a')]
      for item in items:
        # Remove extra words
        txt=' '.join([t for t in item.split(' ') if t not in dropwords])
        if len(txt)<2: continue
        itemowners.setdefault(txt,{})
        itemowners[txt][currentuser]=1
      currentuser+=1

out=file('zebo.txt','w')
out.write('Item')
for user in range(0,currentuser): out.write('\tU%d' % user)
out.write('\n')
for item,owners in itemowners.items():
  if len(owners)>10:
    out.write(item)
    for user in range(0,currentuser):
      if user in owners: out.write('\t1')
      else: out.write('\t0')
    out.write('\n')
```
