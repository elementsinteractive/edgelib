:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetLightSource


```c++
void SetLightSource(unsigned char nr, long x, long y, long z, COLORREF diffuse = ECOLOR_BLACK, COLORREF ambient = ECOLOR_BLACK, COLORREF specular = ECOLOR_BLACK)
```

## Parameters
### x, y, z (in)
Defines the position of the light source.

### diffuse (in)
The color for diffuse lighting.

### ambient (in)
The color for ambient lighting.

### specular (in)
The color for specular lighting.

## Return value
None.

## Description
Configure lightsources, specular lighting isn't supported in the EDGELIB internal renderer. Don't forget to [enable](classedisplay_uselight.md) the light source.

## Version changes
- EDGELIB 3.00: Added this function to the SDK

