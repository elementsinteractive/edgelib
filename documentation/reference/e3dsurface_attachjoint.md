:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# AttachJoint


```c++
void AttachJoint(unsigned long index, unsigned short *vertexlist, unsigned short listsize)
```

## Parameters
### index (in)
The index of the joint to attach vertices to.

### *vertexlist (in)
A list of vertex references to attach.

### listsize (in)
The amount of vertices in the list.

## Return value
None.

## Description
Attach vertices to joints. If a list already exists, it will be appended.

## Version changes
- EDGELIB 3.00: Added this function to the SDK

