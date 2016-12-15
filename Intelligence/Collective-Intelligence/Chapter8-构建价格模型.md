**主要是用到了kNN最近邻算法.**
# 为近邻分配权重
##  Inverse Function

```python
def inverseweight(dist,num=1.0,const=0.1):
  return num/(dist+const)
```
潜在缺陷:对噪声过于敏感.(稍远项的权重衰减太快)

## Subtraction Function
克服前述对近邻项分配权重过大的潜在问题,但是也可能对于某些项根本无法作出预测.
```python
def subtractweight(dist,const=1.0):
  if dist>const:
    return 0
  else:
    return const-dist
```

## Gaussian Function
执行速度较慢.
```python
def gaussian(dist,sigma=5.0):
  return math.e**(-dist**2/(2*sigma**2))
```

# Weighted kNN
```python
def weightedknn(data,vec1,k=5,weightf=gaussian):
  # Get distances
  dlist=getdistances(data,vec1)
  avg=0.0
  totalweight=0.0

  # Get weighted average
  for i in range(k):
    dist=dlist[i][0]
    idx=dlist[i][1]
    weight=weightf(dist)
    avg+=weight*data[idx]['result']
    totalweight+=weight
  if totalweight==0: return 0
  avg=avg/totalweight
  return avg
```

# Cross-Validation
```python
def crossvalidate(algf,data,trials=100,test=0.1):
  error=0.0
  for i in range(trials):
    trainset,testset=dividedata(data,test)
    error+=testalgorithm(algf,trainset,testset)
  return error/trials
```


# Estimating the probability Density
```python
def probguess(data,vec1,low,high,k=5,weightf=gaussian):
  dlist=getdistances(data,vec1)
  nweight=0.0
  tweight=0.0

  for i in range(k):
    dist=dlist[i][0]
    idx=dlist[i][1]
    weight=weightf(dist)
    v=data[idx]['result']

    # Is this point in the range?
    if v>=low and v<=high:
      nweight+=weight
    tweight+=weight
  if tweight==0: return 0

  # The probability is the weights in the range
  # divided by all the weights
  return nweight/tweight
```

# Graphing the Probabilities
用到了高斯平滑?
ss指定平滑程度.
```python
def probabilitygraph(data,vec1,high,k=5,weightf=gaussian,ss=5.0):
  # Make a range for the prices
  t1=arange(0.0,high,0.1)

  # Get the probabilities for the entire range
  probs=[probguess(data,vec1,v,v+0.1,k,weightf) for v in t1]

  # Smooth them by adding the gaussian of the nearby probabilites
  smoothed=[]
  for i in range(len(probs)):
    sv=0.0
    for j in range(0,len(probs)):
      dist=abs(i-j)*0.1
      weight=gaussian(dist,sigma=ss)
      sv+=weight*probs[j]
    smoothed.append(sv)
  smoothed=array(smoothed)

  plot(t1,smoothed)
  show()
```

# Exercises
1. 留一式(Leave-one-out)交叉验证
>另一种计算预测误差的方法,将数据集中每一行看做单独的一个测试集,剩余部分看训练集.
