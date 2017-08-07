:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# How to handle the limits of each platform

## Know your limits
Each platform comes with its own limits. Some platforms are faster than the others and devices of the same platform can have differences as well. While some devices can be slower, this is usually compensated by having a smaller screen. A smaller screen doesn't need as much time for drawing than a larger screen.

Here is a list of limits a developer can run into:

* Programming limits: source code might not be able to compile on a different platform
* Performance limits: some devices are slow compared to others
* Input limits: while a button-intensive action game might work well on one platform, it usually doesn't work that good on phone devices.

## Work around those limits
When knowing your limits, you can evade or work around them. Some devices might be excluded from the start, because the game is not suitable for that platform. After determining a list of devices that will be supported, the list of limits that applies for a project can be made.

Programming limits are easy to solve if the developer knows these limits from the start. It's a good idea to build versions for each platform regularly to see if the code still compiles for every platform.

Performance limits are a bit harder to solve, because they can show up in every stage of the development process. The best thing is to build versions for the slowest device as much as possible.

Input limits are the hardest to solve. Some games may be simplified and other games may be impossible to control on some devices. Most devices can be controlled using buttons, but other devices can be controlled with a touch screen and stylus. Try to support both input systems for playability accross a broad range of devices.

## Chapters
* **How to handle the limits of each platform**
* [The differences between the input of each device](tutorials_multiplatform_inputdifferences.md)
* [Creating graphics for multi-platform games](tutorials_multiplatform_graphics.md)
* [Handling of device-independent animations](tutorials_multiplatform_animations.md)

