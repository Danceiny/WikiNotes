---
date: 2016-04-20 21:16
status: public
title: 'chapter3-Buiding a Windows debugger-python黑客'
---

原型：PyDbg

# Debuggee， Where Art Thou?
In order to perform a debugging task on a process,you must first to be able to associated the debugger to the process in some way. Therefore, our debugger must be able to either open an executable and run it or attach to a running process. The Windows debugging API provides an easy way to do both.

Differences between opening a process and attaching to a process:
>opening: have control of the process before it has a chance to run any code.
>attaching: breaks into an already running process, which allows you to skip the startup portion of the code and analyze specific areas of code that you are interested in.

![](~/21-25-53.jpg)
The important parameters are `lpApplicationName`,`lpCommandLine`,`dwCreationFlags`,`lpStartupInfo` and `lpProcessInformation`. The rest can be set to NULL.

## my_debugger_defines.py
```python:n
from ctypes import *

#let's map the microsoft types to ctypes for clarity
WORD = c_ushort
DWORD = c_ulong
LPBYTE = POINTER(c_ubyte)
LPTSTR = POINTER(c_char)
HANDLE = c_void_p

#constants
DEBUG_PROCESS = 0x00000001
CREATE_NEW_CONSOLE = 0x00000010

#structures for CreateProcessA() function
class STARTUPINFO(Structure):
    _fields_ = [
        ("cb",DWORD),
        ("lpReserved",LPTSTR),
        ("lpDesktop",LPTSTR),
        ("lpTitle",LPTSTR),
        ("dwX",DWORD),
        ("dwY",DWORD),
        ("dwXSize",DWORD),
        ("dwYSize",DWORD),
        ("dwXCountChars",DWORD),
        ("dwYCountChars",DWORD),
        ("dwFillAttribute",DWORD),
        ("dwFlags",DWORD),
        ("wShowWindow",WORD),
        ("cbReserved2",WORD),
        ("lpReserved2",LPBYTE),
        ("hStdInput",HANDLE),
        ("hStdOutput",HANDLE),
        ("hStdError",HANDLE),
    ]

class PROCESS_INFORMATION(Structure):
    _fields_ = [
        ("hProcess",HANDLE),
        ("hThread",HANDLE),
        ("dwProcessId",DWORD),
        ("dwThreadId",DWORD),
    ]
```

## my_debugger.py
```python:n
from ctypes import *
from my_debugger_defines import *

kernel32 = windll.kernel32

class debugger():
    def __init__(self):
        pass
    def load(self,path_to_exe):

        #dwCreation flag determines how to create the process
        #set creation_flags = CREATE_NEW_CONSOLE if you want
        #to see the calculator GUI
        creation_flags = DEBUG_PROCESS

        #instantiate the structs
        startupinfo = STARTUPINFO()
        process_information = PROCESS_INFORMATION()

        #the following two options allow the started process to be shown as a separate window.
        # This also illustrates how different settings in the STARTUPINFO struct can affect the debuggee.
        startupinfo.dwFlags = 0x1
        startupinfo.wShowWindow = 0x0

        #we then initialize the cb variable in the STARTUPINFO struct
        #which is just the size of the struct itself
        startupinfo.cb = sizeof(startupinfo)

        if kernel32.CreateProcessA(path_to_exe,
                                   None,
                                   None,
                                   None,
                                   None,
                                   creation_flags,
                                   None,
                                   None,
                                   byref(startupinfo),
                                   byref(process_information)):
            print "[*] We have successfully launched the process!"
            print "[*] PID: %d" % process_information.dwProcessId
        else:
            print "[*] Error: 0x%08x." % kernel32.GetLastError()
```

## my_test.py
```python:n
import my_debugger
debugger = my_debugger.debugger()
debugger.load("C:\\Windows\\system32\\calc.exe")

```
![](~/22-04-36.jpg)


In order to prepare a process to attach to, it is useful to obtain a handle to the process itself. Most of the functions we will be using require a valid process handle, and it's nice to know whether we can access the process before we attempt to debug it. This is done with `OpenProcess()`, which is exported from kernel32.dll and has the following prototype:
![](~/22-10-30.jpg)

The dwDesiredAccess parameter indicates what type of access rights we are requesting for the process object we wish to obtain a handle to. In order to perform debugging, we have to set it to PROCESS_ALL_ACCESS. The bInheritHandle parameter will always be set to False for our purposes, and the dwProcessId parameter is simply the PID of the process we wish to obtain a handle to. If the function is successful, it will return a handle to the process object.

We attach to the process using the DebugActiveProcess() function:
`Bool WINAPI DebugActiveProcess(DWORD dwProcessId);`

The debugger traps these debugging events by calling WaitForDebugEvent() in a loop:

![](~/22-23-31.jpg)
Often set the second parameter to Infinite.

Once the handles are finished executing, we want the process to continue. Once the handlers are finished executing, we want the process to continue executing:

![](~/22-27-05.jpg)
The dwContinueStatus parameter signals the process to continue executing (DBG_CONTINUE) or to continue processing the exception (DBG_EXCEPTION_NOT_HANDLED).

The only thing left to do is to detach(分离) from the process, by calling DebugActiveProcessStop(), which takes the PID as its only parameter.



Let's put all of this together and extend our my_debugger class by providing it the ability to attach to and detach from a process.
The final implementation detail will be to create our primary debug loop to handle debugging events. 

### my_debugger.py
```python:n
from ctypes import *
from my_debugger_defines import *

kernel32 = windll.kernel32

class debugger():
    def __init__(self):
        #pass
        self.h_process = None
        self.pid = None
        self.debugger_active = False

    def load(self,path_to_exe):

        #dwCreation flag determines how to create the process
        #set creation_flags = CREATE_NEW_CONSOLE if you want
        #to see the calculator GUI
        creation_flags = DEBUG_PROCESS

        #instantiate the structs
        startupinfo = STARTUPINFO()
        process_information = PROCESS_INFORMATION()

        #the following two options allow the started process to be shown as a separate window.
        # This also illustrates how different settings in the STARTUPINFO struct can affect the debuggee.
        startupinfo.dwFlags = 0x1
        startupinfo.wShowWindow = 0x0

        #we then initialize the cb variable in the STARTUPINFO struct
        #which is just the size of the struct itself
        startupinfo.cb = sizeof(startupinfo)

        if kernel32.CreateProcessA(path_to_exe,
                                   None,
                                   None,
                                   None,
                                   None,
                                   creation_flags,
                                   None,
                                   None,
                                   byref(startupinfo),
                                   byref(process_information)):
            print "[*] We have successfully launched the process!"
            print "[*] PID: %d" % process_information.dwProcessId
            #obtain a valid handle to the newly created process
            #and store it for future access
            self.h_process = self.open_process(process_information.dwProcessId)
        else:
            print "[*] Error: 0x%08x." % kernel32.GetLastError()

    def open_process(self,pid):
        h_process = kernel32.OpenProcess(PROCESS_ALL_ACCESS,pid,False)
        return h_process

    def attach(self,pid):
        self.h_process = self.open_process(pid)
        #we attemp to attach to the process
        #if this fails we exit the call
        if kernel32.DebugActiveProcess(pid):
            self.debugger_active = True
            self.pid = int(pid)
            self.run()
        else:
            print "[*] Unable to attach to the process."
    def run(self):
        #now we have to poll the debuggee for debugging events
        while self.debugger_active==True:
            self.get_debug_event()
    def get_debug_event(self):
        debug_event = DEBUG_EVENT()
        continue_status = DBG_CONTINUE

        if kernel32.WaitForDebugEvent(byref(debug_event),INFINITE):
            #we aren't going to build any event handlers just yet.
            #Let's just resume the process for now.
            # if everything works ok, then comment out the following 2 lines
            # raw_input("Press a key to continue...")
            # self.debugger_active = False
            kernel32.ContinueDebugEvent(\
                debug_event.dwProcessId,\
                debug_event.dwThreadId,\
                continue_status)
    def detach(self):
        if kernel32.DebugActiveProcessStop(self.pid):
            print "[*] Finished debugging. Exiting..."
            return True
        else:
            print "There was an error"
            return False
```

### my_test.py
```python:n
import my_debugger
debugger = my_debugger.debugger()
pid = raw_input("Enter the PID of the process to attach to: ")
debugger.attach(int(pid))
debugger.detach()
#debugger.load("C:\\Windows\\system32\\calc.exe")
```

### How to test

![](~/23-03-30.jpg)
![](~/23-04-23.jpg)


