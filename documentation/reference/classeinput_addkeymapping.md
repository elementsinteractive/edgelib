:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# AddKeyMapping


```c++
bool AddKeyMapping(unsigned char action, unsigned long nr) 
bool AddKeyMapping(unsigned char action, unsigned long count, int nr, ...)
```

## Parameters
### action (in)
A user defined action to map. Actions range from 0 to 128

### nr (in)
The key to map to this action

### count (in)
The amount of keys to map to this action

### ... (in)
Additional keys to map to this action, should be of type int

## Return value
Returns true if the key is successfully mapped, otherwise returns false.

## Description
Maps a key to a user defined action. Multiple keys can be mapped to the same action.

## Example
Here are small examples on how to map two custom actions with one and multiple keys:

```c++
//Define custom actions 
#define ACTION_FIRE           0 
#define ACTION_SWITCHWEAPON   1 
 
//Initialize application 
ERESULT ClassMain::OnInit(EINSTANCE instance) 
{ 
    ecd.input->AddKeyMapping(ACTION_FIRE, e_buttons.ButtonA); 
    ecd.input->AddKeyMapping(ACTION_SWITCHWEAPON, 4, e_buttons.ButtonB, e_buttons.ButtonC, e_buttons.Soft1, e_buttons.Soft2); 
    return(E_OK); 
}
```

## Version changes
- EDGELIB 2.70: Added this function to the SDK

