:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Adding powerups

## Make the game more exciting
Right now the gameplay works, but there isn't much going on. The gameplay needs more variety. To do this, we're going to add a number of powerups. A powerdown will also be added for extra challenge. Powerups include a triple-shot bullet, rapid shooting and strong bullets that are able to destroy a wall sprite in one shot. These powerups will last a few seconds, depending on the powerup. The powerdown will switch up and down movement.

To add powerups we need a new sprite and a new collision type (for picking up powerups), powerup definitions and expand the bullet sprite update/creation routines. First let's add the neccessary definitions:

_level.h (40 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Sprite definitions                                              //
/////////////////////////////////////////////////////////////////////
 
#define SPR_NONE        0
#define SPR_PLAYER      1
#define SPR_BULLET      2
#define SPR_WALLGEN     3
#define SPR_WALL        4
#define SPR_BOSS        5
#define SPR_BOSSBULLET  6
#define SPR_LASERFOCUS  7
#define SPR_LASER       8
#define SPR_POWERUP     9
 
#define MAX_SPRITES   128
#define SPRITE_DIV    256
 
 
/////////////////////////////////////////////////////////////////////
// Collision types                                                 //
/////////////////////////////////////////////////////////////////////
 
#define COL_NONE        0
#define COL_PLAYER      1
#define COL_GOODSHOT    2
#define COL_ENEMY       3
#define COL_ENEMYSHOT   4
#define COL_POWERUP     5
 
 
/////////////////////////////////////////////////////////////////////
// Powerup types                                                   //
/////////////////////////////////////////////////////////////////////
 
#define PUP_NONE        0
#define PUP_RAPIDFIRE   1
#define PUP_TRIPLESHOT  2
#define PUP_POWERSHOT   3
#define PUP_SWITCH      4 
```

The new collision definition needs to be set up properly. This powerup collision type will only collide with the player collision type:

_level.h (6 lines)_
```c++
    colmask[COL_PLAYER    * 16 + COL_ENEMYSHOT] = true;
    colmask[COL_PLAYER    * 16 + COL_POWERUP]   = true;
    colmask[COL_GOODSHOT  * 16 + COL_ENEMY]     = true;
    colmask[COL_ENEMY     * 16 + COL_GOODSHOT]  = true;
    colmask[COL_ENEMYSHOT * 16 + COL_PLAYER]    = true;
    colmask[COL_POWERUP   * 16 + COL_PLAYER]    = true;
```

## The powerup sprite
Before adding the powerup sprite itself, we need to add a powerup timer variable to the class definition and reset this variable to zero on level creation. Also the powerup itself needs to be reset in this function.

_level.h (3 lines)_
```c++
        unsigned long msgtimer;
        unsigned long timerspeed;
        unsigned long puptimer;
```

_level.h (4 lines)_
```c++
    spritetimer.Reset(0);
    spritetimer.SetSpeed(timerspeed);
    powerup = PUP_NONE;
    puptimer = 0;
```

The powerup sprite can be added. First link it to the correct sheet position within the `CreateSprite` function. The type of powerup will be determined by the property parameter of this function and will be saved in the first state of the sprite. The position of the sheet will depend on the powerup type.

_level.h (9 lines)_
```c++
            case SPR_LASER:
                SetRect(&sprite[found].rc, 28, 17, 32, 18);
                sprite[found].coltype = COL_ENEMYSHOT;
                break;
            case SPR_POWERUP:
                SetRect(&sprite[found].rc, property * 4, 14, 4 + property * 4, 18);
                sprite[found].coltype = COL_POWERUP;
                sprite[found].state1 = property;
                break;
```

And add the movement routine:

_level.h (10 lines)_
```c++
            case SPR_LASER:
                sprite[ctr].x -= timedelta * MOVE_SPEED * 16;
                if (sprite[ctr].x / SPRITE_DIV + (sprite[ctr].rc.right - sprite[ctr].rc.left) < 0)
                    sprite[ctr].type = SPR_NONE;
                break;
            case SPR_POWERUP:
                sprite[ctr].x -= timedelta * MOVE_SPEED * 2;
                if (sprite[ctr].x / SPRITE_DIV + (sprite[ctr].rc.right - sprite[ctr].rc.left) < 0)
                    sprite[ctr].type = SPR_NONE;
                break;
```

## Picking up powerups
The sprite is ready, but the powerups still need to appear. In this game, a powerup will appear randomly when a wall sprite has been destroyed. Adjust the collision event of a wall sprite:

_level.h (14 lines)_
```c++
    else if (sprite[nr].type == SPR_WALL && withtype == SPR_BULLET)
    {
        if (sprite[nr].health > 0)
        {
            sprite[nr].health--;
        }
        else
        {
            sprite[nr].type = SPR_NONE;
            score += 10 * curlevel;
            if (shared->ecd->math->Rand() % 100 < 15)
                CreateSprite(sprite[nr].x / SPRITE_DIV + DIM_SPRITE * 2, sprite[nr].y / SPRITE_DIV + DIM_SPRITE * 2, SPR_POWERUP, (unsigned char)(shared->ecd->math->Rand() % 4));
        }
    }
```

Now something goes wrong. When a player tries to pick up a powerup sprite, he will lose a life. It's needed to adjust the collision event for the player ship:

_level.h (17 lines)_
```c++
    else if (sprite[nr].type == SPR_PLAYER && sprite[nr].state4 == 0)
    {
        if (withtype == SPR_BOSSBULLET || withtype == SPR_LASER)
        {
            if (lives > 0)
            {
                sprite[nr].state4 = 2000;
                lives--;
            }
            else
            {
                sprite[nr].type = SPR_NONE;
                modulestate = MSLV_GAMEOVER;
                msgtimer = 0;
            }
        }
    }
```

And add a new collision event for the powerup sprite itself:

_level.h (6 lines)_
```c++
    else if (sprite[nr].type == SPR_POWERUP)
    {
        sprite[nr].type = SPR_NONE;
        powerup = (unsigned char)sprite[nr].state1 + 1;
        puptimer = 5000;
    }
```

Finally keep track of the powerup timer, and set the powerup to none after the timer has reached zero. This will be done in the `OnNextFrame` function:

_level.h (9 lines)_
```c++
        if (puptimer <= timedelta)
        {
            puptimer = 0;
            powerup = PUP_NONE;
        }
        else
            puptimer -= timedelta;
        if (timerspeed > 1000)
        {
```

## Power up and power down
First let's program the easy powerup types, these will be the rapid fire powerup and the switched movement powerdown. Rapid firing can be easily done by changing the shooting time behaviour of the player sprite.

_level.h (8 lines)_
```c++
                        while (sprite[ctr].state3 >= 500)
                        {
                            CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE * 6, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_BULLET, 0);
                            if (powerup == PUP_RAPIDFIRE)
                                sprite[ctr].state3 -= 200;
                            else
                                sprite[ctr].state3 -= 500;
                        }
```

Switched movement is also easy, because there is one variable that keeps track of the spaceship movement:

_level.h (4 lines)_
```c++
                    if (powerup == PUP_SWITCH)
                        tomove = -tomove;
                    sprite[ctr].y += tomove;
                    sprite[ctr].state1 += tomove * 8 / MOVE_SPEED / 5;
```

## Triple shot
Triple shooting will be a bit more complicated because the bullet sprite routine needs to be changed as well. The property for the sprite creation function will be used to select the bullet direction and picture: straight, diagonal up or diagonal down.

_level.h (10 lines)_
```c++
            case SPR_BULLET:
                if (property == 0)
                    SetRect(&sprite[found].rc, 30, 0, 32, 1);
                else if (property == 1)
                    SetRect(&sprite[found].rc, 30, 2, 32, 4);
                else if (property == 2)
                    SetRect(&sprite[found].rc, 30, 4, 32, 6);
                sprite[found].coltype = COL_GOODSHOT;
                sprite[found].state1 = property;
                break;
```

And expand the sprite update function to enable diagonal movement for the bullet:

_level.h (14 lines)_
```c++
            case SPR_BULLET:
                if (sprite[ctr].state1 == 1 || sprite[ctr].state1 == 2)
                {
                    if (sprite[ctr].state1 == 1)
                        sprite[ctr].y -= timedelta * MOVE_SPEED * 6;
                    else
                        sprite[ctr].y += timedelta * MOVE_SPEED * 6;
                    sprite[ctr].x += timedelta * MOVE_SPEED * 10;
                }
                else
                    sprite[ctr].x += timedelta * MOVE_SPEED * 12;
                if (sprite[ctr].x / SPRITE_DIV >= XRES)
                    sprite[ctr].type = SPR_NONE;
                break;
```

Expand the movement routine of the player to shoot 3 shots when the triple shot powerup is active:

_level.h (6 lines)_
```c++
                            CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE * 6, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_BULLET, 0);
                            if (powerup == PUP_TRIPLESHOT)
                            {
                                CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE * 6, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_BULLET, 1);
                                CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE * 6, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_BULLET, 2);
                            }
```

## Powershot
For the powershot powerup, we need a new sprite type. Add it to the list of sprite definitions:

_level.h (2 lines)_
```c++
#define SPR_POWERUP     9
#define SPR_POWBULLET  10 
```

Add it to the switch-case structure of the sprite creation function:

_level.h (4 lines)_
```c++
            case SPR_POWBULLET:
                SetRect(&sprite[found].rc, 30, 1, 32, 2);
                sprite[found].coltype = COL_GOODSHOT;
                break;
```

And program its movement routine:

_level.h (5 lines)_
```c++
            case SPR_POWBULLET:
                sprite[ctr].x += timedelta * MOVE_SPEED * 10;
                if (sprite[ctr].x / SPRITE_DIV >= XRES)
                    sprite[ctr].type = SPR_NONE;
                break;
```

This will create a slow moving bullet with a different graphic picture than an ordinary bullet. Adjust the player movement routine to shoot these bullets when the powershot powerup is active:

_level.h (5 lines)_
```c++
                            if (powerup == PUP_POWERSHOT)
                                CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE * 6, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_POWBULLET, 0);
                            else
                                CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE * 6, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_BULLET, 0);
                            if (powerup == PUP_TRIPLESHOT)
```

The power bullet needs to disappear when hitting a wall object, also a wall object must be destroyed when hit by this type of bullet. Adjust the collision events:

_level.h (18 lines)_
```c++
    if (sprite[nr].type == SPR_BULLET || sprite[nr].type == SPR_POWBULLET)
    {
        sprite[nr].type = SPR_NONE;
    }
    else if (sprite[nr].type == SPR_WALL && (withtype == SPR_BULLET || withtype == SPR_POWBULLET))
    {
        if (sprite[nr].health > 0 && withtype == SPR_BULLET)
        {
            sprite[nr].health--;
        }
        else
        {
            sprite[nr].type = SPR_NONE;
            score += 10 * curlevel;
            if (shared->ecd->math->Rand() % 100 < 15)
                CreateSprite(sprite[nr].x / SPRITE_DIV + DIM_SPRITE * 2, sprite[nr].y / SPRITE_DIV + DIM_SPRITE * 2, SPR_POWERUP, (unsigned char)(shared->ecd->math->Rand() % 4));
        }
    }
```

## Fix a bug
After introducing these powerups, a bug has also been introduced. When the player spaceship sprite blinks, it will also ignore the powerup collision which shouldn't happen. A way to fix this is to introduce a new collision type that won't react on enemy projectiles, but does react on powerups. Add it to the definition list:

_level.h (2 lines)_
```c++
#define COL_POWERUP     5
#define COL_PLAYERHIT   6 
```

Initialize it properly in the class constructor:

_level.h (3 lines)_
```c++
    colmask[COL_POWERUP   * 16 + COL_PLAYER]    = true;
    colmask[COL_POWERUP   * 16 + COL_PLAYERHIT] = true;
    colmask[COL_PLAYERHIT * 16 + COL_POWERUP]   = true;
```

And finally, expand the player sprite update routine to use this new collision type in blinking state:

_level.h (4 lines)_
```c++
                    if (sprite[ctr].state4 > 0)
                        sprite[ctr].coltype = COL_PLAYERHIT;
                    else
                        sprite[ctr].coltype = COL_PLAYER;
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
* **Adding powerups**
* [Special effects](tutorials_blastar_specialfx.md)
* [Completing the game](tutorials_blastar_completing.md)

