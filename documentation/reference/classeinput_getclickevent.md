:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetClickEvent


```c++
bool GetClickEvent(E_INPUTEVENT *event)
```

## Parameters
### *event (out)
An [input event structure](classeinput_structures.md) containing more information about the click event.

## Return value
Returns true if an event is available, otherwise returns false.

## Description
Check for click events. When calling this function and an event is found, it will be removed from the event list.

## Version changes
- EDGELIB 2.70: Added this function to the SDK

