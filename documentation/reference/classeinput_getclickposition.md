:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetClickPosition


```c++
void GetClickPosition(long &x, long &y, unsigned char button = EIC_ALL)
```

## Parameters
### &x, &y (out)
The position of the click device.

### button (in)
The position of the button or touch to check.

## Return value
None.

## Description
Returns the current position of the click device. A click device is a mouse, stylus or finger. A list of buttons can be found [here](classeinput_definitions.md).

## Version changes
- EDGELIB 3.95: Added a button parameter to this function 
- EDGELIB 2.70: Added this function to the SDK

