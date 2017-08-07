:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# BuildFont


```c++
void BuildFont(bool variablewidth = false, long autoreorder = EFR_DEFAULT)
```

## Parameters
### variablewidth (in)
When set to true, the font will have characters with variable width which is determined by a non-colorkeyed line above each symbol in the surface.

### autoreorder (in)
Additional [link=classedisplay_definitions]font reordering flags[/link] to select when the resource should be modified for characters to fit into an OpenGL texture.

## Return value
None.

## Description
Turns the surface into a font surface. Use [SetFontCharacters](e2dsurfacergba_setfontcharacters.md), [SetFontCharacterSet](e2dsurfacergba_setfontcharacterset.md) and [SetFontSheetLayout](e2dsurfacergba_setfontsheetlayout.md) to determine properties of the font surface before calling this function.

## Version changes
- EDGELIB 4.00: Added the autoreorder argument 
- EDGELIB 3.40: Added this function to the SDK

