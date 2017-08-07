:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# ReadFolder


```c++
bool ReadFolder(char *filename) 
bool ReadFolder(WCHAR *filename)
```

## Parameters
### *filename (out)
A pointer to a string to contain a filename.

## Return value
Return true if there are more files to search, otherwise return false.

## Description
Read filenames from a folder. Call this function multiple times until it returns false.

## Version changes
- EDGELIB 2.90: Added Unicode overloaded function

