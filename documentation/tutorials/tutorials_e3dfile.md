:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# The EDGELIB 3D (.e3d) file format specification

## About the file format
The EDGELIB 3D file format (E3D), introduced in EDGELIB 3.30, is an open format for storing 3D model data and is part of the EDGELIB SDK. The file format may be used freely without having to worry about royalties or other types of licensing. More details about the file structure and components are described in detail below.

EDGELIB 3D is a chunk-based binary file format. Chunks are used to describe several data elements, like vertices, polygons and lighting. Each chunk has an identifier indicating which data it contains. Additional and custom chunks may be used to expand the file format. All data is saved as fixed point to keep loading efficient and fast.

**Important: The E3D format is in progress and not complete yet. The file format might change at some points and new chunks will be added in future releases.**

## File layout
The first 12 bytes of the file are used to store header information. The first eight bytes store the file signature and must be E3D_EIBV. The last four bytes store the version of the file. After the 12-byte header the file contains a variable amount of chunks. The end of file is marked by a special 'end of data' chunk.

## Chunks
Chunks are data blocks with a variable size. The first four bytes are used to identify the chunk. The next four bytes tell the size of the chunk excluding the 8 byte chunk header. The identifier of the chunk tells if the chunk is official or custom. Official chunks start with the character 'e' or 'E', custom chunks should start with a different character. If the first character is a capital character, the chunk is mandatory, otherwise it's optional. This is in line with the PNG file format specification.

## Official chunk types
### EMSH (model)
This chunk identifies and stores mesh properties.

```c++
0 : char[32] name 
32 : unsigned long contentflags 
36 : unsigned long vertices count 
40 : unsigned long triangle count 
 
contentflags: 
 1: contains vertex array 
 2: contains triangle array 
 4: contains color array 
 8: contains normal array 
16: contains texture coordinate array
```

### EVTX (vertex array)
This chunk contains the vertex list.


```c++
0: long x 
4: long y 
8: long z 
 
Chunk size = vertex count * 12 
Uses 20:12 fixed point values
```

### ETRI (triangle list)
This chunk contains the triangle list.


```c++
0: unsigned short vertex 1 
2: unsigned short vertex 2 
4: unsigned short vertex 3 
 
Chunk size = triangle count * 3
```

### eCOL (color array)
This chunk contains the vertex color array.


```c++
0: unsigned char blue 
1: unsigned char green 
2: unsigned char red 
3: unsigned char alpha 
 
Chunk size = vertex count * 4
```

### eNRM (normal array)
This chunk contains the vertex normal array for model lighting.


```c++
0: signed char x 
1: signed char y 
2: signed char z 
 
Chunk size = vertex count * 3
```

### eTXC (texture coordinate array)
This chunk contains the vertex texture coordinate array for model texturing.


```c++
0: long u 
4: long v 
 
Chunk size = vertex count * 8 
Uses 16:16 fixed point values
```

### _END (end-of-file)
This chunk indicates when the file is ended. This chunk type has no size.

