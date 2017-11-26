http://songlee24.github.io/2015/07/29/mapreduce-word-count/


MapReduce采用”分而治之”的思想，把对大规模数据集的操作，分发给一个主节点管理下的各个分节点共同完成，然后通过整合各个节点的中间结果，得到最终结果。简单地说，MapReduce就是”任务的分解与结果的汇总”。