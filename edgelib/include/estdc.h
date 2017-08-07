// estdC.h
// A standard class for access of most-used methods
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED  //Can only be included by the main engine
#ifndef ESTD_INCLUDED  //Make sure the module isn't included multiple times
#define ESTD_INCLUDED

#define ESTD_MAXSTR    65536 //Maximum counter for internal string algorithms

///////////////////////////////////////////////////////////////////
// Classes                                                       //
///////////////////////////////////////////////////////////////////

class ClassEStd
{
	public:
		bool static IsWhiteSpace(WCHAR chr);
		void static StrCpy(char *dst, const char *src, long maxlen = -1);
		void static StrCpy(WCHAR *dst, const char *src, long maxlen = -1);
		void static StrCpy(char *dst, const WCHAR *src, long maxlen = -1);
		void static StrCpy(WCHAR *dst, const WCHAR *src, long maxlen = -1);
		void static StrCat(char *dst, const char *src);
		void static StrCat(WCHAR *dst, const char *src);
		void static StrCat(char *dst, const WCHAR *src);
		void static StrCat(WCHAR *dst, const WCHAR *src);
		void static StrCat(char *dst, int i, unsigned char base = 10, bool signedvalue = true);
		void static StrCat(WCHAR *dst, int i, unsigned char base = 10, bool signedvalue = true);
		long static StrLen(const char *str, long max_str = -1);
		long static StrLen(const WCHAR *str, long max_str = -1);
		bool static StrEqual(const char *str1, const char *str2, bool casesensitive = true);
		bool static StrEqual(const WCHAR *str1, const char *str2, bool casesensitive = true);
		bool static StrEqual(const char *str1, const WCHAR *str2, bool casesensitive = true);
		bool static StrEqual(const WCHAR *str1, const WCHAR *str2, bool casesensitive = true);
		void static IntToStr(char *dst, int i, unsigned char base = 10, bool signedvalue = true);
		void static IntToStr(WCHAR *dst, int i, unsigned char base = 10, bool signedvalue = true);
		int static StrToInt(char *str, unsigned char base = 10);
		int static StrToInt(WCHAR *str, unsigned char base = 10);
		void static StrToUtf8(char *dst, const WCHAR *src, long maxlen = -1);
		void static Utf8ToStr(WCHAR *dst, const char *src, long maxlen = -1);
		long static StrFind(const char *str, char character);
		long static StrFind(const WCHAR *str, WCHAR character);
		long static StrFind(const char *str, const char *findstr, bool casesensitive = true);
		long static StrFind(const WCHAR *str, const char *findstr, bool casesensitive = true);
		long static StrFind(const char *str, const WCHAR *findstr, bool casesensitive = true);
		long static StrFind(const WCHAR *str, const WCHAR *findstr, bool casesensitive = true);
		char static *StrTrimLeft(char *str);
		WCHAR static *StrTrimLeft(WCHAR *str);
		char static *StrTrimRight(char *str);
		WCHAR static *StrTrimRight(WCHAR *str);
		char static *StrTrim(char *str);
		WCHAR static *StrTrim(WCHAR *str);
		int static StrReplace(char *src, const char *search, const char *replace, bool casesensitive = true, int max_replace = -1);
		int static StrReplace(WCHAR *src, const char *search, const char *replace, bool casesensitive = true, int max_replace = -1);
		int static StrReplace(WCHAR *src, const char *search, const WCHAR *replace, bool casesensitive = true, int max_replace = -1);
		int static StrReplace(WCHAR *src, const WCHAR *search, const char *replace, bool casesensitive = true, int max_replace = -1);
		int static StrReplace(WCHAR *src, const WCHAR *search, const WCHAR *replace, bool casesensitive = true, int max_replace = -1);
		char static *StrUpper(char *str);
		WCHAR static *StrUpper(WCHAR *str);
		char static *StrLower(char *str);
		WCHAR static *StrLower(WCHAR *str);
		void static MD5Data(char *result, const void *buffer, unsigned long buflen);
		void static MD5Str(char *result, const char *str);
		void static MD5Str(char *result, const WCHAR *str);
		void static MD5File(char *result, ClassEFile *ef);
		void static MD5File(char *result, const char *filename);
		unsigned long static CRCData(const void *buffer, unsigned long buflen);
		ETYPE_TIMESTAMP static DateTimeToTimestamp(E_DATETIME *datetime);
		void static TimestampToDateTime(E_DATETIME *datetime, ETYPE_TIMESTAMP timestamp);
};//The standard class

#endif
#endif
