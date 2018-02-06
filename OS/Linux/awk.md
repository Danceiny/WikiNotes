http://www.cnblogs.com/chengmo/archive/2010/10/03/1841753.html

1、获得普通外部变量

[chengmo@localhost ~]$ test='awk code'                            
[chengmo@localhost ~]$ echo | awk  '{print test}' test="$test"
awk code
[chengmo@localhost ~]$ echo | awk  test="$test" '{print test}' 
awk: cmd. line:1: fatal: cannot open file `{print test}' for reading (No such file or directory)

 

格式如：awk ‘{action}’  变量名=变量值   ，这样传入变量，可以在action中获得值。 注意：变量名与值放到’{action}’后面。

[chengmo@localhost ~]$ echo | awk  'BEGIN{print test}' test="$test"         

 

这种变量在：BEGIN的action不能获得。

2.BEGIN程序块中变量

[chengmo@localhost ~]$ test='awk code'                                 
[chengmo@localhost ~]$ echo | awk -v test="$test" 'BEGIN{print test}'
awk code
[chengmo@localhost ~]$ echo | awk -v test="$test" '{print test}'     
awk code

 

格式如：awk –v 变量名=变量值 [–v 变量2=值2 …] 'BEGIN{action}’  注意：用-v 传入变量可以在3中类型的action 中都可以获得到，但顺序在  action前面。

  

3.获得环境变量

[chengmo@localhost ~]$ awk  'BEGIN{for (i in ENVIRON) {print i"="ENVIRON[i];}}'
AWKPATH=.:/usr/share/awk
SSH_ASKPASS=/usr/libexec/openssh/gnome-ssh-askpass
SELINUX_LEVEL_REQUESTED=
SELINUX_ROLE_REQUESTED=
LANG=en_US.UTF-8
.......

 

只需要调用：awk内置变量 ENVIRON,就可以直接获得环境变量。它是一个字典数组。环境变量名 就是它的键值。