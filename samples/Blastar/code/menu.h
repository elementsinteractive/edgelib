// code/menu.h
// Module: the menu system
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Menu definitions                                                //
/////////////////////////////////////////////////////////////////////

#define MI_NONE         0
#define MI_STARTGAME    1
#define MI_QUITGAME     2
#define MI_AUTOFIRE     3
#define MI_STARTLEVEL   4
#define MI_MINIMIZE     5
#define MI_RESUMELEVEL  6
#define MI_QUITLEVEL    7

#define MAX_MENUCOUNT   4


/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

class ClassMenu : public ClassModule
{
	private:
		char menutext[MAX_MENUCOUNT][32];
		unsigned char menuitem[MAX_MENUCOUNT];
		unsigned char menucount;
		unsigned char curitem;
		unsigned long flashtimer;
		unsigned char HandleMenuInput(void);
		void RenderMenuItems(E2DSurface *buffer);
	public:
		ClassMenu(void);
		~ClassMenu(void);
		void OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta);
};


/////////////////////////////////////////////////////////////////////
// ClassMenu::Private                                              //
/////////////////////////////////////////////////////////////////////

//Handle the menu input system
unsigned char ClassMenu::HandleMenuInput(void)
{
	E_INPUTEVENT inputevent;
	long wheeldir = shared->ecd->input->GetWheelScroll();
	while (wheeldir != 0)
	{
		if (wheeldir < 0)
		{
			wheeldir++;
			curitem = (curitem + 1) % menucount;
		}
		else
		{
			wheeldir--;
			curitem = (curitem + menucount - 1) % menucount;
		}
		flashtimer = 512;
	}
	while(shared->ecd->input->GetKeyEvent(&inputevent))
	{
		if (inputevent.action == DIAM_MENUUP && (inputevent.type == EIE_PRESSED || inputevent.type == EIE_REPEAT))
		{
			curitem = (curitem + menucount - 1) % menucount;
			flashtimer = 512;
		}
		if (inputevent.action == DIAM_MENUDOWN && (inputevent.type == EIE_PRESSED || inputevent.type == EIE_REPEAT))
		{
			curitem = (curitem + 1) % menucount;
			flashtimer = 512;
		}
		if (inputevent.action == DIAM_MENULEFT && (inputevent.type == EIE_PRESSED || inputevent.type == EIE_REPEAT))
		{
			if (menuitem[curitem] == MI_STARTLEVEL)
			{
				if (shared->cfg.startlevel == 1)
					shared->cfg.startlevel = 9;
				else
					shared->cfg.startlevel--;
			}
			else if (menuitem[curitem] == MI_AUTOFIRE)
				shared->cfg.autoshot = !shared->cfg.autoshot;
			flashtimer = 512;
		}
		if (inputevent.action == DIAM_MENURIGHT && (inputevent.type == EIE_PRESSED || inputevent.type == EIE_REPEAT))
		{
			if (menuitem[curitem] == MI_STARTLEVEL)
			{
				if (shared->cfg.startlevel < 9)
					shared->cfg.startlevel++;
				else
					shared->cfg.startlevel = 1;
			}
			else if (menuitem[curitem] == MI_AUTOFIRE)
				shared->cfg.autoshot = !shared->cfg.autoshot;
			flashtimer = 512;
		}
		if (inputevent.type == EIE_PRESSED && (inputevent.action == DIAM_MENUCONFIRM || inputevent.action == DIAM_MENUSOFT1 || inputevent.action == DIAM_MENUSOFT2))
			return(menuitem[curitem]);
	}
	while(shared->ecd->input->GetClickEvent(&inputevent))
	{
		if (inputevent.type == EIE_PRESSED)
		{
			long ypos, yspace;
			yspace = shared->font.GetHeight() * 3 / 2;
			ypos = YRES / 2 - menucount * yspace / 2;
			if (inputevent.clicky >= ypos && inputevent.clicky < ypos + yspace * menucount)
			{
				curitem = (unsigned char)((inputevent.clicky - ypos) / yspace);
				flashtimer = 512;
				return(menuitem[curitem]);
			}
		}
	}
	return(MI_NONE);
}

//Render the menu items on the display buffer
void ClassMenu::RenderMenuItems(E2DSurface *buffer)
{
	char drawtext[32];
	COLORREF textcolor;
	long ypos, yspace;
	unsigned char ctr;
	yspace = shared->font.GetHeight() * 3 / 2;
	ypos = YRES / 2 - menucount * yspace / 2;
	for (ctr = 0; ctr < menucount; ctr++)
	{
		ClassEStd::StrCpy(drawtext, menutext[ctr]);
		if (menuitem[ctr] == MI_AUTOFIRE)
		{
			if (shared->cfg.autoshot)
				ClassEStd::StrCat(drawtext, "On");
			else
				ClassEStd::StrCat(drawtext, "Off");
		}
		else if (menuitem[ctr] == MI_STARTLEVEL)
			ClassEStd::StrCat(drawtext, shared->cfg.startlevel);
		if (ctr == curitem)
		{
			if (flashtimer % 1024 < 512)
				textcolor = E_RGBX(128 + flashtimer % 512 / 4, 128 + flashtimer % 512 / 4, 128 + flashtimer % 512 / 4);
			else
				textcolor = E_RGBX(255 - flashtimer % 512 / 4, 255 - flashtimer % 512 / 4, 255 - flashtimer % 512 / 4);
		}
		else
			textcolor = E_RGBX(160, 160, 160);
		buffer->DrawFont(buffer->GetWidth() / 2, ypos, &shared->font, drawtext, EFX_COLORKEY | EFX_COLORIZE | EFO_HCENTER, textcolor);
		ypos += yspace;
	}
}

/////////////////////////////////////////////////////////////////////
// ClassMenu::Public                                               //
/////////////////////////////////////////////////////////////////////

//ClassMenu: Constructor
ClassMenu::ClassMenu(void)
{
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_MENUCOUNT; ctr++)
		ClassEStd::StrCpy(menutext[ctr], "");
	menucount = MAX_MENUCOUNT;
	curitem = 0;
}

//ClassMenu: Destructor
ClassMenu::~ClassMenu(void)
{
}

//Module game loop
void ClassMenu::OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta)
{
	if (*modulemsg == MM_INIT)
	{
		menucount = 0;
		menuitem[menucount] = MI_STARTGAME;
		ClassEStd::StrCpy(menutext[menucount++], "Start Game");
		menuitem[menucount] = MI_AUTOFIRE;
		ClassEStd::StrCpy(menutext[menucount++], "Auto-Fire: ");
		menuitem[menucount] = MI_STARTLEVEL;
		ClassEStd::StrCpy(menutext[menucount++], "Start Level: ");
		menuitem[menucount] = MI_QUITGAME;
		ClassEStd::StrCpy(menutext[menucount++], "Quit Game");
		curitem = 0;
		*modulemsg = MM_NONE;
	}
	else if (*modulemsg == MM_FROMMODULE + MODULE_LEVEL)
	{
		menucount = 0;
		menuitem[menucount] = MI_RESUMELEVEL;
		ClassEStd::StrCpy(menutext[menucount++], "Resume Level");
		menuitem[menucount] = MI_AUTOFIRE;
		ClassEStd::StrCpy(menutext[menucount++], "Auto-Fire: ");
		#if !defined(DEVICE_GP2X) && !defined(DEVICE_IPHONE)
			menuitem[menucount] = MI_MINIMIZE;
			ClassEStd::StrCpy(menutext[menucount++], "Minimize");
		#endif
		menuitem[menucount] = MI_QUITLEVEL;
		ClassEStd::StrCpy(menutext[menucount++], "Quit Level");
		curitem = 0;
		*modulemsg = MM_NONE;
	}
	else if (*modulemsg == MM_NONE)
	{
		unsigned char result = HandleMenuInput();
		switch(result)
		{
			case MI_STARTGAME:
				*module = MODULE_LEVEL;
				*modulemsg = MM_INIT;
				break;
			case MI_QUITGAME:
				*module = MODULE_CLEANUP;
				break;
			case MI_AUTOFIRE:
				shared->cfg.autoshot = !shared->cfg.autoshot;
				break;
			case MI_STARTLEVEL:
				if (shared->cfg.startlevel < 9)
					shared->cfg.startlevel++;
				else
					shared->cfg.startlevel = 1;
				break;
			case MI_MINIMIZE:
				shared->forceminimize = true;
				break;
			case MI_RESUMELEVEL:
				*module = MODULE_LEVEL;
				*modulemsg = MM_FROMMODULE + MODULE_MENU;
				break;
			case MI_QUITLEVEL:
				*modulemsg = MM_INIT;
				break;
		}
		flashtimer += timedelta;
		buffer->BltFast(0, 0, &shared->titlebg, NULL);
		RenderMenuItems(buffer);
	}
}
