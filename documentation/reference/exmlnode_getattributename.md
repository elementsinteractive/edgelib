:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetAttributeName


```c++
WCHAR *GetAttributeName(int id)
```

## Parameters
### id (in)
The index of the attribute.

## Return value
A string containing the name of the attribute.

## Description
Returns the name of the specified attribute. If the node contains less attributes than specified by the index, it will return a string containing '#Error#'.

## Version changes
- EDGELIB 3.80: Added this function to the EDGELIB SDK

