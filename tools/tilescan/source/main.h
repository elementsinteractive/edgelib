// main.h
// Definitions                                                     //
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Gui positioning
#define GUI_WIDTH         216
#define GUI_HEIGHT        108

//Checkbox types
#define CHECK_SHOWINFO      0
#define CHECK_TILEBMP       1
#define CHECK_TILEMAP       2

//Other definitions
#define MAX_CHECKS          3
#define MAX_OPENLEN      4096
#define MAX_PFILES        256
#define MAX_TILES       16384


/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

//The main class
class ClassMain : public ClassEdge
{
	private:
		ClassEDisplay *dsp;
		E2DSurface source;
		E2DSurface imgtiled;
		unsigned short *tilemap;
		unsigned short *tileref;
		bool bchecked[MAX_CHECKS];
		char programpath[512];
		char selectedfiles[MAX_OPENLEN];
		bool initonce;
		unsigned long tilesfound;
		unsigned long tilesize;
		unsigned long tilesx;
		unsigned long tilesy;
		bool SelectProcessFiles(HWND hwnd, char *caption, char *desc, char *ext);
		void PixelScan(void);
		bool ScanTiles(HWND ewnd);
		void WriteImgTiled(HWND ewnd);
		void WriteTileMap(HWND ewnd);
		bool GetRegistryValue(char *dst, char *key);
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
		void OnButtonUp(unsigned long bnr, EBUTTONLIST *blist);
		void OnWheel(long direction);
		void OnStylusUp(POINT pnt);
		void OnStylusDown(POINT pnt);
		void OnStylusDouble(POINT pnt);
		void OnStylusMove(POINT pnt);
		bool OnNativeEvent(ENATIVETYPE winhandle, ENATIVETYPE msg, ENATIVETYPE param1, ENATIVETYPE param2);
		void OnNetMessage(long id, long msg, long bufsize, unsigned char *buffer);
};


/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////

//Select files to process
bool ClassMain::SelectProcessFiles(HWND hwnd, char *caption, char *desc, char *ext)
{
	OPENFILENAME ofn;
    char filter[512], filenames[MAX_OPENLEN];
	unsigned short len, ctr;
	_getcwd(programpath, 512);
	memset(filter, 0, 512);
	memset(filenames, 0, MAX_OPENLEN);
	strcpy(filter, desc);
	strcpy(&filter[strlen(filter)], " (*");
	strcpy(&filter[strlen(filter)], ext);
	strcpy(&filter[strlen(filter)], ")");
	strcpy(&filter[strlen(filter)+1], "*");
	strcpy(&filter[strlen(filter)+2], ext);
	strcpy(filter, desc);
	strcat(filter, " (*.bmp;*.gif;*.png;*.tga)");
	strcpy(&filter[strlen(filter) + 1], "*.bmp;*.gif;*.png;*.tga");
	ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = hwnd;
	ofn.hInstance         = NULL;
	ofn.lpstrFilter       = filter;
    ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter    = 0;
	ofn.nFilterIndex      = 1;
	ofn.lpstrFile         = filenames;
	ofn.nMaxFile          = MAX_OPENLEN;
	ofn.lpstrFileTitle    = NULL;
	ofn.nMaxFileTitle     = 0;
	ofn.lpstrInitialDir   = programpath;
	ofn.lpstrTitle        = caption;
	ofn.Flags             = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_HIDEREADONLY;
	ofn.nFileOffset       = 0;
	ofn.nFileExtension    = 0;
	ofn.lpstrDefExt       = ext;
	ofn.lCustData         = 0;
	ofn.lpfnHook          = NULL;
	ofn.lpTemplateName    = NULL;
    GetOpenFileName(&ofn);
	if (filenames[0] == 0)
		return(false);
	for (ctr = 0; ctr < MAX_OPENLEN - 1; ctr++)
	{
		if (filenames[ctr] == 0 && filenames[ctr + 1] == 0)
			break;
		if (filenames[ctr] == 0)
			filenames[ctr] = '+';
	}
	for (ctr = 0; ctr < MAX_OPENLEN - 1; ctr++)
		if (filenames[ctr] == '+')
		{
			filenames[ctr] = '\\';
			break;
		}
	strcpy(programpath, filenames);
	len = strlen(programpath) - 1;
	for (ctr = len; ctr > 0; ctr--)
	{
		if (programpath[ctr] == '\\')
			break;
		programpath[ctr] = 0;
	};
	strcpy(selectedfiles, filenames);
	return(true);
}

//Scan pixels and find tiles
void ClassMain::PixelScan(void)
{
	bool duplicate;
	unsigned long xc, yc, px, py, tc, pcnt;
	tilesfound = 0;
	for (yc = 0; yc < tilesy; yc++)
		for (xc = 0; xc < tilesx; xc++)
		{
			duplicate = false;
			for (tc = 0; tc < tilesfound; tc++)
			{
				pcnt = 0;
				for (py = 0; py < tilesize; py++)
					for (px = 0; px < tilesize; px++)
						if (source.GetPixel(xc * tilesize + px, yc * tilesize + py) == source.GetPixel(tileref[tc] % tilesx * tilesize + px, tileref[tc] / tilesx * tilesize + py))
							pcnt++;
				if (pcnt == tilesize * tilesize)
				{
					if (bchecked[CHECK_TILEMAP])
						tilemap[yc * tilesx + xc] = (unsigned short)tc;
					duplicate = true;
					break;
				}
			}
			if (!duplicate)
			{
				if (bchecked[CHECK_TILEMAP])
					tilemap[yc * tilesx + xc] = (unsigned short)tilesfound;
				tileref[tilesfound] = (unsigned short)(yc * tilesx + xc);
				tilesfound++;
			}
		}
}

//Search and scan tiles
bool ClassMain::ScanTiles(HWND ewnd)
{
	if (dsp->CreateSurface(&source, selectedfiles) != E_OK)
	{
		MessageBox(ewnd, "The image file couldn't be loaded or is corrupt", "Couldn't load file", MB_OK | MB_ICONEXCLAMATION);
		return(false);
	}
	tilesx = (source.GetWidth() + tilesize - 1) / tilesize;
	tilesy = (source.GetHeight() + tilesize - 1) / tilesize;
	if (tilesx * tilesy >= MAX_TILES)
	{
		MessageBox(ewnd, "Limit of 16384 tiles exceeded, please take a smaller image or larger tile size", "Couldn't scan file", MB_OK | MB_ICONEXCLAMATION);
		return(false);
	}
	if (tileref != NULL)
	{
		ClassEMemory::DeAlloc(tileref);
		tileref = NULL;
	}
	tileref = (unsigned short *)ClassEMemory::Alloc(tilesx * tilesy * 2);
	if (tileref == NULL)
	{
		MessageBox(ewnd, "Reserving memory for the scanning has failed, please free up more memory", "Couldn't scan file", MB_OK | MB_ICONEXCLAMATION);
		return(false);
	}
	if (tilemap != NULL)
	{
		ClassEMemory::DeAlloc(tilemap);
		tilemap = NULL;
	}
	if (bchecked[CHECK_TILEMAP])
	{
		tilemap = (unsigned short *)ClassEMemory::Alloc(tilesx * tilesy * 2);
		if (tilemap == NULL)
		{
			MessageBox(ewnd, "Reserving memory for the tilemap has failed, please free up more memory", "Couldn't scan file", MB_OK | MB_ICONEXCLAMATION);
			return(false);
		}
	}
	PixelScan();
	return(true);
}

//Write seperated tiles to an image file
void ClassMain::WriteImgTiled(HWND ewnd)
{
	char fname[1024];
	unsigned long tc, ctr, len = strlen(selectedfiles);
	fname[0] = 0;
	for (ctr = 0; ctr < len; ctr++)
	{
		if (selectedfiles[ctr] == '.')
		{
			strcat(fname, "_tiled.bmp");
			break;
		}
		else
		{
			fname[ctr] = selectedfiles[ctr];
			fname[ctr + 1] = 0;
		}
	}
	if (dsp->CreateSurface(&imgtiled, tilesize * 16, (tilesfound + 15) / 16 * tilesize) != E_OK)
	{
		MessageBox(ewnd, "Reserving memory for the bitmapped tiles has failed, the file couldn't be created", "Couldn't write tiled bitmap file", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	for (tc = 0; tc < tilesfound; tc++)
	{
		RECT rc;
		SetRect(&rc, tileref[tc] % tilesx * tilesize, tileref[tc] / tilesx * tilesize, tileref[tc] % tilesx * tilesize + tilesize, tileref[tc] / tilesx * tilesize + tilesize);
		imgtiled.BltFast(tc % 16 * tilesize, tc / 16 * tilesize, &source, &rc);
	}
	imgtiled.WriteBMP(fname);
}

//Write the mapped tiledata to a textfile to include in C++
void ClassMain::WriteTileMap(HWND ewnd)
{
	ClassEFile ef;
	char fname[1024], hexstr[10];
	unsigned long tc, ctr, len = strlen(selectedfiles);
	fname[0] = 0;
	for (ctr = 0; ctr < len; ctr++)
	{
		if (selectedfiles[ctr] == '.')
		{
			strcat(fname, "_tilemap.txt");
			break;
		}
		else
		{
			fname[ctr] = selectedfiles[ctr];
			fname[ctr + 1] = 0;
		}
	}
	if (ef.New(fname))
	{
		if (tilesfound <= 256)
			ef.Write("unsigned char tilemap[] =\r\n{", strlen("unsigned char tilemap[] =\r\n{"));
		else
			ef.Write("unsigned short tilemap[] =\r\n{", strlen("unsigned short tilemap[] =\r\n{"));
		for (tc = 0; tc < tilesx * tilesy; tc++)
		{
			if (tc % 16 == 0)
				ef.Write("\r\n\x09", strlen("\r\n\x08"));
			if (tilesfound <= 256)
			{
				strcpy(hexstr, "0x00");
				hexstr[2] = (unsigned char)(tilemap[tc] / 16 % 16) + '0';
				hexstr[3] = (unsigned char)(tilemap[tc] % 16) + '0';
				for (ctr = 2; ctr < 4; ctr++)
					if (hexstr[ctr] > '9')
						hexstr[ctr] = hexstr[ctr] - '9' - 1 + 'A';
			}
			else
			{
				strcpy(hexstr, "0x0000");
				hexstr[2] = (unsigned char)(tilemap[tc] / 4096 % 16) + '0';
				hexstr[3] = (unsigned char)(tilemap[tc] / 256 % 16) + '0';
				hexstr[4] = (unsigned char)(tilemap[tc] / 16 % 16) + '0';
				hexstr[5] = (unsigned char)(tilemap[tc] % 16) + '0';
				for (ctr = 2; ctr < 6; ctr++)
					if (hexstr[ctr] > '9')
						hexstr[ctr] = hexstr[ctr] - '9' - 1 + 'A';
			}
			ef.Write(hexstr, strlen(hexstr));
			ef.Write(", ", strlen(", "));
		}
		ef.Seek(ef.Tell() - 2);
		ef.Write("\r\n};\r\n", strlen("\r\n};\r\n"));
		ef.Close();
	}
	else
		MessageBox(ewnd, "The tilemap file couldn't be created or written", "Couldn't write tilemap data", MB_OK | MB_ICONEXCLAMATION);
}

//Get a registry key value
bool ClassMain::GetRegistryValue(char *dst, char *key)
{
	HKEY keyreg, keyroot = HKEY_LOCAL_MACHINE;
	unsigned long ctr, result, rsize = MAX_ESTR, rtype = REG_BINARY, ka = KEY_ALL_ACCESS;
	char keypath[MAX_ESTR], keyname[MAX_ESTR];
	char *nkey = key;
	ClassEStd::StrCpy(dst, "");
	if (ClassEStd::StrFind(key, "HKCR") == 0)
	{
		keyroot = HKEY_CLASSES_ROOT;
		nkey = &key[5];
	}
	else if (ClassEStd::StrFind(key, "HKCU") == 0)
	{
		keyroot = HKEY_CURRENT_USER;
		nkey = &key[5];
	}
	else if (ClassEStd::StrFind(key, "HKLM") == 0)
	{
		keyroot = HKEY_LOCAL_MACHINE;
		nkey = &key[5];
	}
	else if (ClassEStd::StrFind(key, "HKU") == 0)
	{
		keyroot = HKEY_USERS;
		nkey = &key[4];
	}
	else if (ClassEStd::StrFind(key, "HKCC") == 0)
	{
		keyroot = HKEY_CURRENT_CONFIG;
		nkey = &key[5];
	}
	ClassEStd::StrCpy(keypath, nkey);
	ClassEStd::StrCpy(keyname, "");
	for (ctr = ClassEStd::StrLen(keypath) - 1; ctr > 0; ctr--)
		if (keypath[ctr] == '\\')
		{
			keypath[ctr] = 0;
			ClassEStd::StrCpy(keyname, &keypath[ctr + 1]);
			break;
		}
	if (RegCreateKeyEx(keyroot, keypath, 0, NULL, 0, ka, NULL, &keyreg, &result) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(keyreg, keyname, NULL, &rtype, (PBYTE)dst, &rsize) != ERROR_SUCCESS)
		{
			RegCloseKey(keyreg);
			return(false);
		}
		RegCloseKey(keyreg);
	}
	else
		return(false);
	return(true);
}


/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////

//ClassMain: constructor
ClassMain::ClassMain(void)
{
	initonce = false;
	programpath[0] = 0;
	bchecked[CHECK_SHOWINFO] = false;
	bchecked[CHECK_TILEBMP] = false;
	bchecked[CHECK_TILEMAP] = false;
	tilemap = NULL;
	tileref = NULL;
}

//ClassMain: destructor
ClassMain::~ClassMain(void)
{
	if (tilemap != NULL)
		ClassEMemory::DeAlloc(tilemap);
	if (tileref != NULL)
		ClassEMemory::DeAlloc(tileref);
}

//Callback: configure framework
ERESULT ClassMain::OnFrameworkConfig(EFRAMEWORKCONFIG *config)
{
	config->ffwdkey         = 'V';            //A key for fast forwarding (multiplies framerate by 2.5)
	config->slomokey        = 'C';            //A key for slow motion     (halves framerate)
	config->maxfps          = 0;              //No animation
	config->flags           = EFL_MANUALFLIP | EFL_IGNOREREDRAW; //Flip on command
	config->numconnections  = 1;              //Keep 1 active network connection
	config->smartfps        = true;           //Enable smart fps lock managing
#ifdef DEVICE_DESKTOP
	config->focusminimize   = false;          //Desktop PC: Don't minimize the application when the focus is lost
#else
	config->focusminimize   = true;           //Mobile devices: Minimize the application when the focus is lost
#endif
	config->jogtiltarrow    = true;           //Emulate the UIQ jog as left/right arrows
	config->focussuspend    = true;           //Desktop PC: suspend the application when the window focus is lost
	return(E_OK);
}

//Callback: display config
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
	ClassEStd::StrCpy(config->caption, "Tile scanner");
	config->icon = IDI_MAIN;
	config->dialogbox = IDD_MAIN;
	config->width      = GUI_WIDTH;      //Desktop PC: Custom display width
	config->height     = GUI_HEIGHT;     //Desktop PC: Custom display height
	config->videotype       = EDSP_DEFAULT;   //Choose the default display mode
	config->orientation     = DOR_NONE;       //Display orientation
	config->enginestatus    = false;          //Desktop PC: Disable the status bar and status information icon
	config->engineconsole   = false;          //Desktop PC: Disable the edge console
	config->fullscreen      = false;          //Desktop PC: A windowed application
	config->fullscreenmouse = true;           //Desktop PC: Enable the mouse cursor for fullscreen applications
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
	dsp = display;
	return(E_OK);
}

//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
	return(E_OK);
}

//Callback: Called when the program exits
void ClassMain::OnExit(ENATIVETYPE instance)
{
}

//Callback: Called when the user pressed a key or button
void ClassMain::OnButtonDown(unsigned long bnr, EBUTTONLIST *blist)
{
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
}

//Callback: Called when the user points the stylus 2 times down quickly or double clicks the left mouse button
void ClassMain::OnStylusDouble(POINT pnt)
{
}

//Callback: Called when the user moves the stylus while pressed down, or moves the mouse cursor
void ClassMain::OnStylusMove(POINT pnt)
{
}

//Callback: Called when a window message occurs
bool ClassMain::OnNativeEvent(ENATIVETYPE winhandle, ENATIVETYPE msg, ENATIVETYPE param1, ENATIVETYPE param2)
{
	char showstr[512];
	switch(msg)
	{
		case WM_COMMAND:
			switch(param1)
			{
				case IDDONE:
				case ID_FILE_QUIT:
					EndDialog((HWND)winhandle, IDABORT);
					break;
				case IDPROCESS:
				case ID_FILE_OPENENPROCESS:
					{
						int i;
						for (i = 0; i < MAX_CHECKS; i++)
							bchecked[i] = false;
						if (IsDlgButtonChecked((HWND)winhandle, IDC_SHOWINFO) == BST_CHECKED)
							bchecked[CHECK_SHOWINFO] = true;
						if (IsDlgButtonChecked((HWND)winhandle, IDC_TILEBMP)  == BST_CHECKED)
							bchecked[CHECK_TILEBMP] = true;
						if (IsDlgButtonChecked((HWND)winhandle, IDC_TILEMAP)  == BST_CHECKED)
							bchecked[CHECK_TILEMAP] = true;
						if (bchecked[CHECK_SHOWINFO] || bchecked[CHECK_TILEBMP] || bchecked[CHECK_TILEMAP])
						{
							GetDlgItemText((HWND)winhandle, IDC_TILESIZE, showstr, 20);
							tilesize = ClassEStd::StrToInt(showstr);
							if (tilesize >= 2)
							{
								if (SelectProcessFiles((HWND)winhandle, "Select image files to process", "Supported image files", ".bmp"))
								{
									if (ScanTiles((HWND)winhandle))
									{
										if (bchecked[CHECK_SHOWINFO])
										{
											strcpy(showstr, "Tiles scanned: ");
											ClassEStd::IntToStr(&showstr[strlen(showstr)], tilesx * tilesy);
											strcat(showstr, "\nUnique tiles found: ");
											ClassEStd::IntToStr(&showstr[strlen(showstr)], tilesfound);
											MessageBox((HWND)winhandle, showstr, "Scan results", MB_OK | MB_ICONINFORMATION);
										}
										if (bchecked[CHECK_TILEBMP])
											WriteImgTiled((HWND)winhandle);
										if (bchecked[CHECK_TILEMAP])
											WriteTileMap((HWND)winhandle);
									}
									else
										source.Free();
								}
							}
							else
								MessageBox((HWND)winhandle, "Tilesize is too small", "Couldn't scan tiles", MB_OK | MB_ICONEXCLAMATION);
						}
						else
							MessageBox((HWND)winhandle, "Nothing to do", "Couldn't scan tiles", MB_OK | MB_ICONEXCLAMATION);
					}
					break;
				case ID_HELP_WEBSITE:
					char regtype[MAX_ESTR];
					if (GetRegistryValue(regtype, "HKCR\\.htm\\"))
					{
						bool onkeyquote = false;
						unsigned long kctr;
						char commandkey[MAX_ESTR], regcommand[MAX_ESTR];
						ClassEStd::StrCpy(commandkey, "HKCR\\%browsertype%\\shell\\open\\command\\");
						ClassEStd::StrReplace(commandkey, "%browsertype%", regtype);
						GetRegistryValue(regcommand, commandkey);
						ClassEStd::StrCpy(commandkey, regcommand);
						for (kctr = 0; kctr < (unsigned long)ClassEStd::StrLen(commandkey); kctr++)
						{
							if (commandkey[kctr] == '"')
								onkeyquote = !onkeyquote;
							if (!onkeyquote && commandkey[kctr] == ' ')
							{
								commandkey[kctr] = 0;
								break;
							}
						}
						ClassEStd::StrCpy(regcommand, &regcommand[ClassEStd::StrLen(commandkey)]);
						if (ClassEStd::StrFind(regcommand, "%1") >= 0)
							ClassEStd::StrReplace(regcommand, "%1", "http://www.elements.nl");
						else
							ClassEStd::StrCat(regcommand, " http://www.elements.nl");
						ShellExecute((HWND)winhandle, "open", commandkey, regcommand, NULL, SW_SHOW);
					}
					return(TRUE);
				case ID_HELP_ABOUT:
					MessageBox((HWND)winhandle, "EDGELIB tile scanner\nScan an image for duplicate tiles\n\nCopyright (c) 2004-2017 Elements Interactive B.V.", "About tilescan", MB_OK);
					break;
			}
			break;
		case WM_ACTIVATE:
			if (!initonce)
				CheckRadioButton((HWND)winhandle, IDC_SHOWINFO, IDC_SHOWINFO, IDC_SHOWINFO);
			GetDlgItemText((HWND)winhandle, IDC_TILESIZE, showstr, 20);
			if (ClassEStd::StrToInt(showstr) == 0)
				if (!initonce)
					SetDlgItemText((HWND)winhandle, IDC_TILESIZE, "16");
			initonce = true;
			break;
	}
	return(false);
}

//Callback: Called when a network event occurs
void ClassMain::OnNetMessage(long id, long msg, long bufsize, unsigned char *buffer)
{
}
