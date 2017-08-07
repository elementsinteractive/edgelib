:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# MappedTo


```c++
bool MappedTo(unsigned char action, unsigned long nr)
```

## Parameters
### action (in)
The action to check in the mapping list.

### nr (in)
The key scancode.

## Return value
Returns true if the key scan code is mapped to the specified action.

## Description
Check if keys are mapped to actions in ClassEInput.

## Version changes
- EDGELIB 2.95: Added this function to the SDK

