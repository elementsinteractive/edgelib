:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# LoadKeyDescriptor


```c++
void LoadKeyDescriptor(const char *keylist) 
void LoadKeyDescriptor(const WCHAR *keylist)
```

## Parameters
### *keylist (in)
An enter-separated array of names.

## Return value
None.

## Description
Change key names returned by [GetKeyName](classeinput_getkeyname.md), such as Enter, Backspace, Jog and more.

## Example
Here is an example on how to set a custom key descriptor:

```c++
const char keydescriptor[] = " \ 
Up\n\ 
Down\n\ 
Left\n\ 
Right\n\ 
F1\n\ 
F2\n\ 
F3\n\ 
F4\n\ 
F5\n\ 
F6\n\ 
F7\n\ 
F8\n\ 
F9\n\ 
F10\n\ 
F11\n\ 
F12\n\ 
Tab\n\ 
Space\n\ 
Enter\n\ 
Shift\n\ 
Control\n\ 
Alt\n\ 
Esc\n\ 
Insert\n\ 
Delete\n\ 
Home\n\ 
End\n\ 
Pageup\n\ 
Pagedown\n\ 
Capslock\n\ 
Numlock\n\ 
Scroll lock\n\ 
Printscreen\n\ 
Pause\n\ 
Backspace\n\ 
Softkey 1\n\ 
Softkey 2\n\ 
Softkey 3\n\ 
Softkey 4\n\ 
Menu\n\ 
Start\n\ 
Select\n\ 
Jog up\n\ 
Jog down\n\ 
Jog\n\ 
Volume up\n\ 
Volume down\n\ 
"; 
     
ecd.input->LoadKeyDescriptor(keydescriptor);
```

## Version changes
- EDGELIB 2.90: Added this function to the SDK

