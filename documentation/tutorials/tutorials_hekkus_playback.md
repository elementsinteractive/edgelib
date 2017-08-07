:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Playback sound and music

## Initialize ClassESound and load Sound effects and Music
Using ClassESound is easy, only few lines of code are needed to start playing sound and music. Minimizing and other events are automatically handled by EDGELIB. Open ClassESound on program initialization. Add the OnInit() event to the framework:

_helloworld.cpp (5 lines)_
```c++
    public:
        ClassMain(void);
        ~ClassMain(void);
        ERESULT OnInit(ENATIVETYPE instance);
        ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
```

And write the implementation:

_helloworld.cpp (14 lines)_
```c++
//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
    if (ecd.snd->Open() == E_OK)
    {
        //Ready for sound playback
    }
    else
    {
        SetErrorMsg("Error opening sound output");
        return(E_ERROR);
    }
    return(E_OK);
}
```

When opening the sound system, it takes no sound parameters. Settings are picked automatically to match the target platform. An additional overloaded function is provided to change these default settings. ClassESound is closed automatically on application shutdown.

Add the following code to load music and a sound effect:

_helloworld.cpp (15 lines)_
```c++
//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
    if (ecd.snd->Open() == E_OK)
    {
        ecd.snd->LoadSoundEffect(0, "sound.wav");
        ecd.snd->LoadMusic(0, "music.mod");
    }
    else
    {
        SetErrorMsg("Error opening sound output");
        return(E_ERROR);
    }
    return(E_OK);
}
```

## Playing sound and music
First play some music after it has been loaded. Modify the OnInit() event:

_helloworld.cpp (6 lines)_
```c++
    if (ecd.snd->Open())
    {
        ecd.snd->LoadSoundEffect(0, "sound.wav");
        if (ecd.snd->LoadMusic(0, "music.mod") == E_OK)
            ecd.snd->PlayMusic(0, true);
    }
```

Then modify the OnButtonDown() event to play the sound effect when button A is pressed:

_helloworld.cpp (8 lines)_
```c++
//Callback: Called when the user pressed a key or button
void ClassMain::OnButtonDown(unsigned long bnr, EBUTTONLIST *blist)
{
    if (bnr == blist->ButtonA)
        ecd.snd->PlaySound(0);
    else
        Quit();
}
```

Finally modify OnNextFrame() to display additional information about the program:

_helloworld.cpp (8 lines)_
```c++
//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
    display->buffer.DrawFont(0, 0, &display->fontinternal, "Opened ClassESound...\nUsing Hekkus wrapper");
    display->buffer.DrawFont(0, display->fontinternal.GetHeight() * 3, &display->fontinternal, "A: Play sound effect");
    display->buffer.DrawFont(0, display->fontinternal.GetHeight() * 4, &display->fontinternal, "Any other key: Quit");
    return(E_OK);
}
```

## Download tutorial project
Here is a [sample that can be downloaded](files/esound_sample_source.zip) which contains the result of this tutorial.

## Chapters
* [How to use Hekkus within EDGELIB](tutorials_hekkus_usinghekkus.md)
* [Create a multi-platform project with Hekkus](tutorials_hekkus_project.md)
* **Playback sound and music**

