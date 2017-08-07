:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetFPS


```c++
void GetFPS(unsigned long &full, unsigned long &decimal) 
unsigned long GetFPS(void)
```

## Parameters
### &full (out)
The full amount of frames per second.

### &decimal (out)
The decimal amount of frames per second (two decimals).

## Return value
The full amount of frames per second.

## Description
Retrieve more information about the performance of the game. The amount of frames per second is the game's ability to process the game loop multiple times in a second.

## Version changes
- EDGELIB 2.50: Added a new overloaded function returning just the full amount of fps

