:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# EDISPLAYCONFIG


```c++
WCHAR caption[MAX_ESTR] 
unsigned long icon 
unsigned long menu 
unsigned long dialogbox 
unsigned long width 
unsigned long height 
unsigned long dlgleft 
unsigned long dlgtop 
unsigned char videotype 
unsigned char emulvideotype 
unsigned char orientation 
unsigned char openmethod 
unsigned char surfacedefault 
unsigned char emulatelowres 
unsigned char supportedorientations 
bool fullscreen 
bool fullscreenmouse 
bool dspnative 
bool enginestatus 
bool engineconsole 
bool surfprerotate 
bool stencil
```

## Members
### caption
The title of the application window for Win32 builds. 
Default setting: "EDGELIB application"

### icon
The icon of the application window for Win32 builds. 
Default setting: default application icon (0)

### menu
The window menu for Win32 builds. 
Default setting: disabled (0)

### dialogbox
The resource ID of a dialog box to start EDGELIB from (Windows desktop only). 
Default setting: disabled (0)

### width, height
The desired resolution or size of the window for Windows desktop applications. 
Default setting: 240x320 pixels

### dlgleft, dlgtop
The amount of pixels as a topleft margin when using an EDGELIB window inside a dialog box. 
Default setting: 0x0 pixels

### videotype
The desired [display mode](classedisplay_definitions.md) for desktop applications. 
Default setting: `EDSP_DEFAULT`

### emulvideotype
Forces a display mode if not set to EDSP_DEFAULT. This can be used if a specific display mode is desired or can be used for simulating displays with a low amount of colors. 
Default setting: `EDSP_DEFAULT`

### orientation
The [display orientation](classedisplay_definitions.md). This can be used to rotate the screen. 
Default setting: `DOR_NONE`

### openmethod
Set the recommended display [open method](ecd_definitions.md). Use DOM_DEFAULT to make sure the application works on all devices with the best performance. 
Default setting: `DOM_DEFAULT`

### surfacedefault
Set the default creation type for new surfaces. 
Default setting: `EST_SYSMEMORY`

### supportedorientations
Sets the [orientations](ecd_definitions.md) that are allowed to be used when orientation is set to DOR_AUTO 
Default setting: `ESO_ALL`

### fullscreen
Use the entire screen. 
Default setting: false for Windows desktop, true for all mobile and handheld devices

### fullscreenmouse
Enable the mouse cursor within fullscreen applications. 
Default setting: false

### dspnative
Set this to true if the native API should handle internal graphics operations (if supported). 
Default setting: true

### enginestatus
Enable the docked statusbar for Windows desktop based applications. 
Default setting: true

### engineconsole
Enable the Edge debugging console. 
Default setting: true

### surfprerotate
Enables pre-rotated surfaces to ensure high performance on all devices. Set this to false to make sure surfaces don't have odd pitches. This setting must be true on Symbian 9+ devices. 
Default setting: true

### emulatelowres
Emulate a specific generation's display resolution if nonzero. 0 = Use maximum resolution, 1 = use 320x480 (iPhone) 
Default setting: 0

### stencil
Request a stencil buffer to be created for an OpenGL context when possible. 
Default setting: false

## Version changes
- EDGELIB 4.00: Added support for DOR_AUTO on iOS, added stencil and supportedorientations option. 
EDGELIB 3.987: Added emulatelowres option. 
- EDGELIB 3.10: Split from EDGECONFIG, added surfprerotate and openmethod, removed resolution and flipmethod, renamed width and height 
- EDGELIB 2.90: Added emulvideotype configuration option 
- EDGELIB 2.20: Added surfacedefault and dspnative configuration options 
- EDGELIB 2.10: Changed the highresolution configuration option to resolution, added more configuration options

