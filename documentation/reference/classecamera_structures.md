:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Structures

## E_CAMERAMODE
```c++
typedef struct 
{ 
    long x 
    long y 
    long format 
    long expectedfps 
} E_CAMERAMODE
```

The configuration structure for camera modes. The format refers to the [data being sent](classecamera_definitions.md)

## E_CAMERAFRAME
```c++
typedef struct 
{ 
    const unsigned char * data 
    long width 
    long height 
    long pitch 
    long format 
    long orientation 
} E_CAMERAFRAME
```

The structure of a camera frame.

