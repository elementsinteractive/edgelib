:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetFontCharacterSet


```c++
void SetFontCharacterSet(char *set) 
void SetFontCharacterSet(WCHAR *set)
```

## Parameters
### *set (in)
A set of characters matching the surface font layout. Set this to NULL if the set should be removed.

## Return value
None.

## Description
Map a range of characters to a font surface if the font layout isn't ASCII oriented. This is an alternative to [SetFontCharacters](e2dsurface_setfontcharacters.md). When using this function, font surface operations have a little performance decrease. This function must be called before [BuildFont](e2dsurface_buildfont.md) to be effective.

## Version changes
- EDGELIB 2.90: Added this function to the SDK

