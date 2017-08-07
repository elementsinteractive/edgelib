:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetParseBufferSize


```c++
bool SetParseBufferSize(unsigned long newsize)
```

## Parameters
### newsize (in)
The new size of the buffer.

## Return value
Return true if the function succeeded or return false if the INI file isn't open or in case of a fatal error.

## Description
Changes the size of the INI parse buffer. By default it takes 4096 bytes.

## Version changes
- EDGELIB 2.80: Added this function to the SDK

