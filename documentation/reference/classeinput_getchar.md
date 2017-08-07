:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetChar


```c++
WCHAR GetChar(unsigned long *scancode = NULL)
```

## Parameters
### *scancode (out)
The scancode before it was converted to the character. This parameter is optional.

## Return value
A character from the character buffer or 0 if none are available.

## Description
Returns a character from the character buffer. After calling this function, the character is removed from the buffer.

## Version changes
- EDGELIB 2.70: Added this function to the SDK

