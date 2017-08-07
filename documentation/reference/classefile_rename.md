:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Rename


```c++
bool Rename(const char *newfilename, const char *oldfilename) 
bool Rename(const WCHAR *newfilename, const WCHAR *oldfilename)
```

## Parameters
### *newfilename (in)
The new filename to change to.

### *oldfilename (in)
The name of the old file to rename.

## Return value
Returns true if renaming has been successful, otherwise returns false.

## Description
Rename an existing file.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function

