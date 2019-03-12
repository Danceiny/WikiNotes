# 列表命令
- lpush/rpush mylist "cannot.cc"
- lrange mylist 0 10
- blpop/brpop key1 [key2] timeout
- lpop/rpop key

命令是b开头代表block阻塞。