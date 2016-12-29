---
date: 2016-01-07 19:44
status: public
title: 'Chapter1 Making  Recommendations'
---

# 协作型过滤 Collaborative-Filtering
David Goldberg1992年提出。通常做法是对一大群人进行搜索，并从中找出品味详尽的一小群人。算法会对偏好的内容进行考查，并组合起来构造出一个经过排名的推荐列表。
## Collecting Preferences
嵌套字典，用户-物品-评价。
## Finding Similar Users
计算相似度评价值：通过欧几里得距离和皮尔逊相关度。
### 欧几里得距离评价 Euclidean Distance Score
以物品为坐标轴，构造偏好空间，用户为空间某点。用户是空间内确定的一点（对于多维情况，如有某些维评价值丧失？），两点距离越近，对应的用户偏好越相近。为建立正比例模型，且避免零除数问题，可将距离+1再取倒数构造新函数。函数值（相关系数）介于0到1之间，为1表示偏好一样，为0则完全不一样。

对于多维情况，如有某些维评价值丧失？该算法考虑用户共有的评价对象。 
```python
def sim_distance(prefs,object1,object2)
```
### 皮尔逊相关度评价 Pearson Correlation Score 
    
![](~/19-16-09.jpg)

很像线性拟合，相关系数评价值为R。
皮尔逊相关系数：是判断两组数据与某一直线拟合程度的一种度量。
在数据不是很规范（normalized)的时候，会倾向于给出更好的结果。

该算法以用户为坐标轴，物品为空间内某点。在该偏好空间的散点图上，作最佳拟合线（best-fit line)。
采用皮尔逊方法进行评价时，修正了“夸大分值（grade inflation）”的情况。（某用户总是倾向于比另一用户给出更高分值，但是最终直线仍然是拟合的。对于该情况，若采用欧几里得距离评价方法，会导致评价始终相对偏低，而得出两者不相近的结论。

```python
def sim_pearson(prefs,object1,object2)
```
## Which SImilarity Metric Should You Use?
取决于具体的应用。其他相似度计算函数有：Jaccard系数，曼哈顿距离算法。这些函数满足：拥有同样的函数签名；以一个浮点数作为返回值；数值越大代表相似度越大。
## Ranking the Critics
为用户匹配偏好相似者（匹配用户）

返回结果的个数（匹配排行榜）、相似度函数均为可选参数。另外必要参数为偏好字典（数据库）、某用户（为该用户提供推荐）。
```python
def topMatches(prefs,object,matchCount=5,similarity=sim_pearson)
```
## Recommending Items
为用户匹配偏好（匹配物品）

问题：评论者（用户）还未对某些对象做出评价。“对于多维情况，如有某些维评价值丧失？”

相似度作为权重进行加权值评分。

简单的情况就是，累加其他（other）用户与目标用户的相似度与其对某对象的评价值，即为该对象经加权后的评价总计值。可以该评价总计值计算排名，但最好除以评价过该对象的所有用户的相似度之和再计算排名。

```python
def getRecommedations(prefs,user,similarity=sim_pearson)
```
## Matching Products
将物品而非用户作为键名（Key)。

为用户选中的物品匹配相似物品。

def transformPrefs(prefs)

transform(), and topMatches()
## Building a del.icio.us Link Recommender
[del.cio.us](http://delicious.com)
# Item-Based Filtering
前文所述技术为User-Based Collaborative Filtering（基于用户的协作型过滤）。

在拥有大量数量集的情况下，Item-Based Collaborative Filtering（基于物品的协作型过滤）能够得出更好的结论，且允许预先执行大量计算任务。

预先计算的前提在于，物品间的比较不会像用户间的比较那样频繁变化。
## Building the Item Comparison Dataset
对物品间的相似度计算须频繁执行，以更新跟进用户评价。    

以物品为键，倒置原偏好矩阵。
```python
    def calculateSimilarItems(prefs,matchCount=10)
```
## Getting Recommendations
与前文Recommending Items中的getRecommendations(prefs,user,similarity=sim_pearson)有些类似。针对某一用户,为用户推荐物品。现对加权表作阐述，说明不同之处。

1.getRecomendations()
    横轴为选定用户还未评价过的物品，纵轴为其他用户。数据为其他用户对某物品的评价，权重是其他用户与选定用户的相似度。
    情景是获取新的推荐。

2.getRecommendedItems()
    横轴为选定用户还未评价过的物品，纵轴为选定用户评价过的物品，数据为物品间的相似度，权重是选定用户对评价过的物品的评价。
    情景是获取新的推荐。

两种获取推荐方式的结果有一定差异。
```python
    def getRecommendedItems(prefs,itemMatch,user)    
```
    calculateSimilarItems(),返回的itemMatch传入getRecommendedItems()

## Exercises
1. Tanimoto分值  
    求出Tanimoto相似度评价值。在何种情况下，可以将该方法作为相似性的度量方法，以替代欧几里得距离法或皮尔逊系数法？利用Tanimoto分值简历一个新的相似度函数。

2. 标签相似度

3. 基于用户算法的执行效率
    由于基于用户的过滤算法，在每次需要推荐时，都会将某位用户与其他所有用户进行比较，故而效率低下。编写一个预先计算用户相似度的函数，并修改涉及推荐的相关代码，只取出当前用户外的其他前5名用户来给出推荐。

4. 基于物品的书签过滤

5. Audioscrobbler
[访问](http://www.audioscrobbler.net)。音乐偏好数据集。利用其提供的API获取一组数据，以此构建一个音乐推荐系统。

#recommendations.py
```python:n
#coding= utf-8
critics={'Lisa Rose':{'Lady in the Water':2.5, 'Snakes on a Plane':3.5,'Just My Luck':3.0,'Superman Returns':3.5,'You, Me and Dupree':2.5,'The Night Listener':3.0},
	'Gene Seymour':{'Lady in the Water':3.0, 'Snakes on a Plane':3.5,'Just My Luck':1.5,'Superman Returns':5.0,'You, Me and Dupree':3.0,'The Night Listener':3.5},
	'Michael Phillips':{'Lady in the Water':2.5, 'Snakes on a Plane':3.0,'Superman Returns':3.5,'The Night Listener':4.0},
	'Claudia Puig':{'Snakes on a Plane':3.5,'Just My Luck':3.0,'Superman Returns':4.0, 'The Night Listener':4.5,'You, Me and Dupree':2.5},	
	'Mick LaSalle':{'Lady in the Water':3.0, 'Snakes on a Plane':4.0,'Just My Luck':2.0,'Superman Returns':3.0,'The Night Listener':3.0,'You, Me and Dupree':2.0},
	'Jack Matthews':{'Lady in the Water':3.0,'Snakes on a Plane':4.0,'The Night Listener':3.0,'Superman Returns':5.0,'You, Me and Dupree':3.5},
	'Toby':{'Snakes on a Plane':4.5,'You, Me and Dupree':1.0,'Superman Returns':4.0}
}

from math import sqrt
# 返回一个有关person1和person2的基于距离的相似度评价
def sim_distance(prefs,person1,person2):
	# 得到shared_items的列表
	si={}
	for item in prefs[person1]:
		if item in prefs[person2]:
			si[item]=1
	# 如果没有共同之处，返回0
	# 此处对分母区无穷大（即距离无限远）的算术情况做了处理
	if len(si)==0:return 0
	# 计算所有差值的平方和（求距离）
	# 对两者共有的评价，避免一者缺省
	sum_of_squares=sum([pow(prefs[person1][item]-prefs[person2][item],2)
		for item in prefs[person1] if item in prefs[person2]]) 
	# 返回函数值
	return 1/(1+sqrt(sum_of_squares))
    # 返回p1和p2的皮尔逊 	si={}
	for item in prefs[p1]:
		if item in prefs[p2]: si[item]=1
	
	# 得到列表元素的个数
	n=len(si)

	# 如果两者没有共同之处，则返回1
	if n==0: return 1

	# 对所有偏好求和
	sum1 = sum([prefs[p1][it] for it in si])
	sum2 = sum([prefs[p2][it] for it in si])

	# 求平方和
	sum1sq = sum([pow(prefs[p1][it],2) for it in si])
	sum2sq = sum([pow(prefs[p2][it],2) for it in si])

	# 求乘积之和
	pSum = sum([prefs[p1][it]*prefs[p2][it] for it in si])

	# 计算皮尔逊评价值
	num = pSum-(sum1*sum2/n)
	den = sqrt((sum1sq-pow(sum1,2)/n)*(sum2sq-pow(sum2,2)/n))
	if den==0: return 0
	r = num/den
	return r
    # 从反映偏好的字典中返回最为匹配者（同类匹配）
    # 返回结果的个数和相似度函数均为可选参数
def topMatches(prefs,person,n=5,similarity=sim_pearson):
	scores = [(similarity(prefs,person,other),other)
		for other in prefs if other!=person]
	# 对列表进行排序，评价值最高者排在最前
	scores.sort()
	scores.reverse()
	return scores[0:n]
# 利用所有他人评价值的加权平均为某人提供建议（异类匹配）
def getRecommendations(prefs,person,similarity=sim_pearson):
	totals={}
	simSums={}
	for other in prefs:
		# 不和自己作比较
		if other == person: continue
		sim = similarity(prefs,person,other)
		# 忽略评价值为0或小于0的情况
		if sim<=0: continue
		for item in prefs[other]:
			# 只对自己未接触未评价的对象进行评价
			if item not in prefs[person] or prefs[person][item]==0:
				# 相似度*评价值
				totals.setdefault (item,0)
				totals[item]+=prefs[other][item]*sim
				# 相似度之和
				simSums.setdefault(item,0)
				simSums[item]+=sim
		# 建立一个归一化的列表
		rankings=[(total/simSums[item],item) for item,total in totals.items()]
    	# 返回经过排序的列表
		rankings.sort()
		rankings.reverse()
		return rankings
 # 倒置偏好矩阵
def transformPrefs(prefs):
 	result={}
 	for person in prefs:
 		for item in prefs[person]:
 			result.setdefault(item,{})
            # 将对象和用户对调
 			result[item][person]=prefs[person][item]
	return result

    # 相似物品
def calculateSimilarItems(prefs,n=10):
	# 建立字典，以给出与这些物品最为相近的其他所有物品
	result={}

	# 以物品为中心，倒置偏好矩阵
	itemPrefs=transformPrefs(prefs)
	c=0
	for item in itemPrefs:
		# 针对大数量集更新状态变量
		c+=1
		if c%100==0: print "%d / %d " % (c,len(itemPrefs))
		# 寻找最为相近的物品
		scores=topMatches(itemPrefs,item,n=n,similarity=sim_distance)
		result[item]=scores
	return result

def getRecommendedItems(prefs,itemMatch,user):
	# itemMatch 是一个物品相似度的数据集，由calculateSimilarItems()得到
	userRatings = prefs[user]
	scores = {}
	totalSim = {}

	# 循环遍历由当前用户评分的物品
	for(item,rating) in userRatings.items():
		# 循环遍历与当前物品相近的物品
		for(similarity,item2) in itemMatch[item]:
			# 如果该用户已经对当前物品做过评价，则忽略之
			if item2 in userRatings: continue
			# 评价值与其相似度的加权之和
			scores.setdefault(item2,0)
			scores[item2]+=similarity*rating
			# 全部相似度之和
			totalSim.setdefault(item2,0)
			totalSim[item2]+=similarity
	# 将每个合计值除以加权和，求出平均值
	rankings = [(score/totalSim[item],item) for item,score in scores.items()]

	# 按最高值到最低值的顺序，返回评分结果
	rankings.sort()
	rankings.reverse()
	return rankings
```