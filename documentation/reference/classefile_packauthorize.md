:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PackAuthorize


```c++
bool PackAuthorize(unsigned long key)
```

## Parameters
### key (in)
A key for opening vendor sigged packs.

## Return value
Return true if authorization has succeeded and the opened file is a pack, otherwise return false.

## Description
Authorization is needed to open packs which has a vendor type other than "FREE". Keys in combination with a vendor code are provided to registered EDGELIB licensees.

