:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# DrawLine


```c++
void DrawLine(long x1, long y1, long x2, long y2, COLORREF col, long flags = 0, unsigned long property = 0)
```

## Parameters
### x1, y1 (in)
The position of the first endpoint of the line.

### x2, y2 (in)
The position of the second endpoint of the line.

### col (in)
The pixel color value.

### flags (in)
Extra [flags](classedisplay_definitions.md) for drawing effects.

### property (in)
A property to be used in combination with the flag parameter.

## Return value
None.

## Description
Draw a line between 2 points.

## Version changes
- EDGELIB 2.30: Changed the property parameter to unsigned long

