:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# StrLower


```c++
char static *StrLower(char *str) 
WCHAR static *StrLower(WCHAR *str)
```

## Parameters
### *str (in/out)
The string to scan

## Return value
A pointer to the parameter *str.

## Description
Changes a string to lowercase.

## See also
[StrUpper](classestd_strupper.md)

## Version changes
- EDGELIB 3.90: Extended function to also change to case of accented letters 
- EDGELIB 2.70: Added this function to the SDK

