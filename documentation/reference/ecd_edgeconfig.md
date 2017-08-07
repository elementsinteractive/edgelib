:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# EDGECONFIG (deprecated)


```c++
unsigned long ffwdkey 
unsigned long slomokey 
unsigned long maxfps 
unsigned long flags 
unsigned long videowidth 
unsigned long videoheight 
unsigned long dlgleft 
unsigned long dlgtop 
unsigned char videotype 
unsigned char emulvideotype 
unsigned char rotation 
unsigned char numconnections 
unsigned char flipmethod 
unsigned char resolution 
unsigned char surfacedefault 
bool smartfps 
bool focusminimize 
bool enginestatus 
bool engineconsole 
bool fullscreen 
bool fullscreenmouse 
bool jogtiltarrow 
bool focussuspend 
bool dspnative 
bool backlighton
```

## Version changes
- EDGELIB 3.10: this structure is deprecated. The structure is split into [EFRAMEWORKCONFIG](ecd_eframeworkconfig.md) and [EDISPLAYCONFIG](ecd_edisplayconfig.md). 
- EDGELIB 2.95: added backlighton configuration option 
- EDGELIB 2.90: added emulvideotype configuration option 
- EDGELIB 2.20: added surfacedefault and dspnative configuration options 
- EDGELIB 2.10: changed the highresolution configuration option to resolution, added more configuration options

