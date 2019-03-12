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


