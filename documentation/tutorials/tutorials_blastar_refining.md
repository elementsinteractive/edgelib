:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Refining the game

## Using a virtual timer for improving gameplay
The basic gameplay elements are ready, but there are still some errors. The boss playership can be shot down even before the wall of objects has been built. We're going to solve this by speeding up the generator and wall sprites at the start of each level, and slowing it down when the game starts. To do this we're going to use a virtual timer and set it to a high speed and slowing it down.

First add private variables to the level class. An instance of the virtual timer class and a variable controlling the speed of this timer:

_level.h (10 lines)_
```c++
    private:
        SPRITE sprite[MAX_SPRITES];
        ClassEVTimer spritetimer;
        bool colmask[256];
        unsigned char lives;
        unsigned char powerup;
        unsigned char curlevel;
        unsigned long score;
        unsigned long msgtimer;
        unsigned long timerspeed;
```

These will be initialized after creating a level:

_level.h (9 lines)_
```c++
    CreateSprite(XRES - DIM_SPRITE * 8, YRES / 2, SPR_BOSS, 0);
    if (curlevel < 5)
        timerspeed = 17000;
    else if (curlevel < 9)
        timerspeed = 9000;
    else
        timerspeed = 5000;
    spritetimer.Reset(0);
    spritetimer.SetSpeed(timerspeed);
```

Now let's use this timer to control the speed of the sprites. The timedelta variable will be passed to the virtual timer, and the virtual timer will calculate an increased time difference based on its speed.

_level.h (16 lines)_
```c++
        if (timerspeed > 1000)
        {
            if (curlevel < 5)
                timerspeed -= timedelta * 16;
            else if (curlevel < 9)
                timerspeed -= timedelta * 8;
            else
                timerspeed -= timedelta * 4;
            if (timerspeed < 1000)
                timerspeed = 1000;
        }
        spritetimer.SetSpeed(timerspeed);
        spritetimer.Increase(timedelta);
        UpdateSprites(spritetimer.RetrieveDelta());
        CheckCollision();
        RenderScreen(buffer);
```

The `timerspeed` variable is being changed depending on the level. Levels with slower moving wall sprites need a higher virtual timer speed than levels with fast moving wall sprites.

## Timer troubles
After adding the virtual timer, you might notice a problem with the wall of objects. These objects don't look so good anymore, because sometimes these sprites are too close to each other. This is because when a time difference is too big, you'll lose precision and the wall sprite generator may not keep up with the high difference in time.

A way to solve this is to cut the time difference in pieces and update the sprites multiple times each frame. First let's set the maximum amount of time difference as a definition.

_level.h (9 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Other definitions                                               //
/////////////////////////////////////////////////////////////////////
 
#define MAX_TIMEDIFF   25 //Maximum amount of time difference
 
 
/////////////////////////////////////////////////////////////////////
// Structures                                                      // 
```

Then adjust the `OnNextFrame` routine:

_level.h (4 lines)_
```c++
    else if (*modulemsg == MM_NONE)
    {
        unsigned long timediff;
        if (modulestate == MSLV_READY || modulestate == MSLV_GAMEOVER)
```

_level.h (18 lines)_
```c++
        spritetimer.SetSpeed(timerspeed);
        spritetimer.Increase(timedelta);
        timediff = spritetimer.RetrieveDelta();
        while(timediff > 0)
        {
            if (timediff >= MAX_TIMEDIFF)
            {
                UpdateSprites(MAX_TIMEDIFF);
                timediff -= MAX_TIMEDIFF;
            }
            else
            {
                UpdateSprites(timediff);
                timediff = 0;
            }
        }
        CheckCollision();
        RenderScreen(buffer);
```

## Game status
The last thing to do is to show the game status on the screen, like the amount of lives and score. A new function will be added to the level class:

_level.h (3 lines)_
```c++
        void RenderSprites(E2DSurface *buffer);
        void RenderStatus(E2DSurface *buffer);
        void RenderScreen(E2DSurface *buffer);
```

_level.h (18 lines)_
```c++
//Render the player status on the screen
void ClassLevel::RenderStatus(E2DSurface *buffer)
{
    char showstr[32];
    unsigned long tmp_score;
    unsigned char ctr;
    ClassEStd::StrCpy(showstr, "Lives: ");
    ClassEStd::StrCat(showstr, lives);
    buffer->DrawFont(DIM_SPRITE / 2, DIM_SPRITE / 2, &shared->font, showstr);
    ClassEStd::StrCpy(showstr, "Score: 0000000");
    tmp_score = score;
    for (ctr = 0; ctr < 7; ctr++)
    {
        showstr[ClassEStd::StrLen(showstr) - 1 - ctr] = (unsigned char)(tmp_score % 10 + '0');
        tmp_score /= 10;
    }
    buffer->DrawFont(XRES - DIM_SPRITE / 2, DIM_SPRITE / 2, &shared->font, showstr, EFX_COLORKEY | EFO_HRIGHT);
}
```

Call this function after rendering everything on-screen in the `RenderScreen` function:

_level.h (3 lines)_
```c++
    if (modulestate == MSLV_GAMEOVER)
        buffer->DrawFont(XRES / 2 - shared->font.GetFontWidth("Game Over") / 2, YRES / 2 - shared->font.GetHeight() / 2, &shared->font, "Game Over");
    RenderStatus(buffer);
```

The score is kind of useless right now. We need to increase the score after destroying a wall sprite or boss sprite. The wall sprite will be 10 points multiplied by the current level and the boss sprite will be 1000 points.

_level.h (12 lines)_
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
        }
    }
```

_level.h (12 lines)_
```c++
    else if (sprite[nr].type == SPR_BOSS)
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
* **Refining the game**
* [Adding powerups](tutorials_blastar_powerups.md)
* [Special effects](tutorials_blastar_specialfx.md)
* [Completing the game](tutorials_blastar_completing.md)

