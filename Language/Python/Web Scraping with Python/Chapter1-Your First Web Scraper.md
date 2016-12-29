---
date: 2016-02-28 16:02
status: public
title: 'Chapter1-Your First Web Scraper'
---

# Connecting
>Web scraping requires stripping away some of this shroud of interface, not just at the browser lever(how it interprets all of this HTML, CSS, and JavaScript), but occasionally at the level of the network connection.

>To give you some idea of the infrastructure required to get information to your browser, let's use the following example. Alice owns a web server. Bob uses a desktop computer, which is trying to connect to Alice's server. When one machine wants to talk to another machine, something like the following exchange takes place:
>
1. Bob's computer sends along a stream of 1 and 0 bits, indicated by high and low voltages on a wire. These bits from some information, containing a header and body. The header contains an immediate destination of his local router's MAC address, with a final destination of Alice's IP address. The body contains his request for Alice's server application.
2. Bob's local router receives all these 1's and 0's and interprets them as a packet, from Bob's own MAC address, and destined for Alice's IP address. His router stamps its own IP address on the packet as the "from" IP address, and sends it off across the Internet.
3. Bob's packet traverses several intermedidary serves, which direct his packet toward the correct physical/wired path, on to Alice's server.
4. Alice's server receives the packet, at her IP address.
5. Alice's server reads the packet port destination (almost always port 80 for web applications, this can be thought of as somthing like an "apartment number" for packet data, where the IP address is the "street address"), in the header, and passes it off to the appropriate application - the web server application.
6. The web server application receives a stream of data from the server processor. This data says something like:
	- This is a GET request
	- The following file is a requested: index.html
7. The web server locates the correct HTML file, bundles it up into a new packet to send to Bob, and sends it through to its local router, for transport back to Bob's machine, through the same process.

**urllib or urllib2?**
>If you've used the urllib2 library in python2.x, you might have noticed that things have changed somewhat between urllib2 and urllib. In python3.x, urllib2 was renamed urllib and was split into several submodules: urllib.request, urllib.parse, and urllib.error. Although function names mostly remain the same, you might want to note which functions have moved to submodules when using the new urllib.

`from urllib.request import urlopen`

urlopen is used to open a remote object across a network and read it. Because it is a fairly generic library(it can read HTML files, or any other file stream with ease),

>we will be using it quite frequently throughout the book.


# An Introduction to BeautifulSoup
BeautifulSoup tries to make sense of the nonsensical; it helps format and organize the messy web by fixing bad HTML and presenting us with easily-traversible Python objects representing XML structures.


`sudo apt-get install python3-bs4`
所以是	`import bs4`。不管是py2还是py3。py2还可以`import BeautifulSoup`。
或者说呢，是`from bs4 import BeautifulSoup`。
就是BeautifulSoup4啦。

**KEEPING LIBRARY STRAIGHT WITH VIRTUAL ENVIRONMENTS**
If you intend to work on multiple Python projects or you need a way to easily bundle projects with all associated libraries, or you're worried about potential conflicts between installed libraries, you can install a Python virtual environments to keep everything separated and easy to manage.

When you install a Python library without a virtual environment, you are installing it **globally**. This usually requires that you be an administrator, or run as root, and that Python library exists for every user and every project on the machine.Fortunately, creating a virtual environment is easy:

`$ virtualenv scrapingEnv`

This creates a new environment called *scrapingEnv*, which you must activate in order to use:

`$ cd scrapingEnv/`

`$ source bin/activate`

After you have activated the environment, you will see environment's name in your command line prompt, reminding you that you're currently working with it. Any libraries you install or scripts you run will be under that virtual environment only.

Working in the newly-created scrapingEnv environment, I can install and use BeautifulSoup.

I can leave the environment with the deactivate command, after which I can no longer access any libraries that were installed inside the virtual environment.

Keeping all your libraries separated by project also makes it easy to zip up the entire environment folder and send it to someone else. As long as they have the same version of Python installed on their machine, your code will work from the virtual environment without requiring them to install any libraries themselves.

(我记得Anaconda的虚拟环境管理非常牛逼的样子)
# Running BeautifulSoup
```python:n
from urllib.request import urlopen
from bs4 import BeautifulSoup
html = urlopen("http://www.pythonscraping.com/exercises/exercise1.html")
bsObj = BeautifulSoup(html.read())
print(bsObj.h1)
```
The output is:
`<h1>An Interesting Title</h1>`

The HTML content is transformed into  a BeautifulSoup object, with the following structure:
- html
- head
	- title
- body
	- h1
	- div

the page source code:
```html:n

<html>
<head>
<title>A Useful Page</title>
</head>
<body>
<h1>An Interesting Title</h1>
<div>
Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
</div>
</body>
</html>
```

Note that the <h1> tag that we extracted from the page was nested two layers deep into our BeautifulSoup object structure (html-body-h1). However, when we actually fetched it from the object, we called the h1 tag directly:

`bsObj.h1`

In fact, any of the following function calls would produce the same output:

`bsObj.html.body.h1`

`bsObj.body.h1`

`bsObj.html.h1`

# Connecting Reliably
The web is messy. Data is poorly formatted, websites go down, and closing tags go missing.

Let's take a look at the first line of our scraper, after the import statements, and figure out how to handle any exceptions this might throw:
`html = urlopen("...")`

There are two main things that can go wrong in this line:
- The page is not found on the server (or there was some error in retrieving it)
- The server is not found

In the first situation（服务器找到了，但是服务器上没有找到该网页）, an HTTP error will be returned. This HTTP error may be "404 Page Not Found", "500 Internal Server Error", etc. The urlopen function will throw the generic exception "HTTPError" We can handle this exception in the following way:

```python:n
try:
	html = urlopen("http://...")
except HTTPError as e:
	print(e)
	# return null, break, or do some other "Plan B"
else:
	# program continues.
	# Note: If you return or break in the exception catch, you do not need to use the "else" statement.
```
**If an HTTP error code is returned, the program now prints the error, and does not execute the rest of the program under the else statement.**

If the server is not found at all (if, say http://... was down, or the URL was mistyped)（连服务器都没有找到）, urlopen returns a None object.

We can add a check to see if the returned html is None:
```python:n
if html is None:
	print("URL is not found")
else:
	# program continues
```

Every time you access a tag in a BeautifulSoup object, it's smart to add a check to make sure the tag actually exists. If you attempt to access to a tag that does not exist, BeautifulSoup will return a None object.

**The problem is, attempting to access a tag on a None object itself will result in an AttributeError being thrown.**

If you don't check for it, the trouble comes. But instead go on and try to call some other function on the None object, as illustrated in the following:
`print(bsObj.nonExistentTag.someTag)`

which returns the exception:
`AttributeError: 'NoneType' Object has no attribute 'someTag'`

So how can we guard against these two situations?

The easiest way is to explicitly check for both situations:
```python:n
try:
	badContent = bsObj.nonExistentTag.anotherTag
except AttributeError as e:
	print("Tag was not found")
else:
	if badContent == None:
		print("Tag was not found")
	else:
		print(badContent)
```

This checking and handling of every error does seem laborious at first, but it's easy to add a little reorganization to this code to make it less difficult to write (and more importantly much less difficult to read).
```python:n
from urllib.request import urlopen
from urllib.error import HTTPError
from bs4 import BeautifulSoup
import sys

def getTitle(url):
    try:
        html = urlopen(url)
    except HTTPError as e:
        print(e)
        return None
    try:
        bsObj = BeautifulSoup(html.read())
        title = bsObj.body.h1
    except AttributeError as e:
        return None
    return title

title = getTitle("http://www.pythonscraping.com/exercises/exercise1.html")
if title == None:
    print("Title could not be found")
else:
    print(title)
```