:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# BltFast


```c++
void BltFast(long x, long y, E2DSurface *source, RECT *rc) 
void BltFast(long x, long y, E2DSurfaceRGBA *source, RECT *rc)
```

## Parameters
### x, y (in)
The position to blit to

### *source (in)
The source surface to blit from

### *rc (in)
A pointer to a rectangle to blit

## Return value
None.

## Description
Blits to the active display. This can be the backbuffer or OpenGL.

## Version changes
- EDGELIB 3.60: Added overloaded function for RGBA surfaces 
- EDGELIB 3.00: Added this function to the SDK

