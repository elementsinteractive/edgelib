:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# EDGELIB Common Data (ECD)

The EDGELIB Common Data (ECD) structures provide an interface to the other modules of the API. These modules are members of the ECD member variable within the wrapper class.

This page also contains the description of other structures commonly used in EDGELIB.


```c++
ClassEStd        *std 
ClassEDisplay    *dsp 
ClassEInput      *input 
ClassEVTimer     *tmr 
ClassEFile       *file 
ClassEIni        *ini 
ClassEMemory     *mem 
ClassEMath       *math 
ClassERegister   *reg 
ClassEDevice     *device 
ClassEConsole    *console 
ClassEConnect    *net[16] 
EFRAMEWORKCONFIG *maincfg 
EDISPLAYCONFIG   *dispcfg 
EDGEDATA         *dat
```

## Members
### *std
A pointer to [`ClassEStd`](classestd.md), a module containing useful functions.

### *dsp
A pointer to [`ClassEDisplay`](classedisplay.md), the video display module.

### *input
A pointer to [`ClassEInput`](classeinput.md), the input module.

### *tmr
A pointer to [`ClassEVTimer`](classevtimer.md), the virtual timer module.

### *file
A pointer to [`ClassEFile`](classefile.md), the extended file input/output module.

### *ini
A pointer to [`ClassEIni`](classeini.md), the ini file module.

### *mem
A pointer to [`ClassEMemory`](classememory.md), the memory manager module.

### *math
A pointer to [`ClassEMath`](classemath.md), the built-in math module.

### *reg
A pointer to [`ClassERegister`](classeregister.md), a game registration helper module.

### *device
A pointer to [`ClassEDevice`](classedevice.md), a device interface module.

### *console
A pointer to [`ClassEConsole`](classeconsole.md), an engine console module.

### *net
An array of pointers to [`ClassEConnect`](classeconnect.md), the network connection module.

### *maincfg
A pointer to [`EFRAMEWORKCONFIG`](ecd_eframeworkconfig.md), the framework configuration.

### *dispcfg
A pointer to [`EDISPLAYCONFIG`](ecd_edisplayconfig.md), the display configuration.

### *dat
A pointer to [`EDGEDATA`](ecd_edgedata.md), the application data.

## Description
This structure contains the EDGELIB data structure, the EDGELIB configuration structure and pointers to each EDGELIB module.

## Other structures and definitions
* [EBUTTONLIST](ecd_ebuttonlist.md)
* [EDGECONFIG (deprecated)](ecd_edgeconfig.md)
* [EDGEDATA](ecd_edgedata.md)
* [EDGESTARTUP](ecd_edgestartup.md)
* [EDISPLAYCONFIG](ecd_edisplayconfig.md)
* [EFRAMEWORKCONFIG](ecd_eframeworkconfig.md)
* [Definitions](ecd_definitions.md)

## Version changes
- EDGELIB 3.10: Splitted EDGECONFIG into EFRAMEWORKCONFIG and EDISPLAYCONFIG 
- EDGELIB 2.70: Added ClassEInput to ECD 
- EDGELIB 2.50: Added ClassEIni to ECD

