---
date: 2016-05-27 12:32
status: public
title: js-object
---

```
function Point(x,y){//construct function CAPITAL
  this.x = x;
  this.y = y;
[lenders.html](/Javascript/_attachment/js-object/lenders.html)  }

var p = new Point(1,1);

//Key:prototype
Point.prototype.r = function(){
  return Math.sqrt(this.x*this.x+this.y*this.y);
}

p.r(); //=>1.414
```