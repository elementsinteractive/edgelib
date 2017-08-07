:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnContextLost


```c++
ERESULT OnContextLost(ClassEDisplay *display)
```

## Parameters
### *display (out)
A pointer to the [`ClassEDisplay` module](classedisplay.md).

## Return value
Return `E_OK` when everything went correctly. Return `E_ERROR` to shutdown EDGELIB.

## Description
This is a callback when the OS has invalidated the rendering context. Use this to reload surfaces and any  manually managed objects.

## Version changes
- EDGELIB 4.00: Added this function to the SDK

