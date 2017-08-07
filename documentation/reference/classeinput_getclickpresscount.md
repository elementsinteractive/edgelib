:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetClickPressCount


```c++
unsigned long GetClickPressCount(long &x, long &y, unsigned char button = EIC_ALL, bool includerepeat = false)
```

## Parameters
### &x,&y (out)
The clickdevice coordinates.

### button (in)
The mouse button or finger to check.

### includerepeat (in)
Include repeat clickdevice events.

## Return value
The number of clickdevice press events.

## Description
Check input events and return the number of click device press events for the specified button.

## Version changes
- EDGELIB 3.00: Added this function to the SDK

