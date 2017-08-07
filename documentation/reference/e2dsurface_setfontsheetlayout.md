:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetFontSheetLayout


```c++
void SetFontSheetLayout(unsigned char numrows, short rowchars = -1)
```

## Parameters
### numrows (in)
The number of rows with font characters in the surface.

### rowchars (in)
The number of characters on a row for fixed fonts. When set to -1, the font builder attempts to calculate it automatically.

## Return value
None.

## Description
Specify the layout of a font surface if it contains multiple rows. This function must be called before [BuildFont](e2dsurface_buildfont.md) to be effective.

## Version changes
- EDGELIB 3.00: Added this function to the SDK

