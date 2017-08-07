// efile.h
// A file wrapper module
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED   //Can only be included by the main engine
#ifndef EFILE_INCLUDED  //Make sure the module isn't included multiple times
#define EFILE_INCLUDED

#define EPK_RESERVERHDR    8 //The number of headers to reserve for new files in an edge pack
#define EPK_VERSION      200 //Current Edge Pack version

#define ECMP_NONE          0
#define ECMP_EDGE          1
#define ECMP_ZLIB          2 //Compression types

#define EFOF_DEFAULT       0
#define EFOF_READ          1
#define EFOF_WRITE         2
#define EFOF_READWRITE     3
#define EFOF_READONLYPATH  4
#define EFOF_NOEPKCHECK    8 //Open flags

#define EFAF_NOTAVAILABLE  0
#define EFAF_NORMAL        1
#define EFAF_READONLY      2
#define EFAF_HIDDEN        4
#define EFAF_SYSTEM        8
#define EFAF_ARCHIVE      16
#define EFAF_FOLDER       32 //File attribute flags

#if defined(DEVICE_LINUX) || defined(DEVICE_OPENKODE)
	#define EFILE_PATHSEPARATOR '/'
#else
	#define EFILE_PATHSEPARATOR '\\'
#endif

///////////////////////////////////////////////////////////////////
// Structures                                                    //
///////////////////////////////////////////////////////////////////

typedef struct
{
	unsigned long fileid;
	unsigned long vendorid;
	unsigned long headersize;
	unsigned char version;
	unsigned long numfiles;
	unsigned long checksum;
	unsigned short numpaths;
}EPK_MAINHEADER;
//The main header of an edge pack

typedef struct
{
	unsigned long headersize;
	char filename[64];
	unsigned long index;
	unsigned long orgsize;
	unsigned long packsize;
	unsigned char compresstype;
	unsigned char encrypttype;
	unsigned short pathid;
	ETYPE_TIMESTAMP filedate;
	ETYPE_TIMESTAMP packdate;
}EPK_FILEINFO;
//A file within an edge pack

typedef struct
{
	unsigned short attributes;
	unsigned long filesize;
	E_DATETIME datecreated;
	E_DATETIME datemodified;
}E_FILEINFO;
//General file information

///////////////////////////////////////////////////////////////////
// Classes                                                       //
///////////////////////////////////////////////////////////////////

class ClassEFile
{
	friend class EHTTPRequest;
	private:
		ECOREOBJ objfile;
		ECOREOBJ objdir;
		EPK_MAINHEADER mainheader;
		WCHAR curfilename[MAX_EFILESTR];
		WCHAR currentfolder[MAX_EFILESTR];
		char currentfolder_chr[MAX_EFILESTR];
		char *packfileindex;
		unsigned char curopentype;
		bool opened;
		bool folderopened;
		bool isedgepack;
		bool authorized;
		bool ConvertPackFile(void);
	public:
		ClassEFile(void);
		~ClassEFile(void);
		char static *GetFolderName(char *fullname);
		WCHAR static *GetFolderName(WCHAR *fullname);
		char static *GetFileName(char *fullname);
		WCHAR static *GetFileName(WCHAR *fullname);
		bool Open(const char *filename, unsigned char opentype = EFOF_DEFAULT);
		bool Open(const WCHAR *filename, unsigned char opentype = EFOF_DEFAULT);
		void Close(void);
		bool New(const char *filename);
		bool New(const WCHAR *filename);
		unsigned long Size(const char *filename, unsigned char location = EFOF_DEFAULT);
		unsigned long Size(const WCHAR *filename = NULL, unsigned char location = EFOF_DEFAULT);
		bool Read(void *dst, unsigned long rsize);
		bool Write(const void *src, unsigned long wsize);
		bool MoveData(unsigned long dst, unsigned long src, unsigned long msize, unsigned long blocksize = 32768);
		void Seek(unsigned long pos);
		unsigned long Tell(void);
		bool Exists(const char *filename, unsigned char location = EFOF_DEFAULT);
		bool Exists(const WCHAR *filename, unsigned char location = EFOF_DEFAULT);
		bool IsFolder(const char *filename, unsigned char location = EFOF_DEFAULT);
		bool IsFolder(const WCHAR *filename, unsigned char location = EFOF_DEFAULT);
		bool SetSize(unsigned long fsize);
		bool Delete(const char *filename);
		bool Delete(const WCHAR *filename);
		bool Rename(const char *newfilename, const char *oldfilename);
		bool Rename(const WCHAR *newfilename, const WCHAR *oldfilename);
		unsigned short GetAttributes(const char *filename, unsigned char location = EFOF_DEFAULT);
		unsigned short GetAttributes(const WCHAR *filename = NULL, unsigned char location = EFOF_DEFAULT);
		bool GetInformation(E_FILEINFO *fileinfo, const char *filename, bool getlocaltime = true);
		bool GetInformation(E_FILEINFO *fileinfo, const WCHAR *filename = NULL, bool getlocaltime = true);
		void SetCurrentFolder(const char *foldername, unsigned char location = EFOF_DEFAULT);
		void SetCurrentFolder(const WCHAR *foldername, unsigned char location = EFOF_DEFAULT);
		char *GetCurrentFolderC(void);
		WCHAR *GetCurrentFolder(void);
		bool NewFolder(const char *foldername);
		bool NewFolder(const WCHAR *foldername);
		bool OpenFolder(const char *searchfolder, const char *mask = NULL, unsigned char location = EFOF_DEFAULT);
		bool OpenFolder(const WCHAR *searchfolder, const WCHAR *mask = NULL, unsigned char location = EFOF_DEFAULT);
		bool ReadFolder(char *filename);
		bool ReadFolder(WCHAR *filename);
		void CloseFolder(void);
		bool DeleteFolder(const char *foldername);
		bool DeleteFolder(const WCHAR *foldername);
		bool RenameFolder(const char *newfoldername, const char *oldfoldername);
		bool RenameFolder(const WCHAR *newfoldername, const WCHAR *oldfoldername);
		bool PackValid(void);
		ERESULT PackNew(const char *filename, const char *vendor = NULL);
		ERESULT PackNew(const WCHAR *filename, const WCHAR *vendor = NULL);
		bool PackAuthorize(unsigned long key);
		unsigned long PackContainFiles(void);
		bool PackGetFileInfo(EPK_FILEINFO *fileinfo, unsigned long id, bool getlocaltime = true);
		bool PackGetFileInfo(EPK_FILEINFO *fileinfo, const char *filename, bool getlocaltime = true);
		bool PackGetFileInfo(EPK_FILEINFO *fileinfo, const WCHAR *filename, bool getlocaltime = true);
		ERESULT PackGetFileData(unsigned char *&ptr, unsigned long &datasize, unsigned long id);
		ERESULT PackGetFileData(unsigned char *&ptr, unsigned long &datasize, const char *filename);
		ERESULT PackGetFileData(unsigned char *&ptr, unsigned long &datasize, const WCHAR *filename);
		bool PackSearchFile(const char *filename, unsigned long &id);
		bool PackSearchFile(const WCHAR *filename, unsigned long &id);
		ERESULT PackAddFile(const char *filename, bool overwrite = false, unsigned char compression = ECMP_ZLIB);
		ERESULT PackAddFile(const WCHAR *filename, bool overwrite = false, unsigned char compression = ECMP_ZLIB);
		ERESULT PackAddFileData(const char *filename, unsigned char *data, unsigned long datasize, bool overwrite = false, unsigned char compression = ECMP_ZLIB, ETYPE_TIMESTAMP filedate = EDT_TIMESTAMPNOW);
		ERESULT PackAddFileData(const WCHAR *filename, unsigned char *data, unsigned long datasize, bool overwrite = false, unsigned char compression = ECMP_ZLIB, ETYPE_TIMESTAMP filedate = EDT_TIMESTAMPNOW);
		ERESULT PackDeleteFile(unsigned long id);
		ERESULT PackDeleteFile(const char *filename);
		ERESULT PackDeleteFile(const WCHAR *filename);
		ERESULT PackExtractFile(const char *path, unsigned long id, bool overwrite = false);
		ERESULT PackExtractFile(const WCHAR *path, unsigned long id, bool overwrite = false);
		ERESULT PackExtractFile(const char *path, const char *filename, bool overwrite = false);
		ERESULT PackExtractFile(const WCHAR *path, const WCHAR *filename, bool overwrite = false);
		bool PackChangeVendor(const char *newvendor);
		bool PackChangeVendor(const WCHAR *newvendor);
		bool PackGetVendor(char *newvendor);
		bool PackGetVendor(WCHAR *newvendor);
		char *PackBuildIndex(void);
		void PackUseIndex(char *newindex);
};//The file class

#endif
#endif
