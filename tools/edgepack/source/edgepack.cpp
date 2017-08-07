// edgepack.cpp
// Packs multiple files into one file with compression
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Product/version                                                 //
/////////////////////////////////////////////////////////////////////

#define PRODUCTNAME TEXT("EDGELIB Packer")
//The name of the product

/////////////////////////////////////////////////////////////////////
// Include and link                                                //
/////////////////////////////////////////////////////////////////////

#define _WIN32_WINNT 0x0501

#define PROGRAM_VERINFO           "Version 2.25"

#pragma comment(lib, "edge.lib")
#include "edgemain.h"             //The Edge

#pragma comment(lib, "comctl32.lib")
#include <commdlg.h>
#include <shellapi.h>
#include <shlobj.h>
#include <direct.h>
#include <fcntl.h>
#include <commctrl.h>             //Link windows components

#include "resource.h"             //The main program and resources

#include "interface.h"            //The GUI interface

/////////////////////////////////////////////////////////////////////
// Main functions                                                  //
/////////////////////////////////////////////////////////////////////

//Load vendor signatures and unlock codes from 'vendor.inf'
void LoadVendorSettings(WCHAR *currentpath, bool createnew = true)
{
	ClassEFile ef;
	char workingdir[MAX_EFILESTR], fname[MAX_EFILESTR];
	long ctr;
	bool freefound = false;
	_getcwd(workingdir, MAX_EFILESTR);
	strcpy(fname, workingdir);
	strcat(fname, "\\");
	if (!ClassEStd::StrEqual(fname, currentpath))
	{
		strcat(fname, "vendor.ini");
		strcpy(vlocalfile, fname);
		if (!ReadVendors(fname))
			strcpy(vlocalfile, "");
		localfound = vendorfound;
	}
	else
	{
		strcpy(vlocalfile, "");
		localfound = 0;
	}
	ClassEStd::StrCpy(fname, currentpath);
	strcat(fname, "vendor.ini");
	strcpy(vglobalfile, fname);
	if (!ReadVendors(fname))
	{
		if (createnew)
		{
			char wstr[MAX_ESTR];
			if (ef.New(fname))
			{
				strcpy(wstr, "# Vendor settings used by the Edge Packer\r\n");
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
				strcpy(wstr, "#   vendor.ini from the current folder, then it reads vendor.ini from the Edge Packer folder.\r\n\r\n");
				ef.Write(wstr, strlen(wstr));
				strcpy(wstr, "FREE = \"0x00000000\"\r\n");
				ef.Write(wstr, strlen(wstr));
				ef.Close();
			}
		}
	}
	for (ctr = 0; ctr < (long)vendorfound; ctr++)
		if (ClassEStd::StrEqual(vendorstr[ctr], "FREE", false))
		{
			freefound = true;
			break;
		}
	if (!freefound && vendorfound < 16)
	{
		vendorcode[vendorfound] = 0;
		strcpy(vendorstr[vendorfound], "FREE");
		vendorfound++;
	}
}

//Load program configuration
void LoadConfig(void)
{
	ClassEIni ini;
	iconfig.compresstype = ECMP_ZLIB;
	if (ini.Open("edgepack.ini"))
	{
		char key[MAX_ESTR], value[MAX_ESTR];
		while(ini.ReadLine(key, value))
		{
			if (ClassEStd::StrEqual(key, "Compression", false))
			{
				if (ClassEStd::StrEqual(value, "None", false))
					iconfig.compresstype = ECMP_NONE;
				else if (ClassEStd::StrEqual(value, "Edge", false))
					iconfig.compresstype = ECMP_EDGE;
			}
		}
		ini.Close();
	}
}

//Save program configuration
void SaveConfig(void)
{
	ClassEIni ini;
	if (ini.New("edgepack.ini"))
	{
		ini.WriteComment("Edge packer configuration file");
		ini.WriteNewline();
		if (iconfig.compresstype == ECMP_NONE)
			ini.WriteLine("Compression", "None");
		else if (iconfig.compresstype == ECMP_EDGE)
			ini.WriteLine("Compression", "Edge");
		else
			ini.WriteLine("Compression", "ZLib");
		ini.Close();
	}
}

//Divide command line parameters
void CheckArgs(char *cmdline)
{
	bool oncomment = false, newstart = true, argcomment;
	unsigned long ctr, clen = strlen(cmdline);
	for (ctr = 0; ctr < clen; ctr++)
	{
		if (newstart && !ClassEStd::IsWhiteSpace(cmdline[ctr]))
		{
			argstart[argcount] = ctr;
			newstart = false;
			argcomment = false;
		}
		if (!newstart)
		{
			if ((ClassEStd::IsWhiteSpace(cmdline[ctr]) && !oncomment) || ctr == clen - 1)
			{
				if (ClassEStd::IsWhiteSpace(cmdline[ctr]))
					argend[argcount] = ctr - 1;
				else
					argend[argcount] = ctr;
				if (argcomment && cmdline[argend[argcount]] == '\"')
					argend[argcount]--;
				argcount++;
				newstart = true;
				if (argcount >= MAX_ARGCOUNT)
					break;
			}
			if (cmdline[ctr] == '\"')
			{
				if (!oncomment)
					if (ctr == argstart[argcount])
						argstart[argcount]++;
				oncomment = !oncomment;
				argcomment = true;
			}
		}
	}
	for (ctr = 0; ctr < argcount; ctr++)
		cmdline[argend[ctr] + 1] = 0;
	cmdargs = cmdline;
}

//Redirect std handles
void RedirectStdHandles(void)
{
	hstdoutput = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteFile(hstdoutput, "test", 5, NULL, NULL);
	CloseHandle(hstdoutput);
}

/////////////////////////////////////////////////////////////////////
// The program entry point                                         //
/////////////////////////////////////////////////////////////////////

ClassEdge *EdgeMain(EDGESTARTUP *data)
{
	ClassEFile path_ef;
	INITCOMMONCONTROLSEX cinfo;
	char tmp_str[MAX_ESTR];
	bool windowed = true;
	ghinst = (HINSTANCE)data->instance;
	programstate = PS_IDLE;
	CheckArgs(data->commandline);
	if (argcount == 1)
	{
		strcpy(tmp_str, ";");
		strcat(tmp_str, &cmdargs[argstart[0]]);
		strcat(tmp_str, ";");
		if (ClassEStd::StrFind((char *)supportcmd, tmp_str, false) >= 0)
			windowed = false;
	}
	else if (argcount > 1)
		windowed = false;
	
	LoadVendorSettings(path_ef.GetCurrentFolder(), windowed);
	LoadConfig();
	memset(programpath, 0, MAX_FILESTR);
	memset(loadedfile, 0, MAX_FILESTR);
	lastpackerror = 0;
	if (!windowed)
	{
		bool quietmode = false;
		unsigned long ctr;
		for (ctr = 0; ctr < argcount; ctr++)
		{
			strcpy(tmp_str, ";");
			strcat(tmp_str, &cmdargs[argstart[ctr]]);
			strcat(tmp_str, ";");
			if (ClassEStd::StrFind((char *)supportcmd, tmp_str, false) >= 0)
				if (ClassEStd::StrEqual(tmp_str, ";-q;") || ClassEStd::StrEqual(tmp_str, ";--quiet;"))
					quietmode = true;
		}
		commandqueue = 0;
		if (quietmode)
		{
			while(!OnCommandQueue(NULL, commandqueue))
				commandqueue++;
		}
		else
			DialogBox((HINSTANCE)data->instance, MAKEINTRESOURCE(IDD_CONSOLE), NULL, ConsoleWinProc);
	}
	else
	{
		if (argcount == 1)
			strcpy(loadedfile, &cmdargs[argstart[0]]);
		cinfo.dwSize = sizeof(INITCOMMONCONTROLSEX);
		cinfo.dwICC  = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES;
		InitCommonControlsEx(&cinfo);
		DialogBoxParam((HINSTANCE)data->instance, MAKEINTRESOURCE(IDD_MAIN), NULL, WinProc, (LPARAM)loadedfile);
	}
	SaveConfig();
	data->returnvalue = lastpackerror;
	return(NULL);
}