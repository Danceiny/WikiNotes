# Intro
书中相关环境:
* go version
go1.5.1 linux/amd64

我是go1.7.3
* lsb_release -d
Description:    Ubuntu 14.04.3 LTS

我是14.04.3

* gdb --version
GNU gdb (Ubuntu 7.7.1-0ubuntu5~14.04.2) 7.7.1

我是GNU gdb (Ubuntu 7.11.1-0ubuntu1~16.04) 7.11.1

书中示例安装包存放在/usr/local/go,我放在/usr/local/share/go.

基本相同.

# 引导

-gcflags "-N -l" 参数关闭编译器代码优化和函数 内联，避免断点和单步执行无法准确对应源码行，避免小函数和局部变量被优化掉。 