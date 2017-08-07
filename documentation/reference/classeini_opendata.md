:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OpenData


```c++
bool OpenData(void *ptr, unsigned long memsize)
```

## Parameters
### *ptr (in)
The pointer of the INI to open.

### memsize (in)
The size of the memory.

## Return value
Return true if the memory has been successfully opened, otherwise return false.

## Description
Open an INI file from memory. When the ini isn't needed anymore, [close](classeini_close.md) it.

## Version changes
- EDGELIB 2.70: Added this function to the SDK

