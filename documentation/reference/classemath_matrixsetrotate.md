:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# MatrixSetRotate


```c++
void MatrixSetRotate(unsigned short x, unsigned short y, unsigned short z, long *matrix)
```

## Parameters
### x, y, z (in)
The amount of rotation, a value between 0 and 4096.

### matrix (in/out)
The matrix to set.

## Return value
None.

## Description
Set rotation to the specified matrix. Matrixes contain 20:12 fixed point values.

## Version changes
- EDGELIB 3.00: Added this function to the SDK

