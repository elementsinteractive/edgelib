:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Plugin definitions

Plugin definitions need to be specified before including "edgemain.h".

## OpenGL
```c++
EGL_NONE     //Don't use OpenGL but use the EDGELIB native rendering engine 
EGL_USEGL    //Use OpenGL (ES) library and rendering  
```

To use OpenGL in an EDGELIB powered application it is also needed to link the OpenGL or OpenGL ES implementation library, include OpenGL (ES) header files, and link a matching OpenGL plugin library.

## Image loader plugins
```c++
EDGE_NODEFAULTPLUGINS  //Disable all image loaders by default  
```

Use this definition to disable all image loaders by default and use [OnPluginLoad](framework_onpluginload.md) to specify the image loaders which are needed.

## Blitlets
```c++
EDGERENDER_LITE    //Include no additional optimizations 
EDGERENDER_DEFAULT  //Include standard, often used optimizations (default) 
EDGERENDER_FULL     //Include optimizations for every filter combination 
EDGERENDER_CUSTOM   //Use a custom preset  
```

Used for including blitlet presets. When EDGERENDER_CUSTOM is defined, include your own custom preset before including "edgemain.h".

## ClassESound wrappers
```c++
ESOUND_SILENT  //Use a dummy sound interface 
ESOUND_HEKKUS  //Use the Hekkus Sound System library 
ESOUND_OPENAL  //Use the OpenAL sound library 
ESOUND_CUSTOM  //Use a different sound library and custom sound wrapper  
```

Define the sound wrapper to use. When ESOUND_CUSTOM is defined, include your own sound wrapper in your project once.

## Version changes
- EDGELIB 3.95: Added ESOUND_OPENAL 
- EDGELIB 3.00: Changed EDGERENDER_LIGHT to EDGERENDER_LITE, added OpenGL plugin definitions

