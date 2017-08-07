:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetPixel


```c++
GetPixel(long x, long y, COLORREF &col, unsigned char &opacity)
```

## Parameters
### x, y (in)
The position of the requested pixel.

### col (in)
The pixel color value to get.

### opacity (in)
The amount of opacity to get. This can be a value between 0 (no opacity) to 255 (full opacity, default setting).

## Return value
None.

## Description
Get the color and opacity value of a pixel on the surface, this only works correctly for non-readonly surfaces.

