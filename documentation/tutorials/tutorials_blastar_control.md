:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Player control

## Sprite animation
At this moment, the sprites aren't able to move yet. To enable movement, we need to add a function containing sprite movement that will be updated every frame. The current input state will be needed to move the player's spaceship. Add this function to `ClassLevel`:

_level.h (3 lines)_
```c++
        unsigned long CreateSprite(long x, long y, unsigned char type, unsigned char property);
        void UpdateSprites(unsigned long timedelta);
        void GetAnimRect(RECT *dst, RECT *src, long frame);
```

_level.h (21 lines)_
```c++
void ClassLevel::UpdateSprites(unsigned long timedelta)
{
    unsigned long ctr;
    for (ctr = 0; ctr < MAX_SPRITES; ctr++)
    {
        sprite[ctr].anitimer += timedelta;
        switch(sprite[ctr].type)
        {
            case SPR_PLAYER:
                if (shared->ecd->input->KeyHold(DIAM_MENUUP))
                {
                    sprite[ctr].y -= timedelta * MOVE_SPEED;
                }
                else if (shared->ecd->input->KeyHold(DIAM_MENUDOWN))
                {
                    sprite[ctr].y += timedelta * MOVE_SPEED;
                }
                break;
        }
    }
}
```

Call this function inside the frameloop, `OnNextFrame`:

_level.h (2 lines)_
```c++
        UpdateSprites(timedelta);
        RenderScreen(buffer);
```

This will enable simple movement for the spaceship.

## Advanced spaceship control
The result of the previous spaceship movement is hardly enough. The ship needs to move faster, have a tilting animation when moving and needs to react on the stylus and scrollwheel. The sprite has a number of state variables to store properties. Property `state1` will be needed to store the amount of tilting the ship has (which can be negative and positive), `state2` will be used to store the amount of movement needed after scrolling a wheel device and `state3` will be used as a bullet timer. Property `state4` isn't used at this moment.

First improve the control of the ship, and add styluscontrol. The controls will only work when in idle state:

_level.h (60 lines)_
```c++
            case SPR_PLAYER:
                if (modulestate == MSLV_IDLE)
                {
                    long tomove = 0, wheeldir = shared->ecd->input->GetWheelScroll();
                    if (wheeldir < 0)
                        sprite[ctr].state2 = timedelta * 10;
                    else if (wheeldir > 0)
                        sprite[ctr].state2 = timedelta * -10;
                    if (shared->ecd->input->KeyHold(DIAM_MENUUP))
                        tomove = timedelta * MOVE_SPEED * -6;
                    else if (shared->ecd->input->KeyHold(DIAM_MENUDOWN))
                        tomove = timedelta * MOVE_SPEED * 6;
                    else if (shared->ecd->input->ClickHold())
                    {
                        long clickx, clicky;
                        shared->ecd->input->GetClickPosition(clickx, clicky);
                        if (clickx < DIM_SPRITE * 12)
                        {
                            long sprite_y = sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2;
                            if (clicky < sprite_y)
                            {
                                tomove = timedelta * MOVE_SPEED * -6;
                                if (sprite_y + tomove / SPRITE_DIV < clicky)
                                    tomove = (clicky - sprite_y) * SPRITE_DIV;
                            }
                            else
                            {
                                tomove = timedelta * MOVE_SPEED * 6;
                                if (sprite_y + tomove / SPRITE_DIV > clicky)
                                    tomove = (clicky - sprite_y) * SPRITE_DIV;
                            }
                        }
                    }
                    if (sprite[ctr].state2 != 0)
                    {
                        if (ClassEMath::Abs(sprite[ctr].state2) < (long)timedelta)
                        {
                            if (tomove == 0)
                                tomove = sprite[ctr].state2;
                            sprite[ctr].state2 = 0;
                        }
                        else
                        {
                            if (sprite[ctr].state2 < 0)
                            {
                                if (tomove == 0)
                                    tomove = timedelta * MOVE_SPEED * -6;
                                sprite[ctr].state2 += timedelta;
                            }
                            else
                            {
                                if (tomove == 0)
                                    tomove = timedelta * MOVE_SPEED * 6;
                                sprite[ctr].state2 -= timedelta;
                            }
                        }
                    }
                    sprite[ctr].y += tomove;
                }
                break;
```

The movement is temporary stored in a variable. This will make sure the ship doesn't have double speed when using the scrollwheel and directional pad at the same time. Also, it will be used to program the tilting of the ship only at one place:

_level.h (20 lines)_
```c++
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
```

And finally, let's make sure the ship isn't able to move outside the screen:

_level.h (6 lines)_
```c++
                    if (sprite[ctr].y / SPRITE_DIV < (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / -2)
                        sprite[ctr].y = (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / -2 * SPRITE_DIV;
                    if (sprite[ctr].y / SPRITE_DIV > YRES - (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2)
                        sprite[ctr].y = (YRES - (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2) * SPRITE_DIV;
                }
                break;
```

## Shooting
The ship only needs to be able to shoot now. For this we need to add a bullet sprite to the definition list and link the right graphic picture on creation:

_level.h (3 lines)_
```c++
#define SPR_NONE        0
#define SPR_PLAYER      1
#define SPR_BULLET      2 
```

_level.h (7 lines)_
```c++
            case SPR_PLAYER:
                SetRect(&sprite[found].rc, 12, 0, 18, 6);
                sprite[found].state3 = 500;
                break;
            case SPR_BULLET:
                SetRect(&sprite[found].rc, 30, 0, 32, 1);
                break;
```

After adding the bullet sprite, the player ship movement will be expanded to add shooting:

_level.h (13 lines)_
```c++
                    if (shared->ecd->input->KeyHold(DIAM_MENUCONFIRM_SP) || shared->cfg.autoshot)
                    {
                        sprite[ctr].state3 += timedelta;
                        while (sprite[ctr].state3 >= 500)
                        {
                            CreateSprite(sprite[ctr].x / SPRITE_DIV + DIM_SPRITE * 6, sprite[ctr].y / SPRITE_DIV + (sprite[ctr].rc.bottom - sprite[ctr].rc.top) / 2, SPR_BULLET, 0);
                            sprite[ctr].state3 -= 500;
                        }
                    }
                    else if (sprite[ctr].state3 < 500)
                        sprite[ctr].state3 += timedelta;
                }
                break;
```

The bullets will move to the right with a fixed speed. They will disappear after moving past the right edge of the screen. Expand the `UpdateSprites()` function:

_level.h (6 lines)_
```c++
                break;
            case SPR_BULLET:
                sprite[ctr].x += timedelta * MOVE_SPEED * 12;
                if (sprite[ctr].x / SPRITE_DIV >= XRES)
                    sprite[ctr].type = SPR_NONE;
                break;
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
* **Player control**
* [Wall of objects](tutorials_blastar_objects.md)
* [Collision detection](tutorials_blastar_collision.md)
* [The boss](tutorials_blastar_boss.md)
* [Refining the game](tutorials_blastar_refining.md)
* [Adding powerups](tutorials_blastar_powerups.md)
* [Special effects](tutorials_blastar_specialfx.md)
* [Completing the game](tutorials_blastar_completing.md)

