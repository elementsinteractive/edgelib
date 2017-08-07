// emod_file.cpp
// File class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "estdc.h"
#include "ememory.h"
#include "efile.h"
#include "edevice.h"
#include "econsole.h"
#include "evtimer.h"

#include "core/edgecore.h"
#include "extra/verinfo.h"

#if defined(DEVICE_ALP)
	#include <alp/alp.h>
#endif

/////////////////////////////////////////////////////////////////////
// Pack key generator                                              //
/////////////////////////////////////////////////////////////////////
/*
unsigned long Tmp_LongFCC(char *fcc)
{
	return(((unsigned char)(fcc[0])) | ((unsigned char)(fcc[1]) << 8) | ((unsigned char)(fcc[2]) << 16) | ((unsigned char)(fcc[3]) << 24));
}

void Tmp_GenPackKey(char *vcode)
{
	unsigned long kc = Tmp_LongFCC(vcode);
	unsigned long key = (kc & 0xFF) * (kc & 0xFF00) / ((kc & 0xFF0000) >> 16) + ((kc & 0xFF000000) >> 7);
	char vstr[16];
	ClassEStd::IntToStr(vstr, key, 16, false);
	ClassEConsole::MsgWindow(vstr);
}
*/
/////////////////////////////////////////////////////////////////////
// Internal structures                                             //
/////////////////////////////////////////////////////////////////////

typedef struct
{
	unsigned long FileID;
	unsigned long VendorID;
	unsigned long DataCrc;
	unsigned long ExCrc;
	unsigned long Numfiles;
	unsigned long ReservedHeaders;
	unsigned long Reserved1;
	unsigned long Reserved2;
}EPK_MAINHEADER_OLD;
//Edge pack main header

typedef struct
{
	char OrgName[128];
	unsigned long Index;
	unsigned long OrgSize;
	unsigned long NewSize;
	unsigned long CompressType;
	unsigned long reserved1;
	unsigned long reserved2;
}EPK_FILEINFO_OLD;
//Edge pack file

typedef struct
{
	unsigned long fileid;
	unsigned long vendorid;
	unsigned long headersize;
}EPK_MAINHEADER_BASIC;
//Edge pack short main header

typedef struct
{
	unsigned long headersize;
}EPK_FILEINFO_BASIC;
//Edge pack short file

/////////////////////////////////////////////////////////////////////
// ClassEFile::Pure functions                                      //
/////////////////////////////////////////////////////////////////////

//Return the value of a four character string
unsigned long EFile_LongFCC(const char *fcc)
{
	return(((unsigned char)(fcc[0])) | ((unsigned char)(fcc[1]) << 8) | ((unsigned char)(fcc[2]) << 16) | ((unsigned char)(fcc[3]) << 24));
}

//Return the value of a four character string
unsigned long EFile_LongFCC(const WCHAR *fcc)
{
	return(((unsigned char)(fcc[0])) | ((unsigned char)(fcc[1]) << 8) | ((unsigned char)(fcc[2]) << 16) | ((unsigned char)(fcc[3]) << 24));
}

//Return the character string from a four character value
void EFile_CharFCC(char *fcc, unsigned long code)
{
	fcc[0] = (char)(code & 0xFF);
	fcc[1] = (char)((code >> 8) & 0xFF);
	fcc[2] = (char)((code >> 16) & 0xFF);
	fcc[3] = (char)((code >> 24) & 0xFF);
	fcc[4] = 0;
}

//Return the character string from a four character value
void EFile_CharFCC(WCHAR *fcc, unsigned long code)
{
	fcc[0] = (char)(code & 0xFF);
	fcc[1] = (char)((code >> 8) & 0xFF);
	fcc[2] = (char)((code >> 16) & 0xFF);
	fcc[3] = (char)((code >> 24) & 0xFF);
	fcc[4] = 0;
}

//Calculate a short checksum over data
unsigned long EFile_QuickChecksum(void *cdata, unsigned long dsize)
{
	char result[256];
	unsigned long rval = 0;
	ClassEStd::MD5Data(result, cdata, dsize);
	ClassEMemory::Copy(&rval, &result[5], sizeof(unsigned long));
	return(rval + 0xA9238);
}

//Copy the source into the destination while checking filenames
void EFile_CheckFileName(WCHAR *dst, const WCHAR *src, const WCHAR *curfilename, WCHAR *curfolder, bool fromreadonly = false)
{
	bool pathabsolute = false;
	unsigned long ctr, len;
	if (src == NULL)
	{
		ClassEStd::StrCpy(dst, curfilename);
		return;
	}
	if (src[0] == '\\' || src[0] == '/')
		pathabsolute = true;
	if (src[1] == ':' && (src[2] == '\\' || src[2] == '/'))
		pathabsolute = true;
	if (!pathabsolute)
	{
		if (ClassEStd::StrEqual(curfolder, ""))
		{
			if (fromreadonly)
				ecFGetReadOnlyPath(curfolder);
			else
				ecFGetBinaryPath(curfolder);
		}
		ClassEStd::StrCpy(dst, curfolder);
		len = ClassEStd::StrLen(dst);
		if (len > 0)
			if (dst[len - 1] != '/' && dst[len - 1] != '\\')
			{
				dst[len] = EFILE_PATHSEPARATOR;
				dst[len + 1] = 0;
			}
		dst += ClassEStd::StrLen(dst);
	}
	len = ClassEStd::StrLen(src);
	for (ctr = 0; ctr < len; ctr++)
	{
		if (src[ctr] == '\\' || src[ctr] == '/')
			dst[ctr] = EFILE_PATHSEPARATOR;
		else
			dst[ctr] = src[ctr];
	}
	dst[len] = 0;
}

//Check the filename for storing in a pack file
void EFile_CheckStoreName(char *dst, const char *src)
{
	char tmpname[MAX_EFILESTR];
	ClassEStd::StrCpy(tmpname, src);
	ClassEFile::GetFileName(tmpname);
	if (ClassEStd::StrLen(tmpname) >= 64)
	{
		long ctr, dotpos = -1;
		for (ctr = ClassEStd::StrLen(tmpname) - 1; ctr >= 0; ctr--)
			if (tmpname[ctr] == '.')
			{
				dotpos = ctr;
				break;
			}
		ClassEMemory::Copy(dst, tmpname, 64);
		if (dotpos != -1)
		{
			unsigned long dotlen = ClassEStd::StrLen(tmpname) - dotpos;
			if (dotlen > 32)
				dotlen = 32;
			ClassEMemory::Copy(&dst[63 - dotlen], &tmpname[dotpos], dotlen);
		}
		dst[63] = 0;
	}
	else
		ClassEStd::StrCpy(dst, tmpname);
}

//Check the filename for storing in a pack file
void EFile_CheckStoreName(char *dst, const WCHAR *src)
{
	char tmpname[MAX_EFILESTR];
	ClassEStd::StrCpy(tmpname, src);
	ClassEFile::GetFileName(tmpname);
	if (ClassEStd::StrLen(tmpname) >= 64)
	{
		long ctr, dotpos = -1;
		for (ctr = ClassEStd::StrLen(tmpname) - 1; ctr >= 0; ctr--)
			if (tmpname[ctr] == '.')
			{
				dotpos = ctr;
				break;
			}
		ClassEMemory::Copy(dst, tmpname, 64);
		if (dotpos != -1)
		{
			unsigned long dotlen = ClassEStd::StrLen(tmpname) - dotpos;
			if (dotlen > 32)
				dotlen = 32;
			ClassEMemory::Copy(&dst[63 - dotlen], &tmpname[dotpos], dotlen);
		}
		dst[63] = 0;
	}
	else
		ClassEStd::StrCpy(dst, tmpname);
}

/////////////////////////////////////////////////////////////////////
// ClassEFile::Private                                             //
/////////////////////////////////////////////////////////////////////

//Convert an old Edge pack file
bool ClassEFile::ConvertPackFile(void)
{
	EPK_MAINHEADER newmainheader;
	ClassEFile newpack;
	WCHAR tmpfname[MAX_EFILESTR];
	unsigned long *indices = NULL;
	bool haserror = true;
	ClassEStd::StrCpy(tmpfname, curfilename);
	ClassEStd::StrCat(tmpfname, "_tmp");
	if (Exists(tmpfname))
		return(false);
	newmainheader.fileid = EFile_LongFCC("EPK2");
	newmainheader.vendorid = mainheader.vendorid;
	newmainheader.headersize = sizeof(EPK_MAINHEADER);
	newmainheader.version = EPK_VERSION;
	newmainheader.numfiles = mainheader.numfiles;
	newmainheader.checksum = 0;
	newmainheader.checksum = EFile_QuickChecksum(&newmainheader, sizeof(EPK_MAINHEADER));
	newmainheader.numpaths = 0;
	indices = (unsigned long *)ClassEMemory::Alloc(newmainheader.numfiles * sizeof(unsigned long));
	if (indices == NULL)
		return(false);
	if (newpack.New(tmpfname))
	{
		unsigned long ctr;
		haserror = false;
		if (!newpack.Write(&newmainheader, newmainheader.headersize))
			haserror = true;
		if (!haserror)
			for (ctr = 0; ctr < newmainheader.numfiles; ctr++)
			{
				EPK_FILEINFO finfo;
				unsigned long readindex = 0;
				if (!PackGetFileInfo(&finfo, ctr, false))
					haserror = true;
				if (!haserror)
				{
					indices[ctr] = newpack.Tell();
					readindex = finfo.index;
					finfo.index = 0;
					finfo.headersize = sizeof(EPK_FILEINFO);
					if (!newpack.Write(&finfo, finfo.headersize))
						haserror = true;
				}
				if (!haserror)
				{
					unsigned char *tmpdata = NULL;
					tmpdata = (unsigned char *)ClassEMemory::Alloc(finfo.packsize);
					if (tmpdata != NULL)
					{
						Seek(readindex);
						if (!Read(tmpdata, finfo.packsize))
							haserror = true;
						if (!haserror)
							if (!newpack.Write(tmpdata, finfo.packsize))
								haserror = true;
					}
					else
						haserror = true;
					ClassEMemory::DeAlloc(tmpdata);
				}
				if (haserror)
					break;
			}
		if (!haserror)
			if (!newpack.Write(indices, newmainheader.numfiles * sizeof(unsigned long)))
				haserror = true;
		newpack.Close();
		if (!haserror)
		{
			WCHAR renameto[MAX_EFILESTR];
			ClassEStd::StrCpy(renameto, curfilename);
			Close();
			Delete(renameto);
			if (!Rename(renameto, tmpfname))
				haserror = true;
			if (Open(renameto))
			{
				authorized = true;
				ClassEMemory::Copy(&mainheader, &newmainheader, newmainheader.headersize);
			}
			else
				haserror = true;
		}
		else
			Delete(tmpfname);
	}
	ClassEMemory::DeAlloc(indices);
	return(!haserror);
}

/////////////////////////////////////////////////////////////////////
// ClassEFile::Public                                              //
/////////////////////////////////////////////////////////////////////

//ClassEFile: constructor
ClassEFile::ClassEFile(void)
{
	objfile = NULL;
	objdir = NULL;
	opened       = false;
	folderopened = false;
	isedgepack   = false;
	authorized   = false;
	ClassEStd::StrCpy(currentfolder, "");
	packfileindex = NULL;
}

//ClassEFile: destructor
ClassEFile::~ClassEFile(void)
{
	Close();
	CloseFolder();
}

//Remove the filename in a full path & filename
char *ClassEFile::GetFolderName(char *fullname)
{
	long ctr, len = ClassEStd::StrLen(fullname) - 1;
	for (ctr = len; ctr >= 0; ctr--)
	{
		if (fullname[ctr] == '\\' || fullname[ctr] == '/')
			break;
		fullname[ctr] = 0;
	}
	len = ClassEStd::StrLen(fullname);
	for (ctr = 0; ctr < len; ctr++)
		if (fullname[ctr] == '/' || fullname[ctr] == '\\')
			fullname[ctr] = EFILE_PATHSEPARATOR;
	return(fullname);
}

//Remove the filename in a full path & filename
WCHAR *ClassEFile::GetFolderName(WCHAR *fullname)
{
	long ctr, len = ClassEStd::StrLen(fullname) - 1;
	for (ctr = len; ctr >= 0; ctr--)
	{
		if (fullname[ctr] == '\\' || fullname[ctr] == '/')
			break;
		fullname[ctr] = 0;
	}
	len = ClassEStd::StrLen(fullname);
	for (ctr = 0; ctr < len; ctr++)
		if (fullname[ctr] == '/' || fullname[ctr] == '\\')
			fullname[ctr] = EFILE_PATHSEPARATOR;
	return(fullname);
}

//Remove the path in a full path & filename
char *ClassEFile::GetFileName(char *fullname)
{
	long ctr, s_pos = 0, len = ClassEStd::StrLen(fullname) - 1;
	for (ctr = len; ctr >= 0; ctr--)
		if (fullname[ctr] == '\\' || fullname[ctr] == '/')
		{
			s_pos = ctr + 1;
			break;
		}
	ClassEStd::StrCpy(fullname, &fullname[s_pos]);
	return(fullname);
}

//Remove the path in a full path & filename
WCHAR *ClassEFile::GetFileName(WCHAR *fullname)
{
	long ctr, s_pos = 0, len = ClassEStd::StrLen(fullname) - 1;
	for (ctr = len; ctr >= 0; ctr--)
		if (fullname[ctr] == '\\' || fullname[ctr] == '/')
		{
			s_pos = ctr + 1;
			break;
		}
	ClassEStd::StrCpy(fullname, &fullname[s_pos]);
	return(fullname);
}

//Open a file for reading and writing
bool ClassEFile::Open(const char *filename, unsigned char opentype)
{
	bool result;
	USTR_CHECK_BEGIN(filename);
	result = Open(ustr_filename, opentype);
	USTR_CHECK_END(filename);
	return(result);
}

//Open a file for reading and writing
bool ClassEFile::Open(const WCHAR *filename, unsigned char opentype)
{
	WCHAR fname[MAX_EFILESTR];
	ecErr result;
	unsigned char openmode = 0;
	Close();
	if ((opentype & EFOF_READWRITE) == EFOF_READ)
		openmode = 1;
	else if ((opentype & EFOF_READWRITE) == EFOF_WRITE)
		openmode = 2;
	curopentype = opentype;
	EFile_CheckFileName(fname, filename, curfilename, currentfolder, (opentype & EFOF_READONLYPATH) > 0);
	objfile = ecFOpen(result, (ecUnistring)fname, openmode);
	if (objfile == NULL)
		return(false);
	EPK_MAINHEADER_OLD oldheader;
	unsigned long epkchecksum = 0, filesig = 0;
	bool hasoldheader = false;
	opened = true;
	if (!(opentype & EFOF_NOEPKCHECK))
	{
		Read(&filesig, 4);
		if (filesig == EFile_LongFCC("EPCK"))
			hasoldheader = true;
		if (hasoldheader)
		{
			Seek(0);
			if (Read(&oldheader, sizeof(EPK_MAINHEADER_OLD)))
			{
				if (oldheader.FileID == EFile_LongFCC("EPCK"))
				{
					mainheader.fileid = oldheader.FileID;
					mainheader.vendorid = oldheader.VendorID;
					mainheader.headersize = sizeof(mainheader);
					mainheader.version = 100;
					mainheader.numfiles = oldheader.Numfiles;
					mainheader.checksum = 0;
					mainheader.numpaths = 0;
					epkchecksum = EFile_QuickChecksum(&mainheader, sizeof(mainheader));
					mainheader.checksum = epkchecksum;
					isedgepack = true;
					if (mainheader.vendorid == EFile_LongFCC("FREE"))
						authorized = true;
				}
			}
		}
		else if (filesig == EFile_LongFCC("EPK2"))
		{
			EPK_MAINHEADER_BASIC shortheader;
			Seek(0);
			if (Read(&shortheader, sizeof(EPK_MAINHEADER_BASIC)))
				if (shortheader.fileid == EFile_LongFCC("EPK2"))
				{
					Seek(0);
					mainheader.fileid = EFile_LongFCC("EPK2");
					mainheader.vendorid = EFile_LongFCC("FREE");
					mainheader.headersize = sizeof(mainheader);
					mainheader.version = EPK_VERSION;
					mainheader.numfiles = 0;
					mainheader.checksum = 0;
					mainheader.numpaths = 0;
					if (Read(&mainheader, sizeof(EPK_MAINHEADER)))
					{
						epkchecksum = mainheader.checksum;
						mainheader.checksum = 0;
						if (epkchecksum == EFile_QuickChecksum(&mainheader, mainheader.headersize))
						{
							mainheader.checksum = EFile_QuickChecksum(&mainheader, sizeof(mainheader));
							isedgepack = true;
							if (mainheader.vendorid == EFile_LongFCC("FREE"))
								authorized = true;
						}
					}
				}
		}
	}
	ClassEStd::StrCpy(curfilename, fname);
	Seek(0);
	return(true);
}

//Close a previously opened file
void ClassEFile::Close(void)
{
	if (opened)
	{
		ecFClose(objfile);
		opened = false;
		isedgepack = false;
		authorized = false;
	}
	packfileindex = NULL;
}

//Creates a new file and opens it for reading and writing
bool ClassEFile::New(const char *filename)
{
	bool result;
	USTR_CHECK_BEGIN(filename);
	result = New(ustr_filename);
	USTR_CHECK_END(filename);
	return(result);
}

//Creates a new file and opens it for reading and writing
bool ClassEFile::New(const WCHAR *filename)
{
	WCHAR fname[MAX_EFILESTR];
	ecErr result;
	Close();
	curopentype = EFOF_DEFAULT;
	EFile_CheckFileName(fname, filename, curfilename, currentfolder);
	objfile = ecFOpen(result, (ecUnistring)fname, 4);
	if (objfile == NULL)
		return(false);
	opened = true;
	ClassEStd::StrCpy(curfilename, fname);
	return(true);
}

//Return the size of an opened file
unsigned long ClassEFile::Size(const char *filename, unsigned char location)
{
	unsigned long result;
	USTR_CHECK_BEGIN(filename);
	result = Size(ustr_filename, location);
	USTR_CHECK_END(filename);
	return(result);
}

//Return the size of an opened file
unsigned long ClassEFile::Size(const WCHAR *filename, unsigned char location)
{
	WCHAR file_temp[MAX_EFILESTR];
	if (location & EFOF_READONLYPATH)
	{
		ecFGetReadOnlyPath(file_temp);
		if (filename) ClassEStd::StrCat(file_temp, filename);
		filename = file_temp;
	}
    
	E_FILEINFO info;
	if (GetInformation(&info, filename))
		return(info.filesize);
	return(0);
}

//Read data from a file
bool ClassEFile::Read(void *dst, unsigned long rsize)
{
	if (!opened || rsize == 0)
		return(false);
	if (ecFRead(objfile, dst, rsize) == 0)
		return(false);
	return(true);
}

//Write data to a file
bool ClassEFile::Write(const void *src, unsigned long wsize)
{
	if (!opened || wsize == 0)
		return(false);
	if (ecFWrite(objfile, src, wsize) == 0)
		return(false);
	return(true);
}

//Move bytes in an opened file
bool ClassEFile::MoveData(unsigned long dst, unsigned long src, unsigned long msize, unsigned long blocksize)
{
	if (!opened)
		return(false);
	unsigned char *buffer = NULL;
	unsigned long ctr, pos_read, pos_write, bytecount, numblocks = (msize + blocksize - 1) / blocksize;
	if (dst == src || msize == 0)
		return(true);
	buffer = (unsigned char *)ClassEMemory::Alloc(blocksize);
	if (buffer == NULL)
		return(false);
	for (ctr = 0; ctr < numblocks; ctr++)
	{
		bool haserror = false;
		if (dst < src)
		{
			if (ctr + 1 < numblocks)
				bytecount = blocksize;
			else
				bytecount = msize % blocksize;
			pos_read = src + ctr * blocksize;
			pos_write = dst + ctr * blocksize;
		}
		else
		{
			if (ctr == 0)
				bytecount = msize % blocksize;
			else
				bytecount = blocksize;
			pos_read = src + (numblocks - ctr - 1) * blocksize;
			pos_write = dst + (numblocks - ctr - 1) * blocksize;
		}
		if (bytecount == 0)
			bytecount = blocksize;
		Seek(pos_read);
		if (Read(buffer, bytecount))
		{
			Seek(pos_write);
			if (!Write(buffer, bytecount))
				haserror = true;
		}
		else
			haserror = true;
		if (haserror)
		{
			ClassEMemory::DeAlloc(buffer);
			return(false);
		}
	}
	ClassEMemory::DeAlloc(buffer);
	return(true);
}

//Set the file pointer to a new position
void ClassEFile::Seek(unsigned long pos)
{
	if (opened)
		ecFSeek(objfile, pos, 0);
}

//Return the current file pointer position
unsigned long ClassEFile::Tell(void)
{
	if (!opened)
		return(0);
	return(ecFTell(objfile));
}

//Returns whether a file exists or not
bool ClassEFile::Exists(const char *filename, unsigned char location){ return(GetAttributes(filename, location) != EFAF_NOTAVAILABLE); }
bool ClassEFile::Exists(const WCHAR *filename, unsigned char location){	return(GetAttributes(filename, location) != EFAF_NOTAVAILABLE); }

//Returns if a file is a folder or a normal file
bool ClassEFile::IsFolder(const char *filename, unsigned char location){ return((GetAttributes(filename, location) & EFAF_FOLDER) > 0); }
bool ClassEFile::IsFolder(const WCHAR *filename, unsigned char location){ return((GetAttributes(filename, location) & EFAF_FOLDER) > 0); }

//Set the file size
bool ClassEFile::SetSize(unsigned long fsize)
{
	bool success, previspack = isedgepack, prevauth = authorized;
	Close();
	success = (ecFTruncate((ecUnistring)curfilename, fsize) == 0);
	Open(curfilename, curopentype);
	if (!opened)
		return(false);
	if (success)
	{
		isedgepack = previspack;
		authorized = prevauth;
	}
	return(success);
}

//Delete a file
bool ClassEFile::Delete(const char *filename)
{
	bool result;
	USTR_CHECK_BEGIN(filename);
	result = Delete(ustr_filename);
	USTR_CHECK_END(filename);
	return(result);
}

//Delete a file
bool ClassEFile::Delete(const WCHAR *filename)
{
	WCHAR fname[MAX_EFILESTR];
	EFile_CheckFileName(fname, filename, curfilename, currentfolder);
	if (filename != NULL)
		return(ecFRemove((ecUnistring)fname) == 0);
	return(false);
}

//Rename a file
bool ClassEFile::Rename(const char *newfilename, const char *oldfilename)
{
	bool result;
	USTR_CHECK_BEGIN(oldfilename);
	USTR_CHECK_BEGIN(newfilename);
	result = Rename(ustr_newfilename, ustr_oldfilename);
	USTR_CHECK_END(newfilename);
	USTR_CHECK_END(oldfilename);
	return(result);
}

//Rename a file
bool ClassEFile::Rename(const WCHAR *newfilename, const WCHAR *oldfilename)
{
	WCHAR newfname[MAX_EFILESTR];
	WCHAR oldfname[MAX_EFILESTR];
	EFile_CheckFileName(oldfname, oldfilename, curfilename, currentfolder);
	EFile_CheckFileName(newfname, newfilename, curfilename, currentfolder);
	if (newfilename != NULL && oldfilename != NULL)
		return(ecFRename((ecUnistring)newfname, (ecUnistring)oldfname) == 0);
	return(false);
}

//Retrieve file attribute flags
unsigned short ClassEFile::GetAttributes(const char *filename, unsigned char location)
{
	WCHAR fname[MAX_EFILESTR];
	ClassEStd::StrCpy(fname, filename);
	return(GetAttributes(fname, location));
}

//Retrieve file attribute flags
unsigned short ClassEFile::GetAttributes(const WCHAR *filename, unsigned char location)
{
	WCHAR fname[MAX_EFILESTR];
	if (location & EFOF_READONLYPATH)
	{
	    ecFGetReadOnlyPath(fname);
	    if (filename) ClassEStd::StrCat(fname, filename);
	    filename = fname;
	}

	E_FILEINFO info;
	if (GetInformation(&info, filename))
		return(info.attributes);
	return(EFAF_NOTAVAILABLE);
}

//Retrieve file information
bool ClassEFile::GetInformation(E_FILEINFO *fileinfo, const char *filename, bool getlocaltime)
{
	bool result;
	USTR_CHECK_BEGIN(filename);
	result = GetInformation(fileinfo, ustr_filename, getlocaltime);
	USTR_CHECK_END(filename);
	return(result);
}

//Retrieve file information
bool ClassEFile::GetInformation(E_FILEINFO *fileinfo, const WCHAR *filename, bool getlocaltime)
{
	WCHAR fname[MAX_EFILESTR];
	EC_FILEINFO info;
	ecErr result;
	EFile_CheckFileName(fname, filename, curfilename, currentfolder);
	if (filename == NULL && opened)
		result = ecFFStat(objfile, &info, getlocaltime);
	else if (filename != NULL)
		result = ecFStat((ecUnistring)fname, &info, getlocaltime);
	else
		return(false);
	if (result != 0)
		return(false);
	fileinfo->filesize = info.size;
	fileinfo->attributes = 0;
	if (info.mode & 1)
		fileinfo->attributes |= EFAF_NORMAL;
	if (!(info.mode & 4))
		fileinfo->attributes |= EFAF_READONLY;
	if (info.mode & 8)
		fileinfo->attributes |= EFAF_FOLDER;
	if (info.mode & 16)
		fileinfo->attributes |= EFAF_HIDDEN;
	if (info.mode & 32)
		fileinfo->attributes |= EFAF_SYSTEM;
	if (info.mode & 64)
		fileinfo->attributes |= EFAF_ARCHIVE;
	fileinfo->datecreated.year = info.timecreated.year;
	fileinfo->datecreated.month = info.timecreated.dt[0];
	fileinfo->datecreated.day = info.timecreated.dt[1];
	fileinfo->datecreated.hour = info.timecreated.dt[2];
	fileinfo->datecreated.minute = info.timecreated.dt[3];
	fileinfo->datecreated.second = info.timecreated.dt[4];
	fileinfo->datemodified.year = info.timemodified.year;
	fileinfo->datemodified.month = info.timemodified.dt[0];
	fileinfo->datemodified.day = info.timemodified.dt[1];
	fileinfo->datemodified.hour = info.timemodified.dt[2];
	fileinfo->datemodified.minute = info.timemodified.dt[3];
	fileinfo->datemodified.second = info.timemodified.dt[4];
	return(true);
}

//Set the current folder
void ClassEFile::SetCurrentFolder(const char *foldername, unsigned char location)
{
	USTR_CHECK_BEGIN(foldername);
	SetCurrentFolder(ustr_foldername, location);
	USTR_CHECK_END(foldername);
}

//Set the current folder
void ClassEFile::SetCurrentFolder(const WCHAR *foldername, unsigned char location)
{
	WCHAR readonlypath[MAX_EFILESTR];
	if (location & EFOF_READONLYPATH)
	{
		ecFGetReadOnlyPath(readonlypath);
		if (foldername) ClassEStd::StrCat(readonlypath, foldername);
		foldername = readonlypath;
	}
	
	if (foldername == NULL)
		return;
	bool pathabsolute = false;
	unsigned long ctr, len;
	if (foldername[0] == '\\' || foldername[0] == '/')
		pathabsolute = true;
	if (foldername[1] == ':' && (foldername[2] == '\\' || foldername[2] == '/'))
		pathabsolute = true;
	if (pathabsolute)
		ClassEStd::StrCpy(currentfolder, "");
	else if (ClassEStd::StrEqual(currentfolder, ""))
		ecFGetBinaryPath(currentfolder);
	ClassEStd::StrCat(currentfolder, foldername);
	len = ClassEStd::StrLen(currentfolder);
	for (ctr = 0; ctr < len; ctr++)
	{
		if (currentfolder[ctr] == '\\' || currentfolder[ctr] == '/')
			currentfolder[ctr] = EFILE_PATHSEPARATOR;
	}
	if (currentfolder[len - 1] != '\\' && currentfolder[len - 1] != '/')
	{
		currentfolder[len] = EFILE_PATHSEPARATOR;
		currentfolder[len + 1] = 0;
	}
	len = ClassEStd::StrLen(currentfolder);
	for (ctr = 0; ctr < len; ctr++)
	{
		bool checkpath = false;
		if (ctr > 0)
		{
			if (currentfolder[ctr - 1] == EFILE_PATHSEPARATOR)
				checkpath = true;
		}
		else
			checkpath = true;
		if (checkpath)
		{
			char tmp_str[4];
			if (ClassEStd::StrLen(&currentfolder[ctr]) >= 2)
			{
				ClassEMemory::Copy(tmp_str, &currentfolder[ctr], 2);
				tmp_str[2] = 0;
				if (ClassEStd::StrEqual(tmp_str, "./") || ClassEStd::StrEqual(tmp_str, ".\\"))
				{
					ClassEMemory::Copy(&currentfolder[ctr], &currentfolder[ctr + 2], len - ctr - 1);
					len = ClassEStd::StrLen(currentfolder);
					ctr--;
					continue;
				}
			}
			if (ClassEStd::StrLen(&currentfolder[ctr]) >= 3)
			{
				ClassEMemory::Copy(tmp_str, &currentfolder[ctr], 3);
				tmp_str[3] = 0;
				if (ClassEStd::StrEqual(tmp_str, "../") || ClassEStd::StrEqual(tmp_str, "..\\"))
				{
					unsigned long subctr, numslash = 0;
					for (subctr = 0; subctr < ctr; subctr++)
						if (currentfolder[subctr] == '\\' || currentfolder[subctr] == '/')
						{
							while(currentfolder[subctr + 1] == EFILE_PATHSEPARATOR)
								subctr++;
							numslash++;
						}
					if (numslash > 1)
					{
						unsigned long startpos = ctr - 2;
						if (ctr < 2)
							startpos = 0;
						while(currentfolder[startpos] != EFILE_PATHSEPARATOR && startpos > 0)
							startpos--;
						ClassEMemory::Copy(&currentfolder[startpos + 1], &currentfolder[ctr + 3], len - ctr - 2);
						ctr = startpos;
					}
					else
					{
						ClassEMemory::Copy(&currentfolder[ctr], &currentfolder[ctr + 3], len - ctr - 2);
						ctr--;
					}
					len = ClassEStd::StrLen(currentfolder);
					continue;
				}
			}
		}
	}
	
}

//Get a pointer to the current folder
char *ClassEFile::GetCurrentFolderC(void)
{
	if (ClassEStd::StrEqual(currentfolder, ""))
		ecFGetBinaryPath(currentfolder);
	ClassEStd::StrCpy(currentfolder_chr, currentfolder);
	return(currentfolder_chr);
}

//Get a pointer to the current folder
WCHAR *ClassEFile::GetCurrentFolder(void)
{
	if (ClassEStd::StrEqual(currentfolder, ""))
		ecFGetBinaryPath(currentfolder);
	return(currentfolder);
}

//Creates a new folder
bool ClassEFile::NewFolder(const char *foldername)
{
	bool result;
	USTR_CHECK_BEGIN(foldername);
	result = NewFolder(ustr_foldername);
	USTR_CHECK_END(foldername);
	return(result);
}

//Creates a new folder
bool ClassEFile::NewFolder(const WCHAR *foldername)
{
	#if (EDITION_SDK < 1000)
		return(false);
	#else
		WCHAR fname[MAX_EFILESTR];
		EFile_CheckFileName(fname, foldername, curfilename, currentfolder);
		return(ecDirNew((ecUnistring)fname) == 0);
	#endif
}

//Open a folder
bool ClassEFile::OpenFolder(const char *searchfolder, const char *mask, unsigned char location)
{
	bool result;
	USTR_CHECK_BEGIN(searchfolder);
	USTR_CHECK_BEGIN(mask);
	result = OpenFolder(ustr_searchfolder, ustr_mask, location);
	USTR_CHECK_END(mask);
	USTR_CHECK_END(searchfolder);
	return(result);
}

//Open a folder
bool ClassEFile::OpenFolder(const WCHAR *searchfolder, const WCHAR *mask, unsigned char location)
{
	#if (EDITION_SDK < 1000)
		return(false);
	#else
		WCHAR search_tmp[MAX_EFILESTR];
		if (location & EFOF_READONLYPATH)
		{
		    ecFGetReadOnlyPath(search_tmp);
		    if (searchfolder) ClassEStd::StrCat(search_tmp, searchfolder);
		    searchfolder = search_tmp;
		}
		
		WCHAR fname[MAX_EFILESTR];
		ecErr result;
		CloseFolder();
		EFile_CheckFileName(fname, searchfolder, curfilename, currentfolder);
		if (mask == NULL)
			ClassEStd::StrCat(fname, "*.*");
		else
			ClassEStd::StrCat(fname, mask);
		objdir = ecDirOpen(result, (ecUnistring)fname);
		if (objdir == NULL)
			return(false);
		folderopened = true;
		return(true);
	#endif
}

//Read the folder file by file
bool ClassEFile::ReadFolder(char *filename)
{
	bool result;
	WCHAR ustr_filename[MAX_EFILESTR];
	result = ReadFolder(ustr_filename);
	ClassEStd::StrCpy(filename, ustr_filename);
	return(result);
}

//Read the folder file by file
bool ClassEFile::ReadFolder(WCHAR *filename)
{
	if (filename == NULL)
		return(false);
	if (folderopened)
	{
		if (ecDirRead(objdir, (ecUnistring)filename))
			return(true);
		else
			CloseFolder();
	}
	return(false);
}

//Close the folder
void ClassEFile::CloseFolder(void)
{
	if (folderopened)
	{
		ecDirClose(objdir);
		folderopened = false;
	}
}

//Delete an empty folder
bool ClassEFile::DeleteFolder(const char *foldername)
{
	bool result;
	USTR_CHECK_BEGIN(foldername);
	result = DeleteFolder(ustr_foldername);
	USTR_CHECK_END(foldername);
	return(result);
}

//Delete an empty folder
bool ClassEFile::DeleteFolder(const WCHAR *foldername)
{
	#if (EDITION_SDK < 1000)
		return(false);
	#else
		WCHAR fname[MAX_EFILESTR];
		EFile_CheckFileName(fname, foldername, curfilename, currentfolder);
		return(ecDirRemove((ecUnistring)fname) == 0);
	#endif
}

//Rename a folder
bool ClassEFile::RenameFolder(const char *newfoldername, const char *oldfoldername)
{
	#if (EDITION_SDK < 1000)
		return(false);
	#else
		return(Rename(newfoldername, oldfoldername));
	#endif
}

//Rename a folder
bool ClassEFile::RenameFolder(const WCHAR *newfoldername, const WCHAR *oldfoldername)
{
	#if (EDITION_SDK < 1000)
		return(false);
	#else
		return(Rename(newfoldername, oldfoldername));
	#endif
}

//Returns if the file is an opened pack
bool ClassEFile::PackValid(void)
{
	if (opened)
		return(isedgepack);
	return(false);
}

//Create a new pack file for reading and writing, supply your vendor signature (NULL = no vendor signature)
ERESULT ClassEFile::PackNew(const char *filename, const char *vendor)
{
	ERESULT result;
	USTR_CHECK_BEGIN(filename);
	USTR_CHECK_BEGIN(vendor);
	result = PackNew(ustr_filename, ustr_vendor);
	USTR_CHECK_END(vendor);
	USTR_CHECK_END(filename);
	return(result);
}

//Create a new pack file for reading and writing, supply your vendor signature (NULL = no vendor signature)
ERESULT ClassEFile::PackNew(const WCHAR *filename, const WCHAR *vendor)
{
	#if (EDITION_SDK < 500)
		return(E_NOFEATURE);
	#else
		if (opened)
			Close();
		if (Exists(filename))
			return(E_ALREADYFOUND);
		if (!New(filename))
			return(E_ERROR);
		unsigned long checksum;
		if (vendor == NULL)
			mainheader.vendorid = EFile_LongFCC("FREE");
		else
			mainheader.vendorid = EFile_LongFCC(vendor);
		mainheader.fileid       = EFile_LongFCC("EPK2");
		mainheader.headersize   = sizeof(mainheader);
		mainheader.version      = EPK_VERSION;
		mainheader.numfiles     = 0;
		mainheader.checksum     = 0;
		mainheader.numpaths     = 0;
		checksum = EFile_QuickChecksum(&mainheader, sizeof(mainheader));
		mainheader.checksum     = checksum;
		if (!Write(&mainheader, sizeof(EPK_MAINHEADER)))
		{
			Close();
			return(E_ERROR);
		}
		else
		{
			isedgepack = true;
			authorized = true;
		}
		Seek(0);
		return(E_OK);
	#endif
}

//Authorization procedure for vendor sigged packs
bool ClassEFile::PackAuthorize(unsigned long key)
{
	if (!isedgepack)
		return(false);
	if ((mainheader.vendorid & 0xFF000000) == 0)
		return(false);
	if ((mainheader.vendorid & 0xFF0000) == 0)
		return(false);
	if ((mainheader.vendorid & 0xFF00) == 0)
		return(false);
	if ((mainheader.vendorid & 0xFF) == 0)
		return(false);
	if (authorized)
		return(true);
	unsigned long kc = mainheader.vendorid;
	if (key == (kc & 0xFF) * (kc & 0xFF00) / ((kc & 0xFF0000) >> 16) + ((kc & 0xFF000000) >> 7))
	{
		authorized = true;
		return(true);
	}
	else
	{
		#if defined(DEVICE_OPENKODE)
			unsigned long curtimestamp = ClassERTimer::Count();
			while(1)
			{
				unsigned long newtimestamp = ClassERTimer::Count();
				if (newtimestamp < curtimestamp || newtimestamp >= curtimestamp + 2000)
					break;
			}
		#else
			Sleep(2000);
		#endif
	}
	return(false);
}

//Retrieve the number of files in the pack
unsigned long ClassEFile::PackContainFiles(void)
{
	if (!authorized)
		return(0);
	return(mainheader.numfiles);
}

//Retrieve individual file information from the pack
bool ClassEFile::PackGetFileInfo(EPK_FILEINFO *fileinfo, unsigned long id, bool getlocaltime)
{
	if (!authorized)
		return(false);
	if (id >= mainheader.numfiles)
		return(false);
	EPK_FILEINFO_BASIC shortfileinfo;
	fileinfo->headersize = sizeof(EPK_FILEINFO);
	ClassEMemory::Set(fileinfo->filename, 0, sizeof(fileinfo->filename));
	fileinfo->index = 0;
	fileinfo->orgsize = 0;
	fileinfo->packsize = 0;
	fileinfo->compresstype = ECMP_NONE;
	fileinfo->encrypttype = 0;
	fileinfo->pathid = 0;
	fileinfo->filedate = 0;
	fileinfo->packdate = 0;
	if (mainheader.version >= 200)
	{
		unsigned long indexpos;
		Seek(Size() - (mainheader.numfiles - id) * sizeof(unsigned long));
		if (Read(&indexpos, sizeof(unsigned long)))
		{
			Seek(indexpos);
			if (!Read(&shortfileinfo, sizeof(EPK_FILEINFO_BASIC)))
				return(false);
			Seek(indexpos);
			if (!Read(fileinfo, shortfileinfo.headersize))
				return(false);
		}
		else
			return(false);
		fileinfo->index = indexpos + shortfileinfo.headersize;
	}
	else
	{
		EPK_FILEINFO_OLD oldfileinfo;
		Seek(sizeof(EPK_MAINHEADER_OLD) + id * sizeof(EPK_FILEINFO_OLD));
		if (Read(&oldfileinfo, sizeof(oldfileinfo)))
		{
			EFile_CheckStoreName(fileinfo->filename, oldfileinfo.OrgName);
			fileinfo->index = oldfileinfo.Index;
			fileinfo->orgsize = oldfileinfo.OrgSize;
			fileinfo->packsize = oldfileinfo.NewSize;
			fileinfo->compresstype = (unsigned char)oldfileinfo.CompressType;
		}
		else
			return(false);
	}
	if (getlocaltime)
	{
		long localoffset = ClassEDevice::LocalTimeOffset();
		if (localoffset >= 0 || fileinfo->filedate >= (ETYPE_TIMESTAMP)(-localoffset))
			fileinfo->filedate += ClassEDevice::LocalTimeOffset();
		else if (localoffset < 0)
			fileinfo->filedate = 0;
		if (localoffset >= 0 || fileinfo->packdate >= (ETYPE_TIMESTAMP)(-localoffset))
			fileinfo->packdate += ClassEDevice::LocalTimeOffset();
		else if (localoffset < 0)
			fileinfo->packdate = 0;
	}
	return(true);
}

//Retrieve individual file information from the pack
bool ClassEFile::PackGetFileInfo(EPK_FILEINFO *fileinfo, const char *filename, bool getlocaltime)
{
	unsigned long id;
	if (!PackSearchFile(filename, id))
		return(false);
	return(PackGetFileInfo(fileinfo, id, getlocaltime));
}

//Retrieve individual file information from the pack
bool ClassEFile::PackGetFileInfo(EPK_FILEINFO *fileinfo, const WCHAR *filename, bool getlocaltime)
{
	unsigned long id;
	if (!PackSearchFile(filename, id))
		return(false);
	return(PackGetFileInfo(fileinfo, id, getlocaltime));
}

//Retrieve individual file data from the pack
ERESULT ClassEFile::PackGetFileData(unsigned char *&ptr, unsigned long &datasize, unsigned long id)
{
	if (!authorized)
		return(E_NOENTRY);
	EPK_FILEINFO fileinfo;
	ptr = NULL;
	if (!PackGetFileInfo(&fileinfo, id))
		return(E_NOTEXISTS);
	if (fileinfo.orgsize > 0)
	{
		Seek(fileinfo.index);
		if (fileinfo.compresstype != ECMP_NONE)
		{
			bool result;
			unsigned char *rdata = NULL;
			rdata = (unsigned char *)ClassEMemory::Alloc(fileinfo.packsize);
			if (rdata == NULL)
				return(E_NOMEMORY);
			Read(rdata, fileinfo.packsize);
			ptr = (unsigned char *)ClassEMemory::Alloc(fileinfo.orgsize);
			if (ptr == NULL)
			{
				ClassEMemory::DeAlloc(rdata);
				return(E_NOMEMORY);
			}
			if (fileinfo.compresstype == ECMP_EDGE)
			{
				ClassEMemory mem;
				result = mem.UnCompress(ptr, fileinfo.orgsize, rdata, fileinfo.packsize);
			}
			else if (fileinfo.compresstype == ECMP_ZLIB)
				result = ClassEMemory::UnCompressZLib(ptr, fileinfo.orgsize, rdata, fileinfo.packsize);
			else
				result = false;
			ClassEMemory::DeAlloc(rdata);
			if (!result)
			{
				ClassEMemory::DeAlloc(ptr);
				return(E_NODATA);
			}
		}
		else
		{
			ptr = (unsigned char *)ClassEMemory::Alloc(fileinfo.orgsize);
			if (ptr == NULL)
				return(E_NOMEMORY);
			Read(ptr, fileinfo.orgsize);
		}
	}
	datasize = fileinfo.orgsize;
	return(E_OK);
}

//Retrieve individual file data from the pack
ERESULT ClassEFile::PackGetFileData(unsigned char *&ptr, unsigned long &datasize, const char *filename)
{
	if (!authorized)
		return(E_NOENTRY);
	unsigned long id;
	if (!PackSearchFile(filename, id))
		return(E_NOTEXISTS);
	return(PackGetFileData(ptr, datasize, id));
}

//Retrieve individual file data from the pack
ERESULT ClassEFile::PackGetFileData(unsigned char *&ptr, unsigned long &datasize, const WCHAR *filename)
{
	if (!authorized)
		return(E_NOENTRY);
	unsigned long id;
	if (!PackSearchFile(filename, id))
		return(E_NOTEXISTS);
	return(PackGetFileData(ptr, datasize, id));
}

//Search for a file within the pack and return its id
bool ClassEFile::PackSearchFile(const char *filename, unsigned long &id)
{
	if (!authorized)
		return(false);
	unsigned long ctr;
	char storefname[MAX_EFILESTR];
	EFile_CheckStoreName(storefname, filename);
	if (packfileindex == NULL)
	{
		EPK_FILEINFO fileinfo;
		for (ctr = 0; ctr < mainheader.numfiles; ctr++)
			if (PackGetFileInfo(&fileinfo, ctr))
				if (ClassEStd::StrEqual(fileinfo.filename, storefname, false))
				{
					id = ctr;
					return(true);
				}
	}
	else
	{
		for (ctr = 0; ctr < mainheader.numfiles; ctr++)
			if (ClassEStd::StrEqual(&packfileindex[ctr * 64], storefname, false))
			{
				id = ctr;
				return(true);
			}
	}
	return(false);
}

//Search for a file within the pack and return its id
bool ClassEFile::PackSearchFile(const WCHAR *filename, unsigned long &id)
{
	if (!authorized)
		return(false);
	unsigned long ctr;
	char storefname[MAX_EFILESTR];
	EFile_CheckStoreName(storefname, filename);
	if (packfileindex == NULL)
	{
		EPK_FILEINFO fileinfo;
		for (ctr = 0; ctr < mainheader.numfiles; ctr++)
			if (PackGetFileInfo(&fileinfo, ctr))
				if (ClassEStd::StrEqual(fileinfo.filename, storefname, false))
				{
					id = ctr;
					return(true);
				}
	}
	else
	{
		for (ctr = 0; ctr < mainheader.numfiles; ctr++)
			if (ClassEStd::StrEqual(&packfileindex[ctr * 64], storefname, false))
			{
				id = ctr;
				return(true);
			}
	}
	return(false);
}

//Add a file to the pack
ERESULT ClassEFile::PackAddFile(const char *filename, bool overwrite, unsigned char compression)
{
	#if (EDITION_SDK < 500)
		return(E_NOFEATURE);
	#else
		if (!authorized)
			return(E_NOENTRY);
		ClassEFile addf;
		E_FILEINFO finfo;
		ETYPE_TIMESTAMP filetime = 0;
		unsigned char *filedata = NULL;
		char storefname[MAX_EFILESTR];
		unsigned long addfilesize = 0, foundid;
		ERESULT result;
		if (mainheader.numfiles >= 32768)
			return(E_TOOBIG);
		EFile_CheckStoreName(storefname, filename);
		if (PackSearchFile(storefname, foundid) && !overwrite)
			return(E_ALREADYFOUND);
		if (addf.GetInformation(&finfo, filename, false))
		{
			filetime = ClassEStd::DateTimeToTimestamp(&finfo.datemodified);
			addfilesize = finfo.filesize;
		}
		if (!addf.Open(filename, EFOF_READ))
			return(E_NOTEXISTS);
		if (addfilesize == 0)
			addfilesize = addf.Size();
		if (addfilesize > 0)
		{
			filedata = (unsigned char *)ClassEMemory::Alloc(addfilesize);
			if (filedata == NULL)
			{
				addf.Close();
				return(E_NOMEMORY);
			}
			addf.Read(filedata, addfilesize);
		}
		result = PackAddFileData(filename, filedata, addfilesize, overwrite, compression, filetime);
		ClassEMemory::DeAlloc(filedata);
		addf.Close();
		return(result);
	#endif
}

//Add a file to the pack
ERESULT ClassEFile::PackAddFile(const WCHAR *filename, bool overwrite, unsigned char compression)
{
	ERESULT result;
	CSTR_CHECK_BEGIN(filename);
	result = PackAddFile(cstr_filename, overwrite, compression);
	CSTR_CHECK_END(filename);
	return(result);
}

//Add data to an edge pack
ERESULT ClassEFile::PackAddFileData(const char *filename, unsigned char *data, unsigned long datasize, bool overwrite, unsigned char compression, ETYPE_TIMESTAMP filedate)
{
	#if (EDITION_SDK < 500)
		return(E_NOFEATURE);
	#else
		if (!authorized)
			return(E_NOENTRY);
		EPK_FILEINFO newfinfo;
		ERESULT result = E_OK;
		unsigned long *indices = NULL;
		unsigned char *tmpdata = NULL;
		unsigned long ctr, newfilecount, insertto, newfsize = 0;
		char storefname[MAX_EFILESTR];
		bool cmpresult = false;
		if (mainheader.version < 200)
			if (!ConvertPackFile())
				return(E_ERROR);
		if (data == NULL)
			datasize = 0;
		EFile_CheckStoreName(storefname, filename);
		if (!PackSearchFile(storefname, insertto))
			insertto = mainheader.numfiles;
		if (insertto < mainheader.numfiles)
		{
			if (!overwrite)
				return(E_ALREADYFOUND);
			newfilecount = mainheader.numfiles;
		}
		else
			newfilecount = mainheader.numfiles + 1;
		newfinfo.orgsize = datasize;
		newfinfo.packsize = newfinfo.orgsize + newfinfo.orgsize / 100 + 12;
		newfinfo.compresstype = compression;
		if (data != NULL)
		{
			tmpdata = (unsigned char *)ClassEMemory::Alloc(newfinfo.packsize);
			if (tmpdata == NULL)
				return(E_NOMEMORY);
			if (newfinfo.compresstype == ECMP_EDGE)
			{
				ClassEMemory mem;
				cmpresult = mem.Compress(tmpdata, newfinfo.packsize, data, newfinfo.orgsize);
			}
			else if (newfinfo.compresstype == ECMP_ZLIB)
				cmpresult = ClassEMemory::CompressZLib(tmpdata, newfinfo.packsize, data, newfinfo.orgsize);
			else
				cmpresult = false;
			if (cmpresult && newfinfo.packsize >= newfinfo.orgsize - 4)
				cmpresult = false;
		}
		if (!cmpresult)
		{
			if (data != NULL)
				ClassEMemory::Copy(tmpdata, data, newfinfo.orgsize);
			newfinfo.packsize = newfinfo.orgsize;
			newfinfo.compresstype = ECMP_NONE;
		}
		newfinfo.headersize = sizeof(EPK_FILEINFO);
		ClassEMemory::Set(newfinfo.filename, 0, sizeof(newfinfo.filename));
		ClassEStd::StrCpy(newfinfo.filename, storefname);
		newfinfo.encrypttype = 0;
		newfinfo.pathid = 0;
		newfinfo.filedate = filedate;
		newfinfo.packdate = ClassEDevice::TimestampNow(false);
		newfinfo.index = 0;
		indices = (unsigned long *)ClassEMemory::Alloc((mainheader.numfiles + 1) * sizeof(unsigned long));
		if (indices == NULL)
			result = E_NOMEMORY;
		if (result == E_OK)
		{
			newfsize = Size() + (newfilecount - mainheader.numfiles) * sizeof(unsigned long);
			if (mainheader.numfiles)
			{
				Seek(Size() - mainheader.numfiles * sizeof(unsigned long));
				if (!Read(indices, mainheader.numfiles * sizeof(unsigned long)))
					result = E_ERROR;
			}
			indices[mainheader.numfiles] = Size() - mainheader.numfiles * sizeof(unsigned long);
		}
		if (result == E_OK)
			if (mainheader.headersize != sizeof(EPK_MAINHEADER))
			{
				long idiff = sizeof(EPK_MAINHEADER) - mainheader.headersize;
				if (!MoveData(sizeof(EPK_MAINHEADER), mainheader.headersize, Size() - mainheader.headersize))
					result = E_ERROR;
				for (ctr = 0; ctr < mainheader.numfiles + 1; ctr++)
					indices[ctr] += idiff;
				newfsize += idiff;
			}
		if (result == E_OK)
		{
			unsigned long insertpos = newfsize - newfilecount * sizeof(unsigned long);
			if (insertto < mainheader.numfiles)
			{
				long ddiff = (newfinfo.headersize + newfinfo.packsize) - (indices[insertto + 1] - indices[insertto]);
				if (insertto < mainheader.numfiles)
					if (!MoveData(indices[insertto + 1] + ddiff, indices[insertto + 1], indices[mainheader.numfiles] - indices[insertto + 1]))
						result = E_ERROR;
				for (ctr = insertto + 1; ctr < mainheader.numfiles + 1; ctr++)
					indices[ctr] += ddiff;
				newfsize += ddiff;
				insertpos = indices[insertto];
			}
			else
				newfsize += newfinfo.headersize + newfinfo.packsize;
			if (result == E_OK)
			{
				Seek(insertpos);
				if (!Write(&newfinfo, newfinfo.headersize))
					result = E_ERROR;
			}
			if (result == E_OK && data != NULL)
			{
				Seek(insertpos + newfinfo.headersize);
				if (!Write(tmpdata, newfinfo.packsize))
					result = E_ERROR;
			}
		}
		if (result == E_OK)
		{
			Seek(newfsize - newfilecount * sizeof(unsigned long));
			if (!Write(indices, newfilecount * sizeof(unsigned long)))
				result = E_ERROR;
		}
		if (result == E_OK)
		{
			mainheader.numfiles   = newfilecount;
			mainheader.headersize = sizeof(EPK_MAINHEADER);
			mainheader.version    = EPK_VERSION;
			mainheader.checksum   = 0;
			mainheader.checksum   = EFile_QuickChecksum(&mainheader, mainheader.headersize);
			Seek(0);
			if (!Write(&mainheader, mainheader.headersize))
				result = E_ERROR;
		}
		if (result == E_OK)
			SetSize(newfsize);
		ClassEMemory::DeAlloc(indices);
		ClassEMemory::DeAlloc(tmpdata);
		return(result);
	#endif
}

//Add data to an edge pack
ERESULT ClassEFile::PackAddFileData(const WCHAR *filename, unsigned char *data, unsigned long datasize, bool overwrite, unsigned char compression, ETYPE_TIMESTAMP filedate)
{
	ERESULT result;
	CSTR_CHECK_BEGIN(filename);
	result = PackAddFileData(cstr_filename, data, datasize, overwrite, compression, filedate);
	CSTR_CHECK_END(filename);
	return(result);
}

//Delete a file from the pack
ERESULT ClassEFile::PackDeleteFile(unsigned long id)
{
	#if (EDITION_SDK < 500)
		return(E_NOFEATURE);
	#else
		if (!authorized)
			return(E_NOENTRY);
		if (id >= mainheader.numfiles)
			return(E_NOTEXISTS);
		if (mainheader.version < 200)
			if (!ConvertPackFile())
				return(E_ERROR);
		ERESULT result = E_OK;
		unsigned long *indices = NULL;
		unsigned long ctr, newfsize = Size();
		long ddiff;
		indices = (unsigned long *)ClassEMemory::Alloc((mainheader.numfiles + 1) * sizeof(unsigned long));
		if (indices == NULL)
			return(E_NOMEMORY);
		Seek(newfsize - mainheader.numfiles * sizeof(unsigned long));
		if (!Read(indices, mainheader.numfiles * sizeof(unsigned long)))
			result = E_ERROR;
		indices[mainheader.numfiles] = newfsize - mainheader.numfiles * sizeof(unsigned long);
		if (result == E_OK)
			if (mainheader.headersize != sizeof(EPK_MAINHEADER))
			{
				long idiff = sizeof(EPK_MAINHEADER) - mainheader.headersize;
				if (!MoveData(sizeof(EPK_MAINHEADER), mainheader.headersize, newfsize - mainheader.headersize))
					result = E_ERROR;
				for (ctr = 0; ctr < mainheader.numfiles + 1; ctr++)
					indices[ctr] += idiff;
				newfsize += idiff;
			}
		ddiff = indices[id + 1] - indices[id];
		if (result == E_OK)
		{
			if (!MoveData(indices[id], indices[id + 1], indices[mainheader.numfiles] - indices[id + 1]))
				result = E_ERROR;
			newfsize -= ddiff;
		}
		if (result == E_OK && mainheader.numfiles > 1)
		{
			for (ctr = id + 1; ctr < mainheader.numfiles; ctr++)
				indices[ctr] = indices[ctr + 1] - ddiff;
			Seek(indices[mainheader.numfiles - 1]);
			if (!Write(indices, (mainheader.numfiles - 1) * sizeof(unsigned long)))
				result = E_ERROR;
		}
		if (result == E_OK)
		{
			mainheader.numfiles--;
			mainheader.headersize = sizeof(EPK_MAINHEADER);
			mainheader.version    = EPK_VERSION;
			mainheader.checksum   = 0;
			mainheader.checksum   = EFile_QuickChecksum(&mainheader, mainheader.headersize);
			Seek(0);
			if (!Write(&mainheader, mainheader.headersize))
				result = E_ERROR;
			else
				SetSize(newfsize - sizeof(unsigned long));
		}
		ClassEMemory::DeAlloc(indices);
		return(result);
	#endif
}

//Delete a file from the pack
ERESULT ClassEFile::PackDeleteFile(const char *filename)
{
	if (!authorized)
		return(E_NOENTRY);
	unsigned long id;
	if (!PackSearchFile(filename, id))
		return(E_NOTEXISTS);
	return(PackDeleteFile(id));
}

//Delete a file from the pack
ERESULT ClassEFile::PackDeleteFile(const WCHAR *filename)
{
	if (!authorized)
		return(E_NOENTRY);
	unsigned long id;
	if (!PackSearchFile(filename, id))
		return(E_NOTEXISTS);
	return(PackDeleteFile(id));
}

//Extract a file from the pack
ERESULT ClassEFile::PackExtractFile(const char *path, unsigned long id, bool overwrite)
{
	if (!authorized)
		return(E_NOENTRY);
	ClassEFile exf;
	EPK_FILEINFO fileinfo;
	WCHAR wpath[MAX_EFILESTR], pname[MAX_EFILESTR];
	char destfilename[MAX_EFILESTR];
	unsigned long datasize;
	unsigned char *filedata = NULL;
	ERESULT eresult;
	if (path == NULL)
		ClassEStd::StrCpy(wpath, "");
	else
		ClassEStd::StrCpy(wpath, path);
	EFile_CheckFileName(pname, wpath, curfilename, currentfolder);
	if (id >= mainheader.numfiles)
		return(E_NOTEXISTS);
	if (!PackGetFileInfo(&fileinfo, id))
		return(E_ERROR);
	ClassEStd::StrCpy(destfilename, pname);
	if (destfilename[ClassEStd::StrLen(destfilename) - 1] != '\\' && destfilename[ClassEStd::StrLen(destfilename) - 1] != '/')
	{
		destfilename[ClassEStd::StrLen(destfilename) + 1] = 0;
		destfilename[ClassEStd::StrLen(destfilename)] = EFILE_PATHSEPARATOR;
	}
	ClassEStd::StrCat(destfilename, fileinfo.filename);
	if (!overwrite)
		if (exf.Exists(destfilename))
			return(E_ALREADYFOUND);
	eresult = PackGetFileData(filedata, datasize, id);
	if (eresult != E_OK)
		return(eresult);
	if (!exf.New(destfilename))
	{
		ClassEMemory::DeAlloc(filedata);
		return(E_ERROR);
	}
	if (datasize > 0)
		exf.Write(filedata, datasize);
	exf.Close();
	ClassEMemory::DeAlloc(filedata);
	return(E_OK);
}

//Extract a file from the pack
ERESULT ClassEFile::PackExtractFile(const WCHAR *path, unsigned long id, bool overwrite)
{
	ERESULT result;
	CSTR_CHECK_BEGIN(path);
	result = PackExtractFile(cstr_path, id, overwrite);
	CSTR_CHECK_END(path);
	return(result);
}

//Extract a file from the pack
ERESULT ClassEFile::PackExtractFile(const char *path, const char *filename, bool overwrite)
{
	if (!authorized)
		return(E_NOENTRY);
	unsigned long id;
	if (!PackSearchFile(filename, id))
		return(E_NOTEXISTS);
	return(PackExtractFile(path, id, overwrite));
}

//Extract a file from the pack
ERESULT ClassEFile::PackExtractFile(const WCHAR *path, const WCHAR *filename, bool overwrite)
{
	if (!authorized)
		return(E_NOENTRY);
	unsigned long id;
	if (!PackSearchFile(filename, id))
		return(E_NOTEXISTS);
	return(PackExtractFile(path, id, overwrite));
}

//Change the vendor signature
bool ClassEFile::PackChangeVendor(const char *newvendor)
{
	#if (EDITION_SDK < 500)
		return(false);
	#else
		if (!authorized)
			return(false);
		if (mainheader.version < 200)
			if (!ConvertPackFile())
				return(false);
		mainheader.vendorid = EFile_LongFCC(newvendor);
		mainheader.checksum = 0;
		mainheader.checksum = EFile_QuickChecksum(&mainheader, mainheader.headersize);
		Seek(0);
		if (!Write(&mainheader, mainheader.headersize))
			return(false);
		return(true);
	#endif
}

//Change the vendor signature
bool ClassEFile::PackChangeVendor(const WCHAR *newvendor)
{
	#if (EDITION_SDK < 500)
		return(false);
	#else
		if (!authorized)
			return(false);
		if (mainheader.version < 200)
			if (!ConvertPackFile())
				return(false);
		mainheader.vendorid = EFile_LongFCC(newvendor);
		mainheader.checksum = 0;
		mainheader.checksum = EFile_QuickChecksum(&mainheader, mainheader.headersize);
		Seek(0);
		if (!Write(&mainheader, mainheader.headersize))
			return(false);
		return(true);
	#endif
}

//Get the vendor signature
bool ClassEFile::PackGetVendor(char *newvendor)
{
	if (opened)
		if (isedgepack)
		{
			EFile_CharFCC(newvendor, mainheader.vendorid);
			return(true);
		}
	return(false);
}

//Get the vendor signature
bool ClassEFile::PackGetVendor(WCHAR *newvendor)
{
	if (opened)
		if (isedgepack)
		{
			EFile_CharFCC(newvendor, mainheader.vendorid);
			return(true);
		}
	return(false);
}

//Generate a file index table for quick file searching in the pack
char *ClassEFile::PackBuildIndex(void)
{
	if (!authorized)
		return(NULL);
	if (mainheader.numfiles == 0)
		return(NULL);
	packfileindex = (char *)ClassEMemory::Alloc(mainheader.numfiles * 64);
	if (packfileindex != NULL)
	{
		unsigned long ctr;
		ClassEMemory::Set(packfileindex, 0, mainheader.numfiles * 64);
		for (ctr = 0; ctr < mainheader.numfiles; ctr++)
		{
			EPK_FILEINFO info;
			if (PackGetFileInfo(&info, ctr, false))
				ClassEStd::StrCpy(&packfileindex[ctr * 64], info.filename);
		}
	}
	return(packfileindex);
}

//Set a new pack file index table for quick file searching in the pack
void ClassEFile::PackUseIndex(char *newindex)
{
	packfileindex = newindex;
}
