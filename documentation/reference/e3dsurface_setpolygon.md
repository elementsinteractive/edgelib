:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetPolygon


```c++
void SetPolygon(unsigned long index, unsigned long v1, unsigned long v2, unsigned long v3) 
void SetPolygon(unsigned long index, EVERTEX3D *v1, EVERTEX3D *v2, EVERTEX3D *v3, bool checkduplicates = false)
```

## Parameters
### index (in)
The specified polygon triangle.

### v1, v2, v3 (in)
Vertex indices or [vertex structures](classedisplay_structures.md) to define the polygon.

### checkduplicates (in)
Check for duplicate vertices to optimize the model.

## Return value
None.

## Description
Define polygons manually. Make sure the vertices are assigned counter-clockwise. It's also possible to pass vertex points directly instead of setting vertices first and linking their indices. When checking for duplicates, it will compare if existing vertices already share the given structure properties and use those instead of creating new vertices.

## Version changes
- EDGELIB 3.00: Removed the color parameter, changed the overloaded function for specifying vertices directly, vertices should be assigned counter-clockwise 
- EDGELIB 2.50: Added a new overloaded function

