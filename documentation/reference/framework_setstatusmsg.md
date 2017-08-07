:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetStatusMsg


```c++
void SetStatusMsg(const char *msg) 
void SetStatusMsg(const WCHAR *msg)
```

## Parameters
### *msg (in)
The message that should be shown within the status bar.

## Return value
None.

## Description
When enabling EDGELIB's status tools, the developer is able to show messages in the status bar while the engine runs. This can be used for debugging purposes and can only be enabled for windowed applications.

## Version changes
- EDGELIB 2.80: Added Unicode overloaded function

