:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# ClassEInput

The EDGELIB input module makes it easier to handle various types of game input. It maps various actions to several keys and reads the status of the mapped actions and other input devices. It also contains a character buffer useful for entering player names, etc.

It is recommended to use the input class linked to [ECD](ecd.md) but it is possible to create new input objects as well. However, the key and click events need to be passed manually for them to work.

## Class members
* [AddKeyMapping](classeinput_addkeymapping.md)
* [CheckRepeat](classeinput_checkrepeat.md)
* [ClickHold](classeinput_clickhold.md)
* [GetButtonList](classeinput_getbuttonlist.md)
* [GetChar](classeinput_getchar.md)
* [GetClickEvent](classeinput_getclickevent.md)
* [GetClickPosition](classeinput_getclickposition.md)
* [GetClickPressCount](classeinput_getclickpresscount.md)
* [GetKeyEvent](classeinput_getkeyevent.md)
* [GetKeyName](classeinput_getkeyname.md)
* [GetKeyPressCount](classeinput_getkeypresscount.md)
* [GetLastKey](classeinput_getlastkey.md)
* [GetTiltAngle](classeinput_gettiltangle.md)
* [GetTiltData](classeinput_gettiltdata.md)
* [GetWheelScroll](classeinput_getwheelscroll.md)
* [KeyHold](classeinput_keyhold.md)
* [LoadKeyDescriptor](classeinput_loadkeydescriptor.md)
* [MappedTo](classeinput_mappedto.md)
* [OnClickDouble](classeinput_onclickdouble.md)
* [OnClickDown](classeinput_onclickdown.md)
* [OnClickMove](classeinput_onclickmove.md)
* [OnClickUp](classeinput_onclickup.md)
* [OnKeyDown](classeinput_onkeydown.md)
* [OnKeyUp](classeinput_onkeyup.md)
* [OnTilt](classeinput_ontilt.md)
* [OnWheelScroll](classeinput_onwheelscroll.md)
* [PutChar](classeinput_putchar.md)
* [RemoveActionMapping](classeinput_removeactionmapping.md)
* [RemoveKeyMapping](classeinput_removekeymapping.md)
* [ResetEvents](classeinput_resetevents.md)
* [ResetState](classeinput_resetstate.md)
* [SetDefaultMappings](classeinput_setdefaultmappings.md)
* [SetRepeatRates](classeinput_setrepeatrates.md)
* [Structures](classeinput_structures.md)
* [Definitions](classeinput_definitions.md)

## Default mappings
Click on one of the device platforms below to see their default key mappings:

* [GP2X](classeinput_map_gp2x.md)
* [Symbian Series 60 (S60)](classeinput_map_s60.md)
* [Symbian Series 80 (Nokia Communicator)](classeinput_map_s80.md)
* [Symbian Series 90 (Nokia 7710)](classeinput_map_s90.md)
* [Symbian UIQ](classeinput_map_uiq.md)
* [Windows desktop](classeinput_map_desktop.md)
* [Windows Mobile Pocket PC](classeinput_map_ppc.md)
* [Windows Mobile Smartphone](classeinput_map_smartphone.md)

## Version changes
- EDGELIB 2.70: Added this class to the SDK

