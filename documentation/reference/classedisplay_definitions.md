:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Definitions

## Display modes
```c++
EDSP_NONE       //No display mode specified 
EDSP_INDEXED8   //256 color display mode with an indexed palette 
EDSP_TRUE12     //4K true color display mode. Pixel format: 4.4.4 
EDSP_TRUE15     //32K true color display mode. Pixel format: 5.5.5 
EDSP_TRUE16     //65K true color display mode. Pixel format: 5.6.5 
EDSP_TRUE24     //16.7M true color display mode. Pixel format: 8.8.8 
EDSP_TRUE24_INV //16.7M true color display mode. Pixel format: 8.8.8 (inverted) 
EDSP_TRUE32     //16.7M true color display mode. Pixel format: 8.8.8.8 
EDSP_TRUE32_INV //16.7M true color display mode. Pixel format: 8.8.8.8 (inverted) 
EDSP_DEFAULT    //Takes the default display mode (depending on the hardware)  
```

Specify a display mode, mobile devices usually have a fixed display mode which can't be changed. When using EDSP_DEFAULT on a desktop PC in a fullscreen application it opens the display in 15 or 16bit true color mode. For the inverted modes, the red and blue color channels are swapped.

## Display orientations
```c++
DOR_NONE      //Use no rotation 
DOR_90DEG     //Rotate the display 90 degrees clockwise 
DOR_180DEG    //Rotate the display 180 degrees 
DOR_270DEG    //Rotate 270 degrees clockwise or 90 degrees counterclockwise 
DOR_PORTRAIT  //Force display in portrait mode 
DOR_LANDSCAPE //Force display in landscape mode 
DOR_STARTUP   //Take the current orientation of the operating system at startup 
DOR_AUTO      //Keep the orientation of the operating system  
```

Most mobile devices and newer desktop devices are able to change the display orientation. EDGELIB is able to change the rotation of the display buffer to the desired format.

## Automatic orientations
```c++
ESO_NONE       // Allow the unrotated orientation 
ESO_90DEG      // Allow the 90 degree orientation 
ESO_180DEG     // Allow the 180 degree orientation 
ESO_270DEG     // Allow the 270 degree orientation 
ESO_ALL        // Allow all orientations  
```

Devices may be able to inform the process when it is rotated. EDGELIB can adjust the display accordingly so that the same side remains up. You can select the rotations allowed by adding these flags together.

## Surface create flags
```c++
EST_DEFAULT      //Uses a default parameter specified by the Edge configuration 
EST_SYSMEMORY    //Create a system memory surface using the internal Edge display API 
EST_SYSTEMAPI    //Create a platform specific surface type 
EST_HARDWARE     //Create a hardware surface type 
EST_DITHER       //Use dithering while loading the surface 
EST_READONLY     //Create the surface as read-only 
EST_MATCHPALETTE //Match colors to the display palette 
EST_READONLYPATH //Use the read-only path for loading files 
EST_DSPINDEXED8   //Force colordepth: indexed, 256 colors 
EST_DSPTRUE12     //Force colordepth: 12 bits true color 
EST_DSPTRUE15     //Force colordepth: 15 bits true color 
EST_DSPTRUE16     //Force colordepth: 16 bits true color 
EST_DSPTRUE24     //Force colordepth: 24 bits true color 
EST_DSPTRUE24_INV //Force colordepth: 24 bits true color (inverted) 
EST_DSPTRUE32     //Force colordepth: 32 bits true color 
EST_DSPTRUE32_INV //Force colordepth: 32 bits true color (inverted) 
EST_NONORMALARRAY   //Disable vertex normal information 
EST_NOCOLORARRAY    //Disable vertex color information 
EST_NOTEXCOORDARRAY //Disable texture coordinate information 
EST_NOINTERLEAVE    //No interleaved arrays  
```

Flags to alter the way surfaces are created. When using EST_DEFAULT it takes the flags defined by the Edge configuration option: [surfacedefault](ecd_edgeconfig.md). EST_DITHER is recommended for backdrop surfaces with many colors for devices with low colordepths. EST_READONLY can speedup the blitting of RGBA surfaces. For 3D surfaces it's possible to disable normal, color and texture coordinate arrays to reduce data throughput. For some OpenGL implementations, interleaved arrays may have a difference in performance.

## Surface blit flags
```c++
EFX_COLORKEY   //Blit with transparency, color keyed pixels won't be drawn 
EFX_COLORFILL  //Blit every pixel with the given color, combine this  
EFX_TINT       //Blit with a colored tint, pass the desired color as a parameter 
EFX_COLORIZE   //Colorize each pixel, pass the desired color as a parameter 
EFX_OPACITY    //Pixel blending: opacity values range from 0 to 255 
EFX_ALPHA      //Pixel blending: Add color channels of each pixel 
EFX_SUB        //Pixel blending: Subtract color channels, source from destination 
EFX_DITHER     //Improves color quality for displays with a low amount of colors  
```

Effects to use while blitting. EFX_DITHER is only available when filling a gradient or in combination with Gouraud shading.

## Font alignment flags
```c++
EFO_HLEFT     //Align the text to the left 
EFO_HRIGHT    //Align the text to the right 
EFO_HCENTER   //Horizontally center the text 
EFO_VTOP      //Align the text to the top 
EFO_VBOTTOM   //Align the text to the bottom 
EFO_VCENTER   //Vertically center the text  
```

Change the font alignment. When none are specified it uses EFO_HLEFT and EFO_VTOP by default.

## Font reordering flags
```c++
EFR_DEFAULT     //Reorder a font resource only in OpenGL modes 
EFR_ALWAYS   //Always try to reorder a font resource 
EFR_NEVER    //Do not reorder a font resource  
```

Font reordering is used in OpenGL modes to fit a font resource into a square texture by moving the characters to different positions.

## Surface blit flip flags
```c++
EFLIP_NONE         //Don't flip in any direction 
EFLIP_HORIZONTAL   //Flip the image horizontally 
EFLIP_VERTICAL     //Flip the image vertically  
```

Flip the image while blitting. Flags can be combined to flip both horizontal and vertical.

## Surface draw control flags
```c++
E2D_ALLOWNATIVE    //Forces Edge to use the platform native API 
E2D_PREVENTNATIVE  //Forces Edge to use the internal Edge API  
```

These flags can be used in individual E2DSurface function calls such as rect filling and blitting. If E2D_ALLOWNATIVE is specified but no platform native API is available, it falls back on the Edge internal API. These flags overrule the [dspinternal](ecd_edgeconfig.md) configuration option.

## Render flags
```c++
E3D_NONE       //Use no shading 
E3D_WIREFRAME  //Draw the polygon edges as lines 
E3D_FLAT       //Each polygon has one color depending on the light source 
E3D_GOURAUD    //Each vertex point is shaded, polygons are filled with a gradient 
E3D_TEXTURE    //Draw polygons with a mapped 2D surface  
```

Use different shading types when rendering 3D objects.

## Texture mapping methods
```c++
ETEX_AFFINE      //Affine texture mapping, the fastest method 
ETEX_PERSPECTIVE //Perspective correct texture mapping  
```

The method used for texture mapping 3D rendering.

## Wipe types
```c++
EWIPE_SLIDELEFT    //The source surface will slide left onto the destination surface 
EWIPE_SLIDERIGHT   //The source surface will slide right onto the destination surface 
EWIPE_SLIDEUP      //The source surface will slide up onto the destination surface 
EWIPE_SLIDEDOWN    //The source surface will slide down onto the destination surface 
EWIPE_BLOCKS       //A custom animation that can be tiled and stretched 
EWIPE_PIXELATE     //Shows a pixelate effect between 2 surfaces 
EWIPE_BLEND        //Alpha blend source surface 1 into source surface 2 
EWIPE_SCREEN       //A wipe between 2 screen sized surfaces  
```

Different wipe effects. EWIPE_BLOCKS and EWIPE_SCREEN require a displacement map. For EWIPE_SCREEN this displacement map will cover the size of the entire surface. EWIPE_BLOCKS will use a small 16x16 displacement map which will be repeated, the pixel size can be specified with the property parameter.

## Screen wipe types
```c++
EWIPE_SCREENDISSOLVE     //Dissolve a surface into another with a random pixel effect  
```

Different wipe types to use in combination with EWIPE_SCREEN.

## Color constants
```c++
ECOLOR_BLACK    //E_RGBX(  0,   0,   0) or #000000 
ECOLOR_GREEN    //E_RGBX(  0, 128,   0) or #008000 
ECOLOR_SILVER   //E_RGBX(192, 192, 192) or #c0c0c0 
ECOLOR_LIME     //E_RGBX(  0, 240,   0) or #00f000 
ECOLOR_GRAY     //E_RGBX(128, 128, 128) or #808080 
ECOLOR_OLIVE    //E_RGBX(128, 128,   0) or #808000 
ECOLOR_WHITE    //E_RGBX(255, 255, 255) or #ffffff 
ECOLOR_YELLOW   //E_RGBX(255, 255,   0) or #ffff00 
ECOLOR_MAROON   //E_RGBX(128,   0,   0) or #800000 
ECOLOR_NAVY     //E_RGBX(  0,   0, 128) or #000080 
ECOLOR_RED      //E_RGBX(255,   0,   0) or #ff0000 
ECOLOR_BLUE     //E_RGBX(  0,   0, 255) or #0000ff 
ECOLOR_PURPLE   //E_RGBX(128,   0, 128) or #800080 
ECOLOR_TEAL     //E_RGBX(  0, 128, 128) or #008080 
ECOLOR_FUCHSIA  //E_RGBX(255,   0, 255) or #ff00ff 
ECOLOR_AQUA     //E_RGBX(  0, 255, 255) or #00ffff 
ECOLOR_ORANGE   //E_RGBX(255, 165,   0) or #ffa500  
```

Color definitions that can be passed as a color parameter. The first sixteen color definitions and their names are taken from the HTML 4.01 specification. The additional orange color is taken from the CSS 2.1 specification.

## Other definitions
```c++
EKERNING_WILDCARD     //Wildcard character for font kerning  
```

Remaining definitions not specified above.

## Version changes
- EDGELIB 4.00: Added flags for font reordering and automatic orientation changing. 
- EDGELIB 3.97: Added EKERNING_WILDCARD 
- EDGELIB 3.90: Added EST_READONLYPATH, EDSP_TRUE24_INV, EDSP_TRUE32_INV, EST_DSPTRUE24_INV and EST_DSPTRUE32_INV 
- EDGELIB 3.10: Changed ROT_ definitions to DOR_, added DOR_STARTUP 
- EDGELIB 3.00: Added surface create flags for 3D surfaces 
- EDGELIB 2.95: Added ROT_PORTRAIT and ROT_LANDSCAPE 
- EDGELIB 2.90: Added EST_MATCHPALETTE 
- EDGELIB 2.50: Added ROT_AUTO 
- EDGELIB 2.30: Added EST_DSPINDEXED8, EST_DSPTRUE12, EST_DSPTRUE15, EST_DSPTRUE16, EST_DSPTRUE24, EST_DSPTRUE32 and EFX_COLORFILL 
- EDGELIB 2.20: Added EST_DEFAULT, E2D_ALLOWNATIVE and E2D_PREVENTNATIVE 
- EDGELIB 2.10: Added EFO_HLEFT, EFO_HRIGHT, EFO_HCENTER, EFO_VTOP, EFO_VBOTTOM and EFO_VCENTER

