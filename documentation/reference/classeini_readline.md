:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# ReadLine


```c++
bool ReadLine(char *key, char *value)
```

## Parameters
### *key (out)
A pointer to a string to contain the key.

### *value (out)
A pointer to a string to contain the value.

## Return value
Return true if a line from the INI file could be read successfully, otherwise return false.

## Description
Read one line (containing a key and value) from an opened INI file.

## Version changes
- EDGELIB 2.50: Added this function to the SDK

