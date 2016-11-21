---
date: 2016-04-23 23:12
status: public
title: 'Obtaining Cpu Register State-python黑客'
---

![](~/00-05-06.jpg)

We must obtain a list of all the threads that are executing inside the process, select the thread we want, and obtain a valid handle to it using OpenThread().

# Thread Enumeration
We can enumerate the threads by using a very powerful function called CreateToolhelp32Snapshot(), which is exported from kernel32.dll. **This function enables us to obtain a list of process, threads, and loaded modules (DLLs) inside a process as well as the heap list that a process owns.**

![](~/00-10-36.jpg)

The dwFlags parameter instructs the function what type of information it is supposed to gather (threads,processes,modules,or heaps). We set this to `Th32CS_SNAPTHREAD`, which has a value of `0x00000004`.
The th32ProcessID is simply the PID of the process we want to take a snapshot of, but it is used only for the TH32CS_SNAPMODULE,Th32CS_SNAPMODULE32,TH32CS_SNAPHEAPLIST, and TH32CS_SNAPALL modes. **So it's up to us to determine whether  a thread belongs to our process or not.**
The function returns a handle to the snapshot object, when successful.


Then begin enumerating the list of threads, using the Thread32First():

![](~/00-17-20.jpg)
The lpte parameter is a pointer to a THREADENTRY32 structure:

```
typedef struct THREADENTRY32{
    DWORD dwSize;
    DWORD cntUsage;
    DWORD th32ThreadId;
    DWORD th32OwnerProcessId;
    LONG tpBasePri;
    LONG tpDeltaPri;
    DWORD dwFlags;
};
```
This structure gets populated when the Thread32First() call completes successfully, and it contains relevant information for the first thread that was found.

The 3 fields in this struct that we are interested in are **dwSize,th32ThreadID,and th32OwnerProcessID**.
The dwSize field must be initialized to the size of the struct itself, before making a call to the Thread32First() function.
The th32ThreadID is the TID for the thread we are examining; we can use this identifier as the dwThreadId parameter for the OpenThread() function.
The th32OwnerProcessID field is the PID that identifies which process the thread is running under.
**In order to determine all threads inside our target process, we'll compare each th32OwnerProcessID value against the PID of the process we either created or attached to.**
When captured the 1st thread's information, move on to the next by calling Thread32Next().


# Putting It All Together
To grab the values of all the registers, by calling GetThreadContext().

![](~/00-31-17.jpg)
The hThread parameter is the handle returned from an OpenThread(), lpContext parameter is a pointer to a CONTEXT structure, which holds all of the register values.
```C:n
typedef struct CONTEXT{
...
};
```
**make sure familiar with this struct!!!**

my_test:
```python:n
import my_debugger
debugger = my_debugger.debugger()
pid = raw_input("Enter the PID of the process to attach to: ")
debugger.attach(int(pid))



# debugger.detach()
# #debugger.load("C:\\Windows\\system32\\calc.exe")

list = debugger.enumerate_threads()

#for each thread in the list we want to grab the value of each of the registers for thread in list:
for thread in list:
    thread_context = debugger.get_thread_context(thread)
    #now let's output the contents of some of the registers
    print '[*] Dumping registers for thread ID: 0x%o8x' % thread
    print '[**] EIP: 0x%08x' % thread_context.Eip
    print '[**] EIP: 0x%08x' % thread_context.Esp
    print '[**] EIP: 0x%08x' % thread_context.Ebp
    print '[**] EIP: 0x%08x' % thread_context.Eax
    print '[**] EIP: 0x%08x' % thread_context.Ebx
    print '[**] EIP: 0x%08x' % thread_context.Ecx
    print '[**] EIP: 0x%08x' % thread_context.Edx
    print '[*] END DUMP'

debugger.detach()

```

my_debugger.py
后面添加。
```python:n

    def open_thread(self,thread_id):
        h_thread = kernel32.OpenThread(THREAD_ALL_ACCESS,None,thread_id)

        if h_thread is not None:
            return h_thread
        else:
            print "[*] Could not obtain a valid thread handle."
            return False
    def enumerate_threads(self):
        thread_entry = THREADENTRY32()
        thread_list = []
        snapshot = kernel32.CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,self.pid)

        if snapshot is not None:
            #you have to set the size of the struct or the call will fail
            thread_entry.dwSize = sizeof(thread_entry)
            success = kernel32.Thread32First(snapshot,byref(thread_entry))

            while success:
                if thread_entry.th32OwnerProcessID == self.pid:
                    thread_list.append(thread_entry.th32ThreadID)
                    success = kernel32.Thread32Next(snapshot,byref(thread_entry))
            kernel32.CloseHandle(snapshot)
            return thread_list
        else:
            return False

    def get_thread_context(self,thread_id):
        context = CONTEXT()
        context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS

        #obtain a handle to the thread
        h_thread = self.open_thread(thread_id)
        if kernel32.GetThreadContext(h_thread,byref(context)):
            kernel32.CloseHandle(h_thread)
            return context
        else:
            return False
```

# Implementing Debug Event Handlers
The DEBUG_EVENT strut is defined like this:
![](~/10-05-25.jpg)

The dwDebugEventCode is of particular interest, as it is dictates what type of event was trapped by the WaitForDebugEvent() function, and dictates the type and value for the u union.

![](~/10-08-02.jpg)

Let's modify the debug loop to show us which event has been fired on the event code. Using that information, we will be able to see the general flow of events after we have spawned or attached to a process.

```python:n

class debugger():
    def __init__(self):
        #pass
        self.h_process = None
        self.pid = None
        self.debugger_active = False
        self.h_thread = None
        self.context = None
    def get_debug_event(self):
        debug_event = DEBUG_EVENT()
        continue_status = DBG_CONTINUE

        if kernel32.WaitForDebugEvent(byref(debug_event),INFINITE):
            #let's obtain the thread and context information
            self.h_thread = self.open_thread(debug_event.dwThreadId)
            self.context = self.get_thread_context(self.h_thread)

            print "Event Code: %d Thread ID: %d" % (debug_event.dwDebugEventCode,debug_event.dwThreadId)

            kernel32.ContinueDebugEvent(\
                debug_event.dwProcessId,\
                debug_event.dwThreadId,\
                continue_status)

```


my_test.py
```python:n
import my_debugger
debugger = my_debugger.debugger()
pid = raw_input("Enter the PID of the process to attach to: ")
debugger.attach(int(pid))

debugger.run()
debugger.detach()
```

![](~/10-26-08.jpg)

![](~/10-26-47.jpg)

Let's start with catching the first Windows-driven breakpoint.

my_debugger.py
```python:n
class debugger():
    def __init__(self):
        #pass
        self.h_process = None
        self.pid = None
        self.debugger_active = False
        self.h_thread = None
        self.context = None

        self.exception = None
        self.exception_address = None

    def get_debug_event(self):
        debug_event = DEBUG_EVENT()
        continue_status = DBG_CONTINUE

        if kernel32.WaitForDebugEvent(byref(debug_event),INFINITE):
            #let's obtain the thread and context information
            self.h_thread = self.open_thread(debug_event.dwThreadId)
            self.context = self.get_thread_context(self.h_thread)

            print "Event Code: %d Thread ID: %d" % (debug_event.dwDebugEventCode,debug_event.dwThreadId)

            #IF THE event code is an exception, we want to examine it further.
            if debug_event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT:
                #obtain the exception code
                exception = debug_event.u.Exception.ExceptionRecord
                self.exception_address = debug_event.u.Exception.ExceptionRecord.ExceptionAddress

                if exception == EXCEPTION_ACCESS_VIOLATION:
                    print "Access Violation Detected."
                    #if a breakpoint is detected, we call an internal handler.
                elif exception == EXCEPTION_BREAKPOINT:
                    continue_status = self.exception_handler_breakpoint()
                elif exception == EXCEPTION_GUARD_PAGE:
                    print "Guard Page Access Detected."
                elif exception == EXCEPTION_SINGLE_STEP:
                    print "Single Stepping."


            kernel32.ContinueDebugEvent(\
                debug_event.dwProcessId,\
                debug_event.dwThreadId,\
                continue_status)

    def exception_handler_breakpoint(self):
        print '[*] Inside the breakpoint handler.'
        print 'Exception Address: 0x%08x' % self.exception_address
        return DBG_CONTINUE
```       

Now rerun test script, you should see the output from the soft breakpoint exception handler.


# The Almighty Breakpoint
## Soft Breakpoints
In order to place soft breakpoints, need to be able to read and write into a process's memory. This is done via the `ReadProcessMemory() and WriteProcessMemory()`

![](~/10-58-40.jpg)

lpBaseAddress is the address where you wish to start reading or writing.
The lpBuffer is a pointer to the data that you are either reading or writing.
nSize is the total number of bytes you wish to read or write.


Using these two function calls, enable our debugger to use soft breakpoints quite easily.

modify core debugging class:

my_debugger.py
```python:n
class debugger():
    def __init__(self):
        # pass
        self.h_process = None
        self.pid = None
        self.debugger_active = False
        self.h_thread = None
        self.context = None

        self.exception = None
        self.exception_address = None

        self.breakpoints = {}

     def read_process_memory(self,address,length):
            data = ""
            read_buf = create_string_buffer(length)
            count = c_ulong(0)

            if not kernel32.ReadProcessMemory(self.h_process,
                                              address,
                                              read_buf,
                                              length,
                                              byref(count)):
                return False
            else:
                data += read_buf.raw
                return data

        def write_process_memory(self,address,data):
            count = c_long(0)
            length = len(data)
            c_data = c_char_p(data[count.value:])

            if not kernel32.WriteProcessMemory(self.h_process,
                                               address,
                                               c_data,
                                               length,
                                               byref(count)):
                return False
            else:
                return True
        def bp_set(self,address):
            if not self.breakpoints.has_key(address):
                try:
                    # store the orginal byte
                    original_byte = self.read_process_memory(address,1)
                    #write the INT3 opcode
                    self.write_process_memory(address,"\xCC")
                    # register the breakpoint in our internal list
                    self.breakpoints[address]=(address,original_byte)

                except:
                    return False
            return True
```

In general, breakpoints are set on a function call of some type.
The Windows debugging API has given a method for determining the virtual address of a function GetProcAddress(). The only primary requirement of this function is a handle to the module that contains the function we are interested in; Obtain this handle by GetModuleHandle().

![](~/16-35-02.jpg)


chain of events:
we obtain a handle to the module and then search for the address of the exported function we want.

modify my_debugger.py:
```python:n
class debugger():
...
    def fun_resolve(self,dll,funtion):
        handle = kernel32.GetModuleHandleA(dll)
        address = kernel32.GetProcAddress(handle,function)
        kernel32.CloseHandle(handle)
        return address
```
Now let's create a second test harness（钩子） that will use printf() in a loop. We'll resolve the function address and then set a soft breakpoint on it. After the breakpoint is hit, we should see some output, and then the process will continue its loop.

printf_loop.py:
```python:n
from ctypes import *
import time

msvcrt = cdll.msvcrt
counter = 0

while 1:
  msvcrt.printf("loop iteration %d!\n" % counter)
  time.sleep(2)
  counter += 1
```

Now let's go test:
**my_test.py**
```python:n
import my_debugger
debugger = my_debugger.debugger()
pid = raw_input("enter the pid of the process to attach to: ")
debugger.attach(int(pid))
printf_address = debugger.func.resolve("msvcrt.dll","printf")
print "[*] Address of printf: 0x%08x" % printf_address
debugger.bp_set(printf_address)
debugger.run()
```

## Hardware Breakpoints

**my_debugger.py**
```python:n
class debugger():
    def __init__(self):
        ...
        # plus the following
        self.first_breakpoint = True
        self.hardware_breakpoints = {}
    ...
    def bp_set_hw(self,address,length,condition):
    # check for a valid length value
        if length not in (1,2,4):
            return False
        else:
            length -= 1
        # check for a valid condition
        if condition not in (HW_ACCESS, HW_EXECUTE, HW_WRITE):
            return False
        # check for available slots
        if not self.hardware_breakpoints.has_key(0):
            available = 0
        elif not self.hardware_breakpoints.has_key(1):
            available = 1
        elif not self.hardware_breakpoints.has_key(2):
            available = 2
        elif not self.hardware_breakpoints.has_key(3):
            available = 3
        else:   
            return False

        # we want to set the debug register in every thread
        for thread_in self.enumerate_threads():
            context =self.get_thread_context(thread_id = thread_id)

            # Enable the appropriate flag in the DR7
            # register to set the breakpoint
            context.Dr7 |= 1 << (available*2)

        # Save the address of the breakpoint in the free register that we found
        if available == 0:
            context.Dr0 = address
        elif available == 1:
            context.Dr1 = address
        elif available == 2:
            context.Dr2 = address
        elif available == 3:
            context.Dr3 = address

        # Set the breakpoint condition
        context.Dr7 |= condition << ((available*4)+16)

        # Set the length
        context.Dr7 |= length << ((available*4)+18)

        # Set thread context with the break set
        h_thread = self.open_thread(thread_id)
        kernel32.SetThreadContext(h_thread,byref(context))

        # update the internal hardware breakpoint array at the used slot index.
        self.hardware_breakpoints[available] = (address,length,condition)

        return True
    def get_debug_event(self):
        if self.exception == EXCEPTION_ACCESS_VIOLATION:
            print "Access Violation Detected."
        elif self.exception == EXCEPTION_BREAKPOINT:
            continue_status = self.exception_handler_breakpoint()
        elif self.exception == EXCEPTION_GUARD_PAGE:
            print "Guard Page Access Detected."
        elif self.exception == EXCEPTION_SINGLE_STEP:
            self.exception_handler_single_step()
    def exception_handler_single_step(self):
        # Comment from PyDbg:
        # determine if this single step event occurred in reaction to a
        # hardware breakpoint and grab the hit breakpoint.
        # according to the Intel docs, we should be able to check for
        # the BS flag in Dr6. but it appears that Windows
        # isn't properly propagating that flag down to us.
        if self.context.Dr6 & 0x1 and self.hardware_breakpoints.has_key(0):
            slot = 0
        elif self.context.Dr6 & 0x2 and self.hardware_breakpoints.has_key(1):
            slot = 1
        elif self.context.Dr6 & 0x4 and self.hardware_breakpoints.has_key(2):
            slot = 2
        elif self.context.Dr6 & 0x8 and self.hardware_breakpoints.has_key(3):
            slot = 3
        else:
            # This wasn't an INT1 generated by a hw breakpoint
            continue_status = DBG_EXCEPTION_NOT_HANDLED
        # Now let's remove the breakpoint from the list
        if self.bp_del_hw(slot):
            continue_status = DBG_CONTINUE
        print "[*] Hardware breakpoint removed."
        return continue_status

    def bp_del_hw(self,slot):
        # Disable the breakpoint for all active threads
        for thread_id in self.enumerate_threads():
            context = self.get_thread_context(thread_id=thread_id)
            # Reset the flags to remove the breakpoint
            context.Dr7 &= ~(1 << (slot * 2))
            # Zero out the address
            if slot == 0:
                context.Dr0 = 0x00000000
            elif slot == 1:
                context.Dr1 = 0x00000000
            elif slot == 2:
                context.Dr2 = 0x00000000
            elif slot == 3:
                context.Dr3 = 0x00000000
            # Remove the condition flag
            context.Dr7 &= ~(3 << ((slot * 4) + 16))
            # Remove the length flag
            context.Dr7 &= ~(3 << ((slot * 4) + 18))
            # Reset the thread's context with the breakpoint removed
            h_thread = self.open_thread(thread_id)
            kernel32.SetThreadContext(h_thread,byref(context))
        # remove the breakpoint from the internal list.
        del self.hardware_breakpoints[slot]
        return True

```  
Let's test:
**my_test.py**
```python:n
import my_debugger
from my_debugger_defines import *
debugger = my_debugger.debugger()
pid = raw_input("Enter the PID of the process to attach to: ")
debugger.attach(int(pid))
printf = debugger.func_resolve("msvcrt.dll","printf")
print "[*] Address of printf: 0x%08x" % printf
debugger.bp_set_hw(printf,1,HW_EXECUTE)
debugger.run()
```

## Memory Breakpoints
```python:n
class debugger():
    def __init__(self):
        ...
        # Here let's determine and store
        # the default page size for the system
        system_info = SYSTEM_INFO()
        kernel32.GetSystemInfo(byref(system_info))
        self.page_size = system_info.dwPageSize
    ...

    def bp_set_mem (self, address, size):
        mbi = MEMORY_BASIC_INFORMATION()
        # If our VirtualQueryEx() call doesn’t return
        # a full-sized MEMORY_BASIC_INFORMATION
        # then return False
        if kernel32.VirtualQueryEx(self.h_process,
                                    address,
                                    byref(mbi),
                                    sizeof(mbi)) < sizeof(mbi):
            return False
        current_page = mbi.BaseAddress
        # We will set the permissions on all pages that are
        # affected by our memory breakpoint.
        while current_page <= address + size:
            # Add the page to the list; this will
            # differentiate our guarded pages from those
            # that were set by the OS or the debuggee process
            self.guarded_pages.append(current_page)
            old_protection = c_ulong(0)
            if not kernel32.VirtualProtectEx(self.h_process,
                                current_page, size,
                                mbi.Protect | PAGE_GUARD, byref(old_protection)):
                return False
            # Increase our range by the size of the
            # default system memory page size
            current_page += self.page_size
        # Add the memory breakpoint to our global list
        self.memory_breakpoints[address] = (address, size, mbi)
        return True

```
