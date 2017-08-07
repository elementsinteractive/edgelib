// app.cpp
// CreateCopy: Copy a file and create a folder if it does not exist
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <direct.h>
#include <sys\types.h>
#include <sys\stat.h>

#define MAX_TMPARG    16

//Uittrekken commandline parameters
void check_args(char *cmdline, int *argc, char *argv[MAX_TMPARG])
{
	bool onquote = false, onspace = true;
	unsigned long ctr, pos = 0, len = strlen(cmdline);
	for (ctr = 0; ctr < len; ctr++)
	{
		if (cmdline[ctr] != ' ' && onspace)
		{
			argv[*argc] = &cmdline[ctr];
			if (argv[*argc][0] == '"')
			{
				argv[*argc] = &argv[*argc][1];
				onquote = true;
			}
			*argc = *argc + 1;
			onspace = false;
		}
		else if ((cmdline[ctr] == '"' && onquote && !onspace) || (cmdline[ctr] == ' ' && !onspace && !onquote) || ctr == len - 1)
		{
			if (ctr < len - 1)
			{
				cmdline[ctr] = 0;
				if (ctr > 0)
					if (cmdline[ctr - 1] == '"')
						cmdline[ctr - 1] = 0;
			}
			else if (cmdline[ctr] == '"')
				cmdline[ctr] = 0;
			onspace = true;
			onquote = false;
		}
	}
}

//Execute a command in the console
int runcommand(char *cmd)
{
	STARTUPINFO startupInfo;
	PROCESS_INFORMATION process_info;
	BOOL bRet;
	DWORD dwExitCode;
	memset(&startupInfo, 0, sizeof(STARTUPINFO));
	startupInfo.cb = sizeof(STARTUPINFO);
	bRet = CreateProcess(NULL, cmd, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &process_info);
	if (bRet)
	{
		WaitForSingleObject(process_info.hProcess, INFINITE);
		GetExitCodeProcess(process_info.hProcess, &dwExitCode);
		return(dwExitCode);
	}
	else
	{
		printf("vc2005cmd: Can't execute command");
		printf("\r\n");
		return(-1);
	}
}

//Entry point
int main(int argc, char *argv[])
{
	int ctr, sim_argc = 0;
	char *sim_argv[MAX_TMPARG];
	char cmdline[8192];
	strcpy(cmdline, GetCommandLine());
	check_args(cmdline, &sim_argc, sim_argv);
	if (sim_argc >= 3)
	{
		char dstdir[MAX_PATH], dstfile[MAX_PATH];
		strcpy(dstdir, sim_argv[2]);
		strcpy(dstfile, "");
		for (ctr = strlen(dstdir) - 1; ctr > 0; ctr--)
			if (dstdir[ctr] == '\\')
			{
				strcpy(dstfile, &dstdir[ctr + 1]);
				dstdir[ctr] = 0;
				break;
			}
		if (dstdir[1] != ':' && dstdir[0] != '\\')
		{
			char tmpdir[MAX_PATH];
			_getcwd(tmpdir, MAX_PATH);
			strcat(tmpdir, "\\");
			strcat(tmpdir, dstdir);
			strcpy(dstdir, tmpdir);
		}
		if (strlen(dstfile) == 0)
		{
			for (ctr = strlen(argv[1]) - 1; ctr > 0; ctr--)
				if (argv[1][ctr] == '\\')
				{
					strcpy(dstfile, &argv[1][ctr + 1]);
					break;
				}
			if (strlen(dstfile) == 0)
				strcpy(dstfile, argv[1]);
		}
		struct stat sbuf;
		if (stat(dstdir, &sbuf) != 0)
		{
			char systemcmd[512];
			strcpy(systemcmd, "mkdir \"");
			strcat(systemcmd, dstdir);
			strcat(systemcmd, "\"");
			system(systemcmd);
			if (stat(dstdir, &sbuf) != 0)
			{
				printf("Error: couldn't create folder");
				return(-3);
			}
		}
		else if (!(sbuf.st_mode & _S_IFDIR) && sbuf.st_mode != 0)
		{
			printf("Error: destination folder is a file (%d)", sbuf.st_mode);
			return(-2);
		}
		char systemcpcmd[1024];
		strcpy(systemcpcmd, "copy /Y \"");
		strcat(systemcpcmd, argv[1]);
		strcat(systemcpcmd, "\" \"");
		strcat(systemcpcmd, dstdir);
		strcat(systemcpcmd, "\\");
		strcat(systemcpcmd, dstfile);
		strcat(systemcpcmd, "\" > nul");
		system(systemcpcmd);
		strcat(dstdir, "\\");
		strcat(dstdir, dstfile);
		if (stat(dstdir, &sbuf) != 0)
		{
			printf("Error: couldn't copy file");
			return(-4);
		}
		return(0);
	}
	return(-1);
}
