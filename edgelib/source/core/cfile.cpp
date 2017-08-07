// cfile.cpp
// Edgecore: provides file API
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Device include
#include "edgedevice.h"

//Device SDK includes
#if defined(DEVICE_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#elif defined(DEVICE_SYMBIAN)
	#include <eikenv.h>
	#include <eikappui.h>
	#include <eikapp.h>
	#include <f32file.h>
#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
	#if defined(DEVICE_ALP)
		#include <alp/alp.h>
	#endif	
	#include <sys/stat.h>
	#include <dirent.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <time.h>
	#if defined(DEVICE_ANDROID)
		#include <fcntl.h>
		#include <jni.h>
		jobject Call_EdgelibFile_open(const char * fn);
		void Call_EdgelibFile_close(jobject file);
		bool Call_EdgelibFile_isValid(jobject file);	
		int Call_EdgelibFile_read(jobject file, char * data, int len);
		int Call_EdgelibFile_seekstart(jobject file, int pos);
		int Call_EdgelibFile_seekskip(jobject file, int pos);
		int Call_EdgelibFile_seekend(jobject file, int pos);
		int Call_EdgelibFile_getLength(jobject file);
		int Call_EdgelibFile_statlength(const char * fn);
        jobject Call_EdgelibFile_opendir(const char * fn);
        int Call_EdgelibFile_getDirectoryCount(jobject file);
        char * Call_EdgelibFile_getDirectoryEntry(jobject file, int index);
		char * Call_EdgelibApp_getStoragePath();
		// they forgot to define these
		#define S_IREAD  (S_IRUSR | S_IRGRP | S_IROTH)
		#define S_IWRITE (S_IWUSR)
	#endif
#endif

#if defined(DEVICE_MAC)
	void MacGetCurrentPath(char *path);
	void MacGetReadOnlyPath(char *path);
#endif

//Core include
#include "edgecore.h"

///////////////////////////////////////////////////////////////////
// Definition safeguards
///////////////////////////////////////////////////////////////////

#if !defined(INVALID_SET_FILE_POINTER)
	#define INVALID_SET_FILE_POINTER 0xFFFFFFFF
#endif

///////////////////////////////////////////////////////////////////
// Internal definitions and structures
///////////////////////////////////////////////////////////////////

//Limits
#define EC_MAXPATH       512

//A file object
typedef struct
{
	#if defined(DEVICE_WIN32)
		HANDLE f;
		bool writeflush;
	#elif defined(DEVICE_SYMBIAN)
		RFs fserv;
		RFile f;
	#elif defined(DEVICE_ANDROID)
		FILE *f;
		jobject jf;
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		FILE *f;
	#elif defined(DEVICE_OPENKODE)
		KDFile *f;
	#else
		int f;
	#endif
	ecUnichar path[EC_MAXPATH];
}EC_FILEOBJ;

//A directory object
typedef struct
{
	#if defined(DEVICE_WIN32)
		HANDLE d;
		WIN32_FIND_DATAW ent;
		unsigned char lastfile;
	#elif defined(DEVICE_SYMBIAN)
		RFs fserv;
		RDir d;
    #elif defined(DEVICE_ANDROID)
        DIR *d;
        jobject jd;
        int lastfile;
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		DIR *d;
	#elif defined(DEVICE_OPENKODE)
		KDDir *d;
	#else
		int d;
	#endif
}EC_DIROBJ;

///////////////////////////////////////////////////////////////////
// File API
///////////////////////////////////////////////////////////////////

//Open file
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR
//Openmode: readonly, writeonly, newfile
ECOREOBJ ecFOpen(ecErr &errcode, const ecUnistring path, unsigned char openmode)
{
	errcode = 0;
	EC_FILEOBJ *fo = new EC_FILEOBJ;
	if (fo)
	{
		ecUnichar fname[EC_MAXPATH];
		ecExScanFilename(fname, path);
		ecStrCpy(fo->path, fname);
		#if defined(DEVICE_WIN32)
			DWORD mode;
			if (openmode & 1)
				mode = GENERIC_READ;
			else if (openmode & 2)
				mode = GENERIC_WRITE;
			else
				mode = GENERIC_READ | GENERIC_WRITE;
			if (openmode & 4)
				fo->f = CreateFileW((LPCWSTR)fname, mode, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			else
				fo->f = CreateFileW((LPCWSTR)fname, mode, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (fo->f == INVALID_HANDLE_VALUE)
				errcode = 3;
			fo->writeflush = false;
		#elif defined(DEVICE_SYMBIAN)
			if (fo->fserv.Connect() == KErrNone)
			{
				TBuf16<EC_MAXPATH> fnamedes;
				TUint mode;
				TInt ferr;
				if (openmode & 1)
					mode = EFileRead | EFileStream | EFileShareReadersOnly;
				else if (openmode & 2)
					mode = EFileWrite | EFileStream;
				else
					mode = EFileRead | EFileWrite | EFileStream;
				fnamedes.Copy((const TUint16 *)fname);
				if (openmode & 4)
					ferr = fo->f.Replace(fo->fserv, fnamedes, mode);
				else
					ferr = fo->f.Open(fo->fserv, fnamedes, mode);
				if (ferr != KErrNone)
				{
					errcode = 3;
					fo->fserv.Close();
				}
			}
			else
				errcode = 3;
		#elif defined(DEVICE_ANDROID)
			char tmpname[EC_MAXPATH];
			ecStrToUtf8(tmpname, fname);
			
			if (strncmp(tmpname, "::/", 3) == 0)
			{
			    fo->f = NULL;
			    fo->jf = Call_EdgelibFile_open(tmpname);
			    if (fo->jf == NULL) 
				errcode = 1;
			    else if (!Call_EdgelibFile_isValid(fo->jf))
			    {
				Call_EdgelibFile_close(fo->jf);
				fo->jf = NULL;
				errcode = 3;
			    }			    
			} else {
			    fo->jf = NULL;
			    char mode[8];
			    if (openmode & 4)
			    {
				    if (openmode & 1)
					    errcode = 3;
				    else if (openmode & 2)
					    ecStrCpy(mode, "wb");
				    else
					    ecStrCpy(mode, "w+b");
			    }
			    else
			    {
				    if (openmode & 1)
					    ecStrCpy(mode, "rb");
				    else if (openmode & 2)
					    errcode = 3;
				    else
					    ecStrCpy(mode, "r+b");
			    }
			    ecStrToUtf8(tmpname, fname);
			    if (errcode == 0)
			    {
				    fo->f = fopen(tmpname, mode);
				    if (fo->f == NULL)
					    errcode = 3;
			    }
			}
		#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
			char tmpname[EC_MAXPATH];
			char mode[8];
			if (openmode & 4)
			{
				if (openmode & 1)
					errcode = 3;
				else if (openmode & 2)
					ecStrCpy(mode, "wb");
				else
					ecStrCpy(mode, "w+b");
			}
			else
			{
				if (openmode & 1)
					ecStrCpy(mode, "rb");
				else if (openmode & 2)
					errcode = 3;
				else
					ecStrCpy(mode, "r+b");
			}
			ecStrToUtf8(tmpname, fname);
			if (errcode == 0)
			{
				fo->f = fopen(tmpname, mode);
				if (fo->f == NULL)
					errcode = 3;
			}
		#elif defined(DEVICE_OPENKODE)
			char tmpname[EC_MAXPATH];
			char mode[8];
			if (openmode & 4)
			{
				if (openmode & 1)
					errcode = 3;
				else if (openmode & 2)
					ecStrCpy(mode, "wb");
				else
					ecStrCpy(mode, "w+b");
			}
			else
			{
				if (openmode & 1)
					ecStrCpy(mode, "rb");
				else if (openmode & 2)
					errcode = 3;
				else
					ecStrCpy(mode, "r+b");
			}
			ecStrToUtf8(tmpname, fname);
			if (errcode == 0)
			{
				fo->f = kdFopen(tmpname, mode);
				if (fo->f == NULL)
					errcode = 3;
			}
		#else
			errcode = 2;
		#endif
		if (errcode)
		{
			delete fo;
			fo = NULL;
		}
	}
	else
		errcode = 1;
	return(fo);
}

//Close file
void ecFClose(ECOREOBJ fileobj)
{
	EC_FILEOBJ *fo = (EC_FILEOBJ *)fileobj;
	#if defined(DEVICE_WIN32)
		CloseHandle(fo->f);
	#elif defined(DEVICE_SYMBIAN)
		fo->f.Close();
		fo->fserv.Close();
	#elif defined(DEVICE_ANDROID)
		if (fo->f)
		    fclose(fo->f);
		if (fo->jf)
		    Call_EdgelibFile_close(fo->jf);
		sync();
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		fclose(fo->f);
		sync();
	#elif defined(DEVICE_OPENKODE)
		kdFclose(fo->f);
	#endif
	delete fo;
}

//Read from file
long ecFRead(ECOREOBJ fileobj, void *buffer, unsigned long bufsize)
{
	EC_FILEOBJ *fo = (EC_FILEOBJ *)fileobj;
	#if defined(DEVICE_WIN32)
		DWORD bytesread;
		if (ReadFile(fo->f, buffer, bufsize, &bytesread, NULL) == FALSE)
			return(0);
		return(bytesread);
	#elif defined(DEVICE_SYMBIAN)
		TPtr8 symbuf((TUint8 *)buffer, bufsize);
		if (fo->f.Read(symbuf, bufsize) != KErrNone)
			return(0);
		return(symbuf.Length());
	#elif defined(DEVICE_ANDROID)
		if (fo->jf)
		    return(Call_EdgelibFile_read(fo->jf, (char*)buffer, bufsize));
		return(fread(buffer, bufsize, 1, fo->f));
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		return(fread(buffer, bufsize, 1, fo->f));
	#elif defined(DEVICE_OPENKODE)
		return(kdFread(buffer, bufsize, 1, fo->f));
	#endif
	return(0);
}

//Write to file
long ecFWrite(ECOREOBJ fileobj, const void *buffer, unsigned long bufsize)
{
	EC_FILEOBJ *fo = (EC_FILEOBJ *)fileobj;
	#if defined(DEVICE_WIN32)
		DWORD byteswritten;
		if (WriteFile(fo->f, buffer, bufsize, &byteswritten, NULL) == FALSE)
			return(0);
		fo->writeflush = true;
		return(byteswritten);
	#elif defined(DEVICE_SYMBIAN)
		TPtrC8 symbuf((TUint8 *)buffer, bufsize);
		if (fo->f.Write(symbuf, bufsize) != KErrNone)
			return(0);
		return(symbuf.Length());
	#elif defined(DEVICE_ANDROID)
		if (fo->jf) return -1;
		return(fwrite(buffer, bufsize, 1, fo->f));
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		return(fwrite(buffer, bufsize, 1, fo->f));
	#elif defined(DEVICE_OPENKODE)
		return(kdFwrite(buffer, bufsize, 1, fo->f));
	#endif
	return(0);
}

//Seek to file position
//Type: begin, current, end
long ecFSeek(ECOREOBJ fileobj, long distance, unsigned char type)
{
	EC_FILEOBJ *fo = (EC_FILEOBJ *)fileobj;
	#if defined(DEVICE_WIN32)
		DWORD movestyle[] = { FILE_BEGIN, FILE_CURRENT, FILE_END };
		DWORD result = SetFilePointer(fo->f, distance, NULL, movestyle[type]);
		if (result == INVALID_SET_FILE_POINTER)
			return(-1);
		return(result);
	#elif defined(DEVICE_SYMBIAN)
		TSeek movestyle[] = { ESeekStart, ESeekCurrent, ESeekEnd };
		TInt pos = distance;
		if (fo->f.Seek(movestyle[type], pos) == KErrNone)
			return(pos);
	#elif defined(DEVICE_ANDROID)
		if (fo->jf)
		{
		    if (type == 0)
			return Call_EdgelibFile_seekstart(fo->jf, distance);
		    if (type == 1)
			return Call_EdgelibFile_seekskip(fo->jf, distance);
		    else
			*(char *) 0xd = 0;
		    return -1;
		}
		else
		{
		    int movestyle[] = { SEEK_SET, SEEK_CUR, SEEK_END };
		    if (fseek(fo->f, distance, movestyle[type]) > 0)
			    return(-1);
		    return(ftell(fo->f));
		}
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		int movestyle[] = { SEEK_SET, SEEK_CUR, SEEK_END };
		if (fseek(fo->f, distance, movestyle[type]) > 0)
			return(-1);
		return(ftell(fo->f));
	#elif defined(DEVICE_OPENKODE)
		KDfileSeekOrigin movestyle[] = { KD_SEEK_SET, KD_SEEK_CUR, KD_SEEK_END };
		if (kdFseek(fo->f, distance, movestyle[type]) > 0)
			return(-1);
		return(kdFtell(fo->f));
	#endif
	return(-1);
}

//Tell file position
long ecFTell(ECOREOBJ fileobj)
{
	EC_FILEOBJ *fo = (EC_FILEOBJ *)fileobj;
	#if defined(DEVICE_WIN32)
		DWORD result = SetFilePointer(fo->f, 0, NULL, FILE_CURRENT);
		if (result == INVALID_SET_FILE_POINTER)
			return(-1);
		return(result);
	#elif defined(DEVICE_SYMBIAN)
		TInt pos = 0;
		if (fo->f.Seek(ESeekCurrent, pos) != KErrNone)
			return(-1);
		return(pos);
	#elif defined(DEVICE_ANDROID)
		if (fo->jf) 
		    return Call_EdgelibFile_seekskip(fo->jf, 0);
		else
		    return(ftell(fo->f));
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		return(ftell(fo->f));
	#elif defined(DEVICE_OPENKODE)
		return(kdFtell(fo->f));
	#endif
	return(-1);
}

//Truncate file length
//Errcode: E_OK, E_UNSUPPORTED, E_ERROR
ecErr ecFTruncate(const ecUnistring path, unsigned long fsize)
{
	ecUnichar fname[EC_MAXPATH];
	ecExScanFilename(fname, path);
	#if defined(DEVICE_WIN32)
		HANDLE f = CreateFileW((LPCWSTR)fname, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (f == INVALID_HANDLE_VALUE)
			return(2);
		SetFilePointer(f, fsize, NULL, FILE_BEGIN);
		SetEndOfFile(f);
		CloseHandle(f);
	#elif defined(DEVICE_SYMBIAN)
		RFs fserv;
		RFile f;
		if (fserv.Connect() == KErrNone)
		{
			TBuf16<EC_MAXPATH> fnamedes;
			fnamedes.Copy((const TUint16 *)fname);
			if (f.Open(fserv, fnamedes, EFileRead | EFileWrite | EFileStream) != KErrNone)
			{
				fserv.Close();
				return(2);
			}
			if (f.SetSize(fsize) != KErrNone)
			{
				f.Close(); fserv.Close();
				return(2);
			}
			f.Close(); fserv.Close();
		}
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		char tmpname[EC_MAXPATH];
		ecStrToUtf8(tmpname, fname);
		if (truncate(tmpname, fsize) != 0)
			return(2);
	#elif defined(DEVICE_OPENKODE)
		char tmpname[EC_MAXPATH];
		ecStrToUtf8(tmpname, fname);
		if (kdTruncate(tmpname, fsize) != 0)
			return(2);
	#else
		return(1);
	#endif
	return(0);
}

//Return file information
//Errcode: E_OK, E_UNSUPPORTED, E_ERROR
//Filemode: regular (1), readable (2), writable (4), directory (8), hidden (16), system (32), archive (64)
ecErr ecFStat(const ecUnistring path, void *statbuf, bool plocaltime)
{
	EC_FILEINFO *fi = (EC_FILEINFO *)statbuf;
	ecUnichar fname[EC_MAXPATH];
	ecExScanFilename(fname, path);
	#if defined(DEVICE_WIN32)
		SYSTEMTIME systime;
		WIN32_FILE_ATTRIBUTE_DATA finfo;
		if (GetFileAttributesExW((LPCWSTR)fname, GetFileExInfoStandard, &finfo) == FALSE)
			return(2);
		fi->size = finfo.nFileSizeLow;
		fi->mode = 2;
		if (finfo.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
			fi->mode |= 1;
		if (!(finfo.dwFileAttributes & FILE_ATTRIBUTE_READONLY))
			fi->mode |= 4;
		if (finfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			fi->mode |= 8;
		if (finfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			fi->mode |= 16;
		if (finfo.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
			fi->mode |= 32;
		if (finfo.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			fi->mode |= 64;
		if (plocaltime)
		{
			FileTimeToLocalFileTime(&finfo.ftCreationTime, &finfo.ftCreationTime);
			FileTimeToLocalFileTime(&finfo.ftLastWriteTime, &finfo.ftLastWriteTime);
		}
		FileTimeToSystemTime(&finfo.ftCreationTime, &systime);
		fi->timecreated.year = systime.wYear;
		unsigned char ndt1[] = { (unsigned char)systime.wMonth, (unsigned char)systime.wDay, (unsigned char)systime.wHour, (unsigned char)systime.wMinute, (unsigned char)systime.wSecond };
		ecMemCpy(fi->timecreated.dt, ndt1, sizeof(ndt1));
		FileTimeToSystemTime(&finfo.ftLastWriteTime, &systime);
		fi->timemodified.year = systime.wYear;
		unsigned char ndt2[] = { (unsigned char)systime.wMonth, (unsigned char)systime.wDay, (unsigned char)systime.wHour, (unsigned char)systime.wMinute, (unsigned char)systime.wSecond };
		ecMemCpy(fi->timemodified.dt, ndt2, sizeof(ndt2));
	#elif defined(DEVICE_SYMBIAN)
		RFs fserv;
		if (fserv.Connect() == KErrNone)
		{
			RFile f;
			TTime systime;
			TBuf16<EC_MAXPATH> fnamedes;
			TUint attvalue;
			fnamedes.Copy((const TUint16 *)fname);
			if (fserv.Modified(fnamedes, systime) == KErrNone)
			{
				if (!plocaltime)
				{
					TLocale locale;
					TTimeIntervalSeconds universalTimeOffset(locale.UniversalTimeOffset());
					if (locale.QueryHomeHasDaylightSavingOn())
					{
						TTimeIntervalSeconds daylightTimeOffset(3600);
						systime -= daylightTimeOffset;
					}
					systime -= universalTimeOffset;
				}
				unsigned char ndt[] = { (unsigned char)systime.DateTime().Month() + 1, (unsigned char)systime.DateTime().Day() + 1, (unsigned char)systime.DateTime().Hour(), (unsigned char)systime.DateTime().Minute(), (unsigned char)systime.DateTime().Second() };
				fi->timecreated.year = systime.DateTime().Year();
				ecMemCpy(fi->timecreated.dt, ndt, sizeof(ndt));
				ecMemCpy(&fi->timemodified, &fi->timecreated, sizeof(fi->timecreated));
			}
			else
			{
				fserv.Close();
				return(2);
			}
			fi->mode = 2;
			if (fserv.Att(fnamedes, attvalue) == KErrNone)
			{
				if (attvalue & KEntryAttNormal)
					fi->mode |= 1;
				if (!(attvalue & KEntryAttReadOnly))
					fi->mode |= 4;
				if (attvalue & KEntryAttDir)
					fi->mode |= 8;
				if (attvalue & KEntryAttHidden)
					fi->mode |= 16;
				if (attvalue & KEntryAttSystem)
					fi->mode |= 32;
				if (attvalue & KEntryAttArchive)
					fi->mode |= 64;
			}
			else
			{
				fserv.Close();
				return(2);
			}
			fi->size = 0;
			if (f.Open(fserv, fnamedes, EFileRead) == KErrNone)
			{
				TInt symfilesize;
				if (f.Size(symfilesize) == KErrNone)
					fi->size = symfilesize;
				f.Close();
			}
			fserv.Close();
		}
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		tm *newtime;
		struct stat sbuf;
		char tmpname[EC_MAXPATH];
		ecStrToUtf8(tmpname, fname);
		#if defined(DEVICE_ANDROID)
			// trap into the virtual asset filesystem
			if (strncmp(tmpname, "::/", 3) == 0) 
			{
				int statlen = Call_EdgelibFile_statlength(tmpname);
				if (statlen < 0) return 2;
				fi->size = statlen;
				fi->mode = 2; // virtual file & readonly
				ecMemSet(&(fi->timecreated), 0, sizeof(EC_DATETIME));
				ecMemSet(&(fi->timemodified), 0, sizeof(EC_DATETIME));
				return(0);
			}
		#endif
		
		if (stat(tmpname, &sbuf) != 0)
			return(2);
		
		if (plocaltime)
			newtime = localtime((time_t *)(&sbuf.st_ctime));
		else
			newtime = gmtime((time_t *)(&sbuf.st_ctime));
		fi->timecreated.year = newtime->tm_year + 1900;
		unsigned char ndt1[] = { newtime->tm_mon + 1, newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec };
		ecMemCpy(fi->timecreated.dt, ndt1, sizeof(ndt1));
		if (plocaltime)
			newtime = localtime((time_t *)(&sbuf.st_mtime));
		else
			newtime = gmtime((time_t *)(&sbuf.st_mtime));
		fi->timemodified.year = newtime->tm_year + 1900;
		unsigned char ndt2[] = { newtime->tm_mon + 1, newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec };
		ecMemCpy(fi->timemodified.dt, ndt2, sizeof(ndt2));
		
		fi->size = sbuf.st_size;
		fi->mode = 0;
		if (sbuf.st_mode & S_IFREG)
			fi->mode |= 1;
		if (sbuf.st_mode & S_IREAD)
			fi->mode |= 2;
		if (sbuf.st_mode & S_IWRITE)
			fi->mode |= 4;
		if (sbuf.st_mode & S_IFDIR)
			fi->mode |= 8;
	#elif defined(DEVICE_OPENKODE)
		KDTm newtime;
		struct KDStat sbuf;
		char tmpname[EC_MAXPATH];
		ecStrToUtf8(tmpname, fname);
		if (kdStat(tmpname, &sbuf) != 0)
			return(2);
		if (plocaltime)
			kdLocaltime_r(&sbuf.st_mtime, &newtime);
		else
			kdGmtime_r(&sbuf.st_mtime, &newtime);
		fi->timecreated.year = newtime.tm_year + 1900;
		unsigned char ndt1[] = { newtime.tm_mon + 1, newtime.tm_mday, newtime.tm_hour, newtime.tm_min, newtime.tm_sec };
		ecMemCpy(fi->timecreated.dt, ndt1, sizeof(ndt1));
		if (plocaltime)
			kdLocaltime_r(&sbuf.st_mtime, &newtime);
		else
			kdGmtime_r(&sbuf.st_mtime, &newtime);
		fi->timemodified.year = newtime.tm_year + 1900;
		unsigned char ndt2[] = { newtime.tm_mon + 1, newtime.tm_mday, newtime.tm_hour, newtime.tm_min, newtime.tm_sec };
		ecMemCpy(fi->timemodified.dt, ndt2, sizeof(ndt2));
		fi->size = sbuf.st_size;
		fi->mode = 0;
		if (KD_ISREG(sbuf.st_mode))
			fi->mode |= 1;
		//if (KD_ISREAD(sbuf.st_mode))
			fi->mode |= 2;
		//if (KD_ISWRITE(sbuf.st_mode))
			fi->mode |= 4;
		if (KD_ISDIR(sbuf.st_mode))
			fi->mode |= 8;
	#else
		return(1);
	#endif
	return(0);
}

//Return file information
//Errcode: E_OK, E_UNSUPPORTED, E_ERROR::/`
//Filemode: regular (1), readable (2), writable (4), directory (8), hidden (16), system (32), archive (64)
ecErr ecFFStat(ECOREOBJ fileobj, void *statbuf, bool localtime)
{
	EC_FILEOBJ *fo = (EC_FILEOBJ *)fileobj;
	#if defined(DEVICE_WIN32)
		if (fo->writeflush)
		{
			FlushFileBuffers(fo->f);
			fo->writeflush = false;
		}
	#endif
	ecErr errcode = ecFStat(fo->path, statbuf, localtime);
	#if defined(DEVICE_SYMBIAN)
		if (errcode == 0)
		{
			EC_FILEINFO *fi = (EC_FILEINFO *)statbuf;
			TInt symfilesize;
			if (fo->f.Size(symfilesize) == KErrNone)
				fi->size = symfilesize;
		}
	#endif
	return(errcode);
}

//Rename/move a file
//Errcode: E_OK, E_UNSUPPORTED, E_ERROR
ecErr ecFRename(const ecUnistring dst, const ecUnistring src)
{
	ecUnichar dstname[EC_MAXPATH];
	ecUnichar srcname[EC_MAXPATH];
	ecExScanFilename(srcname, src);
	ecExScanFilename(dstname, dst);
	#if defined(DEVICE_WIN32)
		if (MoveFileW((LPCWSTR)srcname, (LPCWSTR)dstname) == FALSE)
			return(2);
	#elif defined(DEVICE_SYMBIAN)
		RFs fserv;
		if (fserv.Connect() == KErrNone)
		{
			TBuf16<EC_MAXPATH> fnamedessrc;
			TBuf16<EC_MAXPATH> fnamedesdst;
			fnamedessrc.Copy((const TUint16 *)srcname);
			fnamedesdst.Copy((const TUint16 *)dstname);
			if (fserv.Rename(fnamedessrc, fnamedesdst) != KErrNone)
			{
				fserv.Close();
				return(2);
			}
			fserv.Close();
		}
		else
			return(2);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		char tmpsrcfname[EC_MAXPATH], tmpdstfname[EC_MAXPATH];
		ecStrToUtf8(tmpsrcfname, srcname);
		ecStrToUtf8(tmpdstfname, dstname);
		if (rename(tmpsrcfname, tmpdstfname) != 0)
			return(2);
	#elif defined(DEVICE_OPENKODE)
		char tmpsrcfname[EC_MAXPATH], tmpdstfname[EC_MAXPATH];
		ecStrToUtf8(tmpsrcfname, srcname);
		ecStrToUtf8(tmpdstfname, dstname);
		if (kdRename(tmpsrcfname, tmpdstfname) != 0)
			return(2);
	#else
		return(1);
	#endif
	return(0);
}

//Rename/move a file
//Errcode: E_OK, E_UNSUPPORTED, E_ERROR
ecErr ecFRemove(const ecUnistring path)
{
	ecUnichar fname[EC_MAXPATH];
	ecExScanFilename(fname, path);
	#if defined(DEVICE_WIN32)
		if (DeleteFileW((LPCWSTR)fname) == FALSE)
			return(2);
	#elif defined(DEVICE_SYMBIAN)
		RFs fserv;
		if (fserv.Connect() == KErrNone)
		{
			TBuf16<EC_MAXPATH> fnamedes;
			fnamedes.Copy((const TUint16 *)fname);
			if (fserv.Delete(fnamedes) != KErrNone)
			{
				fserv.Close();
				return(2);
			}
			fserv.Close();
		}
		else
			return(2);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		char tmpname[EC_MAXPATH];
		ecStrToUtf8(tmpname, fname);
		if (remove(tmpname) != 0)
			return(2);
	#elif defined(DEVICE_OPENKODE)
		char tmpname[EC_MAXPATH];
		ecStrToUtf8(tmpname, fname);
		if (kdRemove(tmpname) != 0)
			return(2);
	#else
		return(1);
	#endif
	return(0);
}

//Get readonly path
void ecFGetReadOnlyPath(ecUnistring path)
{
	#if !defined(DEVICE_IPHONE) && !defined(DEVICE_ALP) && !defined(DEVICE_OPENKODE) && !defined(DEVICE_MAEMO) && !defined(DEVICE_MAC) && !defined(DEVICE_ANDROID)
		ecFGetBinaryPath(path);
		return;
	#endif
	unsigned short pos = 0;
	path[0] = 0;
	#if defined(DEVICE_IPHONE) || defined(DEVICE_MAC)
		char modulepath[256];
		MacGetReadOnlyPath(modulepath);
		ecStrCpy(path, modulepath);
	#elif defined(DEVICE_ALP)
		char *char_getpath = alp_bundle_ref_ro_pathname(alp_bundle_ref_application(), "rsc");
		if (char_getpath)
		{
			ecStrCpy(path, char_getpath);
			g_free(char_getpath);
		}
		else
			ecStrCpy(path, "/");
	#elif defined(DEVICE_OPENKODE)
		ecStrCpy(path, "/res/");
	#elif defined(DEVICE_ANDROID)
		ecStrCpy(path, "::/");
	#elif defined(DEVICE_MAEMO)
		pid_t pid;
		char modulepath[256];
		char modulepid[128];
		unsigned long backctr;
		int linklength;
		pid = getpid();
		ecStrCpy(modulepid, "/proc/");
		ecIntToStr(&modulepid[ecStrLen(modulepid)], pid);
		ecStrCat(modulepid, "/exe");
		linklength = readlink(modulepid, modulepath, 256);
		if (linklength < 0)
			modulepath[0] = 0;
		else
			modulepath[linklength] = 0;
		for (backctr = ecStrLen(modulepath) - 1; backctr > 0; backctr--)
			if (modulepath[backctr] == '/' || backctr == 1)
			{
				ecStrCpy(path, "/usr/share/");
				if (modulepath[backctr] == '/')
					ecStrCat(path, &modulepath[backctr + 1]);
				else
					ecStrCat(path, &modulepath[backctr]);
				ecStrCat(path, "/");
				break;
			}
	#endif
	pos = (unsigned short)ecStrLen(path);
	if (pos)
	{
		pos--;
		while (path[pos] != '\\' && path[pos] != '/' && pos > 0)
		{
			path[pos] = 0;
			pos--;
		}
	}
}

//Get binary/module startup path
void ecFGetBinaryPath(ecUnistring path)
{
	unsigned short pos = 0;
	path[0] = 0;
	#if defined(DEVICE_WIN32)
		GetModuleFileNameW(NULL, (LPWSTR)path, MAX_PATH);
	#elif defined(DEVICE_SYMBIAN)
		unsigned short *pathptr;
		unsigned long len = 0;
		#if defined(DEVICE_SYMBIANEXE)
			if (CEikonEnv::Static()->EikAppUi())
			{
				TBuf<512> pathWithoutDrive;
				TBuf<512> driveAndPath;
				TBuf<2> appDrive;
				CEikonEnv::Static()->FsSession().PrivatePath(pathWithoutDrive);
				appDrive.Copy(CEikonEnv::Static()->EikAppUi()->Application()->AppFullName().Left(2));
 				driveAndPath.Copy(appDrive);
				driveAndPath.Append(pathWithoutDrive);
				pathptr = (unsigned short *)driveAndPath.Ptr();
				len = driveAndPath.Length();
				if (len > 256)
					len = 256;
				for (unsigned long i = 0; i < len; i++)
					path[i] = (char)pathptr[i];
				path[len] = 0;
			}
			else
				ecStrCpy(path, "c:\\");
		#else
			TFileName app;
			Dll::FileName(app);
			TParsePtrC parse(app);
			pathptr = (unsigned short *)parse.DriveAndPath().Ptr();
			len = parse.DriveAndPath().Length();
			if (len > 256)
				len = 256;
			for (unsigned long i = 0; i < len; i++)
				path[i] = (char)pathptr[i];
			path[len] = 0;
		#endif
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		#if defined(DEVICE_ALP)
			//char *char_getpath = alp_bundle_ref_ro_pathname(alp_bundle_ref_application(), "rsc");
			char *char_getpath = alp_bundle_rw_pathname(alp_bundle_application(), "", FALSE);
			if (char_getpath)
			{
				ecStrCpy(path, char_getpath);
				g_free(char_getpath);
			}
			else
				ecStrCpy(path, "/");
		#elif defined(DEVICE_MAC)
			char modulepath[256];
			MacGetCurrentPath(modulepath);
			ecStrCpy(path, modulepath);
		#elif defined(DEVICE_ANDROID)
			char * char_getpath = Call_EdgelibApp_getStoragePath();			
			ecStrCpy(path, char_getpath);
			free(char_getpath);
		#else
			pid_t pid;
			char modulepath[256];
			char modulepid[128];
			int linklength;
			pid = getpid();
			ecStrCpy(modulepid, "/proc/");
			ecIntToStr(&modulepid[ecStrLen(modulepid)], pid);
			ecStrCat(modulepid, "/exe");
			linklength = readlink(modulepid, modulepath, 256);
			if (linklength < 0)
				modulepath[0] = 0;
			else
				modulepath[linklength] = 0;
			#if defined(DEVICE_MAEMO)
				unsigned long backctr;
				for (backctr = ecStrLen(modulepath) - 1; backctr > 0; backctr--)
					if (modulepath[backctr] == '/' || backctr == 1)
					{
						EC_FILEINFO finfo;
						ecUnichar makepath[512];
						ecStrCpy(path, getenv("HOME"));
						ecStrCat(path, "/.");
						if (modulepath[backctr] == '/')
							ecStrCat(path, &modulepath[backctr + 1]);
						else
							ecStrCat(path, &modulepath[backctr]);
						ecStrCpy(makepath, path);
						if (ecFStat(makepath, &finfo, false) != 0)
							ecDirNew(makepath);
						ecStrCat(path, "/");
						break;
					}
			#else
				ecStrCpy(path, modulepath);
			#endif
		#endif
	#elif defined(DEVICE_OPENKODE)
		ecStrCpy(path, "/data/");
	#endif
	pos = (unsigned short)ecStrLen(path);
	if (pos)
	{
		pos--;
		while (path[pos] != '\\' && path[pos] != '/' && pos > 0)
		{
			path[pos] = 0;
			pos--;
		}
	}
}

//Open a directory
//Errcode: E_OK, E_UNSUPPORTED, E_ERROR
ECOREOBJ ecDirOpen(ecErr &errcode, const ecUnistring path)
{
	errcode = 0;
	EC_DIROBJ *dobj = new EC_DIROBJ;
	if (dobj)
	{
		ecUnichar dname[EC_MAXPATH];
		ecExScanFilename(dname, path);
		#if defined(DEVICE_WIN32)
			dobj->d = FindFirstFileW((LPCWSTR)dname, &dobj->ent);
			if (dobj->d == INVALID_HANDLE_VALUE)
				errcode = 2;
			dobj->lastfile = 0;
		#elif defined(DEVICE_SYMBIAN)
			unsigned long mslen = ecStrLen(dname);
			if (mslen > 2)
			{
				if (dname[mslen - 3] == '*' && dname[mslen - 2] == '.' && dname[mslen - 1] == '*')
					dname[mslen - 2] = 0;
			}
			if (dobj->fserv.Connect() == KErrNone)
			{
				TBuf16<EC_MAXPATH> dnamedes;
				dnamedes.Copy((const TUint16 *)dname);
				if (dobj->d.Open(dobj->fserv, dnamedes, KEntryAttNormal | KEntryAttDir) != KErrNone)
				{
					errcode = 2;
					dobj->fserv.Close();
				}
			}
			else
				errcode = 2;
		#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)			
			char tmpname[EC_MAXPATH];
			long fctr, len = ecStrLen(dname);
			for (fctr = len - 1; fctr >= 0; fctr--)
			{
				if (dname[fctr] == '/')
					break;
				dname[fctr] = 0;
			}
			ecStrToUtf8(tmpname, dname);
			
			#if defined(DEVICE_ANDROID)
                dobj->d = NULL;
                dobj->jd = NULL;
                dobj->lastfile = 0;
			    if (strncmp(tmpname, "::/", 3) == 0)
                {
                    dobj->jd = Call_EdgelibFile_opendir(tmpname);
                    if (dobj->jd == NULL)
                    {
                        errcode = 2;
                    } else if (Call_EdgelibFile_getDirectoryCount(dobj->jd) < 0) {
                        errcode = 2;
                    }

                    if (errcode)
                    {
                        delete dobj;
                        dobj = NULL;
                    }
                    return dobj;
                }
			#endif
			
			dobj->d = opendir(tmpname);
			if (dobj->d == NULL)
				errcode = 2;
		#elif defined(DEVICE_OPENKODE)
			char tmpname[EC_MAXPATH];
			long fctr, len = ecStrLen(dname);
			for (fctr = len - 1; fctr >= 0; fctr--)
			{
				if (dname[fctr] == '/')
					break;
				dname[fctr] = 0;
			}
			ecStrToUtf8(tmpname, dname);
			dobj->d = kdOpenDir(tmpname);
			if (dobj->d == NULL)
				errcode = 2;
		#else
			errcode = 1;
		#endif
		if (errcode)
		{
			delete dobj;
			dobj = NULL;
		}
	}
	else
		errcode = 1;
	return(dobj);
}

//Close a directory
void ecDirClose(ECOREOBJ dirobj)
{
	EC_DIROBJ *dobj = (EC_DIROBJ *)dirobj;
	#if defined(DEVICE_WIN32)
		FindClose(dobj->d);
	#elif defined(DEVICE_SYMBIAN)
		dobj->d.Close();
		dobj->fserv.Close();
    #elif defined(DEVICE_ANDROID)
        if (dobj->d)
            closedir(dobj->d);
        if (dobj->jd)
            Call_EdgelibFile_close(dobj->jd);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		closedir(dobj->d);
	#elif defined(DEVICE_OPENKODE)
		kdCloseDir(dobj->d);
	#endif
	delete dobj;
}

//Read a directory entry
bool ecDirRead(ECOREOBJ dirobj, ecUnistring entry)
{
	EC_DIROBJ *dobj = (EC_DIROBJ *)dirobj;
	entry[0] = 0;
	#if defined(DEVICE_WIN32)
		ecStrCpy(entry, (ecUnistring)dobj->ent.cFileName);
		if (dobj->lastfile == 0)
		{
			if (FindNextFileW(dobj->d, &dobj->ent) == FALSE)
				dobj->lastfile++;
		}
		else if (dobj->lastfile == 1)
			return(false);
	#elif defined(DEVICE_SYMBIAN)
		TInt result;
		TEntry symentry;
		result = dobj->d.Read(symentry);
		if (result == KErrEof)
			return(false);
		if (result == KErrNone)
			ecStrCpy(entry, (ecUnistring)symentry.iName.Des().PtrZ());
		else
			return(false);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
        #if defined(DEVICE_ANDROID)
            if (dobj->jd)
            {
                char * direntry = Call_EdgelibFile_getDirectoryEntry(dobj->jd, dobj->lastfile);
                if (direntry)
                {
                    ecUtf8ToStr(entry, direntry);
                    free(direntry);
                    dobj->lastfile++;
                    return(true);
                }
                else
                    return(false);
            }
        #endif
		struct dirent *dbuf;
		dbuf = readdir(dobj->d);
		if (dbuf != NULL)
			ecUtf8ToStr(entry, dbuf->d_name);
		else
			return(false);
	#elif defined(DEVICE_OPENKODE)
		struct KDDirent *dbuf;
		dbuf = kdReadDir(dobj->d);
		if (dbuf != NULL)
			ecUtf8ToStr(entry, dbuf->d_name);
		else
			return(false);
	#else
		return(false);
	#endif
	return(true);
}

//Create a new directory
//Errcode: E_OK, E_UNSUPPORTED, E_ERROR
ecErr ecDirNew(const ecUnistring path)
{
	ecUnichar dname[EC_MAXPATH];
	ecExScanFilename(dname, path);
	#if defined(DEVICE_WIN32)
		if (CreateDirectoryW((LPCWSTR)dname, NULL) == FALSE)
			return(2);
	#elif defined(DEVICE_SYMBIAN)
		RFs fserv;
		if (fserv.Connect() == KErrNone)
		{
			TBuf16<EC_MAXPATH> dnamedes;
			ecStrCat(dname, "\\");
			dnamedes.Copy((const TUint16 *)dname);
			if (fserv.MkDir(dnamedes) != KErrNone)
			{
				fserv.Close();
				return(2);
			}
			fserv.Close();
		}
		else
			return(2);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		char tmpname[EC_MAXPATH];
		ecStrToUtf8(tmpname, dname);
		if (mkdir(tmpname, S_IRWXU | S_IRWXG | S_IRWXO) != 0)
			return(2);
	#elif defined(DEVICE_OPENKODE)
		char tmpname[EC_MAXPATH];
		ecStrToUtf8(tmpname, dname);
		if (kdMkdir(tmpname) != 0)
			return(2);
	#else
		return(1);
	#endif
	return(0);
}

//Remove a directory
//Errcode: E_OK, E_UNSUPPORTED, E_ERROR
ecErr ecDirRemove(const ecUnistring path)
{
	ecUnichar dname[EC_MAXPATH];
	ecExScanFilename(dname, path);
	#if defined(DEVICE_WIN32)
		if (RemoveDirectoryW((LPCWSTR)dname) == FALSE)
			return(2);
	#elif defined(DEVICE_SYMBIAN)
		RFs fserv;
		if (fserv.Connect() == KErrNone)
		{
			TBuf16<EC_MAXPATH> dnamedes;
			ecStrCat(dname, "\\");
			dnamedes.Copy((const TUint16 *)dname);
			if (fserv.RmDir(dnamedes) != KErrNone)
			{
				fserv.Close();
				return(2);
			}
			fserv.Close();
		}
		else
			return(2);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		char tmpname[EC_MAXPATH];
		ecStrToUtf8(tmpname, dname);
		if (rmdir(tmpname) != 0)
			return(2);
	#elif defined(DEVICE_OPENKODE)
		char tmpname[EC_MAXPATH];
		ecStrToUtf8(tmpname, dname);
		if (kdRmdir(tmpname) != 0)
			return(2);
	#else
		return(1);
	#endif
	return(0);
}
