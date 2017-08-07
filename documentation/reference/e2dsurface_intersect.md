:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Intersect


```c++
bool Intersect(long x1, long y1, RECT *rc1, long x2, long y2, RECT *rc2, E2DSurface *cmp, POINT *pnt = NULL) 
bool Intersect(long x1, long y1, RECT *rc1, long x2, long y2, RECT *rc2, E2DSurfaceRGBA *cmp, POINT *pnt = NULL)
```

## Parameters
### x1, y1 (in)
The position of the first rectangle to check.

### *rc1 (in)
The first rectangle to check for collision detection.

### x2, y2 (in)
The position of the second rectangle to check.

### *rc2 (in)
The second rectangle to check for collision detection.

### *cmp (in)
A pointer to the 2D surface or [RGBA surface](e2dsurfacergba.md) to check for collision.

### *pnt (out)
Pass this [point](ref_globalstructures.md) parameter to retrieve the position the sprite has been hit.

## Return value
Returns true if a collision has been detected, otherwise return false.

## Description
This function can be used for collision detection. A point can be passed as a parameter to hold the position of the collision. It contains the value x,y = -1,-1 if no collision has been detected. Pass NULL if the collision position isn't of interest, it will increase performance.

