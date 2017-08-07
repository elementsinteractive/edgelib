// compiler.h
// Handles compiling for each platform
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

#define MAX_RSCTMP          8
#define MAX_INSTLTMP        8
#define MAX_OBJLIST      1024
#define MAX_COMMAND     (16384 * 4)

//Class definition
class ClassCompiler : ClassEStd
{
	friend class ClassMain;
	private:
		ClassEnv env;
		HWND outputwnd;
		bool buildScript;
		bool firstLineInBatch;
		void CheckEnv(EDGEPROJECT *project, unsigned short pnr);
		void ResetPlatform(unsigned short nr);
		bool ReadPlatform(unsigned short nr, char *inipath, char *fname, HWND output);
		void AppendOutput(HWND output, char *text, bool newline = false);
		bool RunCommand(const char *command, HWND output, unsigned long *returnvalue = NULL);
		bool WriteCommand(const char *command, char *batchfile, bool linuxpath);
		bool GetBuildFolder(char *folder, EDGEPROJECT *project, unsigned short platformnr, bool absolutepath = true);
		void GetSimpleFile(char *simplefile, char *projectname);
		void GetRawFile(char *rawfile, char *buildfile);
		void GetObjFile(char *objfile, char *buildfile);
		void GetBldFile(char *objfile, char *buildfile, EDGEPROJECT *project, unsigned short platformnr);
		void OutputBuildString(EDGEPROJECT *project, unsigned short platformnr);
		unsigned long MultiCommand(char *cmd, unsigned char stoplevel = 0, bool linuxpath = false);
		bool CompileResource(char *buildfolder, EDGEPROJECT *project, unsigned short pnr);
		bool CompileSource(char *buildfolder, char *sourcefile, EDGEPROJECT *project, unsigned short pnr);
		bool LinkObjects(EDGEPROJECT *project, unsigned short pnr);
		bool LinkStaticlib(EDGEPROJECT *project, unsigned short pnr);
		bool CompileInstall(char *buildfolder, EDGEPROJECT *project, unsigned short pnr);
	public:
		EDGEPLATFORM platform[MAX_PLATFORMS];
		unsigned short numplatforms;
		char apppath[MAX_EFILESTR];
		ClassCompiler(void);
		~ClassCompiler(void);
		void StartOutput(HWND output);
		void SearchPlatforms(char *applicationpath, HWND droplist, HWND output, char *preferredplatform);
		void BuildSource(ClassEnv *mainenv, EDGEPROJECT *project, unsigned short platformnr, unsigned long sourcefile);
		bool BuildLink(ClassEnv *mainenv, EDGEPROJECT *project, unsigned short platformnr);
		bool BuildProject(ClassEnv *mainenv, EDGEPROJECT *project, unsigned short platformnr);
		bool BuildInstall(ClassEnv *mainenv, EDGEPROJECT *project, unsigned short platformnr);
		void SetBuildScript(bool buildScript = true);
};

/////////////////////////////////////////////////////////////////////
// ClassCompiler: private
/////////////////////////////////////////////////////////////////////

//Check the environment
void ClassCompiler::CheckEnv(EDGEPROJECT *project, unsigned short pnr)
{
	char tmp_str[MAX_EFILESTR * 4];
	unsigned long tmpuid, datelong;
	long result, ctr, depidx;
	unsigned char datechar[5];
	char chr;
	result = env.SearchString("%incpath%", ".");
	if (result >= 0)
	{
		env.RemoveString(result);
		StrCpy(tmp_str, project->loadfolder);
		chr = tmp_str[StrLen(tmp_str) - 1];
		if (chr == '\\' || chr == '/')
			tmp_str[StrLen(tmp_str) - 1] = 0;
		if (platform[pnr].linuxpath)
			StrReplace(tmp_str, "\\", "/");
		env.AddString("incpath", tmp_str);
	}
	StrCpy(tmp_str, getenv("comspec"));
	StrCat(tmp_str, " /C");
	env.AddString("comspec", tmp_str);
	StrCpy(tmp_str, project->loadfolder);
	if (tmp_str[StrLen(tmp_str) - 1] == '\\' || tmp_str[StrLen(tmp_str) - 1] == '/')
	{
		tmp_str[StrLen(tmp_str) - 1] = 0;
		if (tmp_str[StrLen(tmp_str) - 1] == '\\' || tmp_str[StrLen(tmp_str) - 1] == '/')
			tmp_str[StrLen(tmp_str) - 1] = 0;
	}
	env.AddString("prjpath", tmp_str);
	env.AddString("bincaption", project->caption);
	env.AddString("vendor", project->vendor);
	env.AddString("domain", project->domain);
	char * src = project->domain; char * dst = tmp_str;
	while (*src)
	{
		if (*src == '.') 
			*dst = '\\';
		else
			*dst = *src;

		src++; dst++;
	}
	*dst = 0;
	env.AddString("domainpath", tmp_str);
	env.AddString("vendorshort", project->vendorshort);
	env.AddString("platformname", platform[pnr].name);
	tmpuid = project->appuid;
	if (platform[pnr].legacyuid)
	{
		/*if ((tmpuid >> 28) == 0x2)
			tmpuid = (tmpuid & 0xFFFFFFF) | 0x10000000;
		else if ((tmpuid >> 28) == 0xA)
			tmpuid = (tmpuid & 0xFFFFFFF) | 0x10000000;
		else */if ((tmpuid >> 28) == 0xE)
			tmpuid = (tmpuid & 0xFFFFFFF);
		else if ((tmpuid >> 28) == 0xF)
			tmpuid = (tmpuid & 0xFFFFFFF) | 0x10000000;
	}
	else
	{
		if ((tmpuid >> 28) == 0x0)
			tmpuid = (tmpuid & 0xFFFFFFF) | 0xE0000000;
		else if ((tmpuid >> 28) == 0x1)
			tmpuid = (tmpuid & 0xFFFFFFF) | 0xF0000000;
	}
	ClassEStd::IntToStr(tmp_str, tmpuid, 16, false);
	env.AddString("appuid", tmp_str);
	env.AddString("vermajor", project->vermajor);
	env.AddString("verminor", project->verminor);
	env.AddString("buildnr", project->buildnr);
	ClassEStd::IntToStr(tmp_str, ClassERTimer::Count(), 16, false);
	env.AddString("sessionuid", tmp_str);
	ClassEDevice::CurrentDate(datelong, datechar[0], datechar[1]);
	ClassEDevice::CurrentTime(datechar[2], datechar[3], datechar[4]);
	ClassEStd::StrCpy(tmp_str, "00000000.000000");
	tmp_str[0] =  (unsigned char)(datelong / 1000 % 10 + '0');
	tmp_str[1] =  (unsigned char)(datelong / 100 % 10 + '0');
	tmp_str[2] =  (unsigned char)(datelong / 10 % 10 + '0');
	tmp_str[3] =  (unsigned char)(datelong / 1 % 10 + '0');
	tmp_str[4] =  datechar[0] / 10 % 10 + '0';
	tmp_str[5] =  datechar[0] / 1 % 10 + '0';
	tmp_str[6] =  datechar[1] / 10 % 10 + '0';
	tmp_str[7] =  datechar[1] / 1 % 10 + '0';
	tmp_str[9] =  datechar[2] / 10 % 10 + '0';
	tmp_str[10] = datechar[2] / 1 % 10 + '0';
	tmp_str[11] = datechar[3] / 10 % 10 + '0';
	tmp_str[12] = datechar[3] / 1 % 10 + '0';
	tmp_str[13] = datechar[4] / 10 % 10 + '0';
	tmp_str[14] = datechar[4] / 1 % 10 + '0';
	env.AddString("timestamp", tmp_str);
	GetSimpleFile(tmp_str, project->name);
	env.AddString("binname_simple", tmp_str);
	ClassEStd::StrCpy(tmp_str, "");
	for (ctr = 0; ctr < 24; ctr++)
	{
		if ((ctr % 8 == 0 && (project->capabilities[ctr / 8] & 1)) || (ctr % 8 > 0 && (project->capabilities[ctr / 8] & (1 << (ctr % 8)))))
		{
			if (ClassEStd::StrLen(tmp_str))
				ClassEStd::StrCat(tmp_str, "+");
			ClassEStd::StrCat(tmp_str, capability_str[ctr]);
		}
	}
	if (ClassEStd::StrLen(tmp_str) == 0)
		ClassEStd::StrCpy(tmp_str, "None");
	env.AddString("capabilities", tmp_str);
	ClassEStd::StrCpy(tmp_str, "AGC-Dependency-0: http://components.antixlabs.com/if/agp 1.*\r\nAGC-Dependency-1: http://components.antixlabs.com/if/egl 1.3-1.*\r\nAGC-Dependency-2: http://components.antixlabs.com/if/openkode 1.*");
	depidx = 3;
	for (ctr = 0; ctr < 8; ctr++)
	{
		if ((ctr % 8 == 0 && (project->fwdependency & 1)) || (ctr % 8 > 0 && (project->fwdependency & (1 << (ctr % 8)))))
		{
			ClassEStd::StrCat(tmp_str, "\r\nAGC-Dependency-");
			ClassEStd::StrCat(tmp_str, depidx);
			ClassEStd::StrCat(tmp_str, ": ");
			ClassEStd::StrCat(tmp_str, fwdependency_str_long[ctr]);
			depidx++;
		}
	}
	env.AddString("antixdependencies", tmp_str);
	if (project->noresemu)
		env.AddString("disableresemu", "HI_RES_AWARE CEUX {1}");
	env.Restart();
	do
	{
		result = env.SearchString("%pluginlib%");
		if (result >= 0)
		{
			char pluginstr[MAX_ESTR];
			ClassEStd::StrCpy(tmp_str, env.ITGetKeyValue(result));
			env.RemoveString(result);
			if (platform[pnr].linuxpath)
				ClassEStd::StrCpy(pluginstr, "%edgelibpath%/");
			else
				ClassEStd::StrCpy(pluginstr, "%edgelibpath%\\");
			ClassEStd::StrCat(pluginstr, tmp_str);
			env.AddString("dummy", "xxx");
			env.AddString("staticlib", pluginstr);
		}
	}
	while(result >= 0);
}

//Reset platform settings
void ClassCompiler::ResetPlatform(unsigned short nr)
{
	StrCpy(platform[nr].name, "Untitled platform");
	platform[nr].legacyuid = false;
	platform[nr].linuxpath = false;
}

//Read platform settings
bool ClassCompiler::ReadPlatform(unsigned short nr, char *inipath, char *fname, HWND output)
{
	ClassEIni ini;
	char ininame[MAX_EFILESTR];
	if (platform[nr].env)
	{
		delete platform[nr].env;
		platform[nr].env = NULL;
	}
	platform[nr].env = new ClassEnv;
	if (platform[nr].env == NULL)
		return(false);
	platform[nr].legacyuid = false;
	platform[nr].linuxpath = false;
	StrCpy(platform[nr].shortname, "");
	StrCpy(ininame, inipath);
	StrCat(ininame, fname);
	if (ini.Open(ininame, 0))
	{
		char key[256], value[8192], tmp_key[8192];
		while(ini.ReadLine(key, value))
		{
			if (StrEqual(key, "notice", false))
			{
				StrCpy(tmp_key, "Notice: ");
				StrCat(tmp_key, value);
				AppendOutput(output, tmp_key, true);
			}
			else if (StrEqual(key, "defname", false))
				StrCpy(platform[nr].name, value);
			else if (StrEqual(key, "defshortname", false))
				StrCpy(platform[nr].shortname, value);
			else if (StrEqual(key, "legacyuid", false))
			{
				if (ClassEStd::StrToInt(value) > 0)
					platform[nr].legacyuid = true;
			}
			else if (StrEqual(key, "linuxpath", false))
			{
				if (ClassEStd::StrToInt(value) > 0)
					platform[nr].linuxpath = true;
			}
			else
				platform[nr].env->AddString(key, value);
		}
		ini.Close();
		if (StrLen(platform[nr].shortname) == 0)
			StrCpy(platform[nr].shortname, platform[nr].name);
		return(true);
	}
	return(false);
}

//Append the output window with text
void ClassCompiler::AppendOutput(HWND output, char *text, bool newline)
{
	char tmptext[MAX_COMMAND];
	bool onnewline = false;
	unsigned long ctr, tpos, len = strlen(text);
	tpos = 0;
	for (ctr = 0; ctr < len; ctr++)
	{
		if ((text[ctr] == '\r' || text[ctr] == '\n') && !onnewline)
		{
			tmptext[tpos] = '\r';
			tmptext[tpos + 1] = '\n';
			tmptext[tpos + 2] = 0;
			tpos += 2;
			onnewline = true;
		}
		else if (text[ctr] == '\r')
			onnewline = false;
		else if (text[ctr] != '\r' && text[ctr] != '\n')
		{
			onnewline = false;
			tmptext[tpos] = text[ctr];
			tmptext[tpos + 1] = 0;
			tpos++;
		}
	}
	if (newline)
		StrCat(tmptext, "\r\n");
	SendMessage(output, EM_SETSEL, 0xFFFFFFF, 0xFFFFFFF);
	SendMessage(output, EM_REPLACESEL, FALSE, (LPARAM)tmptext);
}

//Run a compile command
bool ClassCompiler::RunCommand(const char *command, HWND output, unsigned long *returnvalue)
{
	char temppath[MAX_PATH];
	char outputfile[MAX_PATH];
	SECURITY_ATTRIBUTES sa;
	STARTUPINFO si;
	HANDLE hOut   = INVALID_HANDLE_VALUE;
	HANDLE hRedir = INVALID_HANDLE_VALUE;
	ZeroMemory(&sa, sizeof(sa));
	sa.nLength        = sizeof(sa);
	sa.bInheritHandle = TRUE;
	GetTempPath(sizeof(temppath), temppath);
	GetTempFileName(temppath, "tmp", 0, outputfile);
	hOut = CreateFile(outputfile, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, 0);
	if (hOut == INVALID_HANDLE_VALUE)
		return(false);
	hRedir = CreateFile(outputfile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	if (hRedir == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hOut);
		DeleteFile(outputfile);
		ClassEConsole::MsgWindow("Invalid hRedir handle");
		return(false);
	}
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	if (hOut != INVALID_HANDLE_VALUE)
	{
		si.dwFlags   |= STARTF_USESTDHANDLES;
		si.hStdOutput  = hOut;
		si.hStdError   = hOut;
		si.wShowWindow = SW_HIDE;
	} 
	char cmd[MAX_COMMAND] = "";
	ClassEStd::StrCat(cmd, command);
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	DWORD dwFlags = CREATE_NO_WINDOW;
	if (!CreateProcess(NULL, (char *)cmd, NULL, NULL, TRUE, dwFlags, NULL, NULL, &si, &pi))
	{
		int err = GetLastError();
		CloseHandle(hOut);
		CloseHandle(hRedir);
		DeleteFile(outputfile);
		SetLastError(err);
		return(false);
	}
	CloseHandle(pi.hThread);
	DWORD dw;
	char buf[256];
	do
	{
		ZeroMemory(&buf,sizeof(buf));
		while (ReadFile(hRedir, &buf, sizeof(buf)-1, &dw, NULL))
		{
			if (dw == 0)
				break;
			AppendOutput(output, buf);
			ZeroMemory(&buf,sizeof(buf));
		}
	}while (WaitForSingleObject(pi.hProcess, 0) != WAIT_OBJECT_0);
	while (ReadFile(hRedir, &buf, sizeof(buf)-1, &dw, NULL))
	{
		if (dw == 0)
			break;
		AppendOutput(output, buf);
		ZeroMemory(&buf,sizeof(buf));
	} 
	WaitForSingleObject(pi.hProcess, INFINITE);
	if (returnvalue != NULL)
		GetExitCodeProcess(pi.hProcess, returnvalue);
	CloseHandle(pi.hProcess);
	CloseHandle(hOut);
	CloseHandle(hRedir);
	DeleteFile(outputfile);
	return(true);
}

//Write command to batch file
bool ClassCompiler::WriteCommand(const char *command, char *batchfile, bool linuxpath)
{
	ClassEFile file;
	unsigned long size = ClassEStd::StrLen(command) * sizeof(char);
	if (firstLineInBatch)
	{
		firstLineInBatch = false;
		if (!file.New(batchfile))
			return(false);
	}
	else
	{
		if (!file.Open(batchfile, EFOF_READWRITE))
			return(false);
	}
	file.Seek(file.Size());
	file.Write(command, size);
	if (linuxpath)
		file.Write("\n", 1);
	else
		file.Write("\r\n", 2);
	
	file.Close();
	return(true);
}

//Retrieve the folder to build to from the project
bool ClassCompiler::GetBuildFolder(char *folder, EDGEPROJECT *project, unsigned short platformnr, bool absolutepath)
{
	ClassEFile ef;
	char absolutefolder[MAX_EFILESTR];
	if (absolutepath)
	{
		StrCpy(folder, project->loadfolder);
		if (platform[platformnr].linuxpath)
			StrReplace(folder, "\\", "/");
		if (platform[platformnr].linuxpath)
			StrCat(folder, "release/");
		else
			StrCat(folder, "release\\");
	}
	else
	{
		if (platform[platformnr].linuxpath)
			StrCpy(folder, "release/");
		else
			StrCpy(folder, "release\\");
	}
	if (!absolutepath)
	{
		StrCpy(absolutefolder, project->loadfolder);
		StrCat(absolutefolder, folder);
	}
	else
		StrCpy(absolutefolder, folder);
	if (!ef.Exists(absolutefolder))
	{
		if (ef.NewFolder(folder))
			ef.CloseFolder();
		else
			return(false);
	}
	StrCat(folder, platform[platformnr].name);
	if (platform[platformnr].linuxpath)
		StrCat(folder, "/");
	else
		StrCat(folder, "\\");
	if (!absolutepath)
	{
		StrCpy(absolutefolder, project->loadfolder);
		StrCat(absolutefolder, folder);
	}
	else
		StrCpy(absolutefolder, folder);
	if (!ef.Exists(absolutefolder))
	{
		if (ef.NewFolder(folder))
			ef.CloseFolder();
		else
			return(false);
	}
	return(true);
}

//Get a simple filename (lowercase) from the projectname
void ClassCompiler::GetSimpleFile(char *simplefile, char *projectname)
{
	unsigned long ctr, len = StrLen(projectname);
	for (ctr = 0; ctr < len; ctr++)
	{
		if ((projectname[ctr] >= 'a' && projectname[ctr] <= 'z') || (projectname[ctr] >= '0' && projectname[ctr] <= '9'))
		{
			simplefile[ctr] = projectname[ctr];
			simplefile[ctr + 1] = 0;
		}
		else if (projectname[ctr] >= 'A' && projectname[ctr] <= 'Z')
		{
			simplefile[ctr] = projectname[ctr] - 'A' + 'a';
			simplefile[ctr + 1] = 0;
		}
		else
		{
			simplefile[ctr] = '_';
			simplefile[ctr + 1] = 0;
		}
	}
}

//Convert the any file string to an raw file string
void ClassCompiler::GetRawFile(char *rawfile, char *buildfile)
{
	unsigned long ctr, len;
	StrCpy(rawfile, buildfile);
	len = StrLen(rawfile);
	for (ctr = 0; ctr < len; ctr++)
		if (rawfile[ctr] == '.')
		{
			rawfile[ctr] = 0;
			return;
		}
}

//Convert the source file string to an object file string
void ClassCompiler::GetObjFile(char *objfile, char *buildfile)
{
	GetRawFile(objfile, buildfile);
	StrCat(objfile, ".obj");
}

//Get the target build file
void ClassCompiler::GetBldFile(char *objfile, char *buildfile, EDGEPROJECT *project, unsigned short platformnr)
{
	GetRawFile(objfile, buildfile);
	if (project->targettype == TARGET_LIB)
		StrCat(objfile, ".lib");
	else
		StrCat(objfile, platform[platformnr].env->GetString("%binextension%", true, true));
}

//Show the building message in the output window
void ClassCompiler::OutputBuildString(EDGEPROJECT *project, unsigned short platformnr)
{
	char showstr[MAX_ESTR];
	StrCpy(showstr, "---------------- Building: ");
	StrCat(showstr, project->name);
	StrCat(showstr, " - ");
	StrCat(showstr, platform[platformnr].name);
	StrCat(showstr, " ");
	StrCat(showstr, "Release");
	StrCat(showstr, " ----------------\r\n");
	AppendOutput(outputwnd, showstr);
}

//Handles multiple resource, compile and link commands
unsigned long ClassCompiler::MultiCommand(char *cmd, unsigned char stoplevel, bool linuxpath)
{
	char startcmd[MAX_COMMAND];
	long numcmd = env.CountKey(cmd), ctr;
	unsigned long lastreturnv = 0;
	for (ctr = 0; ctr < numcmd; ctr++)
	{
		bool cmdresult;
		unsigned long returnvalue;
		StrCpy(startcmd, "%i_00");
		startcmd[3] = (char)(ctr / 10 + '0');
		startcmd[4] = (char)(ctr % 10 + '0');
		StrCat(startcmd, cmd);
		StrCat(startcmd, "_i%");
		env.ScanString(startcmd);
		//ClassEConsole::MsgWindow(startcmd);
		//AppendOutput(outputwnd, startcmd, true);
		cmdresult = false;
		if (startcmd[0] == '!')
		{
			char startfunction[MAX_ESTR];
			if (StrFind(startcmd, "chdir!", false) == 1)
			{
				StrCpy(startfunction, &startcmd[1 + StrLen("chdir!")]);
				StrTrim(startfunction);
				_chdir(startfunction);
				cmdresult = true;
			}
		}
		if (!cmdresult)
		{
			if (buildScript)
			{
				char batchfile[MAX_ESTR];
				ClassEStd::StrCpy(batchfile, "%bldpath%\\build.bat");
				env.ScanString(batchfile);
				cmdresult = WriteCommand(startcmd, batchfile, linuxpath);
				returnvalue = 0;
			}
			else
				cmdresult = RunCommand(startcmd, outputwnd, &returnvalue);
		}
		if (cmdresult == false)
		{
			AppendOutput(outputwnd, "Fatal error: Can't execute command '");
			AppendOutput(outputwnd, startcmd);
			AppendOutput(outputwnd, "'", true);
			if (stoplevel < 2)
				return(0xFFFFFFFF);
		}
		else if (returnvalue > 0)
		{
			if (stoplevel < 1)
				return(returnvalue);
			else if (stoplevel < 2)
				lastreturnv = returnvalue;
		}
	}
	return(lastreturnv);
}

//Generate and compile a resource file
bool ClassCompiler::CompileResource(char *buildfolder, EDGEPROJECT *project, unsigned short pnr)
{
	if (project->cancelbuild)
		return(false);
	ClassEFile ef;
	char tmpstr[MAX_ESTR];
	char genresource[MAX_COMMAND];
	char tmpfname[MAX_RSCTMP][MAX_EFILESTR];
	char binname[MAX_EFILESTR], binfile[MAX_EFILESTR], binfile_dslash[MAX_EFILESTR];
	unsigned long result = 0;
	long len, ctr, pos, idbname, idbfile, idbfile_ds, numrsc = env.CountKey("generateresource");
	_chdir(project->loadfolder);
	GetRawFile(binname, project->name);
	for (ctr = 0; ctr < MAX_RSCTMP; ctr++)
		ClassEStd::StrCpy(tmpfname[ctr], "");
	idbname = env.AddString("binname", binname);
	StrCpy(binfile, buildfolder);
	StrCat(binfile, binname);
	idbfile = env.AddString("binfile", binfile);
	pos = 0;
	len = StrLen(binfile);
	for (ctr = 0; ctr < len; ctr++)
	{
		if (binfile[ctr] == '\\' && binfile[ctr + 1] != '\\')
		{
			binfile_dslash[pos] = '\\';
			binfile_dslash[pos + 1] = '\\';
			binfile_dslash[pos + 2] = 0;
			pos += 2;
		}
		else
		{
			binfile_dslash[pos] = binfile[ctr];
			binfile_dslash[pos + 1] = 0;
			pos++;
		}
	}
	idbfile_ds = env.AddString("binfile_dslash", binfile_dslash);
	if (numrsc > MAX_RSCTMP)
	{
		numrsc = MAX_RSCTMP;
		AppendOutput(outputwnd, "Warning: Number of generated resources exceeds limit", true);
	}
	if (result == 0)
	{
		unsigned long numicons = 0;
		for (ctr = 0; ctr < project->numresourcefiles; ctr++)
		{
			unsigned long currscparent = project->resource[ctr].parent;
			while (currscparent > 0)
			{
				if (project->resourcefld[currscparent - 1].parent == 0)
					break;
				currscparent = project->resourcefld[currscparent - 1].parent;
			}
			if (currscparent > 0)
				if (ClassEStd::StrEqual(project->resourcefld[currscparent - 1].name, "icons", false))
				{
					char iconfile[MAX_EFILESTR];
					StrCpy(iconfile, project->resource[ctr].path);
					StrCat(iconfile, project->resource[ctr].file);
					env.AddString("iconfile", iconfile);
					numicons++;
				}
		}
		if (numicons == 0)
		{
			if (platform[pnr].linuxpath)
				env.AddString("iconfile", "%edgeidepath%/resources/ico16.bmp");
			else
				env.AddString("iconfile", "%edgeidepath%\\resources\\ico16.bmp");
			numicons = 1;
		}
		IntToStr(tmpstr, numicons);
		env.AddString("numicons", tmpstr);
	}
	if (result == 0)
		for (ctr = 0; ctr < numrsc; ctr++)
		{
			bool cmdresult, onquote = false;
			char genparameters[MAX_COMMAND];
			char templatefile[MAX_EFILESTR];
			unsigned long returnvalue, sctr, gslen, parsepos = 0, strstart = 0;
			StrCpy(genresource, "%i_00");
			genresource[3] = (char)(ctr / 10 + '0');
			genresource[4] = (char)(ctr % 10 + '0');
			StrCat(genresource, "generateresource");
			StrCat(genresource, "_i%");
			env.ScanString(genresource);
			gslen = StrLen(genresource);
			for (sctr = 0; sctr < gslen; sctr++)
			{
				if (parsepos < 2)
					if (genresource[sctr] == '"')
						onquote = !onquote;
				if (parsepos < 2)
				{
					bool endparam = false;
					if (sctr == gslen - 1)
						endparam = true;
					else if (genresource[sctr + 1] == ' ' && !onquote)
						endparam = true;
					if (endparam)
					{
						char tmpstr[MAX_EFILESTR];
						StrCpy(tmpstr, &genresource[strstart]);
						tmpstr[sctr - strstart + 1] = 0;
						strstart = sctr + 1;
						StrTrim(tmpstr);
						if (tmpstr[0] == '"' && tmpstr[StrLen(tmpstr) - 1] == '"')
						{
							tmpstr[0] = ' ';
							tmpstr[StrLen(tmpstr) - 1] = ' ';
							StrTrim(tmpstr);
						}
						if (parsepos == 0)
						{
							bool pathabsolute = false;
							if (StrLen(tmpstr) > 2)
							{
								if (tmpstr[0] == '\\' || tmpstr[0] == '/')
									pathabsolute = true;
								if (tmpstr[1] == ':' && (tmpstr[2] == '\\' || tmpstr[2] == '/'))
									pathabsolute = true;
							}
							if (!pathabsolute)
							{
								StrCpy(templatefile, env.GetString("%edgeidepath%", true, true));
								if (platform[pnr].linuxpath)
									StrCat(templatefile, "/templates/");
								else
									StrCat(templatefile, "\\templates\\");
								StrCat(templatefile, tmpstr);
							}
							else
								StrCpy(templatefile, tmpstr);
						}
						else if (parsepos == 1)
						{
							StrCpy(tmpfname[ctr], tmpstr);
							StrCpy(genparameters, &genresource[strstart]);
							StrTrim(genparameters);
							break;
						}
						parsepos++;
					}
				}
			}
			StrCpy(genresource, "\"");
			StrCat(genresource, env.GetString("%edgeidepath%", true, true));
			if (platform[pnr].linuxpath)
				StrCat(genresource, "/bin/genresource\" ");
			else
				StrCat(genresource, "\\bin\\genresource\" ");
			StrCat(genresource, "\"");
			StrCat(genresource, templatefile);
			StrCat(genresource, "\" \"");
			if (tmpfname[ctr][0] == '!')
			{
				StrCat(genresource, &tmpfname[ctr][1]);
				StrCpy(tmpfname[ctr], "");
			}
			else
				StrCat(genresource, tmpfname[ctr]);
			StrCat(genresource, "\" ");
			StrCat(genresource, genparameters);
			if (buildScript)
			{
				char batchfile[MAX_ESTR];
				ClassEStd::StrCpy(batchfile, "%bldpath%\\build.bat");
				env.ScanString(batchfile);
				cmdresult = WriteCommand(genresource, batchfile, platform[pnr].linuxpath);
				returnvalue = 0;
			}
			else
				cmdresult = RunCommand(genresource, outputwnd, &returnvalue);
			if (cmdresult == false)
			{
				AppendOutput(outputwnd, "Fatal error: Can't execute command '");
				AppendOutput(outputwnd, genresource);
				AppendOutput(outputwnd, "'", true);
				result = 1;
			}
			else if (returnvalue > 0)
				result = 1;
		}
	if (result == 0)
		result = MultiCommand("commandresource", 0, platform[pnr].linuxpath);
	if (result == 0)
		result = MultiCommand("commandresource_user", 0, platform[pnr].linuxpath);
	env.RemoveString(idbname);
	env.RemoveString(idbfile);
	env.RemoveString(idbfile_ds);
	for (ctr = 0; ctr < MAX_RSCTMP; ctr++)
		if (!StrEqual(tmpfname[ctr], ""))
			ef.Delete(tmpfname[ctr]);
	if (result > 0)
		return(false);
	return(true);
}

//Compile a source file
bool ClassCompiler::CompileSource(char *buildfolder, char *sourcefile, EDGEPROJECT *project, unsigned short pnr)
{
	if (project->cancelbuild)
		return(false);
	ClassEFile ef;
	char tmp_obj[MAX_EFILESTR], objdst[MAX_EFILESTR];
	long id_src, id_obj;
	unsigned long result;
	GetObjFile(tmp_obj, sourcefile);
	id_src = env.AddString("srcfile", sourcefile);
	id_obj = env.AddString("objfile", tmp_obj);
	StrCpy(objdst, buildfolder);
	StrCat(objdst, tmp_obj);
	ef.Delete(objdst);
	result = MultiCommand("commandcompile", 0, platform[pnr].linuxpath);
	if (result == 0)
		result = MultiCommand("commandcompile_user", 0, platform[pnr].linuxpath);
	env.RemoveString(id_src);
	env.RemoveString(id_obj);
	if (result > 0)
		return(false);
	if (!buildScript && !ef.Exists(objdst))
	{
		AppendOutput(outputwnd, "Error: Error creating compiled source file: ");
		AppendOutput(outputwnd, tmp_obj, true);
		return(false);
	}
	return(true);
}

//Link object files into the build target
bool ClassCompiler::LinkObjects(EDGEPROJECT *project, unsigned short pnr)
{
	if (project->cancelbuild)
		return(false);
	ClassEFile ef;
	char tmp_obj[MAX_EFILESTR], objfile[MAX_EFILESTR], binfile[MAX_EFILESTR], buildfolder[MAX_EFILESTR];
	unsigned long ctr, result;
	bool fixlargeobjects = false;
	if (project->env->SearchString("%eide_fixlargeobjects%") >= 0)
		fixlargeobjects = true;
	_getcwd(tmp_obj, MAX_EFILESTR);
	ef.SetCurrentFolder(tmp_obj);
	GetBuildFolder(buildfolder, project, pnr, false);
	GetRawFile(tmp_obj, project->name);
	env.AddString("binname", tmp_obj);
	StrCpy(binfile, buildfolder);
	StrCat(binfile, tmp_obj);
	env.AddString("binfile", binfile);
	if (project->targettype == TARGET_LIB)
		StrCat(binfile, ".lib");
	else
		StrCat(binfile, env.GetString("%binextension%", true, true));
	ef.Delete(binfile);
	if (fixlargeobjects)
	{
		unsigned long partctr;
		for (partctr = 0; (long)partctr < (project->numsourcefiles + 63) / 64; partctr++)
		{
			long foundobjstridx;
			do
			{
				foundobjstridx = env.SearchString("%objfile%");
				if (foundobjstridx >= 0)
					env.RemoveString(foundobjstridx);
			}
			while(foundobjstridx >= 0);
			for (ctr = partctr * 64; ctr < partctr * 64 + 64 && ctr < project->numsourcefiles; ctr++)
			{
				GetObjFile(tmp_obj, project->source[ctr].file);
				StrCpy(objfile, buildfolder);
				StrCat(objfile, tmp_obj);
				env.AddString("objfile", objfile);
			}
			if (project->targettype == TARGET_LIB)
			{
				result = MultiCommand("commandlib", 0, platform[pnr].linuxpath);
				if (result == 0)
					result = MultiCommand("commandlib_user", 0, platform[pnr].linuxpath);
			}
			else
			{
				result = MultiCommand("commandlink", 0, platform[pnr].linuxpath);
				if (result == 0)
					result = MultiCommand("commandlink_user", 0, platform[pnr].linuxpath);
			}
		}
	}
	else
	{
		for (ctr = 0; ctr < project->numsourcefiles; ctr++)
		{
			GetObjFile(tmp_obj, project->source[ctr].file);
			StrCpy(objfile, buildfolder);
			StrCat(objfile, tmp_obj);
			env.AddString("objfile", objfile);
		}
		if (project->targettype == TARGET_LIB)
		{
			result = MultiCommand("commandlib", 0, platform[pnr].linuxpath);
			if (result == 0)
				result = MultiCommand("commandlib_user", 0, platform[pnr].linuxpath);
		}
		else
		{
			result = MultiCommand("commandlink", 0, platform[pnr].linuxpath);
			if (result == 0)
				result = MultiCommand("commandlink_user", 0, platform[pnr].linuxpath);
		}
	}
	if (result > 0)
		return(false);
	if (!buildScript && !ef.Exists(binfile))
	{
		AppendOutput(outputwnd, "Error: Error creating executable file: ");
		AppendOutput(outputwnd, project->name);
		if (project->targettype == TARGET_LIB)
			AppendOutput(outputwnd, ".lib", true);
		else
			AppendOutput(outputwnd, env.GetString("%binextension%", true, true), true);
		return(false);
	}
	return(true);
}

//Compile the install file
bool ClassCompiler::CompileInstall(char *buildfolder, EDGEPROJECT *project, unsigned short pnr)
{
	if (project->cancelbuild)
		return(false);
	ClassEFile ef;
	char geninstall[MAX_COMMAND];
	char tmpfname[MAX_INSTLTMP][MAX_EFILESTR];
	char binname[MAX_EFILESTR], binfile[MAX_EFILESTR];
	unsigned long result = 0;
	long ctr, idbname, idbfile, numinstall = env.CountKey("generateinstall");
	_chdir(project->loadfolder);
	GetRawFile(binname, project->name);
	for (ctr = 0; ctr < MAX_INSTLTMP; ctr++)
		ClassEStd::StrCpy(tmpfname[ctr], "");
	idbname = env.AddString("binname", binname);
	StrCpy(binfile, buildfolder);
	StrCat(binfile, binname);
	idbfile = env.AddString("binfile", binfile);
	if (numinstall > MAX_INSTLTMP)
	{
		numinstall = MAX_INSTLTMP;
		AppendOutput(outputwnd, "Warning: Number of generated install template files exceeds limit", true);
	}
	if (result == 0)
	{
		unsigned long numinstall = 0;
		for (ctr = 0; ctr < project->numresourcefiles; ctr++)
		{
			unsigned long currscparent = project->resource[ctr].parent;
			while (currscparent > 0)
			{
				if (project->resourcefld[currscparent - 1].parent == 0)
					break;
				currscparent = project->resourcefld[currscparent - 1].parent;
			}
			if (currscparent > 0)
				if (ClassEStd::StrEqual(project->resourcefld[currscparent - 1].name, "install", false))
				{
					char instfile[MAX_EFILESTR], insttmp[MAX_EFILESTR];
					StrCpy(instfile, project->resource[ctr].path);
					StrCat(instfile, project->resource[ctr].file);
					env.AddString("installsrc", instfile);
					ClassEStd::StrCpy(instfile, project->resource[ctr].file);
					currscparent = project->resource[ctr].parent;
					while (currscparent > 0)
					{
						if (project->resourcefld[currscparent - 1].parent == 0)
							break;
						ClassEStd::StrCpy(insttmp, instfile);
						ClassEStd::StrCpy(instfile, project->resourcefld[currscparent - 1].name);
						if (platform[pnr].linuxpath)
							ClassEStd::StrCat(instfile, "/");
						else
							ClassEStd::StrCat(instfile, "\\");
						ClassEStd::StrCat(instfile, insttmp);
						currscparent = project->resourcefld[currscparent - 1].parent;
					}
					env.AddString("installdst", instfile);
					numinstall++;
				}
		}
	}
	if (result == 0)
		for (ctr = 0; ctr < numinstall; ctr++)
		{
			bool cmdresult, onquote = false;
			char genparameters[MAX_COMMAND];
			char templatefile[MAX_EFILESTR];
			unsigned long returnvalue, sctr, gslen, parsepos = 0, strstart = 0;
			StrCpy(geninstall, "%i_00");
			geninstall[3] = (char)(ctr / 10 + '0');
			geninstall[4] = (char)(ctr % 10 + '0');
			StrCat(geninstall, "generateinstall");
			StrCat(geninstall, "_i%");
			env.ScanString(geninstall);
			gslen = StrLen(geninstall);
			for (sctr = 0; sctr < gslen; sctr++)
			{
				if (parsepos < 2)
					if (geninstall[sctr] == '"')
						onquote = !onquote;
				if (parsepos < 2)
				{
					bool endparam = false;
					if (sctr == gslen - 1)
						endparam = true;
					else if (geninstall[sctr + 1] == ' ' && !onquote)
						endparam = true;
					if (endparam)
					{
						char tmpstr[MAX_COMMAND];
						StrCpy(tmpstr, &geninstall[strstart]);
						tmpstr[sctr - strstart + 1] = 0;
						strstart = sctr + 1;
						StrTrim(tmpstr);
						if (tmpstr[0] == '"' && tmpstr[StrLen(tmpstr) - 1] == '"')
						{
							tmpstr[0] = ' ';
							tmpstr[StrLen(tmpstr) - 1] = ' ';
							StrTrim(tmpstr);
						}
						if (parsepos == 0)
						{
							bool pathabsolute = false;
							if (StrLen(tmpstr) > 2)
							{
								if (tmpstr[0] == '\\' || tmpstr[0] == '/')
									pathabsolute = true;
								if (tmpstr[1] == ':' && (tmpstr[2] == '\\' || tmpstr[2] == '/'))
									pathabsolute = true;
							}
							if (!pathabsolute)
							{
								StrCpy(templatefile, env.GetString("%edgeidepath%", true, true));
								if (platform[pnr].linuxpath)
									StrCat(templatefile, "/templates/");
								else
									StrCat(templatefile, "\\templates\\");
								StrCat(templatefile, tmpstr);
							}
							else
								StrCpy(templatefile, tmpstr);
						}
						else if (parsepos == 1)
						{
							StrCpy(tmpfname[ctr], tmpstr);
							StrCpy(genparameters, &geninstall[strstart]);
							StrTrim(genparameters);
							break;
						}
						parsepos++;
					}
				}
			}
			StrCpy(geninstall, "\"");
			StrCat(geninstall, env.GetString("%edgeidepath%", true, true));
			if (platform[pnr].linuxpath)
				StrCat(geninstall, "/bin/genresource\" ");
			else
				StrCat(geninstall, "\\bin\\genresource\" ");
			StrCat(geninstall, "\"");
			StrCat(geninstall, templatefile);
			StrCat(geninstall, "\" \"");
			if (tmpfname[ctr][0] == '!')
			{
				StrCat(geninstall, &tmpfname[ctr][1]);
				StrCpy(tmpfname[ctr], "");
			}
			else
				StrCat(geninstall, tmpfname[ctr]);
			StrCat(geninstall, "\" ");
			StrCat(geninstall, genparameters);
			if (buildScript)
			{
				char batchfile[MAX_ESTR];
				ClassEStd::StrCpy(batchfile, "%bldpath%\\build.bat");
				env.ScanString(batchfile);
				cmdresult = WriteCommand(geninstall, batchfile, platform[pnr].linuxpath);
				returnvalue = 0;
			}
			else
				cmdresult = RunCommand(geninstall, outputwnd, &returnvalue);
			if (cmdresult == false)
			{
				AppendOutput(outputwnd, "Fatal error: Can't execute command '");
				AppendOutput(outputwnd, geninstall);
				AppendOutput(outputwnd, "'", true);
				result = 1;
			}
			else if (returnvalue > 0)
				result = 1;
		}
	if (result == 0)
		result = MultiCommand("commandinstall", 0, platform[pnr].linuxpath);
	if (result == 0)
		result = MultiCommand("commandinstall_user", 0, platform[pnr].linuxpath);
	env.RemoveString(idbname);
	env.RemoveString(idbfile);
	for (ctr = 0; ctr < MAX_INSTLTMP; ctr++)
		if (!StrEqual(tmpfname[ctr], ""))
			ef.Delete(tmpfname[ctr]);
	if (result > 0)
		return(false);
	return(true);
}

/////////////////////////////////////////////////////////////////////
// ClassCompiler: public
/////////////////////////////////////////////////////////////////////

//ClassCompiler: constructor
ClassCompiler::ClassCompiler(void)
{
	buildScript = false;
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_PLATFORMS; ctr++)
		platform[ctr].env = NULL;
}

//ClassCompiler destructor
ClassCompiler::~ClassCompiler(void)
{
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_PLATFORMS; ctr++)
		if (platform[ctr].env != NULL)
		{
			delete platform[ctr].env;
			platform[ctr].env = NULL;
		}
}

//Start output for compiles and builds
void ClassCompiler::StartOutput(HWND output)
{
	outputwnd = output;
	SetWindowText(outputwnd, "");
	SendMessage(outputwnd, WM_SETREDRAW, TRUE, 0);
	SendMessage(outputwnd, EM_SETLIMITTEXT, 0, 0); // set maximum text size
	UpdateWindow(outputwnd);
}

//Search for compiler target platforms
void ClassCompiler::SearchPlatforms(char *applicationpath, HWND droplist, HWND output, char *preferredplatform)
{
	unsigned short defaultplatform = 0;
	char inipath[MAX_EFILESTR];
	char fname[MAX_EFILESTR];
	StrCpy(apppath, applicationpath);
	ClassEFile ef;
	numplatforms = 0;
	AppendOutput(output, "\r\nLoading platforms...\r\n");
	StrCpy(inipath, applicationpath);
	StrCat(inipath, "platforms\\");
	if (ef.OpenFolder(inipath, "*.ini"))
	{
		while (ef.ReadFolder(fname))
		{
			AppendOutput(output, fname);
			AppendOutput(output, "\r\n");
			if (ReadPlatform(numplatforms, inipath, fname, output))
			{
				if (StrEqual(preferredplatform, platform[numplatforms].name, false) || StrEqual(preferredplatform, platform[numplatforms].shortname, false))
					defaultplatform = numplatforms;
				StrCpy(platform[numplatforms].reloadfile, fname);
				SendMessage(droplist, CB_ADDSTRING, 0, (LPARAM)platform[numplatforms].name);
				numplatforms++;
			}
			else
				AppendOutput(output, "Error: Couldn't read platform ini file");
		}
		ef.CloseFolder();
	}
	if (numplatforms == 0)
	{
		AppendOutput(output, "Warning: No platforms found");
		SendMessage(droplist, CB_ADDSTRING, 0, (LPARAM)"No platforms found");
	}
	else
	{
		if (StrEqual(preferredplatform, "select", false))
			defaultplatform = numplatforms;
		else if (StrEqual(preferredplatform, "all", false))
			defaultplatform = numplatforms + 1;
		SendMessage(droplist, CB_ADDSTRING, 0, (LPARAM)"Selected platforms");
		SendMessage(droplist, CB_ADDSTRING, 0, (LPARAM)"All platforms");
	}
	SendMessage(droplist, CB_SETCURSEL, defaultplatform, 0);
}

//Build one source file
void ClassCompiler::BuildSource(ClassEnv *mainenv, EDGEPROJECT *project, unsigned short platformnr, unsigned long sourcefile)
{
	if (!project->loaded || project->cancelbuild)
		return;

	char inipath[MAX_EFILESTR];
	StrCpy(inipath, apppath);
	StrCat(inipath, "platforms\\");
	if (!ReadPlatform(platformnr, inipath, platform[platformnr].reloadfile, outputwnd))
		return;

	if (platformnr >= numplatforms)
	{
		for (unsigned short pctr = 0; pctr < numplatforms; pctr++)
			if (project->selectplatform[pctr] || platformnr >= numplatforms + 1)
				BuildSource(mainenv, project, pctr, sourcefile);
		return;
	}
	ClassEFile sp_ef;
	char buildfolder[MAX_EFILESTR], sourcefolder[MAX_EFILESTR], objfile[MAX_EFILESTR];
	char tmp_str[MAX_ESTR];
	firstLineInBatch = true;
	GetBuildFolder(buildfolder, project, platformnr);
	sp_ef.SetCurrentFolder(project->loadfolder);
	sp_ef.SetCurrentFolder(project->source[sourcefile].path);
	_chdir(sp_ef.GetCurrentFolderC());
	StrCpy(sourcefolder, project->loadfolder);
	StrCat(sourcefolder, project->source[sourcefile].path);
	if (platform[platformnr].linuxpath)
		StrReplace(sourcefolder, "\\", "/");
	OutputBuildString(project, platformnr);
	env.Clear();
	env.CopyFrom(project->env);
	env.CopyFrom(platform[platformnr].env);
	env.CopyFrom(mainenv);
	StrCpy(tmp_str, buildfolder);
	if (tmp_str[StrLen(tmp_str) - 1] == '\\' || tmp_str[StrLen(tmp_str) - 1] == '/')
	{
		tmp_str[StrLen(tmp_str) - 1] = 0;
		if (tmp_str[StrLen(tmp_str) - 1] == '\\' || tmp_str[StrLen(tmp_str) - 1] == '/')
			tmp_str[StrLen(tmp_str) - 1] = 0;
	}
	if (platform[platformnr].linuxpath)
		StrReplace(tmp_str, "\\", "/");
	env.AddString("bldpath", tmp_str);
	env.AddString("srcpath", sourcefolder);
	CheckEnv(project, platformnr);
	if (CompileSource(buildfolder, project->source[sourcefile].file, project, platformnr))
	{
		AppendOutput(outputwnd, "\r\n");
		if (buildScript)
		{
			AppendOutput(outputwnd, "Batch created: build.bat");
		}
		else
		{
			GetObjFile(objfile, project->source[sourcefile].file);
			AppendOutput(outputwnd, objfile);
		}
		AppendOutput(outputwnd, "\r\n");
	}
}

//Link compiled source files
bool ClassCompiler::BuildLink(ClassEnv *mainenv, EDGEPROJECT *project, unsigned short platformnr)
{
	if (!project->loaded || project->cancelbuild)
		return(false);

	char inipath[MAX_EFILESTR];
	StrCpy(inipath, apppath);
	StrCat(inipath, "platforms\\");
	if (!ReadPlatform(platformnr, inipath, platform[platformnr].reloadfile, outputwnd))
		return(false);

	project->buildreturn = RVAL_OK;
	if (platformnr >= numplatforms)
	{
		bool bresult = true;
		for (unsigned short pctr = 0; pctr < numplatforms; pctr++)
			if (project->selectplatform[pctr] || platformnr >= numplatforms + 1)
			{
				if (pctr > 0)
					AppendOutput(outputwnd, "\r\n");
				if (bresult == true)
					bresult = BuildLink(mainenv, project, pctr);
			}
		return(bresult);
	}
	char buildfolder[MAX_EFILESTR], buildfile[MAX_EFILESTR];
	char tmp_str[MAX_ESTR];
	firstLineInBatch = true;
	GetBuildFolder(buildfolder, project, platformnr);
	OutputBuildString(project, platformnr);
	env.Clear();
	env.CopyFrom(project->env);
	env.CopyFrom(platform[platformnr].env);
	env.CopyFrom(mainenv);
	StrCpy(tmp_str, buildfolder);
	if (tmp_str[StrLen(tmp_str) - 1] == '\\' || tmp_str[StrLen(tmp_str) - 1] == '/')
	{
		tmp_str[StrLen(tmp_str) - 1] = 0;
		if (tmp_str[StrLen(tmp_str) - 1] == '\\' || tmp_str[StrLen(tmp_str) - 1] == '/')
			tmp_str[StrLen(tmp_str) - 1] = 0;
	}
	if (platform[platformnr].linuxpath)
		StrReplace(tmp_str, "\\", "/");
	env.AddString("bldpath", tmp_str);
	CheckEnv(project, platformnr);
	_chdir(project->loadfolder);
	if (LinkObjects(project, platformnr))
	{
		AppendOutput(outputwnd, "\r\n");
		if (buildScript)
			AppendOutput(outputwnd, "Batch created: build.bat");
		else
		{
			GetBldFile(buildfile, project->name, project, platformnr);
			AppendOutput(outputwnd, buildfile);
		}
		AppendOutput(outputwnd, "\r\n");
	}
	else
		project->buildreturn = RVAL_ERRLINK;
	return(true);
}

//Build all sources in the project file
bool ClassCompiler::BuildProject(ClassEnv *mainenv, EDGEPROJECT *project, unsigned short platformnr)
{
	if (!project->loaded || project->cancelbuild)
		return(false);
	
	char inipath[MAX_EFILESTR];
	StrCpy(inipath, apppath);
	StrCat(inipath, "platforms\\");
	if (!ReadPlatform(platformnr, inipath, platform[platformnr].reloadfile, outputwnd))
		return(false);

	project->buildreturn = RVAL_OK;
	if (platformnr >= numplatforms)
	{
		bool bresult = true;
		for (unsigned short pctr = 0; pctr < numplatforms; pctr++)
			if (project->selectplatform[pctr] || platformnr >= numplatforms + 1)
			{
				if (pctr > 0)
					AppendOutput(outputwnd, "\r\n");
				if (bresult == true)
					bresult = BuildProject(mainenv, project, pctr);
			}
		return(bresult);
	}
	bool compilesuccess = true;
	unsigned short ctr;
	char buildfolder[MAX_EFILESTR], sourcefolder[MAX_EFILESTR], buildfile[MAX_EFILESTR];
	char tmp_str[MAX_ESTR];
	firstLineInBatch = true;
	GetBuildFolder(buildfolder, project, platformnr);
	OutputBuildString(project, platformnr);
	env.Clear();
	env.CopyFrom(project->env);
	env.CopyFrom(platform[platformnr].env);
	env.CopyFrom(mainenv);
	StrCpy(tmp_str, buildfolder);
	if (tmp_str[StrLen(tmp_str) - 1] == '\\' || tmp_str[StrLen(tmp_str) - 1] == '/')
	{
		tmp_str[StrLen(tmp_str) - 1] = 0;
		if (tmp_str[StrLen(tmp_str) - 1] == '\\' || tmp_str[StrLen(tmp_str) - 1] == '/')
			tmp_str[StrLen(tmp_str) - 1] = 0;
	}
	if (platform[platformnr].linuxpath)
		StrReplace(tmp_str, "\\", "/");
	env.AddString("bldpath", tmp_str);
	CheckEnv(project, platformnr);
	if (project->targettype == TARGET_EXE)
		if (!CompileResource(buildfolder, project, platformnr))
		{
			project->buildreturn = RVAL_ERRRESOURCE;
			return(false);
		}
	for (ctr = 0; ctr < project->numsourcefiles; ctr++)
	{
		long tmp_srcp_id;
		bool cresult;
		StrCpy(sourcefolder, project->loadfolder);
		StrCat(sourcefolder, project->source[ctr].path);
		if (platform[platformnr].linuxpath)
			StrReplace(sourcefolder, "\\", "/");
		_chdir(sourcefolder);
		tmp_srcp_id = env.AddString("srcpath", sourcefolder);
		cresult = CompileSource(buildfolder, project->source[ctr].file, project, platformnr);
		env.RemoveString(tmp_srcp_id);
		if (!cresult)
		{
			project->buildreturn = RVAL_ERRCOMPILE;
			compilesuccess = false;
		}
	}
	if (!compilesuccess)
		return(false);
	_chdir(project->loadfolder);
	if (LinkObjects(project, platformnr))
	{
		AppendOutput(outputwnd, "\r\n");
		if (buildScript)
			AppendOutput(outputwnd, "Batch created: build.bat");
		else
		{
			GetBldFile(buildfile, project->name, project, platformnr);
			AppendOutput(outputwnd, buildfile);
		}
		AppendOutput(outputwnd, "\r\n");
	}
	else
		project->buildreturn = RVAL_ERRLINK;
	return(true);
}

//Build the install file
bool ClassCompiler::BuildInstall(ClassEnv *mainenv, EDGEPROJECT *project, unsigned short platformnr)
{
	if (!project->loaded || project->cancelbuild)
		return(false);
	
	char inipath[MAX_EFILESTR];
	StrCpy(inipath, apppath);
	StrCat(inipath, "platforms\\");
	if (!ReadPlatform(platformnr, inipath, platform[platformnr].reloadfile, outputwnd))
		return(false);

	project->buildreturn = RVAL_OK;
	if (platformnr >= numplatforms)
	{
		bool bresult = true;
		for (unsigned short pctr = 0; pctr < numplatforms; pctr++)
			if (project->selectplatform[pctr] || platformnr >= numplatforms + 1)
			{
				if (pctr > 0)
					AppendOutput(outputwnd, "\r\n");
				if (bresult == true)
					bresult = BuildInstall(mainenv, project, pctr);
			}
		return(bresult);
	}
	char buildfolder[MAX_EFILESTR];
	char tmp_str[MAX_ESTR];
	firstLineInBatch = true;
	GetBuildFolder(buildfolder, project, platformnr);
	OutputBuildString(project, platformnr);
	env.Clear();
	env.CopyFrom(project->env);
	env.CopyFrom(platform[platformnr].env);
	env.CopyFrom(mainenv);
	StrCpy(tmp_str, buildfolder);
	if (tmp_str[StrLen(tmp_str) - 1] == '\\' || tmp_str[StrLen(tmp_str) - 1] == '/')
	{
		tmp_str[StrLen(tmp_str) - 1] = 0;
		if (tmp_str[StrLen(tmp_str) - 1] == '\\' || tmp_str[StrLen(tmp_str) - 1] == '/')
			tmp_str[StrLen(tmp_str) - 1] = 0;
	}
	if (platform[platformnr].linuxpath)
		StrReplace(tmp_str, "\\", "/");
	env.AddString("bldpath", tmp_str);
	CheckEnv(project, platformnr);
	if (CompileInstall(buildfolder, project, platformnr))
	{
		GetRawFile(tmp_str, project->name);
		StrCat(tmp_str, "%installextension%");
		env.ScanString(tmp_str);
		AppendOutput(outputwnd, "\r\n");
		if (buildScript)
			AppendOutput(outputwnd, "Batch created: build.bat");
		else
			AppendOutput(outputwnd, tmp_str);
		AppendOutput(outputwnd, "\r\n");
	}
	else
		project->buildreturn = RVAL_ERRINSTALL;
	return(true);
}

//Enable or disable writing to batch file
void ClassCompiler::SetBuildScript(bool buildScript)
{
	this->buildScript = buildScript;
}
