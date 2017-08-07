// eini.h
// An ini-file module
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED   //Can only be included by the main engine
#ifndef EINI_INCLUDED   //Make sure the module isn't included multiple times
#define EINI_INCLUDED

///////////////////////////////////////////////////////////////////
// Classes                                                       //
///////////////////////////////////////////////////////////////////

class ClassEIni
{
	private:
		ClassEFile ef;
		unsigned char *einidptr;
		unsigned char *einifilebuf;
		unsigned long einifilebufpos, einifilesize, einifilepos, einifilebufsize;
		bool opened;
		bool openmem;
		bool HandleFilebuf(void);
	public:
		ClassEIni(void);
		~ClassEIni(void);
		bool Open(const char *name, unsigned char flags = EFOF_READ);
		bool Open(const WCHAR *name, unsigned char flags = EFOF_READ);
		bool OpenData(void *ptr, unsigned long memsize);
		bool New(const char *name);
		bool New(const WCHAR *name);
		void Close(void);
		void ResetCursor(void);
		bool ReadLine(char *key, char *value);
		bool WriteLine(const char *key, const char *value);
		bool WriteLine(const char *key, long longvalue);
		bool WriteComment(const char *comment);
		bool WriteNewline(void);
		bool FindKey(const char *key, char *value, bool casesensitive = false);
		bool FindValue(char *key, const char *value, bool casesensitive = false);
		bool SetParseBufferSize(unsigned long newsize);
};//The ini class

#endif
#endif
