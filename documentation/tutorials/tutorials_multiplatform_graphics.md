:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Creating graphics for multi-platform games

## Different devices, different resolutions
There are many devices, each with their own screen layout and resolution. The following resolutions are commonly used in devices supported by EDGELIB:

* 176x208 pixels for traditional Symbian Series 60 phones
* 176x220 pixels for Windows Mobile Smartphones
* 208x320 pixels for Symbian UIQ phones
* 240x240 pixels for square Pocket PC screens
* 240x320 pixels for Pocket PC's or high resolution Smartphones
* 320x240 pixels for GP2X devices, Pocket PCs running in landscape mode or certain Smartphones
* 352x416 pixels for high resolution Symbian Series 60 phones
* 480x640 pixels for high resolution Pocket PCs
* 640x200 pixels for Symbian Series 80 devices (Nokia 9500)
* 640x320 pixels for Symbian Series 90 devices (Nokia 7710)
* 640x480 pixels for Windows desktop devices or Pocket PC devices running in landscape mode

## Scaling graphics depending on the resolution
When creating graphics for these resolutions, there are two solutions for solving this problem. Graphics can be scaled down for devices with less pixels (graphics contain less detail), or the graphics can stay the same but there will be less overview on the game action (the level will scroll). Some resolutions are similar like the Symbian phones and Windows Mobile Smartphones. The Symbian UIQ, Series 80 and Series 90 devices are special cases and may need some extra attention during the development phase.

When scaling down graphics it's a good idea to set up a table of graphics dimensions for different resolutions (in percentage). Here is a table that works well:

* 176x208 and 176x220: 75%
* 208x320: depends on the game (can be 75%, 100% or something in between)
* 240x240 and 240x320: 100%
* 352x416: 150%
* 480x640: 200%

Start with the graphics for the device with the highest resolution (depending on which devices will be supported by the game) and scale down. When graphics are hand-drawn pixel by pixel it can be harder to support all different resolutions.

Another possibility is to scale down graphics at run-time, though this is not recommended.

## Chapters
* [How to handle the limits of each platform](tutorials_multiplatform_limits.md)
* [The differences between the input of each device](tutorials_multiplatform_inputdifferences.md)
* **Creating graphics for multi-platform games**
* [Handling of device-independent animations](tutorials_multiplatform_animations.md)

