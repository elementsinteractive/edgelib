// code/main.h
// Contains the main wrapper class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Include internal resources for native Win32 builds
#if defined(DEVICE_WIN32)
#include "resource.h"
#else
#define IDI_MAIN 0
#endif

//Set render types
const unsigned long rendertype_shading[] = {
	E3D_WIREFRAME,
	E3D_NONE,
	E3D_FLAT,
	E3D_GOURAUD,
	E3D_TEXTURE
};

/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

//The main class
class ClassMain : public ClassEdge
{
	private:
		E2DSurface texture;
		E3DSurface cube;
		E3DSurface edgelogo;
		long xr, yr, zr, bufferwidth, bufferheight, rendertype;
		long clickx, clicky;
		
		void ModelInteract(unsigned long timedelta);
	public:
		ClassMain(void);
		~ClassMain(void);
		ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config);
		ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
		ERESULT OnInit(ENATIVETYPE instance);
		ERESULT OnMinimize(void);
		ERESULT OnRestore(void);
		ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display);
		ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
		void OnExit(ENATIVETYPE instance);
		void OnButtonDown(unsigned long bnr, EBUTTONLIST *blist);
};


/////////////////////////////////////////////////////////////////////
// ClassMain: private                                              //
/////////////////////////////////////////////////////////////////////

//Let the user interact with the models using the d-pad or change the rendertype with the software buttons
void ClassMain::ModelInteract(unsigned long timedelta)
{
	//Mouse/Stylus control
	long newclickx, newclicky;
	ecd.input->GetClickPosition(newclickx, newclicky);
	if (newclickx != clickx || newclicky != clicky)
	{
		xr = (clickx - bufferwidth / 2) * 16;
		yr = (clicky - bufferheight / 2) * 16;
		zr = 0;
		clickx = newclickx;
		clicky = newclicky;
	}

	//Arrow key control
	if (ecd.input->KeyHold(DIAM_MENUUP))
		yr -= timedelta;
	else if (ecd.input->KeyHold(DIAM_MENUDOWN))
		yr += timedelta;
	if (ecd.input->KeyHold(DIAM_MENULEFT))
		xr -= timedelta;
	else if (ecd.input->KeyHold(DIAM_MENURIGHT))
		xr += timedelta;

	//Change render method through the soft keys
	E_INPUTEVENT ievent;
	while(ecd.input->GetKeyEvent(&ievent))
	{
		if (ievent.type == EIE_PRESSED)
		{
			if (ievent.action == DIAM_MENUSOFT1)
			{
				rendertype++;
				if (rendertype > 4)
					rendertype = 0;
			}
			else if (ievent.action == DIAM_MENUSOFT2)
			{
				rendertype--;
				if (rendertype < 0)
					rendertype = 4;
			}
		}
	}

	//Change render method through scrollwheel
	long wheeldirection = ecd.input->GetWheelScroll();
	if (wheeldirection > 0)
	{
		rendertype++;
		if (rendertype > 4)
			rendertype = 0;
	}
	else if (wheeldirection < 0)
	{
		rendertype--;
		if (rendertype < 0)
			rendertype = 4;
	}
}


/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////

//ClassMain: constructor
ClassMain::ClassMain(void)
{
}

//ClassMain: destructor
ClassMain::~ClassMain(void)
{
}

//Configure framework
ERESULT ClassMain::OnFrameworkConfig(EFRAMEWORKCONFIG *config)
{
	config->maxfps          = 60;                 //Limit the maximum frames per second by 60
	config->flags           = 0;                  //No extra flag options
	config->smartfps        = true;               //Enable smart fps lock managing
	return(E_OK);
}

//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
	ClassEStd::StrCpy(config->caption, "View 3D");
	config->icon            = IDI_MAIN;           //Application icon
	config->width           = 640;                //Desktop PC: Custom display width
	config->height          = 480;                //Desktop PC: Custom display height
	#if defined(DEVICE_DESKTOP) || defined(DEVICE_SERIES80) || defined(DEVICE_SERIES90) || defined(DEVICE_GP2X)
		config->orientation = DOR_NONE;           //No display rotation for Desktop, Series 80, Series 90 and GP2X devices
	#else
		config->orientation = DOR_90DEG;          //Other devices: Rotate the display 90 degrees
	#endif
	config->engineconsole   = false;              //Desktop PC: Disable the edge console
	#if defined(DESKTOP) && !defined(EGL_PC)
		config->fullscreen      = false;          //Desktop PC: windowed application for OpenGL ES emulator library
	#else
		config->fullscreen      = true;           //Fullscreen application for other cases
	#endif
	config->fullscreenmouse = true;               //Desktop PC: Enable the mouse cursor for fullscreen applications
	return(E_OK);
}

//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
	rendertype = 2;
	xr = 0;
	yr = 0;
	zr = 0;
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
	//Enable blending for OpenGL (ES)
	#if defined(EGL_USEGL)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	#endif

	//Get the mouse/stylus position on startup
	ecd.input->GetClickPosition(clickx, clicky);

	//Retrieve and save the size of the display
	bufferwidth  = display->buffer.GetWidth();
	bufferheight = display->buffer.GetHeight();

	//Load the texture
	//(important: use texture sizes like 32x32, 64x64, 128x128, etc. pixels for optimal performance)
	if (display->CreateSurface(&texture, "texture.png", EST_READONLYPATH) != E_OK)
		return(E_ERROR);
	display->UploadTexture(&texture);

	//Create a cube manually using 24 vertices and 12 polygons with coloring, texture coordinates and normals for lighting
	if (display->CreateSurface(&cube, 24, 12) == E_OK)
	{
		//front
		cube.SetPolygon(0, 0, 2, 1);
		cube.SetPolygon(1, 1, 2, 3);
		cube.SetVertex(0, -2048,  2048, 2048); cube.SetColor(0, E_RGBA(255, 0, 0, 255)); cube.SetNormal(0, 0, 0, 127); cube.SetTexCoord(0, 0, 0);
		cube.SetVertex(1,  2048,  2048, 2048); cube.SetColor(1, E_RGBA(255, 0, 0, 255)); cube.SetNormal(1, 0, 0, 127); cube.SetTexCoord(1, 65536, 0);
		cube.SetVertex(2, -2048, -2048, 2048); cube.SetColor(2, E_RGBA(255, 0, 0, 255)); cube.SetNormal(2, 0, 0, 127); cube.SetTexCoord(2, 0, 65536);
		cube.SetVertex(3,  2048, -2048, 2048); cube.SetColor(3, E_RGBA(255, 0, 0, 255)); cube.SetNormal(3, 0, 0, 127); cube.SetTexCoord(3, 65536, 65536);

		//back
		cube.SetPolygon(2, 4, 5, 6);
		cube.SetPolygon(3, 5, 7, 6);
		cube.SetVertex(4, -2048,  2048, -2048); cube.SetColor(4, E_RGBA(255, 128, 0, 255)); cube.SetNormal(4, 0, 0, -127); cube.SetTexCoord(4, 0, 0);
		cube.SetVertex(5,  2048,  2048, -2048); cube.SetColor(5, E_RGBA(255, 128, 0, 255)); cube.SetNormal(5, 0, 0, -127); cube.SetTexCoord(5, 65536, 0);
		cube.SetVertex(6, -2048, -2048, -2048); cube.SetColor(6, E_RGBA(255, 128, 0, 255)); cube.SetNormal(6, 0, 0, -127); cube.SetTexCoord(6, 0, 65536);
		cube.SetVertex(7,  2048, -2048, -2048); cube.SetColor(7, E_RGBA(255, 128, 0, 255)); cube.SetNormal(7, 0, 0, -127); cube.SetTexCoord(7, 65536, 65536);

		//left
		cube.SetPolygon(4, 8, 10, 9);
		cube.SetPolygon(5, 9, 10, 11);
		cube.SetVertex(8,  -2048,  2048, -2048); cube.SetColor(8,  E_RGBA(255, 255, 0, 255)); cube.SetNormal(8,  -127, 0, 0); cube.SetTexCoord(8,  0, 0);
		cube.SetVertex(9,  -2048,  2048,  2048); cube.SetColor(9,  E_RGBA(255, 255, 0, 255)); cube.SetNormal(9,  -127, 0, 0); cube.SetTexCoord(9,  65536, 0);
		cube.SetVertex(10, -2048, -2048, -2048); cube.SetColor(10, E_RGBA(255, 255, 0, 255)); cube.SetNormal(10, -127, 0, 0); cube.SetTexCoord(10, 0, 65536);
		cube.SetVertex(11, -2048, -2048,  2048); cube.SetColor(11, E_RGBA(255, 255, 0, 255)); cube.SetNormal(11, -127, 0, 0); cube.SetTexCoord(11, 65536, 65536);

		//right
		cube.SetPolygon(6, 12, 13, 14);
		cube.SetPolygon(7, 13, 15, 14);
		cube.SetVertex(12, 2048,  2048, -2048); cube.SetColor(12, E_RGBA(0, 255, 0, 255)); cube.SetNormal(12, 127, 0, 0); cube.SetTexCoord(12, 0, 0);
		cube.SetVertex(13, 2048,  2048,  2048); cube.SetColor(13, E_RGBA(0, 255, 0, 255)); cube.SetNormal(13, 127, 0, 0); cube.SetTexCoord(13, 65536, 0);
		cube.SetVertex(14, 2048, -2048, -2048); cube.SetColor(14, E_RGBA(0, 255, 0, 255)); cube.SetNormal(14, 127, 0, 0); cube.SetTexCoord(14, 0, 65536);
		cube.SetVertex(15, 2048, -2048,  2048); cube.SetColor(15, E_RGBA(0, 255, 0, 255)); cube.SetNormal(15, 127, 0, 0); cube.SetTexCoord(15, 65536, 65536);

		//top
		cube.SetPolygon(8, 16, 18, 17);
		cube.SetPolygon(9, 17, 18, 19);
		cube.SetVertex(16, -2048, 2048, -2048); cube.SetColor(16, E_RGBA(0, 255, 255, 255)); cube.SetNormal(16, 0, 127, 0); cube.SetTexCoord(16, 0, 0);
		cube.SetVertex(17,  2048, 2048, -2048); cube.SetColor(17, E_RGBA(0, 255, 255, 255)); cube.SetNormal(17, 0, 127, 0); cube.SetTexCoord(17, 65536, 0);
		cube.SetVertex(18, -2048, 2048,  2048); cube.SetColor(18, E_RGBA(0, 255, 255, 255)); cube.SetNormal(18, 0, 127, 0); cube.SetTexCoord(18, 0, 65536);
		cube.SetVertex(19,  2048, 2048,  2048); cube.SetColor(19, E_RGBA(0, 255, 255, 255)); cube.SetNormal(19, 0, 127, 0); cube.SetTexCoord(19, 65536, 65536);

		//top
		cube.SetPolygon(10, 20, 21, 22);
		cube.SetPolygon(11, 21, 23, 22);
		cube.SetVertex(20, -2048, -2048, -2048); cube.SetColor(20, E_RGBA(0, 0, 255, 255)); cube.SetNormal(20, 0, -127, 0); cube.SetTexCoord(20, 0, 0);
		cube.SetVertex(21,  2048, -2048, -2048); cube.SetColor(21, E_RGBA(0, 0, 255, 255)); cube.SetNormal(21, 0, -127, 0); cube.SetTexCoord(21, 65536, 0);
		cube.SetVertex(22, -2048, -2048,  2048); cube.SetColor(22, E_RGBA(0, 0, 255, 255)); cube.SetNormal(22, 0, -127, 0); cube.SetTexCoord(22, 0, 65536);
		cube.SetVertex(23,  2048, -2048,  2048); cube.SetColor(23, E_RGBA(0, 0, 255, 255)); cube.SetNormal(23, 0, -127, 0); cube.SetTexCoord(23, 65536, 65536);
	}

	//Load the Edge logo model
	if (display->CreateSurface(&edgelogo, "edgelogo.3ds", -3, EST_READONLYPATH) != E_OK)
		return(E_ERROR);
	
	//Set 1 light source
	display->UseLight(0);
	display->SetLightSource(0, 4096, 4096, -4096, ECOLOR_WHITE);

	//Loading went OK
	return(E_OK);
}

//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
	//Let the user have control over the models
	ModelInteract(timedelta);

	//Before rotating, translating or scaling, reset the cube to its original position to prevent rounding errors
	cube.Reset();
	cube.Translate(-1024, 0, 0);
	cube.Scale(20000, 20000, 20000);
	cube.Rotate(yr, xr, zr);
	edgelogo.Reset();
	edgelogo.Translate(1024, 0, 0);
	edgelogo.Scale(20000, 20000, 20000);
	edgelogo.Rotate(yr, xr, zr);

	//Fill the screen with a nice blue color
	#if defined(EGL_USEGL)
		#if defined(EGL_USEGLES)
			glClearColorx(0, 19968, 38912, 65536);
		#else
			glClearColor(0, 0.3f, 0.6f, 1);
		#endif
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	#else
		display->buffer.FillRect(NULL, E_RGBX(0, 78, 152));
	#endif

	//Set 3D perspective and move the camera backwards (scale for the Edge internal renderer)
	display->Perspective3D(display->GetWidth(), display->GetHeight());
	display->WorldReset();
	display->WorldTranslate(0, 0, -6000);
	#if !defined(EGL_USEGL)
		display->WorldScale(65536 * 2, 65536 * 2, 65536 * 2);
	#endif

	//Render the models using various shaders/textures, let the user decide
	display->BindTexture(&texture);
	display->SetShading(rendertype_shading[rendertype]);
	display->Render(&cube);
	display->Render(&edgelogo);

	//Show general information on how to use this program
	display->Perspective2D(display->GetWidth(), display->GetHeight(), false);
	display->SetShading(E3D_TEXTURE);
	display->DrawFont(2, display->GetHeight() - display->fontinternal.GetFontHeight() * 3 - 2, &display->fontinternal, "Stylus/Arrows: Rotate");
	display->DrawFont(2, display->GetHeight() - display->fontinternal.GetFontHeight() * 2 - 2, &display->fontinternal, "Wheel/Softkeys: Change shading");
	display->DrawFont(2, display->GetHeight() - display->fontinternal.GetFontHeight() - 2, &display->fontinternal, "Other keys: Quit program");

	//Retrieve and show the current amount of frames per second
	display->DrawFont(2, 2, &display->fontinternal, GetFPS());

	//Everything went OK
	return(E_OK);
}

//Callback: Called when the program exits
void ClassMain::OnExit(ENATIVETYPE instance)
{
}

//Callback: Called when the user pressed a key or button
void ClassMain::OnButtonDown(unsigned long bnr, EBUTTONLIST *blist)
{
	//Quit if not an arrow or softkey
	if (
		!ecd.input->MappedTo(DIAM_MENUUP, bnr) && !ecd.input->MappedTo(DIAM_MENUDOWN, bnr) &&
		!ecd.input->MappedTo(DIAM_MENULEFT, bnr) && !ecd.input->MappedTo(DIAM_MENURIGHT, bnr) &&
		!ecd.input->MappedTo(DIAM_MENUSOFT1, bnr) && !ecd.input->MappedTo(DIAM_MENUSOFT2, bnr)
	)
	{
		Quit();
	}
}
