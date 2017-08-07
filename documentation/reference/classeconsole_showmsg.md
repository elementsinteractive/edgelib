:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# ShowMsg


```c++
void ShowMsg(const char *msg, bool newline = true) 
void ShowMsg(const WCHAR *msg, bool newline = true) 
void ShowMsg(int value, bool newline = true)
```

## Parameters
### *msg (in)
A string to add to the console window.

### value (in)
An integer value to add to the console window.

### newline (in)
Set this to false to disable a newline after the function call. Newlines are enabled by default.

## Return value
None.

## Description
This function is used to show messages into the console window. Because the window uses memory to display its text, it's recommended to [empty](classeconsole_empty.md) the console window regularly (after it contains about 2000 lines of text is recommended).

## Version changes
- EDGELIB 2.60: Added Unicode overloaded function

