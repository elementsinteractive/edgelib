:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Structures

## E_FILEINFO
```c++
struct 
{ 
    unsigned short attributes 
    unsigned long filesize 
    E_DATETIME datecreated 
    E_DATETIME datemodified 
}E_FILEINFO
```

This structure contains the file attributes and filedate information.

## EPK_FILEINFO
```c++
struct 
{ 
    char filename[64] 
    unsigned long orgsize 
    unsigned long packsize 
    unsigned char compresstype 
    ETYPE_TIMESTAMP filedate 
    ETYPE_TIMESTAMP packdate 
}EPK_FILEINFO
```

This contains general file information about a file inside a resource pack.

## Version changes
- EDGELIB 3.10: Added filesize to E_FILEINFO 
- EDGELIB 2.70: Changed E_TIMESTAMP to ETYPE_TIMESTAMP 
- EDGELIB 2.60: Major changes to EPK_FILEINFO

