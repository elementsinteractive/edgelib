:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetClipper


```c++
void SetClipper(RECT *rc = NULL)
```

## Parameters
### *rc (in)
A pointer to a [rectangle structure](ref_globalstructures.md) containing the rectangle to clip.

## Return value
None.

## Description
Use clippers to write only within a designated rectangle area of the surface. The whole surface is clipped by default. Clippers can't be bigger than the actual surface size.

