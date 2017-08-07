:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# BindTexture


```c++
void BindTexture(E2DSurface *texture) 
void BindTexture(E2DSurfaceRGBA *texture)
```

## Parameters
### *texture (in)
A surface which contains the texture image.

## Return value
None.

## Description
Binds a texture surface for 3D rendering. When using OpenGL, upload the texture first.

## Version changes
- EDGELIB 3.00: Added this function to the SDK

