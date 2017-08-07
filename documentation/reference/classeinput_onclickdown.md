:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnClickDown


```c++
void OnClickDown(long x, long y, unsigned char button = EIC_ALL)
```

## Parameters
### x, y (in)
The position of the click device.

### button (in)
The [button](classeinput_definitions.md) of the click device, if available.

## Return value
None.

## Description
For input objects not part of [ECD](ecd.md): call this function manually after the click device has been pressed.

## Version changes
- EDGELIB 2.70: Added this function to the SDK

