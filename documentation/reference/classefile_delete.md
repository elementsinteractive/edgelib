:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Delete


```c++
bool Delete(const char *filename) 
bool Delete(const WCHAR *filename)
```

## Parameters
### *filename (in)
The name of the file to delete.

## Return value
Returns true if deleting has been successful, otherwise returns false.

## Description
Delete an existing file. Files that are currently opened, can't be deleted until they're closed.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function

