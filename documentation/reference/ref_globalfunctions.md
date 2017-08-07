:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Global functions

## SetRect
```c++
void SetRect(RECT *lprc, int xLeft, int yTop, int xRight, int yBottom)
```

Quickly set a rectangle structure.

## RectIntersect
```c++
bool RectIntersect(RECT *rc1, RECT *rc2, RECT *result = NULL)
```

Check if two rectangles intersect, if the third parameter isn't NULL it will contain the bounding box of the intersection.

## InRect
```c++
bool InRect(RECT *rc, POINT *pnt)
```

Returns true if a point is located inside the given rectangle.

## Version changes
- EDGELIB 2.60: Added RectIntersect, InRect

