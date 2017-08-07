:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# UploadTexture


```c++
ERESULT UploadTexture(E2DSurface *texture, bool uploadrgba = false, bool cleardata = EDGE_SAFE_CONTEXT, bool conversion = true, bool autoreload = true) 
ERESULT UploadTexture(E2DSurfaceRGBA *texture, bool cleardata = EDGE_SAFE_CONTEXT, bool autoreload = true)
```

## Parameters
### *texture (in)
A pointer to a [E2DSurface](e2dsurface.md) or [E2DSurfaceRGBA](e2dsurfacergba.md).

### uploadrgba (in)
When set to true, it uploads the texture as RGBA. Colorkeyed pixels will have zero opacity.

### cleardata (in)
When set to true, clears pixel data from the surface after the upload process is complete. The default EDGE_SAFE_CONTEXT is to keep data when the OpenGL context can be lost, and to discard it when it is safe to do so.

### conversion (in)
When set to true, it converts the surface colors (to swap red and blue) and flips the image vertically.

### autoreload (in)
When set to true, OpenGL textures are automatically uploaded again after a context loss.

## Return value
None.

## Description
Uploads a texture for use with OpenGL.

## Optimization
In certain cases, texture uploading can be faster. With normal RGB textures the following texture video types can be uploaded with increased speed: EDSP_TRUE15, EDSP_TRUE16 and EDSP_TRUE24_INV (if the texture is not uploaded as RGBA). For RGBA surfaces, the EDSP_TRUE32_INV videotype is the fastest. The videotype of the texture can be specified in the [CreateSurface](classedisplay_createsurface.md) function. For example, pass EST_DSPTRUE24_INV as the usetype parameter.

## Version changes
- EDGELIB 3.00: Added this function to the SDK

