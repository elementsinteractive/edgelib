:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# RegisterLoader


```c++
bool RegisterLoader(ClassELoaderBase *loader)
```

## Parameters
### *loader (in)
A pointer to a class derived from ClassELoaderBase

## Return value
Returns true if the loader is registered successfully.

## Description
Registers a loader which can be used for reading surfaces. Registered loader classes are freed automatically on destruction of the display object. To prevent this behaviour (for example, if the loaders objects are defined statically), call [UnRegisterLoader](classedisplay_unregisterloader.md).

### EDGELIB built-in loaders
```c++
ELoader_BMP 
ELoader_GIF 
ELoader_PNG 
ELoader_JPG 
ELoader_TGA 
ELoader_3DS 
ELoader_MS3D
```

## Version changes
- EDGELIB 2.80: Added this function to the SDK

