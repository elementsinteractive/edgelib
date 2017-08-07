:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# BltFast


```c++
ERESULT BltFast(long x, long y, E2DSurface *source, RECT *rc, long flags = 0, unsigned long property = 0) 
ERESULT BltFast(long x, long y, E2DSurfaceRGBA *source, RECT *rc, long flags = 0, unsigned long property = 0)
```

## Parameters
### x, y (in)
The position of the destination to blit.

### *source (in)
A pointer to a 2D surface or [RGBA surface](e2dsurfacergba.md) to blit from.

### *rc (in)
A pointer to a [rectangle structure](ref_globalstructures.md) with the shape and position of the rectangle.

### flags (in)
Extra [surface blit flags](classedisplay_definitions.md) for drawing effects.

### property (in)
A property to be used in combination with the flag parameter.

## Return value
`E_OK`: the blit has been successful. 
`E_NOACTION`: no pixels were copied, for example if the full rectangle was clipped. 
`E_TYPEMISMATCH`: an internal error, datatypes didn't match. 
`E_LOCKFAILED`: the surfaces couldn't be locked. 
`E_INVALIDPARAM`: one of the parameters was invalid.

## Description
Blit a rectangle of pixels from one surface to another. When combining color effect filters and opacity, specify the property as: E_RGBA(red, green, blue, opacity).

## Version changes
- EDGELIB 2.30: Changed the property parameter to unsigned long 
- EDGELIB 2.10: Added a return value

