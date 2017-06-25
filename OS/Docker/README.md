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