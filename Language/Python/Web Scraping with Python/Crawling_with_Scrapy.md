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



# MySQL Integrating with Python
```
$ curl -L https://github.com/PyMySQL/PyMySQL/tarball/pymysql-0.6.2 | tar xz
$ cd PyMySQL-PyMySQL-f953785/
$ python setup.py
```


```Python
import pymysql
conn = pymysql.connect(host='127.0.0.1',unix_socket='/tmp/mysql.sock',user='root',passwd=None,db='mysql')
cur = conn.cursor()
cur.execute("USE scraping")
cur.execute("SELECT * FROM pages WHERE id=1")
print(cur.fetchone)
cur.close()
conn.close()
```

```python
from urllib.request import urlopen
from bs4 import BeautifulSoup
import datetime
import random
import pymysql

conn = pymysql.connect(host='127.0.0.1',uunix_socket='/tmp/mysql.sock',user='root',passwd=None,db='mysql',charset='utf8')

cur = conn.cursor()
cur.execute("USE scraping")
random.seed(datetime.datetime.now())

def store(title, content):
    cur.execute("INSERT INTO pages (title,content) VALUES (\"%s\",\"%s\")",(title,content))
    cur.connection.commit()

def getLinks(articleUrl):
    html = urlopen("http://en.wikipedia.org"+articleUrl)
    bsObj = BeautifulSoup(html)
    title = bsObj.find("span").get_text()
    content = bsObj.find("div",{"id":"mw-content-text"}).find("p").get_text()
    content = bsObj.find("div",{"id":"mw-content-text"}).find("p").get_text()
    store(title,content)
    return bsObj.find("div",{"id":"bodyContent"}).findAll("a",href=re.compile("^(/wiki/)((?!:).)*$"))
