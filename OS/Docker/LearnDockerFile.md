
# DockerFile
FROM nginx  #FROM 指定基础镜像
RUN echo '<h1>Hello, Docker!</h1>' > /usr/share/nginx/html/index.html

# 构建
在当前目录下有Dockerfile-py2的dockerfile，
`docker build -t py2 . -f ./Dockerfile-py2`