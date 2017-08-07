:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Size


```c++
unsigned long Size(const char *filename, unsigned char location = EFOF_DEFAULT) 
unsigned long Size(const WCHAR *filename = NULL, unsigned char location = EFOF_DEFAULT)
```

## Parameters
### *filename (in)
The name of the file to check the size of.

### location (in)
Select the [location](classefile_definitions.md) from where to open this file. Can be either EFOF_DEFAULT or EFOF_READONLYPATH.

## Return value
The size of the file.

## Description
Return the size of a file. If a filename is `NULL` or not specified, it calculates the size of the currently opened file.

## Version changes
EDGELIB 3.987: Added location argument 
- EDGELIB 3.10: Added const keyword for parameters that aren't modified, renamed SizeC to Size 
- EDGELIB 2.90: Added Unicode overloaded function, renamed non-unicode version to SizeC

