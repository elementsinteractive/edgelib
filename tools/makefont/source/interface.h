// interface.h
// Contains the main wrapper class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Important headers
#include <direct.h>

//Freetype
#pragma comment(lib, "freetype235.lib")
#include <ft2build.h>
#include FT_FREETYPE_H

//Main definitions
#define MAX_CHARSET    512
#define MAX_FONTLIST  1024
#define MAX_FONTFNAME   64

//Flag definitions
#define PFL_VARFONT      1
#define PFL_SAVE24BIT    2
#define PFL_ANTIALIAS    4
#define PFL_INTEGRATE    8

//Font style flags
#define FS_NONE          0
#define FS_BOLD          1
#define FS_ITALIC        2

//Program configuration
typedef struct
{
	RECT paddfont;
	WCHAR charlist[MAX_CHARSET];
	long pixwidth;
	unsigned long color;
	unsigned long size;
	unsigned char flags;
}ECONFIG;

//Font character settings
typedef struct
{
	RECT rc;
	long yoffset;
}FONTCHAR;

//Font name entry
typedef struct
{
	char fontfile[MAX_FONTFNAME];
	unsigned char style;
}FONTENTRY;

//Globals
int charsetlistsel = 0;

/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

//The main class
class ClassMain : public ClassEdge
{
	private:
		ECONFIG cfg;
		RECT rcchar[MAX_CHARSET];
		FONTENTRY fontentry[MAX_FONTLIST];
		char programpath[512];
		char savefn[512];
		unsigned long numfontentries;
		void IntToHex(char *dst, int v);
		bool SelectSaveFile(HWND hwnd, char *caption, char *desc, char *ext);
		void LoadCfg(void);
		void SaveCfg(void);
		void CheckWinConfig(HWND hwnd);
		bool Generator(char *font_file, char *png_file);
		bool Generator(char *font_file, char *png_file, FONTCHAR *entries, long xmax, long ymin, long ymax, unsigned long surfwidth, unsigned long surfheight);
		bool GetTTFSettings(char *font_file, FONTCHAR *entries);
		bool ProcessFile(char *font_file, char *png_file);
	public:
		ClassMain(void);
		~ClassMain(void);
		ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config);
		ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
		ERESULT OnInit(ENATIVETYPE instance);
		ERESULT OnMinimize(void);
		ERESULT OnRestore(void);
		ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display);
		void OnDisplayChange(ClassEDisplay *display);
		ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
		void OnExit(ENATIVETYPE instance);
		void OnButtonDown(unsigned long bnr, EBUTTONLIST *blist);
		void OnButtonUp(unsigned long bnr, EBUTTONLIST *blist);
		void OnWheel(long direction);
		void OnStylusUp(POINT pnt);
		void OnStylusDown(POINT pnt);
		void OnStylusDouble(POINT pnt);
		void OnStylusMove(POINT pnt);
		void OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer);
		bool OnNativeEvent(ENATIVETYPE winhandle, ENATIVETYPE msg, ENATIVETYPE param1, ENATIVETYPE param2);
};


/////////////////////////////////////////////////////////////////////
// Global functions
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
	if (RegCreateKeyExA(keyroot, keypath, 0, NULL, 0, ka, NULL, &keyreg, &result) == ERROR_SUCCESS)
	{
		if (RegQueryValueExA(keyreg, keyname, NULL, &rtype, (PBYTE)dst, &rsize) != ERROR_SUCCESS)
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

//Callback function for character set
BOOL CALLBACK CharsetWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	switch(wmsg)
	{
		case WM_COMMAND:
			switch(wparam)
			{
				case IDCANCEL:
				case IDOK:
					charsetlistsel = SendMessage(GetDlgItem(hwnd, IDC_CHARSETLIST), LB_GETCURSEL, 0, 0);
					EndDialog(hwnd, wparam);
					break;
			}
			break;
		case WM_CLOSE:
			EndDialog(hwnd, IDABORT);
			return(TRUE);
		case WM_INITDIALOG:
			SendMessage(GetDlgItem(hwnd, IDC_CHARSETLIST), LB_ADDSTRING, 0, (LPARAM)"Ascii");
			SendMessage(GetDlgItem(hwnd, IDC_CHARSETLIST), LB_ADDSTRING, 0, (LPARAM)"Extended ascii");
			SendMessage(GetDlgItem(hwnd, IDC_CHARSETLIST), LB_SETCURSEL, 0, 0);
			return(TRUE);
	}
	return(FALSE);
}

/////////////////////////////////////////////////////////////////////
// ClassMain: private                                              //
/////////////////////////////////////////////////////////////////////

//Convert integer to hex string (color value)
void ClassMain::IntToHex(char *dst, int v)
{
	for (unsigned long ctr = 0; ctr < 6; ctr++)
	{
		if (v % 16 < 10)
			dst[5 - ctr] = '0' + v % 16;
		else
			dst[5 - ctr] = 'A' + v % 16 - 10;
		v /= 16;
	}
	dst[6] = 0;
}

//Open the save file dialog box
bool ClassMain::SelectSaveFile(HWND hwnd, char *caption, char *desc, char *ext)
{
	OPENFILENAMEA ofn;
	char filter[512], filename[512];
	unsigned short len, ctr;
	if (programpath[0] == 0)
		_getcwd(programpath, 512);
	memset(filter, 0, 512);
	memset(filename, 0, 512);
	ClassEStd::StrCpy(filter,desc);
	ClassEStd::StrCpy(&filter[ClassEStd::StrLen(filter)], " (*");
	ClassEStd::StrCpy(&filter[ClassEStd::StrLen(filter)], ext);
	ClassEStd::StrCpy(&filter[ClassEStd::StrLen(filter)], ")");
	ClassEStd::StrCpy(&filter[ClassEStd::StrLen(filter)+1], "*");
	ClassEStd::StrCpy(&filter[ClassEStd::StrLen(filter)+2], ext);
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
	ofn.lpstrInitialDir = programpath;
	ofn.lpstrTitle = caption;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = ext;
	ofn.lCustData = 0;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	if (GetSaveFileNameA(&ofn) == FALSE)
		return(false);
	if (filename[0] == 0)
		return(false);
	strcpy(programpath, filename);
	len = strlen(programpath) - 1;
	for (ctr = len; ctr > 0; ctr--)
	{
		if (programpath[ctr] == '\\')
			break;
		programpath[ctr] = 0;
	}
	strcpy(savefn, filename);
	return(true);
}

//Load configuration
void ClassMain::LoadCfg(void)
{
	ClassEFile ef;
	unsigned long ctr;
	for (ctr = 32; ctr < 127; ctr++)
	{
		cfg.charlist[ctr - 32] = (unsigned short)ctr;
		cfg.charlist[ctr - 31] = 0;
	}
	cfg.flags = PFL_VARFONT;
	cfg.size = 12;
	cfg.color = 0;
	cfg.pixwidth = 0;
	SetRect(&cfg.paddfont, 0, 0, 0, 0);
	if (ef.Open("makefont.cfg", EFOF_READ))
	{
		ef.Read(&cfg, sizeof(cfg));
		ef.Close();
	}
}

//Save configuration
void ClassMain::SaveCfg(void)
{
	ClassEFile ef;
	if (ef.New("makefont.cfg"))
	{
		ef.Write(&cfg, sizeof(cfg));
		ef.Close();
	}
}

//Read new config from window items
void ClassMain::CheckWinConfig(HWND hwnd)
{
	char valstr[16];
	cfg.flags = 0;
	if (IsDlgButtonChecked(hwnd, IDC_VARFONT))
		cfg.flags |= PFL_VARFONT;
	if (IsDlgButtonChecked(hwnd, IDC_SAVE24BIT))
		cfg.flags |= PFL_SAVE24BIT;
	if (IsDlgButtonChecked(hwnd, IDC_ANTIALIAS))
		cfg.flags |= PFL_ANTIALIAS;
	//if (IsDlgButtonChecked(hwnd, IDC_INTEGRATE))
	//	cfg.flags |= PFL_INTEGRATE;
	GetWindowTextW(GetDlgItem(hwnd, IDC_CHARLIST), (LPWSTR)cfg.charlist, MAX_CHARSET);
	GetWindowTextA(GetDlgItem(hwnd, IDC_FONTSIZE), valstr, 16);
	cfg.size = ClassEStd::StrToInt(valstr);
	GetWindowTextA(GetDlgItem(hwnd, IDC_FONTCOLOR), valstr, 16);
	cfg.color = ClassEStd::StrToInt(valstr, 16);
	GetWindowTextA(GetDlgItem(hwnd, IDC_PIXELSROW), valstr, 16);
	cfg.pixwidth = ClassEStd::StrToInt(valstr);
	GetWindowTextA(GetDlgItem(hwnd, IDC_PADDLEFT), valstr, 16);
	cfg.paddfont.left = ClassEStd::StrToInt(valstr);
	GetWindowTextA(GetDlgItem(hwnd, IDC_PADDTOP), valstr, 16);
	cfg.paddfont.top = ClassEStd::StrToInt(valstr);
	GetWindowTextA(GetDlgItem(hwnd, IDC_PADDRIGHT), valstr, 16);
	cfg.paddfont.right = ClassEStd::StrToInt(valstr);
	GetWindowTextA(GetDlgItem(hwnd, IDC_PADDBOTTOM), valstr, 16);
	cfg.paddfont.bottom = ClassEStd::StrToInt(valstr);
	if (cfg.paddfont.left < 0)
		cfg.paddfont.left = 0;
	if (cfg.paddfont.top < 0)
		cfg.paddfont.top = 0;
	if (cfg.paddfont.right < 0)
		cfg.paddfont.right = 0;
	if (cfg.paddfont.bottom < 0)
		cfg.paddfont.bottom = 0;
}

//Font generator
bool ClassMain::Generator(char *font_file, char *png_file)
{
	E2DSurface tmpsurf, fontsurf;
	COLORREF paldata[256];
	FT_Library font_library;
	FT_Face    font_face;
	FT_Bitmap  bitmap;
	FT_GlyphSlot cur_glyph;
	FT_Glyph_Metrics glyph_metrics;
	int glyphcount, ctr, surfx;
	if (FT_Init_FreeType(&font_library))
		return(false);
	if (FT_New_Face(font_library, font_file, 0, &font_face))
		return(false);
	if (FT_Set_Char_Size(font_face, 0, cfg.size * 64, 96, 96))
		return(false);
	for (ctr = 0; ctr < 256; ctr++)
		paldata[ctr] = ((cfg.color & 0xFF) << 16) | ((cfg.color & 0xFF00)) | ((cfg.color & 0xFF0000) >> 16) | (ctr << 24);
	ecd.dsp->SetPalette(paldata, 0, 256);
	if (ecd.dsp->CreateSurface(&tmpsurf, cfg.size * ClassEStd::StrLen(cfg.charlist), cfg.size * 5 / 2, EST_DSPINDEXED8) != E_OK)
	{
		FT_Done_FreeType(font_library);
		return(false);
	}
	glyphcount = (int)font_face->num_glyphs;
	ctr = 0;
	surfx = 0;
	while(cfg.charlist[ctr] != 0)
	{
		bool illegalchar = false;
		long idx = FT_Get_Char_Index(font_face, cfg.charlist[ctr]);
		if (idx > 0 && idx < 0xFFFFFFF)
		{
			if (!FT_Load_Glyph(font_face, idx, FT_LOAD_DEFAULT))
			{
				cur_glyph = font_face->glyph;
				if (cur_glyph->format != FT_GLYPH_FORMAT_BITMAP)
				{
					if (cfg.flags & PFL_ANTIALIAS)
					{
						if (FT_Render_Glyph(font_face->glyph, FT_RENDER_MODE_NORMAL))
							illegalchar = true;
					}
					else
					{
						if (FT_Render_Glyph(font_face->glyph, FT_RENDER_MODE_MONO))
							illegalchar = true;
					}
				}
				if (!illegalchar)
				{
					bitmap = cur_glyph->bitmap;
					glyph_metrics = cur_glyph->metrics;
					long x, y, surfy = cfg.size * 3 / 2 - glyph_metrics.horiBearingY / 64;
					if (bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
					{
						for (y = 0; y < bitmap.rows; y++)
							for (x = 0; x < bitmap.width; x++)
								tmpsurf.PutPixel(surfx + x, surfy + y, bitmap.buffer[y * bitmap.pitch + x]);
					}
					else
					{
						for (y = 0; y < bitmap.rows; y++)
							for (x = 0; x < bitmap.width; x++)
							{
								unsigned char bitmask = bitmap.buffer[y * bitmap.pitch + x / 8];
								unsigned char col = 0;
								if ((bitmask >> (7 - x % 8)) & 1)
									col = 255;
								tmpsurf.PutPixel(surfx + x, surfy + y, col);
							}
					}
					SetRect(&rcchar[ctr], surfx, surfy, surfx + bitmap.width, surfy + bitmap.rows);
					surfx += 1 + bitmap.width;
				}
			}
			else
				illegalchar = true;
		}
		else
			illegalchar = true;
		if (illegalchar)
			rcchar[ctr].right = -1;
		ctr++;
	}
	long miny = tmpsurf.GetHeight(), maxy = 0, maxwidth = 0, maxcharwidth = 1;
	ctr = 0;
	while(cfg.charlist[ctr] != 0)
	{
		if (cfg.charlist[ctr] == ' ')
			rcchar[ctr].right = -1;
		if (rcchar[ctr].right >= 0)
		{
			if (rcchar[ctr].top < miny)
				miny = rcchar[ctr].top;
			if (rcchar[ctr].bottom > maxy)
				maxy = rcchar[ctr].bottom;
			if (rcchar[ctr].right - rcchar[ctr].left > maxcharwidth)
				maxcharwidth = rcchar[ctr].right - rcchar[ctr].left;
			maxwidth += rcchar[ctr].right - rcchar[ctr].left + 1;
		}
		else
		{
			if (cfg.charlist[ctr] == ' ')
				maxwidth += 4 + cfg.size / 3;
			else
				maxwidth += 2;
		}
		if (ctr == 0)
			maxwidth--;
		ctr++;
	}
	if (miny > maxy)
	{
		miny = maxy;
		maxy++;
	}
	if (cfg.flags & PFL_VARFONT)
		maxy += 2;
	else
		maxwidth = maxcharwidth * ClassEStd::StrLen(cfg.charlist);
	if (ecd.dsp->CreateSurface(&fontsurf, maxwidth, maxy - miny, EST_DSPINDEXED8) == E_OK)
	{
		ctr = 0;
		surfx = 0;
		while(cfg.charlist[ctr] != 0)
		{
			if (cfg.flags & PFL_VARFONT)
			{
				if (rcchar[ctr].right >= 0)
				{
					fontsurf.DrawLine(surfx, 0, surfx + rcchar[ctr].right - rcchar[ctr].left - 1, 0, 255);
					fontsurf.BltFast(surfx, 2 + rcchar[ctr].top - miny, &tmpsurf, &rcchar[ctr]);
					surfx += rcchar[ctr].right - rcchar[ctr].left + 1;
				}
				else
				{
					if (cfg.charlist[ctr] == ' ')
					{
						fontsurf.DrawLine(surfx, 0, surfx + 2 + cfg.size / 3, 0, 255);
						surfx += 4 + cfg.size / 3;
					}
					else
					{
						fontsurf.PutPixel(surfx, 0, 255);
						surfx += 2;
					}
				}
			}
			else
			{
				if (rcchar[ctr].right >= 0)
					fontsurf.BltFast(ctr * maxcharwidth + (maxcharwidth - (rcchar[ctr].right - rcchar[ctr].left)) / 2, rcchar[ctr].top - miny, &tmpsurf, &rcchar[ctr]);
			}
			ctr++;
		}
		fontsurf.WritePNG(png_file);
		if (cfg.flags & PFL_SAVE24BIT)
		{
			E2DSurfaceRGBA surf24;
			char dst_file[512];
			ClassEStd::StrCpy(dst_file, png_file);
			for (ctr = 0; ctr < ClassEStd::StrLen(dst_file); ctr++)
				if (dst_file[ClassEStd::StrLen(dst_file) - 1 - ctr] == '.')
				{
					ClassEStd::StrCpy(&dst_file[ClassEStd::StrLen(dst_file) - 1 - ctr], "_24.png");
					break;
				}
			if (ecd.dsp->CreateSurface(&surf24, png_file, EST_DSPTRUE32) == E_OK)
				surf24.WritePNG(dst_file);
		}
	}
	FT_Done_FreeType(font_library);
	return(true);
}

//Font generator
bool ClassMain::Generator(char *font_file, char *png_file, FONTCHAR *entries, long xmax, long ymin, long ymax, unsigned long surfwidth, unsigned long surfheight)
{
	E2DSurface tmpsurf, fontsurf;
	COLORREF paldata[256];
	FT_Library font_library;
	FT_Face    font_face;
	FT_Bitmap  bitmap;
	FT_GlyphSlot cur_glyph;
	FT_Glyph_Metrics glyph_metrics;
	int glyphcount, ctr, surfx, numlines;
	if (FT_Init_FreeType(&font_library))
		return(false);
	if (FT_New_Face(font_library, font_file, 0, &font_face))
		return(false);
	if (FT_Set_Char_Size(font_face, 0, cfg.size * 64, 96, 96))
		return(false);
	for (ctr = 0; ctr < 256; ctr++)
		paldata[ctr] = ((cfg.color & 0xFF) << 16) | ((cfg.color & 0xFF00)) | ((cfg.color & 0xFF0000) >> 16) | (ctr << 24);
	ecd.dsp->SetPalette(paldata, 0, 256);
	if (ecd.dsp->CreateSurface(&tmpsurf, cfg.size * 3 / 2, cfg.size * 5 / 2, EST_DSPINDEXED8) != E_OK)
	{
		FT_Done_FreeType(font_library);
		return(false);
	}
	if (ecd.dsp->CreateSurface(&fontsurf, surfwidth, surfheight, EST_DSPINDEXED8) != E_OK)
	{
		FT_Done_FreeType(font_library);
		return(false);
	}
	glyphcount = (int)font_face->num_glyphs;
	ctr = 0;
	surfx = 0;
	numlines = 0;
	while(cfg.charlist[ctr] != 0)
	{
		bool illegalchar = false;
		long idx = FT_Get_Char_Index(font_face, cfg.charlist[ctr]);
		tmpsurf.Clear();
		if (entries[ctr].rc.right > 0 && idx > 0 && idx < 0xFFFFFFF)
		{
			if (!FT_Load_Glyph(font_face, idx, FT_LOAD_DEFAULT))
			{
				cur_glyph = font_face->glyph;
				if (cur_glyph->format != FT_GLYPH_FORMAT_BITMAP)
				{
					if (cfg.flags & PFL_ANTIALIAS)
					{
						if (FT_Render_Glyph(font_face->glyph, FT_RENDER_MODE_NORMAL))
							illegalchar = true;
					}
					else
					{
						if (FT_Render_Glyph(font_face->glyph, FT_RENDER_MODE_MONO))
							illegalchar = true;
					}
				}
				if (!illegalchar)
				{
					long x, y;
					bitmap = cur_glyph->bitmap;
					glyph_metrics = cur_glyph->metrics;
					if (bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
					{
						for (y = 0; y < bitmap.rows; y++)
							for (x = 0; x < bitmap.width; x++)
								tmpsurf.PutPixel(x, y, bitmap.buffer[y * bitmap.pitch + x]);
					}
					else
					{
						for (y = 0; y < bitmap.rows; y++)
							for (x = 0; x < bitmap.width; x++)
							{
								unsigned char bitmask = bitmap.buffer[y * bitmap.pitch + x / 8];
								unsigned char col = 0;
								if ((bitmask >> (7 - x % 8)) & 1)
									col = 255;
								tmpsurf.PutPixel(x, y, col);
							}
					}
				}
			}
			else
				illegalchar = true;
		}
		else
			illegalchar = true;
		RECT rc;
		unsigned long charwidth = entries[ctr].rc.right - entries[ctr].rc.left, lineheight = (ymax - ymin + cfg.paddfont.top + cfg.paddfont.bottom);
		long fixoffs = xmax / 2 - charwidth / 2;
		if (fixoffs < 0)
			fixoffs = 0;
		if (fixoffs + charwidth > (unsigned long)xmax)
			fixoffs = xmax - charwidth;
		if (cfg.flags & PFL_VARFONT)
			lineheight += 3;
		else
			charwidth = xmax;
		if (charwidth == 0 || illegalchar) //Other solution: discard unfound font characters (configurable?)
			charwidth = 1;
		if (cfg.pixwidth > 0 && surfx + (long)charwidth + cfg.paddfont.left + cfg.paddfont.right > cfg.pixwidth)
		{
			if ((long)surfwidth < surfx - 1)
				surfwidth = surfx - 1;
			numlines++;
			SetRect(&rc, 0, 0, entries[ctr].rc.right, entries[ctr].rc.bottom);
			if (cfg.flags & PFL_VARFONT)
			{
				fontsurf.DrawLine(0, numlines * lineheight, charwidth + cfg.paddfont.left + cfg.paddfont.right - 1, numlines * lineheight, 255);
				fontsurf.BltFast(cfg.paddfont.left, 2 + numlines * lineheight + cfg.paddfont.top + entries[ctr].yoffset - ymin, &tmpsurf, &rc);
			}
			else
				fontsurf.BltFast(cfg.paddfont.left + fixoffs, numlines * lineheight + cfg.paddfont.top + entries[ctr].yoffset - ymin, &tmpsurf, &rc);
			surfx = charwidth + cfg.paddfont.left + cfg.paddfont.right + 1;
		}
		else
		{
			SetRect(&rc, 0, 0, entries[ctr].rc.right, entries[ctr].rc.bottom);
			if (cfg.flags & PFL_VARFONT)
			{
				fontsurf.DrawLine(surfx, numlines * lineheight, surfx + charwidth + cfg.paddfont.left + cfg.paddfont.right - 1, numlines * lineheight, 255);
				fontsurf.BltFast(surfx + cfg.paddfont.left, 2 + numlines * lineheight + cfg.paddfont.top + entries[ctr].yoffset - ymin, &tmpsurf, &rc);
			}
			else
				fontsurf.BltFast(surfx + fixoffs + cfg.paddfont.left, numlines * lineheight + cfg.paddfont.top + entries[ctr].yoffset - ymin, &tmpsurf, &rc);
			surfx += charwidth + cfg.paddfont.left + cfg.paddfont.right + 1;
		}
		ctr++;
	}
	fontsurf.WritePNG(png_file);
	if (cfg.flags & PFL_SAVE24BIT)
	{
		E2DSurfaceRGBA surf24;
		char dst_file[512];
		ClassEStd::StrCpy(dst_file, png_file);
		for (ctr = 0; ctr < ClassEStd::StrLen(dst_file); ctr++)
			if (dst_file[ClassEStd::StrLen(dst_file) - 1 - ctr] == '.')
			{
				ClassEStd::StrCpy(&dst_file[ClassEStd::StrLen(dst_file) - 1 - ctr], "_24.png");
				break;
			}
		if (ecd.dsp->CreateSurface(&surf24, png_file, EST_DSPTRUE32) == E_OK)
			surf24.WritePNG(dst_file);
	}
	FT_Done_FreeType(font_library);
	return(true);
}

//Return information for each character
bool ClassMain::GetTTFSettings(char *font_file, FONTCHAR *entries)
{
	FT_Library font_library;
	FT_Face    font_face;
	FT_Bitmap  bitmap;
	FT_GlyphSlot cur_glyph;
	FT_Glyph_Metrics glyph_metrics;
	int glyphcount, ctr;
	if (FT_Init_FreeType(&font_library))
		return(false);
	if (FT_New_Face(font_library, font_file, 0, &font_face))
		return(false);
	if (FT_Set_Char_Size(font_face, 0, cfg.size * 64, 96, 96))
		return(false);
	glyphcount = (int)font_face->num_glyphs;
	ctr = 0;
	while(cfg.charlist[ctr] != 0)
	{
		bool illegalchar = false;
		long idx = FT_Get_Char_Index(font_face, cfg.charlist[ctr]);
		if (idx > 0 && idx < 0xFFFFFFF)
		{
			if (!FT_Load_Glyph(font_face, idx, FT_LOAD_DEFAULT))
			{
				cur_glyph = font_face->glyph;
				if (cur_glyph->format != FT_GLYPH_FORMAT_BITMAP)
				{
					if (cfg.flags & PFL_ANTIALIAS)
					{
						if (FT_Render_Glyph(font_face->glyph, FT_RENDER_MODE_NORMAL))
							illegalchar = true;
					}
					else
					{
						if (FT_Render_Glyph(font_face->glyph, FT_RENDER_MODE_MONO))
							illegalchar = true;
					}
				}
				if (!illegalchar)
				{
					bitmap = cur_glyph->bitmap;
					glyph_metrics = cur_glyph->metrics;
					entries[ctr].yoffset = (long)cfg.size * 3 / 2 - (long)glyph_metrics.horiBearingY / 64;
					SetRect(&entries[ctr].rc, 0, 0, bitmap.width, bitmap.rows);
					if (cfg.charlist[ctr] == ' ')
						SetRect(&entries[ctr].rc, 0, 0, cfg.size / 2, 1);
				}
			}
			else
				illegalchar = true;
		}
		else
			illegalchar = true;
		if (illegalchar)
			SetRect(&entries[ctr].rc, 0, 0, 0, 0);
		ctr++;
	}
	FT_Done_FreeType(font_library);
	return(true);
}

//Process font file and generate image
bool ClassMain::ProcessFile(char *font_file, char *png_file)
{
	FONTCHAR *entries;
	unsigned long ctr, charcount = ClassEStd::StrLen(cfg.charlist);
	bool success = false;
	entries = (FONTCHAR *)ClassEMemory::Alloc(charcount * sizeof(FONTCHAR));
	if (entries)
	{
		if (GetTTFSettings(font_file, entries))
		{
			long ymin = 0xFFFFFF, ymax = -0xFFFFFF, xmax = 0, surfwidth = 0, surfheight = 0;
			for (ctr = 0; ctr < charcount; ctr++)
				if (entries[ctr].rc.right > 0)
				{
					long charheight = entries[ctr].rc.bottom - entries[ctr].rc.top;
					if (entries[ctr].yoffset < ymin)
						ymin = entries[ctr].yoffset;
					if (entries[ctr].yoffset + charheight > ymax)
						ymax = entries[ctr].yoffset + charheight;
					if (entries[ctr].rc.right - entries[ctr].rc.left > xmax)
						xmax = entries[ctr].rc.right - entries[ctr].rc.left;
				}
			if (ymin < ymax)
			{
				long surfx = 0, numlines = 1;
				success = true;
				for (ctr = 0; ctr < charcount; ctr++)
				{
					unsigned long charwidth = entries[ctr].rc.right - entries[ctr].rc.left;
					if (!(cfg.flags & PFL_VARFONT))
						charwidth = xmax;
					if (charwidth == 0) //Other solution: discard unfound font characters (configurable?)
						charwidth = 1;
					if (cfg.pixwidth > 0 && (long)charwidth + cfg.paddfont.left + cfg.paddfont.right > cfg.pixwidth)
					{
						success = false;
						break;
					}
					if (cfg.pixwidth > 0 && surfx + (long)charwidth + cfg.paddfont.left + cfg.paddfont.right > cfg.pixwidth)
					{
						if (surfwidth < surfx - 1)
							surfwidth = surfx - 1;
						numlines++;
						surfx = charwidth + cfg.paddfont.left + cfg.paddfont.right + 1;
					}
					else
						surfx += charwidth + cfg.paddfont.left + cfg.paddfont.right + 1;
				}
				if (success)
				{
					if (surfwidth < surfx - 1)
						surfwidth = surfx - 1;
					if (cfg.flags & PFL_VARFONT)
						surfheight = numlines * (ymax - ymin + 3 + cfg.paddfont.top + cfg.paddfont.bottom);
					else
						surfheight = numlines * (ymax - ymin + cfg.paddfont.top + cfg.paddfont.bottom);
					success = Generator(font_file, png_file, entries, xmax, ymin, ymax, surfwidth, surfheight);
				}
			}
		}
	}
	ClassEMemory::DeAlloc(entries);
	return(success);
}


/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////

//ClassMain: constructor
ClassMain::ClassMain(void)
{
	numfontentries = 0;
	//AfxInitRichEdit2();
}

//ClassMain: destructor
ClassMain::~ClassMain(void)
{
}

//Configure framework
ERESULT ClassMain::OnFrameworkConfig(EFRAMEWORKCONFIG *config)
{
	config->ffwdkey         = 'A';                //A key for fast forwarding (multiplies framerate by 2.5)
	config->slomokey        = 'S';                //A key for slow motion     (halves framerate)
	config->maxfps          = 60;                  //Limit the maximum frames per second by 60
	config->flags           = EFL_MANUALFLIP | EFL_IGNOREREDRAW;     //No extra flag options
	config->numconnections  = 1;                  //Keep 1 active network connection
	config->smartfps        = false;              //GP2X: No smartfps needed
	config->focusminimize   = false;            //Desktop PC: Don't minimize the application when the focus is lost
	config->jogtiltarrow    = true;               //Emulate the UIQ jog as left/right arrows
	config->focussuspend    = true;               //Desktop PC: suspend the application when the window focus is lost
	config->backlighton     = true;               //Keep backlight on
	config->netlogging      = true;               //Enable logging of network events in the console
	config->lockkeys        = true;               //Lock keys for use in this application
	config->keysounds       = false;              //Disable key 'click' sounds for smartphones
	return(E_OK);
}

//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
	ClassEStd::StrCpy(config->caption, "Hello World!");
	config->icon            = IDI_MAIN;           //Application icon
	config->menu            = 0;                  //Application contains no Windows Menu
	config->dialogbox       = IDD_MAIN;                  //Don't start from dialogbox (Desktop only)
	config->width           = 16;                //Desktop PC: Custom display width
	config->height          = 16;                //Desktop PC: Custom display height
	config->videotype       = EDSP_DEFAULT;       //Choose the default display mode
	config->emulvideotype   = EDSP_DEFAULT;       //No color depth emulation
	config->orientation     = DOR_AUTO;           //Change orientation to match the settings of the device
	config->openmethod      = DOM_DEFAULT;        //Take recommended (fastest) display open method
	config->surfacedefault  = EST_SYSMEMORY;      //Create surfaces in system memory by default, don't use DirectDraw or Symbian bitmap surfaces
	#if defined(DEVICE_DESKTOP)
		config->fullscreen  = false;              //Windowed application for Desktop
		config->fullscreenmouse = true;           //Desktop PC: Enable the mouse cursor for fullscreen applications
	#else
		config->fullscreen  = true;               //Fullscreen application for mobile devices
	#endif
	config->dspnative       = true;               //If possible, try to use native blitters (like DirectDraw)
	config->enginestatus    = false;              //Desktop PC: Enable the status bar and status information icon
	config->engineconsole   = false;              //Desktop PC: Enable the edge console
	config->surfprerotate   = true;               //Use pre-rotated surfaces to increase speed in other orientations
	return(E_OK);
}

//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
	FT_Library font_library;
	if (FT_Init_FreeType(&font_library))
		return(E_ERROR);
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
	return(E_OK);
}

//Callback: Called when the display has been changed (because of a change in orientation)
void ClassMain::OnDisplayChange(ClassEDisplay *display)
{
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

//Callback: Called when a network event occurs
void ClassMain::OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer)
{
}

//Native event
bool ClassMain::OnNativeEvent(ENATIVETYPE winhandle, ENATIVETYPE msg, ENATIVETYPE param1, ENATIVETYPE param2)
{
	char valstr[12];
	unsigned long ctr;
	ClassEFile edir;
	HWND hwnd = (HWND)winhandle;
	UINT wmsg = (UINT)msg;
	WPARAM wparam = (WPARAM)param1;
	LPARAM lparam = (LPARAM)param2;
	switch(wmsg)
	{
		case WM_COMMAND:
			switch(wparam)
			{
				case ID_FILE_LOADCHARSET:
					if (DialogBox((HINSTANCE)ecd.dat->instance, MAKEINTRESOURCE(IDD_CHARSET), hwnd, CharsetWinProc) == IDOK)
					{
						if (charsetlistsel == 0)
						{
							for (ctr = 0; ctr < 95; ctr++)
							{
								cfg.charlist[ctr] = (unsigned short)(ctr + 32);
								cfg.charlist[ctr + 1] = 0;
							}
						}
						else if (charsetlistsel == 1)
						{
							for (ctr = 0; ctr < 96 + 128; ctr++)
							{
								cfg.charlist[ctr] = (unsigned short)(ctr + 32);
								cfg.charlist[ctr + 1] = 0;
							}
						}
						else
							cfg.charlist[0] = 0;
						SetWindowTextW(GetDlgItem(hwnd, IDC_CHARLIST), (LPWSTR)cfg.charlist);
					}
					return(true);
				case ID_FILE_EXITAPP:
					PostMessage(hwnd, WM_CLOSE, 0, 0);
					return(true);
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
						ShellExecuteA(hwnd, "open", commandkey, regcommand, NULL, SW_SHOW);
					}
					return(true);
				case ID_HELP_ABOUT:
					MessageBoxA(hwnd, "EDGELIB font tool\nAutomatically creates a font image based on a character set and font file.\n\nCopyright (c) 2004-2017 Elements Interactive", "About makefont", MB_OK);
					return(true);
				case IDOK:
					char fontfn[512];
					char fontnamestr[128];
					CheckWinConfig(hwnd);
					SendMessage(GetDlgItem(hwnd, IDC_FONTLIST), CB_GETLBTEXT, SendMessage(GetDlgItem(hwnd, IDC_FONTLIST), CB_GETCURSEL, 0, 0), (LPARAM)fontnamestr);
					if (SelectSaveFile(hwnd, "Select destination file", "PNG files", ".png"))
					{
						ClassEStd::StrCpy(fontfn, "C:\\Windows\\Fonts\\");
						ClassEStd::StrCat(fontfn, fontnamestr);
						if (!ProcessFile(fontfn, savefn))
							MessageBoxA(hwnd, "Failed to generate font file", "Font error", MB_OK | MB_ICONERROR);
					}
					return(true);
				case IDCANCEL:
					PostMessage(hwnd, WM_CLOSE, 0, 0);
					return(true);
			}
			break;
		case WM_INITDIALOG:
			programpath[0] = 0;
			LoadCfg();
			if (edir.OpenFolder("C:\\Windows\\Fonts\\", "*.ttf"))
			{
				char fname[512];
				while(edir.ReadFolder(fname))
				{
					ClassEStd::StrCpy(fontentry[numfontentries].fontfile, fname);
					fontentry[numfontentries].style = FS_NONE;
					numfontentries++;
				}
				edir.CloseFolder();
			}
			if (numfontentries)//lbl
			{
				for (ctr = 0; ctr < numfontentries; ctr++)
				{
					unsigned long ctr2, flen = ClassEStd::StrLen(fontentry[ctr].fontfile);
					char checkfname[256];
					ClassEStd::StrCpy(checkfname, fontentry[ctr].fontfile);
					for (ctr2 = flen - 1; ctr2 > 0; ctr2--)
						if (checkfname[ctr2] == '.')
							checkfname[ctr2] = 0;
					for (ctr2 = 0; ctr2 < numfontentries; ctr2++)
						if (ctr2 != 0)
						{
							unsigned long checkfnlen = ClassEStd::StrLen(checkfname);
							if (ClassEStd::StrFind(fontentry[ctr].fontfile, checkfname, false) == 0)
							{
								if (fontentry[ctr2].fontfile[checkfnlen] == 'B' || fontentry[ctr2].fontfile[checkfnlen] == 'b')
									fontentry[ctr2].style |= fontentry[ctr].style | FS_BOLD;
								else if (fontentry[ctr2].fontfile[checkfnlen] == 'I' || fontentry[ctr2].fontfile[checkfnlen] == 'i')
									fontentry[ctr2].style |= fontentry[ctr].style | FS_ITALIC;
							}
						}
				}
				//check styles in filename
				for (ctr = 0; ctr < numfontentries; ctr++)
					if (fontentry[ctr].style == FS_NONE)
						SendMessage(GetDlgItem(hwnd, IDC_FONTLIST), CB_ADDSTRING, 0, (LPARAM)fontentry[ctr].fontfile);
				SendMessage(GetDlgItem(hwnd, IDC_FONTLIST), CB_SETCURSEL, 0, 0);
			}
			
			//HFONT f;
			//HDC dc;
			//f = CreateFont(0, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, 0, 0, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial Unicode MS");
			//dc = GetDC(GetDlgItem(hwnd, IDC_CHARLIST));
			//((HFONT) SelectObject((dc), (HGDIOBJ)(HFONT)(f)));

			ClassEStd::IntToStr(valstr, cfg.size);
			SetWindowTextA(GetDlgItem(hwnd, IDC_FONTSIZE), valstr);
			IntToHex(valstr, cfg.color);
			SetWindowTextA(GetDlgItem(hwnd, IDC_FONTCOLOR), valstr);
			ClassEStd::IntToStr(valstr, cfg.pixwidth);
			SetWindowTextA(GetDlgItem(hwnd, IDC_PIXELSROW), valstr);
			ClassEStd::IntToStr(valstr, cfg.paddfont.left);
			SetWindowTextA(GetDlgItem(hwnd, IDC_PADDLEFT), valstr);
			ClassEStd::IntToStr(valstr, cfg.paddfont.top);
			SetWindowTextA(GetDlgItem(hwnd, IDC_PADDTOP), valstr);
			ClassEStd::IntToStr(valstr, cfg.paddfont.right);
			SetWindowTextA(GetDlgItem(hwnd, IDC_PADDRIGHT), valstr);
			ClassEStd::IntToStr(valstr, cfg.paddfont.bottom);
			SetWindowTextA(GetDlgItem(hwnd, IDC_PADDBOTTOM), valstr);
			SetWindowTextW(GetDlgItem(hwnd, IDC_CHARLIST), (LPWSTR)cfg.charlist);
			if (cfg.flags & PFL_VARFONT)
				CheckDlgButton(hwnd, IDC_VARFONT, BST_CHECKED);
			if (cfg.flags & PFL_SAVE24BIT)
				CheckDlgButton(hwnd, IDC_SAVE24BIT, BST_CHECKED);
			if (cfg.flags & PFL_ANTIALIAS)
				CheckDlgButton(hwnd, IDC_ANTIALIAS, BST_CHECKED);
			//if (cfg.flags & PFL_INTEGRATE)
			//	CheckDlgButton(hwnd, IDC_INTEGRATE, BST_CHECKED);
			return(true);
		case WM_CLOSE:
			CheckWinConfig(hwnd);
			SaveCfg();
			EndDialog(hwnd, IDABORT);
			return(TRUE);
	}
	return(false);
}
