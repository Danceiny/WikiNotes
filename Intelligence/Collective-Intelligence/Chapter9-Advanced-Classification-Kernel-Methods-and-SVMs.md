# 点积
对于线性分类器,根据两向量的点积正负判断分类:
`class = sign((X - (M0 + M1)/2) . (M0-M1)) = sign(X . M0 - X . M1 + (M0 . M0 - M1 . M1)/2) = sign(X.M0 - X.M1 + (M0+M1)/2 . (M0-M1))`
```python
def dotproduct(v1,v2):
  return sum([v1[i]*v2[i] for i in range(len(v1))])

def veclength(v):
  return sum([p**2 for p in v])

def dpclassify(point,avgs):
  b=(dotproduct(avgs[1],avgs[1])-dotproduct(avgs[0],avgs[0]))/2
  y=dotproduct(point,avgs[0])-dotproduct(point,avgs[1])+b
  if y>0: return 0
  else: return 1
```

# 将SVM用于婚介数据集

## 特征分析
数据集每一行形如:
`39,yes,no,skiing:knitting:dancing,220 W 42nd St New York NY,43,no,yes,soccer:reading:scrabble,824 3rd Ave New York NY,0`
家庭住址分析:住得越近越可能匹配成功.借用GoogleMaps API. `def milesdistance(a1,a2)`
兴趣列表分析:共同爱好的数量(忽略潜在的不同却"匹配"的爱好).

## 构造新的数据集

```python
def loadnumerical():
  oldrows=loadmatch('matchmaker.csv')
  newrows=[]
  for row in oldrows:
    d=row.data
    data=[float(d[0]),yesno(d[1]),yesno(d[2]),
          float(d[5]),yesno(d[6]),yesno(d[7]),
          matchcount(d[3],d[8]),
          milesdistance(d[4],d[9]),
          row.match]
    newrows.append(matchrow(data))
  return newrows
```
需要对数据进行缩放处理:

```python
def scaledata(rows):
  low=[999999999.0]*len(rows[0].data)
  high=[-999999999.0]*len(rows[0].data)
  # Find the lowest and highest values
  for row in rows:
    d=row.data
    for i in range(len(d)):
      if d[i]<low[i]: low[i]=d[i]
      if d[i]>high[i]: high[i]=d[i]

  # Create a function that scales data
  def scaleinput(d):
     return [(d[i]-low[i])/(high[i]-low[i])
            for i in range(len(low))]

  # Scale all the data
  newrows=[matchrow(scaleinput(row.data)+[row.match])
           for row in rows]

  # Return the new data and the function
  return newrows,scaleinput

```


# The Kernel Trick
对于任何用到了点积运算的算法(包括线性分类器),我们可以采用一种叫做核技法的技术.

核技法的思路是用一个新函数替代原来的点积函数,当借助某个映射函数将数据第一次变换到更高维度的坐标空间时,新函数将返回高维度坐标空间内的点积结果.
比较经典的一种"新函数"就是 **径向基函数(radial-basis-function)**.

```python
def rbf(v1,v2,gamma=10):
  dv=[v1[i]-v2[i] for i in range(len(v1))]
  l=veclength(dv)
  return math.e**(-gamma*l)

```
即完成非线性分类:
```python
def nlclassify(point,rows,offset,gamma=10):
  sum0=0.0
  sum1=0.0
  count0=0
  count1=0

  for row in rows:
    if row.match==0:
      sum0+=rbf(point,row.data,gamma)
      count0+=1
    else:
      sum1+=rbf(point,row.data,gamma)
      count1+=1
  y=(1.0/count0)*sum0-(1.0/count1)*sum1+offset

  if y>0: return 0
  else: return 1

```

# 使用LIBSVM
`sudo apt install libsvm-dev`


**---------------SVM TRAIN------------------**
```python
>>> answers,inputs = [r.match for in scaledset],[r.data for r in scaledset]
>>> param = svm_parameter(kernel_type = RBF)
>>> prob = svm_problem(answers,inputs)
>>> m = svm_model(prob,param)
*
optimization finished ....

>>> newrow = [28.0,-1,-1,26.0,-1,1,2,0.8]   # 男士不想要小孩,女士想要
>>> m.predict(scalef(newrow))
0.0
>>> newrow = [28.0,-1,1,26.0,-1,1,2,0.8]    #双方都想要小孩
>>> m.predict(scalef(newrow))
1.0

# 交叉验证
>>> guesses = cross_validation(prob,param,4)
...
>>> sum([abs(answers[i] - guesses[i]) for i in range(len(guesses))])
116.0

```

# Matching on Facebook
