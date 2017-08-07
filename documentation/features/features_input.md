:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Input

## Supported input devices and buttons
EDGELIB provides information about the device input state through the framework. The following input events are monitored:

* stylus taps and dragging
* directional pad (D-Pad)
* hardware buttons
* telephone keys (including soft keys)
* UIQ jog/wheel device
* keyboard keys
* mouse moves, clicks and scrollwheel

## Key mappings
EDGELIB 2.70 introduced an advanced input class including default mappings recommended for use in games and game menus.

Menu mappings are used for game menu navigation and supports two styles of menus. One being a traditional menu where the user browses through multiple options and selects menu items where it's also possible to go back to a previous menu/cancel a menu item. The other is a softkey based menu where a softkey is linked to an action (depending on the game status) like the common phone interface. It also provides a default 'character type' key for soft keyboards which doesn't conflict with incoming character events.

Game mappings are available for in-game control in four directions, two actions and a button for pausing the game. Other mappings include arrow movements, volume control keys and additional softkeys which may or may not be mapped, depending on the device.

Input mappings are fully configurable and can be added and removed through the input module.

The names of key scan codes can also be retrieved through the input module, useful for games where custom key mappings can be defined through the menu.

## Accelerometer support
EDGELIB can handle and process input from devices supporting an accelerometer or TiltCONTROL device. The SDK supports several devices with an accelerometer or tilt sensor, including iOS, Symbian and Windows Mobile.

EDGELIB 2.40 introduced support for Pocketmotion's TiltCONTROL hardware device for Windows Mobile. The TiltCONTROL is a small device that attaches to a Windows Mobile Device such as a Pocket PC or Smartphone. The TiltCONTROL is able to detect the precise angle that the mobile device is being held at, and sends this information to the EDGELIB framework.

As of version 3.60, EDGELIB also supports the accelerometer found in various Nokia handsets. Supported Nokia phones so far are the 5500, N93, N95 8GB and N82 models.

In version 3.95, iOS support was added to EGELIB, including support for the accelerometer.

A stylus and d-pad emulation mode is available for automatic integration and support of the tilt devices. EDGELIB utilizes a new input event which is fired multiple times per second to report the framework of horizontal and vertical tilting. The emulation mode for stylus input is disabled by default, but can be turned on easily with one function call.


## Camera support
EDGELIB 4.00 introduced a brand new [class](../reference/classecamera.md) for accessing the hardware camera to capture video and image capture on several platforms, including Apple iOS 4, Symbian 3rd edition, Windows Mobile 5, Maemo 5, Windows desktop and Linux desktop.

EDGELIB gives you direct access to raw video frames for performance where available, yet allows you to convert raw data into a drawable surface with one call. Where hardware and firmware supports it, multiple cameras can be operated at the same time.
