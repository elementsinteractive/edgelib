:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# DrawFont


```c++
void DrawFont(long x, long y, E2DSurface *source, const char *text, long flags = EFX_COLORKEY, unsigned long property = 0) 
void DrawFont(long x, long y, E2DSurface *source, const WCHAR *text, long flags = EFX_COLORKEY, unsigned long property = 0) 
void DrawFont(long x, long y, E2DSurface *source, long value, long flags = EFX_COLORKEY, unsigned long property = 0) 
void DrawFont(long x, long y, E2DSurfaceRGBA *source, const char *text, long flags = EFX_COLORKEY, unsigned long property = 0) 
void DrawFont(long x, long y, E2DSurfaceRGBA *source, const WCHAR *text, long flags = EFX_COLORKEY, unsigned long property = 0) 
void DrawFont(long x, long y, E2DSurfaceRGBA *source, long value, long flags = EFX_COLORKEY, unsigned long property = 0)
```

## Parameters
### x, y (in)
The position for drawing the font.

### *source (in)
A pointer to the 2D surface containing the font.

### *text (in)
A string containing text.

### value (in)
An integer value.

### flags (in)
Extra [flags](classedisplay_definitions.md) for drawing effects or font alignment.

### property (in)
A property to be used in combination with the flag parameter.

## Return value
None.

## Description
This function is used to display text with a 2D surface containing a font.

## Version changes
- EDGELIB 3.40: Added overloads for RGBA surface drawing 
- EDGELIB 2.80: Added Unicode overloaded function 
- EDGELIB 2.30: Changed the property parameter to unsigned long 
- EDGELIB 2.10: The ability to combine font alignment flags with the other drawing flags

