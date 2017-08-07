:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# FrameToSurfaceAspect


```c++
static void FrameToSurfaceAspect(const E_CAMERAFRAME * frame, E2DSurface * surface)
```

## Parameters
### frame (in)
The [camera](classecamera_structures.md) frame to process

### surface (in)
The [surface](e2dsurface.md) to receive the camera frame

## Return value
none

## Description
Converts a camera frame to an image object, preserving the aspect ratio of the input. The input is scaled to fill the surface as much as possible, and positioned in the middle of the surface. The borders that are not covered by image data are not written.

## Version changes
- EDGELIB 4.00: Added this function

