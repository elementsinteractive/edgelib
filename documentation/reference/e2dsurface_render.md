:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Render


```c++
void Render(long x, long y, E3DSurface *source, unsigned long flags = E3D_FLAT, unsigned long property = 0)
```

## Parameters
### x, y (in)
The position of the destination to render.

### *source (in)
A pointer to a [3D surface](e3dsurface.md) containing a 3D model.

### flags (in)
Extra [flags](classedisplay_definitions.md) for shading and texture mapping. Regular [blit flags](classedisplay_definitions.md)can also be used in some cases.

### property (in)
An effect property when the flags parameter contain blit flags.

## Return value
None.

## Description
Render a 3D model on the surface. When combining color effect filters and opacity, specify the property as: E_RGBA(red, green, blue, opacity).

## Version changes
- EDGELIB 2.30: Changed the property parameter to unsigned long

