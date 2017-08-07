:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# FindValue


```c++
bool FindValue(char *key, const char *value, bool casesensitive = false)
```

## Parameters
### *key (out)
A pointer that holds the key.

### *value (in)
A pointer to a string with the value.

### casesensitive (in)
Set this to true if the search should be case sensitive.

## Return value
Return true if the value has been found, otherwise return false.

## Description
Search the ini file for the specified value from the current ini file cursor.

## Version changes
- EDGELIB 2.60: Added this function to the SDK

