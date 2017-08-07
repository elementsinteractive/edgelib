:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PlayMusic


```c++
long PlayMusic(unsigned long index, bool loopmusic = true, unsigned long flags = SFX_NONE, unsigned long property = 0)
```

## Parameters
### index (in)
The number of the music to play.

### loopsound
Set this to true if the music should continue playing.

### flags
Flags for extra playback effects.

### property
A property used in combination with the flags parameter.

## Return value
Returns the playing music channel.

## Description
Play music with various effects.

## Version changes
- EDGELIB 2.95: Added this function to the SDK

