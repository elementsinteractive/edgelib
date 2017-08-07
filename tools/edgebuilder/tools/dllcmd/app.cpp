// app.cpp
// Vc2005Cmd: Start a Visual Studio 2005 command by:
// - Setting path to external DLL
// - Execute command
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define MAX_TMPARG    16

//Uittrekken commandline parameters
void check_args(char *cmdline, int *argc, char *argv[MAX_TMPARG])
{
	unsigned char status = 0;
	unsigned long ctr, pos = 0, len = strlen(cmdline);
	for (ctr = 0; ctr < len; ctr++)
	{
		if (status >= 2)
		{
			if (cmdline[ctr] == ' ' && status == 2)
				status = 3;
			else if (cmdline[ctr] == '-' && status == 3)
				status = 4;
			else if (cmdline[ctr] == '~' && status == 4)
				status = 5;
			else if (cmdline[ctr] == '-' && status == 5)
				status = 6;
			else if (cmdline[ctr] == ' ' && status == 6)
				status = 7;
			if (ctr == len - 1 || status >= 7)
			{
				if (status >= 7)
					cmdline[ctr - 4] = 0;
				argv[*argc] = &cmdline[pos];
				*argc = *argc + 1;
				pos = ctr + 1;
				status = 2;
			}
		}
		else
		{
			if (cmdline[ctr] == '"' && status == 0)
				status = 1;
			else if (cmdline[ctr] == '"' && status == 1 || cmdline[ctr] == ' ' && status == 0)
			{
				cmdline[ctr] = 0;
				if (status == 1)
					argv[*argc] = &cmdline[pos + 1];
				else
					argv[*argc] = &cmdline[pos];
				*argc = *argc + 1;
				pos = ctr + 1;
				if (status == 1)
					pos++;
				status = 2;
			}
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
	startupInfo.cb = sizeof (STARTUPINFO);
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
	int sim_argc = 0;
	char *sim_argv[MAX_TMPARG];
	char envstr[8192], cmdline[8192];
	strcpy(cmdline, GetCommandLine());
	check_args(cmdline, &sim_argc, sim_argv);

	if (sim_argc > 2)
	{
		char envpath[8192];
		if (GetEnvironmentVariable("path", envpath, 8192) != 0)
		{
			bool beginpath = false;
			if (sim_argv[1][0] == '/' && sim_argv[1][1] == 's' && sim_argv[1][2] == '_')
			{
				beginpath = true;
				sim_argv[1] = &sim_argv[1][3];
			}
			int retval;
			if (beginpath)
			{
				strcpy(envstr, sim_argv[1]);
				strcat(envstr, ";");
				strcat(envstr, envpath);
			}
			else
			{
				strcpy(envstr, envpath);
				strcat(envstr, ";");
				strcat(envstr, sim_argv[1]);
			}
			SetEnvironmentVariable("path", envstr);
			retval = runcommand(sim_argv[2]);
			SetEnvironmentVariable("path", envpath);
			return(retval);
		}
	}

	return(-1);
}
