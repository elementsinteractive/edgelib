:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# ReplaceColor


```c++
bool ReplaceColor(COLORREF dst, COLORREF src, bool exclude = false)
```

## Parameters
### dst (in)
The color to change into.

### src (in)
The selected color for replacing.

### exclude (in)
If set to false, the color defined as the source color will be changed into the destination color. If set to true, all colors will be changed, except the color defined by source color.

## Return value
None.

## Description
Quickly change colors in a surface.

## Version changes
- EDGELIB 2.20: Added this function to the SDK

