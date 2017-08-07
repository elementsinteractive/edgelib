:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PrepareFontWrap


```c++
void PrepareFontWrap(char *dst, char *src, unsigned long width) 
void PrepareFontWrap(WCHAR *dst, WCHAR *src, unsigned long width)
```

## Parameters
### *dst (out)
The destination string containing formatted text.

### *src (in)
The source string containing unformatted text.

### width (in)
The maximum line width in pixels.

## Return value
None.

## Description
Prepare text and wrap with newlines when the text exceeds the given width.

## Version changes
- EDGELIB 3.40: Added this function to the SDK

