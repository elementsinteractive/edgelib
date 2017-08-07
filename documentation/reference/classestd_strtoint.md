:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# StrToInt


```c++
int static StrToInt(char *str, unsigned char base = 10) 
int static StrToInt(WCHAR *str, unsigned char base = 10)
```

## Parameters
### *str (in)
The string to convert.

### base (in)
The number base to use while converting (for example, decimal is 10 and hexadecimal is 16).

## Return value
The integer value of the converted string.

## Description
Converts a string to an integer value.

## Version changes
- EDGELIB 2.10: Changed the `unsigned short` parameter types to `WCHAR`

