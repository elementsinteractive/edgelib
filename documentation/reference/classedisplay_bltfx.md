:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# BltFx


```c++
void BltFx(long center_x, long center_y, E2DSurface *source, RECT *rc, E2DBLTFX *bltfx) 
void BltFx(long center_x, long center_y, E2DSurfaceRGBA *source, RECT *rc, E2DBLTFX *bltfx)
```

## Parameters
### center_x, center_y (in)
The position of the destination to blit, the center of the source surface rectangle.

### *source (in)
A pointer to a 2D surface to blit from.

### *rc (in)
A pointer to a [rectangle structure](ref_globalstructures.md) with the shape and position of the rectangle.

### *bltfx (in)
A pointer to a [blit effect structure](classedisplay_structures.md) containing blit properties.

## Return value
None.

## Description
Blits to the active display. This can be the backbuffer or OpenGL.

## Version changes
- EDGELIB 3.80: Added this function to the SDK

