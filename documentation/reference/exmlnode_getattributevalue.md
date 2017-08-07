:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetAttributeValue


```c++
WCHAR *GetAttributeValue(int id) 
WCHAR *GetAttributeValue(const char *name)
```

## Parameters
### id (in)
The index of the attribute.

### name (in)
Search for an attribute with this name.

## Return value
A string containing the value of the attribute.

## Description
Returns the value of the specified attribute. If the node contains less attributes than specified by the index or an attribute with the specified name can't be found, it will return a string containing '#Error#'.

## Version changes
- EDGELIB 3.80: Added this function to the EDGELIB SDK

