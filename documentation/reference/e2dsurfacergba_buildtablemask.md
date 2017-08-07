:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# BuildTableMask


```c++
void BuildTableMask(unsigned char opacity = 255)
```

## Parameters
### opacity (in)
The minimum amount of opacity each pixel must have for adding it to the table.

## Return value
None.

## Description
Build a table to use collision detection with pixel precision. Call this function after surface creation and when the surface contains the needed pixel data.

