:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnClickMove


```c++
void OnClickMove(long x, long y, unsigned char button = EIC_ALL)
```

## Parameters
### x, y (in)
The new position of the click device.

### button (in)
The button of the click device.

## Return value
None.

## Description
For input objects not part of [ECD](ecd.md): call this function manually after the click device has been moved.

## Version changes
- EDGELIB 3.95: Added a button parameter to this function 
- EDGELIB 2.70: Added this function to the SDK

