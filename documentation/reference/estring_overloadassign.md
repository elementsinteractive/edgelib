:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# operator=


```c++
EString &operator=(const char *str) 
EString &operator=(const WCHAR *str) 
EString &operator=(const EString &str) 
EString &operator=(int value)
```

## Parameters
### str (in)
Assigns the string object with this string object or zero terminated (unicode) string.

### value (in)
Assigns the string object with this integer value.

## Return value
Returns a reference to itself.

## Description
Assigns the string object by using the = operator. For example: EString object = "string".

## Version changes
- EDGELIB 3.96: Added extra operator overload for integer value 
- EDGELIB 3.60: Changed the EString parameter to constant 
- EDGELIB 3.30: Added this operator to the EDGELIB SDK

