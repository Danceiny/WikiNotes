# Mac下伪分布式环境安装
http://cdn2.jianshu.io/p/bca08edaf0c8
https://segmentfault.com/a/1190000009103629


brew install hadoop

export HADOOP_HOME=/usr/local/Cellar/hadoop/2.8.1/libexec

ls -lad /usr/local /usr/local/Cellar


配置文件
${HADOOP_HOME}/etc/hadoop/
1. core-site.xml, NameNode的主机名和端口号
2. hdfs-site.xml, 默认参数副本数（等于节点数）
3. mapred-site.xml, 指定了JobTracker的主机名与端口(也可配置为Yarn)

hadoop namenode -format

start-all.sh

jps


hdfs dfs -mkdir /demo-test

hdfs dfs -ls /

hdfs dfs -put wcl.txt /demo-test

DFS: http://hadoop.cannot.cc:50070/dfshealth.html#tab-overview
Yarn: http://hadoop.cannot.cc:8088/cluster