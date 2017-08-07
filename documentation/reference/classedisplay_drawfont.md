:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# DrawFont


```c++
void DrawFont(long x, long y, E2DSurface *source, const char *text, long flags = 0) 
void DrawFont(long x, long y, E2DSurface *source, const WCHAR *text, long flags = 0) 
void DrawFont(long x, long y, E2DSurface *source, long value, long flags = 0) 
void DrawFont(long x, long y, E2DSurfaceRGBA *source, const char *text, long flags = 0) 
void DrawFont(long x, long y, E2DSurfaceRGBA *source, const WCHAR *text, long flags = 0) 
void DrawFont(long x, long y, E2DSurfaceRGBA *source, long value, long flags = 0)
```

## Parameters
### x, y (in)
The position to draw the text

### *source (in)
The font surface to draw with

### *text (in)
A pointer to a string to draw

### value (in)
The value to draw

### flags (in)
Extra [flags](classedisplay_definitions.md) for font alignment

## Return value
None.

## Description
Draw text using a font to the active display. This can be the backbuffer or OpenGL.

## Version changes
- EDGELIB 3.60: Added overloaded function for RGBA surfaces 
- EDGELIB 3.00: Added this function to the SDK

