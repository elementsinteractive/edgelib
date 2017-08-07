:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Blt


```c++
ERESULT Blt(long center_x, long center_y, E2DSurface *source, RECT *rc, unsigned short rotation, unsigned long scale, unsigned char flipflags = EFLIP_NONE, long flags = 0, unsigned long property = 0)
```

## Parameters
### center_x, center_y (in)
The position of the destination to blit, the center of the source surface rectangle.

### *source (in)
A pointer to a 2D surface to blit from.

### *rc (in)
A pointer to a [rectangle structure](ref_globalstructures.md) with the shape and position of the rectangle.

### rotation (in)
Used to rotate blitting, a value between 0 and 4096 represents a whole circle.

### scale (in)
Used for scale blitting. Values bigger than 65536 will enlarge the image, values smaller than 65536 will shrink the image.

### flipflags (in)
Extra [flags](classedisplay_definitions.md) to flip the image.

### flags (in)
Extra [flags](classedisplay_definitions.md) for drawing effects.

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

## Note
Please note that center_x and center_y values represent the _center_ of the source surface rectangle, not the top left position!

## Version changes
- EDGELIB 3.00: Changed x and y parameter names into center_x and center_y 
- EDGELIB 2.30: Changed the property parameter to unsigned long 
- EDGELIB 2.10: Added a return value

