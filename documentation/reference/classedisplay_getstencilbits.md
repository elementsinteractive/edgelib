:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetStencilBits


```c++
unsigned char GetStencilBits(void)
```

## Parameters
None.

## Return value
The amount of bits in the stencil buffer that are available for each pixel, or zero when no stencil buffer is present.

## Description
This function allows you to check the presence and size of the stencil buffer that was created together with an OpenGL context. A stencil buffer has to be specifically requested for in [OnDisplayConfig](framework_ondisplayconfig.md). If the hardware does not support stencil buffers or if EDGELIB is running with software drawing then a stencil buffer will not be created.

## Version changes
- EDGELIB 4.00: Added this function to the SDK

