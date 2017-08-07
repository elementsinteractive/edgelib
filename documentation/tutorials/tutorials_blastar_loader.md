:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# The loader module

## Necessary preparations
The loader module will be used to load the graphics for the game, showing a nice progress bar. Graphics will be loaded when the game starts and will be kept in memory until the game quits. The game only needs to load once, players won't be interrupted with loading screens after this phase. This is also an advantage for mobile devices, because the game could often be minimized. After resuming, the graphics will still be present in the memory of the device.

The graphics for the game can be downloaded from the previous chapter. They are packed in an EDGELIB Packer tool package, which can only be opened by the developer. The downloaded graphics are already packed, it's only necessary to copy the high-resolution graphics package into the pc workspace release folder: `C:\projects\blastar\workspace_pc\Release`. The loader will read all graphics files from this package.

## The loader progress bar
Because a progress bar needs to be updated every frame, the graphics can't be loaded all at once. The module will contain a number of module states, one state for each step in the loading process. The modulestate variable is already defined in the module base class, there is no need to add it to the class definition. There are a number of module state definitions: one for each graphic to be loaded and a few miscellaneous states:

_loader.h (16 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Module states                                                   //
/////////////////////////////////////////////////////////////////////
 
#define MSLD_INIT       0
#define MSLD_OPENPACK   1
#define MSLD_FONT       2
#define MSLD_TITLE      3
#define MSLD_GAMEBG     4
#define MSLD_SHEET      5
#define MSLD_CLOSEPACK  6
#define MSLD_DONE       7
 
 
/////////////////////////////////////////////////////////////////////
// Class definition                                                // 
```

The modulestate will begin with `MSLD_INIT` and will be increased by 1, every frame. The loader will continue until it reaches `MSLD_DONE`, then the active module can be changed to the main menu. This way, the progress bar is updated after processing each state.

In this loader class, certain graphics are loaded earlier than others. The first thing that has to be done is to display a load picture and an empty progress bar. This picture can be a company logo or, in our case, a simple gradient. A nice effect is to let this picture fade in from black. Another step that's important is to read the in-game font, which will be used to display "loading" in the middle of the screen.

## Loading graphics with the loader module
Let's load the game graphics using the loader module. Because graphics are used in each module, they need to be added to the shared data structure. Add a number of surfaces to this structure:

_main.h (12 lines)_
```c++
//Program shared data
typedef struct
{
    unsigned long module;
    unsigned long modulemsg;
    E2DSurface font;
    E2DSurface titlebg;
    E2DSurface gamebg;
    E2DSurface sheet;
    GAMECFG cfg;
    ECD *ecd;
}SHARED_DATA;
```

First we're going to program the basic functionality of the progress bar, no graphics are actually loaded yet. The module will react on its state using a switch-case structure and displays the background and progress bar afterwards. It's recommended to separate graphics operations from other operations: first handle game logic, then render the screen. Add the following code to OnNextFrame() of the loader module:

_loader.h (43 lines)_
```c++
//Module game loop
void ClassLoader::OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta)
{
    if (*modulemsg == MM_INIT)
    {
        modulestate = MSLD_INIT;
        *modulemsg = MM_NONE;
    }
    else if (*modulemsg == MM_NONE)
    {
        RECT rc;
        POINT midpnt;
        switch(modulestate)
        {
            case MSLD_OPENPACK:
                break;
            case MSLD_FONT:
                break;
            case MSLD_TITLE:
                break;
            case MSLD_GAMEBG:
                break;
            case MSLD_SHEET:
                break;
            case MSLD_CLOSEPACK:
                break;
        }
        modulestate++;
        if (modulestate == MSLD_DONE)
        {
            *module = MODULE_MENU;
            *modulemsg = MM_INIT;
        }
        midpnt.x = buffer->GetWidth() / 2;
        midpnt.y = buffer->GetHeight() / 2;
        SetRect(&rc, midpnt.x, buffer->GetHeight() * 13 / 14, buffer->GetWidth() * 39 / 40, buffer->GetHeight() * 39 / 40);
        buffer->GradientRect(NULL, E_RGBX(0, 0, 128), E_RGBX(0, 128, 192), E_RGBX(0, 0, 255), E_RGBX(64, 160, 255), EFX_DITHER);
        buffer->DrawRect(&rc, E_RGBX(255, 255, 255));
        SetRect(&rc, rc.left, rc.top, rc.left + (rc.right - rc.left) * modulestate / MSLD_DONE, rc.bottom);
        buffer->FillRect(&rc, E_RGBX(255, 255, 255));
        buffer->DrawFont(midpnt.x, midpnt.y, &shared->font, "Loading...", EFX_COLORKEY | EFO_HCENTER | EFO_VCENTER);
    }
}
```

When running this sample, you can see a progress bar that is being filled very quickly. The last line of code writes the text "loading" onscreen. This can't be seen yet, because no font has been loaded. When the loader bar is full, it should stop (because nothing has been programmed yet, after this point).

It's time to finally load in the graphics. Add an EDGELIB file object as a private variable of the loader class:

_loader.h (5 lines)_
```c++
class ClassLoader : public ClassModule
{
    private:
        ClassEFile ef;
    public:
```

The package file will be opened only once at the beginning of the load routine, and closed when done loading. Program each program state case for the loader gameloop:

_loader.h (63 lines)_
```c++
        POINT midpnt;
        unsigned char *filedata;
        unsigned long filedatasize;
        switch(modulestate)
        {
            case MSLD_OPENPACK:
                if (ef.Open("gfx.epk", EFOF_READ))
                {
                    if (!ef.PackAuthorize(0))
                        *module = MODULE_CLEANUP;
                }
                else
                    *module = MODULE_CLEANUP;
                break;
            case MSLD_FONT:
                if (ef.PackGetFileData(filedata, filedatasize, "font.bmp") == E_OK)
                {
                    if (shared->ecd->dsp->CreateSurface(&shared->font, filedata, filedatasize) != E_OK)
                        *module = MODULE_CLEANUP;
                    shared->font.SetFontCharacters(0, 127);
                    shared->font.BuildFont(false, ECOLOR_FUCHSIA);
                    ClassEMemory::DeAlloc(filedata);
                }
                else
                    *module = MODULE_CLEANUP;
                break;
            case MSLD_TITLE:
                if (ef.PackGetFileData(filedata, filedatasize, "title.bmp") == E_OK)
                {
                    if (shared->ecd->dsp->CreateSurface(&shared->titlebg, filedata, filedatasize, EST_SYSMEMORY | EST_DITHER) != E_OK)
                        *module = MODULE_CLEANUP;
                    ClassEMemory::DeAlloc(filedata);
                }
                else
                    *module = MODULE_CLEANUP;
                break;
            case MSLD_GAMEBG:
                if (ef.PackGetFileData(filedata, filedatasize, "background.bmp") == E_OK)
                {
                    if (shared->ecd->dsp->CreateSurface(&shared->gamebg, filedata, filedatasize, EST_SYSMEMORY | EST_DITHER) != E_OK)
                        *module = MODULE_CLEANUP;
                    ClassEMemory::DeAlloc(filedata);
                }
                else
                    *module = MODULE_CLEANUP;
                break;
            case MSLD_SHEET:
                if (ef.PackGetFileData(filedata, filedatasize, "sheet.bmp") == E_OK)
                {
                    if (shared->ecd->dsp->CreateSurface(&shared->sheet, filedata, filedatasize) != E_OK)
                        *module = MODULE_CLEANUP;
                    else
                        shared->sheet.SetColorKey(ECOLOR_FUCHSIA);
                    ClassEMemory::DeAlloc(filedata);
                }
                else
                    *module = MODULE_CLEANUP;
                break;
            case MSLD_CLOSEPACK:
                ef.Close();
                break;
        }
        modulestate++;
```

When a file can't be loaded or an error occurs, the module is changed to `MODULE_CLEANUP`. This will exit the game.

## Updating installation files
Because the game needs a graphics pack file to load, we need to add it to the installation definition files. We need to update the install.inf file for Windows Mobile, and the EDGELIB Builder blastar.epj project file for the other platforms.

_install.inf (56 lines)_
```c++
[Version]
Signature   = "$Windows NT$"
Provider    = "EDGELIB"
CESignature = "$Windows CE$"
 
 
[CEDevice]
BuildMax    = 0xE0000000
 
 
[CEStrings]
AppName     = "Blastar"
InstallDir  = %CE1%\%AppName%
 
 
[DefaultInstall]
CopyFiles   = ExeFiles,ResFiles
CEShortcuts = Startmenu
 
 
[SourceDisksNames.ARMPPC]
1 = ,"ExeFiles",,"..\workspace_eide_320x240\release\WM Pocket PC (2002)"
2 = ,"ResFiles",,"..\res\graphics\320x240"
[SourceDisksNames.ARMPPCVGA]
1 = ,"ExeFiles",,"..\workspace_eide_640x480\release\WM Pocket PC (2003)"
2 = ,"ResFiles",,"..\res\graphics\640x480"
[SourceDisksNames.ARMWSP]
1 = ,"ExeFiles",,"..\workspace_eide_176x220\release\WM Smartphone (2002)"
2 = ,"ResFiles",,"..\res\graphics\176x220"
[SourceDisksNames.ARMWSPQVGA]
1 = ,"ExeFiles",,"..\workspace_eide_320x240\release\WM Smartphone (2003)"
2 = ,"ResFiles",,"..\res\graphics\320x240"
 
 
[SourceDisksFiles]
Blastar.exe = 1
gfx.epk = 2
 
 
[DestinationDirs]
ExeFiles  = 0, %InstallDir%
ResFiles  = 0, %InstallDir%
Startmenu = 0, %CE11%\Edge
DefaultDestDir = 0, %InstallDir%
 
 
[ExeFiles]
"Blastar.exe"
 
 
[ResFiles]
"gfx.epk"
 
 
[Startmenu]
"Blastar", 0, "Blastar.exe"
```

To update the EDGELIB Builder project file, add a new folder 'Install' in the resource files folder. Add gfx.epk to this folder. Because there are multiple gfx.epk files, we recommend copying the EDGELIB Builder project file for each resolution. Create a separate folders for each EDGELIB Builder workspace:

* workspace_eide_176x208
* workspace_eide_176x220
* workspace_eide_320x208
* workspace_eide_320x240
* workspace_eide_352x416
* workspace_eide_640x200
* workspace_eide_640x320
* workspace_eide_640x480

## Chapters
* [Designing a game](tutorials_blastar_design.md)
* [The project framework](tutorials_blastar_framework.md)
* [Creating graphics](tutorials_blastar_graphics.md)
* **The loader module**
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

