:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# The boss

## Creating a challenge
A boss needs to be challenging. This means the player must be constantly prepared for a variety of attacks. Also, the player mustn't be able to hide from boss attacks for a large amount of time. But the boss shouldn't be impossible. Things that make a boss more annoying than challenging are attacks that are impossible to evade, and a boss that takes a large amount of time to defeat.

The boss of Blastar will be a hostile version of the player's ship. He will be moving up and down on the other side of the wall, and fires missiles in certain patterns. Between each attack there will be a short period of rest, the player will be able to focus on the wall of objects again. His first attack is to focus lasers on specific parts of the screen, which will fire all at once. His second attack is to shoot a row of small bullets, the player needs to fly carefully to evade them.

## The boss sprite
First add the new sprite definitions to the level module:

_level.h (3 lines)_
```c++
#define SPR_WALL        4
#define SPR_BOSS        5
#define SPR_BOSSBULLET  6 
```

These sprites should be set up properly to use the correct sprite in the sheet, this will be done in the `CreateSprite` function:

_level.h (9 lines)_
```c++
                break;
            case SPR_BOSS:
                SetRect(&sprite[found].rc, 12, 18, 18, 24);
                sprite[found].coltype = COL_ENEMY;
                break;
            case SPR_BOSSBULLET:
                SetRect(&sprite[found].rc, 24, 16, 28, 18);
                sprite[found].coltype = COL_ENEMYSHOT;
                break;
```

Because the boss' spaceship tilts the same way as the player ship, we need to make an exception in the `RenderSprite` routine:

_level.h (8 lines)_
```c++
        if (sprite[ctr].type != SPR_NONE)
        {
            if (sprite[ctr].type == SPR_PLAYER || sprite[ctr].type == SPR_BOSS)
                GetAnimRect(&rc, &sprite[ctr].rc, sprite[ctr].state1 / 1024);
            else
                GetAnimRect(&rc, &sprite[ctr].rc, sprite[ctr].anitimer / sprite[ctr].anidelay % sprite[ctr].aniframes);
            buffer->BltFast(sprite[ctr].x / SPRITE_DIV, sprite[ctr].y / SPRITE_DIV, &shared->sheet, &rc, EFX_COLORKEY);
        }
```

The boss sprite will be added after creating a new level. Add it to the `CreateLevel` function:

_level.h (8 lines)_
```c++
    for (ctr = 0; ctr < num_generators; ctr++)
    {
        if (ctr % 2 == 0)
            CreateSprite(XRES / 2 - num_generators * DIM_SPRITE * 3 + ctr * DIM_SPRITE * 6, YRES / 2 + DIM_SPRITE * 32, SPR_WALLGEN, 0);
        else
            CreateSprite(XRES / 2 - num_generators * DIM_SPRITE * 3 + ctr * DIM_SPRITE * 6, YRES / 2 - DIM_SPRITE * 32, SPR_WALLGEN, 1);
    }
    CreateSprite(XRES - DIM_SPRITE * 8, YRES / 2, SPR_BOSS, 0);
```

## The boss movement routine
After adding the sprite, the sprite update function needs to be programmed for the boss. First we're going to program a simple up and down moving boss that also has a tilt animation, just like the player spaceship.

_level.h (44 lines)_
```c++
                break;
            case SPR_BOSS:
                if (modulestate == MSLV_IDLE)
                {
                    bool newroutine = false;
                    long tomove = 0;
                    if (sprite[ctr].y / SPRITE_DIV < (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / -2)
                    {
                        sprite[ctr].y = (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / -2 * SPRITE_DIV;
                        sprite[ctr].state2 = 1;
                        newroutine = true;
                    }
                    if (sprite[ctr].y / SPRITE_DIV > YRES - (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2)
                    {
                        sprite[ctr].y = (YRES - (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2) * SPRITE_DIV;
                        sprite[ctr].state2 = 0;
                        newroutine = true;
                    }
                    if (sprite[ctr].state2 == 0)
                        tomove = timedelta * MOVE_SPEED * -6;
                    else
                        tomove = timedelta * MOVE_SPEED * 6;
                    sprite[ctr].y += tomove;
                    sprite[ctr].state1 += tomove * 8 / MOVE_SPEED / 5;
                    if (sprite[ctr].state1 < 0)
                    {
                        if (ClassEMath::Abs(sprite[ctr].state1) < (long)timedelta * 5)
                            sprite[ctr].state1 = 0;
                        else
                            sprite[ctr].state1 += timedelta * 5;
                        if (sprite[ctr].state1 < -3000)
                            sprite[ctr].state1 = -3000;
                    }
                    else
                    {
                        if (ClassEMath::Abs(sprite[ctr].state1) < (long)timedelta * 5)
                            sprite[ctr].state1 = 0;
                        else
                            sprite[ctr].state1 -= timedelta * 5;
                        if (sprite[ctr].state1 > 3000)
                            sprite[ctr].state1 = 3000;
                    }
                }
                break;
```

The newroutine boolean variable isn't used yet. This will be needed for our new addition to the boss' movement routine. After taking a turn at a screen edge, the boss will pick a new random attack routine. This attack routine will be saved in the third state, the fourth state will be used as a timer or property variable for the attack. First let's add a simple attack routine, shooting of random bullets for 2 turns:

_level.h (40 lines)_
```c++
                    switch(sprite[ctr].state3)
                    {
                        case 3:
                        case 4:
                            if (sprite[ctr].state4 == 0)
                            {
                                CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE * 2, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_BOSSBULLET, 0);
                                sprite[ctr].state4 = 500 + shared->ecd->math->Rand() % 500;
                            }
                            else
                            {
                                if (sprite[ctr].state4 > (long)timedelta)
                                    sprite[ctr].state4 -= timedelta;
                                else
                                    sprite[ctr].state4 = 0;
                            }
                            if (newroutine)
                                sprite[ctr].state3--;
                            if (sprite[ctr].state3 == 2)
                                sprite[ctr].state3 = 0;
                            break;
                        default:
                            if (newroutine)
                            {
                                if (sprite[ctr].state3 == 0)
                                {
                                    sprite[ctr].state3 = 4;
                                    sprite[ctr].state4 = 0;
                                }
                                else if (sprite[ctr].state3 < 5)
                                    sprite[ctr].state3--;
                            }
                    }
                }
                break;
            case SPR_BOSSBULLET:
                sprite[ctr].x -= timedelta * MOVE_SPEED * 12;
                if (sprite[ctr].x / SPRITE_DIV + (sprite[ctr].rc.right - sprite[ctr].rc.left) < 0)
                    sprite[ctr].type = SPR_NONE;
                break;
```

The next attack is to focus a number of lasers that will fire at once. Two sprites need to be added, one sprite for the focus animation and the actual laser.

_level.h (3 lines)_
```c++
#define SPR_BOSSBULLET  6
#define SPR_LASERFOCUS  7
#define SPR_LASER       8 
```

_level.h (13 lines)_
```c++
            case SPR_BOSSBULLET:
                SetRect(&sprite[found].rc, 24, 16, 28, 18);
                sprite[found].coltype = COL_ENEMYSHOT;
                break;
            case SPR_LASERFOCUS:
                SetRect(&sprite[found].rc, 24, 14, 26, 16);
                sprite[found].aniframes = 4;
                sprite[found].anidelay = 64;
                break;
            case SPR_LASER:
                SetRect(&sprite[found].rc, 28, 17, 32, 18);
                sprite[found].coltype = COL_ENEMYSHOT;
                break;
```

The boss needs to pick his old attack or his new attack randomly. The sprite update routine will be expanded:

_level.h (38 lines)_
```c++
                        case 5:
                            if (sprite[ctr].state4 == 0)
                            {
                                CreateSprite(sprite[ctr].x / SPRITE_DIV, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_LASERFOCUS, 0);
                                sprite[ctr].state4 = 500;
                            }
                            else
                            {
                                if (sprite[ctr].state4 > (long)timedelta)
                                    sprite[ctr].state4 -= timedelta;
                                else
                                    sprite[ctr].state4 = 0;
                            }
                            if (newroutine)
                            {
                                unsigned long sctr;
                                for (sctr = 0; sctr < MAX_SPRITES; sctr++)
                                    if (sprite[sctr].type == SPR_LASERFOCUS)
                                    {
                                        sprite[sctr].type = SPR_NONE;
                                        CreateSprite(sprite[sctr].x / SPRITE_DIV + (sprite[sctr].rc.right - sprite[sctr].rc.left) / 2, sprite[sctr].y / SPRITE_DIV + (sprite[sctr].rc.bottom - sprite[sctr].rc.top) / 2, SPR_LASER, 0);
                                    }
                                sprite[ctr].state3 = 1;
                            }
                            break;
                        default:
                            if (newroutine)
                            {
                                if (sprite[ctr].state3 == 0)
                                {
                                    sprite[ctr].state3 = 4 + shared->ecd->math->Rand() % 2;
                                    sprite[ctr].state4 = 0;
                                    if (sprite[ctr].state3 == 5)
                                        sprite[ctr].state4 = shared->ecd->math->Rand() % 2 * 250;
                                }
                                else if (sprite[ctr].state3 < 5)
                                    sprite[ctr].state3--;
                            }
```

And finally, add the update routine for the laser itself:

_level.h (5 lines)_
```c++
            case SPR_LASER:
                sprite[ctr].x -= timedelta * MOVE_SPEED * 16;
                if (sprite[ctr].x / SPRITE_DIV + (sprite[ctr].rc.right - sprite[ctr].rc.left) < 0)
                    sprite[ctr].type = SPR_NONE;
                break;
```

## Player collision
The enemy bullets need to hurt the player. One bullet takes a life away until the player is game over. After the player is hit, the ship will blink for a short amount of time. When the player ship blinks, it will be temporarily invincible. The player ship has one unused state, which will be used for the blinking animation. Expand the sprite update routine:

_level.h (10 lines)_
```c++
                    if (sprite[ctr].state4 > (long)timedelta)
                        sprite[ctr].state4 -= timedelta;
                    else
                        sprite[ctr].state4 = 0;
                    if (sprite[ctr].state4 > 0)
                        sprite[ctr].coltype = COL_NONE;
                    else
                        sprite[ctr].coltype = COL_PLAYER;
                }
                break;
```

This will handle the blink timer for the player ship, and makes it invincible (ignores collision) when the timer is still active. Add an exception to the render routine for the blinking animation.

_level.h (9 lines)_
```c++
        if (sprite[ctr].type != SPR_NONE)
        {
            if (sprite[ctr].type == SPR_PLAYER || sprite[ctr].type == SPR_BOSS)
                GetAnimRect(&rc, &sprite[ctr].rc, sprite[ctr].state1 / 1024);
            else
                GetAnimRect(&rc, &sprite[ctr].rc, sprite[ctr].anitimer / sprite[ctr].anidelay % sprite[ctr].aniframes);
            if (!(sprite[ctr].type == SPR_PLAYER && sprite[ctr].state4 % 300 >= 150))
                buffer->BltFast(sprite[ctr].x / SPRITE_DIV, sprite[ctr].y / SPRITE_DIV, &shared->sheet, &rc, EFX_COLORKEY);
        }
```

The last thing to do is to react on a collision. Expand the `OnCollision` callback function:

_level.h (14 lines)_
```c++
    else if (sprite[nr].type == SPR_PLAYER && sprite[nr].state4 == 0)
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
```

## Defeating the boss
The boss needs to be hit as well. Expand the collision routine again:

_level.h (11 lines)_
```c++
    else if (sprite[nr].type == SPR_BOSS)
    {
        unsigned long ctr;
        for (ctr = 0; ctr < MAX_SPRITES; ctr++)
            if (sprite[ctr].type != SPR_PLAYER)
                sprite[ctr].type = SPR_NONE;
        curlevel++;
        CreateLevel();
        modulestate = MSLV_READY;
        msgtimer = 0;
    }
```

This will destroy the wall of sprites, and every other object except the player spaceship. Then it will generate a new level.

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
* **The boss**
* [Refining the game](tutorials_blastar_refining.md)
* [Adding powerups](tutorials_blastar_powerups.md)
* [Special effects](tutorials_blastar_specialfx.md)
* [Completing the game](tutorials_blastar_completing.md)

