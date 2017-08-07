:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Basic file functions

## Available functions
EDGELIB contains a number of functions to read, write and manipulate files. These include:

* Open, close and create new files
* Read and write file data
* Move the file index pointer and reading the size of a file
* Getting more information about files or folders
* Rename and delete

## How to open, close or create a new file
Before reading or writing file information, it's needed to open the file. After a file has been opened, it's wise to close it as soon as possible when it isn't needed anymore. This will ensure all file data is written to keep the file up-to-date.

Here is an example on how to open a file:

_Code (7 lines)_
```c++
ClassEFile ef;
if (ef.Open("log.txt", EFOF_READ))
{
    ef.Close();
    return(true);
}
return(false);
```

The second argument when opening a file indicates the way how to open the file. The following types can be used:

`EFOF_DEFAULT`: The default open mode, it opens the file for reading and writing. 
`EFOF_READ`: Opens the file only for reading. 
`EFOF_WRITE`: Opens the file for writing. 
`EFOF_READWRITE`: Opens the file for reading and writing.

Creating a new file is similar to opening a file. When a new file is created, it will be opened for reading and writing:

_Code (1 line)_
```c++
ef.New("new.txt");
```

Creating a new folder goes the same way, only it can't be opened for reading or writing:

_Code (1 line)_
```c++
ef.NewFolder("newfolder");
```

## Reading and writing file data
After opening a file, it can be read or written to. The file contains a file pointer which keeps track on which byte of the file will be read or written to. The file pointer will move after a read or write operation. For example: the file pointer starts at 0 and when 10 bytes are written the file pointer will be 10. Here is a sample how to write and read file data:

_Code (4 lines)_
```c++
char filetext[MAX_ESTR];
ef.Read(filetext, MAX_ESTR);
ClassEStd::StrCpy(filetext, "Updated file text");
ef.Write(filetext, MAX_ESTR);
```

## Getting file or folder information
A file or folder contains data and attributes. These attributes contain the date of creation, modification date and a couple of flags telling if the file can only be read, or if it's hidden. Here is a code sample on how to read more information about a file:

_Code (5 lines)_
```c++
ClassEFile ef;
E_FILEINFO fileinfo;
unsigned short attrib_flags;
ef.GetInformation(&fileinfo, "file.txt");
attrib_flags = ef.GetAttributes("file.txt");
```

## Other file functions
EDGELIB also supports a number of additional file functions. These include functions to read and move the file pointer, reading the size of a file and setting the size of a file. Here is a sample that reads and moves the file pointer:

_Code (4 lines)_
```c++
unsigned long filepointer = 0;
filepointer = ef.Tell();
filepointer += 128;
ef.Seek(filepointer);
```

The following example explains how to retrieve the size of the file, and expanding the file:

_Code (4 lines)_
```c++
unsigned long filesize = 0;
filesize = ef.Size();
filesize += 128;
ef.SetSize(filesize);
```

When a file or empty folder isn't needed anymore, it can be deleted by the file class. It's also possible to quickly rename or move a file or folder. Here is a code sample:

_Code (5 lines)_
```c++
ClassEFile ef;
ef.Delete("log_old.txt");
ef.Rename("log_old.txt", "log.txt");
ef.DeleteFolder("log_backups");
ef.Rename("log_backups", "log_files");
```

## Chapters
* **Basic file functions**
* [Reading folders](tutorials_files_folders.md)
* [EDGELIB package](tutorials_files_edgepackages.md)

