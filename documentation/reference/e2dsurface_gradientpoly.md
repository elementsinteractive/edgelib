:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GradientPoly


```c++
void GradientPoly(long x1, long y1, COLORREF col1, long x2, long y2, COLORREF col2, long x3, long y3, COLORREF col3, long flags = 0)
```

## Parameters
### x1, y1, x2, y2, x3, y3 (in)
The positions of the polygon points.

### col1, col2, col3 (in)
The color of the gradient polygon points.

### flags (in)
Extra [flags](classedisplay_definitions.md) for drawing effects. Only EFX_DITHER is supported.

## Return value
None.

## Description
Draw a filled polygon with a gradient.

## Version changes
- EDGELIB 2.60: Split functionality from FillPoly to new function GradientPoly

