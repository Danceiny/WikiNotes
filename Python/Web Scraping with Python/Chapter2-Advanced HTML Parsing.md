---
date: 2016-02-29 18:09
status: public
title: 'Chapter2-Advanced HTML Parsing'
---

1. the following two lines are identical:
* `bsObj.findAll(id="text")`
* `bsObj.findAll("",{"id":"text"})`

但是class关键字不可用，不过又可以加个_，`bsObj.findAll(class_="green")`

To create a new Scrapy project in the current directory, run from the command line:
`scrapy startproject wikiSpider`

![](~/15-17-07.jpg)
file structure:
* scrapy.cfg
* wikiSpider
    * __init__.py
    * items.py
    * pipelines.py
    * settings.py
    * spiders
        * __init__.py