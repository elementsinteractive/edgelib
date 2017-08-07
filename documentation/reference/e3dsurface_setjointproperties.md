:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetJointProperties


```c++
void SetJointProperties(unsigned long index, EVECTOR3D *location, EVECTOR3D *rotation, long parent = -1)
```

## Parameters
### index (in)
The index of the joint to set.

### *location (in)
The location vector for joint translation.

### *rotation (in)
The rotation vector for joint rotation.

### parent (in)
The parent of the joint.

## Return value
None.

## Description
Set joint properties for skeletal animation.

## Version changes
- EDGELIB 3.00: Added this function to the SDK

