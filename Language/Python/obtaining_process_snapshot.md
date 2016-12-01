1. get an accurate picture of what the target process was up
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
