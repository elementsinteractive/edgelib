:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# operator[]


```c++
unsigned char &operator[](int i)
```

## Parameters
### i (in)
The index of the data array element to access.

## Return value
A reference to a single data array element.

## Description
Use bracket overloading to simply access single data array elements and modify them. When using EData16 or EData32, the return value changes to unsigned short, or unsigned long.

## Version changes
- EDGELIB 3.30: Added this operator to the EDGELIB SDK

