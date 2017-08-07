:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Definitions

## Opening types
```c++
EFOF_DEFAULT       //Default opening mode 
EFOF_READ          //Open the file for reading only 
EFOF_WRITE         //Open the file for writing only 
EFOF_READWRITE     //Open the file for reading and writing 
EFOF_READONLYPATH  //Open the file in the read-only path  
```

File open modes. EFOF_DEFAULT opens the file for reading and writing.

## Attribute flags
```c++
EFAF_NOTAVAILABLE  //The file doesn't exist or file attribute reading has failed 
EFAF_NORMAL        //A normal file 
EFAF_READONLY      //The file can only be read, not deleted or written to 
EFAF_HIDDEN        //The file is hidden from the user 
EFAF_SYSTEM        //A file used by the system, these should be treated with care 
EFAF_ARCHIVE       //A file marked for archiving 
EFAF_FOLDER        //Used as a folder for storing files  
```

File or folder attribute flags.

## Compression types
```c++
ECMP_NONE    //No compression 
ECMP_EDGE    //Internal EDGELIB compression algorithm 
ECMP_ZLIB    //Compression using ZLib  
```

Methods for data compression.

## Version changes
- EDGELIB 3.90: Added EFOF_READONLYPATH

