:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Definitions

## Input event types
```c++
EIE_NONE           //No event 
EIE_PRESSED        //A one-time press event 
EIE_DOUBLEPRESSED  //A double press event 
EIE_REPEAT         //A repeat event 
EIE_RELEASED       //A release event  
```

Input device events. When an input device is held for a longer time, multiple repeat events are sent.

## Click device buttons
```c++
EIC_ALL     //Check for all buttons 
EIC_LEFT    //Check for the left button 
EIC_MIDDLE  //Check for the middle button 
EIC_RIGHT   //Check for the right button 
EIC_TOUCH1  //The first touch position 
EIC_TOUCH2  //The second touch position 
EIC_TOUCH3  //The third touch position 
EIC_TOUCH4  //The fourth touch position 
EIC_TOUCH5  //The fifth touch position  
```

Buttons for the click device. EIC_ALL will check for all buttons at the same time. Touches are alternatives for multitouch devices and work similar to buttons.

## Default action mappings
```c++
DIAM_ARROWUP 
DIAM_ARROWDOWN 
DIAM_ARROWLEFT 
DIAM_ARROWRIGHT      //Arrow movement 
DIAM_SOFTKEY1 
DIAM_SOFTKEY2 
DIAM_SOFTKEY3 
DIAM_SOFTKEY4        //Soft keys 
DIAM_VOLUMEUP 
DIAM_VOLUMEDOWN      //Volume control 
DIAM_MENUUP 
DIAM_MENUDOWN 
DIAM_MENULEFT 
DIAM_MENURIGHT       //Menu movement 
DIAM_MENUCONFIRM 
DIAM_MENUCONFIRM_SP 
DIAM_MENUCANCEL      //Menu confirm and cancel 
DIAM_MENUSOFT1 
DIAM_MENUSOFT2       //Menu softkey interface 
DIAM_KBTYPEKEY       //Type key for soft keyboards 
DIAM_GAMEUP 
DIAM_GAMEDOWN 
DIAM_GAMELEFT 
DIAM_GAMERIGHT 
DIAM_GAMEACTION1 
DIAM_GAMEACTION2 
DIAM_GAMEPAUSE       //In-game mappings  
```

These mappings can be used by default for actions in your game. DIAM_MENUCONFIRM_SP is an alternative to DIAM_MENUCONFIRM which is softkey proof. This means the keys mapped to this action aren't mapped to the softkeys, useful for menu interfaces requiring menu confirm and the two softkeys.

## Version changes
- EDGELIB 3.95: Added EIC_TOUCH definitions 
- EDGELIB 2.90: Added DIAM_MENUCONFIRM_SP

