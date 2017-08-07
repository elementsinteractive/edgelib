// app.cpp
// EasyCabWiz: Improvement for CabWiz and CabWizSp
// Ensures for handy files list and INI structure. Prevents redundancy
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Link Edge
#pragma comment(lib, "edge.lib")
#include "edgemain.h"

//Windows headers
#include <direct.h>
#include <stdio.h>

//Processing class
#include "process.h"

//Check de commandline en lees parameters uit
int checkcommandline(char *cmdline, char *cabwizexe, char *inputfile, char *outputfile)
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
					if (onp == 1)
						ClassEStd::StrCpy(cabwizexe, cmdarg);
					else if (onp == 2)
						ClassEStd::StrCpy(inputfile, cmdarg);
					else if (onp == 3)
						ClassEStd::StrCpy(outputfile, cmdarg);
				}
				onparam = false;
				onp++;
			}
		}
	}
	if (onp == 0)
	{
		printf("easycabwiz: Command error\n");
		return(-1);
	}
	else if (onp == 1)
	{
		printf("usage: easycabwiz [cabwizexe] [inputfile] [outputfile]\n");
		return(-2);
	}
	else
	{
		if (onp < 3)
		{
			printf("Error: no input file specified\n");
			return(-3);
		}
	}
	return(0);
}

//Entry point
ClassEdge *EdgeMain(EDGESTARTUP *data){ return(NULL); } //Edge dummy
int main(int argc, char argv[])
{
	int result;
	char cmdlinestr[8192];
	char cabwizexe[MAX_EFILESTR], inputfile[MAX_EFILESTR], outputfile[MAX_EFILESTR];
	cabwizexe[0] = 0;
	inputfile[0] = 0;
	outputfile[0] = 0;
	strcpy(cmdlinestr, GetCommandLine());
	result = checkcommandline(cmdlinestr, cabwizexe, inputfile, outputfile);
	if (result == 0)
	{
		CabProcess prc;
		if (!prc.Start(cabwizexe, inputfile, outputfile))
			result = -4;
	}
	return(result);
}
