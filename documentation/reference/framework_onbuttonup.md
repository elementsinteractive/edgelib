:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnButtonUp


```c++
void OnButtonUp(unsigned long bnr, EBUTTONLIST *blist)
```

## Parameters
### bnr (out)
The code of the button or key that has been released.

### *blist (out)
A [list](ecd_ebuttonlist.md) of commonly used buttoncodes.

## Return value
None.

## Description
This is a callback when the player releases a button or key. Use it for game interaction.

