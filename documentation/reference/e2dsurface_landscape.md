:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Landscape


```c++
void Landscape(RECT *viewport, long pitch, long mapx, long mapy, unsigned short turn, short horizon, unsigned short scale, E2DSurface *source)
```

## Parameters
### *viewport (in)
A pointer to a [rectangle structure](ref_globalstructures.md) with the viewport to draw the landscape in.

### pitch (in)
The extra offset from the bottom of the landscape.

### mapx, mapy (in)
The position on the landscape map.

### turn (in)
The rotation of the landscape.

### horizon (in)
The horizon or height of the landscape.

### scale (in)
The scale or size of the landscape.

### *source (in)
A pointer to a 2D surface containing the landscape.

## Return value
None.

## Description
Draw a 2D landscape within the viewport. This function executes faster for some occasions when the source surface width and height are equal and have a specific size.

Optimized sizes: 8x8, 16x16, 32x32, 64x64, 128x128, 256x256, 512x512 and 1024x1024 pixels.

