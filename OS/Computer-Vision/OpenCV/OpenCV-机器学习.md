---
date: 2016-04-16 00:37
status: public
title: OpenCV-机器学习
---

# 训练集和测试集
# 监督数据和无监督数据
有标签的是监督的。没标签的是无监督的。
# 生成模型和判别模型
OpenCV倾向于支持判别模型算法。

# OpenCV机器学习算法
Mahalanobis和K-Means在CXCORE库中，人脸检测算法在CV库中，其他都在ML库中。

|算法名称|描述|
|---|---|
|Mahalanobis|通过除以协方差来对数据空间进行变换，然后计算距离。如果协方差矩阵是单位矩阵，那么该度量等价于欧式距离|
|K-Means|非监督聚类算法，使用K个均值表示数据的分布，其中K由用户定义。该方法与期望最大化方法（expectation maximization）的区别是K均值的中心不是高斯分布，而且因为各个中心竞争“俘获”最近的点，所以聚类结果更像肥皂泡。聚类区域常用作稀疏直方图的bin，用来描述数据。|
|正态/朴素贝叶斯分类器|通用分类器，假设特征是高斯分布且统计上独立。|
|决策树|判别分类器。该决策树在当前节点通过寻找数据特征和一个阈值，来最优划分数据到不同的类别。处理流程是不停地划分数据并向下到树的左侧子节点或右侧子节点。往往是测试算法的第一选择，因为速度较快。|
|Boosting|多个判别子分类器的组合，最终的分类决策是多个子分类器的加权组合来决定。在训练时逐个训练子分类器，且每个子分类器是一个弱分类器。这些弱分类器由单变量决策树构成，称为“树桩”。在训练时，树桩从数据中学习分类决策，并根据识别精度学习“投票”的权重。逐个训练分类器时，数据样本的权重被重新分配，以达到最优。训练直到总错误低于设定的阈值。|
|随机森林|许多决策树组成，每个决策树向下递归以获得最大深度。学习过程中，每棵树的每个节点只从特征数据的一个随机子集中选择，保证了每棵树是统计上不相关的分类器。每棵树权重相同，可以对每棵树的输出进行平均，可以处理回归问题。|
|人脸检测/Haar分类器|使用boosting算法，擅长检测特定视角的刚性物体。|
|期望最大化|用于聚类的非监督生成算法，可拟合N个多维高斯到数据上。|
|K近邻|简单，速度慢，内存开销大|
|神经网络/多层传感器|训练慢，识别快，字符识别很不错|
|支持向量机SVM|可分类，可递归，需要定一个高维空间中任两点的距离函数。可以学习一个分类超平面，以实现高维空间中最优分类。数据有限时性能佳。|


# 视觉中使用机器学习

机器学习中遇到的问题以及可能的解决方法。

|问题|可能的解决方案|
|---|---|
|欠拟合|使用更多的特征有利于拟合；选用一个学习能力更好的拟合算法；|
|过拟合|增加训练数据的数量可以使拟合曲线更光滑；减少特征的数量可以降低过拟合程度|
|训练和测试效果较好，实际应用效果不好|采集更加真实的数据|
|模型无法学习数据|重新选择特征，使特征更能表达数据的不变特性；采集更新、更相关的数据；选用一个学习能力更好的拟合算法；|

## 交叉验证、自抽样法、ROC曲线和混淆矩阵
分类器准确吗？
需要用到交叉验证cross-validation和自抽样法bootstrapping。

交叉验证首先把数据分为k个不同的子集，然后用k-1个子集进行训练，用剩下的那个子集来进行测试。每个子集都有一次机会作为测试集，最后把结果平均。

自抽样法与之类似，只是验证集是从训练集中随机抽取的。选取的点仅用于测试，不用于训练。这样做N次，每次随机选择一些验证集，最后平均。这意味着一些数据样本会出现在不同的验证集中，自抽样法的效果一般胜于交叉验证。

在评估和调整分类器的方法中，还有两个比较有用的方法：
* 画ROC(receiver operating characteristic)曲线
* 填充混淆矩阵

ROC曲线评估了分类器参数的变化对分类器性能的影响。

# ML库的通用类
|基类CvStatModel中的函数|描述|
|---|---|
|save();|将训练出的模型以XML或YAML的格式保存到硬盘|
|load();|首先需要调用clear()函数，然后通过此函数装载XML或YAML格式的模型|
|clear();|释放所有内存，为重用做准备|
|bool train();|此训练函数从输入的数据集上学习一个模型，算法不同时输入参数不同|
|float predict();|训练之后，使用此函数来预测一个新数据样本的标签或者值|
|下面的是构造函数、析构函数||
|CvStatModel();CvStatModel(const CvMat* train_data...);|默认构造函数，以及将创建并训练模型功能集成的构造函数|
|CvStatModel::~CvStatModel();|机器学习模型的析构函数|
|下面的是读写支持函数（实际应用时请使用save()&load())|
|write();|通用的写CvFileStorage结构的函数，此处被save()调用|
|read();|……|


## 训练

```
bool CvStatModel::train(

);
```
机器学习只接受单通道矩阵。

输入数据矩阵的构成方式：(row,col)=(数据样本，特征变量).

* tflag = CV_ROW_SAMPLE   特征向量以行排列（默认）
* tflag = CV_COL_SAMPLE   特征向量以列排列

## 预测
```
float CvStatModel::predic(

);
```

## Mahalanobis
计算协方差矩阵。
```
void cvCalcCovarMatrix(

);
```

计算协方差矩阵的逆矩阵。
```
double cvInvert(

);
```
得到协方差矩阵的逆矩阵后就可以求两个向量的Mahalanobis距离。
与欧几里得距离相似，只是Mahalanobis距离还需要除以空间的协方差矩阵。如果协方差矩阵是单位矩阵，则Mahalanobis距离退化为Euclidean距离。
```
double cvMahalanobis(

);
```
Mahalanobis距离是多维空间中两点相似性的度量，本身不是聚类或者分类算法。

## K-Means
```
void cvKMeans2(

);
```

# 朴素贝叶斯分类
OpenCV最简单的监督学习分类器CvNormalBayesClassfier，也叫正态贝叶斯分类器或朴素贝叶斯分类器。

条件概率 = 可能性 * 先验概率 / 证据

朴素贝叶斯分类器的训练方法：
```
bool CvNormalBayesClassfier::train(

);
bool CvNormalBayesClassfier::predict(

);
```

# 二叉决策树
度量：
* entropy 熵
* Giniindex 吉尼系数
* misclassfication 错分类

- 回归不纯度
- 分类不纯度

## 决策树的使用
注意看类CvDTree{}、训练类CvDTreeTrainData{}、节点CvDTreeNode{}和分裂CvDTreeSplit{}。

### 训练树
参数结构体：CvDTreeParams{}
```
struct CvDTreeParams{
	int max_categories;//until pre-clustering
	int max_depth;//maximum levels in a tree
	int min_sample_count;//don't split a node if less
	int cv_folds;//prune tree with K fold cross-validation
	bool use_surrogates;//alternate splits for missing data
	bool use_lse_rule;//harsher pruning
	bool truncate_pruned_tree;//don't "remember" pruned branches
	float regression_accuracy;//one of the "step splitting" criteria
	const float* priors;//weight of each prediction category

	CvDTreeParams():max_categories(10),max_depth(INT_MAX),
	min_sample_count(10),cv_folds(10),use_surrogates(true),
	use_lse_rule(true),truncate_pruned_tree(true),
	regression_accuracy(0.01f),priors(NULL){;}

	CvDTreeParams(
		int _max_depth,
		int _min_sample_count,
		float _regression_accuracy,
		bool use_surrogates,
		int _cv_folds,
		bool _use_lse_rule,
		bool _truncate_pruned_tree,
		const float* _priors
	);
}
```

#### 创建并训练一个决策树
```
float priors[] = {1.0,10.0};//edible vs poisonous weights
CvMat* var_type;
var_type = cvCreateMat(data->cols+1,1,CV_8U);
cvSet(var_type,cvScalarAll(CV_VAR_CATEGORICAL));//all these vars are categorical
CvDTree* dtree;
dtree = new CvDTree;
dtree->train(
	data,
	CV_ROW_SAMPLE,
	responses,
	0,
	0,
	var_type,
	missing,
	CvDTreeParams(
	8,//max depth
	10,//min sample count
	0,//regression accuracy:N/A here
	true,//compute surrogate split, since we have missing data
	15,//max number of categories (use suboptimal algorithm for larger numbers)
	10,//cross-validations
	true,//use lSE rule=>smaller tree
	true,//throw away the pruned tree branches
	priors//the array of priors, the bigger p_weight, the more attention to the poisonous mushrooms
	)
);

dtree->save("tree.xml","MyTree");
dtree->clear();
dtree->load("tree.xml","MyTree");
```

多对象存储时不能用save，要用cvOpenFileStorage()和write()，但是load()可以提取多个对象。

决策树的预测函数：
```
CvDTreeNode* CvDTree::predict(
	const CvMat* _sample,   //需预测的浮点型的特征向量
	const CvMat* _missing_data_mask = 0,    //同样长度和同样大小的字节向量
	bool raw_mode = false   //false指出输入类别数据没有归一化，主要作用是提高预测速度
)const;
```

预测方法返回决策树的一个节点，通过`(CvDTreeNode*)->value`得到预测值：
`double r = dtree->predict(&sample,&mask)->value;`

调用`get_var_importance()`得到初始特征的重要性，该函数返回一个N*1的`CV_64FC1`向量,向量包含每个特征的重要性，1表示最重要，0表示完全不重要，不重要的特征在二次训练中可以被排除。
`const CvMat* var_importance = dtree->get_var_importance();`

每个独立特征的重要性可以如下访问：
`double val = var_importance->data.db[i];`


## boosting
boosting和随机森林在内部使用了决策树。

监督学习领域，有一个叫统计提升（meta-learning）的学习算法。

OpenCV包含4种类型的boosting：
1. CvBoost::DISCRETE(discrete AdaBoost)
2. CvBoost::REAL(real AdaBoost)
3. CvBoost::LOGIT(LogitBoost)
4. CvBoost::GENTLE(gentle AdaBoost)


### AdaBoost

### boosting代码
```
struct CvBoostParams : public CvDTreeParams{
	int boost_type;//CvBoost::DISCRETE,REAL,LOGIT,GENTLE
	int weak_count;//how many classifiers
	int split_criteria;//CvBoost::DEFAULT,GINT,MISCLASS,SQERR
	double weight_trim_rate;
	CvBoostParams();
	CvBoostParams(
		int boost_type,
		int weak_count,
		double weight_trim_rate,
		int max_depth,
		bool use_surrogates,
		const float* priors
	);
};

class CvBoostTree : public CvDTree{
	public:
		CvBoostTree();
		virtual ~CvBoostTree();
		virtual bool train(
			CvDTreeTrainData* _train_data,
			const CvMat* subsample_idx,
			CvBoost* emsemble
		);
		virtual void scale(double s);
		virtual void read(
		CvFileStorage* fs,
		CvFileNode* node,
		CvBoost* ensemble,
		CvDTreeTrainData* _data
		);
		virtual void clear();
	protected:
		...
		CvBoost* ensemble;
};
```
训练boosting分类器的代码片段：
```
var_type = cvCreateMat(var_count+2,1,CV_8U);
cvSet(var_type,cvScalarAll(CV_VAR_ORDERED));

//the last indicator variable, as well as the new (binary) response are categorical

cvSetReal1D(var_type,var_count,CV_VAR_CATEGORICAL);
cvSetReal1D(var_type,var_count+1,CV_VAR_CATEGORICAL);

//train the classifier
boost.train(
	new_data,
	CV_ROW_SAMPLE,
	responses,
	0,
	0,
	var_type,
	0,
	CvBoostParams(CvBoost::REAL,100,0.95,5,false,0)
);
cvReleaseMat(&new_data);
cvReleaseMat(&new_responses);
```


## 人脸识别和Harr分类器
训练方法createsamples()和harrtraining()和识别方法cvHaarDetectObjects()可以用于具有纹理的近似刚性的任何物体。


预先训练好的物体检测器位于`D:\Program Files\opencv\sources\data\haarcascades`,其中正面人脸识别效果最好的模型文件是`haarcascade_frontalface_alt2.xml`

OpenCV中的Haar分类器是一个监督分类器，先对图像进行直方图均衡化并归一化到同样大小，然后标记里面是否包含要检测的物体。

### 人脸检测代码
detect_and_draw()可以检测人脸，并在图中用不同颜色的矩形标示出位置。
```
//Detect and draw detected object boxes on image
//Presumes 2 Globals:
//Cascade级联 is loaded by:
//cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name,0,0,0);
//and that storage is allocated:
//CvMemStorage* storage = cvCreateMemStorage(0);
#include "stdafx.h"
void detect_and_draw(
	IplImage* img,
	double scale ) {


	/*cvNamedWindow("face",1);
	cvShowImage("face", img);*/

	/*CvMemStorage* storage=cvCreateMemStorage(0);
	char* cascade_name = "D:\\Program Files\\opencv\\sources\\data\haarcascades\\haarcascade_frontalface_alt2.xml";
	CvHaarClassifierCascade* cascade = (CvHaarClassifierCascade*)cvLoad(cascade_name, 0, 0, 0);*/

	static CvScalar colors[] = {
		{{0,0,255}},{{0,128,255}},{ { 0,255,255 } },{ { 0,255,0 } },
		{ { 255,128, } },{ { 255,255,0 } },{ { 255,0,0 } },{ { 255,0,255 } }
	};//just some pretty colors to draw with

	//Image preparation:
	IplImage* gray = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	IplImage* small_img = cvCreateImage(cvSize(cvRound(img->width / scale), cvRound(img->height / scale)), 8, 1);
	cvCvtColor(img, gray, CV_BGR2GRAY);//转成灰度图像
	cvResize(gray, small_img, CV_INTER_LINEAR);//调整大小
	cvEqualizeHist(small_img, small_img);//直方图均衡：平衡亮度值

	//detect objects if any
	cvClearMemStorage(storage);//分类器识别出的目标矩形在CvSeq序列中返回，则需先清除内存
	//实际的检测代码
	CvSeq* objects = cvHaarDetectObjects(small_img, cascade, storage, 1.1, 2, 0/*CV_HAAR_DO_CANNY_PRUNING*/, cvSize(30, 30));

	//逐个取出人脸的矩形区域，然后用不同的颜色通过函数cvRectangle()画出来
	//loop through found objects and draw boxes around them
	for (int i = 0; i < (objects ? objects->total : 0); i++) {
		CvRect* r = (CvRect*)cvGetSeqElem(objects, i);
		cvRectangle(img, cvPoint(r->x, r->y), cvPoint(r->x + r->width, r->y + r->height), colors[i % 8]);
	}
	cvReleaseImage(&gray);
	cvReleaseImage(&small_img);
	cvDestroyAllWindows();
}


//CvSeq* cvHaarDetectObjects(
//	const CvArr* image,
//	CvHaarClassifierCascade* cascade,	//分类器cascade是通过cvLoad()加载的Haar特征级联
//	CvMemStorage* storage,		//工作缓存
//	double scale_factor CV_DEFAULT(1.1),	//决定每两个不同大小的窗口之间有多大的跳跃，参数值较大则计算变快，也就可能丢失物体
//	int min_neighbors CV_DEFAULT(3),	//控制误检测，默认值3表明至少有3次重叠检测，我们才认为人脸确实存在
//	int flags CV_DEFAULT(0),	//有4个待选数值
//	CvSize min_size CV_DEFAULT(cvSize(0, 0)),	//指示寻找人脸的最小区域
//	CvSize max_size CV_DEFAULT(cvSize(0, 0))
//	);


//flag:
//1. CV_HAAR_DO_CANNY_PRUNING, 分类器将跳过平滑（无边缘区域）
//2. CV_HAAR_SCALE_IMAGE，分类器将不缩放分类器，而是缩放图像（处理内存和缓存的使用问题，以提高性能）
//3. CV_HAAR_FIND_BIGGEST_OBJECTS，分类器只返回最大的目标（返回物体个数为1或0）
//4. CV_HAAR_DO_ROUGH_SEARCH，只可与CV_HAAR_FIND_BIGGEST_OBJECTS一起使用，告诉分类器在任何窗口，只要第一个候选者被发现即结束寻找。
```


## 学习新的物体

![](~/00-09-09.jpg)

训练一个分类器的4个步骤：（参考文档opencv/apps/HaarTraining/doc/haartraining）
1. 收集打算学习的物体的数据集（如正面人脸图，汽车侧面图），并存储在一个或多个目录下面。这些文件通过一个文本文件以如下形式建立索引：

![](~/00-11-18.jpg)

文本文档的每一行包含路径和文件名，紧接着物体的个数和物体的矩形列表，矩形的格式是(左上角的x和y坐标，宽度，高度)。

![](~/00-12-57.jpg)

2. 使用辅助工具createsamples来建立正样本的向量输出文件。通过这个文件即可重复训练过程，使用同一个向量输出文件尝试各种参数。例如：
`createsamples -vec face.vec -info face.idx -w 30 -h 40`
该程序读入第一步所说的face.idx文件，输出一个格式化的训练文件face.vec，然后createsamples提取图像中的正样本，再归一化并调整到指定的大小（此处为30x40）。createsamples还可以对数据进行几何变换、增加噪声、改变颜色等。

3. Viola-Jones级联是一个两类分类器：只判断图像中的物体是否与训练集相似。
反样本：从视频的不包含人脸的帧中获得反样本等。把反样本图像放到一个或者几个路径下面，并由图像文件名组成索引文件，一个文件名占一行。例如一个叫background.idx的图像索引文件可以包含下面的路径和文件名：

![](~/00-40-39.jpg)

4. 训练。
命令行输入或者批处理文件创建的训练：

![](~/00-41-26.jpg)



# 其他机器学习算法
参考`.../opencv/docs/ref/openref_ml.htm`
## 期望值最大化
expectation maximization，EM。聚类技术。OpenCV仅支持混合高斯模型的EM，在类CvEM{}中。
EM算法通过迭代先找到给定模型时的最大可能性的猜想，然后调整模型使猜想正确率最大化。

## K近邻
K-nearest neighbor,KNN

算法实现在类CvKNeast{}中
## 多层感知器
MLP，也叫反向传播，在文字识别方面性能卓越。

算法实现在类CvANN_MLP{}中。用法参见文件`.../opencv/samples/c/leter_recog.cpp`

论文LeCun,Botton,Bengio和Haffner[LeCun98a]，MLP在文字识别和物体识别方面的效能；
论文LeCun，Botton和Muller[LeCun98b]，介绍了实现和调节细节。
## 支持向量机
算法实现在类CvSVM{}中。
