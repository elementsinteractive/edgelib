:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Optimizing memory and diskspace usage

## Resource file compression
To decrease the size of game resources like graphics and music, you can use EDGELIB packages. An EDGELIB package is a collection of multiple files which can be compressed using ZLib. We recommend disabling compression for certain file types that are compressed already, such as JPEG and PNG. See the file and EDGELIB Packer tutorial for more information.

## Image file formats
The size of image files can be decreased by using a lower color depth. For example, some images such as spritesheets can often be saved in 8-bit data using a palette. Use non-interlaced (GIF and PNG) and non-progressive (JPEG) images, as they are usually smaller in file size.

## Executable size
The size of the game executable can be decreased by disabling image loaders and using a light `edgerender` preset. If you don't need them, consider disabling the relative large JPEG and PNG image loaders.

Default image loaders can be disabled by defining `EDGE_NODEFAULTPLUGINS`. A light `edgerender` preset will be used if `EDGERENDER_LIGHT` is defined. Define these before including edgemain.h for them to take effect. See [`OnPluginLoad`](../reference/framework_onpluginload.md) for more information.

## Optimizing memory usage
Memory usage can be decreased if resources are only loaded when they are needed. For example, load per-level graphics once a new level starts. A common misunderstanding is that loading an 8-bit image occupies less memory than loading its 24-bit equivalent. Surfaces in EDGELIB are stored in the native color depth of the display hardware.

It is however possible to emulate another color depth, for example 16-bit to decrease memory usage if the display hardware uses a 32-bit color depth. See the [ECD definitions](../reference/ecd_definitions.md) for more information.

## Chapters
* [Optimizing performance](tutorials_optimization_performance.md)
* **Optimizing memory and diskspace usage**

