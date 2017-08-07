:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# IsFolder


```c++
bool IsFolder(const char *filename, unsigned char location = EFOF_DEFAULT) 
bool IsFolder(const WCHAR *filename, unsigned char location = EFOF_DEFAULT)
```

## Parameters
### *filename (in)
The name of the file to check if it exists and is a folder.

### location (in)
Select the [location](classefile_definitions.md) from where to open this file. Can be either EFOF_DEFAULT or EFOF_READONLYPATH.

## Return value
Returns true if the file is a folder, otherwise returns false.

## Description
Check if a file is a folder, or a normal file.

## Version changes
EDGELIB 3.987: Added location argument 
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function

