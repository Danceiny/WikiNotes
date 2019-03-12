---
date: 2016-05-14 00:31
status: public
title: 2016-05-14-bash
---

![](~/ddddd.PNG)

![](~/11-13-42.jpg)

![](~/00-52-21.jpg)
Ctrl+r,搜索命令

管理后台进程：
* jobs
* bg
* fg


![](~/10-52-08.jpg)

![](~/11-45-56.jpg)

![](~/14-42-30.jpg)

![](~/14-50-31.jpg)

![](~/15-02-29.jpg)




后面的代码返回值非0不退出，`set +e`。 `set -e`则退出（debug时有用）

`echo -e "\ndjfa\n"` -e开启转义。



# shell脚本中切换用户

https://www.cnblogs.com/bodhitree/p/6018369.html



# 一些简单的脚本

##### first  -  A shell script that searches for files containing the word, "POSIX", and prints these files to screen.

```sh
#!/bin/sh
# first
# This file looks through all the files in the current
# directory for the string POSIX, and then prints the name of
# those files to the standard output.
for file in *
do
  if grep -q POSIX $file
  then
    echo $file
  fi
done
exit 0
```

##### variable  -  A script that demonstrates the quoting rules for variables.

```sh
#!/bin/sh

myvar="Hi there"

echo $myvar
echo "$myvar"
echo '$myvar'
echo \$myvar

echo Enter some text
read myvar

echo '$myvar' now equals $myvar

exit 0

=======================
Hi there
Hi there
$myvar
$myvar
Enter some text
new
$myvar now equals new
=======================
```



try_var  -  A script that performs some simple variable manipulation using the environment and 
parameter variables.

_if  -  A script that asks a question and then uses an "if" statement to make a decision based on the answer.

elif1  -  A modified version of _if. It reports an error message if the user types in anything other than 
"yes" or "no".

elif2  -  An improved version of elif1.

for1  -  A script that demonstrates a "for" loop.

for2  -  A script that demonstrates a wildcard expansion in a "for" loop.

for3  -  A script that prints a message 20 times using a "for" loop.

while1  -  A script that asks a user for a password until the correct word is entered.

while2  -  A less cumbersome version of for3 that uses a "while" loop.

_until  -  A script that watches for an unexpected user.

case1  -  A revised version of elif2 which is a little more selective and forgiving of unexpected input. 

case2  -  An improved version of case1.

case3  -  A reusable version of case2.

and_list  -  A script that tests for the existence of files and echoes some text in between.

or_list  -  Another script that tests for the existence of files and echoes some text in between.

function  -  A script that demonstrates a shell function.

local  -  A script that demonstrates a local variable within a shell function.

my_name  -  A script that passes a parameter to a function.

_break  -  A script that shows how to break out of a loop by using a "break" statement.

colon  -  A script that demonstrates the ":" command.

cont1, cont2  -  Two scripts that demonstrate the "continue" command.

classic, latest  -  Two scripts that demonstrate the "." command.

export1, export2  -  Two scripts that show how to export variables.

_set  -  A scipt that sets the parameter variables of the shell.

_shift  -  A script that changes the position of the shell parameters.

_trap  -  A script that traps a signal.

_unset  -  A script that removes the foo variable from the shell environment.

command.sh shows how to execute a command from within a script.

arith  -  A script that performs simple arithmetic quickly.

param  -  A script that demonstrates parameter matching operators.

here1  -  A script that feeds input to the "cat" command.

here2  -  A script that uses a here document to edit a file called "textfile".

questions - a script using a textual dialog box.

gquestions - a script using the GNOME graphical dialog box.

app/cd_db  -  A shell script that implements the CD Database application.
