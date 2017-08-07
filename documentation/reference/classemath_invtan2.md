:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# InvTan2


```c++
unsigned long InvTan2(long xdelta, long ydelta)
```

## Parameters
### xdelta, ydelta (in)
The distance between two reference points.

## Return value
The calculated inverted tangent value. A whole circle is represented as a value between 0 and 4095.

## Description
This function can be used to calculate the direction angle based on object distances. Like Sin and Cos, the y-axis points upwards (real world coordinates) instead of downwards (screen coordinates).

## Version changes
- EDGELIB 3.50: Added this function to the SDK

