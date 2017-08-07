:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# The level module

## Preparations
The level module will contain the actual gameplay, we're now going to make the game playable. There are going to be three states in this module: showing a "ready" message, the gameplay and the "game over" state. Add these definitions:

_level.h (11 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Module states                                                   //
/////////////////////////////////////////////////////////////////////
 
#define MSLV_READY      0
#define MSLV_IDLE       1
#define MSLV_GAMEOVER   2
 
 
/////////////////////////////////////////////////////////////////////
// Class definition                                                // 
```

The ready message will flash, a timer value will be added as a private member variable:

_level.h (5 lines)_
```c++
class ClassLevel : public ClassModule
{
    private:
        unsigned long msgtimer;
    public:
```

## Rendering the screen layout
Let's render a basic screen layout. A background picture, two menu items and the ready or game over message. Add a `RenderScreen()` function to the module:

_level.h (3 lines)_
```c++
    private:
        unsigned long msgtimer;
        void RenderScreen(E2DSurface *buffer);
```

_level.h (33 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// ClassLevel::Private                                             //
/////////////////////////////////////////////////////////////////////
 
//Render the graphics on the screen
void ClassLevel::RenderScreen(E2DSurface *buffer)
{
    RECT rc;
    buffer->BltFast(0, 0, &shared->gamebg, NULL);
    SetRect(&rc, DIM_SPRITE * 4, YRES - DIM_SPRITE * 3 - shared->font.GetHeight(), XRES / 2 - DIM_SPRITE * 4, YRES - DIM_SPRITE);
    buffer->FillRect(&rc, E_RGBX(16, 48, 32), EFX_OPACITY, 128);
    buffer->DrawRect(&rc, E_RGBX(16, 48, 32));
    if (modulestate == MSLV_IDLE)
        buffer->DrawFont(rc.left + (rc.right - rc.left) / 2, rc.top + (rc.bottom - rc.top) / 2, &shared->font, "Minimize", EFX_COLORKEY | EFO_HCENTER | EFO_VCENTER);
    else
        buffer->DrawFont(rc.left + (rc.right - rc.left) / 2, rc.top + (rc.bottom - rc.top) / 2, &shared->font, "Minimize", EFX_COLORKEY | EFX_OPACITY | EFO_HCENTER | EFO_VCENTER, 64);
    SetRect(&rc, XRES / 2 + DIM_SPRITE * 4, YRES - DIM_SPRITE * 3 - shared->font.GetHeight(), XRES - DIM_SPRITE * 4, YRES - DIM_SPRITE);
    buffer->FillRect(&rc, E_RGBX(16, 48, 32), EFX_OPACITY, 128);
    buffer->DrawRect(&rc, E_RGBX(16, 48, 32));
    if (modulestate == MSLV_IDLE)
        buffer->DrawFont(rc.left + (rc.right - rc.left) / 2, rc.top + (rc.bottom - rc.top) / 2, &shared->font, "Pause", EFX_COLORKEY | EFO_HCENTER | EFO_VCENTER);
    else
        buffer->DrawFont(rc.left + (rc.right - rc.left) / 2, rc.top + (rc.bottom - rc.top) / 2, &shared->font, "Pause", EFX_COLORKEY | EFX_OPACITY | EFO_HCENTER | EFO_VCENTER, 64);
    if (modulestate == MSLV_READY)
        if (msgtimer % 384 < 192)
            buffer->DrawFont(XRES / 2, YRES / 2, &shared->font, "Ready", EFX_COLORKEY | EFO_HCENTER | EFO_VCENTER);
    if (modulestate == MSLV_GAMEOVER)
        buffer->DrawFont(XRES / 2, YRES / 2, &shared->font, "Game Over", EFX_COLORKEY | EFO_HCENTER | EFO_VCENTER);
}
 
 
/////////////////////////////////////////////////////////////////////
// ClassLevel::Public                                              // 
```

This will always render the background. The menu items will be rendered normally in idle state, but faded (inactive) for game over and ready. A message will be seen in the middle of the screen for the ready and game over states.

The next thing to do is to handle the different module states. The ready state will be active for one second, then it will head to the idle state. The game over state will be active for three seconds, after that the module will quit and head back to the menu module. Program the basics of the level gameloop:

_level.h (35 lines)_
```c++
//Module game loop
void ClassLevel::OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta)
{
    if (*modulemsg == MM_INIT)
    {
        modulestate = MSLV_READY;
        msgtimer = 0;
        *modulemsg = MM_NONE;
    }
    else if (*modulemsg == MM_FROMMODULE + MODULE_MENU)
    {
        if (modulestate != MSLV_GAMEOVER)
            modulestate = MSLV_READY;
        msgtimer = 0;
        *modulemsg = MM_NONE;
    }
    else if (*modulemsg == MM_NONE)
    {
        if (modulestate == MSLV_READY || modulestate == MSLV_GAMEOVER)
        {
            msgtimer += timedelta;
            if ((modulestate == MSLV_READY && msgtimer >= 1000) || (modulestate == MSLV_GAMEOVER && msgtimer >= 3000))
            {
                if (modulestate == MSLV_GAMEOVER)
                {
                    *module = MODULE_MENU;
                    *modulemsg = MM_INIT;
                }
                else if (modulestate == MSLV_READY)
                    modulestate = MSLV_IDLE;
            }
        }
        RenderScreen(buffer);
    }
}
```

## Minimizing
Because the game is made for mobile devices, expect that the game may be minimized at any moment (for example, when a phonecall comes in). This shouldn't be a problem for the menu or loader module, but may be a problem for the level module. This is because the game can get minimized in the middle of the action, and after it will be resumed the player needs to act immediately. A way to solve this is to switch to the menu module and pause the game when this happens.

The player should also be able to minimize the game by a menu option. Because the minimize function is only present in the main class, we need a way to inform the main class to minimize the game. We're going to add a variable to the shared data structure for minimizing the game.

_main.h (7 lines)_
```c++
//Program shared data
typedef struct
{
    unsigned long module;
    unsigned long modulemsg;
    bool forceminimize;
    E2DSurface font;
```

This variable will be disabled by default in the `ClassMain` constructor:

_main.h (10 lines)_
```c++
//ClassMain: constructor
ClassMain::ClassMain(void)
{
    unsigned char ctr;
    for (ctr = 0; ctr < MAX_MODULES; ctr++)
        cmodule[ctr] = NULL;
    shared.module = MODULE_LOADER;
    shared.modulemsg = MM_INIT;
    shared.forceminimize = false;
}
```

Now let's react if this variable is set to true. Add the following code to `OnNextFrame()`:

_main.h (8 lines)_
```c++
    if (shared.module == MODULE_CLEANUP)
        Quit();
    if (shared.forceminimize)
    {
        shared.forceminimize = false;
        Minimize();
    }
    return(E_OK);
```

When the game is being minimized and the level is active, head to the pause menu. This can be done by catching the minimize event by a module. First add functionality to `ClassMain` to forward minimize and restore events to modules:

_main.h (20 lines)_
```c++
//Callback: Called when the program minimizes
ERESULT ClassMain::OnMinimize(void)
{
    if (shared.module < MAX_MODULES)
        if (cmodule[shared.module] != NULL)
            cmodule[shared.module]->OnMinimize();
    return(E_OK);
}
 
//Callback: Called when the program restores back from a minimized state
ERESULT ClassMain::OnRestore(void)
{
    unsigned char ctr;
    for (ctr = 0; ctr < MAX_INPUTSTATE; ctr++)
        shared.inputstate[ctr] = IS_NONE;
    if (shared.module < MAX_MODULES)
        if (cmodule[shared.module] != NULL)
            cmodule[shared.module]->OnRestore();
    return(E_OK);
}
```

Then add an implementation of the `OnMinimize` event for the level module:

_level.h (5 lines)_
```c++
    public:
        ClassLevel(void);
        ~ClassLevel(void);
        void OnMinimize(void);
        void OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta);
```

_level.h (12 lines)_
```c++
//Module minimize callback event
void ClassLevel::OnMinimize(void)
{
    if (modulestate != MSLV_GAMEOVER)
    {
        shared->module = MODULE_MENU;
        shared->modulemsg = MM_FROMMODULE + MODULE_LEVEL;
    }
}
 
//Module game loop
void ClassLevel::OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta)
```

This will bring up the pause menu whenever the level module is minimized.

## Finishing the menu's
The last thing that needs to be done is to program the two menu items in-game and the pause menu. First let's add functionality to the two buttons in-game. These can be tapped with the stylus, or activated by pressing softbutton 1 or 2. These menu items will only work while in idle state:

_level.h (32 lines)_
```c++
        if (modulestate == MSLV_IDLE)
        {
            E_INPUTEVENT inputevent;
            while(shared->ecd->input->GetClickEvent(&inputevent))
            {
                if (inputevent.type == EIE_PRESSED)
                {
                    if (inputevent.clicky >= (long)(YRES - shared->font.GetHeight() - DIM_SPRITE * 3))
                    {
                        if (inputevent.clickx < XRES / 2)
                        {
                            shared->forceminimize = true;
                        }
                        else
                        {
                            *module = MODULE_MENU;
                            *modulemsg = MM_FROMMODULE + MODULE_LEVEL;
                        }
                    }
                }
            }
            if (shared->ecd->input->GetKeyEvent(DIAM_MENUSOFT1) == EIE_PRESSED)
            {
                shared->forceminimize = true;
            }
            else if (shared->ecd->input->GetKeyEvent(DIAM_MENUSOFT2) == EIE_PRESSED)
            {
                *module = MODULE_MENU;
                *modulemsg = MM_FROMMODULE + MODULE_LEVEL;
            }
        }
        RenderScreen(buffer);
```

Now let's expand the menu module to resume, minimize and quit the level:

_menu.h (10 lines)_
```c++
            case MI_MINIMIZE:
                shared->forceminimize = true;
                break;
            case MI_RESUMELEVEL:
                *module = MODULE_LEVEL;
                *modulemsg = MM_FROMMODULE + MODULE_MENU;
                break;
            case MI_QUITLEVEL:
                *modulemsg = MM_INIT;
                break;
```

## Chapters
* [Designing a game](tutorials_blastar_design.md)
* [The project framework](tutorials_blastar_framework.md)
* [Creating graphics](tutorials_blastar_graphics.md)
* [The loader module](tutorials_blastar_loader.md)
* [Platform definitions](tutorials_blastar_definitions.md)
* [The menu module](tutorials_blastar_menu.md)
* **The level module**
* [Using sprites](tutorials_blastar_sprites.md)
* [Player control](tutorials_blastar_control.md)
* [Wall of objects](tutorials_blastar_objects.md)
* [Collision detection](tutorials_blastar_collision.md)
* [The boss](tutorials_blastar_boss.md)
* [Refining the game](tutorials_blastar_refining.md)
* [Adding powerups](tutorials_blastar_powerups.md)
* [Special effects](tutorials_blastar_specialfx.md)
* [Completing the game](tutorials_blastar_completing.md)

