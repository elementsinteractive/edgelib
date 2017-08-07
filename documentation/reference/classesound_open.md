:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Open


```c++
ERESULT Open(void) 
ERESULT Open(unsigned long frequency, unsigned char bitspersample, bool stereo)
```

## Parameters
### frequency (in)
The frequency for sound playback. A higher frequency needs more performance. Suggested values are: 44100, 22050, 16000, 11025 and 8000.

### bitspersample (in)
The amount of bits to use per sample. A higher value gives more precision to sound playback and increases playback quality, but needs more performance. Suggested values are: 8 and 16.

### stereo
Starts sound playback in stereo. Not supported on all mobile devices.

## Return value
Returns E_OK when ready for sound playback, otherwise returns an Edge error code.

## Description
Opens the sound system for audio playback. The function with no parameter will use recommended settings depending on the device.

## Version changes
- EDGELIB 2.95: Added this function to the SDK

