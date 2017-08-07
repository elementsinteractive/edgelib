// app.cpp
// IconConv: Converts files to .ico, .mbm or .png files for icons
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Link Edge
#pragma comment(lib, "edge.lib")
#include "edgemain.h"

//Windows headers
#include <direct.h>
#include <stdio.h>

//Edge loaders
#include "eloader.h"

#define MAX_GFXLOADER   5

//Globale colorkey instelling
unsigned long icocolorkey = ECOLOR_FUCHSIA;
bool icoinvertalpha = false;
char supportsizes[MAX_ESTR];

//Check supported size
unsigned char sizeissupported(long width, long height)
{
	if (width > 128 || height > 128)
		return(0);
	if (ClassEStd::StrLen(supportsizes))
	{
		char checksize[32];
		ClassEStd::IntToStr(checksize, width);
		ClassEStd::StrCat(checksize, "x");
		ClassEStd::IntToStr(&checksize[ClassEStd::StrLen(checksize)], height);
		ClassEStd::StrCat(checksize, ",");
		if (ClassEStd::StrFind(supportsizes, checksize) >= 0)
			return(1);
		return(2);
	}
	return(1);
}

#define MAX_PALETTE 256

//Bestands schrijvers
#include "writeico.h"
#include "writembm.h"
#include "writepng.h"

//Limieten
#define MAX_INPUTIMAGE 32

//Vlaggen
#define IT_FORMATNONE      0
#define IT_FORMATICO       1
#define IT_FORMATMBM       2
#define IT_FORMATPNG       4
#define IT_NOCREATE    0x100

//Check de commandline en lees parameters uit
int checkcommandline(char *cmdline, unsigned long &imgtype, char *outputfile, char *inputfiles)
{
	char cmdarg[MAX_ESTR];
	bool onquote = false, onparam = false;
	unsigned char onp = 0, onfile = 0;
	unsigned long ctr, startp = 0, len = strlen(cmdline);
	for (ctr = 0; ctr < len; ctr++)
	{
		if (!onparam && cmdline[ctr] != ' ')
		{
			startp = ctr;
			onparam = true;
		}
		if (onparam)
		{
			if (cmdline[ctr] == '\"')
				onquote = !onquote;
			if ((!onquote && cmdline[ctr] == ' ') || ctr == len - 1)
			{
				if (onp > 0)
				{
					bool quotearg = false;
					memcpy(cmdarg, &cmdline[startp], ctr - startp + 1);
					cmdarg[ctr - startp + 1] = 0;
					ClassEStd::StrTrim(cmdarg);
					if (cmdarg[0] == '"' && cmdarg[strlen(cmdarg) - 1] == '"')
					{
						quotearg = true;
						cmdarg[0] = ' ';
						cmdarg[strlen(cmdarg) - 1] = ' ';
					}
					ClassEStd::StrTrim(cmdarg);
					if (!quotearg && (cmdarg[0] == '-' || cmdarg[0] == '/'))
					{
						if (ClassEStd::StrEqual(&cmdarg[1], "a", false) || ClassEStd::StrEqual(&cmdarg[1], "append", false))
						{
							imgtype |= IT_NOCREATE;
						}
						else if (ClassEStd::StrEqual(&cmdarg[1], "i", false) || ClassEStd::StrEqual(&cmdarg[1], "invert", false))
						{
							icoinvertalpha = true;
						}
						else if (ClassEStd::StrEqual(&cmdarg[1], "?", false) || ClassEStd::StrEqual(&cmdarg[1], "help", false))
						{
							onp = 1;
							break;
						}
						else if ((cmdarg[1] == 'f' || cmdarg[1] == 'F') && cmdarg[2] == ':')
						{
							char formatstr[MAX_ESTR];
							strcpy(formatstr, &cmdarg[3]);
							if (ClassEStd::StrEqual(formatstr, "ico", false))
							{
								imgtype = (imgtype & 0xFFFFFF00) + IT_FORMATICO;
							}
							else if (ClassEStd::StrEqual(formatstr, "mbm", false))
							{
								imgtype = (imgtype & 0xFFFFFF00) + IT_FORMATMBM;
							}
							else if (ClassEStd::StrEqual(formatstr, "png", false))
							{
								imgtype = (imgtype & 0xFFFFFF00) + IT_FORMATPNG;
							}
							else
							{
								printf("Unknown format: ");
								printf(formatstr);
								printf("\n");
								return(-4);
							}
						}
						else if ((cmdarg[1] == 'c' || cmdarg[1] == 'C') && cmdarg[2] == ':')
						{
							char ckeystr[MAX_ESTR];
							unsigned long newckey = 0;
							unsigned char ckctr, cklen;
							strcpy(ckeystr, &cmdarg[3]);
							cklen = strlen(ckeystr);
							for (ckctr = 0; ckctr < cklen; ckctr++)
							{
								newckey *= 16;
								if (ckeystr[ckctr] >= '0' && ckeystr[ckctr] <= '9')
									newckey += ckeystr[ckctr] - '0';
								else if (ckeystr[ckctr] >= 'A' && ckeystr[ckctr] <= 'F')
									newckey += ckeystr[ckctr] - 'A' + 10;
								else if (ckeystr[ckctr] >= 'a' && ckeystr[ckctr] <= 'f')
									newckey += ckeystr[ckctr] - 'a' + 10;
								else
								{
									printf("Illegal color value: ");
									printf(ckeystr);
									printf("\n");
									return(-10);
								}
							}
							icocolorkey = (newckey & 0xFF00) | ((newckey >> 16) & 0xFF) | ((newckey << 16) & 0xFF0000);
						}
						else if ((cmdarg[1] == 's' || cmdarg[1] == 'S') && cmdarg[2] == ':')
						{
							ClassEStd::StrCpy(supportsizes, &cmdarg[3]);
							ClassEStd::StrCat(supportsizes, ",");
						}
						else
						{
							printf("Unknown option: ");
							printf(cmdarg);
							printf("\n");
							return(-3);
						}
					}
					else
					{
						if (onfile == 0)
						{
							strcpy(outputfile, cmdarg);
						}
						else if (onfile <= MAX_INPUTIMAGE)
							strcpy(&inputfiles[(onfile - 1) * MAX_EFILESTR], cmdarg);
						onfile++;
					}
				}
				onparam = false;
				onp++;
			}
		}
	}
	if (onp == 0)
	{
		printf("iconconv: Command error\n");
		return(-1);
	}
	else if (onp == 1)
	{
		printf("usage: iconconv [options] [iconfile] [inputfiles]\n");
		printf("\nSupported options:\n");
		printf("-a, -append: append icon file, don't create a new file\n");
		printf("-i, -invert: invert alpha mask\n");
		printf("-s:[sizes] : a comma-seperated list of icon sizes, others are ignored\n");
		printf("-f:[format]: set icon format, based on extension when not specified\n");
		printf("-c:[color] : set colorkey for bitmap icons, default = -c:FF00FF\n");
		printf("\nSupported formats:\n");
		printf("-f:ico: Windows icon file\n");
		printf("-f:mbm: Symbian multi-bitmap file\n");
		return(-2);
	}
	else
	{
		if (strlen(outputfile) == 0)
		{
			printf("Error: no output file specified\n");
			return(-5);
		}
		else if (strlen(inputfiles) == 0)
		{
			printf("Error: no input file specified\n");
			return(-6);
		}
		if ((imgtype & 0xFF) == IT_FORMATNONE)
		{
			long dotpos = 0, newpos;
			do
			{
				newpos = ClassEStd::StrFind(&outputfile[dotpos], '.');
				if (newpos >= 0)
					dotpos += newpos + 1;
				else if (dotpos == 0)
					dotpos = -1;
			}
			while(newpos != -1);
			if (dotpos >= 0)
			{
				char extensionstr[MAX_ESTR];
				strcpy(extensionstr, &outputfile[dotpos]);
				if (ClassEStd::StrEqual(extensionstr, "ico", false))
				{
					imgtype = (imgtype & 0xFFFFFF00) + IT_FORMATICO;
				}
				else if (ClassEStd::StrEqual(extensionstr, "mbm", false))
				{
					imgtype = (imgtype & 0xFFFFFF00) + IT_FORMATMBM;
				}
				else if (ClassEStd::StrEqual(extensionstr, "png", false))
				{
					imgtype = (imgtype & 0xFFFFFF00) + IT_FORMATPNG;
				}
				else
				{
					printf("Unknown format: ");
					printf(extensionstr);
					printf("\n");
					return(-4);
				}
			}
			else
			{
				printf("Error: no output file extension\n");
				return(-7);
			}
		}
	}
	return(0);
}

//Entry point
ClassEdge *EdgeMain(EDGESTARTUP *data){ return(NULL); } //Edge dummy
int main(int argc, char argv[])
{
	int result;
	unsigned long imgtype;
	char cmdlinestr[8192];
	char inputfiles[MAX_INPUTIMAGE * MAX_EFILESTR];
	char outputfile[MAX_EFILESTR];
	strcpy(supportsizes, "");
	strcpy(cmdlinestr, GetCommandLine());
	result = checkcommandline(cmdlinestr, imgtype, outputfile, inputfiles);
	if (result == 0)
	{
		bool result;
		unsigned char inpctr = 0;
		if (!(imgtype & IT_NOCREATE))
		{
			if ((imgtype & 0xFF) == IT_FORMATICO)
				result = iconewfile(outputfile);
			else if ((imgtype & 0xFF) == IT_FORMATMBM)
				result = mbmnewfile(outputfile);
			else if ((imgtype & 0xFF) == IT_FORMATPNG)
				result = pngnewfile(outputfile);
			else
				result = false;
			if (!result)
			{
				printf("Error: couldn't create ");
				printf(outputfile);
				printf("\n");
				return(-8);
			}
		}
		while(inpctr < MAX_INPUTIMAGE && inputfiles[inpctr * MAX_EFILESTR])
		{
			if ((imgtype & 0xFF) == IT_FORMATICO)
				result = icoaddfile(outputfile, &inputfiles[inpctr * MAX_EFILESTR]);
			else if ((imgtype & 0xFF) == IT_FORMATMBM)
				result = mbmaddfile(outputfile, &inputfiles[inpctr * MAX_EFILESTR]);
			else if ((imgtype & 0xFF) == IT_FORMATPNG)
				result = pngaddfile(outputfile, &inputfiles[inpctr * MAX_EFILESTR]);
			else
				result = false;
			if (!result)
			{
				printf("Error: couldn't add ");
				printf(&inputfiles[inpctr * MAX_EFILESTR]);
				printf("\n");
				return(-9);
			}
			inpctr++;
		}
	}
	return(result);
}
