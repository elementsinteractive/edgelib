// emod_ini.cpp
// INI class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "estdc.h"
#include "ememory.h"
#include "efile.h"
#include "eini.h"
#include "econsole.h"

#include "extra/verinfo.h"

#define DEF_INIFILEBUF 4096

/////////////////////////////////////////////////////////////////////
// ClassEIni::Pure functions                                       //
/////////////////////////////////////////////////////////////////////

//Write a line to the ini
bool EIni_WriteLine(ClassEFile *ef, unsigned long &einifilesize, const char *key, const char *value, bool quotes = true, bool iscomment = false)
{
	char separator[] = " = ";
	char quote[] = "\"";
	char commenttag[] = "# ";
	char newline[] = "\r\n";
	if (einifilesize >= 2)
	{
		char tmp[2];
		ef->Seek(einifilesize - 2);
		ef->Read(tmp, 2);
		ef->Seek(einifilesize);
		if (tmp[0] != '\n' && tmp[1] != '\n')
		{
			ef->Write(newline, sizeof(newline) - 1);
			einifilesize += sizeof(newline) - 1;
		}
	}
	else
		ef->Seek(einifilesize);
	if (iscomment)
	{
		ef->Write(commenttag, sizeof(commenttag) - 1);
		einifilesize += sizeof(commenttag) - 1;
	}
	ef->Write(key, ClassEStd::StrLen(key));
	einifilesize += ClassEStd::StrLen(key);
	if (!iscomment)
	{
		ef->Write(separator, sizeof(separator) - 1);
		if (quotes)
			ef->Write(quote, sizeof(quote) - 1);
		ef->Write(value, ClassEStd::StrLen(value));
		if (quotes)
			ef->Write(quote, sizeof(quote) - 1);
		einifilesize += ClassEStd::StrLen(value) + sizeof(separator) - 1;
		if (quotes)
			einifilesize += sizeof(quote) * 2 - 2;
	}
	ef->Write(newline, sizeof(newline) - 1);
	einifilesize += sizeof(newline) - 1;
	return(false);
}

//Return if the character is a space separator
bool inline EIni_IsWhiteSpace(char chr)
{
	return(chr == ' ' || chr == '\t' || chr == '\n' || chr == '\r' || chr == '\f' || chr == '\x0b');
}

//Return if the character is a new line
bool inline EIni_IsNewline(char chr)
{
	return(chr == '\r' || chr == '\n');
}

//Return if the character is a comment tag
bool inline EIni_IsComment(char chr)
{
	return(chr == ';' || chr == '#');
}

//Return if the character is a quote
bool inline EIni_IsQuote(char chr)
{
	return(chr == '\"');
}

//Return if the character is a separator for assigning values
bool inline EIni_IsSeparator(char chr)
{
	return(chr == '=');
}

//Trim quoted strings, remove quotes
void EIni_QuoteTrim(char *str)
{
	ClassEStd::StrTrim(str);
	if (ClassEStd::StrLen(str))
		if (EIni_IsQuote(str[0]) && EIni_IsQuote(str[ClassEStd::StrLen(str) - 1]))
		{
			ClassEMemory::Copy(str, &str[1], ClassEStd::StrLen(str) - 2);
			str[ClassEStd::StrLen(str) - 2] = 0;
		}
}

/////////////////////////////////////////////////////////////////////
// ClassEIni::Private                                              //
/////////////////////////////////////////////////////////////////////

//Handle the file buffer
bool ClassEIni::HandleFilebuf(void)
{
	if (!opened)
		return(false);
	if (einifilepos >= einifilesize)
		return(false);
	if (einifilebufpos >= einifilebufsize)
	{
		if (einifilesize - einifilepos < einifilebufsize)
		{
			einifilebufpos = einifilebufsize - (einifilesize - einifilepos);
			if (!openmem)
			{
				if (!ef.Read(&einifilebuf[einifilebufpos], (einifilesize - einifilepos)))
					return(false);
			}
			else
				einifilebuf = einidptr + einifilesize - einifilebufsize;
			einifilepos = einifilesize;
		}
		else
		{
			einifilebufpos = 0;
			if (!openmem)
			{
				if (!ef.Read(einifilebuf, einifilebufsize))
					return(false);
			}
			else
				einifilebuf = &einidptr[einifilepos];
			einifilepos += einifilebufsize;
		}
		return(true);
	}
	return(false);
}

/////////////////////////////////////////////////////////////////////
// ClassEIni::Public                                               //
/////////////////////////////////////////////////////////////////////

//ClassEIni: constructor
ClassEIni::ClassEIni(void)
{
	opened = false;
	einifilebuf = NULL;
	einidptr = NULL;
}

//ClassEIni destructor
ClassEIni::~ClassEIni(void)
{
	Close();
}

//Open an ini
bool ClassEIni::Open(const char *name, unsigned char flags)
{
	#if (EDITION_SDK < 500)
		return(false);
	#else
		opened = ef.Open(name, flags);
		openmem = false;
		if (opened)
		{
			einifilesize = ef.Size();
			SetParseBufferSize(DEF_INIFILEBUF);
		}
		return(opened);
	#endif
}

//Open an ini
bool ClassEIni::Open(const WCHAR *name, unsigned char flags)
{
	#if (EDITION_SDK < 500)
		return(false);
	#else
		opened = ef.Open(name, flags);
		openmem = false;
		if (opened)
		{
			einifilesize = ef.Size();
			SetParseBufferSize(DEF_INIFILEBUF);
		}
		return(opened);
	#endif
}

//Open an ini from memory
bool ClassEIni::OpenData(void *ptr, unsigned long memsize)
{
	#if (EDITION_SDK < 500)
		return(false);
	#else
		if (memsize == 0)
			return(false);
		einidptr = (unsigned char *)ptr;
		einifilesize = memsize;
		opened = true;
		openmem = true;
		SetParseBufferSize(DEF_INIFILEBUF);
		return(true);
	#endif
}

//Create a new ini file
bool ClassEIni::New(const char *name)
{
	#if (EDITION_SDK < 1000)
		return(false);
	#else
		opened = ef.New(name);
		if (opened)
		{
			einifilesize = 0;
			einifilebufpos = DEF_INIFILEBUF;
			einifilepos = 0;
			openmem = false;
		}
		return(opened);
	#endif
}

//Create a new ini file
bool ClassEIni::New(const WCHAR *name)
{
	#if (EDITION_SDK < 1000)
		return(false);
	#else
		opened = ef.New(name);
		if (opened)
		{
			einifilesize = 0;
			einifilebufpos = DEF_INIFILEBUF;
			einifilepos = 0;
			openmem = false;
		}
		return(opened);
	#endif
}

//Close an ini
void ClassEIni::Close(void)
{
	if (opened && !openmem)
		if (einifilebuf != NULL)
		{
			ClassEMemory::DeAlloc(einifilebuf);
			einifilebuf = NULL;
		}
	if (opened)
	{
		if (!openmem)
			ef.Close();
		else
			einidptr = NULL;
	}
	opened = false;
	openmem = false;
}

//Reset the file cursor
void ClassEIni::ResetCursor(void)
{
	if (opened && !openmem)
		ef.Seek(0);
	einifilebufpos = einifilebufsize;
	einifilepos = 0;
}

//Read a line from the ini
bool ClassEIni::ReadLine(char *key, char *value)
{
	if (opened && (einifilepos < einifilesize || einifilebufpos < einifilebufsize) && key != NULL && value != NULL)
	{
		bool readingactive = true, busy = true, fileend = false;
		ClassEStd::StrCpy(key, "");
		ClassEStd::StrCpy(value, "");
		while (readingactive)
		{
			bool isparsing = false, oncomment = false, onquote = false, onvalue = false;
			unsigned long lpos = 0;
			while (busy)
			{
				if (einifilebufpos >= einifilebufsize)
					if (!HandleFilebuf())
					{
						readingactive = false;
						fileend = true;
						break;
					}
				if (readingactive)
				{
					if (EIni_IsNewline(einifilebuf[einifilebufpos]))
					{
						if (isparsing)
						{
							einifilebufpos++;
							break;
						}
						else
							oncomment = false;
					}
					if (EIni_IsComment(einifilebuf[einifilebufpos]) && !onquote)
						oncomment = true;
					if (!oncomment)
					{
						if (EIni_IsWhiteSpace(einifilebuf[einifilebufpos]))
							isparsing = true;
						if (EIni_IsQuote(einifilebuf[einifilebufpos]))
							onquote = !onquote;
						if (!onquote && EIni_IsSeparator(einifilebuf[einifilebufpos]))
						{
							lpos = 0;
							onvalue = true;
						}
						else
						{
							if (onvalue)
							{
								value[lpos] = einifilebuf[einifilebufpos];
								value[lpos + 1] = 0;
							}
							else
							{
								key[lpos] = einifilebuf[einifilebufpos];
								key[lpos + 1] = 0;
							}
							lpos++;
						}
					}
					einifilebufpos++;
				}
			}
			EIni_QuoteTrim(key);
			EIni_QuoteTrim(value);
			if (fileend)
				break;
			if (ClassEStd::StrLen(key) > 0)
				break;
		}
		if (ClassEStd::StrLen(key) == 0)
			return(false);
		return(true);
	}
	return(false);
}

//Write a line to the ini
bool ClassEIni::WriteLine(const char *key, const char *value)
{
	#if (EDITION_SDK < 1000)
		return(false);
	#else
		if (opened && !openmem)
			return(EIni_WriteLine(&ef, einifilesize, key, value, true));
		return(false);
	#endif
}

//Write a line to the ini
bool ClassEIni::WriteLine(const char *key, long longvalue)
{
	#if (EDITION_SDK < 1000)
		return(false);
	#else
		if (opened && !openmem)
		{
			char valuestr[16];
			ClassEStd::IntToStr(valuestr, longvalue);
			return(EIni_WriteLine(&ef, einifilesize, key, valuestr, false));
		}
		return(false);
	#endif
}

//Write a comment to the ini
bool ClassEIni::WriteComment(const char *comment)
{
	#if (EDITION_SDK < 1000)
		return(false);
	#else
		if (opened && !openmem)
			return(EIni_WriteLine(&ef, einifilesize, comment, NULL, false, true));
		return(false);
	#endif
}

//Write a newline to the ini
bool ClassEIni::WriteNewline(void)
{
	#if (EDITION_SDK < 1000)
		return(false);
	#else
		if (opened && !openmem)
		{
			char newline[] = "\r\n";
			if (ef.Write(newline, sizeof(newline) - 1))
			{
				einifilesize += sizeof(newline) - 1;
				return(true);
			}
		}
		return(false);
	#endif
}

//Search for a key
bool ClassEIni::FindKey(const char *key, char *value, bool casesensitive)
{
	if (opened && key != NULL && value != NULL)
	{
		char nkey[MAX_ESTR], nvalue[MAX_ESTR];
		while(ReadLine(nkey, nvalue))
			if (ClassEStd::StrEqual(key, nkey, casesensitive))
			{
				ClassEStd::StrCpy(value, nvalue);
				return(true);
			}
	}
	return(false);
}

//Search for a value
bool ClassEIni::FindValue(char *key, const char *value, bool casesensitive)
{
	if (opened && key != NULL && value != NULL)
	{
		char nkey[MAX_ESTR], nvalue[MAX_ESTR];
		while(ReadLine(nkey, nvalue))
			if (ClassEStd::StrEqual(value, nvalue, casesensitive))
			{
				ClassEStd::StrCpy(key, nkey);
				return(true);
			}
	}
	return(false);
}

//Increase the buffer size of the ini parser
bool ClassEIni::SetParseBufferSize(unsigned long newsize)
{
	if (opened && !openmem)
	{
		if (einifilebuf != NULL)
		{
			ClassEMemory::DeAlloc(einifilebuf);
			einifilebuf = NULL;
		}
		einifilebufsize = newsize;
		einifilebuf = (unsigned char *)ClassEMemory::Alloc(newsize);
		if (einifilebuf == NULL)
			return(false);
		ResetCursor();
	}
	if (opened && openmem)
	{
		einifilebufsize = newsize;
		ResetCursor();
		return(true);
	}
	return(false);
}
