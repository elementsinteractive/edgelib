:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# StrEqual


```c++
bool static StrEqual(const char *str1, const char *str2, bool casesensitive = true) 
bool static StrEqual(const WCHAR *str1, const char *str2, bool casesensitive = true) 
bool static StrEqual(const char *str1, const WCHAR *str2, bool casesensitive = true) 
bool static StrEqual(const WCHAR *str1, const WCHAR *str2, bool casesensitive = true)
```

## Parameters
### *str1 (in)
The first string to compare.

### *str2 (in)
The second string to compare.

### casesensitive (in)
When true (default setting), compare with case sensitivity enabled.

## Return value
Returns true if these strings match, otherwise returns false.

## Description
Compare the characters of these strings, to see if they're equal.

## Version changes
- EDGELIB 2.10: Changed the `unsigned short` parameter types to `WCHAR`

