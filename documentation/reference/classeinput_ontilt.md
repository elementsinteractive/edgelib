:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnTilt


```c++
void OnTilt(long x, long y, long z)
```

## Parameters
### x, y, z (in)
The new angle of the tilt device.

## Return value
None.

## Description
For input objects not part of [ECD](ecd.md): call this function manually after the angle of the tilt device has been changed.

## Version changes
- EDGELIB 3.90: Added a z-axis parameter to this function 
- EDGELIB 2.70: Added this function to the SDK

