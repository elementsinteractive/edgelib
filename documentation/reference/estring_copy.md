:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Copy


```c++
void Copy(const char *str) 
void Copy(const WCHAR *str) 
void Copy(const EString &str) 
void Copy(int value)
```

## Parameters
### str (in)
An existing string to copy to the string object.

### value (in)
A value to copy to the string object.

### base (in)
The number base to use while converting (for example, decimal is 10 and hexadecimal is 16).

### signedvalue (in)
Set this to false when conversion of high unsigned values are needed.

## Return value
None.

## Description
Copy an existing string to the object. This string can be a zero terminated (unicode) string, or an EString object.

## Version changes
- EDGELIB 3.96: Added overloaded function for integer value 
- EDGELIB 3.60: Changed the EString parameter to constant 
- EDGELIB 3.30: Added this function to the EDGELIB SDK

