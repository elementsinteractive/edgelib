:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# UseLight


```c++
void UseLight(unsigned char nr, bool enable = true)
```

## Parameters
### nr (in)
The light source index.

### enable (in)
Set to true to enable the light source, otherwise disable it.

## Return value
None.

## Description
Enable or disable light sources for 3D rendering. The EDGELIB internal renderer supports up to eight light sources. For OpenGL this depends on the implementation.

## Version changes
- EDGELIB 3.00: Added this function to the SDK

