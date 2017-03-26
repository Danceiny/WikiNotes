# Ch02
## Part 1 Dating
import kNN
group,labels = kNN.createDataSet()
group
labels
kNN.classify0([0,0],group,labels,3)
datingDataMat,datingLabels = kNN.file2matrix('datingTestSet2.txt')


import matplotlib
import matplotlib.pyplot as plt
fig = plt.figure()
ax = fig.add_subplot(111)
ax.scatter(datingDataMat[:,1],datingDataMat[:,2])
plt.show()


normMat,ranges,minVals = kNN.autoNorm(datingDataMat)
normMat
ranges
kNN.datingClassTest()
kNN.classifyPerson()


## Part2 Handwriting Recognition
testVector = kNN.img2vector('testDigits/0_13.txt')


# Ch03 Decision Tree（ID3）

import trees
myDat,labels = trees.createDataSet()
myDat
trees.calcShannonEnt(myDat)
myDat[0][-1] = 'maybe'
myDat
trees.calcShannonEnt(myDat)

// splitDataSet
trees.splitDataSet(myDat,0,1)
trees.splitDataSet(myDat,0,0)


//trees
myDat,labels = trees.createDataSet()
myTree = trees.createTree(myDat,labels)
myTree

//plot tree node
import treePlotter
treePlotter.createPlot()
treePlotter.retrieveTree(1)
myTree = treePlotter.retrieveTree(0)
treePlotter.getNumLeafs(myTree)
treePlotter.getTreeDepth(myTree)

myTree = trees.classify(myTree,labels,[1,0])
myTree = trees.classify(myTree,labels,[1,1])


fr = open('lenses.txt')
lenses = [inst.strip().split('\t') for inst in fr.readlines()]
lensesLabels = ['age','prescript','astigmatic','tearRate']
lensesTree = trees.createTree(lenses,lensesLabels)
lensesTree
treePlotter.createPlot(lensesTree)



# Ch04 Bayes
import bayes
listOPosts,listClasses = bayes.loadDataSet()
myVocabList = bayes.createVocabList(listOPosts)
myVocabList

import numpy as np
listOPosts,listClasses = bayes.loadDataSet()
myVocabList = bayes.createVocabList(listOPosts)
trainMat=[]
for postinDoc in listOPosts:
    trainMat.append(bayes.setOfWords2Vec(myVocabList,postinDoc))
p0V,p1V,pAb = bayes.trainNB0(trainMat,listClasses)


import re
regEx = re.compile('\\W*')
listOfTokens = regEx.split(mySentence)

bayes.spamTest()
bayes.spamTest()

[tok for tok in listOfTokens if len(tok) > 0]

emailText = open('email/ham/6.txt').read()
listOfTokens = regEx.split(emailText)
