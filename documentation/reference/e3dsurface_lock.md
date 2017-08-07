:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Lock


```c++
bool Lock(E3DSURFACEINFO *surfaceinfo, bool render = false)
```

## Parameters
### *surfaceinfo (out)
A pointer to an [E3DSURFACEINFO](classedisplay_structures.md) structure containing surface data.

### render (in)
Specifies if the data should be suitable for rendering. Render data should be treated as readonly.

## Return value
Returns true if the surface is successfully locked.

## Description
Lock the surface to access its data directly. Don't forget to [unlock](e3dsurface_unlock.md) it when done.

## Version changes
- EDGELIB 3.00: Added this function to the SDK

