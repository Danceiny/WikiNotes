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
