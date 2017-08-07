:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PutPixelRow


```c++
void PutPixelRow(long x, long y, unsigned long width, COLORREF col, long flags = 0, unsigned long property = 0)
```

## Parameters
### x, y (in)
The position of the target pixel.

### width (in)
The amount of pixels in the row.

### col (in)
The pixel color value.

### flags (in)
Extra [flags](classedisplay_definitions.md) for drawing effects.

### property (in)
A property to be used in combination with the flag parameter.

## Return value
None.

## Description
Put a row of pixels on the surface. This function is faster than plotting each pixel individually.

## Version changes
- EDGELIB 2.30: Changed the property parameter to unsigned long

