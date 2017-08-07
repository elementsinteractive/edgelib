:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# ClickHold


```c++
bool ClickHold(unsigned char button = EIC_ALL)
```

## Parameters
### button (in)
Check for a button on the click device.

## Return value
Returns true if (a button on) the click device is held.

## Description
Check if (a button of) a click device is held. A click device is a mouse, stylus or finger (touch). For stylus devices, the button doesn't need to be specified. A list of buttons can be found [here](classeinput_definitions.md).

## Version changes
- EDGELIB 2.70: Added this function to the SDK

