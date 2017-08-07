:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetFontHeight


```c++
unsigned long GetFontHeight(char *text = NULL)
```

## Parameters
### *text (in)
A string containing text.

## Return value
The height of the text written with the font within the surface.

## Description
Use this function to calculate the height when drawing text onanother surface. The possible variable width of the font is taken into account. If the text parameter is `NULL`, it returns the height of one character.

## Version changes
- EDGELIB 3.40: Added this function to the SDK

