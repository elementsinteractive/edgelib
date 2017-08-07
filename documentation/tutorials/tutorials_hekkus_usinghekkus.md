:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# How to use Hekkus within EDGELIB

## Using Hekkus
Hekkus is a sound engine that can be used in combination with EDGELIB. Hekkus can play sound effects and music modules in the .mod format. Hekkus is developed and maintained by Thomas "Gerson" Maniero and can be downloaded from [his website](http://www.shlzero.com/wordpress/?page_id=4).

Hekkus supports a variety of devices:

* Windows Mobile
* Symbian (up to Symbian 9+)
* GP2X
* PSP
* Windows desktop

## Installing Hekkus
As with other tutorials, this tutorial assumes EDGELIB has been installed in `C:\projects\Edge`. Hekkus will be installed at `C:\projects\Hekkus`. When developing for Symbian, an additional task needs to be done. Hekkus contains two folders: `C:\projects\Hekkus\lib\symbian-armi` and `C:\projects\Hekkus\dll\symbian-armi`. These files should be copied inside the following folder of each Symbian SDK: `\epoc32\release\armi\urel`

## Chapters
* **How to use Hekkus within EDGELIB**
* [Create a multi-platform project with Hekkus](tutorials_hekkus_project.md)
* [Playback sound and music](tutorials_hekkus_playback.md)

