---
date: 2016-02-29 18:09
status: public
title: 'Chapter2-Advanced HTML Parsing'
---

1. the following two lines are identical:
* `bsObj.findAll(id="text")`
* `bsObj.findAll("",{"id":"text"})`

但是class关键字不可用，不过又可以加个_，`bsObj.findAll(class_="green")`
