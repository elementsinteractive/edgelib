:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PackUseIndex


```c++
void PackUseIndex(char *newindex)
```

## Parameters
### *newindex (in)
A pointer to the file index.

## Description
Link an existing file index to the pack file. The file index should be created by [PackBuildIndex](classefile_packbuildindex.md) and from the same pack file. This is useful if a pack file is opened multiple times throughout the program and the index doesn't need to be recreated every time it is opened.

## Version changes
- EDGELIB 2.80: Added this function to the SDK

