:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# EString (constructor)


```c++
EString(void) 
EString(const char *str) 
EString(const WCHAR *str) 
EString(const EString &str)
```

## Parameters
### str (in)
An existing string to copy to the string object.

## Return value
None.

## Description
The constructor is called when the EString class is created. Optionally, an existing string can be specified to copy to the object. This string can be a zero terminated (unicode) string, or an EString object.

## Version changes
- EDGELIB 3.70: Made EString parameter constant 
- EDGELIB 3.30: Added this class to the EDGELIB SDK

