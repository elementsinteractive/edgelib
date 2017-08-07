:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Collision detection

## Checking for collision
The wall of sprites has no use yet, it needs to stop bullets from passing through. To make this work, we need to check if each sprite collides against each other and react accordingly. For this game we're going to use pixel precise collision detection, which is very accurate. EDGELIB contains this functionality, but we need to build a table first. This needs to be done after setting up a colorkey and will only apply for the sprite sheet. Adjust the loader class:

_loader.h (17 lines)_
```c++
            case MSLD_SHEET:
                if (ef.PackGetFileData(filedata, filedatasize, "sheet.bmp") == E_OK)
                {
                    if (shared->ecd->dsp->CreateSurface(&shared->sheet, filedata, filedatasize) != E_OK)
                    {
                        *module = MODULE_CLEANUP;
                    }
                    else
                    {
                        shared->sheet.SetColorKey(ECOLOR_FUCHSIA);
                        shared->sheet.BuildTableMask();
                    }
                    ClassEMemory::DeAlloc(filedata);
                }
                else
                    *module = MODULE_CLEANUP;
                break;
```

After creating the table, we're going to expand the level class with collision detection. Two functions will be added. One that checks for collision between several sprites, and a function for the reaction between two collided sprites.

_level.h (4 lines)_
```c++
        void GetAnimRect(RECT *dst, RECT *src, long frame);
        void OnCollision(unsigned long nr, unsigned char withtype);
        void CheckCollision(void);
        void CreateLevel(void);
```

_level.h (36 lines)_
```c++
//On collision callback
void ClassLevel::OnCollision(unsigned long nr, unsigned char withtype)
{
    if (sprite[nr].type == SPR_NONE || withtype == SPR_NONE)
        return;
    if (sprite[nr].type == SPR_BULLET && withtype == SPR_WALL)
    {
        sprite[nr].type = SPR_NONE;
    }
    else if (sprite[nr].type == SPR_WALL && withtype == SPR_BULLET)
    {
        if (sprite[nr].health > 0)
            sprite[nr].health--;
        else
            sprite[nr].type = SPR_NONE;
    }
}
 
//Check for collision between sprites
void ClassLevel::CheckCollision(void)
{
    unsigned long ctr1, ctr2, numsprites = 0;
    unsigned char type1, type2;
    for (ctr1 = 0; ctr1 < MAX_SPRITES; ctr1++)
        if (sprite[ctr1].type != SPR_NONE)
            numsprites = ctr1 + 1;
    for (ctr1 = 0; ctr1 < numsprites; ctr1++)
        for (ctr2 = ctr1 + 1; ctr2 < numsprites; ctr2++)
            if (shared->sheet.Intersect(sprite[ctr1].x / SPRITE_DIV, sprite[ctr1].y / SPRITE_DIV, &sprite[ctr1].rc, sprite[ctr2].x / SPRITE_DIV, sprite[ctr2].y / SPRITE_DIV, &sprite[ctr2].rc, &shared->sheet))
            {
                type1 = sprite[ctr1].type;
                type2 = sprite[ctr2].type;
                OnCollision(ctr1, type2);
                OnCollision(ctr2, type1);
            }
}
```

Before calling the `OnCollision` callback function, the types of the colliding sprites need to be saved. This is done because the types in the structure may have changed, before the second `OnCollision` function has been called. Checking for collision will be done after updating all sprites:

_level.h (3 lines)_
```c++
        UpdateSprites(timedelta);
        CheckCollision();
        RenderScreen(buffer);
```

## Optimization
When a lot of sprites are onscreen, collision detection may take a relatively large amount of time in the gameloop. Because of this, optimizations are always welcome. A good optimization method for this type of game is to have a collision type for each sprite. Certain collision types don't react, and therefore don't need to be checked. This will save time checking for intersection, and calling the `OnCollision` callback function.

First we're going to make definitions for each collision type:

_level.h (13 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Collision types                                                 //
/////////////////////////////////////////////////////////////////////
 
#define COL_NONE        0
#define COL_PLAYER      1
#define COL_GOODSHOT    2
#define COL_ENEMY       3
#define COL_ENEMYSHOT   4
 
 
/////////////////////////////////////////////////////////////////////
// Structures                                                      // 
```

Certain types will hit with each other, the player with an enemy bullet and an enemy with a player bullet. There are also sprites that won't be able to collide, like the wall sprite generators (they are invincible). An array of booleans will determine if a combination of collision types will hit or not. The array will be added to the class as a private variable:

_level.h (3 lines)_
```c++
    private:
        SPRITE sprite[MAX_SPRITES];
        bool colmask[256];
```

This array will contain every combination of two collision types. We're reserving 16 definitions, just in case a few collision types will be added. The number of combinations will be: 16 * 16 = 256. The same amount of variables in the array. Each index in the array will be a certain combination, for example: index 36 will be a combination between `COL_GOODSHOT` and `COL_ENEMYSHOT` (2 * 16 + 4 = 36). An index will be set to true if a collision can be made between these two types, otherwise it's false.

First this array will be initialized with false, and colliding types will be set to true manually. This will be done in the constructor:

_level.h (11 lines)_
```c++
//ClassLevel: Constructor
ClassLevel::ClassLevel(void)
{
    unsigned short ctr;
    for (ctr = 0; ctr < 256; ctr++)
        colmask[ctr] = false;
    colmask[COL_PLAYER    * 16 + COL_ENEMYSHOT] = true;
    colmask[COL_GOODSHOT  * 16 + COL_ENEMY]     = true;
    colmask[COL_ENEMY     * 16 + COL_GOODSHOT]  = true;
    colmask[COL_ENEMYSHOT * 16 + COL_PLAYER]    = true;
}
```

After these preparations, each sprite will have an extra property: the collision type. This type will be set in the sprite creation function.

_level.h (3 lines)_
```c++
    unsigned char health;
    unsigned char coltype;
}SPRITE;
```

_level.h (3 lines)_
```c++
        sprite[found].health = 0;
        sprite[found].coltype = COL_NONE;
        sprite[found].state1 = 0;
```

_level.h (41 lines)_
```c++
        switch(type)
        {
            case SPR_PLAYER:
                SetRect(&sprite[found].rc, 12, 0, 18, 6);
                sprite[found].coltype = COL_PLAYER;
                sprite[found].state3 = 500;
                break;
            case SPR_BULLET:
                SetRect(&sprite[found].rc, 30, 0, 32, 1);
                sprite[found].coltype = COL_GOODSHOT;
                break;
            case SPR_WALLGEN:
                sprite[found].state1 = property;
                sprite[found].state2 = 15;
                if (curlevel < 5)
                    sprite[found].state3 = 1200;
                else if (curlevel < 9)
                    sprite[found].state3 = 600;
                else
                    sprite[found].state3 = 300;
                break;
            case SPR_WALL:
                SetRect(&sprite[found].rc, 0, 6, 4, 10);
                sprite[found].coltype = COL_ENEMY;
                sprite[found].aniframes = 8;
                sprite[found].anidelay = 64;
                sprite[found].health = 1 + curlevel / 2;
                if (sprite[found].health > 5)
                    sprite[found].health = 5;
                if (property == 0)
                    sprite[found].state1 = -1;
                else
                    sprite[found].state1 = 1;
                if (curlevel < 5)
                    sprite[found].state1 *= 1;
                else if (curlevel < 9)
                    sprite[found].state1 *= 2;
                else
                    sprite[found].state1 *= 4;
                break;
        }
```

Now let's expand the collision check algorithm with the new optimization:

_level.h (20 lines)_
```c++
//Check for collision between sprites
void ClassLevel::CheckCollision(void)
{
    unsigned long ctr1, ctr2, numsprites = 0;
    unsigned char type1, type2;
    for (ctr1 = 0; ctr1 < MAX_SPRITES; ctr1++)
        if (sprite[ctr1].type != SPR_NONE)
            if (sprite[ctr1].coltype != COL_NONE)
                numsprites = ctr1 + 1;
    for (ctr1 = 0; ctr1 < numsprites; ctr1++)
        for (ctr2 = ctr1 + 1; ctr2 < numsprites; ctr2++)
            if (colmask[sprite[ctr1].coltype * 16 + sprite[ctr2].coltype])
                if (shared->sheet.Intersect(sprite[ctr1].x / SPRITE_DIV, sprite[ctr1].y / SPRITE_DIV, &sprite[ctr1].rc, sprite[ctr2].x / SPRITE_DIV, sprite[ctr2].y / SPRITE_DIV, &sprite[ctr2].rc, &shared->sheet))
                {
                    type1 = sprite[ctr1].type;
                    type2 = sprite[ctr2].type;
                    OnCollision(ctr1, type2);
                    OnCollision(ctr2, type1);
                }
}
```

For the bullet collision event, don't check for the `withtype` argument anymore. Remove the code that isn't needed:

_level.h (3 lines)_
```c++
    if (sprite[nr].type == SPR_NONE || withtype == SPR_NONE)
        return;
    if (sprite[nr].type == SPR_BULLET)
```

## Chapters
* [Designing a game](tutorials_blastar_design.md)
* [The project framework](tutorials_blastar_framework.md)
* [Creating graphics](tutorials_blastar_graphics.md)
* [The loader module](tutorials_blastar_loader.md)
* [Platform definitions](tutorials_blastar_definitions.md)
* [The menu module](tutorials_blastar_menu.md)
* [The level module](tutorials_blastar_level.md)
* [Using sprites](tutorials_blastar_sprites.md)
* [Player control](tutorials_blastar_control.md)
* [Wall of objects](tutorials_blastar_objects.md)
* **Collision detection**
* [The boss](tutorials_blastar_boss.md)
* [Refining the game](tutorials_blastar_refining.md)
* [Adding powerups](tutorials_blastar_powerups.md)
* [Special effects](tutorials_blastar_specialfx.md)
* [Completing the game](tutorials_blastar_completing.md)

