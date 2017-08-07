:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Main entry function: EdgeMain


```c++
ClassEdge *EdgeMain(EDGESTARTUP *data)
```

## Parameters
### *data (out)
A pointer to an [`EDGESTARTUP` structure](ecd_edgestartup.md). This structure doesn't have to be allocated and is used for retrieving startup information like the command line.

## Return value
An instance of a ClassEdge derived class.

## Description
This is the function that needs to be created, and this is where the program starts (a replacement for the `main()` and `WinMain()` functions; these shouldn't be written in a program that uses Edge).

## Version changes
- EDGELIB 3.10: The return value is changed to an instance of a ClassEdge derived class, EDGEDATA is changed to EDGESTARTUP

