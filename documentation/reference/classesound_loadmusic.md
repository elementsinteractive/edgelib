:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# LoadMusic


```c++
ERESULT LoadMusic(unsigned long index, void *data, unsigned long datasize) 
ERESULT LoadMusic(unsigned long index, char *sndfile) 
ERESULT LoadMusic(unsigned long index, WCHAR *sndfile)
```

## Parameters
### index (in)
A number between 0 and 31 for storing the music.

### *sndfile (in)
A pointer to a string containing the filename.

### *data
Data to load from.

### datasize
The size of data.

## Return value
Returns E_OK when loaded correctly, otherwise returns an EDGELIB error code.

## Description
Loads music and stores it in the sound class.

## Version changes
- EDGELIB 2.95: Added this function to the SDK

