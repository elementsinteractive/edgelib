:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Open


```c++
bool Open(const char *filename, unsigned char opentype = EFOF_DEFAULT) 
bool Open(const WCHAR *filename, unsigned char opentype = EFOF_DEFAULT)
```

## Parameters
### *filename (in)
The name of the file to open.

### opentype (in)
Select the [mode](classefile_definitions.md) to open this file.

## Return value
Return true if the file exists and has been successfully opened, otherwise return false.

## Description
Open a file. When the file isn't needed anymore, [close](classefile_close.md) it.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added a Unicode overloaded function

