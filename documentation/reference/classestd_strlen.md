:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# StrLen


```c++
long static StrLen(const char *str, long max_str = -1) 
long static StrLen(const WCHAR *str, long max_str = -1)
```

## Parameters
### *str (in)
The string to evaluate.

### max_str (in)
The maximum amount of string characters te check, before returning.

## Return value
The length of the string.

## Description
Returns the amount of characters the string contains.

## Version changes
- EDGELIB 3.40: Changed `max_str` datatype to long and set to default value -1 (infinite) 
- EDGELIB 2.10: Changed the `unsigned short` parameter types to `WCHAR`

