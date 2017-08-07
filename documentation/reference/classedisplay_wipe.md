:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Wipe


```c++
void Wipe(E2DSurface *dst, E2DSurface *src1, E2DSurface *src2, unsigned long timer, unsigned char type, void *customdata = NULL, long property = 0)
```

## Parameters
### *dst (out)
A pointer to the [2D surface](e2dsurface.md) to wipe to.

### *src1, *src2 (in)
A pointer to the 2D surface to wipe from. Some wipes may require 2 source surfaces, while others may not.

### timer (in)
A value between 0 and 65535 determining the state of the wipe.

### type (in)
The [type](classedisplay_definitions.md) of wipe to show.

### *customdata (in)
A pointer to custom data for customizing your wipes.

### property (in)
A property for customizing your wipes.

## Return value
None.

## Description
Call this function every frame to show a wipe between two surfaces.

