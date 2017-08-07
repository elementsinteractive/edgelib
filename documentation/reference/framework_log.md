:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Log


```c++
void Log(char *text, bool newline = true) 
void Log(WCHAR *text, bool newline = true)
```

## Parameters
### *text (in)
The line of text to log to the file.

### newline (in)
Enable this to add a newline to the file after writing a line of text.

## Return value
None.

## Description
Use this function to log lines of text to a file `log.txt`. Useful for debugging.

## Version changes
- EDGELIB 2.80: Added Unicode overloaded function

