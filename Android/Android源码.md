1. 源码管理和编译工具: `sudo apt-get install git-core gnupg sun-java5-jdk flex bison gperf libsdl-dev libesd0-dev libwxgtk2. 6-dev build-essential zip curl libncurses5-dev zlib1g-dev`

2. [Android开源项目网址](http://source.android.com)

```
$cd～
$mkdir bin
$curl http：//android.git.kernel.org/repo＞～/bin/repo
$chmod a+x～/bin/repo
$mkdir MY_ANDROID
$cd MY_ANDROID
$repo init-u git：//android.git.kernel.org/platform/manifest.git ##同步最新
$repo init-u git：//android.git.kernel.org/platform/manifest.git-b cupcake   ##同步cupcake分支

$repo sync
```

通过上述方法获取到的源代码，包含了Android应用层、框架层、运行时和底层类库的所有代码，可以用来编译出运行在模拟器上的Android ROM，以及相关的开发工具。但是，这份源代码并不能编译出可以在真实设备上运行的ROM，因为还缺少了Linux操作系统层和硬件抽象层相关的代码。

run-java-tool抛出，要`$export ANDROID_JAVA_HOME=$JAVA_HOME`.

编译指定的产品：
```
$cd$MY_ANDROID
$.build/envsetup.sh
$make-j4 THE_PRODUCT-generic-user
```


# Android源代码结构
frameworks有三个重要的子目录，分别是base、opt、polices。


# Android组件模型解析
```]`+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++`