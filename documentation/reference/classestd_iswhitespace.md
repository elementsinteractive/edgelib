:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# IsWhiteSpace


```c++
bool static IsWhiteSpace(WCHAR chr)
```

## Parameters
### chr (in)
A string character to check.

## Return value
Returns true if the supplied character is a _white space_ character, otherwise returns false.

## Description
Determines whitespace characters. Whitespace characters are: space (' '), tab ('\t'), newline ('\n'), linefeed ('\r'), formfeed ('\f') and vertical tab ('\x0b').

## Version changes
- EDGELIB 2.95: Removed obsolete overloaded function 
- EDGELIB 2.50: Added this function to the SDK

