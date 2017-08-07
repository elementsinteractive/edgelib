:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetFontKerning


```c++
void SetFontKerning(WCHAR charleft, WCHAR charright, long kerning)
```

## Parameters
### charleft (in)
The left character of the combination.

### charright (in)
The right character of the combination.

### kerning (in)
The horizontal spacing between the 2 characters, can be a negative value.

## Return value
None.

## Description
Specify kerning between 2 font characters. To support wildcards (for example, any character after 'i' should be moved one pixel to the left), specify EKERNING_WILDCARD as a character.

