:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# ClassEIni

This is an INI module containing functions to read and write INI (initialization) files. This can be used to handle saved games or other game data.

The INI files supported by EDGELIB contain lines with pairs of keys and values separated by an equals sign (=). Lines starting with a hash sign (#) are treated as comments. Values can be enclosed by double quotes.

A simple INI file could look like this:

_Code (3 lines)_
```c++
# Paths
edgepath     = "c:\projects\edge"
incpath      = "%edgepath%\include"
```

## Class members
* [Close](classeini_close.md)
* [FindKey](classeini_findkey.md)
* [FindValue](classeini_findvalue.md)
* [New](classeini_new.md)
* [Open](classeini_open.md)
* [OpenData](classeini_opendata.md)
* [ReadLine](classeini_readline.md)
* [ResetCursor](classeini_resetcursor.md)
* [SetParseBufferSize](classeini_setparsebuffersize.md)
* [WriteComment](classeini_writecomment.md)
* [WriteLine](classeini_writeline.md)
* [WriteNewline](classeini_writenewline.md)

## Version changes
- EDGELIB 2.50: Added this class to the SDK

