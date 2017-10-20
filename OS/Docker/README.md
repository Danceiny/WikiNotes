https://yeasy.gitbooks.io/docker_practice/content/index.html


## 三个基本概念
- 镜像 Image
- 容器 Container
- 仓库 Repository
仓库和Git类似。

docker pull registry.hub.docker.com/ubuntu:12.04
sudo docker pull dl.dockerpool.com:5000/ubuntu:12.04


列出镜像：docker images
启动容器：docker run -t ubuntu /bin/bash
(-t启动容器后无法输入？还需要-i参数！)

容器的ID。

在容器内进行操作后，可以`$ sudo docker commit -m "Added json gem" -a "Docker Newbee" 0b2616b0e5a8 ouruser/sinatra:v2`

利用Dockerfile来创建镜像。`docker build`

利用 docker run 来创建容器时，Docker 在后台运行的标准操作包括：

• 检查本地是否存在指定的镜像，不存在就从公有仓库下载
• 利用镜像创建并启动一个容器
• 分配一个文件系统，并在只读的镜像层外面挂载一层可读写层
• 从宿主主机配置的网桥接口中桥接一个虚拟接口到容器中去
• 从地址池配置一个 ip 地址给容器
• 执行用户指定的应用程序
• 执行完毕后容器被终止

更多的时候，需要让 Docker 容器在后台以守护态(Daemonized)形式运行。此时，可以通过添加 -d 参数 来实现。

sudo docker export 7691a814370e > ubuntu.tar

cat ubuntu.tar | sudo docker import - test/ubuntu:v1.0 #快照

用户既可以使用 docker load 来导入镜像存储文件到本地镜像库，也可以使用 docker import 来导入一 个容器快照到本地镜像库。这两者的区别在于容器快照文件将丢弃所有的历史记录和元数据信息(即仅保存容器 当时的快照状态)，而镜像存储文件将保存完整记录，体积也要大。此外，从容器快照文件导入时可以重新指定 标签等元数据信息。


## 数据卷
数据卷是一个可供一个或多个容器使用的特殊目录，它绕过 UFS，可以提供很多有用的特性:
• 数据卷可以在容器之间共享和重用
• 对数据卷的修改会立马生效
• 对数据卷的更新，不会影响镜像
• 卷会一直存在，直到没有容器使用 *数据卷的使用，类似于 Linux 下对目录或文件进行 mount。
