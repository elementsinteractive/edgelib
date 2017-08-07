:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PackBuildIndex


```c++
char *PackBuildIndex(void)
```

## Parameters
None.

## Return value
A pointer to the file index.

## Description
Create a file index for quick file searching in the pack file. Create it once before loading data to increase loading speed. After reading data and the pack file is closed, free the pointer with [ClassEMemory::DeAlloc](classememory_dealloc.md). The file index may also be reused if the pack file is reopened. When reusing the file index, set it to the ClassEFile object using [PackUseIndex](classefile_packuseindex.md).

## Example
Here is an example on how to load two pictures from a pack file using the index:

```c++
bool LoadPictures(ClassEDisplay *display, E2DSurface *picture1, E2DSurface *picture2) 
{ 
    ClassEFile ef; 
    if (ef.Open("sprites.epk", EFOF_READ)) 
    { 
        if (ef.PackAuthorize(0x00000000)) 
        { 
            bool result = true; 
            char *index; 
            unsigned char *fdata; 
            unsigned long fsize; 
            index = ef.PackBuildIndex(); 
            if (ef.PackGetFileData(fdata, fsize, "sprite1.bmp") == E_OK) 
            { 
                if (display->CreateSurface(picture1, fdata, fsize) != E_OK) 
                    result = false; 
                ClassEMemory::DeAlloc(fdata); 
            } 
            else 
                result = false; 
            if (ef.PackGetFileData(fdata, fsize, "sprite2.bmp") == E_OK) 
            { 
                if (display->CreateSurface(picture2, fdata, fsize) != E_OK) 
                    result = false; 
                ClassEMemory::DeAlloc(fdata); 
            } 
            else 
                result = false; 
            ClassEMemory::DeAlloc(index); 
            return(result); 
        } 
        ef.Close(); 
    } 
    return(false); 
}
```

## Version changes
- EDGELIB 2.80: Added this function to the SDK

