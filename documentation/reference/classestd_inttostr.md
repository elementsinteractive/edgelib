:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# IntToStr


```c++
void static IntToStr(char *dst, int i, unsigned char base = 10, bool signedvalue = true) 
void static IntToStr(WCHAR *dst, int i, unsigned char base = 10, bool signedvalue = true)
```

## Parameters
### *dst (out)
The string to convert to.

### i (in)
The integer value to convert.

### base (in)
The number base to use while converting (for example, decimal is 10 and hexadecimal is 16).

### signedvalue (in)
Set this to false when conversion of high unsigned values are needed.

## Return value
None.

## Description
Converts an integer value to a string.

## Version changes
- EDGELIB 2.70: Added a signedvalue parameter 
- EDGELIB 2.10: Changed the `unsigned short` parameter types to `WCHAR`

