:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SubStr


```c++
void SubStr(EString &str, long start, long length = 0) 
EString &SubStr(long start, long length = 0);
```

## Parameters
### &str (out)
Copies the sub string to this new object.

### start (in)
When positive, the first character from left to right. When negative, the first character from right to left.

### length (in)
When positive the amount of characters to take. When negative the ending character counted from the right.

## Return value
A reference to itself.

## Description
Takes a substring and discards all other characters. It's possible to modify the string object itself, or copy the substring to a new object.

## Version changes
- EDGELIB 3.30: Added this function to the EDGELIB SDK

