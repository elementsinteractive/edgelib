// interface.h
// Handles the windows GUI interface
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

#define NUM_COLUMNS       7
#define MAX_FILESTR   65536
#define MAX_ARGCOUNT    256
#define MAX_PACKFILES 32768

#define SORT_NUMERIC      0
#define SORT_SEP_NUMERIC  1
#define SORT_ALPHANUMERIC 2
#define SORT_DATE         3

#define PS_IDLE           0
#define PS_LOADED         1
//Program definitions

typedef struct
{
	char coltext[NUM_COLUMNS][64];
}RESITEM;
//A column item

typedef struct
{
	char sep_nr[2];
	char sep_dt[2];
	char sep_tm[2];
	unsigned long dateformat;
	unsigned long timeformat;
}ELOCALEINFO;
//Locale information

typedef struct
{
	unsigned char compresstype;
}EPRGCONFIG;
//Program configuration

char vlocalfile[MAX_EFILESTR];
char vglobalfile[MAX_EFILESTR];
char vendorstr[20][5];
unsigned long vendorcode[20];
unsigned long currentvendor;
unsigned long vendorfound;
unsigned long localfound;
//Vendor information

ELOCALEINFO ilocale;
EPRGCONFIG iconfig;
char programpath[MAX_FILESTR];
char loadedfile[MAX_FILESTR];
char extractpath[MAX_FILESTR];
char idtext[MAX_ESTR];
char codetext[MAX_ESTR];
unsigned char sorttype[NUM_COLUMNS];
unsigned long argstart[MAX_ARGCOUNT];
unsigned long argend[MAX_ARGCOUNT];
HINSTANCE ghinst;
RESITEM  *resitem = NULL;
ClassEFile packf1;
ClassEFile packf2;
ClassEFile *epack = &packf1;
HANDLE hstdoutput;
unsigned long selectedvendor, argcount, commandqueue, lastpackerror;
unsigned char programstate, sortcolumn;
bool sortascending, commandbusy;
char *cmdargs;
//Globals

const char supportcmd[] = ";/?;-h;--help;-v;--version;-q;--quiet;-c;--autoclose;-a;--add;-e;--extract;-d;--delete;-l;--list;-n;--vendor;-f;--force;-o;--compression;";
//Supported commands

//Prototypes
void BuildList(HWND parenthwnd, bool norebuild = false);
void SortListView(HWND hwnd, unsigned char column);
BOOL CALLBACK AllMsgWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);

/////////////////////////////////////////////////////////////////////
// Subclassed controls
/////////////////////////////////////////////////////////////////////

//Subclass fix for keyboard shortcuts
BOOL CALLBACK SubClass_FileList(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	WNDPROC ParentClass = (WNDPROC)GetWindowLong(hwnd, GWL_USERDATA);
	switch(wmsg)
	{
		case WM_GETDLGCODE:
			return(DLGC_WANTALLKEYS);
		case WM_KEYDOWN:
			if (wparam == VK_DELETE)
				SendMessage(GetParent(hwnd), WM_COMMAND, ID_PACK_DELETE, 0);
			if (wparam == VK_INSERT)
				SendMessage(GetParent(hwnd), WM_COMMAND, ID_PACK_ADD, 0);
			if (wparam == VK_APPS)
				SendMessage(GetParent(hwnd), WM_COMMAND, IDC_QUICKMENU, 0);
			if (GetAsyncKeyState(VK_CONTROL))
			{
				if (wparam == 'N')
					SendMessage(GetParent(hwnd), WM_COMMAND, ID_FILE_NEWF, 0);
				if (wparam == 'O')
					SendMessage(GetParent(hwnd), WM_COMMAND, ID_FILE_OPENF, 0);
				if (wparam == 'A')
					SendMessage(GetParent(hwnd), WM_COMMAND, IDC_SELECTALL, 0);
			}
			break;
	}
	return(CallWindowProc(ParentClass, hwnd, wmsg, wparam, lparam));
}

//Subclass fix for keyboard shortcuts
BOOL CALLBACK SubClass_AppWindow(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	WNDPROC ParentClass = (WNDPROC)GetWindowLong(hwnd, GWL_USERDATA);
	switch(wmsg)
	{
		case WM_GETDLGCODE:
			return(DLGC_WANTALLKEYS);
		case WM_KEYDOWN:
			if (wparam == VK_DELETE)
				SendMessage(hwnd, WM_COMMAND, ID_PACK_DELETE, 0);
			if (wparam == VK_INSERT)
				SendMessage(hwnd, WM_COMMAND, ID_PACK_ADD, 0);
			if (GetAsyncKeyState(VK_CONTROL))
			{
				if (wparam == 'N')
					SendMessage(hwnd, WM_COMMAND, ID_FILE_NEWF, 0);
				if (wparam == 'O')
					SendMessage(hwnd, WM_COMMAND, ID_FILE_OPENF, 0);
				if (wparam == 'A')
					SendMessage(hwnd, WM_COMMAND, IDC_SELECTALL, 0);
			}
			break;
	}
	return(CallWindowProc(ParentClass, hwnd, wmsg, wparam, lparam));
}

/////////////////////////////////////////////////////////////////////
// Functions
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

//Read the number seperator symbol from locale info
void ReadLocaleInfo(void)
{
	TCHAR bufdata[12];
	long count;
	ilocale.sep_nr[0] = '.';
	ilocale.sep_nr[1] = 0;
	ilocale.sep_dt[0] = '-';
	ilocale.sep_dt[1] = 0;
	ilocale.sep_tm[0] = ':';
	ilocale.sep_tm[1] = 0;
	ilocale.dateformat = 2;
	ilocale.timeformat = 0;
	count = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, bufdata, 12);
	if (count >= 1)
		ilocale.sep_nr[0] = bufdata[0];
	count = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDATE, bufdata, 12);
	if (count >= 1)
		ilocale.sep_dt[0] = bufdata[0];
	count = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIME, bufdata, 12);
	if (count >= 1)
		ilocale.sep_tm[0] = bufdata[0];
	count = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ILDATE, bufdata, 12);
	if (count >= 1)
		ilocale.dateformat = ClassEStd::StrToInt(bufdata);
	count = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, bufdata, 12);
	if (count >= 1)
		ilocale.timeformat = ClassEStd::StrToInt(bufdata);
	if (ilocale.dateformat > 2)
		ilocale.dateformat = 2;
	if (ilocale.timeformat > 1)
		ilocale.timeformat = 1;
}

//Choose a folder
bool BrowseFolder(HWND hwnd, char *newpath)
{
	BROWSEINFO browseinfo;
	LPITEMIDLIST pmidlist;
	LPMALLOC pmalloc;
	char caption[512];
	strcpy(newpath, "");
	strcpy(caption, "Select the folder to extract to ...");
	SHGetMalloc(&pmalloc);
	browseinfo.hwndOwner      = hwnd;
	browseinfo.pidlRoot       = NULL;
	browseinfo.pszDisplayName = newpath;
	browseinfo.lpszTitle      = caption;
	browseinfo.ulFlags        = BIF_RETURNONLYFSDIRS | 64;
	browseinfo.lpfn           = NULL;
	browseinfo.lParam         = 0;
	browseinfo.iImage         = NULL;
	pmidlist = SHBrowseForFolder(&browseinfo);
	if (pmidlist != NULL)
	{
		SHGetPathFromIDList(pmidlist, newpath);
		pmalloc->Free(pmidlist);
		pmalloc->Release();
		return(true);
	}
	else
	{
		pmalloc->Release();
		return(false);
	}
}

//Select a file to create and save the last path
bool SelectNewFile(HWND hwnd, char *caption, char *desc, char *ext)
{
	OPENFILENAME ofn;
	char filter[MAX_FILESTR], filename[MAX_FILESTR];
	unsigned short len, ctr;
	if (programpath[0] == 0)
		_getcwd(programpath, MAX_FILESTR);
	memset(filter  , 0, MAX_FILESTR);
	memset(filename, 0, MAX_FILESTR);
	strcpy(filter, desc);
	strcpy(&filter[strlen(filter)], " (*");
	strcpy(&filter[strlen(filter)], ext);
	strcpy(&filter[strlen(filter)], ")");
	strcpy(&filter[strlen(filter)+1], "*");
	strcpy(&filter[strlen(filter)+2], ext);
	if (strcmp(ext, ".epk") == 0)
	{
		unsigned long npos = 0;
		strcpy(filter, desc);
		strcat(filter, " (*");
		strcat(filter, ext);
		strcat(filter, ")");
		npos = strlen(filter) + 1;
		strcpy(&filter[npos], "*");
		strcat(&filter[npos], ext);
		npos += strlen(&filter[npos]) + 1;
		strcpy(&filter[npos], "All files (*.*)");
		npos += strlen(&filter[npos]) + 1;
		strcat(&filter[npos], "*.*");
		npos += strlen(&filter[npos]) + 1;
		filter[npos] = 0;
	}
	ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = hwnd;
	ofn.hInstance         = NULL;
	ofn.lpstrFilter       = filter;
    ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter    = 0;
	ofn.nFilterIndex      = 1;
	ofn.lpstrFile         = filename;
	ofn.nMaxFile          = MAX_FILESTR;
	ofn.lpstrFileTitle    = NULL;
	ofn.nMaxFileTitle     = 0;
	ofn.lpstrInitialDir   = programpath;
	ofn.lpstrTitle        = caption;
	ofn.Flags             = OFN_PATHMUSTEXIST;
	ofn.nFileOffset       = 0;
	ofn.nFileExtension    = 0;
	ofn.lpstrDefExt       = ext;
	ofn.lCustData         = 0;
	ofn.lpfnHook          = NULL;
	ofn.lpTemplateName    = NULL;
    GetOpenFileName(&ofn);
	if (filename[0] == 0)
		return(false);
	strcpy(programpath, filename);
	len = strlen(programpath) - 1;
	for (ctr = len; ctr > 0; ctr--)
	{
		if (programpath[ctr] == '\\')
			break;
		programpath[ctr] = 0;
	};
	strcpy(loadedfile, filename);
	return(true);
}

//Select a file to open and save the last path
bool SelectOpenFile(HWND hwnd, char *caption, char *desc, char *ext)
{
	OPENFILENAME ofn;
	char filter[MAX_FILESTR], filename[MAX_FILESTR];
	unsigned short len, ctr;
	if (programpath[0] == 0)
		_getcwd(programpath, MAX_FILESTR);
	memset(filter  , 0, MAX_FILESTR);
	memset(filename, 0, MAX_FILESTR);
	strcpy(filter, desc);
	strcpy(&filter[strlen(filter)], " (*");
	strcpy(&filter[strlen(filter)], ext);
	strcpy(&filter[strlen(filter)], ")");
	strcpy(&filter[strlen(filter)+1], "*");
	strcpy(&filter[strlen(filter)+2], ext);
	if (strcmp(ext, ".epk") == 0)
	{
		unsigned long npos = 0;
		strcpy(filter, desc);
		strcat(filter, " (*");
		strcat(filter, ext);
		strcat(filter, ")");
		npos = strlen(filter) + 1;
		strcpy(&filter[npos], "*");
		strcat(&filter[npos], ext);
		npos += strlen(&filter[npos]) + 1;
		strcpy(&filter[npos], "All files (*.*)");
		npos += strlen(&filter[npos]) + 1;
		strcat(&filter[npos], "*.*");
		npos += strlen(&filter[npos]) + 1;
		filter[npos] = 0;
	}
	ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = hwnd;
	ofn.hInstance         = NULL;
	ofn.lpstrFilter       = filter;
    ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter    = 0;
	ofn.nFilterIndex      = 1;
	ofn.lpstrFile         = filename;
	ofn.nMaxFile          = MAX_FILESTR;
	ofn.lpstrFileTitle    = NULL;
	ofn.nMaxFileTitle     = 0;
	ofn.lpstrInitialDir   = programpath;
	ofn.lpstrTitle        = caption;
	ofn.Flags             = OFN_FILEMUSTEXIST;
	ofn.nFileOffset       = 0;
	ofn.nFileExtension    = 0;
	ofn.lpstrDefExt       = ext;
	ofn.lCustData         = 0;
	ofn.lpfnHook          = NULL;
	ofn.lpTemplateName    = NULL;
    GetOpenFileName(&ofn);
	if (filename[0] == 0)
		return(false);
	strcpy(programpath, filename);
	len = strlen(programpath) - 1;
	for (ctr = len; ctr > 0; ctr--)
	{
		if (programpath[ctr] == '\\')
			break;
		programpath[ctr] = 0;
	};
	strcpy(loadedfile, filename);
	return(true);
}

//Select files to add and save the last path
bool SelectAddFiles(HWND hwnd, char *caption, char *desc, char *ext)
{
	OPENFILENAME ofn;
	char filter[MAX_FILESTR], filename[MAX_FILESTR];
	unsigned long len, subctr, ctr, npos = 0;
	if (programpath[0] == 0)
		_getcwd(programpath, MAX_FILESTR);
	memset(filter  , 0, MAX_FILESTR);
	memset(filename, 0, MAX_FILESTR);
	strcpy(filter, desc);
	strcpy(&filter[strlen(filter)], " (*");
	strcpy(&filter[strlen(filter)], ext);
	strcpy(&filter[strlen(filter)], ")");
	npos += strlen(filter) + 1;
	strcpy(&filter[npos], "*");
	strcat(&filter[npos], ext);
	npos += strlen(&filter[npos]) + 1;
	filter[npos] = 0;
	ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = hwnd;
	ofn.hInstance         = NULL;
	ofn.lpstrFilter       = filter;
    ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter    = 0;
	ofn.nFilterIndex      = 1;
	ofn.lpstrFile         = filename;
	ofn.nMaxFile          = MAX_FILESTR;
	ofn.lpstrFileTitle    = NULL;
	ofn.nMaxFileTitle     = 0;
	ofn.lpstrInitialDir   = programpath;
	ofn.lpstrTitle        = caption;
	ofn.Flags             = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	ofn.nFileOffset       = 0;
	ofn.nFileExtension    = 0;
	ofn.lpstrDefExt       = ext;
	ofn.lCustData         = 0;
	ofn.lpfnHook          = NULL;
	ofn.lpTemplateName    = NULL;
    GetOpenFileName(&ofn);
	if (filename[0] == 0)
		return(false);
	ClassEStd::StrCpy(programpath, filename);
	if (filename[ClassEStd::StrLen(filename) + 1] != 0)
		ClassEStd::StrCat(programpath, "\\");
	ClassEFile::GetFolderName(programpath);
	ctr = 0;
	len = strlen(filename);
	if (filename[len + 1] != 0)
		ctr = len + 1;
	for (subctr = ctr; subctr < len; subctr++)
	{
		if (filename[subctr] == '\\')
			ctr = subctr + 1;
	}
	for (subctr = 0; subctr < MAX_FILESTR && ctr < MAX_FILESTR; subctr++)
	{
		loadedfile[subctr] = filename[ctr];
		if (ctr > 0)
			if (filename[ctr - 1] == 0 && filename[ctr] == 0)
				break;
		ctr++;
	}
	return(true);
}

//Custom message box with all options
unsigned long AllMessageBox(HWND hwnd, char *text, char *caption)
{
	char mstr[8192];
	strcpy(mstr, caption);
	strcpy(&mstr[1024], text);
	return(DialogBoxParam(ghinst, MAKEINTRESOURCE(IDD_YESNOALL), hwnd, AllMsgWinProc, (LPARAM)mstr));
}

//Add files to the resource pack
void AddFiles(HWND hwnd, char *filestr)
{
	ERESULT result;
	char filename[MAX_EFILESTR];
	char tmpstr[MAX_ESTR], msgstr[MAX_ESTR];
	unsigned long len, ctr, dlgresult, totalfiles = 0, currentfile = 0;
	unsigned char overwrite = 0;
	if (loadedfile[0] == 0)
		return;
	for (ctr = 0; ctr < MAX_FILESTR; ctr++)
	{
		if (loadedfile[ctr] == 0)
		{
			totalfiles++;
			if (loadedfile[ctr + 1] == 0)
				break;
		}
	}
	if (totalfiles == 1)
		SetWindowText(GetDlgItem(hwnd, IDC_STATUSBAR), "Adding file...");
	else
		SetWindowText(GetDlgItem(hwnd, IDC_STATUSBAR), "Adding files...");
	ctr = 0;
	while(ctr < MAX_FILESTR)
	{
		bool alreadyfound = false;
		if (loadedfile[ctr] == 0)
			break;
		strcpy(filename, programpath);
		strcat(filename, &loadedfile[ctr]);
		len = strlen(&loadedfile[ctr]);
		if (overwrite == 1)
			result = epack->PackAddFile(filename, true, iconfig.compresstype);
		else
			result = epack->PackAddFile(filename, false, iconfig.compresstype);
		if (result == E_ALREADYFOUND)
		{
			dlgresult = IDNO;
			if (overwrite == 0)
			{
				strcpy(tmpstr, filename);
				ClassEFile::GetFileName(tmpstr);
				strcpy(msgstr, "File ");
				strcat(msgstr, tmpstr);
				strcat(msgstr, " already exists. Overwrite?");
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
				dlgresult = AllMessageBox(hwnd, msgstr, "Adding file(s)");
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_WAIT)));
				if (dlgresult == IDYESALL)
					overwrite = 1;
				else if (dlgresult == IDNOALL)
					overwrite = 2;
			}
			if ((overwrite == 1 && overwrite != 2) || dlgresult == IDYES)
				result = epack->PackAddFile(filename, true, iconfig.compresstype);
			else
				alreadyfound = true;
		}
		if (result != E_OK && result != E_ALREADYFOUND)
		{
			strcpy(msgstr, "Unable to add file: ");
			strcat(msgstr, filename);
			if (MessageBox(hwnd, msgstr, "Add file error", MB_OKCANCEL | MB_ICONEXCLAMATION) == IDCANCEL)
				break;
		}
		if (totalfiles)
		{
			currentfile++;
			SendMessage(GetDlgItem(hwnd, IDC_PROGRESS), PBM_SETPOS, currentfile * 100 / totalfiles, 0);
			if (!alreadyfound)
				Sleep(10);
		}
		currentfile++;
		ctr += len + 1;
	}
	SendMessage(GetDlgItem(hwnd, IDC_PROGRESS), PBM_SETPOS, 0, 0);
}

//Load a pack file
bool LoadPackFile(char *filename, HWND hwnd)
{
	char msgstr[MAX_ESTR];
	unsigned long ctr, newvendor = currentvendor;
	ClassEFile *newpack = &packf2;
	if (newpack == epack)
		newpack = &packf1;
	if (newpack->Open(filename, 0))
	{
		char fvendor[8];
		if (!newpack->PackValid())
		{
			MessageBox(hwnd, "This file is not a valid EDGELIB Pack file.", "File error", MB_ICONEXCLAMATION | MB_OK);
			newpack->Close();
			return(false);
		}
		newpack->PackGetVendor(fvendor);
		for (ctr = 0; ctr < vendorfound; ctr++)
			if (ClassEStd::StrEqual(fvendor, vendorstr[ctr]))
			{
				newvendor = ctr;
				break;
			}
		if (!newpack->PackAuthorize(vendorcode[newvendor]))
		{
			MessageBox(hwnd, "This file is locked by a vendor signature you aren't authorized to open.", "Protected File", MB_ICONEXCLAMATION | MB_OK);
			newpack->Close();
			return(false);
		}
	}
	else
	{
		MessageBox(hwnd, "An error has occurred when opening the file, please try it again.", "File error", MB_ICONEXCLAMATION | MB_OK);
		return(false);
	}
	epack->Close();
	currentvendor = newvendor;
	epack = newpack;
	if (hwnd != NULL)
	{
		strcpy(msgstr, filename);
		ClassEFile::GetFileName(msgstr);
		strcat(msgstr, " - ");
		strcat(msgstr, PRODUCTNAME);
		strcat(msgstr, " '");
		strcat(msgstr, vendorstr[currentvendor]);
		strcat(msgstr, "'");
		SetWindowText(hwnd, msgstr);
	}
	EnableWindow(GetDlgItem(hwnd, IDC_FILELIST), TRUE);
	programstate = PS_LOADED;
	BuildList(hwnd);
	SortListView(GetDlgItem(hwnd, IDC_FILELIST), sortcolumn);
	return(true);
}

//Create a new pack file
bool NewPackFile(char *filename, HWND hwnd)
{
	ERESULT result;
	char msgstr[MAX_ESTR];
	unsigned long ctr, newvendor = currentvendor;
	ClassEFile *newpack = &packf2;
	if (newpack == epack)
		newpack = &packf1;
	for (ctr = 0; ctr < vendorfound; ctr++)
		if (ClassEStd::StrEqual(vendorstr[ctr], "FREE", false))
		{
			newvendor = ctr;
			break;
		}
	result = newpack->PackNew(filename, vendorstr[newvendor]);
	if (result == E_ALREADYFOUND)
	{
		if (MessageBox(hwnd, "File already exists, do you want to open it instead?", "Open file", MB_OK | MB_YESNO) == IDYES)
			return(LoadPackFile(filename, hwnd));
	}
	else if (result != E_OK)
	{
		MessageBox(hwnd, "An error has occurred when creating the file, please try again.", "File error", MB_ICONEXCLAMATION | MB_OK);
		return(false);
	}
	epack->Close();
	currentvendor = newvendor;
	epack = newpack;
	if (hwnd != NULL)
	{
		strcpy(msgstr, filename);
		ClassEFile::GetFileName(msgstr);
		strcat(msgstr, " - ");
		strcat(msgstr, PRODUCTNAME);
		strcat(msgstr, " '");
		strcat(msgstr, vendorstr[currentvendor]);
		strcat(msgstr, "'");
		SetWindowText(hwnd, msgstr);
	}
	EnableWindow(GetDlgItem(hwnd, IDC_FILELIST), TRUE);
	programstate = PS_LOADED;
	BuildList(hwnd);
	SortListView(GetDlgItem(hwnd, IDC_FILELIST), sortcolumn);
	return(true);
}

//Close a pack file
bool ClosePackFile(HWND hwnd)
{
	unsigned long ctr;
	char msgstr[MAX_ESTR];
	epack->Close();
	for (ctr = 0; ctr < vendorfound; ctr++)
		if (ClassEStd::StrEqual(vendorstr[ctr], "FREE", false))
		{
			currentvendor = ctr;
			break;
		}
	if (hwnd != NULL)
	{
		strcpy(msgstr, PRODUCTNAME);
		strcat(msgstr, " '");
		strcat(msgstr, vendorstr[currentvendor]);
		strcat(msgstr, "'");
		SetWindowText(hwnd, msgstr);
	}
	EnableWindow(GetDlgItem(hwnd, IDC_FILELIST), FALSE);
	programstate = PS_IDLE;
	BuildList(hwnd);
	return(true);
}

//Convert string to number (with symbols)
long StrToNr(char *str)
{
	long v = 0;
	unsigned char ctr, len = strlen(str), negnr = 1;
	for (ctr = 0; ctr < len; ctr++)
	{
		if (str[ctr] >= '0' && str[ctr] <= '9')
		{
			v *= 10;
			v += str[ctr] - '0';
		}
		else if (str[ctr] == '-')
			negnr = -1;
	}
	return(v * negnr);
}

//Convert a number to a string (including symbols)
void NrToStr(long nr, char *str)
{
	bool prevdigit = false;
	long t_value;
	unsigned char ctr, subctr;
	if (nr < 0)
		strcpy(str, "-");
	else
		strcpy(str, "");
	
	for (ctr = 0; ctr < 6; ctr++)
	{
		t_value = nr;
		for (subctr = 0; subctr < 5 - ctr; subctr++)
			t_value /= 1000;
		if (t_value > 0 || ctr == 5)
		{
			unsigned long pos = strlen(str);
			if (t_value / 100)
			{
				str[pos] = t_value % 1000 / 100 % 10 + '0';
				pos++;
			}
			if (t_value / 10)
			{
				str[pos] = t_value % 1000 / 10 % 10 + '0';
				pos++;
			}
			if (t_value || ctr == 5)
			{
				str[pos] = t_value % 1000 / 1 % 10 + '0';
				pos++;
			}
			str[pos] = 0;
			if (ctr < 5)
				strcat(str, ilocale.sep_nr);
		}
	}
}

//Convert a timestamp to a string
void TimestampToStr(ETYPE_TIMESTAMP timestamp, char *str)
{
	E_DATETIME dt;
	bool alttime = false;
	unsigned long daypos, monthpos, yearpos;
	ClassEStd::TimestampToDateTime(&dt, timestamp);
	if (ilocale.dateformat == 2)
		strcpy(str, "0000");
	else
		strcpy(str, "00");
	strcat(str, ilocale.sep_dt);
	strcat(str, "00");
	strcat(str, ilocale.sep_dt);
	if (ilocale.dateformat == 2)
		strcat(str, "00");
	else
		strcat(str, "0000");
	strcat(str, " 00");
	strcat(str, ilocale.sep_tm);
	strcat(str, "00");
	if (ilocale.dateformat == 0)
	{
		daypos = 3;
		monthpos = 0;
		yearpos = 6;
	}
	else if (ilocale.dateformat == 1)
	{
		daypos = 0;
		monthpos = 3;
		yearpos = 6;
	}
	else
	{
		daypos = 8;
		monthpos = 5;
		yearpos = 0;
	}
	str[daypos] = dt.day / 10 + '0';
	str[daypos + 1] = dt.day % 10 + '0';
	str[monthpos] = dt.month / 10 + '0';
	str[monthpos + 1] = dt.month % 10 + '0';
	str[yearpos] = (char)(dt.year / 1000 % 10 + '0');
	str[yearpos + 1] = (char)(dt.year / 100 % 10 + '0');
	str[yearpos + 2] = (char)(dt.year / 10 % 10 + '0');
	str[yearpos + 3] = (char)(dt.year % 10 + '0');
	str[11] = dt.hour / 10 + '0';
	str[12] = dt.hour % 10 + '0';
	str[14] = dt.minute / 10 + '0';
	str[15] = dt.minute % 10 + '0';
}

//Convert a string to a timestamp
ETYPE_TIMESTAMP StrToTimestamp(char *str_p)
{
	E_DATETIME dt;
	char str[MAX_ESTR];
	unsigned long daypos, monthpos, yearpos;
	strcpy(str, str_p);
	monthpos = strlen(str);
	for (daypos = 0; daypos < monthpos; daypos++)
		if (str[daypos] >= '0' && str[daypos] <= '9')
			str[daypos] -= '0';
	if (ilocale.dateformat == 0)
	{
		daypos = 3;
		monthpos = 0;
		yearpos = 6;
	}
	else if (ilocale.dateformat == 1)
	{
		daypos = 0;
		monthpos = 3;
		yearpos = 6;
	}
	else
	{
		daypos = 8;
		monthpos = 5;
		yearpos = 0;
	}
	dt.day = str[daypos] * 10 + str[daypos + 1];
	dt.month = str[monthpos] * 10 + str[monthpos + 1];
	dt.year = str[yearpos] * 1000 + str[yearpos + 1] * 100 + str[yearpos + 2] * 10 + str[yearpos + 3];
	dt.hour = str[11] * 10 + str[12];
	dt.minute = str[14] * 10 + str[15];
	return(ClassEStd::DateTimeToTimestamp(&dt));
}

//Sort function
int CALLBACK ListCompareAlphaNumericA(LPARAM lparam1, LPARAM lparam2, LPARAM lparamsort)
{
	RESITEM *item1 = (RESITEM *)lparam1;
	RESITEM *item2 = (RESITEM *)lparam2;
	return(_stricmp(item1->coltext[lparamsort], item2->coltext[lparamsort]));
}

//Sort function
int CALLBACK ListCompareAlphaNumericD(LPARAM lparam1, LPARAM lparam2, LPARAM lparamsort)
{
	return(-ListCompareAlphaNumericA(lparam1, lparam2, lparamsort));
}

//Sort function
int CALLBACK ListCompareNumericA(LPARAM lparam1, LPARAM lparam2, LPARAM lparamsort)
{
	RESITEM *item1 = (RESITEM *)lparam1;
	RESITEM *item2 = (RESITEM *)lparam2;
	return(atoi(item1->coltext[lparamsort]) - atoi(item2->coltext[lparamsort]));
}

//Sort function
int CALLBACK ListCompareNumericD(LPARAM lparam1, LPARAM lparam2, LPARAM lparamsort)
{
	return(-ListCompareNumericA(lparam1, lparam2, lparamsort));
}

//Sort function
int CALLBACK ListCompareSepNumericA(LPARAM lparam1, LPARAM lparam2, LPARAM lparamsort)
{
	RESITEM *item1 = (RESITEM *)lparam1;
	RESITEM *item2 = (RESITEM *)lparam2;
	return(StrToNr(item1->coltext[lparamsort]) - StrToNr(item2->coltext[lparamsort]));
}

//Sort function
int CALLBACK ListCompareSepNumericD(LPARAM lparam1, LPARAM lparam2, LPARAM lparamsort)
{
	return(-ListCompareSepNumericA(lparam1, lparam2, lparamsort));
}

//Sort function
int CALLBACK ListCompareDateA(LPARAM lparam1, LPARAM lparam2, LPARAM lparamsort)
{
	RESITEM *item1 = (RESITEM *)lparam1;
	RESITEM *item2 = (RESITEM *)lparam2;
	return(StrToTimestamp(item1->coltext[lparamsort]) - StrToTimestamp(item2->coltext[lparamsort]));
}

//Sort function
int CALLBACK ListCompareDateD(LPARAM lparam1, LPARAM lparam2, LPARAM lparamsort)
{
	return(-ListCompareDateA(lparam1, lparam2, lparamsort));
}

//Sort files in the list view
void SortListView(HWND hwnd, unsigned char column)
{
	HWND listwnd = GetDlgItem(hwnd, IDC_FILELIST);
	if (sorttype[column] == SORT_NUMERIC)
	{
		if (sortascending)
			ListView_SortItems(listwnd, ListCompareNumericA, column);
		else
			ListView_SortItems(listwnd, ListCompareNumericD, column);
	}
	else if (sorttype[column] == SORT_SEP_NUMERIC)
	{
		if (sortascending)
			ListView_SortItems(listwnd, ListCompareSepNumericA, column);
		else
			ListView_SortItems(listwnd, ListCompareSepNumericD, column);
	}
	else if (sorttype[column] == SORT_DATE)
	{
		if (sortascending)
			ListView_SortItems(listwnd, ListCompareDateA, column);
		else
			ListView_SortItems(listwnd, ListCompareDateD, column);
	}
	else
	{
		if (sortascending)
			ListView_SortItems(listwnd, ListCompareAlphaNumericA, column);
		else
			ListView_SortItems(listwnd, ListCompareAlphaNumericD, column);
	}
}

//Build the file list
void BuildList(HWND parenthwnd, bool norebuild)
{
	LVITEM lvi;
	EPK_FILEINFO fileinfo;
	HWND hwnd = GetDlgItem(parenthwnd, IDC_FILELIST);
	unsigned long totalfiles = 0, totalsizeorg = 0, totalsizenew = 0;
	unsigned short ctr;
	char statustxt[MAX_ESTR];
	if (!norebuild)
	{
		SendMessage(hwnd, WM_SETREDRAW, FALSE, 0);
		ListView_DeleteAllItems(hwnd);
		if (resitem != NULL)
		{
			free(resitem);
			resitem = NULL;
		}
		if (programstate == PS_LOADED)
		{
			resitem = (RESITEM *)malloc((epack->PackContainFiles() + 2) * sizeof(RESITEM));
			if (resitem != NULL)
			{
				for (ctr = 0; ctr < epack->PackContainFiles(); ctr++)
				{
					E_DATETIME dt;
					epack->PackGetFileInfo(&fileinfo, ctr);
					itoa(ctr, resitem[ctr].coltext[0], 10);
					strcpy(resitem[ctr].coltext[1], fileinfo.filename);
					totalsizeorg += fileinfo.orgsize;
					totalsizenew += fileinfo.packsize;
					totalfiles++;
					NrToStr(fileinfo.orgsize, resitem[ctr].coltext[2]);
					NrToStr(fileinfo.packsize, resitem[ctr].coltext[3]);
					if (fileinfo.compresstype == ECMP_NONE)         strcpy(resitem[ctr].coltext[4], "None");
					else if (fileinfo.compresstype == ECMP_EDGE)    strcpy(resitem[ctr].coltext[4], "EDGELIB");
					else if (fileinfo.compresstype == ECMP_ZLIB)    strcpy(resitem[ctr].coltext[4], "ZLib");
					else                                            strcpy(resitem[ctr].coltext[4], "????");
					if (fileinfo.orgsize >= 10000000)
						itoa(100 - fileinfo.packsize / (fileinfo.orgsize / 100), resitem[ctr].coltext[5], 10);
					else if (fileinfo.orgsize > 0)
						itoa(100 - fileinfo.packsize * 100 / fileinfo.orgsize, resitem[ctr].coltext[5], 10);
					else
						strcpy(resitem[ctr].coltext[5], "0");
					strcat(resitem[ctr].coltext[5], "%");
					ClassEStd::TimestampToDateTime(&dt, fileinfo.filedate);
					TimestampToStr(fileinfo.filedate, resitem[ctr].coltext[6]);
					lvi.mask      = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
					lvi.state     = 0;
					lvi.stateMask = 0;
					lvi.pszText   = LPSTR_TEXTCALLBACK;
					lvi.iImage    = 0;
					lvi.iItem     = ctr;
					lvi.iSubItem  = 0;
					lvi.lParam    = (LPARAM)&resitem[ctr];
					ListView_InsertItem(hwnd, &lvi);
				}
			}
		}
		SendMessage(hwnd, WM_SETREDRAW, TRUE, 0);
	}
	if (totalfiles != 0)
	{
		NrToStr(totalfiles, statustxt);
		if (totalfiles > 1)
			strcat(statustxt, " files: ");
		else
			strcat(statustxt, " file: ");
		NrToStr(totalsizeorg, &statustxt[strlen(statustxt)]);
		strcat(statustxt, " bytes original size, ");
		NrToStr(totalsizenew, &statustxt[strlen(statustxt)]);
		strcat(statustxt, " bytes compressed size");
	}
	else if (programstate == PS_LOADED)
		strcpy(statustxt, "No files");
	else
		strcpy(statustxt, "");
	SetWindowText(GetDlgItem(parenthwnd, IDC_STATUSBAR), statustxt);
}

//Resize the list view to fill the window
void SizeList(HWND hwnd)
{
	RECT rc;
	HWND fwnd = GetDlgItem(hwnd, IDC_FILELIST);
	HWND swnd = GetDlgItem(hwnd, IDC_STATUSBAR);
	HWND pwnd = GetDlgItem(hwnd, IDC_PROGRESS);
	GetClientRect(hwnd, &rc);
	MoveWindow(fwnd, 0, 0, rc.right - rc.left, rc.bottom - rc.top - 20, TRUE);
	MoveWindow(swnd, 0, rc.bottom - 20, rc.right - rc.left - 140, 20, TRUE);
	MoveWindow(pwnd, rc.right - 140, rc.bottom - 20, 140, 20, TRUE);
}

//Resize the status console to fill the window
void SizeStatus(HWND hwnd)
{
	RECT wrc, irc;
	HWND txtwnd = GetDlgItem(hwnd, IDC_PACKSTATUS);
	HWND bwnd   = GetDlgItem(hwnd, IDCLOSE);
	GetClientRect(hwnd, &wrc);
	MoveWindow(txtwnd, 0, 0, wrc.right - wrc.left, wrc.bottom - wrc.top - 56, TRUE);
	unsigned long buttonh_pos = 16;
	GetClientRect(bwnd, &irc);
	MoveWindow(bwnd, buttonh_pos, wrc.bottom - wrc.top - 38, irc.right - irc.left, 24, TRUE);
}

//Read vendors from a file
bool ReadVendors(char *filename)
{
	ClassEIni ini;
	long ctr, newstart = vendorfound;
	if (ini.Open(filename))
	{
		char key[MAX_ESTR], value[MAX_ESTR];
		while (ini.ReadLine(key, value))
		{
			if (strlen(key) == 4)
			{
				bool alreadyfound = false;
				for (ctr = newstart; ctr < (long)vendorfound; ctr++)
					if (ClassEStd::StrEqual(vendorstr[ctr], key))
						alreadyfound = true;
				if (vendorfound < 16 && !alreadyfound)
				{
					vendorcode[vendorfound] = ClassEStd::StrToInt(value, 16);
					if (strlen(value) > 2)
						if (value[1] == 'x' || value[1] == 'X')
							vendorcode[vendorfound] = ClassEStd::StrToInt(&value[2], 16);
					strcpy(vendorstr[vendorfound], key);
					vendorfound++;
				}
			}
		}
	}
	else
		return(false);
	return(true);
}

//Reload vendor files
void ReloadVendors(void)
{
	vendorfound = 0;
	if (strlen(vlocalfile) > 2)
		ReadVendors(vlocalfile);
	localfound = vendorfound;
	ReadVendors(vglobalfile);
}

//Write vendor files
void WriteVendors(void)
{
	ClassEFile ef;
	char wstr[MAX_ESTR];
	unsigned long ctr;
	if (strlen(vlocalfile) > 2)
		if (ef.New(vlocalfile))
		{
			strcpy(wstr, "# Vendor settings used by the EDGELIB Packer\r\n");
			ef.Write(wstr, strlen(wstr));
			strcpy(wstr, "# This file contains a maximum of 16 vendors which are defined by a 4 character string\r\n");
			ef.Write(wstr, strlen(wstr));
			strcpy(wstr, "# and a pack unlock code\r\n\r\n");
			ef.Write(wstr, strlen(wstr));
			strcpy(wstr, "# Important:\r\n");
			ef.Write(wstr, strlen(wstr));
			strcpy(wstr, "# - The first vendor ID is used as default\r\n");
			ef.Write(wstr, strlen(wstr));
			strcpy(wstr, "# - It reads vendor.ini files in the following order: first it reads all vendor ID's from\r\n");
			ef.Write(wstr, strlen(wstr));
			strcpy(wstr, "# - vendor.ini from the current folder, then it reads vendor.ini from the EDGELIB Packer folder.\r\n\r\n");
			ef.Write(wstr, strlen(wstr));
			for (ctr = 0; ctr < localfound; ctr++)
			{
				strcpy(wstr, vendorstr[ctr]);
				strcat(wstr, " = 0x");
				ClassEStd::IntToStr(&wstr[strlen(wstr)], vendorcode[ctr], 16);
				strcat(wstr, "\r\n");
				ef.Write(wstr, strlen(wstr));
			}
			ef.Close();
		}
	if (ef.New(vglobalfile))
	{
		strcpy(wstr, "# Vendor settings used by the EDGELIB Packer\r\n");
		ef.Write(wstr, strlen(wstr));
		strcpy(wstr, "# This file contains a maximum of 16 vendors which are defined by a 4 character string\r\n");
		ef.Write(wstr, strlen(wstr));
		strcpy(wstr, "# and a pack unlock code\r\n\r\n");
		ef.Write(wstr, strlen(wstr));
		strcpy(wstr, "# Important:\r\n");
		ef.Write(wstr, strlen(wstr));
		strcpy(wstr, "# - The first vendor ID is used as default\r\n");
		ef.Write(wstr, strlen(wstr));
		strcpy(wstr, "# - It reads vendor.ini files in the following order: first it reads all vendor ID's from\r\n");
		ef.Write(wstr, strlen(wstr));
		strcpy(wstr, "# - vendor.ini from the current folder, then it reads vendor.ini from the EDGELIB Packer folder.\r\n\r\n");
		ef.Write(wstr, strlen(wstr));
		for (ctr = localfound; ctr < vendorfound; ctr++)
		{
			strcpy(wstr, vendorstr[ctr]);
			strcat(wstr, " = 0x");
			ClassEStd::IntToStr(&wstr[strlen(wstr)], vendorcode[ctr], 16);
			strcat(wstr, "\r\n");
			ef.Write(wstr, strlen(wstr));
		}
		ef.Close();
	}
}

//Append to output console
void PrintOutput(HWND hwnd, char *str)
{
	if (hwnd != NULL)
	{
		SendMessage(GetDlgItem(hwnd, IDC_PACKSTATUS), EM_SETSEL, 0xFFFFFFF, 0xFFFFFFF);
		SendMessage(GetDlgItem(hwnd, IDC_PACKSTATUS), EM_REPLACESEL, FALSE, (LPARAM)str);
	}
}

//Check for relative paths, prepend if neccessary
void PathCheckFile(char *dst, char *src)
{
	bool pathabsolute = false;
	ClassEStd::StrCpy(dst, src);
	if (dst[0] == '\\' || dst[0] == '/')
		pathabsolute = true;
	if (dst[1] == ':' && (dst[2] == '\\' || dst[2] == '/'))
		pathabsolute = true;
	if (!pathabsolute)
	{
		_getcwd(dst, MAX_EFILESTR);
		if (dst[ClassEStd::StrLen(dst) - 1] != '\\' && dst[ClassEStd::StrLen(dst) - 1] != '/')
			ClassEStd::StrCat(dst, "\\");
		ClassEStd::StrCat(dst, src);
	}
}

//Callback for commandline commands
bool OnCommandQueue(HWND hwnd, unsigned long nr)
{
	ERESULT result;
	bool onadd = false, onextract = false, ondelete = false, onvendor = false, oncompress = false, forceaction = false;
	unsigned char showhelp = 0, showversion = 0, listfiles = 0, firstfile = 0, packfile = 0, compresstype = ECMP_ZLIB;
	unsigned long ctr, index, addcount = 0, extractcount = 0, deletecount = 0;
	char tmp_str[MAX_ESTR], vidstr[MAX_ESTR];
	commandbusy = true;
	ClassEStd::StrCpy(vidstr, "");
	for (ctr = 0; ctr < argcount; ctr++)
	{
		strcpy(tmp_str, ";");
		strcat(tmp_str, &cmdargs[argstart[ctr]]);
		strcat(tmp_str, ";");
		if (ClassEStd::StrFind((char *)supportcmd, tmp_str, false) >= 0)
		{
			if (ClassEStd::StrEqual(tmp_str, ";-h;") || ClassEStd::StrEqual(tmp_str, ";--help;") || ClassEStd::StrEqual(tmp_str, ";/?;"))
				showhelp = 1;
			else if (ClassEStd::StrEqual(tmp_str, ";-v;") || ClassEStd::StrEqual(tmp_str, ";--version;"))
				showversion = 1;
			else if (ClassEStd::StrEqual(tmp_str, ";-l;") || ClassEStd::StrEqual(tmp_str, ";--list;"))
				listfiles = 1;
			else if (ClassEStd::StrEqual(tmp_str, ";-f;") || ClassEStd::StrEqual(tmp_str, ";--force;"))
				forceaction = true;
			if (ClassEStd::StrEqual(tmp_str, ";-a;") || ClassEStd::StrEqual(tmp_str, ";--add;"))
				onadd = true;
			else
				onadd = false;
			if (ClassEStd::StrEqual(tmp_str, ";-e;") || ClassEStd::StrEqual(tmp_str, ";--extract;"))
				onextract = true;
			else
				onextract = false;
			if (ClassEStd::StrEqual(tmp_str, ";-d;") || ClassEStd::StrEqual(tmp_str, ";--delete;"))
				ondelete = true;
			else
				ondelete = false;
			if (ClassEStd::StrEqual(tmp_str, ";-n;") || ClassEStd::StrEqual(tmp_str, ";--vendor;"))
				onvendor = true;
			else
				onvendor = false;
			if (ClassEStd::StrEqual(tmp_str, ";-o;") || ClassEStd::StrEqual(tmp_str, ";--compression;"))
				oncompress = true;
			else
				oncompress = false;
		}
		else
		{
			if (ctr == 0)
				firstfile = 1;
			if (onadd)
				addcount++;
			else if (onextract)
				extractcount++;
			else if (ondelete)
				deletecount++;
			else if (onvendor && ClassEStd::StrLen(vidstr) == 0)
				ClassEStd::StrCpy(vidstr, &cmdargs[argstart[ctr]]);
			else if (oncompress)
			{
				if (ClassEStd::StrEqual(&cmdargs[argstart[ctr]], "ZLib", false))
					compresstype = ECMP_ZLIB;
				else if (ClassEStd::StrEqual(&cmdargs[argstart[ctr]], "EDGELIB", false))
					compresstype = ECMP_EDGE;
				else if (ClassEStd::StrEqual(&cmdargs[argstart[ctr]], "None", false))
					compresstype = ECMP_NONE;
			}
		}
	}
	if (ClassEStd::StrLen(vidstr) == 0)
		ClassEStd::StrCpy(vidstr, "FREE");
	if (addcount + extractcount + deletecount + listfiles > 0 && firstfile == 0)
	{
		PrintOutput(hwnd, "Error(1): When adding, extracting, deleting or listing files, the first parameter must be a valid pack filename\r\n");
		lastpackerror = 1;
		return(true);
	}
	if (addcount + extractcount + deletecount + listfiles > 0)
		packfile = 1;
	if (showversion && nr == 0)
	{
		strcpy(tmp_str, "Running EDGELIB Packer: ");
		strcat(tmp_str, PROGRAM_VERINFO);
		strcat(tmp_str, "\r\n");
		PrintOutput(hwnd, tmp_str);
	}
	else if (nr == showversion && addcount + extractcount + deletecount + listfiles > 0)
	{
		bool hasext = false, packexists;
		char packfn[MAX_EFILESTR];
		PathCheckFile(packfn, &cmdargs[argstart[0]]);
		for (ctr = ClassEStd::StrLen(packfn) - 1; ctr > 0; ctr--)
		{
			if (packfn[ctr] == '.')
				hasext = true;
			if (packfn[ctr] == '\\' || packfn[ctr] == '/' || packfn[ctr] == '.')
				break;
		}
		if (!hasext)
			ClassEStd::StrCat(packfn, ".epk");
		packexists = epack->Exists(packfn);
		if (packexists || addcount == 0)
		{
			if (!packexists)
			{
				strcpy(tmp_str, "Error(10): ");
				strcat(tmp_str, &cmdargs[argstart[0]]);
				if (!hasext)
					ClassEStd::StrCat(tmp_str, ".epk");
				strcat(tmp_str, " doesn't exists\r\n");
				PrintOutput(hwnd, tmp_str);
				lastpackerror = 10;
				return(true);
			}
			if (epack->Open(packfn))
			{
				if (epack->PackValid())
				{
					char fvendor[8];
					strcpy(tmp_str, "Opening: ");
					strcat(tmp_str, &cmdargs[argstart[0]]);
					if (!hasext)
						ClassEStd::StrCat(tmp_str, ".epk");
					strcat(tmp_str, "\r\n");
					PrintOutput(hwnd, tmp_str);
					epack->PackGetVendor(fvendor);
					for (ctr = 0; ctr < vendorfound; ctr++)
						if (ClassEStd::StrEqual(fvendor, vendorstr[ctr]))
						{
							currentvendor = ctr;
							break;
						}
					if (!epack->PackAuthorize(vendorcode[currentvendor]))
					{
						epack->Close();
						PrintOutput(hwnd, "Error(2): This file is locked by a vendor signature, you aren't authorized to open\r\n");
						lastpackerror = 2;
						return(true);
					}
				}
				else
				{
					strcpy(tmp_str, "Error(9): ");
					strcat(tmp_str, &cmdargs[argstart[0]]);
					if (!hasext)
						ClassEStd::StrCat(tmp_str, ".epk");
					strcat(tmp_str, " is not a valid EDGELIB Pack file\r\n");
					PrintOutput(hwnd, tmp_str);
					lastpackerror = 9;
					return(true);
				}
			}
			else
			{
				strcpy(tmp_str, "Error(3): Couldn't open ");
				strcat(tmp_str, &cmdargs[argstart[0]]);
				if (!hasext)
					ClassEStd::StrCat(tmp_str, ".epk");
				strcat(tmp_str, "\r\n");
				PrintOutput(hwnd, tmp_str);
				lastpackerror = 3;
				return(true);
			}
		}
		else
		{
			if (epack->PackNew(packfn, vidstr) == E_OK)
			{
				strcpy(tmp_str, "Creating: ");
				strcat(tmp_str, &cmdargs[argstart[0]]);
				if (!hasext)
					ClassEStd::StrCat(tmp_str, ".epk");
				strcat(tmp_str, "\r\n");
				PrintOutput(hwnd, tmp_str);
			}
			else
			{
				strcpy(tmp_str, "Error(4): Couldn't create ");
				strcat(tmp_str, &cmdargs[argstart[0]]);
				if (!hasext)
					ClassEStd::StrCat(tmp_str, ".epk");
				strcat(tmp_str, "\r\n");
				PrintOutput(hwnd, tmp_str);
				lastpackerror = 4;
				return(true);
			}
		}
	}
	else if (nr < showversion + packfile + addcount)
	{
		char addfn[MAX_EFILESTR];
		unsigned long showarg = 0;
		index = nr - showversion - packfile;
		ClassEStd::StrCpy(addfn, "");
		onadd = false;
		for (ctr = 0; ctr < argcount; ctr++)
		{
			strcpy(tmp_str, ";");
			strcat(tmp_str, &cmdargs[argstart[ctr]]);
			strcat(tmp_str, ";");
			if (ClassEStd::StrFind((char *)supportcmd, tmp_str, false) >= 0)
			{
				if (ClassEStd::StrEqual(tmp_str, ";-a;") || ClassEStd::StrEqual(tmp_str, ";--add;"))
					onadd = true;
				else
					onadd = false;
			}
			else if (onadd)
			{
				if (index == 0)
				{
					PathCheckFile(addfn, &cmdargs[argstart[ctr]]);
					showarg = ctr;
					break;
				}
				else
					index--;
			}
		}
		result = epack->PackAddFile(addfn, forceaction, compresstype);
		if (result == E_OK)
		{
			strcpy(tmp_str, "Adding: ");
			strcat(tmp_str, &cmdargs[argstart[showarg]]);
			strcat(tmp_str, "\r\n");
			PrintOutput(hwnd, tmp_str);
		}
		else if (result == E_ALREADYFOUND)
		{
			strcpy(tmp_str, "Warning: ");
			strcat(tmp_str, &cmdargs[argstart[showarg]]);
			strcat(tmp_str, " already exists, skipped (use -f to force overwrite)\r\n");
			PrintOutput(hwnd, tmp_str);
		}
		else
		{
			strcpy(tmp_str, "Error(5): Couldn't add ");
			strcat(tmp_str, &cmdargs[argstart[showarg]]);
			strcat(tmp_str, "\r\n");
			PrintOutput(hwnd, tmp_str);
			lastpackerror = 5;
			return(true);
		}
	}
	else if (nr < showversion + packfile + addcount + extractcount)
	{
		char extractfn[MAX_EFILESTR], curcwd[MAX_EFILESTR];
		unsigned long showarg = 0;
		_getcwd(curcwd, MAX_EFILESTR);
		index = nr - showversion - packfile - addcount;
		ClassEStd::StrCpy(extractfn, "");
		onextract = false;
		for (ctr = 0; ctr < argcount; ctr++)
		{
			strcpy(tmp_str, ";");
			strcat(tmp_str, &cmdargs[argstart[ctr]]);
			strcat(tmp_str, ";");
			if (ClassEStd::StrFind((char *)supportcmd, tmp_str, false) >= 0)
			{
				if (ClassEStd::StrEqual(tmp_str, ";-e;") || ClassEStd::StrEqual(tmp_str, ";--extract;"))
					onextract = true;
				else
					onextract = false;
			}
			else if (onextract)
			{
				if (index == 0)
				{
					ClassEStd::StrCpy(extractfn, &cmdargs[argstart[ctr]]);
					showarg = ctr;
					break;
				}
				else
					index--;
			}
		}
		if (epack->PackSearchFile(extractfn, index))
		{
			result = epack->PackExtractFile(curcwd, index, forceaction);
			if (result == E_OK)
			{
				strcpy(tmp_str, "Extracting: ");
				strcat(tmp_str, &cmdargs[argstart[showarg]]);
				strcat(tmp_str, "\r\n");
				PrintOutput(hwnd, tmp_str);
			}
			else if (result == E_ALREADYFOUND)
			{
				strcpy(tmp_str, "Warning: ");
				strcat(tmp_str, &cmdargs[argstart[showarg]]);
				strcat(tmp_str, " already exists, skipped (use -f to force overwrite)\r\n");
				PrintOutput(hwnd, tmp_str);
			}
			else
			{
				strcpy(tmp_str, "Error(6): Couldn't extract ");
				strcat(tmp_str, &cmdargs[argstart[showarg]]);
				strcat(tmp_str, "\r\n");
				PrintOutput(hwnd, tmp_str);
				lastpackerror = 6;
				return(true);
			}
		}
		else
		{
			strcpy(tmp_str, "Error(7): ");
			strcat(tmp_str, &cmdargs[argstart[showarg]]);
			strcat(tmp_str, " not found\r\n");
			PrintOutput(hwnd, tmp_str);
			lastpackerror = 7;
			return(true);
		}
	}
	else if (nr < showversion + packfile + addcount + extractcount + deletecount)
	{
		char deletefn[MAX_EFILESTR];
		unsigned long showarg = 0;
		index = nr - showversion - packfile - addcount - extractcount;
		ClassEStd::StrCpy(deletefn, "");
		ondelete = false;
		for (ctr = 0; ctr < argcount; ctr++)
		{
			strcpy(tmp_str, ";");
			strcat(tmp_str, &cmdargs[argstart[ctr]]);
			strcat(tmp_str, ";");
			if (ClassEStd::StrFind((char *)supportcmd, tmp_str, false) >= 0)
			{
				if (ClassEStd::StrEqual(tmp_str, ";-d;") || ClassEStd::StrEqual(tmp_str, ";--delete;"))
					ondelete = true;
				else
					ondelete = false;
			}
			else if (ondelete)
			{
				if (index == 0)
				{
					ClassEStd::StrCpy(deletefn, &cmdargs[argstart[ctr]]);
					showarg = ctr;
					break;
				}
				else
					index--;
			}
		}
		if (epack->PackSearchFile(deletefn, index))
		{
			result = epack->PackDeleteFile(index);
			if (result == E_OK)
			{
				strcpy(tmp_str, "Deleting: ");
				strcat(tmp_str, &cmdargs[argstart[showarg]]);
				strcat(tmp_str, "\r\n");
				PrintOutput(hwnd, tmp_str);
			}
			else if (result == E_NOTEXISTS)
			{
				strcpy(tmp_str, "Warning: ");
				strcat(tmp_str, &cmdargs[argstart[showarg]]);
				strcat(tmp_str, " doesn't exist\r\n");
				PrintOutput(hwnd, tmp_str);
			}
			else
			{
				strcpy(tmp_str, "Error(11): Couldn't delete ");
				strcat(tmp_str, &cmdargs[argstart[showarg]]);
				strcat(tmp_str, "\r\n");
				PrintOutput(hwnd, tmp_str);
				lastpackerror = 11;
				return(true);
			}
		}
		else
		{
			strcpy(tmp_str, "Error(7): ");
			strcat(tmp_str, &cmdargs[argstart[showarg]]);
			strcat(tmp_str, " not found\r\n");
			PrintOutput(hwnd, tmp_str);
			lastpackerror = 7;
			return(true);
		}
	}
	else if (nr < showversion + packfile + addcount + extractcount + deletecount + listfiles && listfiles)
	{
		if (showversion + packfile + addcount + extractcount > 0)
			PrintOutput(hwnd, "\r\n");
		PrintOutput(hwnd, "Listing files\r\n-------------\r\n");
		for (ctr = 0; ctr < epack->PackContainFiles(); ctr++)
		{
			EPK_FILEINFO info;
			if (epack->PackGetFileInfo(&info, ctr))
			{
				ClassEStd::StrCpy(tmp_str, info.filename);
				ClassEStd::StrCat(tmp_str, "\r\n");
				PrintOutput(hwnd, tmp_str);
			}
			else
			{
				PrintOutput(hwnd, "Error(8): A problem occurred when trying to display the file list\r\n");
				lastpackerror = 8;
				return(true);
			}
		}
		listfiles = 0;
	}
	else if (nr < showversion + packfile + addcount + extractcount + deletecount + listfiles + showhelp && showhelp)
	{
		if (showversion + packfile + addcount + extractcount + listfiles > 0)
			PrintOutput(hwnd, "\r\n");
		PrintOutput(hwnd, "Usage: edgepack.exe [file.epk] -options\r\n\r\n");
		PrintOutput(hwnd, "Options:\r\n-------\r\n");
		PrintOutput(hwnd, "-a, --add [files]: Add files to the pack\r\n");
		PrintOutput(hwnd, "-e, --extract [files]: Extract files from the pack\r\n");
		PrintOutput(hwnd, "-d, --delete [files]: Delete files from the pack\r\n");
		PrintOutput(hwnd, "-l, --list: Show a list of all files in the pack\r\n");
		PrintOutput(hwnd, "-n, --vendor [id]: Create new pack files with this vendor id, default is \"FREE\"\r\n");
		PrintOutput(hwnd, "-o, --compression [id]: Compress files using ZLib (default), Edge or None\r\n");
		PrintOutput(hwnd, "-f, --force: Force overwrite if file already exists (both add and extract)\r\n");
		PrintOutput(hwnd, "-q, --quiet: Don't display a status window\r\n");
		PrintOutput(hwnd, "-c, --autoclose: Automatically closes the status window when operations are done successfully\r\n");
		PrintOutput(hwnd, "-v, --version: Show the current version of the EDGELIB Packer\r\n");
		PrintOutput(hwnd, "-h, --help, /?: Show this help screen\r\n");
	}
	else
	{
		epack->Close();
		PrintOutput(hwnd, "\r\nDone.\r\n");
		return(true);
	}
	commandbusy = false;
	return(false);
}

BOOL CALLBACK AllMsgWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	switch(wmsg)
	{
		case WM_COMMAND:
			if (wparam == IDYES || wparam == IDNO || wparam == IDYESALL || wparam == IDNOALL)
			{
				EndDialog(hwnd, wparam);
				return(TRUE);
			}
			break;
		case WM_INITDIALOG:
			char *ptl;
			ptl = (char *)lparam;
			SetWindowText(hwnd, ptl);
			SetWindowText(GetDlgItem(hwnd, IDC_CUSTOMTEXT), &ptl[1024]);
			return(TRUE);
		case WM_CLOSE:
			EndDialog(hwnd, IDNO);
			return(TRUE);
	}
	return(FALSE);
}

BOOL CALLBACK ConsoleWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	switch(wmsg)
	{
		case WM_COMMAND:
			switch(wparam)
			{
				case IDCLOSE:
					EndDialog(hwnd, IDABORT);
					return(TRUE);
			}
			break;
		case WM_TIMER:
			if (!commandbusy)
			{
				if (OnCommandQueue(hwnd, commandqueue))
				{
					bool autoclose = false;
					char tmp_str[MAX_ESTR];
					unsigned long ctr;
					if (lastpackerror == 0)
					{
						for (ctr = 0; ctr < argcount; ctr++)
						{
							strcpy(tmp_str, ";");
							strcat(tmp_str, &cmdargs[argstart[ctr]]);
							strcat(tmp_str, ";");
							if (ClassEStd::StrFind((char *)supportcmd, tmp_str, false) >= 0)
								if (ClassEStd::StrEqual(tmp_str, ";-c;") || ClassEStd::StrEqual(tmp_str, ";--autoclose;"))
									autoclose = true;
						}
					}
					KillTimer(hwnd, 0);
					if (autoclose)
						EndDialog(hwnd, IDABORT);
					else
						EnableWindow(GetDlgItem(hwnd, IDCLOSE), TRUE);
				}
				commandqueue++;
			}
			break;
		case WM_CLOSE:
			EndDialog(hwnd, IDABORT);
			return(TRUE);
		case WM_SIZE:
			SizeStatus(hwnd);
			return(0);
		case WM_INITDIALOG:
			commandbusy = false;
			SizeStatus(hwnd);
			SetTimer(hwnd, 0, 100, NULL);
			return(TRUE);
		case WM_DESTROY:
			KillTimer(hwnd, 0);
			return(TRUE);
	}
	return(FALSE);
}

BOOL CALLBACK AddWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	switch(wmsg)
	{
		case WM_COMMAND:
			switch(wparam)
			{
				case IDOK:
					GetWindowText(GetDlgItem(hwnd, IDC_VENDORID), idtext, MAX_ESTR);
					GetWindowText(GetDlgItem(hwnd, IDC_VENDORCODE), codetext, MAX_ESTR);
					EndDialog(hwnd, IDOK);
					return(TRUE);
				case IDCANCEL:
					EndDialog(hwnd, IDABORT);
					return(TRUE);
			}
			break;
		case WM_CLOSE:
			EndDialog(hwnd, IDABORT);
			return(TRUE);
	}
	return(FALSE);
}

BOOL CALLBACK PickWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	bool rebuildlist = false, adddouble = true;
	char addstr[MAX_ESTR];
	unsigned long ctr;
	long selvendor, addto;
	switch(wmsg)
	{
		case WM_COMMAND:
			long iselected;
			iselected = (unsigned long)SendMessage(GetDlgItem(hwnd, IDC_VENDORID), LB_GETCURSEL, 0, 0);
			iselected = (unsigned long)SendMessage(GetDlgItem(hwnd, IDC_VENDORID), LB_GETITEMDATA, iselected, 0);
			switch(LOWORD(wparam))
			{
				case IDDELETE:
					if (iselected != LB_ERR && iselected < (long)vendorfound)
					{
						if (iselected < (long)localfound || !ClassEStd::StrEqual(vendorstr[iselected], "FREE", false))
						{
							if (iselected < (long)localfound)
								localfound--;
							for (ctr = iselected; ctr < vendorfound - 1; ctr++)
							{
								vendorcode[ctr] = vendorcode[ctr + 1];
								strcpy(vendorstr[ctr], vendorstr[ctr + 1]);
							}
							selvendor = iselected;
							vendorfound--;
							rebuildlist = true;
						}
						else
							MessageBox(hwnd, "The non-local vendor ID 'FREE' can't be deleted", "Can't delete vendor", MB_OK);
					}
					break;
				case IDADDLOCAL:
				case IDADD:
					if (wparam == IDADDLOCAL)
						addto = localfound;
					else
						addto = vendorfound;
					if (vendorfound < 16)
					{
						if (DialogBox(ghinst, MAKEINTRESOURCE(IDD_ADDVENDOR), hwnd, AddWinProc) == IDOK)
						{
							selvendor = iselected;
							if (addto < (long)vendorfound)
							{
								if (selvendor >= (long)localfound)
									selvendor++;
								for (ctr = vendorfound; (long)ctr > addto; ctr--)
								{
									vendorcode[ctr] = vendorcode[ctr - 1];
									strcpy(vendorstr[ctr], vendorstr[ctr - 1]);
								}
								localfound++;
							}
							vendorcode[addto] = ClassEStd::StrToInt(codetext, 16);
							if (strlen(codetext) > 2)
								if (codetext[1] == 'x' || codetext[1] == 'X')
									vendorcode[addto] = ClassEStd::StrToInt(&codetext[2], 16);
							strcpy(vendorstr[addto], idtext);
							vendorfound++;
							rebuildlist = true;
						}
					}
					else
						MessageBox(hwnd, "The EDGELIB packer only supports a maximum of 16 vendor ID's", "Couldn't add vendor", MB_OK);
					break;
				case IDOK:
					selectedvendor = iselected;
					EndDialog(hwnd, IDOK);
					break;
				case IDCANCEL:
					EndDialog(hwnd, IDABORT);
					return(TRUE);
			}
			switch(HIWORD(wparam))
			{
				case LBN_DBLCLK:
					if (LOWORD(wparam) == IDC_VENDORID)
					{
						char ctx[512];
						GetWindowText(GetDlgItem(hwnd, IDC_VENDORTXT), ctx, 512);
						if (strcmp(ctx, "Change vendor ID of package:") == 0)
							SendMessage(hwnd, WM_COMMAND, IDOK, 0);
					}
					return(TRUE);
			}
			break;
		case WM_CLOSE:
			EndDialog(hwnd, IDABORT);
			return(TRUE);
		case WM_INITDIALOG:
			if (lparam == ID_PACK_CHANGEVENDOR)
			{
				adddouble = false;
				SetWindowText(GetDlgItem(hwnd, IDC_VENDORTXT), "Change vendor ID of package:");
			}
			else if (lparam == ID_EXTRA_CONFIGUREVENDORS)
				SetWindowText(GetDlgItem(hwnd, IDC_VENDORTXT), "Add or delete vendor ID's:");
			rebuildlist = true;
			selvendor = currentvendor;
			if (localfound == 0)
				EnableWindow(GetDlgItem(hwnd, IDADDLOCAL), FALSE);
			break;
	}
	if (rebuildlist)
	{
		bool doublefound;
		unsigned long index, subctr;
		if (selvendor >= (long)vendorfound)
			selvendor = vendorfound - 1;
		SendMessage(GetDlgItem(hwnd, IDC_VENDORID), LB_RESETCONTENT, 0, 0);
		index = 0;
		for (ctr = 0; ctr < vendorfound; ctr++)
		{
			doublefound = false;
			for (subctr = 0; subctr < ctr; subctr++)
				if (ClassEStd::StrEqual(vendorstr[subctr], vendorstr[ctr]))
				{
					if (adddouble)
						selvendor = subctr;
					doublefound = true;
				}
			if (adddouble || !doublefound)
			{
				strcpy(addstr, vendorstr[ctr]);
				if (ctr < localfound && adddouble)
					strcat(addstr, " (local)");
				SendMessage(GetDlgItem(hwnd, IDC_VENDORID), LB_ADDSTRING, 0, (LPARAM)addstr);
				SendMessage(GetDlgItem(hwnd, IDC_VENDORID), LB_SETITEMDATA, index, ctr);
				index++;
			}
		}
		SendMessage(GetDlgItem(hwnd, IDC_VENDORID), LB_SETCURSEL, selvendor, 0);
		return(TRUE);
	}
	return(FALSE);
}

BOOL CALLBACK WinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	WNDPROC SubClass;
	LVCOLUMN lvc;
	LVITEM lvi;
	ERESULT result;
	char tmpstr[256];
	char msgstr[256];
	char dropstr[MAX_FILESTR];
	unsigned long count, dlgresult;
	unsigned short ctr, ictr;
	unsigned char overwrite;
	switch(wmsg)
	{
		case WM_COMMAND:
			switch(wparam)
			{
				case ID_PACKFILE_SELECTALL:
				case ID_PACK_SELECTALL:
				case IDC_SELECTALL:
					count = epack->PackContainFiles();
					for (ctr = 0; ctr < count; ctr++)
					{
						lvi.iItem = ctr;
						lvi.mask = LVIF_STATE;
						lvi.state = LVIS_SELECTED;
						lvi.stateMask = LVIS_SELECTED;
						ListView_SetItem(GetDlgItem(hwnd, IDC_FILELIST), &lvi);
					}
					return(TRUE);
				case IDC_QUICKMENU:
					long lv_mark;
					lv_mark = ListView_GetSelectionMark(GetDlgItem(hwnd, IDC_FILELIST));
					if (lv_mark != -1)
						if (ListView_GetItemState(GetDlgItem(hwnd, IDC_FILELIST), lv_mark, LVIS_SELECTED))
						{
							HMENU hmenu = LoadMenu(ghinst, MAKEINTRESOURCE(IDR_QUICKMENU));
							POINT pnt;
							pnt.x = 0;
							pnt.y = 0;
							ClientToScreen(hwnd, &pnt);
							hmenu = GetSubMenu(hmenu, 0);
							TrackPopupMenu(hmenu, 0, pnt.x, pnt.y, 0, hwnd, NULL);
						}
					return(TRUE);
				case ID_FILE_NEWF:
					if (SelectNewFile(hwnd, "Create New EDGELIB Pack", "EDGELIB pack files", ".epk"))
						NewPackFile(loadedfile, hwnd);
					return(TRUE);
				case ID_FILE_OPENF:
					if (SelectOpenFile(hwnd, "Open EDGELIB Pack", "EDGELIB pack files", ".epk"))
						LoadPackFile(loadedfile, hwnd);
					return(TRUE);
				case ID_FILE_CLOSEF:
					ClosePackFile(hwnd);
					return(TRUE);
				case ID_FILE_EXITP:
					EndDialog(hwnd, IDABORT);
					return(TRUE);
				case ID_PACK_ADD:
					if (programstate == PS_LOADED)
					{
						if (SelectAddFiles(hwnd, "Select files to add to the pack", "All files", ".*"))
						{
							SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_WAIT)));
							AddFiles(hwnd, loadedfile);
							SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
							BuildList(hwnd);
							SortListView(hwnd, sortcolumn);
						}
					}
					else
						MessageBox(hwnd, "You have to create a new pack, or open an existing one to add files.", "EDGELIB pack not loaded", MB_ICONEXCLAMATION | MB_OK);
					return(TRUE);
				case ID_PACKFILE_DELETE:
				case ID_PACK_DELETE:
					if (programstate == PS_LOADED)
					{
						if (MessageBox(hwnd, "Deleting file(s), are you sure?", "Deleting file(s)", MB_YESNO) == IDYES)
						{
							unsigned char todelete[MAX_PACKFILES];
							count = 0;
							SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_WAIT)));
							for (ctr = 0; ctr < epack->PackContainFiles(); ctr++)
								todelete[ctr] = 0;
							for (ctr = 0; ctr < epack->PackContainFiles(); ctr++)
								if (ListView_GetItemState(GetDlgItem(hwnd, IDC_FILELIST), ctr, LVIS_SELECTED))
								{
									RESITEM *ri;
									long f_id;
									lvi.mask = LVIF_PARAM;
									lvi.iItem = ctr;
									lvi.iSubItem = 0;
									ListView_GetItem(GetDlgItem(hwnd, IDC_FILELIST), &lvi);
									ri = (RESITEM *)(lvi.lParam);
									f_id = atoi(ri->coltext[0]);
									todelete[f_id] = 1;
									count++;
								}
							if (count == 0)
								return(TRUE);
							if (count == 1)
								SetWindowText(GetDlgItem(hwnd, IDC_STATUSBAR), "Deleting file...");
							else
								SetWindowText(GetDlgItem(hwnd, IDC_STATUSBAR), "Deleting files...");
							ictr = 0;
							for (ctr = (unsigned short)epack->PackContainFiles(); ctr > 0; ctr--)
								if (todelete[ctr - 1])
								{
									if (epack->PackDeleteFile(ctr - 1) != E_OK)
									{
										strcpy(msgstr, "Unable to delete file: ");
										strcat(msgstr, resitem[ctr - 1].coltext[1]);
										if (MessageBox(hwnd, msgstr, "Delete error", MB_OKCANCEL | MB_ICONEXCLAMATION) == IDCANCEL)
											break;
									}
									ictr++;
									if (count)
									{
										SendMessage(GetDlgItem(hwnd, IDC_PROGRESS), PBM_SETPOS, ictr * 100 / count, 0);
										Sleep(10);
									}
								}
							SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
							SendMessage(GetDlgItem(hwnd, IDC_PROGRESS), PBM_SETPOS, 0, 0);
							BuildList(hwnd);
							SortListView(hwnd, sortcolumn);
						}
					}
					else
						MessageBox(hwnd, "There are no files to delete.", "No selection", MB_ICONEXCLAMATION | MB_OK);
					return(TRUE);
				case ID_PACKFILE_EXTRACT:
				case ID_PACK_EXTRACT:
					if (programstate == PS_LOADED)
					{
						strcpy(extractpath, programpath);
						if (BrowseFolder(hwnd, extractpath))
						{
							unsigned char toextract[MAX_PACKFILES];
							overwrite = 0;
							count = 0;
							for (ctr = 0; ctr < epack->PackContainFiles(); ctr++)
								toextract[ctr] = 0;
							for (ctr = 0; ctr < epack->PackContainFiles(); ctr++)
								if (ListView_GetItemState(GetDlgItem(hwnd, IDC_FILELIST), ctr, LVIS_SELECTED))
								{
									RESITEM *ri;
									long f_id;
									lvi.mask = LVIF_PARAM;
									lvi.iItem = ctr;
									lvi.iSubItem = 0;
									ListView_GetItem(GetDlgItem(hwnd, IDC_FILELIST), &lvi);
									ri = (RESITEM *)(lvi.lParam);
									f_id = atoi(ri->coltext[0]);
									toextract[f_id] = 1;
									count++;
								}
							if (count == 0)
							{
								count = epack->PackContainFiles();
								for (ctr = 0; ctr < count; ctr++)
									toextract[ctr] = 0;
							}
							if (count == 1)
								SetWindowText(GetDlgItem(hwnd, IDC_STATUSBAR), "Extracting file...");
							else
								SetWindowText(GetDlgItem(hwnd, IDC_STATUSBAR), "Extracting files...");
							ictr = 0;
							SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_WAIT)));
							for (ctr = (unsigned short)epack->PackContainFiles(); ctr > 0; ctr--)
							{
								bool alreadyfound = false;
								if (toextract[ctr - 1])
								{
									if (overwrite == 1)
										result = epack->PackExtractFile(extractpath, ctr - 1, true);
									else
										result = epack->PackExtractFile(extractpath, ctr - 1, false);
									if (result == E_ALREADYFOUND)
									{
										dlgresult = IDNO;
										if (overwrite == 0)
										{
											strcpy(msgstr, "File ");
											strcat(msgstr, resitem[ctr - 1].coltext[1]);
											strcat(msgstr, " already exists. Overwrite?");
											SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
											dlgresult = AllMessageBox(hwnd, msgstr, "Extracting file(s)");
											SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_WAIT)));
											if (dlgresult == IDYESALL)
												overwrite = 1;
											else if (dlgresult == IDNOALL)
												overwrite = 2;
										}
										if ((overwrite == 1 && overwrite != 2) || dlgresult == IDYES)
											result = epack->PackExtractFile(extractpath, ctr - 1, true);
										else
											alreadyfound = true;
									}
									if (result != E_OK && result != E_ALREADYFOUND)
									{
										strcpy(msgstr, "Unable to extract file: ");
										strcat(msgstr, resitem[ctr - 1].coltext[1]);
										if (MessageBox(hwnd, msgstr, "Extract error", MB_OKCANCEL | MB_ICONEXCLAMATION) == IDCANCEL)
											break;
									}
									ictr++;
									if (count)
									{
										SendMessage(GetDlgItem(hwnd, IDC_PROGRESS), PBM_SETPOS, ictr * 100 / count, 0);
										if (!alreadyfound)
											Sleep(10);
									}
								}
							}
							SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
							SendMessage(GetDlgItem(hwnd, IDC_PROGRESS), PBM_SETPOS, 0, 0);
							BuildList(hwnd, true);
						}
					}
					return(TRUE);
				case ID_PACKFILE_OPEN:
				case ID_PACK_OPEN:
				case ID_PACKFILE_VIEW:
				case ID_PACK_VIEW:
					unsigned char toopen[MAX_PACKFILES];
					count = 0;
					for (ctr = 0; ctr < epack->PackContainFiles(); ctr++)
						toopen[ctr] = 0;
					for (ctr = 0; ctr < epack->PackContainFiles(); ctr++)
						if (ListView_GetItemState(GetDlgItem(hwnd, IDC_FILELIST), ctr, LVIS_SELECTED))
						{
							RESITEM *ri;
							long f_id;
							lvi.mask = LVIF_PARAM;
							lvi.iItem = ctr;
							lvi.iSubItem = 0;
							ListView_GetItem(GetDlgItem(hwnd, IDC_FILELIST), &lvi);
							ri = (RESITEM *)(lvi.lParam);
							f_id = atoi(ri->coltext[0]);
							toopen[f_id] = 1;
							count++;
						}
					if (count > 1)
						if (MessageBox(hwnd, "Opening multiple files, are you sure?", "Multiple files selected", MB_YESNO) == IDNO)
							return(TRUE);
					for (ctr = 0; ctr < epack->PackContainFiles(); ctr++)
						if (toopen[ctr])
						{
							EPK_FILEINFO finfo;
							long f_id;
							char tmp_path[MAX_PATH];
							f_id = ctr;
							GetTempPath(sizeof(tmp_path), tmp_path);
							strcat(tmp_path, "\\$EdgePacker");
							if (!epack->Exists(tmp_path))
								epack->NewFolder(tmp_path);
							if (epack->PackExtractFile(tmp_path, f_id, true) == E_OK)
							{
								epack->PackGetFileInfo(&finfo, f_id);
								strcat(tmp_path, "\\");
								strcat(tmp_path, finfo.filename);
								if (wparam == ID_PACK_VIEW || wparam == ID_PACKFILE_VIEW)
									ShellExecute(hwnd, NULL, "notepad", tmp_path, "", SW_SHOW);
								else
									ShellExecute(hwnd, NULL, tmp_path, "", "", SW_SHOW);
							}
							else
								MessageBox(hwnd, "Couldn't open file for viewing", "Pack error", MB_OK | MB_ICONEXCLAMATION);
						}
					return(TRUE);
				case ID_PACK_CHANGEVENDOR:
					if (DialogBoxParam(ghinst, MAKEINTRESOURCE(IDD_CHANGEVENDOR), hwnd, PickWinProc, wparam) == IDOK)
					{
						currentvendor = selectedvendor;
						if (programstate == PS_LOADED)
							epack->PackChangeVendor(vendorstr[currentvendor]);
						strcpy(msgstr, "");
						if (programstate == PS_LOADED)
						{
							strcpy(msgstr, loadedfile);
							ClassEFile::GetFileName(msgstr);
							strcat(msgstr, " - ");
						}
						strcat(msgstr, PRODUCTNAME);
						strcat(msgstr, " '");
						strcat(msgstr, vendorstr[currentvendor]);
						strcat(msgstr, "'");
						SetWindowText(hwnd, msgstr);
					}
					return(TRUE);
				case ID_EXTRA_CONFIGUREVENDORS:
					if (DialogBoxParam(ghinst, MAKEINTRESOURCE(IDD_SELECTVENDOR), hwnd, PickWinProc, wparam) == IDOK)
						WriteVendors();
					else
						ReloadVendors();
					return(TRUE);
				case ID_TOOLS_COMPRESSNONE:
					if (iconfig.compresstype == ECMP_NONE)
						CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSNONE, MF_UNCHECKED | MF_BYCOMMAND);
					else if (iconfig.compresstype == ECMP_EDGE)
						CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSEDGE, MF_UNCHECKED | MF_BYCOMMAND);
					else
						CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSZLIB, MF_UNCHECKED | MF_BYCOMMAND);
					iconfig.compresstype = ECMP_NONE;
					CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSNONE, MF_CHECKED | MF_BYCOMMAND);
					return(TRUE);
				case ID_TOOLS_COMPRESSEDGE:
					if (iconfig.compresstype == ECMP_NONE)
						CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSNONE, MF_UNCHECKED | MF_BYCOMMAND);
					else if (iconfig.compresstype == ECMP_EDGE)
						CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSEDGE, MF_UNCHECKED | MF_BYCOMMAND);
					else
						CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSZLIB, MF_UNCHECKED | MF_BYCOMMAND);
					iconfig.compresstype = ECMP_EDGE;
					CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSEDGE, MF_CHECKED | MF_BYCOMMAND);
					return(TRUE);
				case ID_TOOLS_COMPRESSZLIB:
					if (iconfig.compresstype == ECMP_NONE)
						CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSNONE, MF_UNCHECKED | MF_BYCOMMAND);
					else if (iconfig.compresstype == ECMP_EDGE)
						CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSEDGE, MF_UNCHECKED | MF_BYCOMMAND);
					else
						CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSZLIB, MF_UNCHECKED | MF_BYCOMMAND);
					iconfig.compresstype = ECMP_ZLIB;
					CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSZLIB, MF_CHECKED | MF_BYCOMMAND);
					return(TRUE);
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
						ShellExecute(hwnd, "open", commandkey, regcommand, NULL, SW_SHOW);
					}
					return(TRUE);
				case ID_HELP_ABOUT:
					strcpy(msgstr, "EDGELIB Packer ");
					strcat(msgstr, PROGRAM_VERINFO);
					strcat(msgstr, "\r\nCreate platform independent file archives with compression.\r\n\r\nCopyright (c) 2004-2017 Elements Interactive B.V.");
					MessageBox(hwnd, msgstr, "About EDGELIB Packer", MB_OK | MB_ICONINFORMATION);
					return(TRUE);
			}
			break;
		case WM_DROPFILES:
			bool dragopen;
			dragopen = false;
			count = DragQueryFile((HDROP)wparam, 0xFFFFFFFF, dropstr, MAX_FILESTR);
			if (count == 1)
			{
				DragQueryFile((HDROP)wparam, 0, dropstr, MAX_FILESTR);
				if (strlen(dropstr) > 4)
					if (ClassEStd::StrEqual(&dropstr[strlen(dropstr) - 4], ".epk", false))
					{
						LoadPackFile(dropstr, hwnd);
						dragopen = true;
					}
			}
			if (programstate == PS_LOADED && !dragopen)
			{
				if (count == 1)
					SetWindowText(GetDlgItem(hwnd, IDC_STATUSBAR), "Adding file...");
				else
					SetWindowText(GetDlgItem(hwnd, IDC_STATUSBAR), "Adding files...");
				overwrite = 0;
				itoa(count, dropstr, 10);
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_WAIT)));
				for (ctr = 0; ctr < count; ctr++)
				{
					bool alreadyfound = false;
					DragQueryFile((HDROP)wparam, ctr, dropstr, MAX_FILESTR);
					if (overwrite == 1)
						result = epack->PackAddFile(dropstr, true, iconfig.compresstype);
					else
						result = epack->PackAddFile(dropstr, false, iconfig.compresstype);
					if (result == E_ALREADYFOUND)
					{
						dlgresult = IDNO;
						if (overwrite == 0)
						{
							strcpy(tmpstr, dropstr);
							ClassEFile::GetFileName(tmpstr);
							strcpy(msgstr, "File ");
							strcat(msgstr, tmpstr);
							strcat(msgstr, " already exists. Overwrite?");
							SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
							dlgresult = AllMessageBox(hwnd, msgstr, "Adding file(s)");
							SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_WAIT)));
							if (dlgresult == IDYESALL)
								overwrite = 1;
							else if (dlgresult == IDNOALL)
								overwrite = 2;
						}
						if ((overwrite == 1 && overwrite != 2) || dlgresult == IDYES)
							result = epack->PackAddFile(dropstr, true, iconfig.compresstype);
						else
							alreadyfound = true;
					}
					if (result != E_OK && result != E_ALREADYFOUND)
					{
						strcpy(msgstr, "Unable to add file: ");
						strcat(msgstr, dropstr);
						if (MessageBox(hwnd, msgstr, "Add file error", MB_OKCANCEL | MB_ICONEXCLAMATION) == IDCANCEL)
							break;
					}
					SendMessage(GetDlgItem(hwnd, IDC_PROGRESS), PBM_SETPOS, ctr * 100 / count, 0);
					if (!alreadyfound)
						Sleep(10);
				}
				SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
				SendMessage(GetDlgItem(hwnd, IDC_PROGRESS), PBM_SETPOS, 0, 0);
				BuildList(hwnd);
				SortListView(hwnd, sortcolumn);
			}
			else if (!dragopen)
				MessageBox(hwnd, "You have to create a new pack, or open an existing one to add files.", "EDGELIB pack not loaded", MB_ICONEXCLAMATION | MB_OK);
			return(TRUE);
		case WM_NOTIFY:
			if (((LPNMHDR)lparam)->idFrom == IDC_FILELIST)
			{
				long lv_mark;
				switch(((LPNMHDR)lparam)->code)
				{
					case LVN_GETDISPINFO:
						NMLVDISPINFO *pdispinfo;
						RESITEM *presitem;
						pdispinfo = (NMLVDISPINFO *)lparam;
						if (pdispinfo->item.mask & LVIF_TEXT)
						{
							presitem = (RESITEM *)pdispinfo->item.lParam;
							strcpy(pdispinfo->item.pszText, presitem->coltext[pdispinfo->item.iSubItem]);
						}
						break;
					case LVN_COLUMNCLICK:
						LPNMLISTVIEW pviewinfo;
						pviewinfo = (LPNMLISTVIEW)lparam;
						if (pviewinfo->iSubItem == sortcolumn)
							sortascending = !sortascending;
						else
							sortascending = true;
						SortListView(hwnd, pviewinfo->iSubItem);
						sortcolumn = pviewinfo->iSubItem;
						break;
					case NM_DBLCLK:
					case NM_RETURN:
						lv_mark = ListView_GetSelectionMark(GetDlgItem(hwnd, IDC_FILELIST));
						if (lv_mark != -1)
							if (ListView_GetItemState(GetDlgItem(hwnd, IDC_FILELIST), lv_mark, LVIS_SELECTED))
							{
								RESITEM *ri;
								EPK_FILEINFO finfo;
								long f_id;
								char tmp_path[MAX_PATH];
								lvi.mask = LVIF_PARAM;
								lvi.iItem = lv_mark;
								lvi.iSubItem = 0;
								ListView_GetItem(GetDlgItem(hwnd, IDC_FILELIST), &lvi);
								ri = (RESITEM *)(lvi.lParam);
								f_id = atoi(ri->coltext[0]);
								GetTempPath(sizeof(tmp_path), tmp_path);
								strcat(tmp_path, "\\$EdgePacker");
								if (!epack->Exists(tmp_path))
									epack->NewFolder(tmp_path);
								if (epack->PackExtractFile(tmp_path, f_id, true) == E_OK)
								{
									epack->PackGetFileInfo(&finfo, f_id);
									strcat(tmp_path, "\\");
									strcat(tmp_path, finfo.filename);
									ShellExecute(hwnd, NULL, tmp_path, "", "", SW_SHOW);
								}
								else
									MessageBox(hwnd, "Couldn't open file for viewing", "Pack error", MB_OK | MB_ICONEXCLAMATION);
							}
						break;
					case NM_RCLICK:
						lv_mark = ListView_GetSelectionMark(GetDlgItem(hwnd, IDC_FILELIST));
						if (lv_mark != -1)
							if (ListView_GetItemState(GetDlgItem(hwnd, IDC_FILELIST), lv_mark, LVIS_SELECTED))
							{
								POINT pnt;
								HMENU hmenu = LoadMenu(ghinst, MAKEINTRESOURCE(IDR_QUICKMENU));
								hmenu = GetSubMenu(hmenu, 0);
								GetCursorPos(&pnt);
								TrackPopupMenu(hmenu, 0, pnt.x, pnt.y, 0, hwnd, NULL);
							}
						break;
				}
			}
			break;
		case WM_SIZE:
			SizeList(hwnd);
			return(0);
		case WM_INITDIALOG:
			{
				char *pass_str = (char *)lparam;
				SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(ghinst, MAKEINTRESOURCE(IDI_MAIN)));
				SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(ghinst, MAKEINTRESOURCE(IDI_MAIN)));
				ReadLocaleInfo();
				sortascending = true;
				sortcolumn    = 0;
				SetWindowText(hwnd, msgstr);
				if (iconfig.compresstype == ECMP_NONE)
					CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSNONE, MF_CHECKED | MF_BYCOMMAND);
				else if (iconfig.compresstype == ECMP_EDGE)
					CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSEDGE, MF_CHECKED | MF_BYCOMMAND);
				else
					CheckMenuItem(GetMenu(hwnd), ID_TOOLS_COMPRESSZLIB, MF_CHECKED | MF_BYCOMMAND);
				lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
				lvc.pszText = tmpstr;
				lvc.fmt = LVCFMT_RIGHT;
				for (ctr = 0; ctr < NUM_COLUMNS; ctr++)
				{
					if (ctr == 1)
						lvc.cx = 200;
					else if (ctr == 0)
						lvc.cx = 30;
					else if (ctr == 2 || ctr == 3)
						lvc.cx = 74;
					else if (ctr == 4)
						lvc.cx = 75;
					else if (ctr == 5)
						lvc.cx = 40;
					else if (ctr == 6)
						lvc.cx = 100;
					else
						lvc.cx = 80;
					if (ctr == 0 || ctr == 2 || ctr == 3 || ctr == 5)
						lvc.fmt = LVCFMT_RIGHT;
					else
						lvc.fmt = LVCFMT_LEFT;
					if (ctr == 0)
						sorttype[ctr] = SORT_NUMERIC;
					else if (ctr == 2 || ctr == 3 || ctr == 5)
						sorttype[ctr] = SORT_SEP_NUMERIC;
					else if (ctr == 6)
						sorttype[ctr] = SORT_DATE;
					else
						sorttype[ctr] = SORT_ALPHANUMERIC;
					lvc.iSubItem = ctr;
					LoadString(ghinst, IDS_COLUMN1 + ctr, tmpstr, sizeof(tmpstr));
					ListView_InsertColumn(GetDlgItem(hwnd, IDC_FILELIST), ctr, &lvc);
				}
				SizeList(hwnd);
				ListView_SetExtendedListViewStyle(GetDlgItem(hwnd, IDC_FILELIST), LVS_EX_FULLROWSELECT);
				ClosePackFile(hwnd);
				if (strlen(pass_str) > 2)
					LoadPackFile(pass_str, hwnd);
				SubClass = (WNDPROC)SetWindowLong(GetDlgItem(hwnd, IDC_FILELIST), GWL_WNDPROC, (LONG)SubClass_FileList);
				SetWindowLong(GetDlgItem(hwnd, IDC_FILELIST), GWL_USERDATA, (unsigned long)SubClass);
				SubClass = (WNDPROC)SetWindowLong(hwnd, GWL_WNDPROC, (LONG)SubClass_AppWindow);
				SetWindowLong(hwnd, GWL_USERDATA, (unsigned long)SubClass);
				return(TRUE);
			}
		case WM_CLOSE:
			EndDialog(hwnd, IDABORT);
			return(TRUE);
		case WM_DESTROY:
			if (programstate != PS_IDLE)
				ClosePackFile(hwnd);
			return(TRUE);
	}
	return(FALSE);
}
