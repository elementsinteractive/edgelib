// code/camera.cpp
// One of the EDGELIB samples
// Set up a basic application framework using EDGELIB
//
// Show "hello world" on the screen and exit by clicking/tapping the
// window or hitting a key
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Include and link the library                                    //
/////////////////////////////////////////////////////////////////////

//Include EDGELIB
#include "edgemain.h"

//Link the EDGELIB static library
#pragma comment(lib, "edge.lib")

//Contains ClassMain, the application framework
#include "edge.h"
#include "main.h"

#include <stdio.h>

/////////////////////////////////////////////////////////////////////
// The program entry point                                         //
/////////////////////////////////////////////////////////////////////

ClassEdge *EdgeMain(EDGESTARTUP *data){ return(new ClassMain); }

/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////

//ClassMain: constructor
ClassMain::ClassMain(void)
{
	mycamera = NULL;
}

//ClassMain: destructor
ClassMain::~ClassMain(void)
{
    if(mycamera)
    {
        delete mycamera;
    }
}

//Configure framework
ERESULT ClassMain::OnFrameworkConfig(EFRAMEWORKCONFIG *config)
{
	config->ffwdkey         = 'A';                //A key for fast forwarding (multiplies framerate by 2.5)
    config->slomokey        = 'S';                //A key for slow motion     (halves framerate)
    config->maxfps          = 60;                 //Limit the maximum frames per second by 60
    config->flags           = 0;                  //No extra flag options
    config->numconnections  = 1;                  //Keep 1 active network connection
    #if defined(DEVICE_GP2X)
        config->smartfps    = false;              //GP2X: No smartfps needed
    #else
        config->smartfps    = true;               //Other devices: Enable smart fps lock managing
    #endif
    #if defined(DEVICE_DESKTOP)
        config->focusminimize = false;            //Desktop PC: Don't minimize the application when the focus is lost
    #else
        config->focusminimize = true;             //Mobile devices: Minimize the application when the focus is lost
    #endif
    config->jogtiltarrow    = true;               //Emulate the UIQ jog as left/right arrows
    config->focussuspend    = true;               //Desktop PC: suspend the application when the window focus is lost
    config->backlighton     = true;               //Keep backlight on
    config->netlogging      = true;               //Enable logging of network events in the console
    config->lockkeys        = true;               //Lock keys for use in this application
    config->keysounds       = false;              //Disable key 'click' sounds for smartphones
    return(E_OK);
}

//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{

    ClassEStd::StrCpy(config->caption, "Camera demo");
    config->icon            = IDI_MAIN;           //Application icon
    config->menu            = 0;                  //Application contains no Windows Menu
    config->dialogbox       = 0;                  //Don't start from dialogbox (Desktop only)
    config->width           = 320;                //Desktop PC: Custom display width (landscape mode)
    config->height          = 240;                //Desktop PC: Custom display height
    config->videotype       = EDSP_DEFAULT;       //Choose the default display mode
    config->emulvideotype   = EDSP_DEFAULT;       //No color depth emulation
    config->orientation     = DOR_NONE;           //Change orientation to match the settings of the device
    config->openmethod      = DOM_DEFAULT;        //Take recommended (fastest) display open method
    config->surfacedefault  = EST_SYSMEMORY;      //Create surfaces in system memory by default, don't use DirectDraw or Symbian bitmap surfaces
    #if defined(DEVICE_DESKTOP) && defined(DEVICE_WINDOWS) // bug in edge - Maemo sets device_desktop, this fix might break linux desktops for now
        config->fullscreen  = false;              //Windowed application for Desktop
        config->fullscreenmouse = true;           //Desktop PC: Enable the mouse cursor for fullscreen applications
    #else
        config->fullscreen  = true;               //Fullscreen application for mobile devices
    #endif
    config->dspnative       = true;               //If possible, try to use native blitters (like DirectDraw)
    config->enginestatus    = true;               //Desktop PC: Enable the status bar and status information icon
    config->engineconsole   = true;               //Desktop PC: Enable the edge console
    config->surfprerotate   = true;               //Use pre-rotated surfaces to increase speed in other orientations
    return(E_OK);
}

//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
    menu = false;
    menuindex = 0;
    menumax = 0;

    mycamera = new ClassECamera(this);

    error = false;
    
    return(E_OK);
}

//Callback: Called when the program minimizes
ERESULT ClassMain::OnMinimize(void)
{
    //mycamera->RequestStop();

    return(E_OK);
}

//Callback: Called when the program restores back from a minimized state
ERESULT ClassMain::OnRestore(void)
{
    //mycamera->RequestStart();
    return(E_OK);
}

//Callback: Called when the display mode has changed, use this to initialize your surfaces, etc.
ERESULT ClassMain::OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)
{
    winheight = display->buffer.GetHeight();
    display->CreateSurface(&cambuffer, display->buffer.GetWidth(), display->buffer.GetHeight());
	
    Log("Camera count: ", false); Log(mycamera->GetCameraCount());
	
    ERESULT retval = mycamera->RequestCamera(0);
    if (retval != E_OK)
    {
        error = true;
        errormsg = "request fail";
    }

    return(E_OK);
}

//Callback: Called when the display has been changed (because of a change in orientation)
void ClassMain::OnDisplayChange(ClassEDisplay *display)
{
    winheight = display->buffer.GetHeight();
    display->CreateSurface(&cambuffer, display->buffer.GetWidth(), display->buffer.GetHeight());
}

//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{

    long scrx = display->buffer.GetWidth();
    long scry = display->buffer.GetHeight();

    // copy the camera image to the screen
    RECT rc_dst;
    rc_dst.left = 0;
    rc_dst.right = scrx;
    rc_dst.top = 0;
    rc_dst.bottom = scry;
	display->BltFast(0, 0, &cambuffer, &rc_dst);    

    // render overlay
    if (error)
    {
        display->buffer.DrawFont(2, scry / 2, &display->fontinternal, errormsg);
    }
    else if (menu)
    {

        for (int i = -2; i <= 2; i++)
        {
            int rendery = scry / 2 + 10 * i;
            if (i < 0) rendery -= 5;
            if (i > 0) rendery += 5;

            int curindex = i + menuindex;

            const char * msg = "";
            char buffer[256];

            if (curindex == 0)
                msg = "Quit";
            else if (curindex == 1)
                msg = "Select rear camera";
            else if (curindex == 2)
                msg = "Select front camera";
            else if (curindex == 3)
                msg = "Information";
            else if (curindex == 4)
                msg = "Cancel";
            else if (curindex > 4 && curindex <= menumax)
            {
                E_CAMERAMODE mode;
                mycamera->GetModeInformation(curindex - 5, mode);
                sprintf(buffer, "Set %i x %i %s @ %iHz", (int) mode.x, (int) mode.y, FormatToString(mode.format), mode.expectedfps >> 16);
                msg = buffer;
            }

            display->buffer.DrawFont(2, rendery, &display->fontinternal, msg);

        }

    }
	

    // accept new frames
    discard = false;

    //display->buffer.DrawFont(2, 2, &display->fontinternal, "Hello World!");
    //display->buffer.DrawFont(2, display->buffer.GetHeight() - 2, &display->fontinternal, "Button/Stylus: Quit", EFX_COLORKEY | EFO_VBOTTOM);
    return(E_OK);
}

//Callback: Called when the program exits
void ClassMain::OnExit(ENATIVETYPE instance)
{
    if (mycamera)
    {
        mycamera->RequestStop();
        delete mycamera;
        mycamera = NULL;
    }
}

//Callback: Called when the user pressed a key or button
void ClassMain::OnButtonDown(unsigned long bnr, EBUTTONLIST *blist)
{
    if (error)
        error = false;
    else
    {
        if( ecd.input->MappedTo(DIAM_MENUUP, bnr))
        {
            if (menuindex > 0) menuindex--;
        }
        else if( ecd.input->MappedTo(DIAM_MENUDOWN, bnr))
        {
            if (menuindex < menumax) menuindex++;
        }
        else if( ecd.input->MappedTo(DIAM_MENUCONFIRM, bnr))
        {
            if (menu)
            {
                MenuAction(menuindex);
            }
            else
                menu = true;
        }
        else if( ecd.input->MappedTo(DIAM_MENULEFT, bnr))
            {} // ignore
        else if( ecd.input->MappedTo(DIAM_MENURIGHT, bnr))
            {} // ignore
        else if( ecd.input->MappedTo(DIAM_MENUCANCEL, bnr))
            menu = false;
        else
        {
            Quit();
        }
    }
}

//Callback: Called when the user released a key or button
void ClassMain::OnButtonUp(unsigned long bnr, EBUTTONLIST *blist)
{
}

//Callback: Called when the user scrolls a wheel device
void ClassMain::OnWheel(long direction)
{
}

//Callback: Called when the user released the stylus or mouse button
void ClassMain::OnStylusUp(POINT pnt)
{
}

//Callback: Called when the user points the stylus down or clicks the left mouse button
void ClassMain::OnStylusDown(POINT pnt)
{
    if (error) 
    {
        error = false;
        return;
    }

    if (!menu)
    {
        menu = true;
        return;
    }

    if (pnt.y < winheight / 3)
    {
        if (menuindex > 0) menuindex--;
    }
    else if (pnt.y > (2 * winheight) / 3)
    {
        if (menuindex < menumax) menuindex++;
    }
    else
    {
        MenuAction(menuindex);
    }


    //menu = false;
}

void ClassMain::MenuAction(int index)
{
    if (index == 0) Quit();
    if (index == 1) mycamera->RequestCamera(ELOC_REAR);
    if (index == 2) mycamera->RequestCamera(ELOC_FRONT);
    if (index == 3)
    {
        error = true;
        errormsg = errbuffer;
        E_CAMERAMODE mode;
        mycamera->GetCurrentMode(mode);
        sprintf(errbuffer, "Camera: %i, orientation %i, state %i; %ix%i", 
            mycamera->GetCameraIndex(), 
            mycamera->GetCameraOrientation(), 
            mycamera->GetCameraState(), 
            mode.x,
            mode.y);
    }
    if (index > 4)
    {
        mycamera->RequestResolution(index - 5);
        //mycamera->RequestStart();
    }

    menu = false;
}

//Callback: Called when the user points the stylus 2 times down quickly or double clicks the left mouse button
void ClassMain::OnStylusDouble(POINT pnt)
{
}

//Callback: Called when the user moves the stylus while pressed down, or moves the mouse cursor
void ClassMain::OnStylusMove(POINT pnt)
{
}

void ClassMain::OnCameraChange(ClassECamera * camera, ERESULT camstatus)
{

    if (camstatus != E_OK)
    {
        error = true;
        errormsg = "A request failed";
		Log("Camera error code: ", false); Log(camstatus);
    }

    menumax = camera->GetModeCount() + 4;
    menu = false;
    menuindex = 4;

    camera->RequestStart();
}

void ClassMain::OnCameraFrame(ClassECamera * camera, const E_CAMERAFRAME * frame)
{

    // Control time spent, discard data that's not needed
    if (discard) return;

    // Use the built-in decoder to convert the camera output to screen format
    ClassECamera::FrameToSurfaceAspect(frame, &cambuffer);

    // If there are other frames queued, ignore them.
    discard = true;

}


// Takes an EDSP_* and returns an user-friendly string format for display.
const char * ClassMain::FormatToString(int format)
{
    switch(format)
    {
        case EDSP_TRUE16:
            return "R5G6B5";
		case EDSP_TRUE24:
			return "R8G8B8";
        case EDSP_TRUE32:
            return "B8G8R8A8";
        case EDSP_TRUE32_INV:
            return "R8G8B8A8";
        case EDSP_YUV420:
            return "YUVi 4:2:0";
        case EDSP_YUV12:
            return "YUVp 4:2:0";
        case EDSP_YVU12:
            return "YVUp 4:2:0";
        case EDSP_UYVY:
            return "UYVY";
        case EDSP_YUYV:
            return "YUYV";
        case EDSP_YVYU:
            return "YVYU";
        default:
            return "unknown";

    }
}
