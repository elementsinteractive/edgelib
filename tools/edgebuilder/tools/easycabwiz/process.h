// process.h:
// Class for processing and converting INI files
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Limits
#define MAX_FILEGROUPS  64
#define MAX_FILELIST   256

//Group structure
typedef struct
{
	char srcdir[MAX_EFILESTR];
	char destdir[MAX_EFILESTR];
	char sectname[32];
}COPYGROUP;

class CabProcess
{
	private:
		COPYGROUP group[MAX_FILEGROUPS];
		char addname[MAX_FILELIST][MAX_EFILESTR];
		char vendor[MAX_ESTR];
		char binname[MAX_ESTR];
		char caption[MAX_ESTR];
		char shortcut[MAX_ESTR];
		char curpath[MAX_EFILESTR];
		unsigned long nameassign[MAX_FILELIST];
		unsigned long buildmax;
		unsigned long filecount;
		unsigned long groupcount;
		void SplitDesc(char *copydesc, char *srcfile, char *dstfile);
		void HandleAddFile(char *copydesc);
		bool WriteInf(char *tmpfile);
		bool StartCabWiz(char *cabwizexe, char *tmpfile, char *outputfile);
	public:
		CabProcess(void);
		~CabProcess(void);
		bool Start(char *cabwizexe, char *inputfile, char *outputfile);
};

/////////////////////////////////////////////////////////////////////
// File write helper functions
/////////////////////////////////////////////////////////////////////

void prg_fwrite(ClassEFile *ef, char *str, bool nl = true)
{
	ef->Write(str, ClassEStd::StrLen(str));
	if (nl)
		ef->Write("\r\n", 2);
}

void prg_fwrite(ClassEFile *ef, unsigned long v, unsigned char base = 10, bool nl = true)
{
	char str[64];
	if (base == 16)
		ClassEStd::StrCpy(str, "0x");
	else
		ClassEStd::StrCpy(str, "");
	ClassEStd::IntToStr(&str[ClassEStd::StrLen(str)], v, base, false);
	ef->Write(str, ClassEStd::StrLen(str));
	if (nl)
		ef->Write("\r\n", 2);
}

/////////////////////////////////////////////////////////////////////
// CabProcess: Private
/////////////////////////////////////////////////////////////////////

//Split addfile description to source and destination
void CabProcess::SplitDesc(char *copydesc, char *srcfile, char *dstfile)
{
	bool onquote = true, onsrc = true;
	unsigned long ctr, onpos = 0, len = ClassEStd::StrLen(copydesc);
	srcfile[0] = 0;
	dstfile[0] = 0;
	for (ctr = 0; ctr < len; ctr++)
	{
		if (copydesc[ctr] == '"')
		{
			if (ctr != 0)
				onquote = !onquote;
		}
		else if (copydesc[ctr] == '-' && !onquote)
		{
			onsrc = false;
			onpos = 0;
		}
		else
		{
			if (onsrc)
			{
				srcfile[onpos] = copydesc[ctr];
				srcfile[onpos + 1] = 0;
			}
			else
			{
				dstfile[onpos] = copydesc[ctr];
				dstfile[onpos + 1] = 0;
			}
			onpos++;
		}
	}
	ClassEStd::StrTrim(srcfile);
	ClassEStd::StrTrim(dstfile);
}

//Handle file groups and list when adding files
void CabProcess::HandleAddFile(char *copydesc)
{
	bool groupfound = false;
	unsigned long ctr;
	char srcfile[MAX_EFILESTR], dstfile[MAX_EFILESTR], srcpath[MAX_EFILESTR], dstpath[MAX_EFILESTR];
	SplitDesc(copydesc, srcfile, dstfile);
	ClassEStd::StrTrim(srcfile);
	ClassEStd::StrTrim(dstfile);
	ClassEStd::StrCpy(srcpath, srcfile);
	ClassEStd::StrCpy(dstpath, dstfile);
	ClassEFile::GetFolderName(srcpath);
	ClassEFile::GetFolderName(dstpath);
	ClassEFile::GetFileName(srcfile);
	ClassEFile::GetFileName(dstfile);
	for (ctr = 0; ctr < groupcount; ctr++)
		if (ClassEStd::StrEqual(group[ctr].srcdir, srcpath, false) && ClassEStd::StrEqual(group[ctr].destdir, dstpath, false))
		{
			ClassEStd::StrCpy(group[ctr].srcdir, srcpath);
			ClassEStd::StrCpy(group[ctr].destdir, dstpath);
			nameassign[filecount] = ctr;
			groupfound = true;
			break;
		}
	if (!groupfound)
	{
		ClassEStd::StrCpy(group[groupcount].srcdir, srcpath);
		ClassEStd::StrCpy(group[groupcount].destdir, dstpath);
		ClassEStd::StrCpy(group[groupcount].sectname, "xxFiles");
		group[groupcount].sectname[0] = (unsigned char)('A' + groupcount % 26);
		group[groupcount].sectname[1] = (unsigned char)('a' + groupcount / 26 % 26);
		nameassign[filecount] = groupcount;
		groupcount++;
	}
	ClassEStd::StrCpy(addname[filecount], srcfile);
	filecount++;
}

//Write file to process through CabWiz
bool CabProcess::WriteInf(char *tmpfile)
{
	ClassEFile ef;
	unsigned long ctr;
	ClassEStd::StrCpy(tmpfile, curpath);
	ClassEStd::StrCat(tmpfile, "\\cabwiz_tmp");
	ClassEStd::StrCat(tmpfile, ClassERTimer::Count());
	ClassEStd::StrCat(tmpfile, ".inf");
	ef.SetCurrentFolder(curpath);
	if (ef.New(tmpfile))
	{
		//Version section
		prg_fwrite(&ef, "[Version]");
		prg_fwrite(&ef, "Signature   = \"$Windows NT$\"");
		prg_fwrite(&ef, "Provider    = \"", false);
		prg_fwrite(&ef, vendor, false);
		prg_fwrite(&ef, "\"");
		prg_fwrite(&ef, "CESignature = \"$Windows CE$\"");

		//Device section
		if (buildmax)
		{
			prg_fwrite(&ef, "\r\n[CEDevice]");
			prg_fwrite(&ef, "BuildMax    = ", false);
			prg_fwrite(&ef, buildmax, 16);
		}

		//Strings section
		prg_fwrite(&ef, "\r\n[CEStrings]");
		prg_fwrite(&ef, "AppName     = \"", false);
		prg_fwrite(&ef, binname, false);
		prg_fwrite(&ef, "\"");
		prg_fwrite(&ef, "InstallDir  = %CE1%\\%AppName%");

		//Default install section
		prg_fwrite(&ef, "\r\n[DefaultInstall]");
		prg_fwrite(&ef, "CopyFiles   = ", false);
		for (ctr = 0; ctr < groupcount; ctr++)
		{
			if (ctr > 0)
				prg_fwrite(&ef, ", ", false);
			prg_fwrite(&ef, group[ctr].sectname, false);
		}
		prg_fwrite(&ef, "");
		prg_fwrite(&ef, "CEShortcuts = Startmenu");

		//Source Disk Name section
		prg_fwrite(&ef, "\r\n[SourceDisksNames]");
		for (ctr = 0; ctr < groupcount; ctr++)
		{
			ClassEFile tmp_path_gen;
			char tpath_noslash[MAX_EFILESTR];
			prg_fwrite(&ef, ctr + 1, 10, false);
			prg_fwrite(&ef, " = ,\"", false);
			prg_fwrite(&ef, group[ctr].sectname, false);
			prg_fwrite(&ef, "\",,\"", false);
			tmp_path_gen.SetCurrentFolder(curpath);
			if (ClassEStd::StrLen(group[ctr].srcdir))
				tmp_path_gen.SetCurrentFolder(group[ctr].srcdir);
			
			ClassEStd::StrCpy(tpath_noslash, tmp_path_gen.GetCurrentFolderC());
			tpath_noslash[ClassEStd::StrLen(tpath_noslash) - 1] = 0;
			prg_fwrite(&ef, tpath_noslash, false);
			prg_fwrite(&ef, "\"");
		}

		//Source Disk File section
		prg_fwrite(&ef, "\r\n[SourceDisksFiles]");
		for (ctr = 0; ctr < filecount; ctr++)
		{
			prg_fwrite(&ef, "\"", false);
			prg_fwrite(&ef, addname[ctr], false);
			prg_fwrite(&ef, "\" = ", false);
			prg_fwrite(&ef, nameassign[ctr] + 1);
		}

		//Destination Dir section
		prg_fwrite(&ef, "\r\n[DestinationDirs]");
		for (ctr = 0; ctr < groupcount; ctr++)
		{
			prg_fwrite(&ef, group[ctr].sectname, false);
			prg_fwrite(&ef, " = 0, \"%InstallDir%", false);
			if (ClassEStd::StrLen(group[ctr].destdir))
			{
				prg_fwrite(&ef, "\\", false);
				group[ctr].destdir[ClassEStd::StrLen(group[ctr].destdir) - 1] = 0;
				prg_fwrite(&ef, group[ctr].destdir, false);
			}
			prg_fwrite(&ef, "\"");
		}
		prg_fwrite(&ef, "Startmenu = 0, \"", false);
		prg_fwrite(&ef, shortcut, false);
		prg_fwrite(&ef, "\"");
		prg_fwrite(&ef, "DefaultDestDir = 0, \"%InstallDir%\"");

		//List groups and files
		for (ctr = 0; ctr < groupcount; ctr++)
		{
			unsigned long fctr;
			prg_fwrite(&ef, "\r\n[", false);
			prg_fwrite(&ef, group[ctr].sectname, false);
			prg_fwrite(&ef, "]");
			for (fctr = 0; fctr < filecount; fctr++)
				if (nameassign[fctr] == ctr)
				{
					prg_fwrite(&ef, "\"", false);
					prg_fwrite(&ef, addname[fctr], false);
					prg_fwrite(&ef, "\"");
				}
		}

		//Startmenu section
		prg_fwrite(&ef, "\r\n[Startmenu]");
		prg_fwrite(&ef, "\"", false);
		prg_fwrite(&ef, caption, false);
		prg_fwrite(&ef, "\", 0, \"", false);
		prg_fwrite(&ef, binname, false);
		prg_fwrite(&ef, ".exe\"");
		ef.Close();
	}
	else
		return(false);
	return(true);
}

//Start CabWiz program
bool CabProcess::StartCabWiz(char *cabwizexe, char *tmpfile, char *outputfile)
{
	bool success = false;
	char wizpath[MAX_EFILESTR], wizcmd[MAX_CMDLINE];
	ClassEStd::StrCpy(wizpath, cabwizexe);
	ClassEFile::GetFolderName(wizpath);
	_chdir(wizpath);
	ClassEStd::StrCpy(wizcmd, cabwizexe);
	ClassEFile::GetFileName(wizcmd);
	ClassEStd::StrCat(wizcmd, " \"");
	ClassEStd::StrCat(wizcmd, tmpfile);
	ClassEStd::StrCat(wizcmd, "\"");
	if (system(wizcmd) == 0)
		success = true;
	_chdir(curpath);
	if (success)
	{
		ClassEFile ef;
		char tmpdst[MAX_EFILESTR], tmpdst2[MAX_EFILESTR], tmpnew[MAX_EFILESTR];
		ClassEStd::StrCpy(tmpdst, tmpfile);
		ClassEStd::StrReplace(tmpdst, ".inf", ".cab");
		if (ClassEStd::StrLen(outputfile) == 0)
		{
			ClassEStd::StrCpy(tmpnew, curpath);
			ClassEStd::StrCat(tmpnew, "\\");
			ClassEStd::StrCat(tmpnew, binname);
			ClassEStd::StrCat(tmpnew, ".cab");
		}
		else
		{
			ClassEStd::StrCpy(tmpnew, outputfile);
			if (!ClassEStd::StrEqual(&tmpnew[ClassEStd::StrLen(tmpnew) - 4], ".cab", false))
				ClassEStd::StrCat(tmpnew, ".cab");
		}
		ef.Delete(tmpnew);
		ef.Rename(tmpnew, tmpdst);
		ClassEStd::StrCpy(tmpdst2, tmpdst);
		ClassEStd::StrReplace(tmpdst2, ".cab", ".dat");
		ef.Delete(tmpdst2);
	}
	return(success);
}

/////////////////////////////////////////////////////////////////////
// CabProcess: Public
/////////////////////////////////////////////////////////////////////

//Constructor
CabProcess::CabProcess(void)
{
	ClassEStd::StrCpy(vendor, "EDGELIB");
	ClassEStd::StrCpy(binname, "");
	ClassEStd::StrCpy(caption, "");
	ClassEStd::StrCpy(shortcut, "programs\\games");
	buildmax = 0;
	filecount = 0;
	groupcount = 0;
}

//Destructor
CabProcess::~CabProcess(void)
{
}

//Start app
bool CabProcess::Start(char *cabwizexe, char *inputfile, char *outputfile)
{
	ClassEIni ini;
	_getcwd(curpath, MAX_EFILESTR);
	if (ini.Open(inputfile))
	{
		char key[MAX_ESTR], value[MAX_ESTR];
		while(ini.ReadLine(key, value))
		{
			if (ClassEStd::StrEqual(key, "addfile", false))
			{
				HandleAddFile(value);
			}
			else if (ClassEStd::StrEqual(key, "capabilities", false))
			{
				if (ClassEStd::StrFind(value, "square", false) >= 0)
					buildmax |= 0x20000000;
				if (ClassEStd::StrFind(value, "rotation", false) >= 0)
					buildmax |= 0xC0000000;
			}
			else if (ClassEStd::StrEqual(key, "vendor", false))
				ClassEStd::StrCpy(vendor, value);
			else if (ClassEStd::StrEqual(key, "binname", false))
				ClassEStd::StrCpy(binname, value);
			else if (ClassEStd::StrEqual(key, "shortcut", false))
				ClassEStd::StrCpy(shortcut, value);
			else if (ClassEStd::StrEqual(key, "caption", false))
				ClassEStd::StrCpy(caption, value);
		}
		if (ClassEStd::StrLen(caption) == 0)
			ClassEStd::StrCpy(caption, binname);
		if (ClassEStd::StrLen(binname) == 0)
		{
			printf("Error: no binary name specfied in input file\n");
			ini.Close();
			return(false);
		}
		if (filecount == 0)
		{
			printf("Error: no files to add\n");
			ini.Close();
			return(false);
		}
		if (ClassEStd::StrFind(shortcut, "programs", false) != 0)
		{
			printf("Error: shortcut not supported, start with programs\n");
			ini.Close();
			return(false);
		}
		ClassEStd::StrReplace(shortcut, "programs", "%CE11%", false, 1);
		ini.Close();
	}
	else
	{
		printf("Error: can't open ");
		printf(inputfile);
		printf("\n");
		return(false);
	}
	ClassEFile ef;
	char tmpfile[MAX_EFILESTR];
	if (!WriteInf(tmpfile))
	{
		printf("Error: can't write ");
		printf(tmpfile);
		printf("\n");
		return(false);
	}
	if (!StartCabWiz(cabwizexe, tmpfile, outputfile))
	{
		ef.Delete(tmpfile);
		return(false);
	}
	ef.Delete(tmpfile);
	return(true);
}
