:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# LoadSoundEffect


```c++
ERESULT LoadSoundEffect(unsigned long index, void *data, unsigned long datasize) 
ERESULT LoadSoundEffect(unsigned long index, char *sndfile) 
ERESULT LoadSoundEffect(unsigned long index, WCHAR *sndfile)
```

## Parameters
### index (in)
A number between 0 and 255 for storing the sound effect.

### *sndfile (in)
A pointer to a string containing the filename.

### *data
Data to load from.

### datasize
The size of data.

## Return value
Returns E_OK when loaded correctly, otherwise returns an EDGELIB error code.

## Description
Loads a sound effect and stores it in the sound class.

## Version changes
- EDGELIB 2.95: Added this function to the SDK

