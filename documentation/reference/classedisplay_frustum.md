:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Frustum


```c++
void Frustum(long xmin, long xmax, long ymin, long ymax, long znear, long zfar)
```

## Parameters
### xmin, xmax, ymin, ymax (in)
The viewport of the frustum in 20:12 fixed point values

### znear, zfar (in)
The z-range of the frustum in 20:12 fixed point values

## Return value
None.

## Description
Set the 3D frustum for perspective transformation.

## Version changes
- EDGELIB 3.80: Added this function to the SDK

