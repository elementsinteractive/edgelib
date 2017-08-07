:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# DrawRect


```c++
void DrawRect(RECT *rc, COLORREF col, long flags = 0, unsigned long property = 0)
```

## Parameters
### *rc (in)
A pointer to a [rectangle structure](ref_globalstructures.md) with the shape and position of the rectangle.

### col (in)
The pixel color value.

### flags (in)
Extra [flags](classedisplay_definitions.md) for drawing effects.

### property (in)
A property to be used in combination with the flag parameter.

## Return value
None.

## Description
Draw a rectangle outline.

## Version changes
- EDGELIB 2.30: Changed the property parameter to unsigned long

