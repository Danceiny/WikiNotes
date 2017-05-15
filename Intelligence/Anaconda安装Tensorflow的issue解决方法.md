# 目的
使用anaconda安装Tensorflow.

环境:Anaconda安装在用户目录下,py2.


# 第一个问题
问题:激活环境后,`pip install --ignore-installed --upgrade ~/Downloads/tensorflow-1.1.0-cp27-none-linux_x86_64.whl`,会报权限问题.使用sudo提升权限可以显示成功安装,但是进入python去import会显示no module,问题可能是提权后把tensorflow安装到系统自带的python里去了.`sudo -H`也并没什么卵用.


我的解决:
参考GitHub的issue回答:
```
$ conda install virtualenv # suggested by the output of terminal not using pip install
$ conda create --name=tensorflow_env python=2.7
$ source activate tensorflow_env
$ pip install --upgrade https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.5.0-cp27-none-linux_x86_64.whl # the code you provided is for the python 3
```
在一个激活的环境中(新建了一个tensorflow),
```
$ conda create --name=tensorflow_env python=2.7
$ pip install --ignore-installed --upgrade ~/Downloads/tensorflow-1.1.0-cp27-none-linux_x86_64.whl
```

和参考的回答执行顺序有所不同.

注意,我没有使用ignore-installed这个参数,pip安装打印信息也有所不同(会uninstall一些包,比如setuptools).最后成功安装并且能够import.

# 第二个问题
Cannot remove entries from nonexistent file/lib/py thon2.7/site-packages/easy-install.pth

解决: pip的时候不用upgrade参数,参看:https://github.com/tensorflow/tensorflow/issues/135