:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Structures

## E2DSURFACEINFO
```c++
typedef struct 
{ 
    long xpitch 
    long ypitch 
    long realpitch 
    long bufferoffset 
    unsigned long width 
    unsigned long height 
    unsigned long realwidth 
    unsigned long realheight 
    unsigned char bitwidth 
    unsigned char pixelformat 
}E2DSURFACEINFO
```

This structure contains detailed information about a surface.

## E3DSURFACEINFO
```c++
typedef struct 
{ 
    long *vertexarray 
    signed char *normalarray 
    COLORREF *colorarray 
    long *texcoordarray 
    unsigned short *indexarray 
    long *matrix 
    long vertexstride 
    long normalstride 
    long colorstride 
    long texcoordstride 
    bool useindices 
}E3DSURFACEINFO
```

This structure contains detailed information about a 3D surface. Values are in fixed point.

## E2DBLTFX
```c++
typedef struct 
{ 
    long flags 
    unsigned long property 
    unsigned long xscale 
    unsigned long yscale 
    unsigned long rotation 
    unsigned char flipflags 
}E2DBLTFX
```

This structure contains properties for the [BltFx](e2dsurface_bltfx.md) function.

## EVECTOR3D
```c++
typedef struct 
{ 
    long x 
    long y 
    long z 
}EVECTOR3D
```

A vector in 3D space.

## EVERTEX3D
```c++
typedef struct 
{ 
    EVECTOR3D position 
    EVECTOR3D normal 
    COLORREF color 
    long u 
    long v 
}EVERTEX3D
```

A vertex in 3D space.

## Version changes
- EDGELIB 3.60: Changed E3DSURFACEINFO 
- EDGELIB 3.00: Added E3DSURFACEINFO, changed EVERTEX3D 
- EDGELIB 2.40: Added E2DBLTFX 
- EDGELIB 2.30: Added the bufferoffset structure member to E2DSURFACEINFO to get the correct buffer pointer for virtual access to the surface 
- EDGELIB 2.20: Added E2DSURFACEINFO, EVECTOR3D and EVERTEX3D structures to the SDK

