:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# AddData


```c++
bool AddData(const char *name, const unsigned char *buffer, unsigned long bufsize, const char *filename = NULL, const char *datamimetype = NULL)
```

## Parameters
### *name (in)
The name of the data to add.

### *buffer (in)
The data buffer to add.

### bufsize (in)
The size of the data buffer.

### *filename (in)
The filename to identify the data with.

### *datamimetype (in)
The mimetype of the data, when it isn't specified it will be based on the filename string extension or will be "application/octet-stream".

## Return value
Returns true if data has been added, otherwise returns false.

## Description
Add data to send with the HTTP request. Data can only be posted with the multipart HTTP POST method.

