:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# CalculateKey


```c++
unsigned short CalculateKey(const char *rpn, const char *id) 
unsigned short CalculateKey(const WCHAR *rpn, const WCHAR *id)
```

## Parameters
### *rpn (in)
The RPN string.

### *id (in)
The unique ID to be processed in combination with the RPN string.

## Return value
The registration key.

## Description
Use this function for calculating registration keys based on the RPN string and a unique ID. This method for calculating registration keys is commonly used by internet resellers.

## Version changes
- EDGELIB 2.80: Added Unicode overloaded function

