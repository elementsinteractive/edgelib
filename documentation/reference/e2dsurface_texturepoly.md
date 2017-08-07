:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# TexturePoly


```c++
void TexturePoly(long x1, long y1, long u1, long v1, long x2, long y2, long u2, long v2, long x3, long y3, long u3, long v3, E2DSurface *source, long flags = 0, unsigned long property = 0)
```

## Parameters
### x1, y1, x2, y2, x3, y3 (in)
The positions of the polygon points.

### u1, v1, u2, v2, u3, v3 (in)
The polygon texture coordinates.

### *source (in)
A pointer to the 2D surface containing the texture.

### flags (in)
Extra [flags](classedisplay_definitions.md) for drawing effects.

### property (in)
A property to be used in combination with the flag parameter.

## Return value
None.

## Description
Draw a polygon with a texture. 
This function executes faster for some occasions when the source surface width and height are equal and have a specific size. 
Optimized sizes: 8x8, 16x16, 32x32, 64x64, 128x128, 256x256, 512x512 or 1024x1024 pixels.

## Version changes
- EDGELIB 2.30: Changed the property parameter to unsigned long

