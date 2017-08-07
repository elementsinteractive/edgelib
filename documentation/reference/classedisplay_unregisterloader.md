:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# UnRegisterLoader


```c++
bool UnRegisterLoader(ClassELoaderBase *loader)
```

## Parameters
### *loader (in)
A pointer to a class derived from ClassELoaderBase

## Return value
Returns true if the loader is found and unregistered successfully.

## Description
Unregisters a previously registered loader. It is only needed to call this function if the loader objects are statically defined or if the loader object should be destroyed manually.

## Version changes
- EDGELIB 2.80: Added this function to the SDK

