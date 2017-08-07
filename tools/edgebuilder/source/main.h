// main.h
// Handles the main dialog
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

#define ET_SOURCEFILE     0
#define ET_HEADERFILE     1
#define ET_RESOURCEFILE   2
#define ET_ICONFILE       3
//Extension types

//Class definition
class ClassMain : public ClassEStd
{
	private:
		BUILDTHREADINFO threadinfo;
		PRGSETTINGS settings;
		GUIINFO guiinfo;
		AUTOCMDINFO autocmd;
		HWND editwnd;
		HINSTANCE curinstance;
		EDGEPROJECT *project;
		TREEVIEW_DRAGINFO tvdraginfo;
		HANDLE hthread;
		HANDLE hthreadexit;
		ClassFileTree ftree;
		ClassCompiler compiler;
		ClassEnv      env;
		char applicationpath[MAX_EFILESTR];
		char commandline[MAX_CMDLINE];
		char programpath[512];
		char loadedfile[8192];
		char toexecute[MAX_EFILESTR];
		long *setrval;
		unsigned long editindex;
		unsigned char lastpathtype;
		bool StartBuildThread(BUILDTHREADINFO *info);
		bool ThreadBusy(void);
		bool GetRegistryValue(char *dst, char *key);
		void SetCaption(HWND hwnd, char *projectname, bool updated = false);
		void GetPathFromFile(char *fname);
		void RemovePathFromFile(char *fname);
		void PathToRelative(char *path, char *compare);
		bool SelectPath(HWND hwnd, char *caption, char *newpath, char *startlocation = NULL);
		bool SelectProjectFile(HWND hwnd, bool savedialog = false);
		bool SelectMultiFiles(HWND hwnd, unsigned char extensiontype);
		void InitProject(HWND hwnd);
		void NewProject(HWND hwnd, char *fname = NULL);
		bool OpenProject(HWND hwnd, char *fname = NULL);
		unsigned char SaveProject(HWND hwnd);
		void UpdateProject(HWND hwnd);
		void RemoveProjectItem(unsigned char itemtype, unsigned long itemid);
		void AddProjectFiles(HWND hwnd, unsigned char itemtype, unsigned long itemid, char *filelist);
		void FillFileTree(HWND hwnd);
		void FillPathList(HWND hwnd, unsigned char pathtype, long selection = LB_ERR);
		void CheckSettings(void);
		bool LoadSettings(HWND hwnd);
		void SaveSettings(void);
		bool CheckCommandLine(void);
		void GetGuiInfo(HWND hwnd);
		void GetClientWindowRect(HWND hwnd, HWND clientwnd, RECT *rc);
		void ResizeWindows(HWND hwnd);
		void SwitchEdgePath(void);
	public:
		ClassMain(void);
		~ClassMain(void);
		DWORD BuildThread(BUILDTHREADINFO *info);
		void OpenDialog(EDGESTARTUP *data);
		BOOL CALLBACK MainWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
		BOOL CALLBACK ProjectPropsWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
		BOOL CALLBACK ProjectCapsWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
		BOOL CALLBACK ProjectDepsWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
		BOOL CALLBACK ProjectPathsWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
		BOOL CALLBACK EditPathWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
		BOOL CALLBACK PrgOptionsWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
};

//Main class object
ClassMain main;

//Wrapper prototypes
BOOL CALLBACK MainWinProcWrapper(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK ProjectPropsWinProcWrapper(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK ProjectCapsWinProcWrapper(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK ProjectDepsWinProcWrapper(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK ProjectPathsWinProcWrapper(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK EditPathWinProcWrapper(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK PrgOptionsWinProcWrapper(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);

/////////////////////////////////////////////////////////////////////
// Build thread
/////////////////////////////////////////////////////////////////////

//The thread function for compiling and building the project
DWORD WINAPI BuildThreadProc(LPVOID lpParameter)
{
	BUILDTHREADINFO *info = (BUILDTHREADINFO *)lpParameter;
	return(info->classmain->BuildThread(info));
}

/////////////////////////////////////////////////////////////////////
// Subclassed controls
/////////////////////////////////////////////////////////////////////

//Subclass fix for editing labels and the enter/escape key
BOOL CALLBACK SubClass_TreeEdit(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	WNDPROC ParentClass = (WNDPROC)GetWindowLong(hwnd, GWL_USERDATA);
	switch(wmsg)
	{
		case WM_GETDLGCODE:
			return(DLGC_WANTALLKEYS);
	}
	return(CallWindowProc(ParentClass, hwnd, wmsg, wparam, lparam));
}

//Subclass for setting the start position for a "browse for folder" dialog
int CALLBACK SelectFolderNotify(HWND hwnd, UINT wmsg, LPARAM wparam, LPARAM lparam)
{
	switch (wmsg)
	{
		case BFFM_INITIALIZED:
			char newpath[MAX_EFILESTR];
			ClassEStd::StrCpy(newpath, (char *)lparam);
			if (newpath[ClassEStd::StrLen(newpath) - 1] == '\\')
				newpath[ClassEStd::StrLen(newpath) - 1] = 0;
			SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)newpath);
			break;
		default:
			return(0);
	}
	return(1);
}

/////////////////////////////////////////////////////////////////////
// ClassMain: private
/////////////////////////////////////////////////////////////////////

//The build thread
DWORD ClassMain::BuildThread(BUILDTHREADINFO *info)
{
	compiler.SetBuildScript(settings.buildbatch);
	project->cancelbuild = false;
	switch(info->command)
	{
		case BTC_COMPILE:
			compiler.BuildSource(&env, project, info->platform, info->param);
			break;
		case BTC_LINK:
			compiler.BuildLink(&env, project, info->platform);
			break;
		case BTC_BUILD:
		case BTC_REBUILDALL:
		case BTC_EXECUTE:
			compiler.BuildProject(&env, project, info->platform);
			break;
		case BTC_BUILDINSTALL:
			compiler.BuildInstall(&env, project, info->platform);
			break;
	}
	if (project->cancelbuild)
		compiler.AppendOutput(compiler.outputwnd, "Build cancelled", true);
	SetEvent(hthreadexit);
	return(0);
}

//Start the build thread
bool ClassMain::StartBuildThread(BUILDTHREADINFO *info)
{
	//DWORD threadid;
	char eventname[128];
	ClassEStd::StrCpy(eventname, "eide_buildthread_exit_");
	ClassEStd::StrCat(eventname, GetCurrentProcessId());
	info->classmain = this;
	if (hthreadexit != NULL)
		if (WaitForSingleObject(hthreadexit, 0) != WAIT_OBJECT_0)
			return(false);
	if (hthread != NULL)
	{
		if (hthreadexit != NULL)
		{
			CloseHandle(hthreadexit);
			hthreadexit = NULL;
		}
        CloseHandle(hthread);
		hthread = NULL;
	}
	hthreadexit = CreateEvent(NULL, TRUE, FALSE, eventname);
	if (hthreadexit == NULL)
		return(false);
	hthread = CreateThread(NULL, 0, BuildThreadProc, (void *)info, CREATE_SUSPENDED, NULL);
	if (hthread != NULL)
	{
		Sleep(100);
		ResumeThread(hthread);
	}
	else
	{
		SetEvent(hthreadexit);
		return(false);
	}
	return(true);
}

//Return if the thread is busy
bool ClassMain::ThreadBusy(void)
{
	if (hthreadexit != NULL)
		if (WaitForSingleObject(hthreadexit, 0) != WAIT_OBJECT_0)
			return(true);
	return(false);
}

//Get a registry key value
bool ClassMain::GetRegistryValue(char *dst, char *key)
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

//Set the caption of the main window
void ClassMain::SetCaption(HWND hwnd, char *projectname, bool updated)
{
	char caption[MAX_ESTR];
	if (projectname != NULL)
	{
		if (updated)
			StrCpy(caption, "*");
		else
			StrCpy(caption, "");
		StrCat(caption, projectname);
		StrCat(caption, " - ");
		StrCat(caption, PBO_CAPTION);
	}
	else
		StrCpy(caption, PBO_CAPTION);
	SetWindowText(hwnd, caption);
}

//Get a path from a full file
void ClassMain::GetPathFromFile(char *fname)
{
	long len = StrLen(fname) - 1;
	long ctr;
	for (ctr = len; ctr >= 0; ctr--)
	{
		if (fname[ctr] == '\\' || fname[ctr] == '/')
			break;
		fname[ctr] = 0;
	}
	len = StrLen(fname);
	for (ctr = 0; ctr < len; ctr++)
		if (fname[ctr] == '/')
			fname[ctr] = '\\';
}

//Remove the path from a full file
void ClassMain::RemovePathFromFile(char *fname)
{
	long s_pos = 0, len = StrLen(fname) - 1;
	for (long ctr = len; ctr >= 0; ctr--)
		if (fname[ctr] == '\\' || fname[ctr] == '/')
		{
			s_pos = ctr + 1;
			break;
		}
	ClassEStd::StrCpy(fname, &fname[s_pos]);
}

//Check if a path is relative to one another
void ClassMain::PathToRelative(char *path, char *compare)
{
	if (path[0] == compare[0])
	{
		char tmppath[MAX_EFILESTR];
		unsigned long ctr, len, slashfound = 0, duplfound = 0;
		len = StrLen(path);
		if (StrLen(compare) < (long)len)
			len = StrLen(compare);
		for (ctr = 3; ctr < len; ctr++)
		{
			if (path[ctr] != compare[ctr])
				break;
			else if (path[ctr] == '\\')
				duplfound++;
		}
		len = StrLen(compare);
		for (ctr = 3; ctr < len; ctr++)
			if (compare[ctr] == '\\')
				slashfound++;
		StrCpy(tmppath, path);
		StrCpy(path, "");
		for (ctr = 0; ctr < slashfound - duplfound; ctr++)
			StrCat(path, "..\\");
		len = StrLen(tmppath);
		duplfound++;
		for (ctr = 0; ctr < len; ctr++)
		{
			if (duplfound == 0)
			{
				StrCat(path, &tmppath[ctr]);
				break;
			}
			if (tmppath[ctr] == '\\')
				duplfound--;
		}
	}
}

//Select a new path
bool ClassMain::SelectPath(HWND hwnd, char *caption, char *newpath, char *startlocation)
{
	BROWSEINFO browseinfo;
	LPITEMIDLIST pmidlist;
	LPMALLOC pmalloc;
	ClassEStd::StrCpy(newpath, "");
	SHGetMalloc(&pmalloc);
	browseinfo.hwndOwner      = hwnd;
	browseinfo.pidlRoot       = NULL;
	browseinfo.pszDisplayName = newpath;
	browseinfo.lpszTitle      = caption;
	browseinfo.ulFlags        = BIF_RETURNONLYFSDIRS | 64;
	if (startlocation != NULL)
	{
		browseinfo.lpfn   = SelectFolderNotify;
		browseinfo.lParam = (LPARAM)startlocation;
	}
	else
	{
		browseinfo.lpfn   = NULL;
		browseinfo.lParam = 0;
	}
	browseinfo.iImage         = NULL;
	pmidlist = SHBrowseForFolder(&browseinfo);
	if (pmidlist != NULL)
	{
		SHGetPathFromIDList(pmidlist, newpath);
		pmalloc->Free(pmidlist);
		pmalloc->Release();
		return(true);
	}
	pmalloc->Release();
	return(false);
}

//Select a project file to open
bool ClassMain::SelectProjectFile(HWND hwnd, bool savedialog)
{
	BOOL result;
	OPENFILENAME ofn;
	char filter[256], filename[8192];
	if (programpath[0] == 0)
		_getcwd(programpath, 8192);
	memset(filter, 0, 256);
	memset(filename, 0, 8192);
	StrCpy(filter, "EDGELIB project files");
	StrCat(filter, " (*.epj)");
	StrCpy(&filter[strlen(filter) + 1], "*.epj");
	ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = hwnd;
	ofn.hInstance         = NULL;
	ofn.lpstrFilter       = filter;
    ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter    = 0;
	ofn.nFilterIndex      = 1;
	ofn.lpstrFile         = filename;
	ofn.nMaxFile          = 8192;
	ofn.lpstrFileTitle    = NULL;
	ofn.nMaxFileTitle     = 0;
	ofn.lpstrInitialDir   = programpath;
	if (savedialog)
		ofn.lpstrTitle    = "Save EDGELIB project";
	else
		ofn.lpstrTitle    = "Open EDGELIB project";
	if (savedialog)
		ofn.Flags         = OFN_PATHMUSTEXIST;
	else
		ofn.Flags         = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.nFileOffset       = 0;
	ofn.nFileExtension    = 0;
	ofn.lpstrDefExt       = "*.epj";
	ofn.lCustData         = 0;
	ofn.lpfnHook          = NULL;
	ofn.lpTemplateName    = NULL;
	if (savedialog)
		result = GetSaveFileName(&ofn);
	else
		result = GetOpenFileName(&ofn);
	if (filename[0] == 0 || result == FALSE)
		return(false);
	StrCpy(programpath, filename);
	GetPathFromFile(programpath);
	StrCpy(loadedfile, filename);
	return(true);
}

//Select multiple files
bool ClassMain::SelectMultiFiles(HWND hwnd, unsigned char extensiontype)
{
	OPENFILENAME ofn;
	char filter[256], filename[8192];
	if (programpath[0] == 0)
		_getcwd(programpath, 8192);
	memset(filter, 0, 256);
	memset(filename, 0, 8192);
	if (extensiontype == ET_SOURCEFILE)
	{
		StrCpy(filter, "C/C++ source files");
		StrCat(filter, " (*.c;*.cpp)");
		StrCpy(&filter[strlen(filter) + 1], "*.c;*.cpp");
	}
	else if (extensiontype == ET_HEADERFILE)
	{
		StrCpy(filter, "C/C++ header files");
		StrCat(filter, " (*.h)");
		StrCpy(&filter[strlen(filter) + 1], "*.h");
	}
	else if (extensiontype == ET_RESOURCEFILE)
	{
		StrCpy(filter, "All files");
		StrCat(filter, " (*.*)");
		StrCpy(&filter[strlen(filter) + 1], "*.*");
	}
	else if (extensiontype == ET_ICONFILE)
	{
		StrCpy(filter, "Icon image files");
		StrCat(filter, " (*.bmp;*.gif;*png)");
		StrCpy(&filter[strlen(filter) + 1], "*.bmp;*.gif;*.png");
	}
	ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = hwnd;
	ofn.hInstance         = NULL;
	ofn.lpstrFilter       = filter;
    ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter    = 0;
	ofn.nFilterIndex      = 1;
	ofn.lpstrFile         = filename;
	ofn.nMaxFile          = 8192;
	ofn.lpstrFileTitle    = NULL;
	ofn.nMaxFileTitle     = 0;
	ofn.lpstrInitialDir   = programpath;
	if (extensiontype == ET_SOURCEFILE)
		ofn.lpstrTitle    = "Select source files";
	else if (extensiontype == ET_HEADERFILE)
		ofn.lpstrTitle    = "Select header files";
	else if (extensiontype == ET_RESOURCEFILE)
		ofn.lpstrTitle    = "Select resource files";
	else if (extensiontype == ET_ICONFILE)
		ofn.lpstrTitle    = "Select icon files";
	ofn.Flags             = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_HIDEREADONLY;
	ofn.nFileOffset       = 0;
	ofn.nFileExtension    = 0;
	if (extensiontype == ET_SOURCEFILE)
		ofn.lpstrDefExt   = "*.cpp";
	else if (extensiontype == ET_HEADERFILE)
		ofn.lpstrDefExt   = "*.h";
	else if (extensiontype == ET_RESOURCEFILE)
		ofn.lpstrDefExt   = "*.*";
	else if (extensiontype == ET_ICONFILE)
		ofn.lpstrDefExt   = "*.bmp";
	ofn.lCustData         = 0;
	ofn.lpfnHook          = NULL;
	ofn.lpTemplateName    = NULL;
	if (GetOpenFileName(&ofn) == FALSE)
		return(false);
	if (filename[0] == 0)
		return(false);
	ClassEMemory::Copy(loadedfile, ofn.lpstrFile, 8192);
	return(true);
}

//Initialize the project file
void ClassMain::InitProject(HWND hwnd)
{
	if (!project->loaded)
		return;
	unsigned short ctr;
	unsigned char numselplatform = 0;
	for (ctr = 0; ctr < project->numsourcefiles; ctr++)
	{
		StrCpy(project->source[ctr].path, project->source[ctr].file);
		GetPathFromFile(project->source[ctr].path);
		RemovePathFromFile(project->source[ctr].file);
	}
	for (ctr = 0; ctr < project->numheaderfiles; ctr++)
	{
		StrCpy(project->header[ctr].path, project->header[ctr].file);
		GetPathFromFile(project->header[ctr].path);
		RemovePathFromFile(project->header[ctr].file);
	}
	for (ctr = 0; ctr < project->numresourcefiles; ctr++)
	{
		StrCpy(project->resource[ctr].path, project->resource[ctr].file);
		GetPathFromFile(project->resource[ctr].path);
		RemovePathFromFile(project->resource[ctr].file);
	}
	for (ctr = 0; ctr < MAX_TREETYPES; ctr++)
		project->expandtreetype[ctr] = true;
	if (!autocmd.autobuild && !autocmd.autosetup)
	{
		for (ctr = 0; ctr < MAX_PLATFORMS; ctr++)
			if (project->selectplatform[ctr])
				numselplatform++;
		if (numselplatform == 1)
		{
			for (ctr = 0; ctr < MAX_PLATFORMS; ctr++)
				if (project->selectplatform[ctr])
				{
					SendMessage(GetDlgItem(hwnd, IDC_PLATFORM), CB_SETCURSEL, ctr, 0);
					break;
				}
		}
		else if (numselplatform > 1)
			SendMessage(GetDlgItem(hwnd, IDC_PLATFORM), CB_SETCURSEL, compiler.numplatforms, 0);
	}
	project->updated = false;
	SwitchEdgePath();
}

//Create a new project file
void ClassMain::NewProject(HWND hwnd, char *fname)
{
	unsigned char ctr;
	StrCpy(project->loadfolder, loadedfile);
	StrCpy(project->loadfile, loadedfile);
	GetPathFromFile(project->loadfolder);
	RemovePathFromFile(project->loadfile);
	StrCpy(project->name, "Untitled");
	StrCpy(project->caption, "Untitled");
	StrCpy(project->vendor, settings.prjnewvendor);
	StrCpy(project->vendorshort, settings.prjnewvendorshort);
	StrCpy(project->vermajor, "0");
	StrCpy(project->verminor, "01");
	StrCpy(project->buildnr, "1");
	StrCpy(project->domain, "com.untitled");
	if (project->env == NULL)
		project->env = new ClassEnv;
	project->env->Clear();
	for (ctr = 0; ctr < MAX_PLATFORMS; ctr++)
		project->selectplatform[ctr] = false;
	for (ctr = 0; ctr < 3; ctr++)
		project->capabilities[ctr] = 0;
	StrCpy(project->pathoverride, env.GetString("defedgepath", false));
	project->fwdependency = 0;
	project->numsourcefiles = 0;
	project->numsourcefolders = 0;
	project->numheaderfiles = 0;
	project->numheaderfolders = 0;
	project->numresourcefiles = 0;
	project->numresourcefolders = 0;
	project->appuid = 0x10205D9D;
	project->targettype = TARGET_EXE;
	project->noresemu = true;
	project->loaded = true;
	project->dopathoverride = false;

	if (project->numresourcefolders < MAX_RESOURCEFOLDERS)
	{
		StrCpy(project->resourcefld[project->numresourcefolders].name, "Icons");
		project->resourcefld[project->numresourcefolders].parent = 0;
		project->numresourcefolders++;
	}
	if (project->numresourcefolders < MAX_RESOURCEFOLDERS)
	{
		StrCpy(project->resourcefld[project->numresourcefolders].name, "Install");
		project->resourcefld[project->numresourcefolders].parent = 0;
		project->numresourcefolders++;
	}
	SwitchEdgePath();
}

//Open a project file
bool ClassMain::OpenProject(HWND hwnd, char *fname)
{
	ClassEFile ef;
	char loadfname[MAX_EFILESTR];
	if (fname != NULL)
	{
		bool pathabsolute = false;
		StrCpy(loadedfile, fname);
		if (loadedfile[0] == '\"' && loadedfile[StrLen(loadedfile) - 1] == '\"')
		{
			unsigned long slen = StrLen(loadedfile);
			for (unsigned long ctr = 0; ctr < slen - 1; ctr++)
				loadedfile[ctr] = loadedfile[ctr + 1];
			loadedfile[slen - 2] = 0;
		}
		if (loadedfile[0] == '\\' || loadedfile[0] == '/')
			pathabsolute = true;
		if (loadedfile[1] == ':' && (loadedfile[2] == '\\' || loadedfile[2] == '/'))
			pathabsolute = true;
		if (!pathabsolute)
		{
			char tmpfname[MAX_EFILESTR];
			_getcwd(tmpfname, MAX_EFILESTR);
			StrCat(tmpfname, "\\");
			StrCat(tmpfname, loadedfile);
			StrCpy(loadedfile, tmpfname);
		}
	}
	else
		SelectProjectFile(hwnd);
	StrCpy(loadfname, loadedfile);
	if (project->loaded && project->updated)
		if (MessageBox(hwnd, "Save project?", "EDGELIB Builder", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVEPROJECT, 0);
	StrCpy(loadedfile, loadfname);
	if (ef.Exists(loadedfile))
	{
		char key[MAX_ESTR], value[MAX_ESTR];
		ClassEIni ini;
		if (ini.Open(loadedfile, 0))
		{
			char readkeys[] = ";sourcefile;headerfile;resourcefile;sourcepath;headerpath;resourcepath;endpath;projectname;appuid;target type;noresemu;caption;capabilities;fwdependencies;selplatform;version;vendor;vendorshort;domain;pathoverride;dopathoverride;";
			char tmp_key[MAX_ESTR], tmp_str[MAX_ESTR];
			unsigned short len, ctr, sourceparent = 0, headerparent = 0, resourceparent = 0;
			unsigned char lasttype = TT_NONE;
			StrCpy(project->loadfolder, loadedfile);
			StrCpy(project->loadfile, loadedfile);
			GetPathFromFile(project->loadfolder);
			RemovePathFromFile(project->loadfile);
			for (ctr = 0; ctr < MAX_PLATFORMS; ctr++)
				project->selectplatform[ctr] = false;
			for (ctr = 0; ctr < 3; ctr++)
				project->capabilities[ctr] = 0;
			project->fwdependency = 0;
			project->numsourcefiles = 0;
			project->numheaderfiles = 0;
			project->numresourcefiles = 0;
			project->numsourcefolders = 0;
			project->numheaderfolders = 0;
			project->numresourcefolders = 0;
			project->appuid = 0x10205D9D;
			project->targettype = TARGET_EXE;
			project->noresemu = true;
			project->dopathoverride = false;
			StrCpy(project->pathoverride, "c:\\");
			StrCpy(project->name, "");
			StrCpy(project->caption, "");
			StrCpy(project->vendor, settings.prjnewvendor);
			StrCpy(project->vendorshort, settings.prjnewvendorshort);
			StrCpy(project->domain, "");
			StrCpy(project->vermajor, "0");
			StrCpy(project->verminor, "01");
			StrCpy(project->buildnr, "1");
			if (project->env == NULL)
				project->env = new ClassEnv;
			project->env->Clear();
			while (ini.ReadLine(key, value))
			{
				StrCpy(tmp_key, ";");
				StrCat(tmp_key, key);
				StrCat(tmp_key, ";");
				if (StrFind(readkeys, tmp_key, false) >= 0)
				{
					if (StrEqual(key, "sourcefile", false) && project->numsourcefiles < MAX_SOURCEFILES)
					{
						StrCpy(project->source[project->numsourcefiles].file, value);
						project->source[project->numsourcefiles].parent = sourceparent;
						project->numsourcefiles++;
					}
					else if (StrEqual(key, "headerfile", false) && project->numheaderfiles < MAX_HEADERFILES)
					{
						StrCpy(project->header[project->numheaderfiles].file, value);
						project->header[project->numheaderfiles].parent = headerparent;
						project->numheaderfiles++;
					}
					else if (StrEqual(key, "resourcefile", false) && project->numresourcefiles < MAX_RESOURCEFILES)
					{
						StrCpy(project->resource[project->numresourcefiles].file, value);
						project->resource[project->numresourcefiles].parent = resourceparent;
						project->numresourcefiles++;
					}
					else if (StrEqual(key, "sourcepath", false) && project->numsourcefolders < MAX_SOURCEFOLDERS)
					{
						lasttype = TT_SOURCEFOLDER;
						StrCpy(project->sourcefld[project->numsourcefolders].name, value);
						project->sourcefld[project->numsourcefolders].parent = sourceparent;
						project->sourcefld[project->numsourcefolders].expanded = true;
						sourceparent = project->numsourcefolders + 1;
						project->numsourcefolders++;
					}
					else if (StrEqual(key, "headerpath", false) && project->numheaderfolders < MAX_HEADERFOLDERS)
					{
						lasttype = TT_HEADERFOLDER;
						StrCpy(project->headerfld[project->numheaderfolders].name, value);
						project->headerfld[project->numheaderfolders].parent = headerparent;
						project->headerfld[project->numheaderfolders].expanded = true;
						headerparent = project->numheaderfolders + 1;
						project->numheaderfolders++;
					}
					else if (StrEqual(key, "resourcepath", false) && project->numresourcefolders < MAX_RESOURCEFOLDERS)
					{
						lasttype = TT_RESOURCEFOLDER;
						StrCpy(project->resourcefld[project->numresourcefolders].name, value);
						project->resourcefld[project->numresourcefolders].parent = resourceparent;
						project->resourcefld[project->numresourcefolders].expanded = true;
						resourceparent = project->numresourcefolders + 1;
						project->numresourcefolders++;
					}
					else if (StrEqual(key, "endpath", false))
					{
						if (sourceparent > 0 && lasttype == TT_SOURCEFOLDER)
						{
							if (StrToInt(value))
								project->sourcefld[sourceparent - 1].expanded = false;
							sourceparent = project->sourcefld[sourceparent - 1].parent;
						}
						if (headerparent > 0 && lasttype == TT_HEADERFOLDER)
						{
							if (StrToInt(value))
								project->headerfld[headerparent - 1].expanded = false;
							headerparent = project->headerfld[headerparent - 1].parent;
						}
						if (resourceparent > 0 && lasttype == TT_RESOURCEFOLDER)
						{
							if (StrToInt(value))
								project->resourcefld[resourceparent - 1].expanded = false;
							resourceparent = project->resourcefld[resourceparent - 1].parent;
						}
					}
					else if (StrEqual(key, "appuid", false))
					{
						if (value[1] == 'x' || value[1] == 'X')
							project->appuid = ClassEStd::StrToInt(&value[2], 16);
						else
							project->appuid = ClassEStd::StrToInt(value, 16);
					}
					else if (StrEqual(key, "target type", false))
					{
						if (StrEqual(value, "bin", false))
							project->targettype = TARGET_EXE;
						else if (StrEqual(value, "lib", false))
							project->targettype = TARGET_LIB;
					}
					else if (StrEqual(key, "capabilities", false))
					{
						unsigned short capctr, oncapstart = 0;
						len = (unsigned short)StrLen(value);
						for (ctr = 0; ctr < len; ctr++)
						{
							if (value[ctr] == ',' || value[ctr] == '+' || ctr == len - 1)
							{
								if (ctr < len - 1)
									value[ctr] = 0;
								StrCpy(tmp_str, &value[oncapstart]);
								StrTrim(tmp_str);
								for (capctr = 0; capctr < 24; capctr++)
									if (StrLen(capability_str[capctr]))
										if (StrEqual(tmp_str, capability_str[capctr], false))
										{
											if (capctr % 8)
												project->capabilities[capctr / 8] |= (1 << (capctr % 8));
											else
												project->capabilities[capctr / 8] |= 1;
										}
								oncapstart = ctr + 1;
							}
						}
					}
					else if (StrEqual(key, "fwdependencies"))
					{
						unsigned short capctr, oncapstart = 0;
						len = (unsigned short)StrLen(value);
						for (ctr = 0; ctr < len; ctr++)
						{
							if (value[ctr] == ',' || value[ctr] == '+' || ctr == len - 1)
							{
								if (ctr < len - 1)
									value[ctr] = 0;
								StrCpy(tmp_str, &value[oncapstart]);
								StrTrim(tmp_str);
								for (capctr = 0; capctr < 8; capctr++)
									if (StrLen(fwdependency_str_short[capctr]))
										if (StrEqual(tmp_str, fwdependency_str_short[capctr], false))
										{
											if (capctr % 8)
												project->fwdependency |= (1 << (capctr % 8));
											else
												project->fwdependency |= 1;
										}
								oncapstart = ctr + 1;
							}
						}
					}
					else if (StrEqual(key, "selplatform", false))
					{
						unsigned short spfctr, onspfstart = 0;
						len = (unsigned short)StrLen(value);
						for (ctr = 0; ctr < len; ctr++)
						{
							if (value[ctr] == ',' || value[ctr] == '+' || ctr == len - 1)
							{
								if (ctr < len - 1)
									value[ctr] = 0;
								StrCpy(tmp_str, &value[onspfstart]);
								StrTrim(tmp_str);
								for (spfctr = 0; spfctr < compiler.numplatforms; spfctr++)
									if (StrEqual(tmp_str, compiler.platform[spfctr].name, false))
										project->selectplatform[spfctr] = true;
								onspfstart = ctr + 1;
							}
						}
					}
					else if (StrEqual(key, "version", false))
					{
						long vdot1 = -1, vdot2 = -1;
						vdot1 = StrFind(value, ".");
						if (vdot1 >= 0)
							vdot2 = StrFind(&value[vdot1 + 1], ".");
						if (vdot2 >= 0)
							vdot2 += vdot1 + 1;
						if (vdot1 >= 0)
							value[vdot1] = 0;
						if (vdot2 >= 0)
							value[vdot2] = 0;
						StrCpy(project->vermajor, value);
						if (vdot1 >= 0)
							StrCpy(project->verminor, &value[vdot1 + 1]);
						if (vdot2 >= 0)
							StrCpy(project->buildnr, &value[vdot2 + 1]);
					}
					else if (StrEqual(key, "vendor", false))
						StrCpy(project->vendor, value);
					else if (StrEqual(key, "vendorshort", false))
						StrCpy(project->vendorshort, value);
					else if (StrEqual(key, "noresemu", false))
						project->noresemu = (StrToInt(value) > 0);
					else if (StrEqual(key, "projectname", false))
						StrCpy(project->name, value);
					else if (StrEqual(key, "pathoverride", false))
						StrCpy(project->pathoverride, value);
					else if (StrEqual(key, "dopathoverride", false))
						project->dopathoverride = (StrToInt(value) > 0);
					else if (StrEqual(key, "caption", false))
						StrCpy(project->caption, value);
					else if (StrEqual(key, "domain", false))
						StrCpy(project->domain, value);
				}
				else if (!StrEqual(key, "", false))
				{
					if (StrEqual(key, "incpath", false) || StrEqual(key, "libpath", false))
					{
						while(StrLen(value) > 0)
						{
							if (value[StrLen(value) - 1] == '\\' || value[StrLen(value) - 1] == '/')
								value[StrLen(value) - 1] = 0;
							else
								break;
						}
					}
					project->env->AddString(key, value);
				}
			}
			if (StrEqual(project->name, ""))
			{
				StrCpy(project->name, project->loadfile);
				for (long ctr = 0; ctr < StrLen(project->name); ctr++)
					if (project->name[ctr] == '.')
					{
						project->name[ctr] = 0;
						break;
					}
			}
			if (StrEqual(project->caption, ""))
				StrCpy(project->caption, project->name);
			ini.Close();
			project->loaded = true;
		}
		InitProject(hwnd);
		return(true);
	}
	else
		return(false);
}

//Save the project
unsigned char ClassMain::SaveProject(HWND hwnd)
{
	char tmp_str[MAX_ESTR], tmp_str2[MAX_ESTR];
	char fname[MAX_EFILESTR];
	if (StrEqual(project->loadfile, ""))
	{
		if (!SelectProjectFile(hwnd, true))
			return(1);
		StrCpy(project->loadfolder, loadedfile);
		StrCpy(project->loadfile, loadedfile);
		GetPathFromFile(project->loadfolder);
		RemovePathFromFile(project->loadfile);
	}
	StrCpy(fname, project->loadfolder);
	StrCat(fname, project->loadfile);
	ClassEIni ini;
	if (ini.New(fname))
	{
		unsigned long tmp_uid, numenv;
		unsigned char ctr;
		ini.WriteComment("EDGELIB project file (edit at your own risk)");
		ini.WriteComment("Copyright (c) 2004-2017 Elements Interactive B.V.");
		ini.WriteComment("-----------------------------------------");
		ini.WriteNewline();
		ini.WriteComment("General project properties");
		ini.WriteLine("projectname ", project->name);
		ini.WriteLine("caption     ", project->caption);
		ini.WriteLine("vendor      ", project->vendor);
		ini.WriteLine("vendorshort ", project->vendorshort);
		ini.WriteLine("domain      ", project->domain);
		if (project->targettype == TARGET_EXE)
			ini.WriteLine("target type ", "bin");
		else if (project->targettype == TARGET_LIB)
			ini.WriteLine("target type ", "lib");
		ClassEStd::StrCpy(tmp_str, "0x00000000");
		tmp_uid = project->appuid;
		for (ctr = 0; ctr < 8; ctr++)
		{
			if (tmp_uid % 16 > 9)
				tmp_str[9 - ctr] = 'a' + (unsigned char)(tmp_uid % 16) - 10;
			else
				tmp_str[9 - ctr] = '0' + (unsigned char)(tmp_uid % 16);
			tmp_uid /= 16;
			if (tmp_uid == 0)
				break;
		}
		ini.WriteLine("appuid      ", tmp_str);
		ClassEStd::StrCpy(tmp_str, project->vermajor);
		ClassEStd::StrCat(tmp_str, ".");
		ClassEStd::StrCat(tmp_str, project->verminor);
		ClassEStd::StrCat(tmp_str, ".");
		ClassEStd::StrCat(tmp_str, project->buildnr);
		ini.WriteLine("version     ", tmp_str);

		//Symbian capabilities
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
		ini.WriteLine("capabilities", tmp_str);

		//Antix framework dependencies
		ClassEStd::StrCpy(tmp_str, "");
		for (ctr = 0; ctr < 8; ctr++)
		{
			if ((ctr % 8 == 0 && (project->fwdependency & 1)) || (ctr % 8 > 0 && (project->fwdependency & (1 << (ctr % 8)))))
			{
				if (ClassEStd::StrLen(tmp_str))
					ClassEStd::StrCat(tmp_str, "+");
				ClassEStd::StrCat(tmp_str, fwdependency_str_short[ctr]);
			}
		}
		if (ClassEStd::StrLen(tmp_str) == 0)
			ClassEStd::StrCpy(tmp_str, "None");
		ini.WriteLine("fwdependencies", tmp_str);
		
		//Others
		ClassEStd::StrCpy(tmp_str, "");
		for (ctr = 0; ctr < compiler.numplatforms; ctr++)
		{
			if (project->selectplatform[ctr])
			{
				if (ClassEStd::StrLen(tmp_str))
					ClassEStd::StrCat(tmp_str, ",");
				ClassEStd::StrCat(tmp_str, compiler.platform[ctr].name);
			}
		}
		if (ClassEStd::StrLen(tmp_str) == 0)
			ClassEStd::StrCpy(tmp_str, "None");
		ini.WriteLine("selplatform ", tmp_str);
		if (project->noresemu)
			StrCpy(tmp_str, "1");
		else
			StrCpy(tmp_str, "0");
		ini.WriteLine("noresemu    ", tmp_str);
		if (project->dopathoverride)
			StrCpy(tmp_str, "1");
		else
			StrCpy(tmp_str, "0");
		ini.WriteLine("dopathoverride", tmp_str);
		ini.WriteLine("pathoverride", project->pathoverride);
		ini.WriteNewline();
		if (project->numsourcefiles > 0 || project->numheaderfiles > 0 || project->numresourcefiles > 0 || project->numsourcefolders > 0 || project->numheaderfolders > 0 || project->numresourcefolders > 0)
		{
			bool continuewhile;
			unsigned short ctr, subctr, curparent, childlevel;
			ini.WriteComment("Project source, header and resource tree");
			curparent = 0;
			childlevel = 0;
			for (ctr = 0; ctr < project->numsourcefolders; ctr++)
				project->sourcefld[ctr].treeadded = false;
			for (ctr = 0; ctr < project->numsourcefiles; ctr++)
				project->source[ctr].treeadded = false;
			do
			{
				continuewhile = false;
				for (ctr = 0; ctr < project->numsourcefolders; ctr++)
					if (project->sourcefld[ctr].parent == curparent)
						if (!project->sourcefld[ctr].treeadded)
						{
							StrCpy(tmp_str, "");
							for (subctr = 0; subctr < childlevel; subctr++)
								StrCat(tmp_str, "\t");
							StrCat(tmp_str, "sourcepath");
							ini.WriteLine(tmp_str, project->sourcefld[ctr].name);
							project->sourcefld[ctr].treeadded = true;
							curparent = ctr + 1;
							childlevel++;
							continuewhile = true;
							break;
						}
				if (continuewhile)
					continue;
				for (ctr = 0; ctr < project->numsourcefiles; ctr++)
					if (project->source[ctr].parent == curparent)
						if (!project->source[ctr].treeadded)
						{
							StrCpy(tmp_str, "");
							for (subctr = 0; subctr < childlevel; subctr++)
								StrCat(tmp_str, "\t");
							StrCat(tmp_str, "sourcefile");
							StrCpy(tmp_str2, project->source[ctr].path);
							StrCat(tmp_str2, project->source[ctr].file);
							ini.WriteLine(tmp_str, tmp_str2);
							project->source[ctr].treeadded = true;
						}
				if (curparent > 0)
				{
					StrCpy(tmp_str, "");
					for (subctr = 0; subctr < childlevel - 1; subctr++)
						StrCat(tmp_str, "\t");
					StrCat(tmp_str, "endpath");
					if (!project->sourcefld[curparent - 1].expanded)
						ini.WriteLine(tmp_str, 1);
					else
						ini.WriteLine(tmp_str, (long)0);
					curparent = project->sourcefld[curparent - 1].parent;
					if (childlevel > 0)
						childlevel--;
				}
				else
					break;
			}while(1);
			if (project->numsourcefiles > 0 || project->numsourcefolders > 0)
				ini.WriteNewline();
			curparent = 0;
			childlevel = 0;
			for (ctr = 0; ctr < project->numheaderfolders; ctr++)
				project->headerfld[ctr].treeadded = false;
			for (ctr = 0; ctr < project->numheaderfiles; ctr++)
				project->header[ctr].treeadded = false;
			do
			{
				continuewhile = false;
				for (ctr = 0; ctr < project->numheaderfolders; ctr++)
					if (project->headerfld[ctr].parent == curparent)
						if (!project->headerfld[ctr].treeadded)
						{
							StrCpy(tmp_str, "");
							for (subctr = 0; subctr < childlevel; subctr++)
								StrCat(tmp_str, "\t");
							StrCat(tmp_str, "headerpath");
							ini.WriteLine(tmp_str, project->headerfld[ctr].name);
							project->headerfld[ctr].treeadded = true;
							curparent = ctr + 1;
							childlevel++;
							continuewhile = true;
							break;
						}
				if (continuewhile)
					continue;
				for (ctr = 0; ctr < project->numheaderfiles; ctr++)
					if (project->header[ctr].parent == curparent)
						if (!project->header[ctr].treeadded)
						{
							StrCpy(tmp_str, "");
							for (subctr = 0; subctr < childlevel; subctr++)
								StrCat(tmp_str, "\t");
							StrCat(tmp_str, "headerfile");
							StrCpy(tmp_str2, project->header[ctr].path);
							StrCat(tmp_str2, project->header[ctr].file);
							ini.WriteLine(tmp_str, tmp_str2);
							project->header[ctr].treeadded = true;
						}
				if (curparent > 0)
				{
					StrCpy(tmp_str, "");
					for (subctr = 0; subctr < childlevel - 1; subctr++)
						StrCat(tmp_str, "\t");
					StrCat(tmp_str, "endpath");
					if (!project->headerfld[curparent - 1].expanded)
						ini.WriteLine(tmp_str, 1);
					else
						ini.WriteLine(tmp_str, (long)0);
					curparent = project->headerfld[curparent - 1].parent;
					if (childlevel > 0)
						childlevel--;
				}
				else
					break;
			}while(1);
			if (project->numheaderfiles > 0 || project->numheaderfolders > 0)
				ini.WriteNewline();
			curparent = 0;
			childlevel = 0;
			for (ctr = 0; ctr < project->numresourcefolders; ctr++)
				project->resourcefld[ctr].treeadded = false;
			for (ctr = 0; ctr < project->numresourcefiles; ctr++)
				project->resource[ctr].treeadded = false;
			do
			{
				continuewhile = false;
				for (ctr = 0; ctr < project->numresourcefolders; ctr++)
					if (project->resourcefld[ctr].parent == curparent)
						if (!project->resourcefld[ctr].treeadded)
						{
							StrCpy(tmp_str, "");
							for (subctr = 0; subctr < childlevel; subctr++)
								StrCat(tmp_str, "\t");
							StrCat(tmp_str, "resourcepath");
							ini.WriteLine(tmp_str, project->resourcefld[ctr].name);
							project->resourcefld[ctr].treeadded = true;
							curparent = ctr + 1;
							childlevel++;
							continuewhile = true;
							break;
						}
				if (continuewhile)
					continue;
				for (ctr = 0; ctr < project->numresourcefiles; ctr++)
					if (project->resource[ctr].parent == curparent)
						if (!project->resource[ctr].treeadded)
						{
							StrCpy(tmp_str, "");
							for (subctr = 0; subctr < childlevel; subctr++)
								StrCat(tmp_str, "\t");
							StrCat(tmp_str, "resourcefile");
							StrCpy(tmp_str2, project->resource[ctr].path);
							StrCat(tmp_str2, project->resource[ctr].file);
							ini.WriteLine(tmp_str, tmp_str2);
							project->resource[ctr].treeadded = true;
						}
				if (curparent > 0)
				{
					StrCpy(tmp_str, "");
					for (subctr = 0; subctr < childlevel - 1; subctr++)
						StrCat(tmp_str, "\t");
					StrCat(tmp_str, "endpath");
					if (!project->resourcefld[curparent - 1].expanded)
						ini.WriteLine(tmp_str, 1);
					else
						ini.WriteLine(tmp_str, (long)0);
					curparent = project->resourcefld[curparent - 1].parent;
					if (childlevel > 0)
						childlevel--;
				}
				else
					break;
			}while(1);
			if (project->numresourcefiles > 0 || project->numresourcefolders > 0)
				ini.WriteNewline();
		}
		numenv = project->env->CountKey();
		if (numenv)
		{
			ini.WriteComment("Project environment");
			project->env->Restart();
			for (ctr = 0; ctr < numenv; ctr++)
			{
				project->env->GetNextString(tmp_str, tmp_str2);
				ini.WriteLine(tmp_str, tmp_str2);
			}
			//ini.WriteNewline();
		}
		ini.Close();
	}
	else
		return(2);
	return(0);
}

//Update the project
void ClassMain::UpdateProject(HWND hwnd)
{
	if (!project->updated)
	{
		project->updated = true;
		SetCaption(hwnd, project->name, true);
	}
}

//Remove a project item
void ClassMain::RemoveProjectItem(unsigned char itemtype, unsigned long itemid)
{
	long subctr;
	if (itemtype == TT_SOURCEFOLDER)
	{
		for (subctr = 0; subctr < project->numsourcefolders; subctr++)
		{
			if (project->sourcefld[subctr].parent == itemid + 1)
			{
				RemoveProjectItem(TT_SOURCEFOLDER, subctr);
				subctr--;
			}
			else if (project->sourcefld[subctr].parent > itemid + 1)
				project->sourcefld[subctr].parent--;
		}
		for (subctr = 0; subctr < project->numsourcefiles; subctr++)
		{
			if (project->source[subctr].parent == itemid + 1)
			{
				RemoveProjectItem(TT_SOURCEFILE, subctr);
				subctr--;
			}
			else if (project->source[subctr].parent > itemid + 1)
				project->source[subctr].parent--;
		}
		for (subctr = itemid + 1; subctr < project->numsourcefolders; subctr++)
			ClassEMemory::Copy(&project->sourcefld[subctr - 1], &project->sourcefld[subctr], sizeof(PRJFOLDER));
		project->numsourcefolders--;
		return;
	}
	else if (itemtype == TT_HEADERFOLDER)
	{
		for (subctr = 0; subctr < project->numheaderfolders; subctr++)
		{
			if (project->headerfld[subctr].parent == itemid + 1)
			{
				RemoveProjectItem(TT_HEADERFOLDER, subctr);
				subctr--;
			}
			else if (project->headerfld[subctr].parent > itemid + 1)
				project->headerfld[subctr].parent--;
		}
		for (subctr = 0; subctr < project->numheaderfiles; subctr++)
		{
			if (project->header[subctr].parent == itemid + 1)
			{
				RemoveProjectItem(TT_HEADERFILE, subctr);
				subctr--;
			}
			else if (project->header[subctr].parent > itemid + 1)
				project->header[subctr].parent--;
		}
		for (subctr = itemid + 1; subctr < project->numheaderfolders; subctr++)
			ClassEMemory::Copy(&project->headerfld[subctr - 1], &project->headerfld[subctr], sizeof(PRJFOLDER));
		project->numheaderfolders--;
		return;
	}
	else if (itemtype == TT_RESOURCEFOLDER)
	{
		for (subctr = 0; subctr < project->numresourcefolders; subctr++)
		{
			if (project->resourcefld[subctr].parent == itemid + 1)
			{
				RemoveProjectItem(TT_RESOURCEFOLDER, subctr);
				subctr--;
			}
			else if (project->resourcefld[subctr].parent > itemid + 1)
				project->resourcefld[subctr].parent--;
		}
		for (subctr = 0; subctr < project->numresourcefiles; subctr++)
		{
			if (project->resource[subctr].parent == itemid + 1)
			{
				RemoveProjectItem(TT_RESOURCEFILE, subctr);
				subctr--;
			}
			else if (project->resource[subctr].parent > itemid + 1)
				project->resource[subctr].parent--;
		}
		for (subctr = itemid + 1; subctr < project->numresourcefolders; subctr++)
			ClassEMemory::Copy(&project->resourcefld[subctr - 1], &project->resourcefld[subctr], sizeof(PRJFOLDER));
		project->numresourcefolders--;
		return;
	}
	unsigned long ctr;
	if (itemtype == TT_SOURCEFILE)
	{
		for (ctr = itemid + 1; ctr < project->numsourcefiles; ctr++)
			ClassEMemory::Copy(&project->source[ctr - 1], &project->source[ctr], sizeof(PRJFILE));
		project->numsourcefiles--;
	}
	else if (itemtype == TT_HEADERFILE)
	{
		for (ctr = itemid + 1; ctr < project->numheaderfiles; ctr++)
			ClassEMemory::Copy(&project->header[ctr - 1], &project->header[ctr], sizeof(PRJFILE));
		project->numheaderfiles--;
	}
	else if (itemtype == TT_RESOURCEFILE)
	{
		for (ctr = itemid + 1; ctr < project->numresourcefiles; ctr++)
			ClassEMemory::Copy(&project->resource[ctr - 1], &project->resource[ctr], sizeof(PRJFILE));
		project->numresourcefiles--;
	}
}

//Add files to the project structure
void ClassMain::AddProjectFiles(HWND hwnd, unsigned char itemtype, unsigned long itemid, char *filelist)
{
	char listpath[MAX_EFILESTR], tmppath[MAX_EFILESTR];
	unsigned long ctr, bpos = 0, epos = StrLen(filelist);
	if (filelist[epos] == 0 && filelist[epos + 1] == 0)
	{
		StrCpy(listpath, filelist);
		for (ctr = epos; ctr > 0; ctr--)
			if (listpath[ctr - 1] == '\\')
			{
				listpath[ctr] = 0;
				bpos = ctr;
				break;
			}
	}
	else
	{
		StrCpy(listpath, filelist);
		StrCat(listpath, "\\");
		bpos = epos + 1;
		epos = bpos + StrLen(&filelist[bpos]);
	}
	if (!StrEqual(project->loadfile, ""))
		PathToRelative(listpath, project->loadfolder);
	StrCpy(tmppath, "");
	if (tvdraginfo.itemtype == TT_SOURCEFOLDER || tvdraginfo.itemtype == TT_HEADERFOLDER || tvdraginfo.itemtype == TT_RESOURCEFOLDER)
		tvdraginfo.itemid++;
	do
	{
		char addfile[MAX_ESTR];
		bool addfail;
		if (filelist[bpos] == 0)
			break;
		StrCpy(addfile, listpath);
		StrCat(addfile, &filelist[bpos]);
		bpos = epos + 1;
		epos = bpos + StrLen(&filelist[bpos]);
		if (tvdraginfo.itemtype == TT_SOURCEROOT || tvdraginfo.itemtype == TT_SOURCEFOLDER && project->numsourcefiles < MAX_SOURCEFILES)
		{
			addfail = false;
			StrCpy(project->source[project->numsourcefiles].file, addfile);
			StrCpy(project->source[project->numsourcefiles].path, addfile);
			GetPathFromFile(project->source[project->numsourcefiles].path);
			RemovePathFromFile(project->source[project->numsourcefiles].file);
			for (ctr = 0; ctr < project->numsourcefiles; ctr++)
				if (StrEqual(project->source[ctr].file, project->source[project->numsourcefiles].file))
					if (StrEqual(project->source[ctr].path, project->source[project->numsourcefiles].path))
					{
						StrCat(tmppath, "\r\n");
						StrCat(tmppath, project->source[ctr].path);
						StrCat(tmppath, project->source[ctr].file);
						addfail = true;
						break;
					}
			if (tvdraginfo.itemid == 0)
				project->expandtreetype[TT_SOURCEROOT] = true;
			else
				project->sourcefld[tvdraginfo.itemid - 1].expanded = true;
			project->source[project->numsourcefiles].parent = (unsigned short)tvdraginfo.itemid;
			if (!addfail)
				project->numsourcefiles++;
		}
		else if (tvdraginfo.itemtype == TT_HEADERROOT || tvdraginfo.itemtype == TT_HEADERFOLDER && project->numheaderfiles < MAX_HEADERFILES)
		{
			addfail = false;
			StrCpy(project->header[project->numheaderfiles].file, addfile);
			StrCpy(project->header[project->numheaderfiles].path, addfile);
			GetPathFromFile(project->header[project->numheaderfiles].path);
			RemovePathFromFile(project->header[project->numheaderfiles].file);
			for (ctr = 0; ctr < project->numheaderfiles; ctr++)
				if (StrEqual(project->header[ctr].file, project->header[project->numheaderfiles].file))
					if (StrEqual(project->header[ctr].path, project->header[project->numheaderfiles].path))
					{
						StrCat(tmppath, "\r\n");
						StrCat(tmppath, project->header[ctr].path);
						StrCat(tmppath, project->header[ctr].file);
						addfail = true;
						break;
					}
			if (tvdraginfo.itemid == 0)
				project->expandtreetype[TT_HEADERROOT] = true;
			else
				project->headerfld[tvdraginfo.itemid - 1].expanded = true;
			project->header[project->numheaderfiles].parent = (unsigned short)tvdraginfo.itemid;
			if (!addfail)
				project->numheaderfiles++;
		}
		else if (tvdraginfo.itemtype == TT_RESOURCEROOT || tvdraginfo.itemtype == TT_RESOURCEFOLDER && project->numresourcefiles < MAX_RESOURCEFILES)
		{
			addfail = false;
			StrCpy(project->resource[project->numresourcefiles].file, addfile);
			StrCpy(project->resource[project->numresourcefiles].path, addfile);
			GetPathFromFile(project->resource[project->numresourcefiles].path);
			RemovePathFromFile(project->resource[project->numresourcefiles].file);
			for (ctr = 0; ctr < project->numresourcefiles; ctr++)
				if (StrEqual(project->resource[ctr].file, project->resource[project->numresourcefiles].file))
					if (StrEqual(project->resource[ctr].path, project->resource[project->numresourcefiles].path))
					{
						StrCat(tmppath, "\r\n");
						StrCat(tmppath, project->resource[ctr].path);
						StrCat(tmppath, project->resource[ctr].file);
						addfail = true;
						break;
					}
			if (tvdraginfo.itemid == 0)
				project->expandtreetype[TT_RESOURCEROOT] = true;
			else
				project->resourcefld[tvdraginfo.itemid - 1].expanded = true;
			project->resource[project->numresourcefiles].parent = (unsigned short)tvdraginfo.itemid;
			if (!addfail)
				project->numresourcefiles++;
		}
		else
		{
			MessageBox(hwnd, "No more files can be added because there isn't enough memory", "EDGELIB Builder", MB_OK | MB_ICONERROR);
			return;
		}
	}while(1);
	if (!StrEqual(tmppath, ""))
	{
		StrCpy(listpath, "Couldn't add one or more files to the project because it already exists:\r\n");
		StrCat(listpath, tmppath);
		MessageBox(hwnd, listpath, "EDGELIB Builder", MB_OK | MB_ICONEXCLAMATION);
	}
}

//Fill the project file tree
void ClassMain::FillFileTree(HWND hwnd)
{
	bool continuewhile;
	unsigned short ctr, curparent;
	SendMessage(GetDlgItem(hwnd, IDC_FILES), WM_SETREDRAW, FALSE, 0);
	ftree.SetHwnd(GetDlgItem(hwnd, IDC_FILES));
	ftree.Clear();
	ftree.MakeProjectStructure(project->name);
	curparent = 0;
	for (ctr = 0; ctr < project->numsourcefolders; ctr++)
		project->sourcefld[ctr].treeadded = false;
	for (ctr = 0; ctr < project->numsourcefiles; ctr++)
		project->source[ctr].treeadded = false;
	do
	{
		continuewhile = false;
		for (ctr = 0; ctr < project->numsourcefolders; ctr++)
			if (project->sourcefld[ctr].parent == curparent)
				if (!project->sourcefld[ctr].treeadded)
				{
					if (curparent > 0)
						ftree.AddSourceFolder(&project->sourcefld[ctr], ctr, project->sourcefld[curparent - 1].handle);
					else
						ftree.AddSourceFolder(&project->sourcefld[ctr], ctr);
					project->sourcefld[ctr].treeadded = true;
					curparent = ctr + 1;
					continuewhile = true;
					break;
				}
		if (continuewhile)
			continue;
		for (ctr = 0; ctr < project->numsourcefiles; ctr++)
			if (project->source[ctr].parent == curparent)
				if (!project->source[ctr].treeadded)
				{
					if (curparent > 0)
						ftree.AddSourceFile(project->source[ctr].file, ctr, project->sourcefld[curparent - 1].handle);
					else
						ftree.AddSourceFile(project->source[ctr].file, ctr);
					project->source[ctr].treeadded = true;
				}
		if (curparent > 0)
			curparent = project->sourcefld[curparent - 1].parent;
		else
			break;
	}while(1);
	curparent = 0;
	for (ctr = 0; ctr < project->numheaderfolders; ctr++)
		project->headerfld[ctr].treeadded = false;
	for (ctr = 0; ctr < project->numheaderfiles; ctr++)
		project->header[ctr].treeadded = false;
	do
	{
		continuewhile = false;
		for (ctr = 0; ctr < project->numheaderfolders; ctr++)
			if (project->headerfld[ctr].parent == curparent)
				if (!project->headerfld[ctr].treeadded)
				{
					if (curparent > 0)
						ftree.AddHeaderFolder(&project->headerfld[ctr], ctr, project->headerfld[curparent - 1].handle);
					else
						ftree.AddHeaderFolder(&project->headerfld[ctr], ctr);
					project->headerfld[ctr].treeadded = true;
					curparent = ctr + 1;
					continuewhile = true;
					break;
				}
		if (continuewhile)
			continue;
		for (ctr = 0; ctr < project->numheaderfiles; ctr++)
			if (project->header[ctr].parent == curparent)
				if (!project->header[ctr].treeadded)
				{
					if (curparent > 0)
						ftree.AddHeaderFile(project->header[ctr].file, ctr, project->headerfld[curparent - 1].handle);
					else
						ftree.AddHeaderFile(project->header[ctr].file, ctr);
					project->header[ctr].treeadded = true;
				}
		if (curparent > 0)
			curparent = project->headerfld[curparent - 1].parent;
		else
			break;
	}while(1);
	curparent = 0;
	for (ctr = 0; ctr < project->numresourcefolders; ctr++)
		project->resourcefld[ctr].treeadded = false;
	for (ctr = 0; ctr < project->numresourcefiles; ctr++)
		project->resource[ctr].treeadded = false;
	do
	{
		continuewhile = false;
		for (ctr = 0; ctr < project->numresourcefolders; ctr++)
			if (project->resourcefld[ctr].parent == curparent)
				if (!project->resourcefld[ctr].treeadded)
				{
					if (curparent > 0)
						ftree.AddResourceFolder(&project->resourcefld[ctr], ctr, project->resourcefld[curparent - 1].handle);
					else
						ftree.AddResourceFolder(&project->resourcefld[ctr], ctr);
					project->resourcefld[ctr].treeadded = true;
					curparent = ctr + 1;
					continuewhile = true;
					break;
				}
		if (continuewhile)
			continue;
		for (ctr = 0; ctr < project->numresourcefiles; ctr++)
			if (project->resource[ctr].parent == curparent)
				if (!project->resource[ctr].treeadded)
				{
					if (curparent > 0)
						ftree.AddResourceFile(project->resource[ctr].file, ctr, project->resourcefld[curparent - 1].handle);
					else
						ftree.AddResourceFile(project->resource[ctr].file, ctr);
					project->resource[ctr].treeadded = true;
				}
		if (curparent > 0)
			curparent = project->resourcefld[curparent - 1].parent;
		else
			break;
	}while(1);
	ftree.Expand(project);
	SendMessage(GetDlgItem(hwnd, IDC_FILES), WM_SETREDRAW, TRUE, 0);
}

//Fill the path list of the project
void ClassMain::FillPathList(HWND hwnd, unsigned char pathtype, long selection)
{
	if (pathtype == lastpathtype)
		return;
	char *addstr;
	char searchstr[64];
	if (pathtype == 0)
		ClassEStd::StrCpy(searchstr, "incpath");
	else
		ClassEStd::StrCpy(searchstr, "libpath");
	project->env->Restart();
	addstr = project->env->GetString(searchstr, false);
	SendMessage(hwnd, WM_SETREDRAW, FALSE, 0);
	SendMessage(hwnd, LB_RESETCONTENT, 0, 0);
	while (!StrEqual(addstr, ""))
	{
		SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)addstr);
		addstr = project->env->GetString(searchstr, false);
	}
	if (selection != LB_ERR)
		SendMessage(hwnd, LB_SETCURSEL, selection, 0);
	SendMessage(hwnd, WM_SETREDRAW, TRUE, 0);
	lastpathtype = pathtype;
}

//Check settings and load defaults
void ClassMain::CheckSettings(void)
{
	char tmpstr[MAX_ESTR];
	if (ClassEStd::StrLen(settings.prjnewvendor) == 0)
		ClassEStd::StrCpy(settings.prjnewvendor, "EDGELIB licensee");
	if (ClassEStd::StrLen(settings.prjnewvendorshort) == 0)
		ClassEStd::StrCpy(settings.prjnewvendorshort, "EDGELIB");
	ClassEStd::StrCpy(tmpstr, env.GetString("%defedgepath%", true, true));
	if (ClassEStd::StrLen(tmpstr) == 0)
	{
		if (!GetRegistryValue(tmpstr, "HKLM\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\EdgeSDK_is1\\InstallLocation"))
			ClassEStd::StrCpy(tmpstr, "C:\\projects\\Edge");
	}
	while(env.OTRemoveKey("%defedgepath%", 0));
	env.AddString("defedgepath", tmpstr);
	SwitchEdgePath();
	ClassEStd::StrCpy(tmpstr, env.GetString("%bldtmpfile%", true, true));
	if (ClassEStd::StrLen(tmpstr) == 0)
		ClassEStd::StrCpy(tmpstr, "C:\\edgeide_bldtmp_%appuid%");
	while(env.OTRemoveKey("%bldtmpfile%", 0));
	env.AddString("bldtmpfile", tmpstr);
	if (env.SearchString("%incpath%", "%edgepath%\\include") == -1)
		env.AddString("incpath", "%edgepath%\\include");
	if (env.SearchString("%macrodef%", "__EDGEIDE__") == -1)
		env.AddString("macrodef", "__EDGEIDE__");
	if (env.SearchString("%macrodef%", "__EDGEBUILDER__") == -1)
		env.AddString("macrodef", "__EDGEBUILDER__");
}

void ClassMain::SwitchEdgePath(void)
{
	env.Restart();
	char * edgepath = env.GetString("defedgepath", false);
	if (project->dopathoverride) edgepath = project->pathoverride;
	
	while(env.OTRemoveKey("%edgepath%", 0));
	env.AddString("edgepath", edgepath);	
}

//Load program settings from EdgeBuilder.ini
bool ClassMain::LoadSettings(HWND hwnd)
{
	char fname[MAX_EFILESTR];
	char key[MAX_ESTR], value[MAX_ESTR];
	ClassEIni ini;
	bool editsettings = false;
	StrCpy(fname, applicationpath);
	StrCat(fname, "EdgeBuilder.ini");
	ClassEStd::StrCpy(settings.prjnewvendor, "");
	ClassEStd::StrCpy(settings.prjnewvendorshort, "");
	settings.buildbatch = false;
	env.Clear();
	if (ini.Open(fname, EFOF_READ))
	{
		char tmp_key[MAX_ESTR];
		char readkeys[] = ";notice;pnew_vendor;pnew_vendorshort;buildbatch;";
		compiler.AppendOutput(GetDlgItem(hwnd, IDC_BUILDOUTPUT), "Loading settings from EdgeBuilder.ini...\r\n");
		while (ini.ReadLine(key, value))
		{
			StrCpy(tmp_key, ";");
			StrCat(tmp_key, key);
			StrCat(tmp_key, ";");
			if (StrFind(readkeys, tmp_key, false) >= 0)
			{
				if (StrEqual(key, "notice"))
				{
					StrCpy(tmp_key, "Notice: ");
					StrCat(tmp_key, value);
					StrCat(tmp_key, "\r\n");
					compiler.AppendOutput(GetDlgItem(hwnd, IDC_BUILDOUTPUT), tmp_key);
				}
				else if (StrEqual(key, "pnew_vendor"))
					ClassEStd::StrCpy(settings.prjnewvendor, value);
				else if (StrEqual(key, "pnew_vendorshort"))
					ClassEStd::StrCpy(settings.prjnewvendorshort, value);
				else if (StrEqual(key, "buildbatch") && StrToInt(value) != 0)
					settings.buildbatch = true;
			}
			else
				env.AddString(key, value);
		}
		ini.Close();
	}
	else
		editsettings = true;
	if (!editsettings)
	{
		if (ClassEStd::StrLen(settings.prjnewvendor) == 0)
			editsettings = true;
		else if (ClassEStd::StrLen(settings.prjnewvendorshort) == 0)
			editsettings = true;
		if (env.GetKeyCount("%bldtmpfile%") == 0)
			editsettings = true;
		if (env.GetKeyCount("%defedgepath%") == 0)
			editsettings = true;
	}
	if (editsettings)
		DialogBox(curinstance, MAKEINTRESOURCE(IDD_OPTIONS), NULL, PrgOptionsWinProcWrapper);
	StrCpy(fname, applicationpath);
	if (StrLen(fname) > 0)
		if (fname[StrLen(fname) - 1] == '\\')
			fname[StrLen(fname) - 1] = 0;
	if (StrLen(fname) > 0)
		if (fname[StrLen(fname) - 1] == '\\')
			fname[StrLen(fname) - 1] = 0;
	env.AddString("edgeidepath", fname);
	return(true);
}

//Save program settings to EdgeBuilder.ini
void ClassMain::SaveSettings(void)
{
	ClassEIni ini;
	char fname[MAX_EFILESTR];
	char tmpstr[MAX_ESTR];
	StrCpy(fname, applicationpath);
	StrCat(fname, "EdgeBuilder.ini");
	if (ini.New(fname))
	{
		unsigned long numenv, ctr;
		bool busy, firstline;
		ini.WriteComment("EDGELIB Builder main configuration file");
		ini.WriteComment("");
		ini.WriteComment("Copyright (c) 2004-2017 Elements Interactive B.V.");
		ini.WriteComment("--------------------------------");
		ini.WriteNewline();
		ini.WriteComment("General options");
		ini.WriteLine("bldtmpfile", env.GetString("%bldtmpfile%", true, true));
		if (settings.buildbatch)
			ini.WriteLine("buildbatch", "1");
		else
			ini.WriteLine("buildbatch", "0");
		ini.WriteNewline();
		ini.WriteComment("Default options for new projects");
		ini.WriteLine("pnew_vendor     ", settings.prjnewvendor);
		ini.WriteLine("pnew_vendorshort", settings.prjnewvendorshort);
		ini.WriteNewline();
		ini.WriteComment("Paths");
		ini.WriteLine("defedgepath", env.GetString("%defedgepath%", true, true));
		env.Restart();
		do
		{
			ClassEStd::StrCpy(tmpstr, env.GetString("%incpath%"));
			if (ClassEStd::StrLen(tmpstr) > 0)
			{
				ini.WriteLine("incpath", tmpstr);
				busy = true;
			}
			else
				busy = false;
		}
		while(busy);
		env.Restart();
		do
		{
			ClassEStd::StrCpy(tmpstr, env.GetString("%libpath%"));
			if (ClassEStd::StrLen(tmpstr) > 0)
			{
				ini.WriteLine("libpath", tmpstr);
				busy = true;
			}
			else
				busy = false;
		}
		while(busy);
		ini.WriteNewline();
		ini.WriteComment("Macro list");
		env.Restart();
		do
		{
			ClassEStd::StrCpy(tmpstr, env.GetString("%macrodef%"));
			if (ClassEStd::StrLen(tmpstr) > 0)
			{
				ini.WriteLine("macrodef", tmpstr);
				busy = true;
			}
			else
				busy = false;
		}
		while(busy);
		firstline = true;
		numenv = env.CountKey();
		if (numenv)
		{
			char tmpstr2[MAX_ESTR];
			env.Restart();
			for (ctr = 0; ctr < numenv; ctr++)
			{
				env.GetNextString(tmpstr, tmpstr2);
				if (!(ClassEStd::StrEqual(tmpstr, "defedgepath") || ClassEStd::StrEqual(tmpstr, "bldtmpfile") || ClassEStd::StrEqual(tmpstr, "incpath") || ClassEStd::StrEqual(tmpstr, "libpath") || ClassEStd::StrEqual(tmpstr, "macrodef") || ClassEStd::StrEqual(tmpstr, "edgeidepath")))
				{
					if (firstline)
					{
						ini.WriteNewline();
						ini.WriteComment("Environment");
						firstline = false;
					}
					ini.WriteLine(tmpstr, tmpstr2);
				}
			}
		}
		ini.Close();
	}
}

//Check the commandline, set options
bool ClassMain::CheckCommandLine(void)
{
	char checkarg[MAX_CMDLINE];
	long subctr, ctr, sublen, argnr = 0, pos = 0, len = StrLen(commandline);
	bool oncomment = false;
	autocmd.autobuild = false;
	autocmd.autosetup = false;
	autocmd.showhelp = false;
	StrCpy(autocmd.useplatform, "");
	for (ctr = 0; ctr < len; ctr++)
	{
		if (commandline[ctr] == '"')
			oncomment = !oncomment;
		if (ctr == len - 1 || (commandline[ctr] == ' ' && !oncomment))
		{
			if (ctr < len - 1)
				commandline[ctr] = 0;
			StrCpy(checkarg, &commandline[pos]);
			StrTrim(checkarg);
			pos = ctr + 1;
			if (StrLen(checkarg) > 0)
			{
				if (argnr > 0)
				{
					bool cmderror = true;
					if (checkarg[0] == '"' && checkarg[StrLen(checkarg) - 1] == '"')
					{
						checkarg[0] = ' ';
						checkarg[StrLen(checkarg) - 1] = ' ';
						StrTrim(checkarg);
					}
					if (StrEqual(checkarg, "/autobuild", false) || ClassEStd::StrEqual(checkarg, "--autobuild", false))
					{
						autocmd.autobuild = true;
						cmderror = false;
					}
					else if (StrEqual(checkarg, "/autosetup", false) || ClassEStd::StrEqual(checkarg, "--autosetup", false))
					{
						autocmd.autosetup = true;
						cmderror = false;
					}
					else
					{
						if (cmderror)
						{
							bool platferr = false;
							unsigned long checkplatform = 0;
							if (StrFind(checkarg, "/p", false) == 0)
								checkplatform = StrLen("/p");
							else if (StrFind(checkarg, "--platform", false) == 0)
								checkplatform = StrLen("--platform");
							if (checkplatform > 0)
							{
								char platfstr[MAX_ESTR];
								StrCpy(platfstr, &checkarg[checkplatform]);
								sublen = StrLen(platfstr);
								for (subctr = 0; subctr < sublen; subctr++)
								{
									if (platfstr[subctr] == ':')
										platfstr[subctr] = ' ';
									else if (!IsWhiteSpace(platfstr[subctr]))
									{
										if (subctr == 0 && platfstr[0] != '"')
											platferr = true;
										break;
									}
								}
								if (!platferr)
								{
									StrTrim(platfstr);
									if (platfstr[0] == '"' && platfstr[StrLen(platfstr) - 1] == '"')
									{
										platfstr[0] = ' ';
										platfstr[StrLen(platfstr) - 1] = ' ';
										StrTrim(platfstr);
									}
									StrCpy(autocmd.useplatform, platfstr);
									cmderror = false;
								}
							}
						}
					}
					if (cmderror)
					{
						char errmsg[MAX_CMDLINE];
						StrCpy(errmsg, "Unknown commandline option: ");
						StrCat(errmsg, checkarg);
						MessageBox(NULL, errmsg, "Commandline error", MB_OK | MB_ICONEXCLAMATION);
						return(false);
					}
				}
				else if (StrEqual(checkarg, "/?", false) || ClassEStd::StrEqual(checkarg, "/help", false) || ClassEStd::StrEqual(checkarg, "--help", false))
					autocmd.showhelp = true;
				argnr++;
			}
		}
	}
	return(true);
}

//Get window positioning information
void ClassMain::GetGuiInfo(HWND hwnd)
{
	RECT clientrc, treerc, outputrc;
	HWND treewnd = GetDlgItem(hwnd, IDC_FILES);
	HWND outputwnd = GetDlgItem(hwnd, IDC_BUILDOUTPUT);
	GetClientRect(hwnd, &clientrc);
	GetClientWindowRect(hwnd, treewnd, &treerc);
	GetClientWindowRect(hwnd, outputwnd, &outputrc);
	guiinfo.topspacing    = treerc.top;
	guiinfo.bottomspacing = clientrc.bottom - outputrc.bottom;
	guiinfo.leftspacing   = treerc.left;
	guiinfo.rightspacing  = clientrc.right - outputrc.right;
	guiinfo.outputheight  = outputrc.bottom - outputrc.top;
	guiinfo.subspacing    = outputrc.top - treerc.bottom;
}

//Get the client position relative to the parent window
void ClassMain::GetClientWindowRect(HWND hwnd, HWND clientwnd, RECT *rc)
{
	POINT pnt;
	pnt.x = 0;
	pnt.y = 0;
	ClientToScreen(hwnd, &pnt);
	GetWindowRect(clientwnd, rc);
	rc->left   -= pnt.x;
	rc->top    -= pnt.y;
	rc->right  -= pnt.x;
	rc->bottom -= pnt.y;
}

//Move window items when resizing
void ClassMain::ResizeWindows(HWND hwnd)
{
	RECT clientrc, treerc, outputrc;
	HWND treewnd = GetDlgItem(hwnd, IDC_FILES);
	HWND outputwnd = GetDlgItem(hwnd, IDC_BUILDOUTPUT);
	GetClientRect(hwnd, &clientrc);
	GetClientWindowRect(hwnd, treewnd, &treerc);
	GetClientWindowRect(hwnd, outputwnd, &outputrc);
	treerc.top = clientrc.top + guiinfo.topspacing;
	treerc.bottom = clientrc.bottom - guiinfo.bottomspacing - guiinfo.subspacing - guiinfo.outputheight;
	MoveWindow(treewnd, treerc.left, treerc.top, treerc.right - treerc.left, treerc.bottom - treerc.top, TRUE);
	outputrc.bottom = clientrc.bottom - guiinfo.bottomspacing;
	outputrc.top = outputrc.bottom - guiinfo.outputheight;
	if (outputrc.top < guiinfo.topspacing)
		outputrc.top = guiinfo.topspacing;
	outputrc.left  = guiinfo.leftspacing;
	outputrc.right = clientrc.right - guiinfo.rightspacing;
	MoveWindow(outputwnd, outputrc.left, outputrc.top, outputrc.right - outputrc.left, outputrc.bottom - outputrc.top, TRUE);
}

/////////////////////////////////////////////////////////////////////
// ClassMain: public
/////////////////////////////////////////////////////////////////////

//ClassMain: constructor
ClassMain::ClassMain(void)
{
	hthread = NULL;
	project = new EDGEPROJECT;
	project->env = NULL;

	StrCpy(project->name, "Project");
	StrCpy(project->caption, "Project");
	StrCpy(project->vendor, "Vendor");
	StrCpy(project->vendorshort, "vID");
	StrCpy(project->vermajor, "0");
	StrCpy(project->verminor, "0");
	StrCpy(project->buildnr, "1");
	StrCpy(project->pathoverride, "Path");
	project->appuid = 0xE0000000;
	StrCpy(project->domain, "");
	project->capabilities[0] = 0;
	project->capabilities[1] = 0;
	project->capabilities[2] = 0;
	project->dopathoverride = false;
}

//ClassMain destructor
ClassMain::~ClassMain(void)
{
	if (hthread != NULL)
	{
		if (hthreadexit != NULL)
		{
			CloseHandle(hthreadexit);
			hthreadexit = NULL;
		}
        CloseHandle(hthread);
		hthread = NULL;
	}
	if (project)
	{
		if (project->env != NULL)
			delete project->env;
		delete project;
	}
}

//Open the main dialog box
void ClassMain::OpenDialog(EDGESTARTUP *data)
{
	ClassEFile path_ef;
	INITCOMMONCONTROLSEX cinfo;
	this->project->loaded = false;
	cinfo.dwSize = sizeof(INITCOMMONCONTROLSEX);
	cinfo.dwICC  = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES | ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx(&cinfo);
	curinstance = (HINSTANCE)data->instance;
	StrCpy(applicationpath, path_ef.GetCurrentFolder());
	StrCpy(commandline, data->commandline);
	if (!CheckCommandLine())
	{
		data->returnvalue = RVAL_ERRCMDLINE;
		return;
	}
	if (autocmd.showhelp)
		ClassEConsole::MsgWindow("Usage: EdgeBuilder [file.epj] /options\n\nOptions:\n/autobuild: starts building automatically\n/autosetup: creates a setup automatically\n/?: shows this help screen\n\n/p:[platform]: builds for the specified platform\nSpecify platform name or /p:all, /p:selected");
	setrval = &data->returnvalue;
	DialogBox(curinstance, MAKEINTRESOURCE(IDD_MAIN), NULL, MainWinProcWrapper);
}

//Message handler for the main window
BOOL CALLBACK ClassMain::MainWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	char labeltxt[MAX_ESTR];
	WNDPROC SubClass;
	POINTS dragposition;
	TVITEM tvitem;
	unsigned long result, s_id;
	unsigned char s_type;
	switch(wmsg)
	{
		case WM_COMMAND:
			switch(wparam)
			{
				case ID_FILE_NEWPROJECT:
					if (ThreadBusy())
					{
						MessageBox(hwnd, "Can't complete the requested operation because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
						return(TRUE);
					}
					if (project->loaded && project->updated)
						if (MessageBox(hwnd, "Save project?", "EDGELIB Builder", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
							SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVEPROJECT, 0);
					if (SelectProjectFile(hwnd, true))
						if (!StrEqual(loadedfile, ""))
						{
							NewProject(hwnd, NULL);
							SaveProject(hwnd);
							SetCaption(hwnd, project->name);
							FillFileTree(hwnd);
						}
					return(TRUE);
				case ID_FILE_OPENPROJECT:
					if (ThreadBusy())
					{
						MessageBox(hwnd, "Can't complete the requested operation because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
						return(TRUE);
					}
					if (OpenProject(hwnd))
					{
						FillFileTree(hwnd);
						SetCaption(hwnd, project->name);
					}
					return(TRUE);
				case ID_FILE_SAVEPROJECT:
					result = SaveProject(hwnd);
					if (result == 0)
					{
						SetCaption(hwnd, project->name);
						project->updated = false;
					}
					else if (result == 2)
						MessageBox(hwnd, "An error occured while trying to save the project file.", "Couldn't save project", MB_OK | MB_ICONERROR);
					return(TRUE);
				case ID_FILE_REVERTPROJECT:
					if (ThreadBusy())
					{
						MessageBox(hwnd, "Can't complete the requested operation because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
						return(TRUE);
					}
					if (project->loaded && project->updated)
						if (MessageBox(hwnd, "Reverting project to last saved, changes will be lost. Are you sure?", "EDGELIB Builder", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
						{
							char revertfile[MAX_EFILESTR];
							project->updated = false;
							StrCpy(revertfile, project->loadfolder);
							StrCat(revertfile, project->loadfile);
							if (OpenProject(hwnd, revertfile))
							{
								FillFileTree(hwnd);
								SetCaption(hwnd, project->name);
							}
						}
					return(TRUE);
				case ID_FILE_CLOSEPROJECT:
					if (ThreadBusy())
					{
						MessageBox(hwnd, "Can't complete the requested operation because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
						return(TRUE);
					}
					if (project->loaded && project->updated)
						if (MessageBox(hwnd, "Save project?", "EDGELIB Builder", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
							SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVEPROJECT, 0);
					project->loaded = false;
					ftree.Clear();
					SetCaption(hwnd, NULL);
					return(TRUE);
				case ID_FILE_EXITPROGRAM:
					SendMessage(hwnd, WM_CLOSE, 0, 0);
					return(TRUE);
				case ID_PROJECT_PATHS:
					if (ThreadBusy())
					{
						MessageBox(hwnd, "Can't complete the requested operation because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
						return(TRUE);
					}
					if (project->loaded)
					{
						DialogBox(curinstance, MAKEINTRESOURCE(IDD_PROJECTPATHS), hwnd, ProjectPathsWinProcWrapper);
						UpdateProject(hwnd);
					}
					else
						MessageBox(hwnd, "No project has been loaded", "EDGELIB Builder", MB_OK | MB_ICONEXCLAMATION);
					return(TRUE);
				case ID_PROJECT_PROPERTIES:
					if (ThreadBusy())
					{
						MessageBox(hwnd, "Can't complete the requested operation because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
						return(TRUE);
					}
					if (DialogBox(curinstance, MAKEINTRESOURCE(IDD_PRJPROPERTIES), hwnd, ProjectPropsWinProcWrapper) == IDOK)
					{
						UpdateProject(hwnd);
						FillFileTree(hwnd);
					}
					return(TRUE);
				case ID_BUILD_COMPILE:
					if (ThreadBusy())
					{
						MessageBox(hwnd, "Can't complete the requested operation because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
						return(TRUE);
					}
					if (project->loaded)
					{
						unsigned char s_type;
						unsigned long s_id;
						ftree.GetSelectedItem(s_type, s_id);
						if (s_type == TT_SOURCEFILE)
						{
							compiler.StartOutput(GetDlgItem(hwnd, IDC_BUILDOUTPUT));
							threadinfo.platform = (unsigned short)SendMessage(GetDlgItem(hwnd, IDC_PLATFORM), CB_GETCURSEL, 0, 0);
							threadinfo.command = BTC_COMPILE;
							threadinfo.param = s_id;
							StartBuildThread(&threadinfo);
						}
						else
							MessageBox(hwnd, "Only files in the sourcefile folder can be compiled", "EDGELIB Builder", MB_OK | MB_ICONEXCLAMATION);
					}
					return(TRUE);
				case ID_BUILD_LINK:
					if (ThreadBusy())
					{
						MessageBox(hwnd, "Can't complete the requested operation because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
						return(TRUE);
					}
					if (project->loaded)
					{
						compiler.StartOutput(GetDlgItem(hwnd, IDC_BUILDOUTPUT));
						threadinfo.platform = (unsigned short)SendMessage(GetDlgItem(hwnd, IDC_PLATFORM), CB_GETCURSEL, 0, 0);
						threadinfo.command = BTC_LINK;
						StartBuildThread(&threadinfo);
					}
					return(TRUE);
				case ID_BUILD_BUILD:
				case ID_BUILD_REBUILDALL:
					if (ThreadBusy())
					{
						MessageBox(hwnd, "Can't complete the requested operation because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
						return(TRUE);
					}
					compiler.StartOutput(GetDlgItem(hwnd, IDC_BUILDOUTPUT));
					threadinfo.platform = (unsigned short)SendMessage(GetDlgItem(hwnd, IDC_PLATFORM), CB_GETCURSEL, 0, 0);
					threadinfo.command = BTC_REBUILDALL;
					StartBuildThread(&threadinfo);
					return(TRUE);
				case ID_BUILD_BUILDSETUP:
					if (ThreadBusy())
					{
						MessageBox(hwnd, "Can't complete the requested operation because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
						return(TRUE);
					}
					compiler.StartOutput(GetDlgItem(hwnd, IDC_BUILDOUTPUT));
					threadinfo.platform = (unsigned short)SendMessage(GetDlgItem(hwnd, IDC_PLATFORM), CB_GETCURSEL, 0, 0);
					threadinfo.command = BTC_BUILDINSTALL;
					StartBuildThread(&threadinfo);
					return(TRUE);
				case ID_BUILD_STOPBUILD:
					if (ThreadBusy())
						project->cancelbuild = true;
					else
						MessageBox(hwnd, "Currently no build is in progress", "Can't stop build", MB_OK);
					return(TRUE);
				case ID_BUILD_EXECUTE:
					if (ThreadBusy())
					{
						MessageBox(hwnd, "Can't complete the requested operation because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
						return(TRUE);
					}
					if (project->loaded && project->targettype == TARGET_EXE)
					{
						char bldfile[MAX_EFILESTR];
						compiler.StartOutput(GetDlgItem(hwnd, IDC_BUILDOUTPUT));
						threadinfo.platform = (unsigned short)SendMessage(GetDlgItem(hwnd, IDC_PLATFORM), CB_GETCURSEL, 0, 0);
						threadinfo.command = BTC_EXECUTE;
						compiler.GetBuildFolder(toexecute, project, threadinfo.platform);
						compiler.GetBldFile(bldfile, project->name, project, threadinfo.platform);
						StrCat(toexecute, bldfile);
						StartBuildThread(&threadinfo);
						SetTimer(hwnd, TMR_AUTOEXECUTE, 100, NULL);
					}
					return(TRUE);
				case ID_TOOLS_OPTIONS:
					DialogBox(curinstance, MAKEINTRESOURCE(IDD_OPTIONS), hwnd, PrgOptionsWinProcWrapper);
					return(TRUE);
				case ID_HELP_ABOUT:
					MessageBox(hwnd, PBO_CAPTION "\r\nHelps you organise and build multiplatform C++ projects.\r\n\r\nCopyright (c) 2004-2017 Elements Interactive B.V.", "About EDGELIB Builder", MB_OK | MB_ICONINFORMATION);
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
				/*case ID_TEST_EDITPROJECTFILE:
					if (this->project->loaded)
						ShellExecute(hwnd, "edit", loadedfile, "", programpath, SW_SHOW);
					return(TRUE);
				case ID_TEST_REFRESH:
					FillFileTree(hwnd);
					return(TRUE);
				case ID_TEST_DEBUG:
					ClassEConsole::MsgWindow(project->numsourcefiles);
					ClassEConsole::MsgWindow(project->numheaderfiles);
					return(TRUE);*/
				case ID_TREEFOLDER_ADDFILES:
				case ID_TREEFOLDERROOT_ADDFILES:
					if (ThreadBusy())
					{
						MessageBox(hwnd, "Can't complete the requested operation because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
						return(TRUE);
					}
					if (tvdraginfo.itemtype == TT_SOURCEROOT || tvdraginfo.itemtype == TT_SOURCEFOLDER)
						s_type = ET_SOURCEFILE;
					else if (tvdraginfo.itemtype == TT_HEADERROOT || tvdraginfo.itemtype == TT_HEADERFOLDER)
						s_type = ET_HEADERFILE;
					else if (tvdraginfo.itemtype == TT_RESOURCEROOT || tvdraginfo.itemtype == TT_RESOURCEFOLDER)
						s_type = ET_RESOURCEFILE;
					if (SelectMultiFiles(hwnd, s_type))
					{
						AddProjectFiles(hwnd, tvdraginfo.itemtype, tvdraginfo.itemid, loadedfile);
						FillFileTree(hwnd);
						UpdateProject(hwnd);
					}
					return(TRUE);
				case ID_TREEFOLDER_NEWFOLDER:
				case ID_TREEFOLDERROOT_NEWFOLDER:
					if (ThreadBusy())
					{
						MessageBox(hwnd, "Can't complete the requested operation because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
						return(TRUE);
					}
					result = 0;
					if (tvdraginfo.itemtype == TT_SOURCEROOT || tvdraginfo.itemtype == TT_SOURCEFOLDER)
					{
						if (project->numsourcefolders < MAX_SOURCEFOLDERS)
						{
							result = project->numsourcefolders;
							StrCpy(project->sourcefld[result].name, "new folder");
							if (tvdraginfo.itemtype == TT_SOURCEFOLDER)
							{
								project->sourcefld[result].parent = (unsigned short)(tvdraginfo.itemid + 1);
								project->sourcefld[tvdraginfo.itemid].expanded = true;
							}
							else
							{
								project->sourcefld[result].parent = 0;
								project->expandtreetype[TT_SOURCEROOT] = true;
							}
							project->numsourcefolders++;
							result = 1;
						}
					}
					else if (tvdraginfo.itemtype == TT_HEADERROOT || tvdraginfo.itemtype == TT_HEADERFOLDER)
					{
						if (project->numheaderfolders < MAX_HEADERFOLDERS)
						{
							result = project->numheaderfolders;
							StrCpy(project->headerfld[result].name, "new folder");
							if (tvdraginfo.itemtype == TT_HEADERFOLDER)
							{
								project->headerfld[result].parent = (unsigned short)(tvdraginfo.itemid + 1);
								project->headerfld[tvdraginfo.itemid].expanded = true;
							}
							else
							{
								project->headerfld[result].parent = 0;
								project->expandtreetype[TT_HEADERROOT] = true;
							}
							project->numheaderfolders++;
							result = 1;
						}
					}
					else if (tvdraginfo.itemtype == TT_RESOURCEROOT || tvdraginfo.itemtype == TT_RESOURCEFOLDER)
					{
						if (project->numresourcefolders < MAX_RESOURCEFOLDERS)
						{
							result = project->numresourcefolders;
							StrCpy(project->resourcefld[result].name, "new folder");
							if (tvdraginfo.itemtype == TT_RESOURCEFOLDER)
							{
								project->resourcefld[result].parent = (unsigned short)(tvdraginfo.itemid + 1);
								project->resourcefld[tvdraginfo.itemid].expanded = true;
							}
							else
							{
								project->resourcefld[result].parent = 0;
								project->expandtreetype[TT_RESOURCEROOT] = true;
							}
							project->numresourcefolders++;
							result = 1;
						}
					}
					if (result)
					{
						FillFileTree(hwnd);
						UpdateProject(hwnd);
					}
					else
						MessageBox(hwnd, "The limit for adding folders to the tree has been exceeded", "EDGELIB Builder", MB_OK | MB_ICONEXCLAMATION);
					return(TRUE);
				case ID_TREEFOLDER_REMOVE:
				case ID_TREEFILE_REMOVE:
					if (ThreadBusy())
					{
						MessageBox(hwnd, "Can't complete the requested operation because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
						return(TRUE);
					}
					RemoveProjectItem(tvdraginfo.itemtype, tvdraginfo.itemid);
					FillFileTree(hwnd);
					UpdateProject(hwnd);
					return(TRUE);
			}
			break;
		case WM_NOTIFY:
			switch(wparam)
			{
				case IDC_FILES:
					LPNMTREEVIEW tvinfo;
					LPNMTVDISPINFO dispinfo;
					tvinfo = (LPNMTREEVIEW)lparam;
					dispinfo = (LPNMTVDISPINFO)lparam;
					switch(((LPNMHDR)lparam)->code)
					{
						case TVN_ITEMEXPANDED:
							tvitem.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
							tvitem.hItem = tvinfo->itemNew.hItem;
							if (TreeView_GetItem(GetDlgItem(hwnd, IDC_FILES), &tvitem) == TRUE)
							{
								if (tvitem.iImage == TI_FOLDEROPEN || tvitem.iImage == TI_FOLDERCLOSE || tvitem.iImage == TI_RESFOLDEROPEN || tvitem.iImage == TI_RESFOLDERCLOSE)
								{
									ftree.ConvertParam(tvitem.lParam, s_type, s_id);
									if (tvinfo->action == TVE_EXPAND)
									{
										if (tvitem.iImage == TI_FOLDEROPEN || tvitem.iImage == TI_FOLDERCLOSE)
											tvitem.iImage = TI_FOLDEROPEN;
										else
											tvitem.iImage = TI_RESFOLDEROPEN;
										if (s_type == TT_SOURCEFOLDER)
											project->sourcefld[s_id].expanded = true;
										else if (s_type == TT_HEADERFOLDER)
											project->headerfld[s_id].expanded = true;
										else if (s_type == TT_RESOURCEFOLDER)
											project->resourcefld[s_id].expanded = true;
										else
											project->expandtreetype[s_type] = true;
									}
									else if (tvinfo->action == TVE_COLLAPSE)
									{
										if (tvitem.iImage == TI_FOLDEROPEN || tvitem.iImage == TI_FOLDERCLOSE)
											tvitem.iImage = TI_FOLDERCLOSE;
										else
											tvitem.iImage = TI_RESFOLDERCLOSE;
										if (s_type == TT_SOURCEFOLDER)
											project->sourcefld[s_id].expanded = false;
										else if (s_type == TT_HEADERFOLDER)
											project->headerfld[s_id].expanded = false;
										else if (s_type == TT_RESOURCEFOLDER)
											project->resourcefld[s_id].expanded = false;
										else
											project->expandtreetype[s_type] = false;
									}
									tvitem.iSelectedImage = tvitem.iImage;
									TreeView_SetItem(GetDlgItem(hwnd, IDC_FILES), &tvitem);
								}
							}
							break;
						case TVN_BEGINLABELEDIT:
							if (ThreadBusy())
							{
								PostMessage(GetDlgItem(hwnd, IDC_FILES), TVM_ENDEDITLABELNOW, FALSE, 0);
								break;
							}
							editwnd = TreeView_GetEditControl(GetDlgItem(hwnd, IDC_FILES));
							SubClass = (WNDPROC)SetWindowLong(editwnd, GWL_WNDPROC, (LONG)SubClass_TreeEdit);
							SetWindowLong(editwnd, GWL_USERDATA, (unsigned long)SubClass);
							ftree.ConvertParam(dispinfo->item.lParam, s_type, s_id);
							if (s_type == TT_SOURCEROOT || s_type == TT_HEADERROOT || s_type == TT_RESOURCEROOT)
							{
								PostMessage(GetDlgItem(hwnd, IDC_FILES), TVM_ENDEDITLABELNOW, FALSE, 0);
								break;
							}
							if (s_type != TT_NONE)
							{
								if (s_type == TT_SOURCEFILE)
								{
									StrCpy(labeltxt, project->source[s_id].path);
									StrCat(labeltxt, project->source[s_id].file);
									SetWindowText(editwnd, labeltxt);
								}
								else if (s_type == TT_HEADERFILE)
								{
									StrCpy(labeltxt, project->header[s_id].path);
									StrCat(labeltxt, project->header[s_id].file);
									SetWindowText(editwnd, labeltxt);
								}
								else if (s_type == TT_RESOURCEFILE)
								{
									StrCpy(labeltxt, project->resource[s_id].path);
									StrCat(labeltxt, project->resource[s_id].file);
									SetWindowText(editwnd, labeltxt);
								}
							}
							else
								return(TRUE);
							break;
						case TVN_ENDLABELEDIT:
							tvitem.hItem = dispinfo->item.hItem;
							ftree.ConvertParam(dispinfo->item.lParam, s_type, s_id);
							if (tvitem.hItem != NULL && dispinfo->item.pszText != NULL)
								if (StrLen(dispinfo->item.pszText) > 0)
								{
									GetWindowText(editwnd, labeltxt, sizeof(labeltxt));
									tvitem.mask = TVIF_HANDLE | TVIF_TEXT;
									tvitem.pszText = labeltxt;
									if (s_type == TT_ROOT)
									{
										StrCpy(project->name, labeltxt);
										SetCaption(hwnd, project->name, project->updated);
									}
									else if (s_type == TT_SOURCEFILE)
									{
										StrCpy(project->source[s_id].file, labeltxt);
										StrCpy(project->source[s_id].path, labeltxt);
										GetPathFromFile(project->source[s_id].path);
										RemovePathFromFile(project->source[s_id].file);
										StrCpy(labeltxt, project->source[s_id].file);
									}
									else if (s_type == TT_HEADERFILE)
									{
										StrCpy(project->header[s_id].file, labeltxt);
										StrCpy(project->header[s_id].path, labeltxt);
										GetPathFromFile(project->header[s_id].path);
										RemovePathFromFile(project->header[s_id].file);
										StrCpy(labeltxt, project->header[s_id].file);
									}
									else if (s_type == TT_RESOURCEFILE)
									{
										StrCpy(project->resource[s_id].file, labeltxt);
										StrCpy(project->resource[s_id].path, labeltxt);
										GetPathFromFile(project->resource[s_id].path);
										RemovePathFromFile(project->resource[s_id].file);
										StrCpy(labeltxt, project->resource[s_id].file);
									}
									else if (s_type == TT_SOURCEFOLDER)
										StrCpy(project->sourcefld[s_id].name, labeltxt);
									else if (s_type == TT_HEADERFOLDER)
										StrCpy(project->headerfld[s_id].name, labeltxt);
									else if (s_type == TT_RESOURCEFOLDER)
										StrCpy(project->resourcefld[s_id].name, labeltxt);
									UpdateProject(hwnd);
									TreeView_SetItem(GetDlgItem(hwnd, IDC_FILES), &tvitem);
								}
							break;
						case TVN_BEGINDRAG:
							if (ThreadBusy())
								break;
							HIMAGELIST dragimg;
							ftree.ConvertParam(tvinfo->itemNew.lParam, s_type, s_id);
							if (s_type == TT_SOURCEFOLDER || s_type == TT_HEADERFOLDER || s_type == TT_RESOURCEFOLDER || s_type == TT_SOURCEFILE || s_type == TT_HEADERFILE || s_type == TT_RESOURCEFILE)
							{
								dragimg = TreeView_CreateDragImage(GetDlgItem(hwnd, IDC_FILES), tvinfo->itemNew.hItem);
								ImageList_BeginDrag(dragimg, 0, 0, 0);
								ImageList_DragEnter(GetDlgItem(hwnd, IDC_FILES), tvinfo->ptDrag.x, tvinfo->ptDrag.y);
								SetCapture(hwnd);
								tvdraginfo.itemtype = s_type;
								tvdraginfo.itemid = s_id;
								tvdraginfo.itemhandle = tvinfo->itemNew.hItem;
								tvdraginfo.active = true;
								if (tvdraginfo.itemtype == TT_SOURCEFILE || tvdraginfo.itemtype == TT_SOURCEFOLDER)
								{
									if (tvdraginfo.itemtype == TT_SOURCEFILE)
									{
										if (project->source[tvdraginfo.itemid].parent > 0)
											tvdraginfo.lasttarget = project->sourcefld[project->source[tvdraginfo.itemid].parent - 1].handle;
										else
											tvdraginfo.lasttarget = ftree.GetHandle(TT_SOURCEROOT);
									}
									else
									{
										if (project->sourcefld[tvdraginfo.itemid].parent > 0)
											tvdraginfo.lasttarget = project->sourcefld[project->sourcefld[tvdraginfo.itemid].parent - 1].handle;
										else
											tvdraginfo.lasttarget = ftree.GetHandle(TT_SOURCEROOT);
									}
								}
								else if (tvdraginfo.itemtype == TT_HEADERFILE || tvdraginfo.itemtype == TT_HEADERFOLDER)
								{
									if (tvdraginfo.itemtype == TT_HEADERFILE)
									{
										if (project->header[tvdraginfo.itemid].parent > 0)
											tvdraginfo.lasttarget = project->headerfld[project->header[tvdraginfo.itemid].parent - 1].handle;
										else
											tvdraginfo.lasttarget = ftree.GetHandle(TT_HEADERROOT);
									}
									else
									{
										if (project->headerfld[tvdraginfo.itemid].parent > 0)
											tvdraginfo.lasttarget = project->headerfld[project->headerfld[tvdraginfo.itemid].parent - 1].handle;
										else
											tvdraginfo.lasttarget = ftree.GetHandle(TT_HEADERROOT);
									}
								}
								else if (tvdraginfo.itemtype == TT_RESOURCEFILE || tvdraginfo.itemtype == TT_RESOURCEFOLDER)
								{
									if (tvdraginfo.itemtype == TT_RESOURCEFILE)
									{
										if (project->resource[tvdraginfo.itemid].parent > 0)
											tvdraginfo.lasttarget = project->resourcefld[project->resource[tvdraginfo.itemid].parent - 1].handle;
										else
											tvdraginfo.lasttarget = ftree.GetHandle(TT_RESOURCEROOT);
									}
									else
									{
										if (project->resourcefld[tvdraginfo.itemid].parent > 0)
											tvdraginfo.lasttarget = project->resourcefld[project->resourcefld[tvdraginfo.itemid].parent - 1].handle;
										else
											tvdraginfo.lasttarget = ftree.GetHandle(TT_RESOURCEROOT);
									}
								}
							}
							break;
						case NM_RCLICK:
							RECT windowrc;
							POINT cursorpos;
							TVHITTESTINFO hittest;
							HTREEITEM hittarget;
							GetWindowRect(GetDlgItem(hwnd, IDC_FILES), &windowrc);
							GetCursorPos(&cursorpos);
							cursorpos.x -= windowrc.left;
							cursorpos.y -= windowrc.top;
							hittest.pt.x = cursorpos.x;
							hittest.pt.y = cursorpos.y;
							hittarget = (HTREEITEM)SendMessage(GetDlgItem(hwnd, IDC_FILES), TVM_HITTEST, NULL, (LPARAM)&hittest);
							if (hittarget != NULL)
							{
								TreeView_Select(GetDlgItem(hwnd, IDC_FILES), hittarget, TVGN_CARET);
								RedrawWindow(GetDlgItem(hwnd, IDC_FILES), NULL, NULL, RDW_VALIDATE | RDW_UPDATENOW);
								ftree.GetHandleItem(hittarget, s_type, s_id);
								if (s_type == TT_SOURCEROOT || s_type == TT_SOURCEFOLDER || s_type == TT_HEADERROOT || s_type == TT_HEADERFOLDER || s_type == TT_RESOURCEROOT || s_type == TT_RESOURCEFOLDER || s_type == TT_SOURCEFILE || s_type == TT_HEADERFILE || s_type == TT_RESOURCEFILE || s_type == TT_ROOT)
								{
									POINT pnt;
									HMENU hmenu = LoadMenu(curinstance, MAKEINTRESOURCE(IDR_POPUPMENU));
									if (s_type == TT_SOURCEFOLDER || s_type == TT_HEADERFOLDER || s_type == TT_RESOURCEFOLDER)
										hmenu = GetSubMenu(hmenu, 0);
									else if (s_type == TT_SOURCEROOT || s_type == TT_HEADERROOT || s_type == TT_RESOURCEROOT)
										hmenu = GetSubMenu(hmenu, 1);
									else if (s_type == TT_SOURCEFILE || s_type == TT_HEADERFILE || s_type == TT_RESOURCEFILE)
										hmenu = GetSubMenu(hmenu, 2);
									else if (s_type == TT_ROOT)
										hmenu = GetSubMenu(hmenu, 3);
									tvdraginfo.itemtype = s_type;
									tvdraginfo.itemid = s_id;
									GetCursorPos(&pnt);
									TrackPopupMenu(hmenu, 0, pnt.x, pnt.y, 0, hwnd, NULL);
								}
							}
							break;
					}
					break;
			}
			break;
		case WM_MOUSEMOVE:
			if (tvdraginfo.active)
			{
				TVHITTESTINFO hittest;
				HTREEITEM hittarget;
				dragposition = MAKEPOINTS(lparam);
				ImageList_DragMove(dragposition.x - 14, dragposition.y - 44);
				ImageList_DragShowNolock(FALSE);
				hittest.pt.x = dragposition.x - 20;
				hittest.pt.y = dragposition.y - 36;
				hittarget = (HTREEITEM)SendMessage(GetDlgItem(hwnd, IDC_FILES), TVM_HITTEST, NULL, (LPARAM)&hittest);
				if (hittarget != NULL)
				{
					ftree.GetHandleItem(hittarget, s_type, s_id);
					if (tvdraginfo.itemtype == TT_SOURCEFILE || tvdraginfo.itemtype == TT_SOURCEFOLDER)
					{
						if (s_type == TT_SOURCEFILE)
						{
							if (project->source[s_id].parent > 0)
								hittarget = project->sourcefld[project->source[s_id].parent - 1].handle;
							else
								hittarget = ftree.GetHandle(TT_SOURCEROOT);
						}
						else if (s_type != TT_SOURCEFOLDER && s_type != TT_SOURCEROOT)
							hittarget = NULL;
						if (hittarget != NULL && tvdraginfo.itemtype == TT_SOURCEFOLDER)
						{
							ftree.GetHandleItem(hittarget, s_type, s_id);
							if (s_type == TT_SOURCEFOLDER)
								s_id++;
							while (s_id > 0)
							{
								if (s_id == tvdraginfo.itemid + 1)
								{
									if (project->sourcefld[tvdraginfo.itemid].parent > 0)
										hittarget = project->sourcefld[project->sourcefld[tvdraginfo.itemid].parent - 1].handle;
									else
										hittarget = ftree.GetHandle(TT_SOURCEROOT);
									break;
								}
								s_id = project->sourcefld[s_id - 1].parent;
							}
						}
					}
					else if (tvdraginfo.itemtype == TT_HEADERFILE || tvdraginfo.itemtype == TT_HEADERFOLDER)
					{
						if (s_type == TT_HEADERFILE)
						{
							if (project->header[s_id].parent > 0)
								hittarget = project->headerfld[project->header[s_id].parent - 1].handle;
							else
								hittarget = ftree.GetHandle(TT_HEADERROOT);
						}
						else if (s_type != TT_HEADERFOLDER && s_type != TT_HEADERROOT)
							hittarget = NULL;
						if (hittarget != NULL && tvdraginfo.itemtype == TT_HEADERFOLDER)
						{
							ftree.GetHandleItem(hittarget, s_type, s_id);
							if (s_type == TT_HEADERFOLDER)
								s_id++;
							while (s_id > 0)
							{
								if (s_id == tvdraginfo.itemid + 1)
								{
									if (project->headerfld[tvdraginfo.itemid].parent > 0)
										hittarget = project->headerfld[project->headerfld[tvdraginfo.itemid].parent - 1].handle;
									else
										hittarget = ftree.GetHandle(TT_HEADERROOT);
									break;
								}
								s_id = project->headerfld[s_id - 1].parent;
							}
						}
					}
					else if (tvdraginfo.itemtype == TT_RESOURCEFILE || tvdraginfo.itemtype == TT_RESOURCEFOLDER)
					{
						if (s_type == TT_RESOURCEFILE)
						{
							if (project->resource[s_id].parent > 0)
								hittarget = project->resourcefld[project->resource[s_id].parent - 1].handle;
							else
								hittarget = ftree.GetHandle(TT_RESOURCEROOT);
						}
						else if (s_type != TT_RESOURCEFOLDER && s_type != TT_RESOURCEROOT)
							hittarget = NULL;
						if (hittarget != NULL && tvdraginfo.itemtype == TT_RESOURCEFOLDER)
						{
							ftree.GetHandleItem(hittarget, s_type, s_id);
							if (s_type == TT_RESOURCEFOLDER)
								s_id++;
							while (s_id > 0)
							{
								if (s_id == tvdraginfo.itemid + 1)
								{
									if (project->resourcefld[tvdraginfo.itemid].parent > 0)
										hittarget = project->resourcefld[project->resourcefld[tvdraginfo.itemid].parent - 1].handle;
									else
										hittarget = ftree.GetHandle(TT_RESOURCEROOT);
									break;
								}
								s_id = project->resourcefld[s_id - 1].parent;
							}
						}
					}
					if (hittarget == NULL)
						hittarget = tvdraginfo.lasttarget;
					else
						tvdraginfo.lasttarget = hittarget;
					SendMessage(GetDlgItem(hwnd, IDC_FILES), TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)hittarget);
				}
				ImageList_DragShowNolock(TRUE);
			}
			break;
		case WM_TIMER:
			if (wparam == TMR_AUTOBUILD)
			{
				if (!ThreadBusy())
				{
					KillTimer(hwnd, wparam);
					if (autocmd.autobuild)
					{
						SendMessage(hwnd, WM_COMMAND, ID_BUILD_REBUILDALL, 0);
						autocmd.autobuild = false;
					}
					else if (autocmd.autosetup)
					{
						SendMessage(hwnd, WM_COMMAND, ID_BUILD_BUILDSETUP, 0);
						autocmd.autosetup = false;
					}
					if (autocmd.autobuild || autocmd.autosetup)
						SetTimer(hwnd, TMR_AUTOBUILD, 100, NULL);
					else
						SetTimer(hwnd, TMR_AUTOEXIT, 100, NULL);
				}
			}
			else if (wparam == TMR_AUTOEXECUTE)
			{
				if (!ThreadBusy())
				{
					KillTimer(hwnd, wparam);
					ShellExecute(hwnd, "open", toexecute, "", "", SW_SHOW);
				}
			}
			else if (wparam == TMR_AUTOEXIT)
			{
				if (!ThreadBusy())
				{
					KillTimer(hwnd, wparam);
					*setrval = project->buildreturn;
					EndDialog(hwnd, IDABORT);
				}
			}
			return(0);
		case WM_SIZE:
			ResizeWindows(hwnd);
			return(0);
		case WM_RBUTTONUP:
		case WM_LBUTTONUP:
			if (tvdraginfo.active)
			{
				HTREEITEM dropselected;
				ImageList_DragLeave(GetDlgItem(hwnd, IDC_FILES));
				ImageList_EndDrag();
				dropselected = (HTREEITEM)SendMessage(GetDlgItem(hwnd, IDC_FILES), TVM_GETNEXTITEM, TVGN_DROPHILITE, 0);
				ftree.GetHandleItem(dropselected, s_type, s_id);
				SendMessage(GetDlgItem(hwnd, IDC_FILES), TVM_SELECTITEM, TVGN_CARET, (LPARAM)dropselected);
				SendMessage(GetDlgItem(hwnd, IDC_FILES), TVM_SELECTITEM, TVGN_DROPHILITE, 0);
				ReleaseCapture();
				if (s_type == TT_SOURCEROOT || s_type == TT_HEADERROOT || s_type == TT_RESOURCEROOT)
					s_id = 0;
				else
					s_id++;
				if (tvdraginfo.itemtype == TT_SOURCEFILE)
					project->source[tvdraginfo.itemid].parent = (unsigned short)s_id;
				else if (tvdraginfo.itemtype == TT_HEADERFILE)
					project->header[tvdraginfo.itemid].parent = (unsigned short)s_id;
				else if (tvdraginfo.itemtype == TT_RESOURCEFILE)
					project->resource[tvdraginfo.itemid].parent = (unsigned short)s_id;
				else if (tvdraginfo.itemtype == TT_SOURCEFOLDER)
					project->sourcefld[tvdraginfo.itemid].parent = (unsigned short)s_id;
				else if (tvdraginfo.itemtype == TT_HEADERFOLDER)
					project->headerfld[tvdraginfo.itemid].parent = (unsigned short)s_id;
				else if (tvdraginfo.itemtype == TT_RESOURCEFOLDER)
					project->resourcefld[tvdraginfo.itemid].parent = (unsigned short)s_id;
				UpdateProject(hwnd);
				FillFileTree(hwnd);
				tvdraginfo.active = false;
			}
			break;
		case WM_INITDIALOG:
			hthread = NULL;
			hthreadexit = NULL;
			/*if (autocmd.autobuild)
			{
				ShowWindow(GetDlgItem(hwnd, IDC_FILES), SW_HIDE);
				EnableMenuItem(GetMenu(hwnd), GetMenuItemID(GetMenu(hwnd), 0), MF_GRAYED);
				//EnableMenu(
				//EnableWindow(GetDlgItem(hwnd, IDC_FILES), FALSE);
			}*/
			compiler.AppendOutput(GetDlgItem(hwnd, IDC_BUILDOUTPUT), "Starting EDGELIB Builder...\r\n");
			if (!LoadSettings(hwnd))
			{
				MessageBox(hwnd, "Couldn't load EdgeBuilder.ini, closing application", "EDGELIB Builder", MB_OK | MB_ICONERROR);
				EndDialog(hwnd, IDABORT);
				return(TRUE);
			}
			GetGuiInfo(hwnd);
			compiler.SearchPlatforms(applicationpath, GetDlgItem(hwnd, IDC_PLATFORM), GetDlgItem(hwnd, IDC_BUILDOUTPUT), autocmd.useplatform);
			if (!StrEqual(commandline, ""))
				OpenProject(hwnd, commandline);
			ftree.SetHwnd(GetDlgItem(hwnd, IDC_FILES));
			ftree.InitIcons(curinstance);
			if (this->project->loaded)
			{
				FillFileTree(hwnd);
				SetCaption(hwnd, project->name);
			}
			else
				SetCaption(hwnd, NULL);
			tvdraginfo.active = false;
			if (autocmd.autobuild || autocmd.autosetup)
				SetTimer(hwnd, TMR_AUTOBUILD, 100, NULL);
				//PostMessage(hwnd, WM_USER_AUTOBUILD, 0, 0);
			//{
			//	compiler.StartOutput(GetDlgItem(hwnd, IDC_BUILDOUTPUT));
			//	compiler.BuildProject(&env, &project, (unsigned short)SendMessage(GetDlgItem(hwnd, IDC_PLATFORM), CB_GETCURSEL, 0, 0));
			//}
			return(TRUE);
		case WM_CLOSE:
			if (ThreadBusy())
			{
				MessageBox(hwnd, "Can't close the program because the EDGELIB Builder is busy building", "Build in progress...", MB_OK);
				return(TRUE);
			}
			result = IDYES;
			if (project->loaded && project->updated)
			{
				result = MessageBox(hwnd, "Save project?", "EDGELIB Builder", MB_YESNOCANCEL | MB_ICONEXCLAMATION);
				if (result == IDYES)
					SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVEPROJECT, 0);
			}
			if (result != IDCANCEL)
				EndDialog(hwnd, IDABORT);
			return(TRUE);
	}
	return(FALSE);
}

//Message handler for the project properties dialog
BOOL CALLBACK ClassMain::ProjectPropsWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	long itemselectcount;
	unsigned long ctr, tmp_uid;
	char tmp_str[32];
	int itemselect[256];
	switch(wmsg)
	{
		case WM_COMMAND:
			switch(LOWORD(wparam))
			{
				case IDCAPABILITIES:
					DialogBox(curinstance, MAKEINTRESOURCE(IDD_CAPABILITIES), hwnd, ProjectCapsWinProcWrapper);
					return(TRUE);
				case IDDEPENDENCIES:
					DialogBox(curinstance, MAKEINTRESOURCE(IDD_DEPENDENCIES), hwnd, ProjectDepsWinProcWrapper);
					return(TRUE);
				case IDOK:
					itemselectcount = SendMessage(GetDlgItem(hwnd, IDC_SELECTPLATFORM), LB_GETSELITEMS, 256, (LPARAM)itemselect);
					for (ctr = 0; ctr < MAX_PLATFORMS; ctr++)
						project->selectplatform[ctr] = false;
					if (itemselectcount > 0)
						for (ctr = 0; ctr < (unsigned long)itemselectcount; ctr++)
							project->selectplatform[itemselect[ctr]] = true;
					GetWindowText(GetDlgItem(hwnd, IDC_PRJNAME), project->name, MAX_ESTR);
					GetWindowText(GetDlgItem(hwnd, IDC_PRJCAPTION), project->caption, MAX_ESTR);
					GetWindowText(GetDlgItem(hwnd, IDC_PRJVENDOR), project->vendor, MAX_ESTR);
					GetWindowText(GetDlgItem(hwnd, IDC_PRJVENDORSHORT), project->vendorshort, MAX_ESTR);
					GetWindowText(GetDlgItem(hwnd, IDC_PRJDOMAIN), project->domain, MAX_ESTR);
					GetWindowText(GetDlgItem(hwnd, IDC_PRJUID), tmp_str, 32);
					GetWindowText(GetDlgItem(hwnd, IDC_PATHOVERRIDE), project->pathoverride, MAX_ESTR);
					GetWindowText(GetDlgItem(hwnd, IDC_VERMAJOR), project->vermajor, 8);
					GetWindowText(GetDlgItem(hwnd, IDC_VERMINOR), project->verminor, 8);
					GetWindowText(GetDlgItem(hwnd, IDC_BUILDNR), project->buildnr, 16);
					if (tmp_str[1] == 'x' || tmp_str[1] == 'X')
						project->appuid = ClassEStd::StrToInt(&tmp_str[2], 16);
					else
						project->appuid = ClassEStd::StrToInt(tmp_str, 16);
					project->targettype = (unsigned char)SendMessage(GetDlgItem(hwnd, IDC_TARGETTYPE), CB_GETCURSEL, 0, 0);
					if (IsDlgButtonChecked(hwnd, IDC_NORESEMU))
						project->noresemu = true;
					else
						project->noresemu = false;
					if (IsDlgButtonChecked(hwnd, IDC_CUSTOMPATH))
						project->dopathoverride = true;
					else
						project->dopathoverride = false;
					StrReplace(project->vermajor, ".", "-");
					StrReplace(project->verminor, ".", "-");
					StrReplace(project->buildnr, ".", "-");
					StrReplace(project->vermajor, ",", "-");
					StrReplace(project->verminor, ",", "-");
					StrReplace(project->buildnr, ",", "-");
					SwitchEdgePath();

				case IDCANCEL:
					EndDialog(hwnd, wparam);
					return(TRUE);
			}
			break;
		case WM_INITDIALOG:
			for (ctr = 0; ctr < compiler.numplatforms; ctr++)
			{
				SendMessage(GetDlgItem(hwnd, IDC_SELECTPLATFORM), LB_ADDSTRING, 0, (LPARAM)compiler.platform[ctr].name);
				if (project->selectplatform[ctr])
					SendMessage(GetDlgItem(hwnd, IDC_SELECTPLATFORM), LB_SETSEL, TRUE, ctr);
			}
			SendMessage(GetDlgItem(hwnd, IDC_TARGETTYPE), CB_ADDSTRING, 0, (LPARAM)"Executable");
			SendMessage(GetDlgItem(hwnd, IDC_TARGETTYPE), CB_ADDSTRING, 0, (LPARAM)"Static library");
			SendMessage(GetDlgItem(hwnd, IDC_TARGETTYPE), CB_SETCURSEL, project->targettype, 0);
			tmp_uid = project->appuid;
			SetWindowText(GetDlgItem(hwnd, IDC_PRJNAME), project->name);
			SetWindowText(GetDlgItem(hwnd, IDC_PRJCAPTION), project->caption);
			SetWindowText(GetDlgItem(hwnd, IDC_PRJVENDOR), project->vendor);
			SetWindowText(GetDlgItem(hwnd, IDC_PRJVENDORSHORT), project->vendorshort);
			SetWindowText(GetDlgItem(hwnd, IDC_PRJDOMAIN), project->domain);
			SetWindowText(GetDlgItem(hwnd, IDC_PATHOVERRIDE), project->pathoverride);
			ClassEStd::StrCpy(tmp_str, "0x00000000");
			tmp_uid = project->appuid;
			for (ctr = 0; ctr < 8; ctr++)
			{
				if (tmp_uid % 16 > 9)
					tmp_str[9 - ctr] = 'a' + (unsigned char)(tmp_uid % 16) - 10;
				else
					tmp_str[9 - ctr] = '0' + (unsigned char)(tmp_uid % 16);
				tmp_uid /= 16;
				if (tmp_uid == 0)
					break;
			}
			SetWindowText(GetDlgItem(hwnd, IDC_PRJUID), tmp_str);
			SetWindowText(GetDlgItem(hwnd, IDC_VERMAJOR), project->vermajor);
			SetWindowText(GetDlgItem(hwnd, IDC_VERMINOR), project->verminor);
			SetWindowText(GetDlgItem(hwnd, IDC_BUILDNR), project->buildnr);
			if (project->noresemu)
				CheckDlgButton(hwnd, IDC_NORESEMU, BST_CHECKED);
			if (project->dopathoverride)
				CheckDlgButton(hwnd, IDC_CUSTOMPATH, BST_CHECKED);	
			return(TRUE);
		case WM_CLOSE:
			EndDialog(hwnd, IDABORT);
			return(TRUE);
	}
	return(FALSE);
}

//Message handler for the project capabilities dialog
BOOL CALLBACK ClassMain::ProjectCapsWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	unsigned char ctr;
	switch(wmsg)
	{
		case WM_COMMAND:
			switch(LOWORD(wparam))
			{
				case IDOK:
					for (ctr = 0; ctr < 3; ctr++)
						project->capabilities[ctr] = 0;
					if (IsDlgButtonChecked(hwnd, IDC_CAPB1)) project->capabilities[0] |= 1;
					if (IsDlgButtonChecked(hwnd, IDC_CAPB2)) project->capabilities[0] |= 2;
					if (IsDlgButtonChecked(hwnd, IDC_CAPB3)) project->capabilities[0] |= 4;
					if (IsDlgButtonChecked(hwnd, IDC_CAPB4)) project->capabilities[0] |= 8;
					if (IsDlgButtonChecked(hwnd, IDC_CAPB5)) project->capabilities[0] |= 16;
					if (IsDlgButtonChecked(hwnd, IDC_CAPA1)) project->capabilities[1] |= 1;
					if (IsDlgButtonChecked(hwnd, IDC_CAPA2)) project->capabilities[1] |= 2;
					if (IsDlgButtonChecked(hwnd, IDC_CAPA3)) project->capabilities[1] |= 4;
					if (IsDlgButtonChecked(hwnd, IDC_CAPA4)) project->capabilities[1] |= 8;
					if (IsDlgButtonChecked(hwnd, IDC_CAPA5)) project->capabilities[1] |= 16;
					if (IsDlgButtonChecked(hwnd, IDC_CAPA6)) project->capabilities[1] |= 32;
					if (IsDlgButtonChecked(hwnd, IDC_CAPA7)) project->capabilities[1] |= 64;
					if (IsDlgButtonChecked(hwnd, IDC_CAPA8)) project->capabilities[1] |= 128;
					if (IsDlgButtonChecked(hwnd, IDC_CAPM1)) project->capabilities[2] |= 1;
					if (IsDlgButtonChecked(hwnd, IDC_CAPM2)) project->capabilities[2] |= 2;
					if (IsDlgButtonChecked(hwnd, IDC_CAPM3)) project->capabilities[2] |= 4;
					if (IsDlgButtonChecked(hwnd, IDC_CAPM4)) project->capabilities[2] |= 8;
					if (IsDlgButtonChecked(hwnd, IDC_CAPM5)) project->capabilities[2] |= 16;
					if (IsDlgButtonChecked(hwnd, IDC_CAPM6)) project->capabilities[2] |= 32;
					if (IsDlgButtonChecked(hwnd, IDC_CAPM7)) project->capabilities[2] |= 64;
				case IDCANCEL:
					EndDialog(hwnd, IDABORT);
					return(TRUE);
			}
			break;
		case WM_INITDIALOG:
			if (project->capabilities[0] &  1) CheckDlgButton(hwnd, IDC_CAPB1, BST_CHECKED);
			if (project->capabilities[0] &  2) CheckDlgButton(hwnd, IDC_CAPB2, BST_CHECKED);
			if (project->capabilities[0] &  4) CheckDlgButton(hwnd, IDC_CAPB3, BST_CHECKED);
			if (project->capabilities[0] &  8) CheckDlgButton(hwnd, IDC_CAPB4, BST_CHECKED);
			if (project->capabilities[0] & 16) CheckDlgButton(hwnd, IDC_CAPB5, BST_CHECKED);
			if (project->capabilities[1] &  1) CheckDlgButton(hwnd, IDC_CAPA1, BST_CHECKED);
			if (project->capabilities[1] &  2) CheckDlgButton(hwnd, IDC_CAPA2, BST_CHECKED);
			if (project->capabilities[1] &  4) CheckDlgButton(hwnd, IDC_CAPA3, BST_CHECKED);
			if (project->capabilities[1] &  8) CheckDlgButton(hwnd, IDC_CAPA4, BST_CHECKED);
			if (project->capabilities[1] & 16) CheckDlgButton(hwnd, IDC_CAPA5, BST_CHECKED);
			if (project->capabilities[1] & 32) CheckDlgButton(hwnd, IDC_CAPA6, BST_CHECKED);
			if (project->capabilities[1] & 64) CheckDlgButton(hwnd, IDC_CAPA7, BST_CHECKED);
			if (project->capabilities[1] &128) CheckDlgButton(hwnd, IDC_CAPA8, BST_CHECKED);
			if (project->capabilities[2] &  1) CheckDlgButton(hwnd, IDC_CAPM1, BST_CHECKED);
			if (project->capabilities[2] &  2) CheckDlgButton(hwnd, IDC_CAPM2, BST_CHECKED);
			if (project->capabilities[2] &  4) CheckDlgButton(hwnd, IDC_CAPM3, BST_CHECKED);
			if (project->capabilities[2] &  8) CheckDlgButton(hwnd, IDC_CAPM4, BST_CHECKED);
			if (project->capabilities[2] & 16) CheckDlgButton(hwnd, IDC_CAPM5, BST_CHECKED);
			if (project->capabilities[2] & 32) CheckDlgButton(hwnd, IDC_CAPM6, BST_CHECKED);
			if (project->capabilities[2] & 64) CheckDlgButton(hwnd, IDC_CAPM7, BST_CHECKED);
			return(TRUE);
		case WM_CLOSE:
			EndDialog(hwnd, IDABORT);
			return(TRUE);
	}
	return(FALSE);
}

//Message handler for the project dependencies dialog
BOOL CALLBACK ClassMain::ProjectDepsWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	int depstdarr[] = { IDC_DEPSTD1, IDC_DEPSTD2, IDC_DEPSTD3 };
	int deparr[] = { IDC_DEP1, IDC_DEP2, IDC_DEP3 };
	unsigned char ctr;
	switch(wmsg)
	{
		case WM_COMMAND:
			switch(LOWORD(wparam))
			{
				case IDOK:
					project->fwdependency = 0;
					for (ctr = 0; ctr < 3; ctr++)
						if (IsDlgButtonChecked(hwnd, deparr[ctr]))
							project->fwdependency |= (1 << ctr);
				case IDCANCEL:
					EndDialog(hwnd, IDABORT);
					return(TRUE);
			}
			break;
		case WM_INITDIALOG:
			for (ctr = 0; ctr < 3; ctr++)
				CheckDlgButton(hwnd, depstdarr[ctr], BST_CHECKED);
			for (ctr = 0; ctr < 3; ctr++)
				if (project->fwdependency & (1 << ctr))
					CheckDlgButton(hwnd, deparr[ctr], BST_CHECKED);
			return(TRUE);
		case WM_CLOSE:
			EndDialog(hwnd, IDABORT);
			return(TRUE);
	}
	return(FALSE);
}

//Message handler for the project library/include paths dialog
BOOL CALLBACK ClassMain::ProjectPathsWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	char searchstr[64];
	unsigned char pathtype;
	long index, numkeys;
	switch(wmsg)
	{
		case WM_COMMAND:
			if (LOWORD(wparam) == IDMOVEUP || LOWORD(wparam) == IDMOVEDOWN || LOWORD(wparam) == IDCHANGE || LOWORD(wparam) == IDREMOVE || LOWORD(wparam) == IDADD)
			{
				pathtype = (unsigned char)SendMessage(GetDlgItem(hwnd, IDC_PATHTYPE), CB_GETCURSEL, 0, 0);
				if (pathtype == 0)
					StrCpy(searchstr, "incpath");
				else
					StrCpy(searchstr, "libpath");
				numkeys = project->env->GetKeyCount(searchstr, false);
				if (LOWORD(wparam) == IDADD)
				{
					index = project->env->AddString(searchstr, "");
					if (index == -1)
						index = LB_ERR;
					else
						index = numkeys;
				}
				else
					index = SendMessage(GetDlgItem(hwnd, IDC_PATHLIST), LB_GETCURSEL, 0, 0);
			}
			switch(LOWORD(wparam))
			{
				case IDMOVEUP:
					if (index != LB_ERR && index > 0)
					{
						lastpathtype = 0xFF;
						project->env->OTSwapKeyValues(searchstr, index, index - 1, false);
						FillPathList(GetDlgItem(hwnd, IDC_PATHLIST), (unsigned char)SendMessage(GetDlgItem(hwnd, IDC_PATHTYPE), CB_GETCURSEL, 0, 0), index - 1);
					}
					return(TRUE);
				case IDMOVEDOWN:
					if (index != LB_ERR && index < numkeys - 1)
					{
						lastpathtype = 0xFF;
						project->env->OTSwapKeyValues(searchstr, index, index + 1, false);
						FillPathList(GetDlgItem(hwnd, IDC_PATHLIST), (unsigned char)SendMessage(GetDlgItem(hwnd, IDC_PATHTYPE), CB_GETCURSEL, 0, 0), index + 1);
					}
					return(TRUE);
				case IDCHANGE:
				case IDADD:
					if (index != LB_ERR)
					{
						editindex = index;
						if (DialogBox(curinstance, MAKEINTRESOURCE(IDD_EDITPATH), hwnd, EditPathWinProcWrapper) == IDOK)
						{
							lastpathtype = 0xFF;
							FillPathList(GetDlgItem(hwnd, IDC_PATHLIST), (unsigned char)SendMessage(GetDlgItem(hwnd, IDC_PATHTYPE), CB_GETCURSEL, 0, 0), index);
						}
						else if (LOWORD(wparam) == IDADD)
							project->env->OTRemoveKey(searchstr, index, true, false);
					}
					return(TRUE);
				case IDREMOVE:
					if (index != LB_ERR)
					{
						project->env->OTRemoveKey(searchstr, index, true, false);
						lastpathtype = 0xFF;
						FillPathList(GetDlgItem(hwnd, IDC_PATHLIST), (unsigned char)SendMessage(GetDlgItem(hwnd, IDC_PATHTYPE), CB_GETCURSEL, 0, 0), index);
					}
					return(TRUE);
				case IDCLOSE:
					SendMessage(hwnd, WM_CLOSE, 0, 0);
					return(TRUE);
			}
			switch(HIWORD(wparam))
			{
				case CBN_SELCHANGE:
					FillPathList(GetDlgItem(hwnd, IDC_PATHLIST), (unsigned char)SendMessage(GetDlgItem(hwnd, IDC_PATHTYPE), CB_GETCURSEL, 0, 0));
					return(TRUE);
			}
			break;
		case WM_INITDIALOG:
			lastpathtype = 1;
			SendMessage(GetDlgItem(hwnd, IDC_PATHTYPE), CB_ADDSTRING, 0, (LPARAM)"Include paths");
			SendMessage(GetDlgItem(hwnd, IDC_PATHTYPE), CB_ADDSTRING, 0, (LPARAM)"Library paths");
			SendMessage(GetDlgItem(hwnd, IDC_PATHTYPE), CB_SETCURSEL, 0, 0);
			SendMessage(hwnd, WM_COMMAND, CBN_SELCHANGE << 16, 0);
			return(TRUE);
		case WM_CLOSE:
			EndDialog(hwnd, IDABORT);
			return(TRUE);
	}
	return(FALSE);
}

//Message handler for path edit and browser
BOOL CALLBACK ClassMain::EditPathWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	char searchstr[64];
	if (lastpathtype == 0)
		StrCpy(searchstr, "incpath");
	else
		StrCpy(searchstr, "libpath");
	switch(wmsg)
	{
		case WM_COMMAND:
			switch(wparam)
			{
				char newvalue[MAX_EFILESTR];
				case IDOK:
					GetWindowText(GetDlgItem(hwnd, IDC_PATHEDIT), newvalue, MAX_EFILESTR);
					if (StrEqual(newvalue, ""))
					{
						SendMessage(hwnd, WM_COMMAND, IDCANCEL, 0);
						return(TRUE);
					}
					while(ClassEStd::StrLen(newvalue) > 0)
					{
						if (newvalue[ClassEStd::StrLen(newvalue) - 1] == '\\' || newvalue[ClassEStd::StrLen(newvalue) - 1] == '/')
							newvalue[ClassEStd::StrLen(newvalue) - 1] = 0;
						else
							break;
					}
					project->env->OTAdjustKeyValue(searchstr, editindex, newvalue, false);
					EndDialog(hwnd, IDOK);
					return(TRUE);
				case IDCANCEL:
					EndDialog(hwnd, IDABORT);
					return(TRUE);
				case IDBROWSE:
					if (SelectPath(hwnd, "Choose new path", newvalue))
						SetWindowText(GetDlgItem(hwnd, IDC_PATHEDIT), newvalue);
					return(TRUE);
			}
			break;
		case WM_INITDIALOG:
			SetWindowText(GetDlgItem(hwnd, IDC_PATHEDIT), project->env->OTGetKeyValue(searchstr, editindex, false));
			SetFocus(GetDlgItem(hwnd, IDC_PATHEDIT));
			return(FALSE);
		case WM_CLOSE:
			EndDialog(hwnd, IDABORT);
			return(TRUE);
	}
	return(FALSE);
}

//Message handler for EDGELIB Builder options dialog
BOOL CALLBACK ClassMain::PrgOptionsWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	switch(wmsg)
	{
		case WM_COMMAND:
			switch(wparam)
			{
				char tmpstr[MAX_ESTR];
				case IDBROWSE:
					GetWindowText(GetDlgItem(hwnd, IDC_EDGEPATH), tmpstr, MAX_ESTR);
					SelectPath(hwnd, "Edge installation path", tmpstr, tmpstr);
					SetWindowText(GetDlgItem(hwnd, IDC_EDGEPATH), tmpstr);
					return(TRUE);
				case IDOK:
					GetWindowText(GetDlgItem(hwnd, IDC_EDGEPATH), tmpstr, MAX_ESTR);
					while(env.OTRemoveKey("%defedgepath%", 0));
					env.AddString("defedgepath", tmpstr);
					while(env.OTRemoveKey("%edgepath%", 0));
					env.AddString("edgepath", tmpstr);
					GetWindowText(GetDlgItem(hwnd, IDC_TMPFILE), tmpstr, MAX_ESTR);
					while(env.OTRemoveKey("%bldtmpfile%", 0));
					env.AddString("bldtmpfile", tmpstr);
					GetWindowText(GetDlgItem(hwnd, IDC_VENDOR), settings.prjnewvendor, MAX_ESTR);
					GetWindowText(GetDlgItem(hwnd, IDC_VENDORSHORT), settings.prjnewvendorshort, MAX_ESTR);
					if (IsDlgButtonChecked(hwnd, IDC_BUILDBATCH))
						settings.buildbatch = true;
					else
						settings.buildbatch = false;
					CheckSettings();
					SaveSettings();
					EndDialog(hwnd, IDOK);
					return(TRUE);
				case IDCANCEL:
					EndDialog(hwnd, IDCANCEL);
					return(TRUE);
			}
			break;
		case WM_INITDIALOG:
			CheckSettings();
			SetWindowText(GetDlgItem(hwnd, IDC_VENDOR), settings.prjnewvendor);
			SetWindowText(GetDlgItem(hwnd, IDC_VENDORSHORT), settings.prjnewvendorshort);
			SetWindowText(GetDlgItem(hwnd, IDC_EDGEPATH), env.GetString("%defedgepath%", true, true));
			SetWindowText(GetDlgItem(hwnd, IDC_TMPFILE), env.GetString("%bldtmpfile%", true, true));
			if (settings.buildbatch)
				CheckDlgButton(hwnd, IDC_BUILDBATCH, BST_CHECKED);
			break;
		case WM_CLOSE:
			EndDialog(hwnd, IDABORT);
			return(TRUE);
	}
	return(FALSE);
}

/////////////////////////////////////////////////////////////////////
// Window procedure wrappers
/////////////////////////////////////////////////////////////////////

BOOL CALLBACK MainWinProcWrapper(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam){ return(main.MainWinProc(hwnd, wmsg, wparam, lparam)); }
BOOL CALLBACK ProjectPropsWinProcWrapper(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam){ return(main.ProjectPropsWinProc(hwnd, wmsg, wparam, lparam)); }
BOOL CALLBACK ProjectCapsWinProcWrapper(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam){ return(main.ProjectCapsWinProc(hwnd, wmsg, wparam, lparam)); }
BOOL CALLBACK ProjectDepsWinProcWrapper(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam){ return(main.ProjectDepsWinProc(hwnd, wmsg, wparam, lparam)); }
BOOL CALLBACK ProjectPathsWinProcWrapper(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam){ return(main.ProjectPathsWinProc(hwnd, wmsg, wparam, lparam)); }
BOOL CALLBACK EditPathWinProcWrapper(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam){ return(main.EditPathWinProc(hwnd, wmsg, wparam, lparam)); }
BOOL CALLBACK PrgOptionsWinProcWrapper(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam){ return(main.PrgOptionsWinProc(hwnd, wmsg, wparam, lparam)); }
