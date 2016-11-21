---
date: 2016-04-19 20:49
status: public
title: Grey-Hat-Python-黑客与逆向工程师的python编程之道
---

# book
Python灰帽子：黑客与逆向工程师的Python编程之道 英文版 by Justin Seitz
![](~/21-25-52.jpg)

This book is designed to allow you to learn some theory behind most hacking tools and techniques, including debuggers, backdoors, fuzzers, emulators, and code injection, while providing you some insight into how prebuilt Python tools can be harnessed when a custom solution isn't needed. You'll learn not only how to use Python-based tools but how to build tools in Python. But be forewarned, this is not an exhaustive reference!

All source will be posted to <http://www.nostarch.com/ghpython.htm>

# Chapter1
## Immunity Debugger
<http://debugger.immunityinc.com/>

## The Hacker's Best Friend: ctypes
## Using Dynamic Libraries

On windows platforms these binaries are called *dynamic link libraries(DLL)*, and on Linux they are called *shared objects(SO)*.
There are 3 different ways to load dynamic libraries in ctypes
1. cdll()       used for loading libraries that export functions using the standard *cdecl* calling convention
2. windll()     used for loading libraries that export functions using the *stdcall* calling convention, which is the native convention of the Microsoft Win32 API
3. oldll()      exactly like the windll() method, but assumes that the exported functions return a Windows HRESULT error code, which is used specifically for error messages returned from Microsoft Component Object Model(COM) functions.

The difference among all three is **in the way the functions inside those libraries are called and their resulting return values.**


Resolve the *printf()* function from the C runtime on both Win and Linux.On Win the C runtime is *msvcrt.dll*, located in `C:\Windows\system32\`, and on Linux it is `libc.so.6`, which is located in `/lib/` by default.

```python:n
# Windows
from ctypes import *
msvcrt = cdll.msvcrt
message_string = "hello world!\n"
msvcrt.printf("testing:%s",message_string)

# Linux
from ctypes import *
libc = CDLL("libc.so.6")
message_string = "hello world!\n"
libc.printf("testing:%s",message_string)
```



![](~/22-20-08.jpg)


**Understanding calling conventions**:
A calling convention describes how to properly call a particular function. This includes the order of how function parameters are allocated, which parameters are pushed onto the stack or passed in registers, and how the stack is unwound when a function returns. You need to understand two calling conventions: *cdecl* and *stdcall*.

In the cdecl convention, parameters are pushed from right to left, and the caller of the function is responsible for clearing the arguments from the stack. It's used by most C systems on the x86 architecture.
![](~/22-24-47.jpg)
You can see clearly see how the arguments are passed, and the last line increments the stack pointer 12 bytes (there are three parameters to the function, and each stack parameter is 4 bytes, and thus 12 bytes), which essentially clears those parameters.


And an example of the stdcall convention, which is used by the Win32 API, is shown here:
![](~/22-27-13.jpg)
You can see that the order of the parameters is the same, but the stack clearing is not done by the caller; rather the my_socks function is responsible for cleaning up before it returns.

**For both convention it's important to note that return values are stored in the EAX register.**

总结：cdecl和stdcall都是从右到左的参数压栈方式，但是cdecl还要负责清理栈中的参数，stdcall则不清理。


## Constructing C Datatypes
![](~/22-32-58.jpg)

```python:n
from ctypes import *
seitz = c_char_p("loves the python")
print seitz
print seitz.value
```
The example describes how to assign the variable seitz a character pointer to the string "loves the python". To access the contents of that pointer use the `seitz.value` method, which is called *dereferencing* a pointer.


## Passing Parameters by Reference
python指针~~~
ctypes comes fully equipped to do just that, by using the byref() function. When a function expects a pointer as a parameter, you call it like this: function_main(byref(parameter)).

# Defining Structures and Unions
You can access any of the member variables in the structure by using dot notation, like this: `beer_recipe.amt_barley`. This would access the amt_barley variable contained in the beer_recipe structure.

Define a struct:
![](~/22-53-39.jpg)


Unions are much the same as structures. However, in a union all of the member variable share the same memory location. By storing variables in this way, unions allow you to specify the same value in different types.
The next example shows a union that allows you to display a number in three different ways.

![](~/23-00-07.jpg)
```python:n
from ctypes import *
class barley_amount(Union):
    _fields_ = [
        ("barley_long",c_long),
        ("barley_int",c_int),
        ("barley_char",c_char*8),
    ]
value = raw_input("Enter the amount of barley to put into the beer vat:")
my_barley = barley_amount(int(value))
print "Barley amount as a long: %ld" % my_barley.barley_long
print "Barley amount as an int: %ld" % my_barley.barley_long
print "Barley amount as a char: %s" % my_barley.barley_char
```
