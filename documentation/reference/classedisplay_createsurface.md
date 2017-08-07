:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# CreateSurface


```c++
ERESULT CreateSurface(E2DSurface *surface, unsigned long width, unsigned long height, unsigned long usetype = EST_DEFAULT) 
ERESULT CreateSurface(E2DSurface *surface, void *imgmem, unsigned long memsize, unsigned long usetype = EST_DEFAULT) 
ERESULT CreateSurface(E2DSurface *surface, char *imgfile, unsigned long usetype = EST_DEFAULT) 
ERESULT CreateSurface(E2DSurface *surface, WCHAR *imgfile, unsigned long usetype = EST_DEFAULT) 
ERESULT CreateSurface(E3DSurface *surface, unsigned long vertices, unsigned long polygons, unsigned long usetype = EST_DEFAULT) 
ERESULT CreateSurface(E3DSurface *surface, void *mdlmem, unsigned long memsize, signed char modelsize = 0, unsigned long usetype = EST_DEFAULT) 
ERESULT CreateSurface(E3DSurface *surface, char *mdlfile, signed char modelsize = 0, unsigned long usetype = EST_DEFAULT) 
ERESULT CreateSurface(E3DSurface *surface, WCHAR *mdlfile, signed char modelsize = 0, unsigned long usetype = EST_DEFAULT) 
ERESULT CreateSurface(E2DSurfaceRGBA *surface, unsigned long width, unsigned long height, unsigned long usetype = EST_DEFAULT) 
ERESULT CreateSurface(E2DSurfaceRGBA *surface, void *imgmem, unsigned long memsize, unsigned long usetype = EST_DEFAULT) 
ERESULT CreateSurface(E2DSurfaceRGBA *surface, char *imgfile, unsigned long usetype = EST_DEFAULT) 
ERESULT CreateSurface(E2DSurfaceRGBA *surface, WCHAR *imgfile, unsigned long usetype = EST_DEFAULT)
```

## Parameters
### *surface (in)
A pointer to the [2D surface](e2dsurface.md), [3D surface](e3dsurface.md) or [RGBA surface](e2dsurfacergba.md) that has to be created.

### width, height (in)
The size in pixels for 2D surfaces.

### vertices, polygons (in)
The number of vertices (3D pixels) and polygons (3D triangles) for 3D surfaces.

### *imgmem (in)
A pointer to image memory containing 2D file data. 
Supported file types:

* Uncompressed Windows Bitmap (BMP)
* Graphics Interchange Format (GIF)
* Portable Network Graphics (PNG)
* Joint Photographic Experts Group (JPEG)
* Targa (TGA)

### *mdlmem (in)
A pointer to image memory containing 3D file data. 
Supported file types:

* 3D Studio (.3ds)
* MilkShape 3D (.ms3d)

### memsize (in)
The size of the memory allocated by the memory pointer.

### *imgfile (in)
The filename of a 2D image file. 
Supported file types:

* Uncompressed Windows Bitmap (BMP)
* Graphics Interchange Format (GIF)
* Portable Network Graphics (PNG)
* Joint Photographic Experts Group (JPEG)
* Targa (TGA)

### *mdlfile (in)
The filename of a 3D model file. 
Supported file types:

* 3D Studio (.3ds)
* MilkShape 3D (.ms3d)

### modelsize (in)
Adjusts model size while loading. Values smaller than 0 makes the model smaller, while values bigger than 0 makes it bigger.

### usetype (in)
Additional [surface create flags](classedisplay_definitions.md) that can be passed through the surface creator.

## Return value
`E_OK`: the surface creation was successful. 
`E_INVALIDPARAM`: the usetype parameter contains an unknown flag. 
`E_UNSUPPORTED`: one of the requested features is not supported by the hardware or software. 
`E_NOMEMORY`: there is not enough available memory to create this surface. 
`E_ERROR`: a general error that doesn't fit in other categories.

## Description
This function is used to create a new surface using a variety of methods.

## Version changes
- EDGELIB 3.00: Added extra parameters for 3D surface creation and support for Milkshape 3D 
- EDGELIB 2.90: Added Unicode overloaded functions 
- EDGELIB 2.80: Added support for loading monochrome and 4-bit bitmap loading 
- EDGELIB 2.30: Changed the usetype parameter datatype to unsigned long 
- EDGELIB 2.20: Changed the default usetype parameter to EST_DEFAULT

