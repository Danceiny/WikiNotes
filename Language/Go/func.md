# 延迟函数
在函数退出前调用，后进先出。

```go:n
for i := 0; i < 5; i++ {
	defer fmt.Printf("%d ",i)
}
```
打印4 3 2 1 0.

带参数的匿名函数：
```go:n
defer func(x int){
	/*...*/
}(5)	// 为输入参数x赋值5
```


