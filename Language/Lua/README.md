a and b  -- a为false，则返回a，否则返回b
a or b    -- a为true，则返回a，否则返回b

优先级：and > or

C:     a?b:c
Lua:    (a and b) or c    -- if a is false, then [a or c], then return b;
                                    -- if a is true, then [b or c], then ????


会话模式（You may use the -i option to instruct Lua to start an interactive session after
running the given chunk.）：
$ lua -i prog
will run the chunk in file prog and then prompt you for interaction.

The -e option allows us to enter code directly into the command line (execute)：
$ lua -e "print(math.sin(12))" 

-l option loads a library.

# Stand-alone interpreter
Before running its arguments, lua looks for an environment variable named
LUA_INIT. If there is such a variable and its content is @filename, then lua runs
the given file. If LUA_INIT is defined but does not start with ‘@’, then lua assumes
that it contains Lua code and runs it. LUA_INIT gives us great power when
configuring the stand-alone interpreter, because we have the full power of Lua
in the configuration. We can pre-load packages, change the prompt and the path,
define our own functions, rename or delete functions, and so on.

# Arguments table
$ lua -e "sin=math.sin" script.lua a b
lua collects the arguments as follows:
arg[-3] = "lua"
arg[-2] = "-e"
arg[-1] = "sin=math.sin"
arg[0] = "script"
arg[1] = "a"
arg[2] = "b"


long string [[ ... ]], like ''' ... '''' in python.
but what the fuck when a=b[c[i]]??? 
modify [[ to [===[, and ]] to ]===]


operator '#' returns the last index (or the size) of an array or list.
-- print the lines
for i = 1, #a do
    print(a[i])
end

a = {}
a[1000]=1
print(table.max(a))         --> 1000

but, print(#a)    --> 0 ???


# SORT排序
network = {
    { name = "..", IP="..."},
    { name = "..", IP="..."},
    ...
}

table.sort(network,function(a,b) return (a.name>b.name)end)


# Table
a = { [F(1)] = g; "x", "y"; x = 1, f(x), [30] = 23; 45}

is equivalent to 
do
	local temp = {}
	temp[f(1)] = g
	temp[1] = "x"	-- 1st exp
	temp[2] = "y"	-- 2nd exp
	temp.x = 1 		-- temp["x"] = 1
	temp[3] = f(x)	-- 3rd exp
	temp[30] = 23
	temp[4] = 45	-- 4th exp  ???
	a = temp
end

# Function Calls
If the function is calledinside another expression or in the middle of a list of expressions, then its return list is adjusted
to one element, thus discarding all returned values except the first one.
例如：g(f(), x) -- f() is adjusted to 1 result

If you enclose a function call in parentheses, then it is adjusted to return exactly one value:
return x,y,(f()) -- returns x, y, and the first value from f()
{(f())} -- creates a table with exactly one element


# Metatable
Metatables control the operations listed next. Each operation is identified by its corresponding
name. The key for each operation is a string with its name prefixed by two underscores; for
instance, the key for operation “add” is the string "__add". The semantics of these operations is
better explained by a Lua function describing how the interpreter executes that operation.
The code shown here in Lua is only illustrative; the real behavior is hard coded in the interpreter
and it is much more efficient than this simulation. All functions used in these descriptions (rawget,
tonumber, etc.) are described in §5.1. In particular, to retrieve the metamethod of a given object,
we use the expression
metatable(obj)[event]
This should be read as
rawget(metatable(obj) or {}, event)
function getbinhandler (op1, op2, event)
	return metatable(op1)[event] or metatable(op2)[event]
end
function add_event (op1, op2)
	local o1, o2 = tonumber(op1), tonumber(op2)
	if o1 and o2 then -- both operands are numeric?
		return o1 + o2 -- ‘+’ here is the primitive ‘add’
	else -- at least one of the operands is not numeric
		local h = getbinhandler(op1, op2, "__add")
		if h then
		-- call the handler with both operands
			return h(op1, op2)
		else -- no handler available: default behavior
			error("...")
		end
	end
end

# Coroutines
coroutine.create返回一个线程类型的句柄，但并不执行；coroutine.yield执行（生成？），yield执行时，相关的resume立马return。create返回的句柄作为conroutine.resume的第一个参数。resume返回值：true，和传入yield的参数；false，和错误消息。yield返回值：调用resume时额外穿进去的参数。理解：第一次resume时即传入给create的function的参数，在function中被yield打断，下次resume，从被打断的点开始继续执行，这次的resume又传入了一些参数，这些参数就由上次打断的yield返回。比如以下代码中第8行的local r = foo1(a+1)，调用foo1时，引发yield，回到主程序打印“main”,a,b（第17行）,然后在主程序中resume（第18行），此次resume传入了"r"。resume后回到create的function中的第8行执行（第8行有调用，再往下，上次在foo1的yield后中断，此次执行yield的下一条指令：return，于是return了此次resume新传入的参数"r"，于是x = "r"。

function foo1(a)
	print("foo",a)
	return coroutine.yield(2*a)
end
co = coroutine.create(function (a,b)
	print("co-body",a,b)
	local x = foo1(a+1)
	print("co-body",x)
	local r,s = coroutine.yield(a+b,a-b)
	print("co-body",r,s)
	return b,"end"
end)
a,b = coroutine.resume(co,1,10)
print("main",a,b)
a,b,c = coroutine.resume(co,"r")
print("main",a,b,c)
a,b,c = coroutine.resume(co,"x","y")
print("main",a,b,c)
a,b = coroutine.resume(co,"x","y")
print("main",a,b)

Lua supports coroutines, also called semi-coroutines or collaborative multithreading. A coroutine in Lua represents an independent thread of execution. Unlike threads in multithread systems,
however, a coroutine only suspends its execution by explicitly calling a yield function.
You create a coroutine with a call to coroutine.create. Its sole argument is a function that is the main function of the coroutine. The create function only creates a new coroutine and returns a handle to it (an object of type thread); it does not start the coroutine execution.
When you first call coroutine.resume, passing as its first argument the thread returned by
coroutine.create, the coroutine starts its execution, at the first line of its main function. Extra arguments passed to coroutine.resume are given as parameters for the coroutine main function.
After the coroutine starts running, it runs until it terminates or yields.
A coroutine can terminate its execution in two ways: Normally, when its main function returns
(explicitly or implicitly, after the last instruction); and abnormally, if there is an unprotected error.
In the first case, coroutine.resume returns true, plus any values returned by the coroutine main
function. In case of errors, coroutine.resume returns false plus an error message.
A coroutine yields by calling coroutine.yield. When a coroutine yields, the corresponding coroutine.resume returns immediately, even if the yield happens inside nested function calls (that is, not in the main function, but in a function directly or indirectly called by the main function). In the case of a yield, coroutine.resume also returns true, plus any values passed to coroutine.yield. The next time you resume the same coroutine, it continues its execution from the point where it yielded, with the call to coroutine.yield returning any extra arguments passed to coroutine.resume.
The coroutine.wrap function creates a coroutine like coroutine.create, but instead of returning the coroutine itself, it returns a function that, when called, resumes the coroutine. Any arguments passed to that function go as extra arguments to resume. The function returns all the
values returned by resume, except the first one (the boolean error code). Unlike coroutine.resume,
this function does not catch errors; any error is propagated to the caller.

book: ReadingLua
1. 阅读外围的库是如何实现功能扩展的，这样可以熟悉Lua公开API，不必陷入功能细节；
2. 阅读API的具体实现。
3. 开始了解Lua VM的实现。
4. 理解函数调用、返回，string，table，metatable等如何实现。
5. debug模块。
6. parser等编译相关部分。
7. 垃圾收集是最难的部分。


Lua 5.1.4的gc部分细致剖析：
http://blog.codingnow.com/2011/04/lua_gc_multithreading.html