// app.cpp
// Genresource: Generates resource files using resource templates
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Link Edge
#pragma comment(lib, "edge.lib")
#include "edgemain.h"

//Andere headers
#include <direct.h>
#include <stdio.h>

//Toevoegen handige environment class
#include "..\\..\\EdgeBuilder\\env.h"

#define MAX_RSVTEMPLATE   50000

const char templateheader[] = "$% File Start %$";

//Check de commandline
int checkcmdline(char *cmdline, char *srcfile, char *dstfile, ClassEnv *env)
{
	char pkey[MAX_ESTR];
	bool onquote = false, onparam = false;
	unsigned long ctr, startp = 0, len = strlen(cmdline);
	unsigned char onp = 0;
	for (ctr = 0; ctr < len; ctr++)
	{
		if (onp < 3)
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
						char *checkptr = srcfile;
						if (onp == 2)
							checkptr = dstfile;
						memcpy(checkptr, &cmdline[startp], ctr - startp + 1);
						checkptr[ctr - startp + 1] = 0;
						ClassEStd::StrTrim(checkptr);
						if (checkptr[0] == '"' && checkptr[strlen(checkptr) - 1] == '"')
						{
							checkptr[0] = ' ';
							checkptr[strlen(checkptr) - 1] = ' ';
						}
						ClassEStd::StrTrim(checkptr);
					}
					onparam = false;
					onp++;
				}
			}
		}
		else
		{
			if (onparam)
			{
				if (!onquote && cmdline[ctr] == '=')
				{
					startp = ctr + 1;
					onquote = true;
				}
				else if (onquote)
				{
					bool endv = false;
					if (ctr == len - 1)
						endv = true;
					else if (cmdline[ctr] == ' ' && cmdline[ctr + 1] == '$')
						endv = true;
					if (endv)
					{
						char pvalue[65536];
						memcpy(pvalue, &cmdline[startp], ctr - startp + 1);
						pvalue[ctr - startp + 1] = 0;
						ClassEStd::StrTrim(pvalue);
						env->AddString(pkey, pvalue);
						onparam = false;
						onquote = false;
					}
				}
			}
			else
			{
				if (cmdline[ctr] == '$')
				{
					if (onquote)
					{
						memcpy(pkey, &cmdline[startp], ctr - startp);
						pkey[ctr - startp] = 0;
						onparam = true;
						onquote = false;
					}
					else
					{
						startp = ctr + 1;
						onquote = true;
					}
				}
			}
		}
	}
	if (onp == 0)
	{
		printf("genresource: Command error");
		return(-1);
	}
	else if (onp == 1)
	{
		printf("usage: genresource [template] [dstfile] (parameters)");
		return(-2);
	}
	if (onp == 2)
	{
		printf("genresource: No destination file given");
		return(-3);
	}
	return(0);
}

//Inlezen definities template -> env
int envreadtemplate(char *srcfile, char *&fdata, ClassEnv *env)
{
	ClassEFile ef;
	char curfolder[MAX_EFILESTR];
	int tstart = -1, fsize = 0;
	ef.SetCurrentFolder(_getcwd(curfolder, MAX_EFILESTR));
	if (ef.Open(srcfile))
	{
		fsize = ef.Size();
		fdata = (char *)ClassEMemory::Alloc(fsize + 1);
		if (fdata == NULL)
		{
			printf("genresource: Not enough memory");
			ef.Close();
			return(-5);
		}
		if (!ef.Read(fdata, fsize))
		{
			printf("genresource: Can't read template file \"");
			printf(srcfile);
			printf("\"");
			ClassEMemory::DeAlloc(fdata);
			ef.Close();
			return(-6);
		}
		else
			fdata[fsize] = 0;
		ef.Close();
	}
	else
	{
		printf("genresource: Can't open template file \"");
		printf(srcfile);
		printf("\"");
		return(-4);
	}
	tstart = ClassEStd::StrFind(fdata, (char *)templateheader, false);
	if (tstart >= 0)
	{
		char tkey[MAX_ESTR], tvalue[MAX_ESTR * 8];
		ClassEIni ini;
		if (tstart >= 0)
			if (ini.OpenData(fdata, tstart))
			{
				while (ini.ReadLine(tkey, tvalue))
					if (strlen(tkey) > 0 && strlen(tvalue) > 0)
						env->AddString(tkey, tvalue);
				ini.Close();
			}
		tstart += strlen(templateheader);
		if (tstart < fsize)
			if (fdata[tstart] == '\r' || fdata[tstart] == '\n')
				tstart++;
		if (tstart < fsize)
			if (fdata[tstart] == '\r' || fdata[tstart] == '\n')
				tstart++;
	}
	else
		tstart = 0;
	return(tstart);
}

//Converteren en wegschrijven template
int writetemplate(char *dstfile, char *fdata, ClassEnv *env)
{
	char curfolder[MAX_EFILESTR];
	char *wdata = NULL;
	wdata = (char *)ClassEMemory::Alloc(MAX_RSVTEMPLATE);
	if (wdata != NULL)
	{
		ClassEFile ef;
		int result = 0;
		ClassEStd::StrCpy(wdata, fdata, MAX_RSVTEMPLATE);
		env->ScanString(wdata);
		ef.SetCurrentFolder(_getcwd(curfolder, MAX_EFILESTR));
		if (ef.New(dstfile))
		{
			if (!ef.Write(wdata, ClassEStd::StrLen(wdata, MAX_RSVTEMPLATE)))
			{
				printf("genresource: Can't write template file \"");
				printf(dstfile);
				printf("\"");
				return(-8);
			}
		}
		else
		{
			printf("genresource: Can't create template file \"");
			printf(dstfile);
			printf("\"");
			return(-7);
		}
		ClassEMemory::DeAlloc(wdata);
		return(result);
	}
	printf("genresource: Not enough memory");
	return(-5);
}

//Entry point
ClassEdge *EdgeMain(EDGESTARTUP *data){ return(NULL); } //Edge dummy
int main(int argc, char argv[])
{
	char *fdata= NULL;
	char cmdlinestr[65536];
	char srcfile[MAX_EFILESTR], dstfile[MAX_EFILESTR];
	int result;
	ClassEnv env;
	env.AddString("enter", "\r\n");
	strcpy(srcfile, "");
	strcpy(dstfile, "");
	strcpy(cmdlinestr, GetCommandLine());
	result = checkcmdline(cmdlinestr, srcfile, dstfile, &env);
	if (result != 0)
		return(result);
	result = envreadtemplate(srcfile, fdata, &env);
	if (result >= 0)
	{
		result = writetemplate(dstfile, &fdata[result], &env);
		ClassEMemory::DeAlloc(fdata);
		return(result);
	}
	return(result);
}
