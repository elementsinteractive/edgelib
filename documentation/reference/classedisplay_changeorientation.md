:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# ChangeOrientation


```c++
bool ChangeOrientation(unsigned char orientation)
```

## Parameters
### orientation (in)
The new orientation the display should take, the orientation of the operating system will not be changed.

## Return value
Returns true when successful.

## Description
Change the display orientation run-time.

## Version changes
- EDGELIB 4.00: Orientations can be changed in OpenGL mode, fixed a bug with orientations on iOS. 
- EDGELIB 3.10: Added this function to the SDK

