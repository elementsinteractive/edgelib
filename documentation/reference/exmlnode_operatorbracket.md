:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# operator[]


```c++
EXmlNode &operator[](int index) 
EXmlNode &operator[](const char *name)
```

## Parameters
### index (in)
Searches a child node by index.

### name (in)
Searches a child node by name.

## Return value
A reference to a child node based on the index or name parameter.

## Description
Use the bracket operator to index child nodes easily. If it can't be found, it returns a dummy node with an error name and value.

## Example
Here is an example to return the value of one of its child nodes:

```c++
WCHAR *url = root["data"][0]["image"].GetValue(). 
ClassEConsole::MsgWindow(url);
```

## Version changes
- EDGELIB 3.80: Added this operator to the EDGELIB SDK

