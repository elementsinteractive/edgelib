// code/main.h
// Contains the main framework class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Definitions                                                     //
/////////////////////////////////////////////////////////////////////

//Palette definitions
#define MAX_PALETTETIME 32768
#define MAX_PALETTEFADE  1024

//Object limits
#define MAX_LINES           6
#define MAX_POINTS        256
#define MAX_GROUPS         32

//State definitions
#define MAX_STATETIME   65536

//Screensaver state types
#define STATE_LINES         0
#define STATE_BOUNCE        1
#define STATE_FOLLOW        2
#define STATE_FOUNTAIN      3
#define STATE_FIRE          4

/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

class ClassMain : public ClassEdge
{
	private:
		E2DSurface dot;
		RECT viewport;
		SAVERSETTINGS settings;
		COLORREF paldata[MAX_PALETTES][256];
		long line_x[MAX_LINES];
		long line_y[MAX_LINES];
		long line_xvel[MAX_LINES];
		long line_yvel[MAX_LINES];
		long point_x[MAX_POINTS];
		long point_y[MAX_POINTS];
		long point_xvel[MAX_POINTS];
		long point_yvel[MAX_POINTS];
		long point_dir[MAX_POINTS];
		long clickx;
		long clicky;
		long anim_unit;
		long dotsize;
		unsigned long palettetimer;
		unsigned long statetimer;
		unsigned long numlines;
		unsigned long numpoints;
		unsigned char curpalette;
		unsigned char newpalette;
		unsigned char curstate;
		unsigned char newstate;
		unsigned char interpolatestate;
		unsigned char numstylusmove;
		bool allowfire;
		void InterpolateSurface(E2DSurface *buffer);
		unsigned short GetDirection(long x1, long y1, long x2, long y2);
		void InitPalettes(void);
		void CreateDotSurface(ClassEDisplay *display);
		void InitObjects(unsigned char initstate, unsigned char prevstate);
		void MoveObjects(unsigned long timedelta);
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
		void OnWheel(long direction);
		void OnStylusDown(POINT pnt);
		void OnStylusMove(POINT pnt);
};//The main class

/////////////////////////////////////////////////////////////////////
// ClassMain::Private                                              //
/////////////////////////////////////////////////////////////////////

//Interpolate the surface with a fast smudge algorithm by accessing the backbuffer directly
void ClassMain::InterpolateSurface(E2DSurface *buffer)
{
	E2DSURFACEINFO info;
	unsigned long xc, yc;
	unsigned char *memptr;
	memptr = buffer->Lock(&info);
	if (memptr)
	{
		unsigned char *tmemptr;
		if (curstate != STATE_FIRE)
		{
			tmemptr = &memptr[info.realpitch];
			for (yc = 0; yc < info.realheight - 2; yc++)
			{
				tmemptr[0] = 0;
				tmemptr[info.realwidth - 1] = 0;
				tmemptr += info.realpitch;
			}
		}
		tmemptr = &memptr[0];
		for (xc = 0; xc < info.realwidth; xc++)
			*tmemptr++ = 0;
		if (curstate != STATE_FIRE)
		{
			tmemptr = &memptr[(info.realheight - 1) * info.realpitch];
			for (xc = 0; xc < info.realwidth; xc++)
				*tmemptr++ = 0;
		}
		if (curstate == STATE_FIRE)
		{
			if (info.realpitch == (long)info.realwidth)
			{
				long totalsize = (info.realheight - 2) * info.realpitch;
				tmemptr = memptr;
				while(totalsize)
				{
					*tmemptr = (tmemptr[info.realpitch - 1] + tmemptr[info.realpitch] + tmemptr[info.realpitch + 1] + tmemptr[info.realpitch * 2]) / 4;
					if (*tmemptr)
						(*tmemptr)--;
					tmemptr++;
					totalsize--;
				}
				totalsize = 2 * info.realpitch;
				while(totalsize)
				{
					*tmemptr = (tmemptr[-info.realpitch * 2] + tmemptr[-info.realpitch] + tmemptr[-1] + tmemptr[1]) / 4;
					if (*tmemptr)
						(*tmemptr)--;
					tmemptr++;
					totalsize--;
				}
			}
			else
			{
				tmemptr = memptr;
				for (yc = 0; yc < info.realheight - 2; yc++)
				{
					for (xc = 0; xc < info.realwidth; xc++)
					{
						*tmemptr = (tmemptr[info.realpitch - 1] + tmemptr[info.realpitch] + tmemptr[info.realpitch + 1] + tmemptr[info.realpitch * 2]) / 4;
						if (*tmemptr)
							(*tmemptr)--;
						tmemptr++;
					}
					tmemptr += info.realpitch - info.realwidth;
				}
				for (yc = 0; yc < 2; yc++)
				{
					for (xc = 0; xc < info.realwidth; xc++)
					{
						*tmemptr = (tmemptr[-info.realpitch * 2] + tmemptr[-info.realpitch] + tmemptr[-1] + tmemptr[1]) / 4;
						if (*tmemptr)
							(*tmemptr)--;
						tmemptr++;
					}
					tmemptr += info.realpitch - info.realwidth;
				}
			}
		}
		else
		{
			if (info.realpitch == (long)info.realwidth)
			{
				long totalsize = (info.realheight - 2) * info.realpitch;
				tmemptr = &memptr[info.realpitch];
				while(totalsize)
				{
					*tmemptr = (tmemptr[-info.realpitch] + tmemptr[-1] + tmemptr[1] + tmemptr[info.realpitch]) / 4;
					if (*tmemptr)
						(*tmemptr)--;
					tmemptr++;
					totalsize--;
				}
			}
			else
			{
				tmemptr = &memptr[info.realpitch + 1];
				for (yc = 0; yc < info.realheight - 2; yc++)
				{
					for (xc = 0; xc < info.realwidth - 2; xc++)
					{
						*tmemptr = (tmemptr[-info.realpitch] + tmemptr[-1] + tmemptr[1] + tmemptr[info.realpitch]) / 4;
						if (*tmemptr)
							(*tmemptr)--;
						tmemptr++;
					}
					tmemptr += info.realpitch - (info.realwidth - 2);
				}
			}
		}
		buffer->Unlock();
	}
}

//Get the direction between 2 points
unsigned short ClassMain::GetDirection(long x1, long y1, long x2, long y2)
{
	long direction = 0;
	if (x1 == x2)
	{
		if (y2 > y1)
			direction = 2048;
		else
			direction = 0;
	}
	else if (y1 == y2)
	{
		if (x2 > x1)
			direction = 1024;
		else
			direction = 3072;
	}
	else if (ecd.math->Abs(x2 - x1) < ecd.math->Abs(y2 - y1))
	{
		if (y2 > y1)
		{
			if (x2 > x1)
				direction = 2047 - 511 * ecd.math->Abs(x2 - x1) / ecd.math->Abs(y2 - y1);
			else
				direction = 2049 + 511 * ecd.math->Abs(x2 - x1) / ecd.math->Abs(y2 - y1);
		}
		else
		{
			if (x2 > x1)
				direction = 1 + 511 * ecd.math->Abs(x2 - x1) / ecd.math->Abs(y2 - y1);
			else
				direction = 4095 - 511 * ecd.math->Abs(x2 - x1) / ecd.math->Abs(y2 - y1);
		}
	}
	else
	{
		if (x2 > x1)
		{
			if (y2 > y1)
				direction = 1025 + 511 * ecd.math->Abs(y2 - y1) / ecd.math->Abs(x2 - x1);
			else
				direction = 1023 - 511 * ecd.math->Abs(y2 - y1) / ecd.math->Abs(x2 - x1);
		}
		else
		{
			if (y2 > y1)
				direction = 3071 - 511 * ecd.math->Abs(y2 - y1) / ecd.math->Abs(x2 - x1);
			else
				direction = 3073 + 511 * ecd.math->Abs(y2 - y1) / ecd.math->Abs(x2 - x1);
		}
	}
	return((unsigned short)direction);
}

//Create palettes
void ClassMain::InitPalettes(void)
{
	unsigned short ctr;
	for (ctr = 0; ctr < 256; ctr++)
	{
		if (ctr < 64)
		{
			paldata[0][ctr] = E_RGBX(ctr * 4, 0, 0);
			paldata[1][ctr] = E_RGBX(0, 0, ctr * 4);
		}
		else if (ctr < 128)
		{
			paldata[0][ctr] = E_RGBX(255, (ctr - 64) * 4, 0);
			paldata[1][ctr] = E_RGBX(0, (ctr - 64) * 4, 255);
		}
		else if (ctr < 192)
		{
			paldata[0][ctr] = E_RGBX(255, 255, (ctr - 128) * 4);
			paldata[1][ctr] = E_RGBX((ctr - 128) * 4, 255, 255);
		}
		else
		{
			paldata[0][ctr] = E_RGBX(255, 255, 255);
			paldata[1][ctr] = E_RGBX(255, 255, 255);
		}
		if (ctr < 128)
			paldata[2][ctr] = E_RGBX(0, ctr * 2, 0);
		else if (ctr < 192)
			paldata[2][ctr] = E_RGBX((ctr - 128) * 4, 255, (ctr - 128) * 4);
		else
			paldata[2][ctr] = E_RGBX(255, 255, 255);
	}
}

//Create a surface for drawing particles (size depends on the screen resolution)
void ClassMain::CreateDotSurface(ClassEDisplay *display)
{
	unsigned char ctr;
	if (anim_unit > 100)
	{
		unsigned char dotlayout81[] =
		{
			  0,   0,  64, 128, 192, 128,  64,   0,   0,
			  0,  80, 144, 192, 224, 192, 144,  80,   0,
			 64, 128, 192, 224, 255, 224, 192, 128,  64,
			128, 192, 224, 255, 255, 255, 224, 192, 128,
			192, 224, 255, 255, 255, 255, 255, 224, 192,
			128, 192, 224, 255, 255, 255, 224, 192, 128,
			 64, 128, 192, 224, 255, 224, 192, 128,  64,
			  0,  80, 144, 192, 224, 192, 144,  80,   0,
			  0,   0,  64, 128, 192, 128,  64,   0,   0
		};
		display->CreateSurface(&dot, 9, 9);
		for (ctr = 0; ctr < 81; ctr++)
			dot.PutPixel(ctr % 9, ctr / 9, dotlayout81[ctr]);
	}
	else if (anim_unit > 50)
	{
		unsigned char dotlayout25[] =
		{
			  0, 160, 192, 160,   0,
			160, 224, 255, 224, 160,
			192, 255, 255, 255, 192,
			160, 224, 255, 224, 160,
			  0, 160, 192, 160,   0
		};
		display->CreateSurface(&dot, 5, 5);
		for (ctr = 0; ctr < 25; ctr++)
			dot.PutPixel(ctr % 5, ctr / 5, dotlayout25[ctr]);
	}
	else
	{
		unsigned char dotlayout9[] =
		{
			160, 224, 160,
			224, 255, 224,
			160, 224, 160
		};
		display->CreateSurface(&dot, 3, 3);
		for (ctr = 0; ctr < 9; ctr++)
			dot.PutPixel(ctr % 3, ctr / 3, dotlayout9[ctr]);
	}
	dotsize = dot.GetWidth() << 8;
	dot.SetColorKey();
}

//Initialize object lines or points
void ClassMain::InitObjects(unsigned char initstate, unsigned char prevstate)
{
	unsigned long ctr;
	if (initstate == STATE_LINES)
	{
		for (ctr = 0; ctr < MAX_LINES; ctr++)
		{
			unsigned short direction = (unsigned short)(ecd.math->Rand() % 4096);
			line_x[ctr] = viewport.left + ecd.math->Rand() % (viewport.right - viewport.left);
			line_y[ctr] = viewport.top + ecd.math->Rand() % (viewport.bottom - viewport.top);
			line_xvel[ctr] = ecd.math->Sin(direction) * anim_unit / 65536;
			line_yvel[ctr] = ecd.math->Cos(direction) * anim_unit / 65536;
		}
	}
	else if (initstate == STATE_BOUNCE || initstate == STATE_FOLLOW)
	{
		for (ctr = 0; ctr < MAX_POINTS; ctr++)
		{
			if (prevstate != STATE_BOUNCE && prevstate != STATE_FOLLOW)
			{
				point_x[ctr] = viewport.left + ecd.math->Rand() % (viewport.right - viewport.left - dotsize);
				point_y[ctr] = viewport.top + ecd.math->Rand() % (viewport.bottom - viewport.top - dotsize);
			}
			if (prevstate != STATE_BOUNCE)
				point_dir[ctr] = ecd.math->Rand() % 65536;
		}
	}
	else if (initstate == STATE_FOUNTAIN || initstate == STATE_FIRE)
	{
		for (ctr = 0; ctr < MAX_POINTS; ctr++)
		{
			point_x[ctr] = -0xFFFF;
			point_y[ctr] = ecd.math->Rand() % 2048;
		}
	}
}

//Move object lines or points
void ClassMain::MoveObjects(unsigned long timedelta)
{
	unsigned long ctr;
	if (curstate == STATE_LINES)
	{
		for (ctr = 0; ctr < MAX_LINES; ctr++)
		{
			line_x[ctr] += line_xvel[ctr] * timedelta;
			if (line_x[ctr] < viewport.left)
			{
				line_x[ctr] = viewport.left;
				if (line_xvel[ctr] < 0)
					line_xvel[ctr] = -line_xvel[ctr];
			}
			if (line_x[ctr] > viewport.right)
			{
				line_x[ctr] = viewport.right;
				if (line_xvel[ctr] > 0)
					line_xvel[ctr] = -line_xvel[ctr];
			}
			line_y[ctr] += line_yvel[ctr] * timedelta;
			if (line_y[ctr] < viewport.top)
			{
				line_y[ctr] = viewport.top;
				if (line_yvel[ctr] < 0)
					line_yvel[ctr] = -line_yvel[ctr];
			}
			if (line_y[ctr] > viewport.bottom)
			{
				line_y[ctr] = viewport.bottom;
				if (line_yvel[ctr] > 0)
					line_yvel[ctr] = -line_yvel[ctr];
			}
		}
	}
	else if (curstate == STATE_BOUNCE || curstate == STATE_FOLLOW)
	{
		for (ctr = 0; ctr < MAX_POINTS; ctr++)
		{
			if (curstate == STATE_FOLLOW && ctr % MAX_GROUPS != 0)
			{
				unsigned long leader = ctr - ctr % MAX_GROUPS;
				long newdirection = GetDirection(point_x[ctr], point_y[ctr], point_x[leader], point_y[leader]) << 4;
				long dirdiff = newdirection % 65536 - point_dir[ctr] % 65536;
				if (ecd.math->Abs(dirdiff) >= 32768)
					dirdiff = -dirdiff;
				if (dirdiff > 0)
					point_dir[ctr] = (point_dir[ctr] + timedelta * 16) % 65536;
				else
					point_dir[ctr] = (point_dir[ctr] + 65536 - timedelta * 16) % 65536;
			}
			point_x[ctr] += ecd.math->Sin(point_dir[ctr] >> 4) * anim_unit / 65536 * timedelta;
			point_y[ctr] -= ecd.math->Cos(point_dir[ctr] >> 4) * anim_unit / 65536 * timedelta;
			if (point_x[ctr] < viewport.left)
			{
				point_x[ctr] = viewport.left;
				if (point_dir[ctr] % 65536 >= 32768)
					point_dir[ctr] = (65536 - point_dir[ctr] % 65536) % 65536;
			}
			else if (point_x[ctr] > viewport.right - dotsize)
			{
				point_x[ctr] = viewport.right - dotsize;
				if (point_dir[ctr] % 65536 < 32768)
					point_dir[ctr] = (65536 - point_dir[ctr] % 65536) % 65536;
			}
			if (point_y[ctr] < viewport.top)
			{
				point_y[ctr] = viewport.top;
				if (point_dir[ctr] % 65536 < 16384 || point_dir[ctr] % 65536 >= 49152)
					point_dir[ctr] = (65536 + 32768 - point_dir[ctr] % 65536) % 65536;
			}
			else if (point_y[ctr] > viewport.bottom - dotsize)
			{
				point_y[ctr] = viewport.bottom - dotsize;
				if (point_dir[ctr] % 65536 >= 16384 && point_dir[ctr] % 65536 < 49152)
					point_dir[ctr] = (65536 + 32768 - point_dir[ctr] % 65536) % 65536;
			}
		}
	}
	else if (curstate == STATE_FOUNTAIN)
	{
		for (ctr = 0; ctr < MAX_POINTS; ctr++)
		{
			if (point_x[ctr] == -0xFFFF)
			{
				if (point_y[ctr] != 0)
				{
					if (point_y[ctr] >= (long)timedelta)
						point_y[ctr] -= timedelta;
					else
						point_y[ctr] = 0;
				}
				if (point_y[ctr] == 0)
				{
					unsigned short rotation = (unsigned short)(4096 - 768 + ecd.math->Rand() % 1536);
					point_x[ctr] = viewport.left + (viewport.right - viewport.left) / 2 - dotsize / 2;
					point_y[ctr] = viewport.bottom;
					point_xvel[ctr] = ecd.math->Sin(rotation) * anim_unit / 65536;
					point_yvel[ctr] = -ecd.math->Cos(rotation) * anim_unit / 36;
				}
			}
			if (point_x[ctr] != -0xFFFF)
			{
				point_x[ctr] += point_xvel[ctr] * (long)timedelta / 2;
				point_y[ctr] += point_yvel[ctr] * (long)timedelta / 1024;
				point_yvel[ctr] += timedelta * anim_unit;
				if (point_y[ctr] >= viewport.bottom)
				{
					point_x[ctr] = -0xFFFF;
					point_y[ctr] = 0;
				}
			}
		}
	}
	else if (curstate == STATE_FIRE)
	{
		for (ctr = 0; ctr < 16 && ctr < MAX_POINTS; ctr++)
		{
			point_x[ctr] = ecd.math->Rand() % (ecd.dsp->buffer.GetWidth() << 8);
			point_y[ctr] = (ecd.dsp->buffer.GetHeight() << 8) - dotsize / 2;
		}
	}
}

/////////////////////////////////////////////////////////////////////
// ClassMain::Public                                               //
/////////////////////////////////////////////////////////////////////

//ClassMain: Constructor
ClassMain::ClassMain(void)
{
}

//ClassMain: Destructor
ClassMain::~ClassMain(void)
{
}

//Configure framework
ERESULT ClassMain::OnFrameworkConfig(EFRAMEWORKCONFIG *config)
{
	config->maxfps = 60;
	return(E_OK);
}

//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
	ClassEStd::StrCpy(config->caption, "EDGELIB Saver");
	config->icon = IDI_MAIN;
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		LoadSettings(&settings);
		if (settings.display == 0 || settings.display == 1)
		{
			config->width = GetSystemMetrics(SM_CXSCREEN);
			config->height = GetSystemMetrics(SM_CYSCREEN);
		}
		else if (settings.display == 2)
		{
			config->width = 640;
			config->height = 480;
		}
		else if (settings.display == 3)
		{
			config->width = 320;
			config->height = 240;
		}
		if (settings.display == 0)
		{
			config->videotype = EDSP_TRUE32;
			config->emulvideotype = EDSP_INDEXED8;
		}
		else
			config->videotype = EDSP_INDEXED8;
	#else
		config->emulvideotype = EDSP_INDEXED8;
	#endif
	config->fullscreen = true;
	config->engineconsole = false;
	config->enginestatus = false;
	config->orientation = DOR_AUTO;
	return(E_OK);
}

//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
	#if !defined(DEVICE_DESKTOP) || !defined(DEVICE_WIN32)
		LoadSettings(&settings);
	#endif
	if (settings.morelines)
		numlines = MAX_LINES;
	else
		numlines = MAX_LINES / 2;
	if (settings.morepoints)
		numpoints = MAX_POINTS;
	else
		numpoints = MAX_POINTS / 2;
	numstylusmove = 0;
	palettetimer = 0;
	statetimer = MAX_PALETTETIME / 2;
	curpalette = (unsigned char)(ecd.math->Rand() % MAX_PALETTES);
	newpalette = curpalette;
	if (settings.palette < MAX_PALETTES)
		curpalette = settings.palette;
	return(E_OK);
}

//Callback: Called when the program minimizes
ERESULT ClassMain::OnMinimize(void)
{
	Quit();
	return(E_OK);
}

//Callback: Called when the program restores back from a minimized state
ERESULT ClassMain::OnRestore(void)
{
	Quit();
	return(E_OK);
}

//Callback: Called when the display mode has changed
ERESULT ClassMain::OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)
{
	E2DSURFACEINFO info;
	display->buffer.GetInfo(&info);
	if (ecd.math->Abs(info.xpitch) < ecd.math->Abs(info.ypitch) && info.ypitch > 0)
		allowfire = true;
	else
		allowfire = false;
	SetRect(&viewport, 5 << 8, 5 << 8, (display->buffer.GetWidth() - 5) << 8, (display->buffer.GetHeight() - 5) << 8);
	anim_unit = (display->buffer.GetWidth() + display->buffer.GetHeight()) / 16;
	InitPalettes();
	display->SetPalette(paldata[curpalette], 0, 256);
	CreateDotSurface(display);
	if (allowfire)
	{
		curstate = (unsigned char)(ecd.math->Rand() % MAX_STATES);
		newstate = (unsigned char)(ecd.math->Rand() % MAX_STATES);
	}
	else
	{
		curstate = (unsigned char)(ecd.math->Rand() % (MAX_STATES - 1));
		newstate = (unsigned char)(ecd.math->Rand() % (MAX_STATES - 1));
	}
	if (settings.style < MAX_STATES)
		curstate = settings.style;
	InitObjects(curstate, 0xFF);
	return(E_OK);
}

//Callback: Called when a new frame can be processed
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
	RECT cliprc;
	unsigned long ctr;
	if (timedelta > 16)
		timedelta = 16;
	palettetimer += timedelta;
	if (palettetimer >= MAX_PALETTETIME && settings.palette >= MAX_PALETTES + 1)
	{
		if (newpalette == curpalette)
			newpalette = (unsigned char)(ecd.math->Rand() % MAX_PALETTES);
		if (newpalette != curpalette)
		{
			unsigned long palfadeindex = (palettetimer - MAX_PALETTETIME) * 256 / MAX_PALETTEFADE;
			if (palfadeindex < 256)
			{
				COLORREF fadepal[256];
				for (ctr = 0; ctr < 256; ctr++)
				{
					unsigned long r = E_GETR(paldata[curpalette][ctr]) * (256 - palfadeindex) + E_GETR(paldata[newpalette][ctr]) * palfadeindex;
					unsigned long g = E_GETG(paldata[curpalette][ctr]) * (256 - palfadeindex) + E_GETG(paldata[newpalette][ctr]) * palfadeindex;
					unsigned long b = E_GETB(paldata[curpalette][ctr]) * (256 - palfadeindex) + E_GETB(paldata[newpalette][ctr]) * palfadeindex;
					fadepal[ctr] = E_RGBX(r / 256, g / 256, b / 256);
				}
				display->SetPalette(fadepal, 0, 256);
			}
			else
			{
				curpalette = newpalette;
				palettetimer -= MAX_PALETTETIME + MAX_PALETTEFADE;
				display->SetPalette(paldata[curpalette], 0, 256);
			}
		}
		else
			palettetimer = MAX_PALETTETIME;
	}
	statetimer += timedelta;
	if (statetimer >= MAX_STATETIME + MAX_PALETTEFADE && settings.style >= MAX_STATES + 1)
	{
		statetimer -= MAX_STATETIME + MAX_PALETTEFADE;
		if (newstate != curstate)
			InitObjects(newstate, curstate);
		curstate = newstate;
		if (allowfire)
			newstate = (unsigned char)(ecd.math->Rand() % MAX_STATES);
		else
			newstate = (unsigned char)(ecd.math->Rand() % (MAX_STATES - 1));
	}
	MoveObjects(timedelta);
	if (curstate != STATE_FIRE)
	{
		SetRect(&cliprc, viewport.left >> 8, viewport.top >> 8, viewport.right >> 8, viewport.bottom >> 8);
		display->buffer.SetClipper(&cliprc);
	}
	if (curstate == STATE_LINES)
	{
		for (ctr = 0; ctr < numlines; ctr++)
			display->buffer.DrawLine(line_x[ctr] >> 8, line_y[ctr] >> 8, line_x[(ctr + 1) % numlines] >> 8, line_y[(ctr + 1) % numlines] >> 8, 255);
	}
	else
	{
		for (ctr = 0; ctr < numpoints; ctr++)
			display->buffer.BltFast(point_x[ctr] >> 8, point_y[ctr] >> 8, &dot, NULL, EFX_COLORKEY);
	}
	display->buffer.SetClipper(NULL);
	interpolatestate++;
	if (interpolatestate % 2 == 1 || !settings.slowinterpolate)
		InterpolateSurface(&display->buffer);
	return(E_OK);
}

//Callback: Called when the program exits
void ClassMain::OnExit(ENATIVETYPE instance)
{
}

//Callback: Input state change
void ClassMain::OnButtonDown(unsigned long bnr, EBUTTONLIST *blist)
{
	Quit();
}

//Callback: Input state change
void ClassMain::OnWheel(long direction)
{
	Quit();
}

//Callback: Input state change
void ClassMain::OnStylusDown(POINT pnt)
{
	Quit();
}

//Callback: Input state change
void ClassMain::OnStylusMove(POINT pnt)
{
	if (numstylusmove == 0)
	{
		clickx = pnt.x;
		clicky = pnt.y;
	}
	numstylusmove++;
	if (numstylusmove > 1)
		if (clickx >= 0 && clicky >= 0)
			if (ClassEMath::Abs(pnt.x - clickx) >= 4 || ClassEMath::Abs(pnt.y - clicky) >= 4)
				Quit();
}
