:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# FixTexCoords


```c++
void FixTexCoords(E2DSurface *paddedtexture) 
void FixTexCoords(E2DSurfaceRGBA *paddedtexture)
```

## Parameters
### paddedtexture
The surface which will be used as a texture for this 3D surface.

## Return value
None.

## Description
When using textures with a size which is not a power of 2 (for example: 128, 256 or 512 pixels). You need to call this function to fix the texture coordinates. Currently this is only needed when using OpenGL.

