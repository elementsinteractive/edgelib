:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# WritePNG


```c++
void WritePNG(char *filename, RECT *rc = NULL) 
void WritePNG(WCHAR *filename, RECT *rc = NULL)
```

## Parameters
### *filename (in)
The name of the file that should be written.

### *rc (in)
A rectangle of the surface to write to the PNG file.

## Return value
None.

## Description
Write a PNG file containing the graphics data of the surface.

## Version changes
- EDGELIB 3.10: Added this function to the SDK

