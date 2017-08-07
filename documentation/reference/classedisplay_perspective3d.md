:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Perspective3D


```c++
void Perspective3D(long width, long height, long fov = 512, long xoffs = 0, long yoffs = 0)
```

## Parameters
### width, height (in)
These should match the resolution of the display for correct perspective view

### fov (in)
The field of view angle of the camera. The default value (512) is a 45 degree camera

### xoffs, yoffs (in)
The film offset which can be used to move the camera view

## Return value
None.

## Description
Changes to perspective view in OpenGL. The viewable object space will be in fixed point around -4096 to 4096 to correctly view 3D surfaces.

## Version changes
- EDGELIB 3.80: Added field of view and film offset parameters 
- EDGELIB 3.00: Added this function to the SDK

