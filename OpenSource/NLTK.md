# 加载自己的语料库
>>> from nltk.corpus import PlaintextCorpusReader
>>> corpus_root = '/tmp'
>>> wordlists = PlaintextCorpusReader(corpus_root, '.*')
>>> wordlists.fileids()

就可以列出自己语料库的各个文件了，也可以使用如wordlists.sents('a.txt')和wordlists.words('a.txt')等方法来获取句子和词信息

# 文档分类
文档分类
不管是什么分类，最重要的是要知道哪些特征是最能反映这个分类的特点，也就是特征选取。文档分类使用的特征就是最能代表这个分类的词。

因为对文档分类要经过训练和预测两个过程，而特征的提取是这两个过程都需要的，所以，习惯上我们会把特征提取单独抽象出来作为一个公共方法，比如：
```python
from nltk.corpus import movie_reviews
all_words = nltk.FreqDist(w.lower() for w in movie_reviews.words())
word_features = all_words.keys()[:2000]
def document_features(document): 
	for word in word_features: 
		features['contains(%s)' % word] = (word in document_words) 
	return features 
```

这是一个简单的特征提取过程，前两行找到movie_reviews语料库中出现词频最高的2000个词作为特征，下面定义的函数就是特征提取函数，每个特征都是形如contains(***)的key，value就是True或False，表示这个词是否在文档中出现

那么我们训练的过程就是：

featuresets = [(document_features(d), c) for (d,c) in documents]
classifier = nltk.NaiveBayesClassifier.train(featuresets)
 

要预测一个新的文档时：

classifier.classify(document_features(d))
 

通过

classifier.show_most_informative_features(5)
可以找到最优信息量的特征，这对我们选取特征是非常有帮助的。