:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetShading


```c++
void SetShading(unsigned long type, unsigned long property = 0)
```

## Parameters
### type (in)
[Render flags](classedisplay_definitions.md) for shading and texture mapping.

### property (in)
A render property to be used in combination with the render type. Can only be used for the internal software renderer.

## Return value
None.

## Description
Change the shading method for 3D rendering.

## Version changes
- EDGELIB 3.80: Added a property parameter to be used for the internal software renderer 
- EDGELIB 3.00: Added this function to the SDK

