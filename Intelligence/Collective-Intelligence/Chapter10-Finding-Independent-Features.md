# Intro
研究如何在数据集未明确标识结果的前提下,从中提取出重要的潜在特征.

方法的目的不是为了预测,而是尝试对数据进行特征识别.


# 搜集信息(数据来源)
创建feedlist,并下载,再转换成矩阵,用贝叶斯分类,聚类画图,


# Non-Negative Matrix Factorization 非负矩阵因式分解
和文章矩阵的关系:目前为止,已有矩阵(需分解的矩阵)为一个带单词计数信息的文章矩阵.

因式分解得到的两个小矩阵分别是 **特征矩阵** 和 **权重矩阵**.

1. 特征矩阵

|特征\单词|hurricane|democrats|florida|elections|
|---|---|---|---|---|
|特征1|2|0|3|0|
|特征2|0|2|0|1|
|特征3|0|0|1|1|

2. 权重矩阵

|文章\特征|特征1|特征2|特征3|
|---|---|---|---|
|hurricane in Florida|10|0|0|
|Democrats sweep elections|0|8|1|
|Democrats dispute Florida ballots|0|5|6|


如果特征数量和文章数量相等,最理想的结果就是能为每一篇文章都找到一个与之完美匹配的特征.

**非负是指其返回的特征和权重都是负值.**


## Solution 1: 如果利用numpy进行算法实现

度量最终结果与理想结果的接近程度:
```python
from numpy import *

def difcost(a,b):
  dif=0
  for i in range(shape(a)[0]):
    for j in range(shape(a)[1]):
      # Euclidean Distance
      dif+=pow(a[i,j]-b[i,j],2)
  return dif

```
现在需要成本函数来逐步更新矩阵.(可以利用退火优化算法或群优化算法搜索得到更优解).


## Solution 2: 乘法更新法则
这里介绍一种更为有效的方法: **[乘法更新法则(multiplicative update rules)](http://hebb.mit.edu/people/seung/papers/nmfconverge.pdf)**.

该方法从原矩阵衍生出4个新的更新矩阵(update matrices):
1. hn
>经转置后的权重矩阵与原矩阵相乘得到的矩阵.

2. hd
>经转置后的权重矩阵与原权重矩阵相乘,再与特征矩阵相乘得到的矩阵.

3. wn
>原矩阵与经转置后的特征矩阵相乘得到的矩阵.

4. wd
>权重矩阵与特征矩阵相乘,再与经转置后的特征矩阵相乘得到的矩阵.

***更新过程:将上述所有矩阵转为数组,然后将特征矩阵中的每一个值与hn中的对应值相乘,并除以hd中的对应值;对权重矩阵作类似计算.
factorize函数完成上述计算任务,但是需要指定希望找到的特征数量.***
```python
def factorize(v,pc=10,iter=50):
  ic=shape(v)[0]
  fc=shape(v)[1]

  # Initialize the weight and feature matrices with random values
  w=matrix([[random.random() for j in range(pc)] for i in range(ic)])
  h=matrix([[random.random() for i in range(fc)] for i in range(pc)])

  # Perform operation a maximum of iter times
  for i in range(iter):
    wh=w*h

    # Calculate the current difference
    cost=difcost(v,wh)

    if i%10==0: print cost

    # Terminate if the matrix has been fully factorized
    if cost==0: break

    # Update feature matrix
    hn=(transpose(w)*v)
    hd=(transpose(w)*w*h)

    h=matrix(array(h)*array(hn)/array(hd))

    # Update weights matrix
    wn=(v*transpose(h))
    wd=(w*h*transpose(h))

    w=matrix(array(w)*array(wn)/array(wd))  

  return w,h
```

## 呈现结果
showfeatures函数,输出结果见features.txt.

调用:
```python
import newsfeatures
import nmf
allw,artw,artt = newsfeatures.getarticlewords()
wordmatrix,wordvec = newsfeatures.makeMatrix(allw,artw)
v=matrix(wordmatrix)
weights,feat = nmf.factorized(v,pc=20,iter=50)
topp,pn = newsfeatures.showfeatures(weights,feat,artt,wordvec)
```
