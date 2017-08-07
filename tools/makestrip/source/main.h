// main.h
// Definitions                                                     //
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Gui positioning
#define GUI_WIDTH         216
#define GUI_HEIGHT        108

//Checkbox types
#define CHECK_BG            0
#define CHECK_SQUARE        1
#define CHECK_VERTICAL      2
#define CHECK_ALPHA         3
#define CHECK_UNUSEDPIX     4

//Other definitions
#define MAX_CHECKS          5
#define MAX_OPENLEN      4096
#define MAX_PFILES        256


/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

//The main class
class ClassMain : public ClassEdge
{
	private:
		ClassEDisplay *dsp;
		E2DSurface final;
		E2DSurfaceRGBA final_rgba;
		bool bchecked[MAX_CHECKS];
		char programpath[512];
		char selectedfiles[MAX_OPENLEN];
		bool SelectSaveFile(HWND hwnd, char *caption, char *desc, char *ext);
		bool SelectProcessFiles(HWND hwnd, char *caption, char *desc, char *ext);
		unsigned long FindSeperateFiles(char **ifile);
		void FindAnimationSize(char **ifile, unsigned long numfiles, RECT *rect);
		void BuildAnimation(char **ifile, unsigned long numfiles, RECT *rect);
		void ProcessImages(void);
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
// Pure functions
/////////////////////////////////////////////////////////////////////

//Get a registry key value
bool GetRegistryValue(char *dst, char *key)
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

//Open the save file dialog box
bool ClassMain::SelectSaveFile(HWND hwnd, char *caption, char *desc, char *ext)
{
	OPENFILENAME ofn;
    char filter[512], lastpath[512], filename[512];
	unsigned short len, ctr;
	if (programpath[0] == 0)
		_getcwd(programpath, 512);
	memset(filter, 0, 512);
	memset(lastpath, 0, 512);
	memset(filename, 0, 512);
	if (ClassEStd::StrEqual(ext, ".png"))
		strcpy(filename, "ani.png");
	else
		strcpy(filename, "ani.bmp");
	strcpy(filter,desc);
	strcpy(&filter[strlen(filter)], " (*");
	strcpy(&filter[strlen(filter)], ext);
	strcpy(&filter[strlen(filter)], ")");
	strcpy(&filter[strlen(filter)+1], "*");
	strcpy(&filter[strlen(filter)+2], ext);
	ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = filter;
    ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = 512;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = lastpath;
	ofn.lpstrTitle = caption;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = ext;
	ofn.lCustData = 0;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
    if (GetSaveFileName(&ofn) == FALSE)
		return(false);
	if (filename[0] == 0)
		return(false);
	strcpy(lastpath, filename);
	len = strlen(lastpath) - 1;
	for (ctr = len; ctr > 0; ctr--)
	{
		if (lastpath[ctr] == '\\')
			break;
		lastpath[ctr] = 0;
	}
	if (bchecked[CHECK_ALPHA])
	{
		final_rgba.WritePNG(filename);
		final_rgba.Free();
	}
	else
	{
		final.WriteBMP(filename);
		final.Free();
	}
	return(true);
}

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
	ofn.Flags             = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_HIDEREADONLY;
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

//Seperate individual files from the long selected files string
unsigned long ClassMain::FindSeperateFiles(char **ifile)
{
	unsigned long ctr, filesfound = 1, flen = strlen(selectedfiles);
	unsigned long sfpos = 0, ifpos = 0, ifnr = 0;
	for (ctr = 0; ctr < flen; ctr++)
		if (selectedfiles[ctr] == '+')
			filesfound++;
	if (filesfound > MAX_PFILES)
		filesfound = MAX_PFILES;
	for (ctr = 0; ctr < filesfound; ctr++)
		ifile[ctr] = (char *)malloc(512);
	for (ctr = 0; ctr < flen; ctr++)
	{
		if (selectedfiles[sfpos] == '+')
		{
			ifile[ifnr][ifpos] = 0;
			ifnr++;
			if (ifnr >= filesfound)
				break;
			strcpy(ifile[ifnr], programpath);
			ifpos = strlen(ifile[ifnr]);
		}
		else
		{
			ifile[ifnr][ifpos] = selectedfiles[sfpos];
			ifpos++;
		}
		sfpos++;
	}
	if (ifnr < filesfound)
		ifile[ifnr][ifpos] = 0;
	return(filesfound);
}

//Find the size of the animation image; cut off unneccesary border space
void ClassMain::FindAnimationSize(char **ifile, unsigned long numfiles, RECT *rect)
{
	E2DSurface img;
	E2DSurfaceRGBA img_rgba;
	COLORREF ck, col;
	unsigned long ctr, yctr, xctr;
	rect->left   = 10000;
	rect->top    = 10000;
	rect->right  = 0;
	rect->bottom = 0;
	for (ctr = 0; ctr < numfiles; ctr++)
	{
		if (bchecked[CHECK_ALPHA])
		{
			if (dsp->CreateSurface(&img_rgba, ifile[ctr]) == E_OK)
			{
				unsigned char alpha;
				for (yctr = 0; yctr < img_rgba.GetHeight(); yctr++)
					for (xctr = 0; xctr < img_rgba.GetWidth(); xctr++)
					{
						img_rgba.GetPixel(xctr, yctr, col, alpha);
						if (alpha > 0 || !bchecked[CHECK_UNUSEDPIX])
						{
							if ((long)xctr < rect->left)
								rect->left = xctr;
							if ((long)yctr < rect->top)
								rect->top = yctr;
							if ((long)xctr > rect->right)
								rect->right = xctr;
							if ((long)yctr > rect->bottom)
								rect->bottom = yctr;
						}
					}
			}
		}
		else
		{
			if (dsp->CreateSurface(&img, ifile[ctr]) == E_OK)
			{
				if (bchecked[CHECK_BG])
					img.SetColorKey(RGB(255, 0, 255));
				else
					img.SetColorKey(RGB(0, 0, 0));
				ck = img.GetColorKey();
				for (yctr = 0; yctr < img.GetHeight(); yctr++)
					for (xctr = 0; xctr < img.GetWidth(); xctr++)
					{
						col = img.GetPixel(xctr, yctr);
						if (col != ck || !bchecked[CHECK_UNUSEDPIX])
						{
							if ((long)xctr < rect->left)
								rect->left = xctr;
							if ((long)yctr < rect->top)
								rect->top = yctr;
							if ((long)xctr > rect->right)
								rect->right = xctr;
							if ((long)yctr > rect->bottom)
								rect->bottom = yctr;
						}
					}
			}
		}
	}
}

void PngBltFast(long x, long y, E2DSurfaceRGBA *dst, E2DSurfaceRGBA *src, RECT *rc)
{
	COLORREF col;
	unsigned char alpha;
	for (int ty = 0; ty < rc->bottom - rc->top; ty++)
		for (int tx = 0; tx < rc->right - rc->left; tx++)
		{
			src->GetPixel(rc->left + tx, rc->top + ty, col, alpha);
			dst->PutPixel(x + tx, y + ty, col, alpha);
		}
}

//Rebuild an ani.bmp/png containing the selected images in a row
void ClassMain::BuildAnimation(char **ifile, unsigned long numfiles, RECT *rect)
{
	E2DSurface img;
	E2DSurfaceRGBA img_rgba;
	unsigned long result, ctr, width, height;
	rect->right++;
	rect->bottom++;
	width  = rect->right - rect->left;
	height = rect->bottom - rect->top;
	if (bchecked[CHECK_ALPHA])
	{
		if (bchecked[CHECK_VERTICAL])
			result = dsp->CreateSurface(&final_rgba, width, height * numfiles);
		else
			result = dsp->CreateSurface(&final_rgba, width * numfiles, height);
	}
	else
	{
		if (bchecked[CHECK_VERTICAL])
			result = dsp->CreateSurface(&final, width, height * numfiles);
		else
			result = dsp->CreateSurface(&final, width * numfiles, height);
	}
	if (bchecked[CHECK_BG])
		if (!bchecked[CHECK_ALPHA])
			final.FillRect(NULL, RGB(255, 0, 255));
	if (result == E_OK)
	{
		for (ctr = 0; ctr < numfiles; ctr++)
		{
			if (bchecked[CHECK_ALPHA])
			{
				if (dsp->CreateSurface(&img_rgba, ifile[ctr]) == E_OK)
				{
					if (bchecked[CHECK_VERTICAL])
						PngBltFast(0, ctr * height, &final_rgba, &img_rgba, rect);
					else
						PngBltFast(ctr * width, 0, &final_rgba, &img_rgba, rect);
				}
			}
			else
			{
				if (dsp->CreateSurface(&img, ifile[ctr]) == E_OK)
				{
					if (bchecked[CHECK_VERTICAL])
						final.BltFast(0, ctr * height, &img, rect);
					else
						final.BltFast(ctr * width, 0, &img, rect);
				}
			}
		}
	}
}

//Open the images and collect screen/image size information, then reopen them and save the images to a new file
void ClassMain::ProcessImages(void)
{
	RECT rect;
	char *ifile[MAX_PFILES];
	unsigned long ctr, filesfound, diff;
	filesfound = FindSeperateFiles(ifile);
	FindAnimationSize(ifile, filesfound, &rect);
	if (rect.top > rect.bottom || rect.left > rect.right)
		return;
	if (bchecked[CHECK_SQUARE])
	{
		diff = abs((rect.bottom - rect.top) - (rect.right - rect.left));
		if (diff > 0)
		{
			if (rect.bottom - rect.top > rect.right - rect.left)
			{
				rect.left   -= diff / 2;
				rect.right  += diff - diff / 2;
			}
			else
			{
				rect.top    -= diff / 2;
				rect.bottom += diff - diff / 2;
			}
		}
	}
	BuildAnimation(ifile, filesfound, &rect);
	for (ctr = 0; ctr < filesfound; ctr++)
		free(ifile[ctr]);
}


/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////

//ClassMain: constructor
ClassMain::ClassMain(void)
{
	programpath[0] = 0;
	bchecked[CHECK_BG] = false;
	bchecked[CHECK_SQUARE] = false;
	bchecked[CHECK_VERTICAL] = false;
	bchecked[CHECK_ALPHA] = false;
	bchecked[CHECK_UNUSEDPIX] = true;
}

//ClassMain: destructor
ClassMain::~ClassMain(void)
{
}

//Callback: configure framework
ERESULT ClassMain::OnFrameworkConfig(EFRAMEWORKCONFIG *config)
{
	config->maxfps          = 0;              //No animation
	config->flags           = EFL_MANUALFLIP | EFL_IGNOREREDRAW; //Flip on command
	return(E_OK);
}

//Callback: configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
	ClassEStd::StrCpy(config->caption, TEXT("Make strip"));
	config->icon            = IDI_MAIN;
	config->dialogbox       = IDD_MAIN;
	config->width           = GUI_WIDTH;      //Desktop PC: Custom display width
	config->height          = GUI_HEIGHT;     //Desktop PC: Custom display height
	config->videotype       = EDSP_DEFAULT;   //Choose the default display mode
	config->emulvideotype   = EDSP_TRUE32;    //Emulate 32-bits to preserve colors
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
	if (ecd.dat->commandline[0])
	{
		char outputname[2048];
		bool incomment = false, showhelp = false, allowparse;
		unsigned short ctr, ctr2, fpos = 0, dpos = 0, len = strlen(ecd.dat->commandline);
		outputname[0] = 0;
		for (ctr = 0; ctr < len; ctr++)
		{
			if (ecd.dat->commandline[ctr] == '\"')
			{
				if (incomment)
					incomment = false;
				else
					incomment = true;
			}
			if (!incomment && ctr < len - 1)
			{
				allowparse = false;
				if (ctr == 0)
					allowparse = true;
				else if (ecd.dat->commandline[ctr - 1] == ' ')
					allowparse = true;
				if (ecd.dat->commandline[ctr] == '-' && allowparse)
				{
					if (ecd.dat->commandline[ctr + 1] == 'm')
						bchecked[CHECK_BG] = true;
					if (ecd.dat->commandline[ctr + 1] == 's')
						bchecked[CHECK_SQUARE] = true;
					if (ecd.dat->commandline[ctr + 1] == 'v')
						bchecked[CHECK_VERTICAL] = true;
					if (ecd.dat->commandline[ctr + 1] == 'a')
						bchecked[CHECK_ALPHA] = true;
					if (ecd.dat->commandline[ctr + 1] == 'u')
						bchecked[CHECK_UNUSEDPIX] = false;
					if (ctr < len - 4)
					{
						if (ecd.dat->commandline[ctr + 1] == 'o')
							if (ecd.dat->commandline[ctr + 2] == '=')
								for (ctr2 = ctr + 3; ctr2 < len; ctr2++)
								{
									if (ecd.dat->commandline[ctr2] == ' ')
										break;
									outputname[fpos] = ecd.dat->commandline[ctr2];
									outputname[fpos + 1] = 0;
									fpos++;
								}
						if (ecd.dat->commandline[ctr + 1] == 'h')
							if (ecd.dat->commandline[ctr + 2] == 'e')
								if (ecd.dat->commandline[ctr + 3] == 'l')
									if (ecd.dat->commandline[ctr + 4] == 'p')
										showhelp = true;
					}
					for (ctr2 = ctr; ctr2 < len; ctr2++)
					{
						if (ecd.dat->commandline[ctr2] == ' ')
							break;
						ecd.dat->commandline[ctr2] = ' ';
					}
				}
			}
		}
		incomment = false;
		for (ctr = 0; ctr < len; ctr++)
		{
			if (ecd.dat->commandline[ctr] == '\"')
			{
				if (incomment)
					incomment = false;
				else
					incomment = true;
			}
			else
			{
				allowparse = true;
				if (ctr == 0)
					allowparse = false;
				else if (ecd.dat->commandline[ctr - 1] == ' ' || ecd.dat->commandline[ctr - 1] == '+')
					allowparse = false;
				if (allowparse || ecd.dat->commandline[ctr] != ' ')
				{
					selectedfiles[dpos] = ecd.dat->commandline[ctr];
					if (selectedfiles[dpos] == ' ' && !incomment && allowparse)
						selectedfiles[dpos] = '+';
					dpos++;
				}
			}
		}
		for (ctr = dpos; ctr > 0; ctr--)
		{
			if (selectedfiles[ctr] != ' ' && selectedfiles[ctr] != '+' && selectedfiles[ctr] != 0)
				break;
			selectedfiles[ctr] = 0;
		}
		if (!showhelp)
		{
			ProcessImages();
			if (outputname[0])
			{
				final.WriteBMP(outputname);
				final.Free();
			}
			else
			{
				if (bchecked[CHECK_ALPHA])
					SelectSaveFile((HWND)GetWindowHandle(), "Select animation file to write", "PNG files", ".png");
				else
					SelectSaveFile((HWND)GetWindowHandle(), "Select animation file to write", "Bitmap files", ".bmp");
			}
		}
		else
			MessageBox((HWND)GetWindowHandle(), "EDGELIB Strip Maker (version 2.0)\nCreate a single animation image from separate image files\n\nCopyright (c) 2004-2017 Elements Interactive B.V.\n\nCommandline usage: makestrip.exe [-m] [-s] [-v] [-o=<output>] <input1> <input2> [....<inputn>]\n\nOptions:\n-m Magenta background\n-s Square images\n-v Vertical build\n-a Include alpha channel\n-u Don't check for unused pixels\n-o Output animation file", "About", MB_OK);
		return(E_ERROR);
	}
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
	HWND ewnd = (HWND)winhandle;
	switch(msg)
	{
		case WM_COMMAND:
			switch(param1)
			{
				case IDDONE:
				case ID_FILE_QUIT:
					EndDialog(ewnd, IDABORT);
					break;
				case IDPROCESS:
				case ID_FILE_OPENENPROCESS:
					{
						int i;
						for (i = 0; i < MAX_CHECKS; i++)
							bchecked[i] = false;
						if (IsDlgButtonChecked(ewnd, IDC_MAGENTA)  == BST_CHECKED)
							bchecked[CHECK_BG] = true;
						if (IsDlgButtonChecked(ewnd, IDC_SQUARE)   == BST_CHECKED)
							bchecked[CHECK_SQUARE] = true;
						if (IsDlgButtonChecked(ewnd, IDC_VERTICAL) == BST_CHECKED)
							bchecked[CHECK_VERTICAL] = true;
						if (IsDlgButtonChecked(ewnd, IDC_ALPHA) == BST_CHECKED)
							bchecked[CHECK_ALPHA] = true;
						if (IsDlgButtonChecked(ewnd, IDC_UNUSEDPIX) != BST_CHECKED)
							bchecked[CHECK_UNUSEDPIX] = false;
						if (SelectProcessFiles(ewnd, "Select image files to process", "Supported image files", ".bmp"))
						{
							ProcessImages();
							if (bchecked[CHECK_ALPHA])
								SelectSaveFile(ewnd, "Select animation file to write", "PNG files", ".png");
							else
								SelectSaveFile(ewnd, "Select animation file to write", "Bitmap files", ".bmp");
						}
					}
					break;
				case ID_HELP_EDGELIBWEBSITE:
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
						ShellExecute(ewnd, "open", commandkey, regcommand, NULL, SW_SHOW);
					}
					return(true);
				case ID_HELP_ABOUT:
					MessageBox(ewnd, "EDGELIB Strip Maker (version 2.0)\nCreate a single animation image from separate image files\n\nCopyright (c) 2004-2017 Elements Interactive B.V.\n\nCommandline usage: makestrip.exe [-m] [-s] [-v] [-o=<output>] <input1> <input2> [....<inputn>]\n\nOptions:\n-m Magenta background\n-s Square images\n-v Vertical build\n-a Include alpha channel\n-u Don't check for unused pixels\n-o Output animation file", "About", MB_OK);
					break;
			}
			break;
		case WM_INITDIALOG:
			CheckDlgButton(ewnd, IDC_UNUSEDPIX, BST_CHECKED);
			break;
	}
	return(false);
}

//Callback: Called when a network event occurs
void ClassMain::OnNetMessage(long id, long msg, long bufsize, unsigned char *buffer)
{
}
