:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# AddFile


```c++
bool AddFile(const char *name, const char *filename, const char *filemimetype = NULL) 
bool AddFile(const char *name, ClassEFile *ef, const char *filemimetype = NULL)
```

## Parameters
### *name (in)
A name which can be given to the file.

### *filename (in)
The name of the file to add.

### *ef (in)
A pointer to a [ClassEFile object](classefile.md) with an opened file.

### *filemimetype (in)
The mimetype of the file, when it isn't specified it will be based on the filename string extension or will be "application/octet-stream".

## Return value
Returns true if file has been added, otherwise returns false.

## Description
Add the contents of a file to a HTTP POST request for sending. This data will be automatically loaded from the specified file and added to the request. Data can only be posted with the [multipart HTTP POST](classeconnect_sendhttppostmultipart.md) method.

### Built-in mimetypes
.avi:	video/x-msvideo 
.c:		text/plain 
.cc:		text/plain 
.cpp:	text/plain 
.css:	text/css 
.doc:	application/msword 
.gif:	image/gif 
.gz:		application/x-gzip 
.h:		text/plain 
.htm:	text/html 
.html:	text/html 
.jpe:	image/jpeg 
.jpg:	image/jpeg 
.jpeg:	image/jpeg 
.js:		application/x-javascript 
.mid:	audio/midi 
.midi:	audio/midi 
.mov:	video/quicktime 
.mp2:	audio/mpeg 
.mp3:	audio/mpeg 
.mpe:	video/mpeg 
.mpeg:	video/mpeg 
.mpg:	video/mpeg 
.mpga:	audio/mpeg 
.png:	image/png 
.rtf:	text/rtf 
.rtx:	text/richtext 
.tif:	image/tiff 
.tiff:	image/tiff 
.txt:	text/plain 
.vrml:	model/vrml 
.wav:	audio/x-wav 
.wrl:	model/vrml 
.xbm:	image/x-xbitmap 
.xml:	text/xml 
.zip:	application/zip

