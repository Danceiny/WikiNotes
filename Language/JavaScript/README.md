---
date: 2016-07-22 15:00
status: public
title: 2016-07-22
---

JavaScript中没有块级作用域，语句块中声明的变量并非语句块私有。

with语句用于临时扩展作用域链。`with(object)statement`，该语句将object添加到作用域链的**头部**，然后执行statement， 最后恢复作用域链到原始状态。
**在对象嵌套层次很深的时候通常会使用with语句来简化代码编写。**


# apply && call
call和apply的功能是一致的，两者细微的差别在于call以参数表来接受被调用函数的参数，而apply以数组来接受被调用函数的参数。

```js
var someuser = {
    name: 'byvoid',
    display: function(words){
        console.log(this.name + 'says' + words);
    }
}; 
var foo = {
    name: 'foobar'
};
someuser.display.call(foo, 'hello');    //输出 foobar says hello
```

**bind**:绑定上下文、参数列表。
两次bind是没有效果的。

# prototype
```js
function Foo(){
    var innverVar = 'hello';
    this.prop1 = 'BYVoid';
    this.func1 = function(){
        innverVar = '';
    };
}
Foo.prototype.prop2 = 'Carbo';
Foo.prototype.func2 = function(){
    console.log(this.prop2);
};
var foo1 = new Foo();
var foo2 = new Foo();
console.log(foo1.func1 == foo2.func1);  //false
console.log(foo1.func2 == foo2.func2);  //true
```