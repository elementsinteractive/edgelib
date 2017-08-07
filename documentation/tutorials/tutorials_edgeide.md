:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Using the EDGELIB Builder tool

The EDGELIB Builder tool (formerly called Edge IDE) for Windows desktop has a user friendly interface and is able to automatically build project files using external commandline programs for multiple platforms.

The EDGELIB Builder tool is still in beta stage.

Currently EDGELIB Builder supports the following platforms:

* Google Android
* Antix Game Player
* Windows Mobile 2002, 2003 and 5.0
* Symbian Series 60 first and third editions
* Symbian Series 80 first and second edition
* Symbian Series 90
* Symbian UIQ 2.1 and UIQ 3.0
* Windows desktop using Visual C++ 6.0 and Visual Studio 2005.
* GP2X

The tool can be expanded with additional platforms by adding ini files to the platform folder.

## Setting up EDGELIB Builder
After the SDK has been installed, the EDGELIB Builder tool can be found inside the tools folder. It contains a main INI file EdgeBuilder.ini and several INI files inside the platforms folder. Before starting the program, open these INI files to correctly set all path locations. First let's begin with EdgeBuilder.ini which contains the path to the EDGELIB SDK:

_EdgeBuilder.ini (18 lines)_
```c++
# Edge IDE main configuration file
# --------------------------------
 
# General options
bldtmpfile       = "C:\edgeide_bldtmp_%appuid%"
 
# Paths
edgepath         = "c:\projects\edge"
incpath          = "%edgepath%\include"
 
# Macro list
macrodef         = "__EDGEIDE__"
macrodef         = "__EDGEBUILDER__"
 
# Configuration notices, remove this section when done editing
notice           = "EDGELIB path is set to default value: c:\projects\edge"
notice           = "Change default values by editing EdgeBuilder.ini"
notice           = "Remove this notice by removing the notice section at the bottom of EdgeBuilder.ini"
```

Set the `edgepath` setting to the folder where EDGELIB has been installed and remove the notices section. After that, open the platforms folder and open each INI file. Set the paths to where the SDKs have been installed and remove the notices section as well.

## Creating projects
Creating projects is easy, start the EDGELIB Builder tool and click on file -> new project. After creating a new project, right click on the source or header files folder to add existing source code files to the tree. Create source files with a text editor, or import the code base of a project written in Visual C++. When all files have been added, select the target platform and choose build from the menu.

## Change project properties
Right click on the project name, select properties or access the project properties screen from project -> properties in the menu. You can change the name of the project and the unique identification number used by Symbian. When it is not defined, it takes the default value (0x10205d9d).

## Adding additional include or library paths
To add extra include or library paths for a project, open the path configuration window through project -> Library/include paths. It's possible to add, edit and remove paths or change the priority of each path in the list.

## Adding icons
It's also possible to add a program icon for every platform, except GP2X. This icon can have various sizes, for example for Windows Mobile you can have both an 16x16 pixel icon and an 32x32 pixel icon. Icons can be added by creating a new folder named "Icons" in the resource root. Add BMP, GIF or PNG files to generate the icon image. The transparent color for bitmap files is RGB 255, 0, 255.

## Installation files
EDGELIB Builder supports building installation (.sis) files for all supported Symbian versions. Add installation files by creating a resource folder named "install". Create a folder structure if the installation files are separated into several paths. The application executable doesn't need to be added, it will be added to the installation automatically. Build the installation file by selecting it in the build menu.

## Editing project files manually
It's also possible to create and edit project files with a text editor. These project files have an INI-file structure and can be easily edited. Here is a sample project file for an hello world project:

_helloworld.epj (33 lines)_
```c++
# EDGELIB project file (edit at your own risk)
# Copyright (c) 2005-2017 Elements Interactive B.V.
# http://www.edgelib.com
# -----------------------------------------
 
# General
projectname = "Hello World"
appuid = "0x10205d9d"
 
# Project tree
sourcepath = "code"
    sourcefile = "..\code\framework.cpp"
    sourcefile = "..\code\util.cpp"
endpath
sourcefile = "..\code\hello.cpp"
 
headerpath = "include"
    headerfile = "..\include\framework.h"
    headerfile = "..\include\util.h"
endpath
 
resourcepath = "icons"
    resourcefile = "..\res\icon16.bmp"
    resourcefile = "..\res\icon32.bmp"
endpath
resourcepath = "install"
    resourcepath = "res"
        resourcefile = "..\res\game.epk"
    endpath
endpath
 
# Project environment
incpath = "..\include"
```

The `projectname` specifies the name of the project and executable. Using tabs in the project tree isn't necessary, but is recommended for clarity. The project environment contains environment values which will be interpreted by the compiler. For example, `incpath` defines an extra include path to search for header files when compiling. These environment values can be defined in EdgeBuilder.ini, a project file and each platform file. When compiling, the environment of all these files are combined and passed to the compiler.

