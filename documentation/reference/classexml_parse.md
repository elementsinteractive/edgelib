:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Parse


```c++
bool Parse(bool storenodes = true, EXml_Callback callback = NULL, void *userdata = NULL)
```

## Parameters
### storenodes (in)
If set to true, the XML parser will store nodes.

### callback (in)
A callback function which handles the XML parser events. If set to NULL, it will be ignored.

### *userdata (in)
A pointer to pass through the callback function with user data.

## Return value
Returns true when successful.

## Description
Parses an XML string and generates nodes and/or events through the callback function.

## Example
Here is an example of using the callback function:

```c++
bool XmlCallback(ClassEXml *parser, unsigned char event, const WCHAR *name, const WCHAR *attribute, const WCHAR *value, void *userdata) 
{ 
    switch(event) 
    { 
        case EXML_CLOSETAG: 
            //A tag is closed: 
            //- name contains the name of the tag 
            break; 
        case EXML_ADDATTRIBUTE: 
            //A tag attribute is added 
            //- name contains the name of the tag 
            //- attribute contains the attribute name 
            //- value contains the attribute value 
            break; 
        case EXML_CLOSEATTRIBUTES: 
            //End of attribute list 
            //- name contains the name of the tag 
            break; 
        case EXML_ADDCONTENT: 
            //Content is being added 
            //- name contains the name of the tag 
            //- value contains content 
            break; 
        case EXML_OPENTAG: 
            //A tag is opened: 
            //- name contains the name of the tag 
            break; 
    } 
    return(true); 
}
```

## Version changes
- EDGELIB 3.80: Changed parameters and added storing of XML nodes 
- EDGELIB 3.30: Added this function to the EDGELIB SDK

