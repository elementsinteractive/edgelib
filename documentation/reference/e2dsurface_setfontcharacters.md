:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetFontCharacters


```c++
void SetFontCharacters(WCHAR start = 32, WCHAR end = 127)
```

## Parameters
### start (in)
Indicates the first font character on the surface (usually 0, or 32 = space).

### end (in)
Indicates the last font character on the surface (usually 127).

## Return value
None.

## Description
Use this function to map a range of font characters (for example, it starts with 32 when the first 32 characters aren't used). By default a surfaces uses character 0 to 127. This function must be called before [BuildFont](e2dsurface_buildfont.md) to be effective.

## Version changes
- EDGELIB 3.10: Set default parameters 
- EDGELIB 2.80: Added this function to the SDK

