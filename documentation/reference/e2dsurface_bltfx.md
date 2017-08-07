:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# BltFx


```c++
ERESULT BltFx(long center_x, long center_y, E2DSurface *source, RECT *rc, E2DBLTFX *bltfx)
```

## Parameters
### center_x, center_y (in)
The position of the destination to blit, the center of the source surface rectangle.

### *source (in)
A pointer to a 2D surface to blit from.

### *rc (in)
A pointer to a [rectangle structure](ref_globalstructures.md) with the shape and position of the rectangle.

### *bltfx (in)
A pointer to a [blit effect structure](classedisplay_structures.md) containing blit properties.

## Return value
`E_OK`: the blit has been successful. 
`E_NOACTION`: no pixels were copied, for example if the full rectangle was clipped. 
`E_TYPEMISMATCH`: an internal error, datatypes didn't match. 
`E_LOCKFAILED`: the surfaces couldn't be locked. 
`E_INVALIDPARAM`: one of the parameters was invalid.

## Description
Blit a rectangle of pixels from one surface to another. When combining color effect filters and opacity, specify the property as: E_RGBA(red, green, blue, opacity). Use this method for advanced blit effects.

## Note
Please note that center_x and center_y values represent the _center_ of the source surface rectangle, not the top left position!

## Version changes
- EDGELIB 3.00: Changed x and y parameter names into center_x and center_y 
- EDGELIB 2.40: Added this function to the SDK

