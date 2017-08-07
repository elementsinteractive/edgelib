:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# DrawFontWrap


```c++
void DrawFontWrap(long x, long y, E2DSurface *source, const char *text, unsigned long width, long flags = EFX_COLORKEY) 
void DrawFontWrap(long x, long y, E2DSurface *source, const WCHAR *text, unsigned long width, long flags = EFX_COLORKEY)
```

## Parameters
### x, y (in)
The position for drawing the font.

### *source (in)
A pointer to the 2D surface containing the font.

### *text (in)
A string containing text.

### width (in)
The text width to wrap around.

### flags (in)
Extra [flags](classedisplay_definitions.md) for font alignment.

## Return value
None.

## Description
This function is used to display text with a 2D surface containing a font. Text will be wrapped to newlines if the text width exceeds the maximum width specified by the parameter.

## Version changes
- EDGELIB 4.00: Added this function to the SDK

