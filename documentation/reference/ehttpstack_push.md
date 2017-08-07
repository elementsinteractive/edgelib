:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Push


```c++
bool Push(void *buffer, unsigned long bufsize)
```

## Parameters
### *buffer (in)
Push Edge network data onto the stack. This needs to be done when network data is received.

### bufsize (in)
The amount of memory to push onto the stack.

## Return value
Return true if the header has been pushed on the stack and content can be read.

## Description
Push data received through the [OnNetMessage](framework_onnetmessage.md) function onto a stack for interpreting HTTP data.

