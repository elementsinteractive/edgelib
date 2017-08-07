:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Perspective2D


```c++
void Perspective2D(long width, long height, bool ypointsup = true)
```

## Parameters
### width, height (in)
The width and height of the viewport

### ypointsup (in)
When enabled, use real-life y-axis coordinates

## Return value
None.

## Description
Changes the perspective to orthographics view in OpenGL. Useful for blitting GUI, fonts and other 2D elements.

## Version changes
- EDGELIB 3.00: Added this function to the SDK

