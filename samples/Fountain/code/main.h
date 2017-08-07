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

/////////////////////////////////////////////////////////////////////
// Definitions                                                     //
/////////////////////////////////////////////////////////////////////

#define BGCOLOR          ECOLOR_NAVY //Use a dark shade of blue as background color

#define MAX_SPRITESHEET 12           //The number of sprites on the sheet

#define MAX_PARTS      300           //The maximum amount of particles
#define MAX_AGE       2000           //The time a particle remains active
#define GRAVITY         32           //The amount of gravity that pulls on each particle
#define SPAWNSPEED      26           //The speed at which the source spawns particles

//Important: double & float datatypes are slow on mobile devices, use fixed point instead
#define efixed        long           //Fixed point data type
#define FPREC           18           //Fixed point precision
#define FTOI(v) ((v) >> FPREC)
#define ITOF(v) ((long)(v) << FPREC) //Fixed point conversion

//Macro for converting scale values that were used before Edge 2.20
#define CONVERT_SCALE(s) (((1 << 30) / (s)) << 2)

/////////////////////////////////////////////////////////////////////
// Structures                                                      //
/////////////////////////////////////////////////////////////////////

//Particle information including position, speed and the time it exists
typedef struct
{
	efixed x;
	efixed y;
	efixed vel_x;
	efixed vel_y;
	efixed age;
}PARTICLE;

/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

//The main class
class ClassMain : public ClassEdge
{
	private:
		PARTICLE particle[MAX_PARTS];
		E2DSurface particlesheet;
		unsigned short spritesize;
		long bufferwidth, bufferheight;
		efixed src_x, src_y, src_spawntime;

		bool SearchParticle(unsigned long &found);
		void SpawnParticles(unsigned long timedelta);
		void MoveParticles(unsigned long timedelta);
		void RenderParticles(E2DSurface *buffer);
	public:
		ClassMain(void);
		~ClassMain(void);
		ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config);
		ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
		ERESULT OnInit(ENATIVETYPE instance);
		ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display);
		ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
		void OnButtonDown(unsigned long bnr, EBUTTONLIST *blist);
		void OnStylusDown(POINT pnt);
};


/////////////////////////////////////////////////////////////////////
// ClassMain: Private                                              //
/////////////////////////////////////////////////////////////////////

//Search for a free particle
bool ClassMain::SearchParticle(unsigned long &found)
{
	for (unsigned long i = 0; i < MAX_PARTS; i++)
		if (particle[i].age >= MAX_AGE)
		{
			found = i;
			return(true);
		}
	return(false);
}

//Spawn a particle from the source into a random direction from time to time
void ClassMain::SpawnParticles(unsigned long timedelta)
{
	efixed angle;
	unsigned long found;
	src_spawntime += timedelta * 4;
	while(src_spawntime >= SPAWNSPEED)
	{
		src_spawntime -= SPAWNSPEED;
		if (SearchParticle(found))
		{
			particle[found].age   = 0;
			particle[found].x     = src_x;
			particle[found].y     = src_y;

			//The Edge sine/cosine calculator takes a value between 0 and 4095
			angle = (rand() % 2048) - 1024;

			//Important: sine/cosine calculates use 16:16 fixed point math
#if (FPREC > 16)
			particle[found].vel_x = (ecd.math->Sin(angle) / 8) << (FPREC - 16);
			particle[found].vel_y = (-ecd.math->Cos(angle) / 2) << (FPREC - 16);
#else
#if (FPREC < 16)
			particle[found].vel_x = (ecd.math->Sin(angle) / 8) >> (16 - FPREC);
			particle[found].vel_y = (-ecd.math->Cos(angle) / 2) >> (16 - FPREC);
#else
			particle[found].vel_x = (ecd.math->Sin(angle) / 8);
			particle[found].vel_y = (-ecd.math->Cos(angle) / 2);
#endif
#endif
		}
		else
			break;
	}
}

//Moving particles are influenced by gravity and bounce at the bottom of the screen
void ClassMain::MoveParticles(unsigned long timedelta)
{
	//Process particles for each millisecond for accurate movement on all devices
	//Note: this may cause some serious slowdown with a lot of particles
	for (unsigned long i = 0; i < MAX_PARTS; i++)
		if (particle[i].age < MAX_AGE)
			for (unsigned long t = 0; t < timedelta; t++)
			{
				if (particle[i].vel_y > 0 && FTOI(particle[i].y) > bufferheight)
					particle[i].vel_y = -particle[i].vel_y / 2;
				particle[i].y += particle[i].vel_y * spritesize / 16;
				particle[i].x += particle[i].vel_x * spritesize / 16;
				if (ecd.input->KeyHold(DIAM_MENULEFT))
					particle[i].vel_x -= GRAVITY;
				else if (ecd.input->KeyHold(DIAM_MENURIGHT))
					particle[i].vel_x += GRAVITY;
				if (ecd.input->KeyHold(DIAM_MENUUP))
					particle[i].vel_y += GRAVITY / 4;
				else if (ecd.input->KeyHold(DIAM_MENUDOWN))
					particle[i].vel_y += GRAVITY * 4;
				else
					particle[i].vel_y += GRAVITY;
				particle[i].age++;
			}
}

//Render each particle on-screen
void ClassMain::RenderParticles(E2DSurface *buffer)
{
	RECT rc;
	SetRect(&rc, 0, 0, spritesize, spritesize);
	for (unsigned long i = 0; i < MAX_PARTS; i++)
		if (particle[i].age < MAX_AGE)
		{
			rc.left  = particle[i].age * MAX_SPRITESHEET / MAX_AGE * spritesize;
			rc.right = rc.left + spritesize;
			buffer->BltFast(FTOI(particle[i].x), FTOI(particle[i].y), &particlesheet, &rc, EFX_COLORKEY | EFX_ALPHA, 128);
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
	return(E_OK);
}

//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
	ClassEStd::StrCpy(config->caption, "Fountain");
	config->icon            = IDI_MAIN;           //Application icon
	config->width           = 360;                //Desktop PC: Custom display width
	config->height          = 480;                //Desktop PC: Custom display height
	config->enginestatus    = true;               //Desktop PC: Enable the status bar and status information icon
	config->engineconsole   = false;              //Desktop PC: Disable the edge console
	return(E_OK);
}

//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
	unsigned long ctr;
	for (ctr = 0; ctr < MAX_PARTS; ctr++)
		particle[ctr].age = MAX_AGE;
	src_spawntime = 0;
	return(E_OK);
}

//Callback: Called when the display mode has changed, use this to initialize your surfaces, etc.
ERESULT ClassMain::OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)
{
	//Retrieve and save the size of the display
	bufferwidth   = display->buffer.GetWidth();
	bufferheight  = display->buffer.GetHeight();

	//Set the particle source at the centre bottom of the screen
	src_spawntime = 0;
	src_x         = ITOF(bufferwidth  / 2);
	src_y         = ITOF(bufferheight);

	//Load the source sprite from a file
	E2DSurface sprite;
	if (display->CreateSurface(&sprite, "particle.bmp", EST_SYSMEMORY | EST_DITHER | EST_READONLYPATH) != E_OK)
		return(E_ERROR);
	sprite.AutoSetColorKey();

	//Build a spritesheet from the source image
	if (bufferwidth < bufferheight)
		spritesize = (unsigned short)(bufferwidth / 15);
	else
		spritesize = (unsigned short)(bufferheight / 15);
	if (spritesize % 2 == 1)
		spritesize++;
	if (display->CreateSurface(&particlesheet, spritesize * MAX_SPRITESHEET, spritesize) == E_OK)
	{
		unsigned char ctr;
		particlesheet.SetColorKey(sprite.GetColorKey());
		for (ctr = 0; ctr < MAX_SPRITESHEET; ctr++)
			particlesheet.Blt(ctr * spritesize + spritesize / 2, spritesize / 2, &sprite, NULL, 0, CONVERT_SCALE(65536 * (sprite.GetWidth() / spritesize + 1) * MAX_SPRITESHEET / (MAX_SPRITESHEET - ctr)));
	}
	else
		return(E_ERROR);

	//Everything went OK
	return(E_OK);
}

//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
	//Spawn particles from the source, pause when holding the second button
	if (!ecd.input->KeyHold(DIAM_MENUCONFIRM))
		SpawnParticles(timedelta);
	
	//Particle behavior
	MoveParticles(timedelta);

	//Fill the screen with a background color
	display->buffer.FillRect(NULL, BGCOLOR);

	//Render all active particles
	RenderParticles(&display->buffer);

	//Show general information on how to use this program
	display->buffer.DrawFont(2, display->buffer.GetHeight() - display->fontinternal.GetFontHeight() * 3 - 2, &display->fontinternal, "Arrows: Adjust gravity");
	display->buffer.DrawFont(2, display->buffer.GetHeight() - display->fontinternal.GetFontHeight() * 2 - 2, &display->fontinternal, "Confirm: Pause generator");
	display->buffer.DrawFont(2, display->buffer.GetHeight() - display->fontinternal.GetFontHeight() - 2, &display->fontinternal, "Other keys/Stylus: Quit");

	//Retrieve and show the current amount of frames per second
	display->buffer.DrawFont(2, 2, &display->fontinternal, GetFPS());

	//Everything went OK
	return(E_OK);
}

//Callback: Called when the user pressed a key or button
void ClassMain::OnButtonDown(unsigned long bnr, EBUTTONLIST *blist)
{
	//Quit if not an arrow or confirm key
	if (
		!ecd.input->MappedTo(DIAM_MENUUP, bnr) && !ecd.input->MappedTo(DIAM_MENUDOWN, bnr) &&
		!ecd.input->MappedTo(DIAM_MENULEFT, bnr) && !ecd.input->MappedTo(DIAM_MENURIGHT, bnr) &&
		!ecd.input->MappedTo(DIAM_MENUCONFIRM, bnr)
	)
	{
		Quit();
	}
}

//Callback: Called when the user points the stylus down or clicks the left mouse button
void ClassMain::OnStylusDown(POINT pnt)
{
	Quit();
}
