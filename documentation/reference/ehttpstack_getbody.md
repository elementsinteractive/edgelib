:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetBody


```c++
bool GetBody(void *buffer, unsigned long &bufsize)
```

## Parameters
### *buffer (out)
The buffer for receiving the HTTP body.

### &bufsize (out)
The size of the HTTP body inside the buffer.

## Return value
Returns true if a HTTP body is present on the stack, otherwise returns false.

## Description
Get the HTTP body after pushing HTTP data on the stack.

