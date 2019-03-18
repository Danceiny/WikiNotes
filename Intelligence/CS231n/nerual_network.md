1. demo http://cs.stanford.edu/people/karpathy/convnetjs/demo/classify2d.html


# 线性整流函数 Rectified Linear Unit, ReLU
[维基百科](https://www.wikiwand.com/zh-hans/%E7%BA%BF%E6%80%A7%E6%95%B4%E6%B5%81%E5%87%BD%E6%95%B0)

是一种激活函数。

* [知乎：激活函数的作用具体是什么？为什么效果比sigmoid好？](https://www.zhihu.com/question/29021768)
    1. 作用
    >是为了增加神经网络模型的非线性。否则你想想，没有激活函数的每层都相当于矩阵相乘。就算你叠加了若干层之后，无非还是个矩阵相乘罢了。所以你没有非线性结构的话，根本就算不上什么神经网络。
    2. 效果显著
    >发现ReLU更容易学习优化。因为其分段线性性质，导致其前传，后传，求导都是分段线性。而传统的sigmoid函数，由于两端饱和，在传播过程中容易丢弃信息：
    >缺点是不能用Gradient-Based方法。同时如果de-active了，容易无法再次active。不过有办法解决，使用maxout激活函数：
    
* [Stackexchange: What are the advantages of ReLU over sigmoid function in deep neural network?](http://stats.stackexchange.com/questions/126238/what-are-the-advantages-of-relu-over-sigmoid-function-in-deep-neural-network)