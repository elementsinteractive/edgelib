:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Resize


```c++
void Resize(const unsigned long width, const unsigned long height = 0, bool usefilter = true)
```

## Parameters
### width (in)
The width for the resized surface.

### height (in)
The height for the resized surface, if it's 0 then the image is resized to keep the correct aspect ratio.

### usefilter (in)
When enabled (default behavior) it uses a bilinear (resampling) filter to scale down the surface. It doesn't work well when upscaling (enlarging).

## Return value
None.

## Description
Resize a surface to a new size using resampling. This can be used to downscale graphics assets for different resolutions, for example title screen images. Usually you resize a surface once and store the result, opposed to resizing it every frame.

## Note
If you want to resize images at real-time (for example scaling a sprite in the game loop), we recommend using [Blt](e2dsurface_blt.md). This high-performance function can be used in your game loop, but will not use a resampling filter.

## See also
[Blt](e2dsurface_blt.md) 
[Resize](e2dsurfacergba_resize.md) (E2DSurfaceRGBA)

## Version changes
- EDGELIB 3.10: Added this function to the SDK

