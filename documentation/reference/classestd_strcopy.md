:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# StrCpy


```c++
void static StrCpy(char *dst, const char *src, long maxlen = -1) 
void static StrCpy(WCHAR *dst, const char *src, long maxlen = -1) 
void static StrCpy(char *dst, const WCHAR *src, long maxlen = -1) 
void static StrCpy(WCHAR *dst, const WCHAR *src, long maxlen = -1)
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
Copies the content of one string to another, and converts between string types if necessary.

## Version changes
- EDGELIB 2.10: Changed the `unsigned short` parameter types to `WCHAR`

