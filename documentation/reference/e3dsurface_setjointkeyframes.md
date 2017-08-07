:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetJointKeyFrames


```c++
void SetJointKeyFrames(unsigned long index, long *keyframelist, unsigned long listsize, bool isrotation = false)
```

## Parameters
### index (in)
The index of the joint.

### *keyframelist (in)
A list of keyframe data, each data item is 4 longs in the following order: time, x, y, z.

### listsize (in)
The amount of keyframe data items.

### isrotation (in)
If set to true, the keyframe data is joint rotation, otherwise the keyframe data is joint translation.

## Return value
None.

## Description
Set animation keyframe data.

## Version changes
- EDGELIB 3.00: Added this function to the SDK

