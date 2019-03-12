# Training the Tree
使用CART(Classification and Regression Trees),即分类回归树算法。
```python
# Divides a set on a specific column. Can handle numeric
# or nominal values
def divideset(rows,column,value):
   # Make a function that tells us if a row is in
   # the first group (true) or the second group (false)mmmmmmmmmmmm
   split_function=None
   if isinstance(value,int) or isinstance(value,float):
      split_function=lambda row:row[column]>=value
   else:
      split_function=lambda row:row[column]==value

   # Divide the rows into two sets and return them
   set1=[row for row in rows if split_function(row)]
   set2=[row for row in rows if not split_function(row)]
   return (set1,set2)

```
# Gini Impurity 基尼不纯度
基尼不纯度是指将来自集合中的某种结果随机应用于集合中某一数据项的预期误差率。
如下代码，imp即为基尼不纯度，计算方法为：每一种结果的概率乘以其可能被误判为其他结果的概率的乘积和。
```python
# Probability that a randomly placed item will
# be in the wrong category
def giniimpurity(rows):
    total=len(rows)
    counts=uniquecounts(rows)
    imp=0
    for k1 in counts:
        p1=float(counts[k1])/total
        for k2 in counts:
          if k1==k2: continue
          p2=float(counts[k2])/total
          imp+=p1*p2
    return imp
```

# 熵 Entropy
entroy=p(i) * log(p(i))
```python
# Entropy is the sum of p(x)log(p(x)) across all
# the different possible results
def entropy(rows):
   from math import log
   log2=lambda x:log(x)/log(2)  
   results=uniquecounts(rows)
   # Now calculate the entropy
   ent=0.0
   for r in results.keys():
      p=float(results[r])/len(rows)
      ent=ent-p*log2(p)
   return ent

```

> 熵和基尼不纯度的主要区别在于，熵达到峰值的过程较慢，对于混乱集合的“判罚”更重，实际使用更为普遍。

# Recursive Tree Building
为了弄明白一个属性的好坏程度，首先求出整个群组的熵，然后尝试利用每个属性的可能取值对群组进行拆分，求出两个新群组的熵。然后计算当前熵与两个新群组加权平均后的熵之间的差值（Information Gain）。信息增益最大即为最好的属性。

```python
def buildtree(rows,scoref=entropy):
  if len(rows)==0: return decisionnode()
  current_score=scoref(rows)

  # Set up some variables to track the best criteria
  best_gain=0.0
  best_criteria=None
  best_sets=None

  column_count=len(rows[0])-1
  for col in range(0,column_count):
    # Generate the list of different values in
    # this column
    column_values={}
    for row in rows:
       column_values[row[col]]=1
    # Now try dividing the rows up for each value
    # in this column
    for value in column_values.keys():
      (set1,set2)=divideset(rows,col,value)

      # Information gain
      p=float(len(set1))/len(rows)
      gain=current_score-p*scoref(set1)-(1-p)*scoref(set2)
      if gain>best_gain and len(set1)>0 and len(set2)>0:
        best_gain=gain
        best_criteria=(col,value)
        best_sets=(set1,set2)
  # Create the sub branches   
  if best_gain>0:
    trueBranch=buildtree(best_sets[0])
    falseBranch=buildtree(best_sets[1])
    return decisionnode(col=best_criteria[0],value=best_criteria[1],
                        tb=trueBranch,fb=falseBranch)
  else:
    return decisionnode(results=uniquecounts(rows))
```

# Pruning the Tree ----->>>>> improve overfitted
剪枝。

```python
def prune(tree,mingain):
    # If the branches aren't leaves, then prune them
    if tree.tb.results==None:
        prune(tree.tb,mingain)
    if tree.fb.results==None:
        prune(tree.fb,mingain)

    # If both the subbranches are now leaves, see if they
    # should merged
    if tree.tb.results!=None and tree.fb.results!=None:
        # Build a combined dataset
        tb,fb=[],[]
        for v,c in tree.tb.results.items():
            tb+=[[v]]*c
        for v,c in tree.fb.results.items():
            fb+=[[v]]*c

        # Test the reduction in entropy
        delta=entropy(tb+fb)-(entropy(tb)+entropy(fb)/2)

        if delta<mingain:
            # Merge the branches
            tree.tb,tree.fb=None,None
            tree.results=uniquecounts(tb+fb)
```

# 总结
决策树对于有大量数值型输入和输出的问题不是一个好选择.
