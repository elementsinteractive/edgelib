:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Cat


```c++
bool Cat(const char *str) 
bool Cat(const WCHAR *str) 
bool Cat(const EString &str) 
bool Cat(int value, unsigned char base = 10, bool signedvalue = true)
```

## Parameters
### str (in)
Concatenates the string object with this string value.

### value (in)
Concatenates the string object with this integer value.

### base (in)
The number base to use while converting (for example, decimal is 10 and hexadecimal is 16).

### signedvalue (in)
Set this to false when conversion of high unsigned values are needed.

## Return value
Returns true when successful.

## Description
Concatenates the string object with another EString object or a zero terminated (unicode) string.

## Version changes
- EDGELIB 3.96: Added overloaded function for integer value 
- EDGELIB 3.60: Changed the EString parameter to constant 
- EDGELIB 3.30: Added this function to the EDGELIB SDK

