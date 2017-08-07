:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# NewFolder


```c++
bool NewFolder(const char *foldername) 
bool NewFolder(const WCHAR *foldername)
```

## Parameters
### *foldername (in)
The name of the folder to create.

## Return value
Returns true if the folder has been created, otherwise returns false.

## Description
Create new folders with this function.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function

