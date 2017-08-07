:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# AddParam


```c++
bool AddParam(const char *name, const char *value) 
bool AddParam(const char *name, long value)
```

## Parameters
### *name (in)
The name of the parameter.

### value (in)
The value of the parameter.

## Return value
Returns true if parameter has been added, otherwise returns false.

## Description
Add a parameter to send with the HTTP request (GET or POST).

## Version changes
- EDGELIB 2.60: Added an overloaded alternative for adding integer values

