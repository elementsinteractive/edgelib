:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# StopSound


```c++
void StopSound(unsigned long index, unsigned long flags = SFX_NONE, unsigned long property = 0)
```

## Parameters
### index (in)
The number of the sound effect to stop playing.

### flags
Flags for extra stop effects.

### property
A property used in combination with the flags parameter.

## Return value
None.

## Description
Stops the specified sound from playing on all channels with an optional fade out effect.

## Version changes
- EDGELIB 2.95: Added this function to the SDK

