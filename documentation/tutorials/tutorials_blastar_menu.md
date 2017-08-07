:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# The menu module

## Creating a dynamic menu system
The menu module has two functions: it will be a title screen and it will contain a pause menu. Because these two menu's are different, it's a good idea to make this menu dynamic. This will also be useful in case a menu item will be added or removed. First let's create a couple of definitions, one for each menu item type and one defining the maximum amount of menu items that will be used.

_menu.h (18 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Menu definitions                                                //
/////////////////////////////////////////////////////////////////////
 
#define MI_NONE         0
#define MI_STARTGAME    1
#define MI_QUITGAME     2
#define MI_AUTOFIRE     3
#define MI_STARTLEVEL   4
#define MI_MINIMIZE     5
#define MI_RESUMELEVEL  6
#define MI_QUITLEVEL    7
 
#define MAX_MENUCOUNT   4
 
 
/////////////////////////////////////////////////////////////////////
// Class definition                                                // 
```

We're also need a number of private variables for containing the menu, and a variable indicating the selected menu item. Add these to `ClassMenu`:

_menu.h (8 lines)_
```c++
class ClassMenu : public ClassModule
{
    private:
        char menutext[MAX_MENUCOUNT][32];
        unsigned char menuitem[MAX_MENUCOUNT];
        unsigned char menucount;
        unsigned char curitem;
    public:
```

And initialize these variables in the constructor, for safety:

_menu.h (9 lines)_
```c++
//ClassMenu: Constructor
ClassMenu::ClassMenu(void)
{
    unsigned char ctr;
    for (ctr = 0; ctr < MAX_MENUCOUNT; ctr++)
        ClassEStd::StrCpy(menutext[ctr], "");
    menucount = MAX_MENUCOUNT;
    curitem = 0;
}
```

## Menu construction
A good place to construct a menu, is after receiving a module message from another module. This way, a different menu can be made depending on the state of the game. `MM_INIT` will be received when the default menu should be created, after first loading the game or when a game is over. The message `MM_FROMMODULE + MODULE_LEVEL` will be sent if the game is paused. Here is a code sample:

_menu.h (39 lines)_
```c++
//Module game loop
void ClassMenu::OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta)
{
    if (*modulemsg == MM_INIT)
    {
        menucount = 0;
        menuitem[menucount] = MI_STARTGAME;
        ClassEStd::StrCpy(menutext[menucount++], "Start Game");
        menuitem[menucount] = MI_AUTOFIRE;
        ClassEStd::StrCpy(menutext[menucount++], "Auto-Fire: ");
        menuitem[menucount] = MI_STARTLEVEL;
        ClassEStd::StrCpy(menutext[menucount++], "Start Level: ");
        menuitem[menucount] = MI_QUITGAME;
        ClassEStd::StrCpy(menutext[menucount++], "Quit Game");
        curitem = 0;
        *modulemsg = MM_NONE;
    }
    else if (*modulemsg == MM_FROMMODULE + MODULE_LEVEL)
    {
        menucount = 0;
        menuitem[menucount] = MI_RESUMELEVEL;
        ClassEStd::StrCpy(menutext[menucount++], "Resume Level");
        menuitem[menucount] = MI_AUTOFIRE;
        ClassEStd::StrCpy(menutext[menucount++], "Auto-Fire: ");
        menuitem[menucount] = MI_MINIMIZE;
        #if !defined(DEVICE_GP2X)
            ClassEStd::StrCpy(menutext[menucount++], "Minimize");
            menuitem[menucount] = MI_QUITLEVEL;
        #endif
        ClassEStd::StrCpy(menutext[menucount++], "Quit Level");
        curitem = 0;
        *modulemsg = MM_NONE;
    }
    else if (*modulemsg == MM_NONE)
    {
        //menu input handling
        //menu rendering
    }
}
```

First we're going to render the menu. The active menu item should flash, while the other menu items should be grayed out. These effects can be realized with the EDGELIB colorize filter. First add a timer variable for the flashing effect to the module and a function definition:

_menu.h (7 lines)_
```c++
    private:
        char menutext[MAX_MENUCOUNT][32];
        unsigned char menuitem[MAX_MENUCOUNT];
        unsigned char menucount;
        unsigned char curitem;
        unsigned long flashtimer;
        void RenderMenuItems(E2DSurface *buffer);
```

The function will render all the menu items in the middle of the screen. Here is the code sample:

_menu.h (42 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// ClassMenu::Private                                              //
/////////////////////////////////////////////////////////////////////
 
//Render the menu items on the display buffer
void ClassMenu::RenderMenuItems(E2DSurface *buffer)
{
    char drawtext[32];
    COLORREF textcolor;
    long ypos, yspace;
    unsigned char ctr;
    yspace = shared->font.GetHeight() * 3 / 2;
    ypos = YRES / 2 - menucount * yspace / 2;
    for (ctr = 0; ctr < menucount; ctr++)
    {
        ClassEStd::StrCpy(drawtext, menutext[ctr]);
        if (menuitem[ctr] == MI_AUTOFIRE)
        {
            if (shared->cfg.autoshot)
                ClassEStd::StrCat(drawtext, "On");
            else
                ClassEStd::StrCat(drawtext, "Off");
        }
        else if (menuitem[ctr] == MI_STARTLEVEL)
            ClassEStd::StrCat(drawtext, shared->cfg.startlevel);
        if (ctr == curitem)
        {
            if (flashtimer % 1024 < 512)
                textcolor = E_RGBX(128 + flashtimer % 512 / 4, 128 + flashtimer % 512 / 4, 128 + flashtimer % 512 / 4);
            else
                textcolor = E_RGBX(255 - flashtimer % 512 / 4, 255 - flashtimer % 512 / 4, 255 - flashtimer % 512 / 4);
        }
        else
            textcolor = E_RGBX(160, 160, 160);
        buffer->DrawFont(buffer->GetWidth() / 2, ypos, &shared->font, drawtext, EFX_COLORKEY | EFX_COLORIZE | EFO_HCENTER, textcolor);
        ypos += yspace;
    }
}
 
 
/////////////////////////////////////////////////////////////////////
// ClassMenu::Public                                               // 
```

The function also handles specific menu items which are rendered depending on the game configuration (the start level and whether auto-fire has been enabled). Now let's render the titlescreen background, the menu items and activate flashing:

_menu.h (7 lines)_
```c++
    else if (*modulemsg == MM_NONE)
    {
        //menu input handling
        flashtimer += timedelta;
        buffer->BltFast(0, 0, &shared->titlebg, NULL);
        RenderMenuItems(buffer);
    }
```

## Menu input
The menu needs to react to input events. When the player presses the menu confirm or one of the softbuttons, the menu option should be activated. Pressing up, down or rotating the scrollwheel should change the selected menu item and tapping on a menu option should activate it. Also, some menu items (for example, the starting level) can be changed by pressing left or right.

All these events will be handled by a new function. This function returns a menu item, when it has been activated. Add it to the class definition as well:

_menu.h (3 lines)_
```c++
        unsigned char HandleMenuInput(void);
        void RenderMenuItems(E2DSurface *buffer);
    public:
```

_menu.h (77 lines)_
```c++
//Handle the menu input system
unsigned char ClassMenu::HandleMenuInput(long *inputstate)
{
    E_INPUTEVENT inputevent;
    long wheeldir = shared->ecd->input->GetWheelScroll();
    while (wheeldir != 0)
    {
        if (wheeldir < 0)
        {
            wheeldir++;
            curitem = (curitem + 1) % menucount;
        }
        else
        {
            wheeldir--;
            curitem = (curitem + menucount - 1) % menucount;
        }
        flashtimer = 512;
    }
    while(shared->ecd->input->GetKeyEvent(&inputevent))
    {
        if (inputevent.action == DIAM_MENUUP && (inputevent.type == EIE_PRESSED || inputevent.type == EIE_REPEAT))
        {
            curitem = (curitem + menucount - 1) % menucount;
            flashtimer = 512;
        }
        if (inputevent.action == DIAM_MENUDOWN && (inputevent.type == EIE_PRESSED || inputevent.type == EIE_REPEAT))
        {
            curitem = (curitem + 1) % menucount;
            flashtimer = 512;
        }
        if (inputevent.action == DIAM_MENULEFT && (inputevent.type == EIE_PRESSED || inputevent.type == EIE_REPEAT))
        {
            if (menuitem[curitem] == MI_STARTLEVEL)
            {
                if (shared->cfg.startlevel == 1)
                    shared->cfg.startlevel = 9;
                else
                    shared->cfg.startlevel--;
            }
            else if (menuitem[curitem] == MI_AUTOFIRE)
                shared->cfg.autoshot = !shared->cfg.autoshot;
            flashtimer = 512;
        }
        if (inputevent.action == DIAM_MENURIGHT && (inputevent.type == EIE_PRESSED || inputevent.type == EIE_REPEAT))
        {
            if (menuitem[curitem] == MI_STARTLEVEL)
            {
                if (shared->cfg.startlevel < 9)
                    shared->cfg.startlevel++;
                else
                    shared->cfg.startlevel = 1;
            }
            else if (menuitem[curitem] == MI_AUTOFIRE)
                shared->cfg.autoshot = !shared->cfg.autoshot;
            flashtimer = 512;
        }
        if (inputevent.type == EIE_PRESSED && (inputevent.action == DIAM_MENUCONFIRM || inputevent.action == DIAM_MENUSOFT1 || inputevent.action == DIAM_MENUSOFT2))
            return(menuitem[curitem]);
    }
    while(shared->ecd->input->GetClickEvent(&inputevent))
    {
        if (inputevent.type == EIE_PRESSED)
        {
            long ypos, yspace;
            yspace = shared->font.GetHeight() * 3 / 2;
            ypos = YRES / 2 - menucount * yspace / 2;
            if (inputevent.clicky >= ypos && inputevent.clicky < ypos + yspace * menucount)
            {
                curitem = (unsigned char)((inputevent.clicky - ypos) / yspace);
                flashtimer = 512;
                return(menuitem[curitem]);
            }
        }
    }
    return(MI_NONE);
}
```

The last step is to call this function every frame and react depending on the menu item. A switch-case setup will be useful. Some menu items will be covered later in this tutorial, when the pause menu can be activated.

_menu.h (32 lines)_
```c++
    else if (*modulemsg == MM_NONE)
    {
        unsigned char result = HandleMenuInput();
        switch(result)
        {
            case MI_STARTGAME:
                *module = MODULE_LEVEL;
                *modulemsg = MM_INIT;
                break;
            case MI_QUITGAME:
                *module = MODULE_CLEANUP;
                break;
            case MI_AUTOFIRE:
                shared->cfg.autoshot = !shared->cfg.autoshot;
                break;
            case MI_STARTLEVEL:
                if (shared->cfg.startlevel < 9)
                    shared->cfg.startlevel++;
                else
                    shared->cfg.startlevel = 1;
                break;
            case MI_MINIMIZE:
                break;
            case MI_RESUMELEVEL:
                break;
            case MI_QUITLEVEL:
                break;
        }
        flashtimer += timedelta;
        buffer->BltFast(0, 0, &shared->titlebg, NULL);
        RenderMenuItems(buffer);
    }
```

## Chapters
* [Designing a game](tutorials_blastar_design.md)
* [The project framework](tutorials_blastar_framework.md)
* [Creating graphics](tutorials_blastar_graphics.md)
* [The loader module](tutorials_blastar_loader.md)
* [Platform definitions](tutorials_blastar_definitions.md)
* **The menu module**
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

