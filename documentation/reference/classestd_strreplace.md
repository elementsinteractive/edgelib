:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# StrReplace


```c++
int static StrReplace(char *src, const char *search, const char *replace, bool casesensitive = true, int max_replace = -1) 
int static StrReplace(WCHAR *src, const WCHAR *search, const WCHAR *replace, bool casesensitive = true, int max_replace = -1) 
int static StrReplace(WCHAR *src, const char *search, const char *replace, bool casesensitive = true, int max_replace = -1) 
int static StrReplace(WCHAR *src, const char *search, const WCHAR *replace, bool casesensitive = true, int max_replace = -1) 
int static StrReplace(WCHAR *src, const WCHAR *search, const char *replace, bool casesensitive = true, int max_replace = -1)
```

## Parameters
### *src (in/out)
The source string to scan.

### *search (in)
The string to search for.

### *replace (in)
The string to replace with.

### casesensitive (in)
Indicate if the search should be case sensitive.

### max_replace (in)
The maximum amount of replaces before the function ends. Pass -1 to replace all occurrences.

## Return value
The amount of replaces.

## Description
Replace one or multiple occurrences of a certain substring in a string.

## Version changes
EDGELIB 3.987: Added versions with mixed wide and non-wide arguments 
- EDGELIB 2.50: Added this function to the SDK

