:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Blt


```c++
void Blt(long center_x, long center_y, E2DSurface *source, RECT *rc, unsigned short rotation, unsigned long scale, unsigned char flipflags = EFLIP_NONE) 
void Blt(long center_x, long center_y, E2DSurfaceRGBA *source, RECT *rc, unsigned short rotation, unsigned long scale, unsigned char flipflags = EFLIP_NONE)
```

## Parameters
### center_x, center_y (in)
The position of the destination to blit, the center of the source surface rectangle.

### *source (in)
A pointer to a 2D surface to blit from.

### *rc (in)
A pointer to a [rectangle structure](ref_globalstructures.md) with the shape and position of the rectangle.

### rotation (in)
Used to rotate blitting, a value between 0 and 4096 represents a whole circle.

### scale (in)
Used for scale blitting. Values bigger than 65536 will enlarge the image, values smaller than 65536 will shrink the image.

### flipflags (in)
Extra [flags](classedisplay_definitions.md) to flip the image.

## Return value
None.

## Description
Blits to the active display. This can be the backbuffer or OpenGL.

## Version changes
- EDGELIB 3.80: Added this function to the SDK

