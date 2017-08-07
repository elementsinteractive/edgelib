:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# MapScreenWipe


```c++
void MapScreenWipe(unsigned char *dst, unsigned long width, unsigned long height, unsigned char type)
```

## Parameters
### *dst (out)
A pointer to the destination screen wipe displacement map.

### width, height (in)
The size in pixels for the displacement map.

### type (in)
The [type](classedisplay_definitions.md) of screenwipe to create.

## Return value
None.

## Description
This function is used to create a displacement map for screenwipes, a wipe that takes up an entire surface. The destination buffer needs to hold enough memory to contain each pixel.

