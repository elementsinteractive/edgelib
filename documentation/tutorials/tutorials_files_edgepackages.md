:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# EDGELIB package

## Overview
EDGELIB packages are multiple files packed into one file created by the EDGELIB Packer tool. Each package has a vendor signature, a four character code indicating which developer owns the package.

Using EDGELIB package files has several advantages. Game resources can be managed easier, can be compressed using ZLib, are accessible the same way on every supported platform and are a bit harder to rip, because each package is locked.

EDGELIB licensees can request a unique four character code to use as a vendor signature. When the signature has been approved, an authorization code will be sent for unlocking packages. A vendor code available to everyone is 'FREE' and unlocks with code 0.

The EDGELIB Packer tool for Windows desktop is included in the SDK to easily create and manage packages through a user friendly user interface or using commandline options.

## Package file functions
There are several functions available for managing EDGELIB packages:

* Creating, opening, closing and authorization of packages
* Reading file information
* Adding, overwriting, deleting and extracting files

## Creating, Opening, closing and authorization of packages
A new package can be created by a simple function call. After creating, the package file is immediately opened for adding files. A vendor signature has to be given as well:

_Code (2 lines)_
```c++
ClassEFile ef;
ef.PackNew("resource.epk", "FREE");
```

A package file can be handled the same way as a regular file. Opening and closing of packages are done by the same method as opening regular files. The only difference is that an EDGELIB package file needs to be authorized with an unlock code. It also checks if the file is an EDGELIB package or a regular file. The functions for managing package files won't work when a package hasn't been authorized. The following example opens, authorize and will close an EDGELIB package file:

_Code (9 lines)_
```c++
ClassEFile ef;
if (ef.Open("resource.epk"))
{
    if (ef.PackAuthorize(0))
    {
        //start reading files
    }
    ef.Close();
}
```

## Reading file information
After authorizing a package file, detailed information can be read. This includes the number of files present in the package, information about each individual file and retrieving file data. The following sample will read a list of files and file data. Please note that when requesting file data, a chunk of memory is allocated and needs to be deallocated afterwards. The sample assumes the package is already opened and authorized.

_Code (10 lines)_
```c++
EPK_FILEINFO fileinfo;
unsigned long ctr, memsize;
unsigned char *filemem;
for (ctr = 0; ctr < ef.PackContainFiles(); ctr++)
{
    ef.PackGetFileInfo(&fileinfo, ctr);
    ef.PackGetFileData(filemem, memsize, ctr);
    //Use filemem for loading data
    ClassEMemory::DeAlloc(filemem);
}
```

It's also possible to search for a filename within an EDGELIB package. It will return a file ID which can be used for retrieving file information and file data:

_Code (2 lines)_
```c++
EPK_FILEINFO fileinfo;
ef.PackGetFileInfo(&fileinfo, "test.txt");
```

## Adding, overwriting, deleting and extracting files
After opening or creating a new package file, files can be added to the package. Files can be added directly, or files can be added by providing memory data. Each function has two types, one that generates an error if the file already exists, and another that will overwrite existing files. The following sample adds files directly and through memory data, overwriting the existing data:

_Code (4 lines)_
```c++
char memdata[128];
ClassEStd::StrCpy(memdata, "EDGELIB package tutorial sample");
ef.PackAddOverwriteFile("test.txt");
ef.PackAddOverwriteFileData("test_mem.txt", memdata, ClassEStd::StrLen(memdata));
```

Files added to the package are compressed if the compressed data isn't larger than the original data.

It's also possible to delete a file from a package. The following sample will demonstrate how to do this:

_Code (1 line)_
```c++
ef.PackDeleteFile("test.txt");
```

Finally, files can also be extracted directly to the file system. The following sample will extract a file `test2.txt` into the program folder:

_Code (1 line)_
```c++
ef.PackExtractFile(ecd.dat->currentpath, "test2.txt");
```

## Chapters
* [Basic file functions](tutorials_files_basic.md)
* [Reading folders](tutorials_files_folders.md)
* **EDGELIB package**

