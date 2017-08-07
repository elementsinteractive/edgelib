:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# MsgWindow


```c++
void static MsgWindow(const char *msg, const char *caption = NULL) 
void static MsgWindow(const WCHAR *msg, const WCHAR *caption = NULL) 
void static MsgWindow(long value, const char *caption = NULL)
```

## Parameters
### *msg (in)
The message to show inside the window.

### value (in)
A value to show as a message in the window.

### *caption (in)
The title of the window, only available on devices running Microsoft Windows.

## Return value
None.

## Description
Open a new window containing a message to easily show debug information. This is a static method that can be called everywhere, like: `ClassEConsole::MsgWindow("Message")`. This function will minimize the application for mobile devices.

## Version changes
- EDGELIB 2.60: Added Unicode overloaded function

