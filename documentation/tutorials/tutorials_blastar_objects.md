:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Wall of objects

## Implementation
It's time to add more interaction into our game. A wall of objects will be generated in the middle of the screen. This wall will be created by having a number of sprites which will move up or down, depending on the column. For example, column one will move up while column two will move down. The number of columns depends on the level, starting with three columns and increasing to a maximum of six columns. The movement speed will also increase, to add challenge.

To generate this wall, we need two sprite types. A generator creating a number of sprites, and the sprites themselves. The generator is created when the level starts and will create 15 sprites and disappear. The wall sprites will move until it reaches the edge of the screen. For example, when an up-moving sprite reaches the top of the screen, it will be placed at the bottom and it will move up again.

## Adding sprite types
First let's add new sprite type definitions to the level:

_level.h (3 lines)_
```c++
#define SPR_BULLET      2
#define SPR_WALLGEN     3
#define SPR_WALL        4 
```

The wall generator will be invisible and doesn't have a selected rectangle in the sprite sheet. The wall sprite contains 8 frames of animation with a delay of 64 milli seconds between each frame. A property can also be given when creating a sprite. This will determine whether the generator creates up-moving or down-moving sprites.

_level.h (31 lines)_
```c++
            case SPR_BULLET:
                SetRect(&sprite[found].rc, 30, 0, 32, 1);
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
```

And finally, program the update routines of these sprites:

_level.h (28 lines)_
```c++
            case SPR_BULLET:
                sprite[ctr].x += timedelta * MOVE_SPEED * 12;
                if (sprite[ctr].x / SPRITE_DIV >= XRES)
                    sprite[ctr].type = SPR_NONE;
                break;
            case SPR_WALLGEN:
                sprite[ctr].state4 += timedelta;
                while(sprite[ctr].state4 >= sprite[ctr].state3 && sprite[ctr].state2 > 0)
                {
                    sprite[ctr].state4 -= sprite[ctr].state3;
                    sprite[ctr].state2--;
                    CreateSprite(sprite[ctr].x / SPRITE_DIV, sprite[ctr].y / SPRITE_DIV, SPR_WALL, (unsigned char)sprite[ctr].state1);
                }
                if (sprite[ctr].state2 == 0)
                    sprite[ctr].type = SPR_NONE;
                break;
            case SPR_WALL:
                sprite[ctr].y += timedelta * MOVE_SPEED * sprite[ctr].state1;
                sprite[ctr].state2 += ClassEMath::Abs(sprite[ctr].state1) * timedelta;
                while(sprite[ctr].state2 >= 18000)
                {
                    if (sprite[ctr].state1 < 0)
                        sprite[ctr].y += 18000 * MOVE_SPEED;
                    else
                        sprite[ctr].y -= 18000 * MOVE_SPEED;
                    sprite[ctr].state2 -= 18000;
                }
                break;
```

## Creating a level
The sprites are ready, but they aren't added to the level yet. A new function will be added to the level class which properly sets up the wall generators, depending on the level number:

_level.h (3 lines)_
```c++
        void GetAnimRect(RECT *dst, RECT *src, long frame);
        void CreateLevel(void);
        void RenderSprites(E2DSurface *buffer);
```

_level.h (15 lines)_
```c++
//Create a new level
void ClassLevel::CreateLevel(void)
{
    unsigned char ctr, num_generators;
    num_generators = 2 + (curlevel + 1) / 2;
    if (num_generators > 6)
        num_generators = 6;
    for (ctr = 0; ctr < num_generators; ctr++)
    {
        if (ctr % 2 == 0)
            CreateSprite(XRES / 2 - num_generators * DIM_SPRITE * 3 + ctr * DIM_SPRITE * 6, YRES / 2 + DIM_SPRITE * 32, SPR_WALLGEN, 0);
        else
            CreateSprite(XRES / 2 - num_generators * DIM_SPRITE * 3 + ctr * DIM_SPRITE * 6, YRES / 2 - DIM_SPRITE * 32, SPR_WALLGEN, 1);
    }
}
```

This function will be called after initialization:

_level.h (5 lines)_
```c++
        curlevel = shared->cfg.startlevel;
        CreateLevel();
        modulestate = MSLV_READY;
        msgtimer = 0;
        *modulemsg = MM_NONE;
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
* **Wall of objects**
* [Collision detection](tutorials_blastar_collision.md)
* [The boss](tutorials_blastar_boss.md)
* [Refining the game](tutorials_blastar_refining.md)
* [Adding powerups](tutorials_blastar_powerups.md)
* [Special effects](tutorials_blastar_specialfx.md)
* [Completing the game](tutorials_blastar_completing.md)

