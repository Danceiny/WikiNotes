# 基本概念解读

## Service (对象)

### 1. name-id

### 2.endpoint（ 虚拟ip（cluster ip, service ip 或 vip）和端口号）

### 3. 提供某种远程服务的能力

### 4. 被映射到了提供这种服务能力的一组容器应用上 



## Pod

“把每个服务进程包装到相应的Pod中。”

pod和service的关联：给pod贴标签，name=*label*；给service定义标签选择器（label selector）。

1. pod运行在node（节点）上，通常一个节点运行几百个pod。
2. 每个pod里运行着一个特殊的叫Pause的容器，其他容器叫业务容器，这些业务容器共享Pause的网络栈和Volume挂载卷。



集群管理方面，master & worker-node。master上有kube-apiserver等进程，node上有kubelet、kube-proxy等服务进程，这些进程负责pod的创建启动销毁等。





## Replication Controller

对应一个pod，负责该pod所关联的service的升级和扩容。

一个RC定义文件中包括：

- 目标Pod的定义
- 目标Pod所要运行的副本数（Replicas）
- 要监控的目标Pod的Label

