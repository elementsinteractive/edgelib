// code/settings.h
// Handles screensaver settings
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Structure with settings
typedef struct
{
	unsigned char style;
	unsigned char palette;
	unsigned char display;
	bool morelines;
	bool morepoints;
	bool slowinterpolate;
}SAVERSETTINGS;

/////////////////////////////////////////////////////////////////////
// Functions                                                       //
/////////////////////////////////////////////////////////////////////

//Load settings from ini
void LoadSettings(SAVERSETTINGS *settings)
{
	ClassEIni ini;
	settings->style = MAX_STATES + 1;
	settings->palette = MAX_PALETTES + 1;
	settings->display = 0;
	settings->morelines = false;
	settings->morepoints = true;
	settings->slowinterpolate = false;
	if (ini.Open("edgesaver.ini", EFOF_READ))
	{
		char key[MAX_ESTR], value[MAX_ESTR];
		while(ini.ReadLine(key, value))
		{
			if (ClassEStd::StrEqual(key, "style", false))
			{
				settings->style = (unsigned char)ClassEStd::StrToInt(value);
				if (settings->style > MAX_STATES + 1)
					settings->style = MAX_STATES + 1;
			}
			else if (ClassEStd::StrEqual(key, "palette", false))
			{
				settings->palette = (unsigned char)ClassEStd::StrToInt(value);
				if (settings->palette > MAX_PALETTES + 1)
					settings->palette = MAX_PALETTES + 1;
			}
			else if (ClassEStd::StrEqual(key, "display", false))
			{
				settings->display = (unsigned char)ClassEStd::StrToInt(value);
				if (settings->display > 3)
					settings->display = 3;
			}
			else if (ClassEStd::StrEqual(key, "morelines", false))
			{
				if (ClassEStd::StrToInt(value) == 1)
					settings->morelines = true;
			}
			else if (ClassEStd::StrEqual(key, "morepoints", false))
			{
				if (ClassEStd::StrToInt(value) == 0)
					settings->morepoints = false;
			}
			else if (ClassEStd::StrEqual(key, "slowinterpolate", false))
			{
				if (ClassEStd::StrToInt(value) == 1)
					settings->slowinterpolate = true;
			}
		}
		ini.Close();
	}
}

//Save settings to ini
void SaveSettings(SAVERSETTINGS *settings)
{
	ClassEIni ini;
	if (ini.New("edgesaver.ini"))
	{
		ini.WriteComment("EDGELIB screensaver settings");
		ini.WriteNewline();
		ini.WriteLine("style", settings->style);
		ini.WriteLine("palette", settings->palette);
		ini.WriteLine("display", settings->display);
		if (settings->morelines)
			ini.WriteLine("morelines", 1);
		if (settings->morepoints)
			ini.WriteLine("morepoints", 1);
		if (settings->slowinterpolate)
			ini.WriteLine("slowinterpolate", 1);
		ini.Close();
	}
}

/////////////////////////////////////////////////////////////////////
// Configuration dialog for desktop builds                         //
/////////////////////////////////////////////////////////////////////

#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)

BOOL CALLBACK WinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	switch(wmsg)
	{
		case WM_COMMAND:
			switch(wparam)
			{
				case IDABOUT:
					MessageBox(hwnd, "EdgeSaver\r\nA multi-platform screensaver created with the EDGELIB SDK.\r\n\r\nCopyright (c) 2004-2017 Elements Interactive B.V.", "About EdgeSaver", MB_OK | MB_ICONINFORMATION);
					return(TRUE);
				case IDOK:
					SAVERSETTINGS settings;
					settings.style = (unsigned char)SendMessage(GetDlgItem(hwnd, IDC_STYLE), CB_GETCURSEL, 0, 0);
					settings.display = (unsigned char)SendMessage(GetDlgItem(hwnd, IDC_DISPLAY), CB_GETCURSEL, 0, 0);
					settings.palette = (unsigned char)SendMessage(GetDlgItem(hwnd, IDC_PALETTE), CB_GETCURSEL, 0, 0);
					settings.morelines = false;
					settings.morepoints = false;
					settings.slowinterpolate = false;
					if (IsDlgButtonChecked(hwnd, IDC_MORELINES) == BST_CHECKED)
						settings.morelines = true;
					if (IsDlgButtonChecked(hwnd, IDC_MOREPOINTS) == BST_CHECKED)
						settings.morepoints = true;
					if (IsDlgButtonChecked(hwnd, IDC_SLOWINTERPOLATE) == BST_CHECKED)
						settings.slowinterpolate = true;
					SaveSettings(&settings);
				case IDCANCEL:
					EndDialog(hwnd, IDABORT);
					return(TRUE);
			}
			break;
		case WM_INITDIALOG:
			SAVERSETTINGS settings;
			LoadSettings(&settings);
			SendMessage(GetDlgItem(hwnd, IDC_STYLE), CB_ADDSTRING, 0, (LPARAM)"Lines");
			SendMessage(GetDlgItem(hwnd, IDC_STYLE), CB_ADDSTRING, 0, (LPARAM)"Bouncing dots");
			SendMessage(GetDlgItem(hwnd, IDC_STYLE), CB_ADDSTRING, 0, (LPARAM)"Following dots");
			SendMessage(GetDlgItem(hwnd, IDC_STYLE), CB_ADDSTRING, 0, (LPARAM)"Fountain");
			SendMessage(GetDlgItem(hwnd, IDC_STYLE), CB_ADDSTRING, 0, (LPARAM)"Fire");
			SendMessage(GetDlgItem(hwnd, IDC_STYLE), CB_ADDSTRING, 0, (LPARAM)"Random");
			SendMessage(GetDlgItem(hwnd, IDC_STYLE), CB_ADDSTRING, 0, (LPARAM)"Shuffle");
			SendMessage(GetDlgItem(hwnd, IDC_DISPLAY), CB_ADDSTRING, 0, (LPARAM)"Native (emulated palette)");
			SendMessage(GetDlgItem(hwnd, IDC_DISPLAY), CB_ADDSTRING, 0, (LPARAM)"Native");
			SendMessage(GetDlgItem(hwnd, IDC_DISPLAY), CB_ADDSTRING, 0, (LPARAM)"Medium resolution");
			SendMessage(GetDlgItem(hwnd, IDC_DISPLAY), CB_ADDSTRING, 0, (LPARAM)"Low resolution");
			SendMessage(GetDlgItem(hwnd, IDC_PALETTE), CB_ADDSTRING, 0, (LPARAM)"Red");
			SendMessage(GetDlgItem(hwnd, IDC_PALETTE), CB_ADDSTRING, 0, (LPARAM)"Blue");
			SendMessage(GetDlgItem(hwnd, IDC_PALETTE), CB_ADDSTRING, 0, (LPARAM)"Green");
			SendMessage(GetDlgItem(hwnd, IDC_PALETTE), CB_ADDSTRING, 0, (LPARAM)"Random");
			SendMessage(GetDlgItem(hwnd, IDC_PALETTE), CB_ADDSTRING, 0, (LPARAM)"Shuffle");
			SendMessage(GetDlgItem(hwnd, IDC_STYLE), CB_SETCURSEL, settings.style, 0);
			SendMessage(GetDlgItem(hwnd, IDC_DISPLAY), CB_SETCURSEL, settings.display, 0);
			SendMessage(GetDlgItem(hwnd, IDC_PALETTE), CB_SETCURSEL, settings.palette, 0);
			if (settings.morelines)
				CheckDlgButton(hwnd, IDC_MORELINES, BST_CHECKED);
			if (settings.morepoints)
				CheckDlgButton(hwnd, IDC_MOREPOINTS, BST_CHECKED);
			if (settings.slowinterpolate)
				CheckDlgButton(hwnd, IDC_SLOWINTERPOLATE, BST_CHECKED);
			break;
		case WM_CLOSE:
			EndDialog(hwnd, IDABORT);
			return(TRUE);
	}
	return(FALSE);
}

#endif
