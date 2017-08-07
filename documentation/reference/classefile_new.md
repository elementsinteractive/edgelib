:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# New


```c++
bool New(const char *filename) 
bool New(const WCHAR *filename)
```

## Parameters
### *filename (in)
The name of the file to create.

## Return value
Return true if the file has been successfully created, otherwise return false.

## Description
Create a new file. When the file already exists, rewrite it.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function

