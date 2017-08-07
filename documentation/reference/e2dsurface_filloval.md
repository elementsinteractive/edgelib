:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# FillOval


```c++
void FillOval(long x, long y, long xradius, long yradius, COLORREF col, long flags = 0, unsigned long property = 0)
```

## Parameters
### x, y (in)
The position of the center point.

### xradius, yradius (in)
The horizontal and vertical radius.

### col (in)
The pixel color value.

### flags (in)
Extra [flags](classedisplay_definitions.md) for drawing effects.

### property (in)
A property to be used in combination with the flag parameter.

## Return value
None.

## Description
Draw a filled oval or circle.

## Version changes
- EDGELIB 2.30: Changed the property parameter to unsigned long

