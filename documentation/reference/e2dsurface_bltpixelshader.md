:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# BltPixelShader


```c++
ERESULT BltPixelShader(long x, long y, E2DSurface *source, RECT *rc, EPIXELSHADER, long flags = 0, unsigned long property = 0)
```

## Parameters
### x, y (in)
The position of the destination to blit.

### *source (in)
A pointer to a 2D surface to blit from.

### *rc (in)
A pointer to a [rectangle structure](ref_globalstructures.md) with the shape and position of the rectangle.

### EPIXELSHADER (in)
A function to pass as a custom pixel shader.

### flags (in)
An extra flag that can contain the `EFX_COLORKEY`. Enables or disables the colorkey check while blitting.

### property (in)
A property to pass to the pixel shader.

## Return value
`E_OK`: the blit has been successful. 
`E_NOACTION`: no pixels were copied, for example if the full rectangle was clipped. 
`E_TYPEMISMATCH`: an internal error, datatypes didn't match. 
`E_LOCKFAILED`: the surfaces couldn't be locked. 
`E_INVALIDPARAM`: one of the parameters was invalid.

## Description
Blit a rectangle of pixels from one surface to another using a custom pixel shader. A custom pixel shader is a function to be called every pixel. Four pixel shaders are included in EDGELIB: EPShader_AND, EPShader_OR, EPShader_XOR, EPShader_Grayscale. Below is an example on how to write a custom pixel shader:

```c++
//A custom pixel shader 
unsigned long EPShader_New(unsigned long dstpixel, unsigned long srcpixel, unsigned long x, unsigned long y, long param) 
{ 
    return(dstpixel + srcpixel); 
}
```

## Version changes
- EDGELIB 2.30: Changed the property parameter to unsigned long 
- EDGELIB 2.10: Added a return value

