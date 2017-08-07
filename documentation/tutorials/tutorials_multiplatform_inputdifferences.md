:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# The differences between the input of each device

## Different types of input
A device usually has two or more different types of input. These are the input types EDGELIB supports:

* A directional pad for moving into 8 directions
* Additional hardware buttons
* A set of keys for the number pad on a phone
* A touchscreen including a stylus, or a mouse
* A jog or scrollwheel
* A TiltCONTROL device from PocketMotion
* An additional qwerty-style keyboard

## Input systems for different types of games
There are different types of games, each requiring their own type of input system.

Platform games depend heavily on the directional pad and one or more additional action buttons. When controlling the character only involves moving and jumping, the stylus might be used as well. However, using a stylus for controlling platform games usually doesn't work too well when a lot of action and precision is involved.

Strategy games are more suitable for the mobile platform. Realtime strategy games can be controlled excellently using the stylus. They can also be controlled using a directional pad and a few action buttons by implementing a software cursor (for unit selection) or by using another method. Turn-based strategy games can be controlled very well by stylus and non-stylus devices, and don't need as much speed as action games.

Racing games can also be controlled very well by many devices. Using the directional pad is preferred, but the touch screen can also be used by pressing the left or right side of the screen for steering (usually done by the thumb). Automatic acceleration can also be a good idea if pressing multiple buttons is an issue.

Puzzle games are the most suitable for mobile devices. They don't require a complex input system and usually can be controlled by using both stylus and buttons. Puzzle games also don't require as much performance as the other types of games.

Shoot-em-ups can also be made well for mobile devices. Flying into different directions can be done by the directional pad, or by pressing the desired location with the stylus. Implementing automatic shooting can also be a good idea for playing the game with one hand (for phone devices).

For other types of games it depends on the game itself which type of input is suitable. This document described common solutions for handling input on different types of games, but there might be other solutions that may work well on a specific game.

## Input conventions
Important also are input conventions. This means that users will expect a certain input system when playing a game on their device. Users with a device that has a touch screen, expect to control most of the game by stylus unless a lot of action is involved (at least the game menus should be able to controlled with the stylus). Users with a phone device will expect to control the game with one hand, because this is very common. The directional pad, and the two softbuttons (located left and right near the bottom of the screen) should be used to navigate the menu or bring up the in-game pause menu. When a device has a scrollwheel, the user will expect to be able to scroll through menu options with the wheel.

Another important input feature that shouldn't be overlooked, is to provide a key configuration tool for changing the key assignments for the in-game actions. For example, the directional pad of some devices are of less quality than others and players might have issues when trying to move the main character.

Try to supply as many ways to control the menus and the in-game action, to give the player choice of the desired way to interact.

## Chapters
* [How to handle the limits of each platform](tutorials_multiplatform_limits.md)
* **The differences between the input of each device**
* [Creating graphics for multi-platform games](tutorials_multiplatform_graphics.md)
* [Handling of device-independent animations](tutorials_multiplatform_animations.md)

