:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetFontWidth


```c++
unsigned long GetFontWidth(char *text) 
unsigned long GetFontWidth(WCHAR *text)
```

## Parameters
### *text (in)
A string containing text.

## Return value
The width of the text written with the font within the surface.

## Description
Use this function to calculate the width when drawing text on another surface. The possible variable width of the font is taken into account.

