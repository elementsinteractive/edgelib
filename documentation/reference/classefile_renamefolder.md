:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# RenameFolder


```c++
bool RenameFolder(const char *newfoldername, const char *oldfoldername) 
bool RenameFolder(const WCHAR *newfoldername, const WCHAR *oldfoldername)
```

## Parameters
### *newfoldername (in)
The new foldername to change to.

### *oldfoldername (in)
The name of the old folder to rename.

## Return value
Returns true if renaming has been successful, otherwise returns false.

## Description
Rename an existing folder.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function

