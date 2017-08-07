:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# IgnoreWhiteSpace


```c++
void IgnoreWhiteSpace(bool set)
```

## Parameters
### set (in)
Set to true to ignore whitespace, otherwise use false.

## Return value
None.

## Description
Ignores whitespace while parsing the XML data. Otherwise empty tags or content with newlines and spaces may appear in the callback mechanism. It ignores whitespace by default.

## Version changes
- EDGELIB 3.30: Added this function to the EDGELIB SDK

