:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PutPixel


```c++
void PutPixel(long x, long y, COLORREF col, unsigned char opacity = 255)
```

## Parameters
### x, y (in)
The position of the target pixel.

### col (in)
The pixel color value.

### opacity (in)
The amount of opacity. This can be a value between 0 (no opacity) to 255 (full opacity, default setting).

## Return value
None.

## Description
Put one pixel on the surface, this only works correctly for non-readonly surfaces.

