:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetPalette


```c++
void SetPalette(COLORREF *palette, unsigned char index = 0, unsigned long entries = 1)
```

## Parameters
### *palette (in)
A pointer to a single palette color or an array of colors.

### index (in)
The palette index to modify.

### entries (in)
The amount of palette colors to set.

## Return value
None.

## Description
Change the display palette. This applies to native or emulated indexed video modes.

## Version changes
- EDGELIB 2.90: Added this function to the SDK

