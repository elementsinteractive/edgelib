:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Using sprites

## Adding sprites to the level module
Sprites are several objects in-game which have a graphics picture and a certain animation. For example the player's spaceship and enemies. Each sprite will contain a reference to the sprite sheet (which graphic to use), animation information, a type, position and a number of additional properties. These will be saved in an array of structures within the level module. First we're going to add a number of definitions: sprite types, the maximum number of sprites onscreen and an animation divider:

_level.h (13 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Sprite definitions                                              //
/////////////////////////////////////////////////////////////////////
 
#define SPR_NONE        0
#define SPR_PLAYER      1
 
#define MAX_SPRITES   128
#define SPRITE_DIV    256
 
 
/////////////////////////////////////////////////////////////////////
// Class definition                                                // 
```

Then we're going to add a sprite structure for saving sprite properties:

_level.h (24 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Structures                                                      //
/////////////////////////////////////////////////////////////////////
 
typedef struct
{
    RECT rc;
    long x;
    long y;
    long state1;
    long state2;
    long state3;
    long state4;
    unsigned long anitimer;
    unsigned long anidelay;
    unsigned char aniframes;
    unsigned char type;
    unsigned char action;
    unsigned char health;
}SPRITE;
 
 
/////////////////////////////////////////////////////////////////////
// Class definition                                                // 
```

And finally, add an array of sprites to the level class which needs to be emptied when the level starts. Also variables are added for tracking the player status, like lives and score:

_level.h (7 lines)_
```c++
    private:
        SPRITE sprite[MAX_SPRITES];
        unsigned char lives;
        unsigned char powerup;
        unsigned char curlevel;
        unsigned long score;
        unsigned long msgtimer;
```

_level.h (12 lines)_
```c++
    if (*modulemsg == MM_INIT)
    {
        unsigned long ctr;
        for (ctr = 0; ctr < MAX_SPRITES; ctr++)
            sprite[ctr].type = SPR_NONE;
        lives = 2;
        score = 0;
        curlevel = shared->cfg.startlevel;
        modulestate = MSLV_READY;
        msgtimer = 0;
        *modulemsg = MM_NONE;
    }
```

## Sprite creation
For adding sprites in the level screen, we're going to create a function to ClassLevel. This function will check if there is a sprite available in the list, and sets every sprite member variable to a default value. A switch-case structure will take care of custom sprite properties per type, for example the spritesheet rectangle.

_level.h (3 lines)_
```c++
        unsigned long CreateSprite(long x, long y, unsigned char type, unsigned char property);
        void RenderScreen(E2DSurface *buffer);
    public:
```

_level.h (35 lines)_
```c++
//Create a new sprite
unsigned long ClassLevel::CreateSprite(long x, long y, unsigned char type, unsigned char property)
{
    unsigned long ctr, found = MAX_SPRITES;
    for (ctr = 0; ctr < MAX_SPRITES; ctr++)
        if (sprite[ctr].type == SPR_NONE)
        {
            found = ctr;
            break;
        }
    if (found < MAX_SPRITES)
    {
        SetRect(&sprite[found].rc, 0, 0, 0, 0);
        sprite[found].type = type;
        sprite[found].anitimer = 0;
        sprite[found].anidelay = 1;
        sprite[found].aniframes = 1;
        sprite[found].health = 0;
        sprite[found].state1 = 0;
        sprite[found].state2 = 0;
        sprite[found].state3 = 0;
        sprite[found].state4 = 0;
        switch(type)
        {
            case SPR_PLAYER:
                SetRect(&sprite[found].rc, 12, 0, 18, 6);
                sprite[found].state3 = 500;
                break;
        }
        SetRect(&sprite[found].rc, sprite[found].rc.left * DIM_SPRITE, sprite[found].rc.top * DIM_SPRITE, sprite[found].rc.right * DIM_SPRITE, sprite[found].rc.bottom * DIM_SPRITE);
        sprite[found].x = (x - (sprite[found].rc.right - sprite[found].rc.left) / 2) * SPRITE_DIV;
        sprite[found].y = (y - (sprite[found].rc.bottom - sprite[found].rc.top) / 2) * SPRITE_DIV;
    }
    return(found);
}
```

The player sprite can be created at the left screen edge. This will be done when the game starts, or when the player ship is destroyed. Create this sprite when the game starts:

_level.h (6 lines)_
```c++
    if (*modulemsg == MM_INIT)
    {
        unsigned long ctr;
        for (ctr = 0; ctr < MAX_SPRITES; ctr++)
            sprite[ctr].type = SPR_NONE;
        CreateSprite(DIM_SPRITE * 8, YRES / 2, SPR_PLAYER, 0);
```

## Rendering sprites
A new function will be added to render all sprites to the screen. A sprite is taken from the sheet with the help of the rectangle sprite member variable. Another function will be added to change this rectangle, based on the animation properties. This function has an exception for the player ship sprite. This will take an animation frame based on his turning (which is stored in state1), rather than animating automatically.

_level.h (4 lines)_
```c++
        unsigned long CreateSprite(long x, long y, unsigned char type, unsigned char property);
        void GetAnimRect(RECT *dst, RECT *src, long frame);
        void RenderSprites(E2DSurface *buffer);
        void RenderScreen(E2DSurface *buffer);
```

_level.h (24 lines)_
```c++
//Calculate a rectangle based on animation
void ClassLevel::GetAnimRect(RECT *dst, RECT *src, long frame)
{
    SetRect(dst, src->left + frame * (src->right - src->left), src->top, src->right + frame * (src->right - src->left), src->bottom);
}
 
//Render all sprites
void ClassLevel::RenderSprites(E2DSurface *buffer)
{
    RECT rc;
    unsigned long ctr;
    for (ctr = 0; ctr < MAX_SPRITES; ctr++)
        if (sprite[ctr].type != SPR_NONE)
        {
            if (sprite[ctr].type == SPR_PLAYER)
                GetAnimRect(&rc, &sprite[ctr].rc, sprite[ctr].state1 / 1024);
            else
                GetAnimRect(&rc, &sprite[ctr].rc, sprite[ctr].anitimer / sprite[ctr].anidelay % sprite[ctr].aniframes);
            buffer->BltFast(sprite[ctr].x / SPRITE_DIV, sprite[ctr].y / SPRITE_DIV, &shared->sheet, &rc, EFX_COLORKEY);
        }
}
 
//Render the graphics on the screen
void ClassLevel::RenderScreen(E2DSurface *buffer)
```

Call this function after drawing the background, but before drawing the menu buttons:

_level.h (6 lines)_
```c++
//Render the graphics on the screen
void ClassLevel::RenderScreen(E2DSurface *buffer)
{
    RECT rc;
    buffer->BltFast(0, 0, &shared->gamebg, NULL);
    RenderSprites(buffer);
```

## Chapters
* [Designing a game](tutorials_blastar_design.md)
* [The project framework](tutorials_blastar_framework.md)
* [Creating graphics](tutorials_blastar_graphics.md)
* [The loader module](tutorials_blastar_loader.md)
* [Platform definitions](tutorials_blastar_definitions.md)
* [The menu module](tutorials_blastar_menu.md)
* [The level module](tutorials_blastar_level.md)
* **Using sprites**
* [Player control](tutorials_blastar_control.md)
* [Wall of objects](tutorials_blastar_objects.md)
* [Collision detection](tutorials_blastar_collision.md)
* [The boss](tutorials_blastar_boss.md)
* [Refining the game](tutorials_blastar_refining.md)
* [Adding powerups](tutorials_blastar_powerups.md)
* [Special effects](tutorials_blastar_specialfx.md)
* [Completing the game](tutorials_blastar_completing.md)

