:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# WriteLine


```c++
bool WriteLine(const char *key, const char *value) 
bool WriteLine(const char *key, long valuenr)
```

## Parameters
### *key (in)
A pointer to a string with the key.

### *value (in)
A pointer to a string with the value.

### valuenr (in)
An integer value.

## Return value
Return true if a line could be written from the INI file successfully, otherwise return false.

## Description
Write a value and key to an opened INI file.

## Version changes
- EDGELIB 2.60: Added an overloaded function for writing integer values 
- EDGELIB 2.50: Added this function to the SDK

