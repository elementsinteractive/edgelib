:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# MD5Str


```c++
void static MD5Str(char *result, const char *str) 
void static MD5Str(char *result, const WCHAR *str)
```

## Parameters
### *result (out)
The string containing the result of the MD5 algorithm in hex.

### *str (in)
A string to calculate the MD5 hash from.

## Return value
None.

## Description
Calculates the MD5 hash from the specified string.

## Version changes
- EDGELIB 2.10: Changed the `unsigned short` parameter types to `WCHAR`

