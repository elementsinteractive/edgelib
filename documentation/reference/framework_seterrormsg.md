:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetErrorMsg


```c++
void SetErrorMsg(const char *msg) 
void SetErrorMsg(const WCHAR *msg)
```

## Parameters
### *msg (in)
The message that should be shown when the application exits.

## Return value
None.

## Description
This will set up a message that will appear after closing down the application. Use this to inform the user in case of an error. This function works only for Windows desktop, Windows Mobile and Symbian applications.

## Version changes
- EDGELIB 2.95: Added support for Symbian 
- EDGELIB 2.80: Added Unicode overloaded function

