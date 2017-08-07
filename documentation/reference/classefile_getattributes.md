:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetAttributes


```c++
unsigned short GetAttributes(const char *filename, unsigned char location = EFOF_DEFAULT) 
unsigned short GetAttributes(const WCHAR *filename = NULL, unsigned char location = EFOF_DEFAULT)
```

## Parameters
### *filename (in)
The name of the file or folder to retrieve attributes from.

### location (in)
Select the [location](classefile_definitions.md) from where to open this file. Can be either EFOF_DEFAULT or EFOF_READONLYPATH.

## Return value
The file or folder's [attribute flags](classefile_definitions.md). Returns `EFAF_NOTAVAILABLE` when the function has failed.

## Description
Gets the attributes of a file or folder. If the filename isn't specified or `NULL`, it will read the attributes of the current open file.

## Version changes
EDGELIB 3.987: Added location argument 
- EDGELIB 3.10: Added const keyword for parameters that aren't modified, renamed GetAttributesC to GetAttributes 
- EDGELIB 2.90: Added Unicode overloaded function, renamed non-Unicode version to GetAttributesC

