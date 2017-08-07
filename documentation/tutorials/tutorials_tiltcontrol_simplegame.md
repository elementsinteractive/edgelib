:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Making a simple game

## Game rules
The goal of the game is to receive the highest score by trying to prevent a block from reaching the boundaries of the screen. The score will increase more if the block moves faster, which takes more challenge to keep inside the screen. The highest score will be saved in an INI file.

## Scoring
First add 2 private framework variables to keep the current and highest score and set these to 0 in the class constructor:

_tiltcontrol.cpp (8 lines)_
```c++
class ClassMain : public ClassEdge
{
    private:
        long tiltx;
        long tilty;
        unsigned long score;
        unsigned long highscore;
    public:
```

_tiltcontrol.cpp (7 lines)_
```c++
ClassMain::ClassMain(void)
{
    tiltx = 0;
    tilty = 0;
    score = 0;
    highscore = 0;
}
```

After that, load the highest score from tiltcontrol.ini and add a new OnExit() event to the framework to save the ini file.

_tiltcontrol.cpp (19 lines)_
```c++
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
    if (ecd.input->GetTiltData(&ecd) != E_OK)
    {
        SetErrorMsg("TiltCONTROL device not found or not connected.");
        return(E_ERROR);
    }
    if (ini.Open("score.ini"))
    {
        char key[MAX_ESTR], value[MAX_ESTR];
        while (ini.ReadLine(key, value))
        {
            if (ClassEStd::StrEqual(key, "highscore"))
                highscore = ClassEStd::StrToInt(value);
        }
        ini.Close();
    }
    return(E_OK);
}
```

_tiltcontrol.cpp (3 lines)_
```c++
        ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
        void OnExit(ENATIVETYPE instance);
        void OnDeviceTilt(long x, long y);
```

_tiltcontrol.cpp (12 lines)_
```c++
//Callback: Called before the application quits
void ClassMain::OnExit(ENATIVETYPE instance)
{
    ClassEIni ini;
    if (ini.New("score.ini"))
    {
        ini.WriteComment("Saves the highest score in the TiltCONTROL sample game");
        ini.WriteNewline();
        ini.WriteLine("highscore", highscore);
        ini.Close();
    }
}
```

## Game logic
Now it's time to finish the game. First add private variables to keep the position and speed of the block:

_tiltcontrol.cpp (10 lines)_
```c++
    private:
        long tiltx;
        long tilty;
        unsigned long score;
        unsigned long highscore;
        long block_x;
        long block_y;
        long block_xvel;
        long block_yvel;
    public:
```

The position will be set to the center of the screen and the speed will be set to zero in the constructor. The position is a 16:16 fixed point value, and the speed is a 24:8 fixed point value. At position of x,y (0, 0) is the center of the screen.

_tiltcontrol.cpp (11 lines)_
```c++
ClassMain::ClassMain(void)
{
    tiltx = 0;
    tilty = 0;
    score = 0;
    highscore = 0;
    block_x = 0;
    block_y = 0;
    block_xvel = 0;
    block_yvel = 0;
}
```

And render the block:

_tiltcontrol.cpp (19 lines)_
```c++
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
    RECT rc;
    long xres = display->buffer.GetWidth();
    long yres = display->buffer.GetHeight();
    long block_unit;
    if (xres < yres)
        block_unit = xres / 24;
    else
        block_unit = yres / 24;
    display->buffer.FillRect(NULL, ECOLOR_BLACK);
    display->buffer.DrawFont(0, 0, &display->fontinternal, tiltx);
    display->buffer.DrawFont(30, 0, &display->fontinternal, tilty);
    long draw_blockx = xres / 2 + (block_x >> 16) - block_unit / 2;
    long draw_blocky = yres / 2 + (block_y >> 16) - block_unit / 2;
    SetRect(&rc, draw_blockx, draw_blocky, draw_blockx + block_unit, draw_blocky + block_unit);
    display->buffer.FillRect(&rc, ECOLOR_WHITE);
    return(E_OK);
}
```

Finally, move the block depending on the position of tilt and update the score. Check if the block hits an edge of the screen, divide the score by 2 and bounce back. Add this code to OnNextFrame():

_tiltcontrol.cpp (81 lines)_
```c++
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
    RECT rc;
    long xres = display->buffer.GetWidth();
    long yres = display->buffer.GetHeight();
    long block_unit;
    if (xres < yres)
        block_unit = xres / 24;
    else
        block_unit = yres / 24;
 
    //Natural decrease of speed
    if (block_xvel > 10 * (long)timedelta)
        block_xvel -= 10 * (long)timedelta;
    else if (block_xvel > 0)
        block_xvel = 0;
    if (block_xvel < 10 * (long)timedelta)
        block_xvel += 10 * (long)timedelta;
    else if (block_xvel < 0)
        block_xvel = 0;
    if (block_yvel > 10 * (long)timedelta)
        block_yvel -= 10 * (long)timedelta;
    else if (block_yvel > 0)
        block_yvel = 0;
    if (block_yvel < 10 * (long)timedelta)
        block_yvel += 10 * (long)timedelta;
    else if (block_yvel < 0)
        block_yvel = 0;
 
    //Move block
    block_xvel += tiltx * (long)timedelta;
    block_yvel += tilty * (long)timedelta;
    block_x += block_xvel / 16 * (long)timedelta * block_unit / 16;
    block_y += block_yvel / 16 * (long)timedelta * block_unit / 16;
 
    //Check screen edges
    if ((block_x >> 16) - block_unit / 2 < -xres / 2)
    {
        block_x = (-xres / 2 + block_unit / 2) << 16;
        block_xvel = -block_xvel / 2;
        score /= 2;
    }
    if ((block_x >> 16) + block_unit / 2 > xres / 2)
    {
        block_x = (xres / 2 - block_unit / 2) << 16;
        block_xvel = -block_xvel / 2;
        score /= 2;
    }
    if ((block_y >> 16) - block_unit / 2 < -yres / 2)
    {
        block_y = (-yres / 2 + block_unit) << 16;
        block_yvel = -block_yvel / 2;
        score /= 2;
    }
    if ((block_y >> 16) + block_unit / 2 > yres / 2)
    {
        block_y = (yres / 2 - block_unit / 2) << 16;
        block_yvel = -block_yvel / 2;
        score /= 2;
    }
 
    //Handle score
    score += abs(block_xvel + block_yvel) * timedelta / 16384;
    if (score > 999999999)
        score = 999999999;
    if (score > highscore)
        highscore = score;
 
    display->buffer.FillRect(NULL, ECOLOR_BLACK);
    display->buffer.DrawFont(0, 0, &display->fontinternal, tiltx);
    display->buffer.DrawFont(30, 0, &display->fontinternal, tilty);
    display->buffer.DrawFont(0, 14, &display->fontinternal, "Score: ");
    display->buffer.DrawFont(40, 14, &display->fontinternal, score / 16);
    display->buffer.DrawFont(0, 26, &display->fontinternal, "High: ");
    display->buffer.DrawFont(40, 26, &display->fontinternal, highscore / 16);
    long draw_blockx = xres / 2 + (block_x >> 16) - block_unit / 2;
    long draw_blocky = yres / 2 + (block_y >> 16) - block_unit / 2;
    SetRect(&rc, draw_blockx, draw_blocky, draw_blockx + block_unit, draw_blocky + block_unit);
    display->buffer.FillRect(&rc, ECOLOR_WHITE);
    return(E_OK);
}
```

## Download tutorial project
Here is a [sample that can be downloaded](files/tiltcontrol_sample_source.zip) which contains the result of this tutorial.

## Chapters
* [Creating the framework](tutorials_tiltcontrol_framework.md)
* **Making a simple game**

