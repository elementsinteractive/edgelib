:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Optimizing performance

## Selecting optimized blitlets
As of EDGELIB SDK version 2.90 it's possible to manually select optimized blitters (called blitlets) for filter combinations. This applies to BltFast and Blt without scaling and optimized rotation values of 1024, 2048 and 3072 (90 degrees clockwise/counterclockwise and 180 degrees rotation). When a blitting routines isn't available in the target executable, EDGELIB falls back to the generic, but slower routine to process the blitting operation.

There are three default presets which can be selected using a simple definition before including `edgemain.h`:

```c++
EDGERENDER_LITE     //Include no additional optimizations 
EDGERENDER_DEFAULT  //Include standard, often used optimizations (default) 
EDGERENDER_FULL     //Include optimizations for every filter combination 
EDGERENDER_CUSTOM   //Use a custom preset  
```

When using a custom preset, define `EDGERENDER_CUSTOM` before including `edgemain.h` and include the custom preset header file once after including `edgemain.h`. We recommend using `edge/include/edgerender/erdr_full.h` as a template by copying it to `erdr_custom.h` and removing unnecessary routines.

Note: Not all blitlet sets are available in the EDGELIB Lite and Standard editions.

## Optimized opacity values
To gain maximum performance we recommend using the most optimized versions of EDGELIB features as much as possible. Optimized blending is available for special cases such as opacity values: 16, 32, 64, 128, 192, 224 and 240. Using these values for [`BltFast`](../reference/e2dsurface_bltfast.md) and [`FillRect`](../reference/e2dsurface_fillrect.md) can increase performance up to 75%. Try to use one of these values as much as possible.

Opacity values 0 and 255 are ignored. When 0% opacity is specified, the operation will return immediately.

## Surface size
Blitting using [`Blt()`](../reference/e2dsurface_blt.md) and [rendering textured models](../reference/e3dsurface_bindtexture.md) will be a lot faster when the source surface or texture is a certain size. Optimized sizes are: 64x64, 128x128, 256x256, 512x512 and 1024x1024 pixels. It would be a good idea resizing a surface of 115x120 to 128x128 pixels. It takes a small amount of extra memory, but will increase performance.

## Filter effects
Some [filter effects](../reference/e2dsurface_bltfast.md) consume valuable speed, try to use them as less as possible, or use a caching technique if a filter operation does not change on a surface.

For the game Quartz 2, for example, the used coloured stones are coloured immediately after loading a neutral gray coloured image. The colorize filter is used to create every stone individually and store them in memory. This way no colorize filter is needed when blitting in-game.

## Viewport
When a large part of the screen doesn't need to be updated every frame, for example a part of the HUD, captions of softkeys, or maybe an in-game logo, you can use a clipper for the only part of the screen that needs to be updated. [Clipping](../reference/e2dsurface_setclipper.md) can be used on a surface, but a [clipper can also be specified for the final flip operation](../reference/classedisplay_setfliprect.md) which draws the buffer on the display.

In the Series 80 versions of the games Blazar and Li-Nuggz this technique is used to boost the performance. When extra performance is needed, consider to make the viewport of the game smaller and, for example, enlarge the HUD.

## DirectX
When targeting Windows Mobile 5.0, consider using DirectX to [create hardware surfaces](../reference/classedisplay_createsurface.md). DirectX-enabled devices can execute graphics operations directly in the video memory by the hardware. This could result in a huge performance boost.

The you can overrule the default surface mode by specifying one of the [following surface definitions](../reference/classedisplay_definitions.md) when creating a surface:

```c++
EST_DEFAULT      //Uses a default parameter specified by the EDGELIB configuration 
EST_SYSMEMORY    //Create a system memory surface using the internal EDGELIB display API 
EST_SYSTEMAPI    //Create a platform specific surface type 
EST_HARDWARE     //Create a hardware surface type  
```

`EST_SYSTEMAPI` is a DirectX surface in system memory. On Windows Mobile 5.0 `EST_SYSTEMAPI` works the same as `EST_HARDWARE`, because DirectX surfaces in system memory are not supported by WM5. Windows Mobile 5.0 and Windows desktop use DirectX by default. You can overrule the by setting by specifying `EST_SYSMEMORY`.

The downside of using DirectX is that filter effects and clipping aren't natively supported. These will be emulated by EDGELIB. Emulated blitting will have a major performance hit when using on hardware surfaces.

## Caching techniques
As explained under Filter effects, you could use caching techniques for a lot of operations that only need to be executed once. A tile-based game using walls could be extended with a shadow effect by creating tiles with shadow after loading. You wouldn't need to blit the walls and their shadows every frame, but only the newly created shadowed sprite.

An example is a game where the user can select levels from five different planets. The planets are 3D spheres rendered with a texture. Only the selected planet is rotating. When the user selects another planet, the 3D render of the active planet is copied to a 2D surface and blitted in stead. This way rendering only one 3D object needs to be rendered at a time.

## Not displaying non-visible objects
Sounds obvious, but we mention it here nevertheless. You should not try to display objects on the screen which aren't visible. You could think of objects "far away" in a landscape-oriented game, off-screen sprites and 3D objects behind the camera.

## Detail level
A good idea for most games is adding a detail level option in your game. Players using a fast device could put the detail level on high, while players with an older, slower device could put the detail level on medium or low.

You can think of reducing or disabling extra graphics that enhances the game and doesn't affect the gameplay. A good example is the use of particles for explosions etcetera. Disabling sound effects and music could speed-up the game on a slow device as well.

A possible detail level configuration could be:

* High: all graphics features enabled
* Medium: opacity disabled, lower amount of particles
* Low: same as medium, disabling (a scrolling) background, no particles and even a simple gradient as a backdrop picture

## RGBA surfaces
While [RGBA surfaces](../reference/e2dsurfacergba.md) provide superior graphics because of the additional alpha layer, it can also be a performance killer on slower devices. Using a RGB surface with a color keyed transparancy is significantly faster.

## Image formats
Don't use interlaced PNG images as they consume more space and need extra processor power. In other cases it's a good idea to save a bitmap file using 256 colors to decrease loading time and memory/disk usage.

## Chapters
* **Optimizing performance**
* [Optimizing memory and diskspace usage](tutorials_optimization_usage.md)

