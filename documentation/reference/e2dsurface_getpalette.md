:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetPalette


```c++
void GetPalette(COLORREF *palettedata, unsigned char index = 0, unsigned long entries = 1)
```

## Parameters
### *palettedata (out)
A pointer to a single palette color or an array of colors.

### index (in)
The palette index to get.

### entries (in)
The amount of palette colors to get.

## Return value
None.

## Description
Retrieve a palette if the surface was loaded from an image with palette.

## Version changes
- EDGELIB 2.90: Added this function to the SDK

