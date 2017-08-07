:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Get


```c++
void *Get(unsigned long index = 0)
```

## Parameters
### index (in)
The index to add to the pointer returned by the function.

## Return value
A pointer to the data array inside the class.

## Description
This function returns a pointer to the data array inside the class. The index behaves differently depending on the size of the class. For example, when using EData16, the index will be specified in words (2 bytes) instead of single bytes.

## Version changes
- EDGELIB 3.30: Added this function to the EDGELIB SDK

