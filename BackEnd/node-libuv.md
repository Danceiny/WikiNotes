

# OS X
安装：
## 通过autotools编译
克隆仓库。
`git clone https://github.com/libuv/libuv`

安装autotools。

$ brew install autoconf
$ brew install automake
$ brew install libtool
编译。
$ sh autogen.sh
$ ./configure
$ make
$ make check
$ make install

## 测试
测试代码main.c
```c
#include <stdio.h>
#include <uv.h>
int main() {
    uv_loop_t *loop = uv_loop_new();
    printf(“Now quitting.\n”);
    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}
```
`gcc -o app main.c /usr/local/lib/libuv.a -framework CoreFoundation -framework CoreServices`
# issues
- OSX下的编译问题：https://my.oschina.net/sunshinedabby/blog/336460
