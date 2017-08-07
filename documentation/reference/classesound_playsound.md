:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PlaySound


```c++
long PlaySound(unsigned long index, long playspeed = 0, signed char pan = 0, bool loopsound = false, unsigned long flags = SFX_NONE, unsigned long property = 0)
```

## Parameters
### index (in)
The number of the sound effect to play.

### playspeed (in)
Changes the speed of sound playback. Negative values specify a slowdown while positive values specify a speedup. Suggested values are -128 to 128. 0 is neutral.

### pan
For stereo playback specifies panning. Negative values take the left speaker while positive values take the right speaker. Suggested values are -128 to 127. 0 is neutral.

### loopsound
Set this to true if the sound should continue playing.

### flags
Flags for extra playback effects.

### property
A property used in combination with the flags parameter.

## Return value
Returns the playing sound channel.

## Description
Play a sound effect with various effects.

## Version changes
- EDGELIB 2.95: Added this function to the SDK

