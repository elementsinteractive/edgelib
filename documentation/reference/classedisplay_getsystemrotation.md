:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetSystemRotation


```c++
unsigned char GetSystemRotation(void)
```

## Parameters
None.

## Return value
Get the [display rotation](classedisplay_definitions.md) of the device before the application starts.

## Description
For some devices the display rotation can be changed. For example, on Windows Mobile the user may rotate the screen to landscape orientation for internet browsing. This function gets the rotation value before the application starts.

## Version changes
- EDGELIB 2.50: Added this function to the SDK

