:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnNativeEvent


```c++
bool OnNativeEvent(ENATIVETYPE winhandle, ENATIVETYPE msg, ENATIVETYPE param1, ENATIVETYPE param2)
```

## Parameters
### winhandle(out)
The window handle of the framework.

### msg (out)
The window message (Win32) or event structure (Symbian).

### param1 (out)
First event parameter.

### param2 (out)
Second event parameter.

## Return value
Return true when handling a message, otherwise return false.

## Description
A custom message handler for Windows desktop, Windows Mobile or Symbian applications. Return true when reacting on a message, return false by default. For Symbian, the msg parameter kan be cast to TWsEvent *. For Win32 the msg parameter can be cast to UINT, param1 to WPARAM and param2 to LPARAM.

## Version changes
- EDGELIB 3.10: Replacement of OnMessage, added support for Symbian

