:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# FillPoly


```c++
void FillPoly(long x1, long y1, long x2, long y2, long x3, long y3, COLORREF col, long flags = 0, unsigned long property = 0)
```

## Parameters
### x1, y1, x2, y2, x3, y3 (in)
The positions of the polygon points.

### col (in)
The color of the polygon.

### flags (in)
Extra [flags](classedisplay_definitions.md) for drawing effects.

### property (in)
A property to be used in combination with the flag parameter.

## Return value
None.

## Description
Draw a filled polygon.

## Version changes
- EDGELIB 2.60: Moved the gradient fill overloaded function to [GradientPoly](e2dsurface_gradientpoly.md). 
- EDGELIB 2.30: Changed the property parameter to unsigned long.

