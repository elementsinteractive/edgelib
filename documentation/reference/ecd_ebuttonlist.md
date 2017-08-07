:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# EBUTTONLIST


```c++
unsigned long ButtonA 
unsigned long ButtonB 
unsigned long ButtonC 
unsigned long ButtonStart 
unsigned long DPadUp 
unsigned long DPadDown 
unsigned long DPadLeft 
unsigned long DPadRight 
unsigned long JogLeft 
unsigned long JogRight 
unsigned long Soft1 
unsigned long Soft2 
unsigned long Soft3 
unsigned long Soft4 
unsigned long Home 
unsigned long KeyEnter 
unsigned long KeyShift 
unsigned long KeyShift2 
unsigned long KeyControl 
unsigned long KeyControl2 
unsigned long KeyEscape 
unsigned long KeySpace 
unsigned long KeyBackspace 
unsigned long KeyTab 
unsigned long KeyPageup 
unsigned long KeyPagedown 
unsigned long KeyInsert 
unsigned long KeyDelete 
unsigned long KeyHome 
unsigned long KeyEnd 
unsigned long KeyMenu
```

## Members
### ButtonA
This is the code of the first action button.

### ButtonB
This is the code of the second action button.

### ButtonC
This is the code of the third action button.

### ButtonStart
This is the code of the start/enter button.

### DPadUp
This is the code for up movement.

### DPadDown
This is the code for down movement.

### DPadLeft
This is the code for left movement.

### DPadRight
This is the code for right movement.

### JogLeft
This is the code for tilting the jog left.

### JogRight
This is the code for tilting the jog right.

### Soft1
The first software button, depends on the device.

### Soft2
The second software button, depends on the device.

### Soft3
The third software button, depends on the device.

### Soft4
The four software button, depends on the device.

### Home
The home key for Windows Mobile on phone devices, for returning to the home screen.

### KeyEnter
The enter key on a keyboard.

### KeyShift
The shift key on a keyboard.

### KeyShift2
The second shift key on a keyboard.

### KeyControl
The control key on a keyboard.

### KeyControl2
The second control key on a keyboard.

### KeyEscape
The escape key on a keyboard.

### KeySpace
The spacebar on a keyboard.

### KeyBackspace
The backspace key on a keyboard.

### KeyTab
The tab key on a keyboard.

### KeyPageup
The page up key on a keyboard.

### KeyPagedown
The page down key on a keyboard.

### KeyInsert
The insert key on a keyboard.

### KeyDelete
The delete key on a keyboard.

### KeyHome
The home key on a keyboard.

### KeyEnd
The end key on a keyboard.

### KeyMenu
The menu key on Symbian (Nokia Communicator) devices.

## Description
This structure contains the scancodes for the default buttons. This can be a good example for assigning buttons to game interaction but it is still recommended to include a key configuration tool within the game.

## Version changes
- EDGELIB 2.40: Added KeyMenu 
- EDGELIB 2.10: Added secondary shift and control button definitions, added a third and fourth soft button definition

