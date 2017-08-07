:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetErrorMessage (deprecated)


```c++
void GetErrorMessage(long errcode, char *errormsg)
```

## Parameters
### errcode (in)
The error code received from the ENET_CPGERROR message.

### *errormsg (out)
A string to hold the error message.

## Return value
None.

## Description
Convert an error code to a error message in readable text.

## Version changes
- EDGELIB 4.00: Removed from SDK 
- EDGELIB 2.70: Added this function to the SDK

