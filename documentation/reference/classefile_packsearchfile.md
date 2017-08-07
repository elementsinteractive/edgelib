:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PackSearchFile


```c++
bool PackSearchFile(const char *filename, unsigned long &id) 
bool PackSearchFile(const WCHAR *filename, unsigned long &id)
```

## Parameters
### *filename (in)
The name of the file to search for.

### &id (out)
The ID of the file when it is found.

## Return value
Return true if the filename is found, otherwise return `false`.

## Description
Search for a filename within a pack to retrieve its ID.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function

