:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# StrFind


```c++
long static StrFind(const char *str, char character) 
long static StrFind(const WCHAR *str, WCHAR character) 
long static StrFind(const char *str, const char *findstr, bool casesensitive = true) 
long static StrFind(const WCHAR *str, const char *findstr, bool casesensitive = true) 
long static StrFind(const char *str, const WCHAR *findstr, bool casesensitive = true) 
long static StrFind(const WCHAR *str, const WCHAR *findstr, bool casesensitive = true)
```

## Parameters
### *str (in)
The string to scan.

### *findstr (in)
A string to search for in the string.

### character (in)
One character to search for in the string.

### casesensitive (in)
When true (default setting), compare with case sensitivity enabled.

## Return value
The position of the character or first character of the whole string, in the string.

## Description
Searches a string for one character or substring. It returns the position within the string when it first finds the character or string. If the character or substring isn't found, it returns -1.

## Version changes
- EDGELIB 3.97: Improved const correctness 
- EDGELIB 2.10: Changed the `unsigned short` parameter types to `WCHAR`

