:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetLandscapePoint


```c++
bool GetLandscapePoint(RECT *viewport, long pitch, long mapx, long mapy, unsigned short turn, short horizon, unsigned short scale, long objx, long objy, E2DSurface *source, EVECTOR3D *vector)
```

## Parameters
### *viewport (in)
A pointer to a [rectangle structure](ref_globalstructures.md) with the viewport to draw the landscape in.

### pitch (in)
The extra offset from the bottom of the landscape.

### mapx, mapy (in)
The x, y position on the landscape map.

### turn (in)
The rotation of the landscape.

### horizon (in)
The horizon or height of the landscape.

### scale (in)
The scale or size of the landscape.

### objx, objy (in)
The x, y position of the requested point on the landscape map.

### *source (in)
A pointer to a 2D surface containing the landscape.

### *vector (in)
A pointer to a vector containing the converted position for display.

## Return value
Returns true if the point is in sight, otherwise returns false.

## Description
Use this function to determine points on the landscape map when using the [Landscape function](e2dsurface_landscape.md) and convert them to 2D display coordinates. Useful for sprite objects on the landscape.

