// cmain.cpp
// Entry point
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Use Windows Desktop OpenGL by default
#if !defined(__EDGEIDE__)
	//#define EGL_PC
#endif

/////////////////////////////////////////////////////////////////////
// Include and link the library                                    //
/////////////////////////////////////////////////////////////////////

//Use a GL library if an implementation has been defined in the workspace
#if defined(EGL_PC) || defined(EGL_SYMBIAN) || defined(EGL_RASTEROID) || defined(EGL_POWERVR) || defined(EGL_NVIDIA)
	#define EGL_USEGL
	#if !defined(EGL_PC)
		#define EGL_USEGLES
	#endif
#endif

#define EGL_USEGL
#define EDGERENDER_LITE

//Include Edge
#include "edgemain.h"

//Link the Edge static library
#pragma comment(lib, "edge.lib")

//Include headers and link OpenGL libraries
#if defined(EGL_USEGL)
	#if defined(EGL_USEGLES)
		#include "GLES\\gl.h"
		#include "GLES\\egl.h"
	#elif defined(EGL_USEGLES2)
		#include <OpenGLES/ES2/gl.h>
	#else
		#include <OpenGL/gl.h>
	#endif
	#if defined(DEVICE_WIN32)
		#if defined(EGL_PC)
			#pragma comment(lib, "plugingl.lib")
		#else
			#if defined(DEVICE_DESKTOP)
				#if defined(EGL_POWERVR)
					#pragma comment(lib, "pluginpowervr.lib")
				#elif defined(EGL_RASTEROID)
					#pragma comment(lib, "pluginrasteroid.lib")
				#endif
			#else
				#pragma comment(lib, "plugin1-0.lib")
			#endif
		#endif
		#if defined(EGL_PC)
			#pragma comment(lib, "opengl32.lib")
		#elif defined(EGL_POWERVR)
			#pragma comment(lib, "libgles_cl.lib")
		#elif defined(EGL_NVIDIA)
			#pragma comment(lib, "libgles_cm.lib")
		#elif defined(EGL_RASTEROID)
			#pragma comment(lib, "libEGL.lib")
			#pragma comment(lib, "libGLES_CM_NoE.lib")
		#endif
	#endif
#endif

//Contains ClassMain, the application framework
#include "main.h"
void EdgeGL_RenderModel(void *driver, void **arrays, long *strides, unsigned long count, bool wireframe, bool strip);
int edgerender_getlibid()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////
// The program entry point                                         //
/////////////////////////////////////////////////////////////////////

ClassEdge *EdgeMain(EDGESTARTUP *data){ return(new ClassMain); }


/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////

static POINT lastpoint;

//ClassMain: constructor
ClassMain::ClassMain(void)
{
	fprintf(stderr, "Hello world\n");
}

//ClassMain: destructor
ClassMain::~ClassMain(void)
{
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
	
	fprintf(stderr, "test\n");
	
	return(E_OK);
}

//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
	ClassEStd::StrCpy(config->caption, "Hello World!");
#if defined(IDI_ICON1)
	config->icon            = IDI_ICON1;           //Application icon
#endif
	config->menu            = 0;                  //Application contains no Windows Menu
	config->dialogbox       = 0;                  //Don't start from dialogbox (Desktop only)
	config->width           = 1024;                //Desktop PC: Custom display width
	config->height          = 768;                //Desktop PC: Custom display height
	config->videotype       = EDSP_DEFAULT;       //Choose the default display mode
	config->emulvideotype   = EDSP_DEFAULT;       //No color depth emulation
	config->orientation     = DOR_AUTO;           //Change orientation to match the settings of the device
	config->openmethod      = DOM_DEFAULT;        //Take recommended (fastest) display open method
	config->surfacedefault  = EST_SYSMEMORY;      //Create surfaces in system memory by default, don't use DirectDraw or Symbian bitmap surfaces
#if defined(DEVICE_DESKTOP)
	config->fullscreen  = true;              //Windowed application for Desktop
	config->fullscreenmouse = true;           //Desktop PC: Enable the mouse cursor for fullscreen applications
#else
	config->fullscreen  = true;               //Fullscreen application for mobile devices
#endif
	config->dspnative       = true;               //If possible, try to use native blitters (like DirectDraw)
	config->enginestatus    = true;               //Desktop PC: Enable the status bar and status information icon
	config->engineconsole   = false;              //Desktop PC: Enable the edge console
	config->surfprerotate   = true;               //Use pre-rotated surfaces to increase speed in other orientations
	config->stencil			= false;				  //Try to get a stencil buffer
	config->emulatelowres   = 0;                  //The device is not to emulate an older device
	config->supportedorientations = 0xF;//B;
	return(E_OK);
}

//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
	return(E_OK);
}

//Callback: Called when the program minimizes
ERESULT ClassMain::OnMinimize(void)
{
	return(E_OK);
}

//Callback: Called when the program restores back from a minimized state
ERESULT ClassMain::OnRestore(void)
{
	return(E_OK);
}

//Callback: Called when the display mode has changed, use this to initialize your surfaces, etc.
ERESULT ClassMain::OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)
{
	/*ClassEFile fd;
	fd.Open("broken.dat", EFOF_READ | EFOF_READONLYPATH);
	int size = fd.Size((char *) NULL);
	char * data = (char*)malloc(size);
	fd.Read(data, size);
	
	ERESULT tmp = display->CreateSurface(&testimage, data, size, EST_READONLY);
	//ERESULT tmp = display->CreateSurface(&testimage, "broken.png", EST_READONLYPATH);
	Log(tmp);
	return(tmp);*/
	return(E_OK);
}

//Callback: Called when the display has been changed (because of a change in orientation)
void ClassMain::OnDisplayChange(ClassEDisplay *display)
{
}

//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
	
	static long passtime = 0;
	static unsigned char orient = 0;
	passtime += timedelta;
	if (passtime > 4000)
	{
		passtime = 0;
		orient = (orient + 1) % 4;
		//display->ChangeOrientation(orient);
	}
	
#ifdef EGL_USEGL
    glClearColor(1.5f, 0.5f, 0.5f, 1.0f);
	glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	
	glBegin(GL_TRIANGLES);
		glColor3f(1, 1, 0);
		glVertex3f(1, -1, 0.5);
		glColor3f(1, 0, 1);
		glVertex3f(-1, -1, 0.5);
		glColor3f(0, 1, 1);
		glVertex3f(0, 1, 0.5);
	glEnd();
	 
#endif

	display->buffer.SetDirty();
	display->WorldReset();	
	display->Perspective2D(display->GetWidth(), display->GetHeight(), false);
	display->SetShading(E3D_GOURAUD); //E3D_TEXTURE);	
	
	display->buffer.DrawFont(2, 2, &display->fontinternal, "Hello World!");
	
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);
	glBegin(GL_TRIANGLES);
		glColor3f(1, 1, 0);
		glVertex3f(0, 500, 0);
		glColor3f(1, 0, 1);
		glVertex3f(0, 0, 0);
		glColor3f(0, 1, 1);
		glVertex3f(500, 250, 0);
	glEnd();
	
	GLint vtx[] = {500, 0, 0, 500, 500, 0, 0, 250, 0};
	void * arrays[] = {&(vtx[0]), NULL, NULL, NULL, NULL };
	long strides[] = {12, 0, 0, 0, 0};
	EdgeGL_RenderModel(NULL, arrays, strides, 3, false, true);
	
	display->DrawLine(0,0, 500, 500, 0xffffffff);
	display->FillOval(50, 50, 50, 50, 0x7fff0000);
	display->DrawOval(50, 50, 25, 25, 0xffffffff);
		
	//display->buffer.BltFast(0, 0, &testimage, NULL, 0, 0);
	
	display->buffer.DrawFont(2, 2, &display->fontinternal, "Hello World!");
	display->SetShading(E3D_GOURAUD); //E3D_TEXTURE);
	
	display->buffer.DrawFont(2, 11, &display->fontinternal, display->GetStencilBits());
	display->buffer.DrawFont(2, 20, &display->fontinternal, lastpoint.x);
	display->buffer.DrawFont(2, 29, &display->fontinternal, lastpoint.y);
	display->buffer.DrawFont(2, display->GetHeight() - 2, &display->fontinternal, "Button/Stylus: Quit", EFX_COLORKEY | EFO_VBOTTOM);
	char name[128];
	ecd.device->GetModelName(name);
	display->buffer.DrawFont(20, 20, &display->fontinternal, name);
	
	display->SetShading(E3D_TEXTURE | EFX_COLORIZE, 0xFF0044FF);
	
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = display->fontinternal.GetWidth();
	rc.bottom = display->fontinternal.GetHeight();
	E2DBLTFX fx;
	fx.xscale = 2 * 65536;
	fx.yscale = 2 * 65536;
	fx.rotation = (passtime * 4096) / 4000;
	fx.flags = EFX_COLORKEY | EFX_COLORIZE;
	fx.property = 0xFF0044FF;
	
	glEnable(GL_STENCIL_TEST);	
	
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xffffffff);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	display->BltFx(display->GetWidth() / 2, display->GetHeight() / 2, &display->fontinternal, &rc, &fx);

	glStencilFunc(GL_NOTEQUAL, 0, 0xffffffff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	fx.rotation *= -1;
	display->BltFx(display->GetWidth() / 2, display->GetHeight() / 2, &display->fontinternal, &rc, &fx);
	
	glDisable(GL_STENCIL_TEST);
	
#ifdef EGL_USEGL
	int error = glGetError();
	if (error)
	{
		display->WorldReset();
	}
#endif
	 /*/
	RECT rc;
	rc.top = 0;
	rc.left = 0;
	rc.right = display->fontinternal.GetWidth();
	rc.bottom = display->fontinternal.GetHeight();
	display->buffer.BltFast(0, 0, &display->fontinternal, &rc, EFX_COLORKEY, 0);
	*/
	return(E_OK);
}

//Callback: Called when the program exits
void ClassMain::OnExit(ENATIVETYPE instance)
{
}

//Callback: Called when the user pressed a key or button
void ClassMain::OnButtonDown(unsigned long bnr, EBUTTONLIST *blist)
{
	Quit();
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
	lastpoint = pnt;
}

//Callback: Called when the user points the stylus down or clicks the left mouse button
void ClassMain::OnStylusDown(POINT pnt)
{
	//Quit();
	
}

//Callback: Called when the user points the stylus 2 times down quickly or double clicks the left mouse button
void ClassMain::OnStylusDouble(POINT pnt)
{
}

//Callback: Called when the user moves the stylus while pressed down, or moves the mouse cursor
void ClassMain::OnStylusMove(POINT pnt)
{
}

//Callback: Called when a network event occurs
void ClassMain::OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer)
{
}
