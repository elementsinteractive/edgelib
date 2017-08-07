:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Creating graphics

## List of graphics needed for this game
At this point we need graphics to continue development. Usually, these graphics are created by 2D graphics artists or 3D modelers. The graphics will be delivered while the game is being programmed. Simple placeholder graphics might be necessary when a certain graphic isn't finished yet. It will be useful to create a list of graphics that have to be created for a game, listed by priority. The graphics needed for this tutorial are:

* A font for writing text in-game
* A title screen picture
* A background picture for in-game
* The player spaceship sprite
* Sprites for the wall of objects
* Powerup sprites
* A boss sprite
* A missile sprite, fired by the boss
* Additional particle sprites for neat effects

There will be a total of four graphics sets, each set is being made for a specific screen resolution. A set contains graphics of a certain size: two sets for high resolution devices, and 2 sets for low resolution devices. A set is a large picture with the sprites used in-game. The other graphics will be made separately for each device.

## Editing graphics
There are several methods for creating and editing graphics. Of course, certain methods can be combined for improving graphics quality.

One method is to create a picture pixel by pixel in a simple graphics editor. This method requires skill, but the graphics will look very good. Each sprite created by this method will have a well-defined look to them. However, this method becomes more tedious when the size of a sprite increases. This method was used to create the in-game font and several small sprites.

Another method is to use an advanced graphic editing program, like Photoshop. Graphics in these programs can be easily edited, especially when using layers. It can be very suitable for creating large pictures and title screens. However, creating small detailed sprites can be tedious with this method. For this tutorial the title screen and game background where created by this method, as well as the in-game particle sprites.

The last method covered by this tutorial is to use a 3D modelling program for creating graphics. A 3D game requires this method, but it can also be useful for 2D graphics. This method is very suitable for creating title screens and backgrounds. It can also be used for creating sprites like a spaceship. Rendering a sprite in a specific size can be a problem, and the picture might look blurred when a sprite becomes too small. Another problem that arises is that a colored edge appears around a rendered sprite (depending on the background used for rendering). However, this can be solved by using RGBA surfaces. This method was used to create the some of the in-game sprites objects.

## Downloading graphics
The graphics for this tutorial can be downloaded in this section. Unpack them into `C:\projects\blastar\res\graphics`. After they have been unpacked, there are several folders within the graphics folder: one folder for each possible screen resolution.

Download the graphics here: blastar graphics.

## Chapters
* [Designing a game](tutorials_blastar_design.md)
* [The project framework](tutorials_blastar_framework.md)
* **Creating graphics**
* [The loader module](tutorials_blastar_loader.md)
* [Platform definitions](tutorials_blastar_definitions.md)
* [The menu module](tutorials_blastar_menu.md)
* [The level module](tutorials_blastar_level.md)
* [Using sprites](tutorials_blastar_sprites.md)
* [Player control](tutorials_blastar_control.md)
* [Wall of objects](tutorials_blastar_objects.md)
* [Collision detection](tutorials_blastar_collision.md)
* [The boss](tutorials_blastar_boss.md)
* [Refining the game](tutorials_blastar_refining.md)
* [Adding powerups](tutorials_blastar_powerups.md)
* [Special effects](tutorials_blastar_specialfx.md)
* [Completing the game](tutorials_blastar_completing.md)

