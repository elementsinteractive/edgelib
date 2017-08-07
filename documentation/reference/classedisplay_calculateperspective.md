:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# CalculatePerspective


```c++
void CalculatePerspective(EVECTOR3D *vector, POINT *point)
```

## Parameters
### *vector (in)
A pointer to a [vector](classedisplay_structures.md) for calculating the perspective.

### *point (out)
The destination [point](ref_globalstructures.md) that will contain the converted vector in 2D space.

## Return value
None.

## Description
Calculate the perspective to convert a 3D point in space to a 2D point for display. It isn't recommended to use this function when using OpenGL.

## Version changes
- EDGELIB 3.00: Added this function to the EDGELIB SDK

