:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Lock


```c++
unsigned char *Lock(E2DSURFACEINFO *surfaceinfo = NULL)
```

## Parameters
### *surfaceinfo (out)
A pointer to a [2D surface information structure](classedisplay_structures.md). If this is not NULL, it will be filled with detailed surface information.

## Return value
A pointer to the memory data of the locked surface.

## Description
The memory within the surface is locked and can be accessed by external functions. Always unlock the surface as soon as possible after that. In the E2DSURFACEINFO structure, the bit depth is always 4. The highest 8 bits contain the alpha channel and the lower bits (depending on the color depth) contain the rgb channel.

## Version changes
- EDGELIB 2.20: Added a parameter for retrieving surface information.

