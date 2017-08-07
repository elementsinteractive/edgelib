:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetVertex


```c++
void SetVertex(unsigned long index, long x, long y, long z) 
void SetVertex(unsigned long index, EVERTEX3D *vertex)
```

## Parameters
### index (in)
The specified 3D vertex point.

### x, y, z (in)
The position of the vertex point.

### *vertex (in)
A [vertex](classedisplay_structures.md) structure containing detailed vertex information.

## Return value
None.

## Description
Define vertex points manually.

## Version changes
- EDGELIB 3.00: Added an overloaded function for setting multiple vertex parameters at once

