:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# StrToUtf8


```c++
void static StrToUtf8(char *dst, const WCHAR *src, long maxlen = -1)
```

## Parameters
### *dst (out)
The destination string to copy to.

### *src (in)
The string to copy.

### maxlen (in)
Defines the maximum amount of characters to copy into the new string.

## Return value
None.

## Description
Copies a Unicode wide character string and converts to a multibyte UTF8 string.

## Version changes
- EDGELIB 3.10: Added this function to the SDK

