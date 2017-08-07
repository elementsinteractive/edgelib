:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# MatrixMultiply


```c++
void static MatrixMultiply(long *dst, const long *src)
```

## Parameters
### dst (in/out)
The first matrix and stores the multiplied matrix.

### fixedprecision (in)
The second matrix to multiply with.

## Return value
None.

## Description
Multiplies 2 4x4 matrices and stores the result in the first parameter. Matrixes contain 20:12 fixed point values.

## Version changes
- EDGELIB 3.00: Added this function to the SDK

