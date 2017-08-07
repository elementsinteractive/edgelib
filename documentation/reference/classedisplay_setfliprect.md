:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetFlipRect


```c++
void SetFlipRect(RECT *rc = NULL)
```

## Parameters
### *rc (in)
The rectangle to mark as updated, only this part of the screen will be refreshed on the next screen flip.

## Return value
None.

## Description
Define a rectangle to only update a part of the screen, rather than every pixel. This will increase performance. When using the evaluation version, the new rectangle will have effect when the message animation has finished.

## Version changes
- EDGELIB 2.40: Changed behaviour of this function in the evaluation version 
- EDGELIB 2.10: Added this function to the SDK

