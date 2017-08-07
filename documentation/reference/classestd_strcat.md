:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# StrCat


```c++
void static StrCat(char *dst, const char *src) 
void static StrCat(WCHAR *dst, const char *src) 
void static StrCat(char *dst, const WCHAR *src) 
void static StrCat(WCHAR *dst, const WCHAR *src) 
void static StrCat(char *dst, int i, unsigned char base = 10, bool signedvalue = true) 
void static StrCat(WCHAR *dst, int i, unsigned char base = 10, bool signedvalue = true)
```

## Parameters
### *dst (out)
The destination string to append

### *src (in)
The string to copy

### i (in)
The integer value to convert.

### base (in)
The number base to use while converting (for example, decimal is 10 and hexadecimal is 16).

### signedvalue (in)
Set this to false when conversion of high unsigned values are needed.

## Return value
None.

## Description
Appends an existing string with a new string or value.

## Version changes
- EDGELIB 2.80: Added overloaded functions for appending integer values 
- EDGELIB 2.10: Changed the `unsigned short` parameter types to `WCHAR`

