:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Handling of device-independent animations

## Basic animation
Typical animations are done by saving an object's position in pixels and moving the object in pixels as well. This may work well with devices which have a constant framerate and for projects supporting one type of device. However, when multiple devices with multiple resolutions should be supported and the framerate isn't constant, it can be troublesome to continue with this method.

A better way to solve this, is to save the position of an object in 'units' which are usually smaller than pixels. When drawing an object on the screen, these units can be converted back to screen coordinates (by a divider) and drawn.

## Device independent animation
There are two ways to make these units device independent. The first is to have a multiplier when moving an object, for example:

_Code (1 line)_
```c++
object[i].x_unit += time * DEF_ANIMULTIPLIER
```

When drawing to the screen, each unit is divided by a fixed value (for example 256, which is fast due to the architecture of the processor).

The other method is to have a different divider depending on the device, for example:

_Code (1 line)_
```c++
object[i].x_draw = object[i].x_unit / DEF_ANIDIVIDER
```

The first method is preferred, because it is faster (a variable divider is slower on mobile devices than a variable multiplier). 
When looking at the first code block, you can see a time variable that's being multiplied. This means that this animation is independent on framerate as well (the time value can be obtained within the game loop).

## Chapters
* [How to handle the limits of each platform](tutorials_multiplatform_limits.md)
* [The differences between the input of each device](tutorials_multiplatform_inputdifferences.md)
* [Creating graphics for multi-platform games](tutorials_multiplatform_graphics.md)
* **Handling of device-independent animations**

