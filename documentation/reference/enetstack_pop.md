:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Pop


```c++
unsigned char Pop(void *buffer, unsigned long &bufsize)
```

## Parameters
### *buffer (out)
A buffer to receive the data.

### &bufsize (out)
The amount of memory that has been popped from the stack.

## Return value
`EPOP_NODATA`: no data has been popped. 
`EPOP_RECEIVEDATA`: an Edge network package has been received. 
`EPOP_REQUESTPING`: the network package contains a ping request. 
`EPOP_RESPONDPING`: the network package contains a ping response.

## Description
Pop EDGELIB network packages from the stack.

