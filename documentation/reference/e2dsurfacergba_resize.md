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
This function shouldn't be used to resize surfaces real-time (i.e. in the game loop).

## See also
[Resize](e2dsurface_resize.md) (E2DSurface)

## Version changes
- EDGELIB 3.10: Added this function to the SDK

