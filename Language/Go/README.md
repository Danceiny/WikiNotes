## 教程

https://github.com/astaxie/build-web-application-with-golang/blob/master/zh/preface.md

### Go命令

- go build

  - 会忽略目录下以“_”或“.”开头的go文件。

  - 如果你的源代码针对不同的操作系统需要不同的处理，那么你可以根据不同的操作系统后缀来命名文件。例如有一个读取数组的程序，它对于不同的操作系统可能有如下几个源文件：

    array_linux.go array_darwin.go array_windows.go array_freebsd.go

  - `-o` 指定输出的文件名，可以带上路径，例如 `go build -o a/b/c`

  - `-i` 安装相应的包，编译+`go install`

  - `-a` 更新全部已经是最新的包的，但是对标准包不适用

  - `-n` 把需要执行的编译命令打印出来，但是不执行，这样就可以很容易的知道底层是如何运行的

  - `-p n` 指定可以并行可运行的编译数目，默认是CPU数目

  - `-race` 开启编译的时候自动检测数据竞争的情况，目前只支持64位的机器

  - `-v` 打印出来我们正在编译的包名

  - `-work` 打印出来编译时候的临时文件夹名称，并且如果已经存在的话就不要删除

  - `-x` 打印出来执行的命令，其实就是和`-n`的结果类似，只是这个会执行

  - `-ccflags 'arg list'` 传递参数给5c, 6c, 8c 调用

  - `-compiler name` 指定相应的编译器，gccgo还是gc

- go clean

  - `-i` 清除关联的安装的包和可运行文件，也就是通过go install安装的文件
  - `-n` 把需要执行的清除命令打印出来，但是不执行，这样就可以很容易的知道底层是如何运行的
  - `-r` 循环的清除在import中引入的包
  - `-x` 打印出来执行的详细命令，其实就是`-n`打印的执行版本

- go get

  - `-d` 只下载不安装
  - `-f` 只有在你包含了`-u`参数的时候才有效，不让`-u`去验证import中的每一个都已经获取了，这对于本地fork的包特别有用
  - `-fix` 在获取源码之后先运行fix，然后再去做其他的事情
  - `-t` 同时也下载需要为运行测试所需要的包
  - `-u` 强制使用网络去更新包和它的依赖包
  - `-v` 显示执行的命令

- go install

  - 这个命令在内部实际上分成了两步操作：第一步是生成结果文件(可执行文件或者.a包)，第二步会把编译好的结果移到`$GOPATH/pkg`或者`$GOPATH/bin`。

    参数支持`go build`的编译参数。大家只要记住一个参数`-v`就好了，这个随时随地的可以查看底层的执行信息。

- go test

  - 自动读取源码目录下面名为`*_test.go`的文件，生成并运行测试用的可执行文件。

- go tool

  - `go tool fix . ` 用来修复以前老版本的代码到新版本，例如go1之前老版本的代码转化到go1,例如API的变化
  - `go tool vet directory|files`用来分析当前目录的代码是否都是正确的代码,例如是不是调用fmt.Printf里面的参数不正确，例如函数里面提前return了然后出现了无用代码之类的。

- go generate 

  - `go generate`和`go build`是完全不一样的命令，通过分析源码中特殊的注释，然后执行相应的命令。这些命令都是很明确的，没有任何的依赖在里面。而且大家在用这个之前心里面一定要有一个理念，这个`go generate`是给你用的，不是给使用你这个包的人用的，是方便你来生成一些代码的。

    这里我们来举一个简单的例子，例如我们经常会使用`yacc`来生成代码，那么我们常用这样的命令：

    ```
    go tool yacc -o gopher.go -p parser gopher.y
    ```

    -o 指定了输出的文件名， -p指定了package的名称，这是一个单独的命令，如果我们想让`go generate`来触发这个命令，那么就可以在当然目录的任意一个`xxx.go`文件里面的任意位置增加一行如下的注释：

    ```
    //go:generate go tool yacc -o gopher.go -p parser gopher.y
    ```

    这里我们注意了，`//go:generate`是没有任何空格的，这其实就是一个固定的格式，在扫描源码文件的时候就是根据这个来判断的。

    所以我们可以通过如下的命令来生成，编译，测试。如果`gopher.y`文件有修改，那么就重新执行`go generate`重新生成文件就好。

    ```
    $ go generate
    $ go build
    $ go test
    ```

### interface

### method

只能为在同一包内定义的类型的接收者声明方法， 而不能为其它包内定义的类型（包括 `int`之类的内建类型）的接收者声明方法。

### 并发 goroutine

- 信道可以带缓冲，仅当信道缓冲区填满后向其发送数据时才会阻塞，缓冲区为空时接收方会阻塞。
- 只有发送者才能关闭信道，而接收者不能。向一个已经关闭的信道发送数据会引发程序恐慌（panic）。
- 信道与文件不同，通常情况下无需关闭它们。只有在必须告诉接收者不再有值需要发送的时候才有必要关闭，例如终止一个 `range` 循环。
- `select` 会阻塞到某个分支可以继续执行为止，这时就会执行该分支。当多个分支都准备好时会随机选择一个执行。
- Sync.Mutex
  - Lock
  - Unlock
  - 在代码前调用 `Lock` 方法，在代码后调用 `Unlock` 方法来保证一段代码的互斥执行。
  - 用 `defer` 语句来保证互斥锁一定会被解锁。

### 文件操作

http://colobu.com/2016/10/12/go-file-operations/



### 构建Web应用

https://golang.org/doc/articles/wiki/

- Json http://colobu.com/2017/06/21/json-tricks-in-Go/



## 调试技巧

- 格式化输出：http://www.cnblogs.com/golove/p/3284304.html

```
fmt.Printf("%+v\n", a)
```

## 框架和库

- API 开发
  - [gin](https://gin-gonic.github.io/gin/)
- Web开发
  - [beego](https://beego.me/docs/install/bee.md)
- 包管理
  - [gopm](https://gopm.io/)
  - ​



## 有趣的开源项目

- [ModuleAB:基于beego框架开发的阿里云备份/归档解决方案](https://gocn.io/article/108)
- Go Hackthon 2017 http://gohack2017.golangfoundation.org/
- 没有对象的聊天工具：https://github.com/lifeisgo/shrimptalk
- 葫芦娃大战多线程的白骨精 - 交互式多线程Go编程教程：https://github.com/Novemser/calabashbrothersfight

## Awesome Go

https://github.com/avelino/awesome-go

## 知乎

- 书： https://www.zhihu.com/question/30461290

## 社区

- https://studygolang.com/
- https://mikespook.com/tag/golang/



https://vimeo.com/53221558



### IDE

- VSCode https://github.com/wonsikin/wonsikin.github.io/issues/2

#### 第三方工具

- [**gomodifytags**](https://github.com/fatih/gomodifytags) 的工具，根据字段名称自动填充结构体标签字段。具体实现：https://juejin.im/post/59f29894f265da43333da41e