Express提供了路由控制权转移的方法，即回调函数的第三个参数next，通过调用next()，会将路由控制权转移给后面的规则，例如：

```js
app.all('/user/:username',function(req,res,next)){
    console.log('all methods captured');
    next();
});

app.get('/user/:username',function(req,res){
    res.send('user:'+req.params.username);
});
```