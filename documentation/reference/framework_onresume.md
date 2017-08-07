:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnRestore


```c++
ERESULT OnRestore(void)
```

## Parameters
None.

## Return value
Return `E_OK` when successful. Return `E_ERROR` to shutdown EDGELIB.

## Description
This is a callback when the application is restored from suspension. Use it to resume external engines like a sound system, or reload a saved game state.

