/////////////////////////////////////////////////////////////////////
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
	E3D_WIREFRAME | E3D_FLAT,
	E3D_GOURAUD,
	E3D_GOURAUD | E3D_TEXTURE
};

/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

//The main class
class ClassMain : public ClassEdge
{
	private:
		E2DSurface background;
		E2DSurface turtle_tex;
		E2DSurface block_tex;
		E3DSurface turtle;
		E3DSurface block;
		long zoom;
		unsigned long anitimer;
		unsigned short rotation;
		signed char rendertype;
		void DrawBackground(ClassEDisplay *display);
		void DrawModels(ClassEDisplay *display);
	public:
		ClassMain(void);
		~ClassMain(void);
		ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
		ERESULT OnInit(ENATIVETYPE instance);
		ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display);
		ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
		void OnButtonDown(unsigned long bnr, EBUTTONLIST *blist);
		void OnWheel(long direction);
		void OnStylusMove(POINT pnt);
};


/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////

//Draw the background picture
void ClassMain::DrawBackground(ClassEDisplay *display)
{
	#if defined(EGL_USEGL)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		display->Perspective2D(256, 256, false);
		display->BltFast(0, 0, &background, NULL);
	#else
		E2DBLTFX bltfx;
		bltfx.flags = 0;
		bltfx.property = 0;
		bltfx.xscale = 65536 * display->buffer.GetWidth() / background.GetWidth();
		bltfx.yscale = 65536 * display->buffer.GetHeight() / background.GetHeight();
		bltfx.rotation = 0;
		bltfx.flipflags = 0;
		display->buffer.BltFx(display->buffer.GetWidth() / 2, display->buffer.GetHeight() / 2, &background, NULL, &bltfx);
	#endif
}

//Draw 3D models
void ClassMain::DrawModels(ClassEDisplay *display)
{
	display->Perspective3D(display->GetWidth(), display->GetHeight());
	display->WorldReset();
	display->WorldTranslate(0, 0, zoom);
	#if !defined(EGL_USEGL)
		display->WorldScale(65536 * 2, 65536 * 2, 65536 * 2);
	#endif
	display->WorldRotate(256, 0, 0);
	display->WorldRotate(0, rotation, 0);
	display->BindTexture(&block_tex);
	display->SetShading(E3D_GOURAUD | E3D_TEXTURE);
	display->Render(&block);
	turtle.Animate(anitimer / 2);
	display->BindTexture(&turtle_tex);
	display->SetShading(rendertype_shading[rendertype]);
	display->Render(&turtle);
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

//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
	//Icon and title
	ClassEStd::StrCpy(config->caption, "Animate 3D");
	config->icon = IDI_MAIN;
	
	//Desktop resolution
	config->width = 640;
	config->height = 480;

	//Other options
	config->orientation = DOR_AUTO;
	config->engineconsole = false;
	return(E_OK);
}

//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
	anitimer = 0;
	rotation = 0;
	zoom = -7000;
	rendertype = 2;
	return(E_OK);
}

//Callback: Called when the display mode has changed, use this to initialize your surfaces, etc.
ERESULT ClassMain::OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)
{
	//Load models and textures
	if (display->CreateSurface(&turtle, "turtle.ms3d", 0, EST_READONLYPATH) != E_OK)
		return(E_ERROR);
	if (display->CreateSurface(&turtle_tex, "turtle.png", EST_READONLYPATH) != E_OK)
		return(E_ERROR);
	if (display->CreateSurface(&block_tex, "block.png", EST_READONLYPATH) != E_OK)
		return(E_ERROR);
	if (display->CreateSurface(&background, "background.jpg", EST_READONLYPATH) != E_OK)
		return(E_ERROR);
	display->UploadTexture(&turtle_tex);
	display->UploadTexture(&block_tex);
	display->UploadTexture(&background);

	//Create block model
	if (display->CreateSurface(&block, 20, 10, EST_NOCOLORARRAY) == E_OK)
	{
		//Top
		block.SetPolygon(0, 0, 2, 1);
		block.SetPolygon(1, 1, 2, 3);
		block.SetVertex(0, -2048, 0, -2048); block.SetTexCoord(0, 0, 0);
		block.SetVertex(1,  2048, 0, -2048); block.SetTexCoord(1, 65536, 0);
		block.SetVertex(2, -2048, 0,  2048); block.SetTexCoord(2, 0, 65536);
		block.SetVertex(3,  2048, 0,  2048); block.SetTexCoord(3, 65536, 65536);

		//Front
		block.SetPolygon(2, 4, 6, 5);
		block.SetPolygon(3, 5, 6, 7);
		block.SetVertex(4, -2048, 0, 2048); block.SetTexCoord(4, 0, 0);
		block.SetVertex(5,  2048, 0, 2048); block.SetTexCoord(5, 65536, 0);
		block.SetVertex(6, -2048, -1024, 2048); block.SetTexCoord(6, 0, 32768);
		block.SetVertex(7,  2048, -1024, 2048); block.SetTexCoord(7, 65536, 32768);

		//Back
		block.SetPolygon(4, 8, 9, 10);
		block.SetPolygon(5, 9, 11, 10);
		block.SetVertex(8,  -2048, 0, -2048); block.SetTexCoord(8, 0, 0);
		block.SetVertex(9,   2048, 0, -2048); block.SetTexCoord(9, 65536, 0);
		block.SetVertex(10, -2048, -1024, -2048); block.SetTexCoord(10, 0, 32768);
		block.SetVertex(11,  2048, -1024, -2048); block.SetTexCoord(11, 65536, 32768);

		//Left
		block.SetPolygon(6, 12, 13, 14);
		block.SetPolygon(7, 13, 15, 14);
		block.SetVertex(12, -2048, 0, -2048); block.SetTexCoord(12, 0, 0);
		block.SetVertex(13, -2048, -1024, -2048); block.SetTexCoord(13, 0, 32768);
		block.SetVertex(14, -2048, 0,  2048); block.SetTexCoord(14, 65536, 0);
		block.SetVertex(15, -2048, -1024,  2048); block.SetTexCoord(15, 65536, 32768);

		//Right
		block.SetPolygon(8, 16, 18, 17);
		block.SetPolygon(9, 17, 18, 19);
		block.SetVertex(16, 2048, 0, -2048); block.SetTexCoord(16, 0, 0);
		block.SetVertex(17, 2048, -1024, -2048); block.SetTexCoord(17, 0, 32768);
		block.SetVertex(18, 2048, 0,  2048); block.SetTexCoord(18, 65536, 0);
		block.SetVertex(19, 2048, -1024,  2048); block.SetTexCoord(19, 65536, 32768);

		//Calculate lighting
		block.CalculateNormals();
	}
	else
		return(E_ERROR);

	//Increase turtle size
	turtle.Scale(100000, 100000, 100000);

	//Add 3 light sources
	display->UseLight(0);
	display->SetLightSource(0, 2048, 2048, -4096, ECOLOR_WHITE, E_RGBX(64, 64, 64));
	display->UseLight(1);
	display->SetLightSource(1, 2048, 2048, -4096, ECOLOR_WHITE);
	display->UseLight(2);
	display->SetLightSource(2, 2048, 2048, -4096, ECOLOR_WHITE);

	//Use blending in OpenGL
	#if defined(EGL_USEGL)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	#endif
	return(E_OK);
}

//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
	//Rotate and zoom based on key input
	if (ecd.input->KeyHold(DIAM_MENUUP))
		zoom += (short)timedelta * 3;
	else if (ecd.input->KeyHold(DIAM_MENUDOWN))
		zoom -= (short)timedelta * 3;
	if (ecd.input->KeyHold(DIAM_MENULEFT))
		rotation -= (short)timedelta;
	else if (ecd.input->KeyHold(DIAM_MENURIGHT))
		rotation += (short)timedelta;

	//Don't zoom past certain points
	if (zoom > -7000)
		zoom = -7000;
	else if (zoom < -20000)
		zoom = -20000;

	//Change render method through the soft keys
	E_INPUTEVENT ievent;
	while(ecd.input->GetKeyEvent(&ievent))
	{
		if (ievent.type == EIE_PRESSED)
		{
			if (ievent.action == DIAM_MENUSOFT1)
			{
				rendertype++;
				if (rendertype > 2)
					rendertype = 0;
			}
			else if (ievent.action == DIAM_MENUSOFT2)
			{
				rendertype--;
				if (rendertype < 0)
					rendertype = 2;
			}
		}
	}

	//Animate and draw the model
	anitimer = (anitimer + timedelta) % 10000;
	DrawBackground(display);
	DrawModels(display);
	display->Perspective2D(display->GetWidth(), display->GetHeight(), false);
	display->SetShading(E3D_TEXTURE);
	display->DrawFont(2, 2, &display->fontinternal, GetFPS());
	display->DrawFont(2, display->GetHeight() - display->fontinternal.GetFontHeight() * 2 - 2, &display->fontinternal, "Softkeys: Change shading", EFX_COLORKEY);
	display->DrawFont(2, display->GetHeight() - display->fontinternal.GetFontHeight() - 2, &display->fontinternal, "Model by: www.psionic3d.co.uk", EFX_COLORKEY);
	return(E_OK);
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

//Callback: Called when the user scrolls a wheel device
void ClassMain::OnWheel(long direction)
{
	if (direction < 0)
		rotation += 128;
	else if (direction > 0)
		rotation -= 128;
}

//Callback: Called when the user moves the stylus while pressed down, or moves the mouse cursor
void ClassMain::OnStylusMove(POINT pnt)
{
	rotation = (unsigned short)((pnt.x - (long)ecd.dsp->GetWidth() / 2) * 8192 / (long)ecd.dsp->GetWidth());
}
