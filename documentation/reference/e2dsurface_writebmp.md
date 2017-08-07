:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# WriteBMP


```c++
void WriteBMP(char *filename, RECT *rc = NULL) 
void WriteBMP(WCHAR *filename, RECT *rc = NULL)
```

## Parameters
### *filename (in)
The name of the file that should be written.

### *rc (in)
A rectangle of the surface to write to the bitmap file.

## Return value
None.

## Description
Write a 24-bit bitmap file containing the graphics data of the surface.

## Version changes
- EDGELIB 3.10: Renamed function name "WriteBmp" to "WriteBMP" 
- EDGELIB 2.90: Added a unicode overloaded function 
- EDGELIB 2.50: Added optional parameter *rc and increased performance of writing the surface

