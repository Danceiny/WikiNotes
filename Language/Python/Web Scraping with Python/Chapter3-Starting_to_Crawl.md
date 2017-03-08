```python
# -*- coding: utf-8 -*-
"""
Created on Wed Feb  8 12:10:46 2017

@author: huangzhen
"""


from urllib.request import urlopen
from bs4 import BeautifulSoup
import re
import random
import datetime

#html = urlopen("http://www.pythonscraping.com/pages/warandpeace.html")
#html = urlopen("http://www.pythonscraping.com/pages/page3.html")
#html = urlopen("http://en.wikipedia.org/wiki/Kevin_Bacon")
#bsObj = BeautifulSoup(html)

#nameList = bsObj.findAll("span",{"class":"green"})
#for name in nameList:
#	print(name.get_text())


#for sibling in bsObj.find("table",{"id":"giftList"}).tr.next_siblings:
#    print(sibling)

#print(bsObj.find("img",{"src":"../img/gifts/img1.jpg"}).parent.previous_sibling.get_text())

#for link in bsObj.findAll("a"):
#    if 'href' in link.attrs:
#        print(link.attrs['href'])

#for link in bsObj.find("div",{"id":"bodyContent"}).findAll("a",href=re.compile("^(/wiki/)((?!:).)*$")):
#    if 'href' in link.attrs:
#        print(link.attrs['href'])


random.seed(datetime.datetime.now())
pages=set()
#only links that are 'new' should be crawled and searched for additional links
def getLinks(pageUrl):
    global pages
    html = urlopen("http://en.wikipedia.org"+pageUrl)
    bsObj = BeautifulSoup(html,"lxml")
    for link in bsObj.findAll("a",href=re.compile("^(/wiki/)")):
        if 'href' in link.attrs:
            if link.attrs['href'] not in pages:
                # encountered a new page
                newPage = link.attrs['href']
                print(newPage)
                pages.add(newPage)
                getLinks(newPage)
    #return bsObj.find("div",{"id":"bodyContent"}).findAll("a",href=re.compile("^(/wiki/)((?!:).)*$"))
getLinks("")
#links = getLinks("/wiki/Kevin_Bacon")
#while len(links)>0:
#    newArticle = links[random.randint(0,len(links)-1)].attrs["href"]
#    print(newArticle)
#    links = getLinks(newArticle)
```


# Crawling an Entire Site
