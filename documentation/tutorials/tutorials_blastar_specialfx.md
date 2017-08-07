:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Special effects

## Sprite filters
Now that the gameplay is finished, it's time to add some eyecandy. First we're going to add a useful effect when a wall sprite is hit by a bullet. The sprite will be colored white for a short amount of time to show the player it has been hit. This can be done by using a colorize drawing filter.

First let's expand the sprite structure with 2 variables, one for storing the color filter and one for storing the property of this filter.

_level.h (18 lines)_
```c++
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
    unsigned long filter;
    unsigned long filterparam;
    unsigned char aniframes;
    unsigned char type;
    unsigned char health;
    unsigned char coltype;
}SPRITE;
```

These will be set to their defaults within the sprite creation function:

_level.h (4 lines)_
```c++
        sprite[found].aniframes = 1;
        sprite[found].filter = EFX_COLORKEY;
        sprite[found].filterparam = 0;
        sprite[found].health = 0;
```

Also adjust the render routine:

_level.h (2 lines)_
```c++
            if (!(sprite[ctr].type == SPR_PLAYER && sprite[ctr].state4 % 300 >= 150))
                buffer->BltFast(sprite[ctr].x / SPRITE_DIV, sprite[ctr].y / SPRITE_DIV, &shared->sheet, &rc, sprite[ctr].filter, sprite[ctr].filterparam);
```

## Flashing wall sprite
When a wall sprite has been hit, it will be colorized for a short amount of time which will be stored in the third state. First set this timer value on the collision event:

_level.h (3 lines)_
```c++
    else if (sprite[nr].type == SPR_WALL && (withtype == SPR_BULLET || withtype == SPR_POWBULLET))
    {
        sprite[nr].state3 = 150;
```

This state will decrease in the sprite update routine until it reaches zero. When the value is bigger than zero, the sprite will be drawn with a colorize filter:

_level.h (16 lines)_
```c++
            case SPR_WALL:
                if (sprite[ctr].state3)
                {
                    if (sprite[ctr].state3 <= (long)timedelta)
                    {
                        sprite[ctr].filter = EFX_COLORKEY;
                        sprite[ctr].state3 = 0;
                    }
                    else
                    {
                        sprite[ctr].filter = EFX_COLORKEY | EFX_COLORIZE;
                        sprite[ctr].filterparam = ECOLOR_WHITE;
                        sprite[ctr].state3 -= timedelta;
                    }
                }
                sprite[ctr].y += timedelta * MOVE_SPEED * sprite[ctr].state1;
```

## Particles
Particles are a good way to handle the removal of objects in-game. It gives a more natural look and makes the game pretty. A particle will be a new sprite, add it to the sprite definition list:

_level.h (3 lines)_
```c++
#define SPR_POWBULLET  10
#define SPR_WHITEPART  11
#define SPR_REDPART    12 
```

These are 2 particles, a white and a red one. These will be linked to the correct sprite sheet picture on the sprite creation function, also they will have the additive alpha filter:

_level.h (10 lines)_
```c++
            case SPR_WHITEPART:
                SetRect(&sprite[found].rc, 30, 18, 31, 19);
                sprite[found].filter = EFX_COLORKEY | EFX_ALPHA;
                sprite[found].state1 = property * 16;
                break;
            case SPR_REDPART:
                SetRect(&sprite[found].rc, 31, 18, 32, 19);
                sprite[found].filter = EFX_COLORKEY | EFX_ALPHA;
                sprite[found].state1 = property * 16;
                break;
```

These 2 particles will have the same movement routine. A particle moves into a direction for a short amount of time until it disappears. It will also change the position in the sprite sheet to simulate a fading particle when the time increases.

_level.h (10 lines)_
```c++
            case SPR_WHITEPART:
            case SPR_REDPART:
                sprite[ctr].x += (long)timedelta * MOVE_SPEED * shared->ecd->math->Sin(sprite[ctr].state1) / 10000;
                sprite[ctr].y -= (long)timedelta * MOVE_SPEED * shared->ecd->math->Cos(sprite[ctr].state1) / 10000;
                sprite[ctr].state2 += timedelta;
                sprite[ctr].rc.top = (18 + sprite[ctr].state2 * 6 / 512) * DIM_SPRITE;
                sprite[ctr].rc.bottom = sprite[ctr].rc.top + DIM_SPRITE;
                if (sprite[ctr].state2 >= 512)
                    sprite[ctr].type = SPR_NONE;
                break;
```

The white particles will be shown when a bullet hits something, spawn 2 particles in the collision event:

_level.h (7 lines)_
```c++
    if (sprite[nr].type == SPR_BULLET || sprite[nr].type == SPR_POWBULLET)
    {
        unsigned char direction = (unsigned char)(shared->ecd->math->Rand() % 32);
        CreateSprite(sprite[nr].x / SPRITE_DIV + DIM_SPRITE, sprite[nr].y / SPRITE_DIV, SPR_WHITEPART, 160 + direction);
        CreateSprite(sprite[nr].x / SPRITE_DIV + DIM_SPRITE, sprite[nr].y / SPRITE_DIV, SPR_WHITEPART, 192 + direction);
        sprite[nr].type = SPR_NONE;
    }
```

Create 8 red particles when a wall sprite has been destroyed:

_level.h (7 lines)_
```c++
            unsigned char ctr, direction = (unsigned char)(shared->ecd->math->Rand() % 32);
            sprite[nr].type = SPR_NONE;
            score += 10 * curlevel;
            if (shared->ecd->math->Rand() % 100 < 15)
                CreateSprite(sprite[nr].x / SPRITE_DIV + DIM_SPRITE * 2, sprite[nr].y / SPRITE_DIV + DIM_SPRITE * 2, SPR_POWERUP, shared->ecd->math->Rand() % 4);
            for (ctr = 0; ctr < 8; ctr++)
                CreateSprite(sprite[nr].x / SPRITE_DIV + DIM_SPRITE * 2, sprite[nr].y / SPRITE_DIV + DIM_SPRITE * 2, SPR_REDPART, ctr * 32 + direction);
```

## Enemy bullet particle
Let's also add a particle when the boss shoots a bullet. This will be added to the sprite definition list, to the sprite creation function and to the sprite update function:

_level.h (3 lines)_
```c++
#define SPR_WHITEPART  11
#define SPR_REDPART    12
#define SPR_BULLETPART 13 
```

_level.h (3 lines)_
```c++
            case SPR_BULLETPART:
                SetRect(&sprite[found].rc, 28, 16, 30, 17);
                break;
```

_level.h (8 lines)_
```c++
            case SPR_BULLETPART:
                sprite[ctr].x += timedelta * MOVE_SPEED * 5;
                sprite[ctr].state1 += timedelta;
                sprite[ctr].rc.left = (28 + sprite[ctr].state1 * 2 / 512 * 2) * DIM_SPRITE;
                sprite[ctr].rc.right = sprite[ctr].rc.left + DIM_SPRITE * 2;
                if (sprite[ctr].state1 >= 512)
                    sprite[ctr].type = SPR_NONE;
                break;
```

The boss bullet sprite will spawn these particles while it moves. Expand the update routine to create these particles:

_level.h (11 lines)_
```c++
            case SPR_BOSSBULLET:
                sprite[ctr].x -= timedelta * MOVE_SPEED * 12;
                if (sprite[ctr].state1 < (long)timedelta)
                {
                    CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE, sprite[ctr].y / SPRITE_DIV + DIM_SPRITE, SPR_BULLETPART, 0);
                    sprite[ctr].state1 = 384 + shared->ecd->math->Rand() % 128;
                }
                else
                    sprite[ctr].state1 -= timedelta;
                if (sprite[ctr].x / SPRITE_DIV + (sprite[ctr].rc.right - sprite[ctr].rc.left) < 0)
                    sprite[ctr].type = SPR_NONE;
```

## Flashing boss sprite
The boss will get the same flash effect as the wall sprite and needs to be hit multiple times. Because there is no state left for the boss' flash animation, we're going to add a fifth state to the sprite structure. Also initialize it to zero in the sprite creation function and initialize the health value of the boss:

_level.h (3 lines)_
```c++
    long state3;
    long state4;
    long state5;
```

_level.h (3 lines)_
```c++
        sprite[found].state3 = 0;
        sprite[found].state4 = 0;
        sprite[found].state5 = 0;
```

_level.h (5 lines)_
```c++
            case SPR_BOSS:
                SetRect(&sprite[found].rc, 12, 18, 18, 24);
                sprite[found].coltype = COL_ENEMY;
                sprite[found].health = 4;
                break;
```

The sprite update routine of the boss needs to be updated to add the flashing effect when he's hit:

_level.h (15 lines)_
```c++
            case SPR_BOSS:
                if (sprite[ctr].state5)
                {
                    if (sprite[ctr].state5 <= (long)timedelta)
                    {
                        sprite[ctr].filter = EFX_COLORKEY;
                        sprite[ctr].state5 = 0;
                    }
                    else
                    {
                        sprite[ctr].filter = EFX_COLORKEY | EFX_COLORIZE;
                        sprite[ctr].filterparam = ECOLOR_WHITE;
                        sprite[ctr].state5 -= timedelta;
                    }
                }
```

Expand the collision event for the boss as well:

_level.h (20 lines)_
```c++
    else if (sprite[nr].type == SPR_BOSS)
    {
        if (sprite[nr].health > 0)
        {
            sprite[nr].health--;
            sprite[nr].state5 = 150;
        }
        else
        {
            unsigned long ctr;
            for (ctr = 0; ctr < MAX_SPRITES; ctr++)
                if (sprite[ctr].type != SPR_PLAYER)
                    sprite[ctr].type = SPR_NONE;
            curlevel++;
            score += 1000;
            CreateLevel();
            modulestate = MSLV_READY;
            msgtimer = 0;
        }
    }
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
* [Collision detection](tutorials_blastar_collision.md)
* [The boss](tutorials_blastar_boss.md)
* [Refining the game](tutorials_blastar_refining.md)
* [Adding powerups](tutorials_blastar_powerups.md)
* **Special effects**
* [Completing the game](tutorials_blastar_completing.md)

