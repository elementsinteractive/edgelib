:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetKeyEvent


```c++
unsigned char GetKeyEvent(unsigned char action) 
bool GetKeyEvent(E_INPUTEVENT *event)
```

## Parameters
### action (in)
Only return events for this action.

### *event (out)
An [input event structure](classeinput_structures.md) containing more information about the key event.

## Return value
Returns an [event type](classeinput_definitions.md) for the specified action, or true if an event is found.

## Description
Check for key events. When calling this function and an event is found, it will be removed from the event list.

## Version changes
- EDGELIB 2.70: Added this function to the SDK

