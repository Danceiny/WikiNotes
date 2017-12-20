## 缘由

不用Jython也行！

Another issue of the Jython approach is the overhead of writing your Python program in such a way that it can interact with Hadoop – just have a look at the example in `$HADOOP_HOME/src/examples/python/WordCount.py` and you see what I mean.



## 想做什么

用Python写一个wordcount，不用Jython转成Java jar files。



## 准备

一个Hadoop集群（单点或者多点）。[Ubuntu多点Hadoop集群](http://www.michael-noll.com/tutorials/running-hadoop-on-ubuntu-linux-multi-node-cluster/)。

