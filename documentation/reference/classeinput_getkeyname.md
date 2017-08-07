:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetKeyName


```c++
void GetKeyName(long keynr, char *name) 
void GetKeyName(long keynr, WCHAR *name)
```

## Parameters
### keynr (in)
The scancode of the key.

### *name (out)
The name of the key.

## Return value
None.

## Description
Use this function to get the name of the key scancode, useful for a key configuration option screen.

## Version changes
- EDGELIB 2.90: Added this function to the SDK

