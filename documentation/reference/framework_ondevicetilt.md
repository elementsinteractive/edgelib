:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnDeviceTilt


```c++
void OnDeviceTilt(long x, long y, long z)
```

## Parameters
### x, y, z (out)
The amount of tilt. This value ranges from -1024 to 1024.

## Return value
None.

## Description
This is a callback when a tilt device is connected. This can be a tilt-enabled Symbian device or Pocketmotion's TiltCONTROL hardware. For Symbian this function is called when the tilt angle changes, for the tiltCONTROL hardware this function is called 10 times per second.

## Version changes
- EDGELIB 3.90: Added a z-axis parameter to this function 
- EDGELIB 2.40: Added this function to the SDK

