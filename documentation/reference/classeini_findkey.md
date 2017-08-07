:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# FindKey


```c++
bool FindKey(const char *key, char *value, bool casesensitive = false)
```

## Parameters
### *key (in)
A pointer to a string with the key.

### *value (out)
A pointer that holds the value.

### casesensitive (in)
Set this to true if the search should be case sensitive.

## Return value
Return true if the key has been found, otherwise return false.

## Description
Search the ini file for the specified key from the current ini file cursor.

## Version changes
- EDGELIB 2.60: Added this function to the SDK

