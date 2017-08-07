:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Global structures

## RECT
```c++
struct 
{ 
        long left 
        long top 
        long right 
        long bottom 
}RECT
```

This defines a rectangle. They are used for 2D graphics operations, and can be quickly set using the [`SetRect`](ref_globalfunctions.md) function.

## POINT
```c++
struct 
{ 
        long x 
        long y 
}POINT
```

This defines a 2D point.

## E_DATETIME
```c++
typedef struct 
{ 
    unsigned long year 
    unsigned char month 
    unsigned char day 
    unsigned char hour 
    unsigned char minute 
    unsigned char second 
}E_DATETIME
```

A date, separated in year, month, day, hour, minute and second.

## Version changes
- EDGELIB 2.60: Changed the year member of E_DATETIME to unsigned long

