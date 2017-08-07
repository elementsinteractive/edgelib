:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Configure


```c++
void Configure(bool allowexpansion = true, bool indexexpansion = false, unsigned long allocstep = 256)
```

## Parameters
### allowexpansion (in)
Allows the class to expand it's memory when needed.

### indexexpansion (in)
Allows the class to expand when using brackets to access its data.

### allocstep (in)
Specified the chunk size the class should reserve when data is expanded.

## Return value
None.

## Description
Configures how the class behaves when its data is too small and when expansion is needed.

## Version changes
- EDGELIB 3.30: Added this function to the EDGELIB SDK

