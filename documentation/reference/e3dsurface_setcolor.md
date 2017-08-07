:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetColor


```c++
void SetColor(unsigned long index, COLORREF color)
```

## Parameters
### index (in)
The vertex to colorize.

### color (in)
Defines the color to use.

## Return value
None.

## Description
Color your 3D surface vertices by using this function.

## Version changes
- EDGELIB 3.00: Renamed the overloaded function to turn the entire model into one color to [E3DSurface::SetModelColor](e3dsurface_setmodelcolor.md), colors are now defined per vertex

