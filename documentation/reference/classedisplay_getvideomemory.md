:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetVideoMemory


```c++
ERESULT GetVideoMemory(unsigned long &memfree, unsigned long &memtotal)
```

## Parameters
### &memfree (out)
The free amount of video memory.

### &memtotal (out)
The total amount of video memory.

## Return value
`E_OK`: the function call was successful. 
`E_UNSUPPORTED`: this function is not supported by the hardware or software. 
`E_ERROR`: the function call failed.

## Description
Get the current and total amount of video memory, currently only supported for Windows desktop.

## Version changes
- EDGELIB 2.20: Added this function to the SDK

