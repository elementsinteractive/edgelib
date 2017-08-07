:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GradientRect


```c++
void GradientRect(RECT *rc, COLORREF topleft, COLORREF topright, COLORREF bottomleft, COLORREF bottomright, long flags = 0) 
void GradientRectH(RECT *rc, COLORREF left, COLORREF right, long flags = 0) 
void GradientRectV(RECT *rc, COLORREF top, COLORREF bottom, long flags = 0)
```

## Parameters
### *rc (in)
A pointer to a [rectangle structure](ref_globalstructures.md) with the shape and position of the rectangle.

### topleft, topright, bottomleft, bottomright (in)
The pixel color values for each corner.

### top, bottom, left, right (in)
The pixel color value for screen edges.

### flags (in)
Extra [flags](classedisplay_definitions.md) for drawing effects. Only `EFX_DITHER` is supported.

## Return value
None.

## Description
Draw a filled rectangle with a gradient.

