// code/loader.h
// Module: loading game data on startup
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Module states                                                   //
/////////////////////////////////////////////////////////////////////

#define MSLD_INIT       0
#define MSLD_OPENPACK   1
#define MSLD_FONT       2
#define MSLD_TITLE      3
#define MSLD_GAMEBG     4
#define MSLD_SHEET      5
#define MSLD_CLOSEPACK  6
#define MSLD_DONE       7


/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

class ClassLoader : public ClassModule
{
	private:
		ClassEFile ef;
	public:
		ClassLoader(void);
		~ClassLoader(void);
		void OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta);
};


/////////////////////////////////////////////////////////////////////
// ClassLoader::Public                                             //
/////////////////////////////////////////////////////////////////////

//ClassLoader: Constructor
ClassLoader::ClassLoader(void)
{
}

//ClassLoader: Destructor
ClassLoader::~ClassLoader(void)
{
}

//Module game loop
void ClassLoader::OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta)
{
	if (*modulemsg == MM_INIT)
	{
		modulestate = MSLD_INIT;
		*modulemsg = MM_NONE;
	}
	else if (*modulemsg == MM_NONE)
	{
		RECT rc;
		POINT midpnt;
		unsigned char *filedata;
		unsigned long filedatasize;
		switch(modulestate)
		{
			case MSLD_OPENPACK:
				if (ef.Open("gfx.epk", EFOF_READ | EFOF_READONLYPATH))
				{
					if (!ef.PackAuthorize(0))
						*module = MODULE_CLEANUP;
				}
				else
					*module = MODULE_CLEANUP;
				break;
			case MSLD_FONT:
				if (ef.PackGetFileData(filedata, filedatasize, "font.bmp") == E_OK)
				{
					if (shared->ecd->dsp->CreateSurface(&shared->font, filedata, filedatasize) != E_OK)
						*module = MODULE_CLEANUP;
					shared->font.SetFontCharacters(0, 127);
					shared->font.BuildFont(false, ECOLOR_FUCHSIA);
					ClassEMemory::DeAlloc(filedata);
				}
				else
					*module = MODULE_CLEANUP;
				break;
			case MSLD_TITLE:
				if (ef.PackGetFileData(filedata, filedatasize, "title.bmp") == E_OK)
				{
					if (shared->ecd->dsp->CreateSurface(&shared->titlebg, filedata, filedatasize, EST_SYSMEMORY | EST_DITHER) != E_OK)
						*module = MODULE_CLEANUP;
					ClassEMemory::DeAlloc(filedata);
				}
				else
					*module = MODULE_CLEANUP;
				break;
			case MSLD_GAMEBG:
				if (ef.PackGetFileData(filedata, filedatasize, "background.bmp") == E_OK)
				{
					if (shared->ecd->dsp->CreateSurface(&shared->gamebg, filedata, filedatasize, EST_SYSMEMORY | EST_DITHER) != E_OK)
						*module = MODULE_CLEANUP;
					ClassEMemory::DeAlloc(filedata);
				}
				else
					*module = MODULE_CLEANUP;
				break;
			case MSLD_SHEET:
				if (ef.PackGetFileData(filedata, filedatasize, "sheet.bmp") == E_OK)
				{
					if (shared->ecd->dsp->CreateSurface(&shared->sheet, filedata, filedatasize) != E_OK)
					{
						*module = MODULE_CLEANUP;
					}
					else
					{
						shared->sheet.SetColorKey(ECOLOR_FUCHSIA);
						shared->sheet.BuildTableMask();
					}
					ClassEMemory::DeAlloc(filedata);
				}
				else
					*module = MODULE_CLEANUP;
				break;
			case MSLD_CLOSEPACK:
				ef.Close();
				break;
		}
		modulestate++;
		if (modulestate == MSLD_DONE)
		{
			*module = MODULE_MENU;
			*modulemsg = MM_INIT;
		}
		midpnt.x = buffer->GetWidth() / 2;
		midpnt.y = buffer->GetHeight() / 2;
		SetRect(&rc, midpnt.x, buffer->GetHeight() * 13 / 14, buffer->GetWidth() * 39 / 40, buffer->GetHeight() * 39 / 40);
		buffer->GradientRect(NULL, E_RGBX(0, 0, 128), E_RGBX(0, 128, 192), E_RGBX(0, 0, 255), E_RGBX(64, 160, 255), EFX_DITHER);
		buffer->DrawRect(&rc, E_RGBX(255, 255, 255));
		SetRect(&rc, rc.left, rc.top, rc.left + (rc.right - rc.left) * modulestate / MSLD_DONE, rc.bottom);
		buffer->FillRect(&rc, E_RGBX(255, 255, 255));
		buffer->DrawFont(midpnt.x, midpnt.y, &shared->font, "Loading...", EFX_COLORKEY | EFO_HCENTER | EFO_VCENTER);
	}
}
