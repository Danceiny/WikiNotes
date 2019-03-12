# Genetic Programming
1. 根据给定的数据集重新构造一个数学函数;
2. 在一个简单的棋类游戏中自动生成一个AI玩家.

## Programs As Trees
四个类:
1. fwrapper
>一个封装类,对应于"函数型"节点上的函数.成员变量包括函数名称\函数本身\该函数接受的参数个数.

2. node
>对应于函数型节点(有子节点的节点).以一个fwrapper类对其进行初始化.evaluate调用时,对各个子节点进行求值运算,然后再将函数本身应用于求得的结果.

3. paramnode
>对应的节点只返回传递给程序的某个参数,其evaluate方法返回由idx指定的参数.

4. constnode
>返回常量值的节点.其evaluate方法仅返回该类被初始化时所传入的值.       

```python
class fwrapper:
  def __init__(self,function,childcount,name):
    self.function=function
    self.childcount=childcount
    self.name=name

class node:
  def __init__(self,fw,children):
    self.function=fw.function
    self.name=fw.name
    self.children=children

  def evaluate(self,inp):    
    results=[n.evaluate(inp) for n in self.children]
    return self.function(results)
  def display(self,indent=0):
    print (' '*indent)+self.name
    for c in self.children:
      c.display(indent+1)


class paramnode:
  def __init__(self,idx):
    self.idx=idx

  def evaluate(self,inp):
    return inp[self.idx]
  def display(self,indent=0):
    print '%sp%d' % (' '*indent,self.idx)


class constnode:
  def __init__(self,v):
    self.v=v
  def evaluate(self,inp):
    return self.v
  def display(self,indent=0):
    print '%s%d' % (' '*indent,self.v)
```

针对节点的操作函数(使用fwrapper):
```python
addw=fwrapper(lambda l:l[0]+l[1],2,'add')
subw=fwrapper(lambda l:l[0]-l[1],2,'subtract')
mulw=fwrapper(lambda l:l[0]*l[1],2,'multiply')

def iffunc(l):
  if l[0]>0: return l[1]
  else: return l[2]
ifw=fwrapper(iffunc,3,'if')

def isgreater(l):
  if l[0]>l[1]: return 1
  else: return 0
gtw=fwrapper(isgreater,2,'isgreater')

flist=[addw,mulw,ifw,gtw,subw]


```

## 构造树
```python
def exampletree():
  return node(ifw,[
                  node(gtw,[paramnode(0),constnode(3)]),
                  node(addw,[paramnode(1),constnode(5)]),
                  node(subw,[paramnode(1),constnode(2)]),
                  ]
              )
```
对于`exampletree().evaluate([2,3])`,运行过程如下:
1. class.node.evaluate()函数,results是一个数组,该数组是三个子节点evaluate得到的值,然后传参results给ifw函数(显然results长度为3).
2. 对三个子节点,分别调用gtw,addw,subw(大于判断,加,减)函数,参数均为两个,第一个参数是从父节点得到的参数(即从[2,3]中取出的值),第二个参数是常量.

## 构造初始种群
pc是所需输入参数的个数.
```python
def makerandomtree(pc,maxdepth=4,fpr=0.5,ppr=0.6):
  if random()<fpr and maxdepth>0:
    f=choice(flist)
    children=[makerandomtree(pc,maxdepth-1,fpr,ppr)
              for i in range(f.childcount)]
    return node(f,children)
  elif random()<ppr:
    return paramnode(randint(0,pc-1))
  else:
    return constnode(randint(0,10))
```

## 测试题解
测试遗传编程算法的一个最简单的例子就是重新构造一个简单的数学函数----根据一组对应的输入输出数据.
评估函数一如既往的重要.

## Mutating Programs [变身^-^基因突变]
变异概率.下面的变异方式是小于某较小概率则用一棵新树替换某一个子树.
```python
def mutate(t,pc,probchange=0.1):
  if random()<probchange:
    return makerandomtree(pc)
  else:
    result=deepcopy(t)
    if hasattr(t,"children"):
      result.children=[mutate(c,pc,probchange) for c in t.children]
    return result
```
## Crossover [配对遗传]
```python
def crossover(t1,t2,probswap=0.7,top=1):
  if random()<probswap and not top:
    return deepcopy(t2)
  else:
    result=deepcopy(t1)
    if hasattr(t1,'children') and hasattr(t2,'children'):
      result.children=[crossover(c,choice(t2.children),probswap,0)
                       for c in t1.children]
    return result
```

## Building the Environment
```python

def evolve(pc,popsize,rankfunction,maxgen=500,
           mutationrate=0.1,breedingrate=0.4,pexp=0.7,pnew=0.05):
  # Returns a random number, tending towards lower numbers. The lower pexp
  # is, more lower numbers you will get
  def selectindex():
    return int(log(random())/log(pexp))

  # Create a random initial population
  population=[makerandomtree(pc) for i in range(popsize)]
  for i in range(maxgen):
    scores=rankfunction(population)
    print scores[0][0]
    if scores[0][0]==0: break

    # The two best always make it
    newpop=[scores[0][1],scores[1][1]]

    # Build the next generation
    while len(newpop)<popsize:
      if random()>pnew:
        newpop.append(mutate(
                      crossover(scores[selectindex()][1],
                                 scores[selectindex()][1],
                                probswap=breedingrate),
                        pc,probchange=mutationrate))
      else:
      # Add a random node to mix things up
        newpop.append(makerandomtree(pc))

    population=newpop
  scores[0][1].display()    
  return scores[0][1]
```

evolve的参数:
- rankfunction
> 将一组程序按从优到劣的顺序进行排序的函数;

- mutationrate
>发生变异的概率.

- breedingrate
>配对概率.

- popsize
>初始种群的大小.

- probexp
>构造新的种群时,"选择评价较低的程序"这一概率的递减比率.该值越大,相应筛选过程就越严格,即 *只选择评价最高者作为复制对象的概率就越大*.

- probnew
构造新的种群时,"引入一个全新的随机程序"的概率. (多样性的重要价值)


## The Importance of Diversity
避免homogeneous近亲交配.



# A Simple Game
游戏规则:网格区域,双人对战,玩家选择4个方向任意一个进行移动,目标是将自己移到对方所在区域,附加规则是在一行的同一个方向移动两次为认输.
