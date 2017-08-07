// extapi_data.cpp
// Data class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#include "extra/verinfo.h"

#define EDGE_INCLUDED
#include "estdc.h"
#include "ememory.h"

#include "extapi/dataapi.h"

///////////////////////////////////////////////////////////////////
// Data class
///////////////////////////////////////////////////////////////////

//Handle data expansion
bool EData::HandleExpansion(unsigned long lastindex)
{
	unsigned long allocstep_bytes = allocstep / indexsize;
	if (allocstep_bytes == 0)
		allocstep_bytes = 0;
	if (arrayptr != NULL)
	{
		if (!allowexpansion)
			return(false);
		if (lastindex * indexsize >= arraysize)
		{
			unsigned long newarraysize = (lastindex + allocstep_bytes) / allocstep_bytes * allocstep_bytes * indexsize;
			if (newarraysize == 0)
				return(false);
			void *newarrayptr = ClassEMemory::ReAlloc(arrayptr, newarraysize);
			if (newarrayptr == NULL)
			{
				ClassEMemory::DeAlloc(arrayptr);
				arrayptr = NULL;
				arraysize = 0;
				datasize = 0;
				return(false);
			}
			arrayptr = newarrayptr;
			arraysize = newarraysize;
		}
	}
	else
	{
		arraysize = (lastindex + allocstep_bytes) / allocstep_bytes * allocstep_bytes * indexsize;
		if (arraysize == 0)
			return(false);
		arrayptr = ClassEMemory::Alloc(arraysize);
		if (arrayptr == NULL)
		{
			arraysize = 0;
			return(false);
		}
		datasize = 0;
	}
	return(true);
}

//Initialize variables
void EData::Init(void)
{
	arrayptr = NULL;
	allocstep = 256;
	indexsize = 1;
	datasize = 0;
	allowexpansion = true;
	indexexpansion = true;
}

//EData: constructor
EData::EData(void)
{
	Init();
}

//EData: constructor
EData::EData(void *data, unsigned long length)
{
	Init();
	Add(data, length);
}

//EData: destructor
EData::~EData(void)
{
	if (arrayptr)
		ClassEMemory::DeAlloc(arrayptr);
}

//Add and expand data
bool EData::Add(void *data, unsigned long length)
{
	if (data == NULL || length == 0 || length % indexsize != 0)
		return(false);
	length /= indexsize;
	if (!HandleExpansion(datasize + length - 1))
		return(false);
	ClassEMemory::Copy(&((unsigned char *)arrayptr)[datasize * indexsize], data, length * indexsize);
	datasize += length;
	return(true);
}

//Get the size of data
unsigned long EData::GetDataSize()
{
	if (arrayptr)
		return(datasize);
	return(0);
}

//Return a pointer to the data (+ optional index)
void *EData::Get(unsigned long index)
{
	if (arrayptr)
		return(((unsigned char *)arrayptr) + index * indexsize);
	return(NULL);
}

//Configure class
void EData::Configure(bool allowexpansion, bool indexexpansion, unsigned long allocstep)
{
	this->allowexpansion = allowexpansion;
	this->indexexpansion = indexexpansion;
	this->allocstep = allocstep;
}

//Truncate data
void EData::Trunc(unsigned long length)
{
	if (length == 0)
	{
		ClassEMemory::DeAlloc(arrayptr);
		arrayptr = NULL;
		arraysize = 0;
	}
	datasize = length;
}

//Overloaded subscript operator
unsigned char &EData::operator[](int i)
{
	dummy = 0;
	if (i < 0)
		return((unsigned char &)(dummy));
	if (arrayptr == NULL || i >= (long)arraysize)
	{
		if (!allowexpansion || !indexexpansion)
			return((unsigned char &)(dummy));
		if (!HandleExpansion(i))
			return((unsigned char &)(dummy));
	}
	return(((unsigned char *)(arrayptr))[i]);
}

///////////////////////////////////////////////////////////////////
// Data class overloads
///////////////////////////////////////////////////////////////////

//EData16: constructor
EData16::EData16(void)
{
	indexsize = 2;
}

//EData16: constructor
EData16::EData16(void *data, unsigned long length)
{
	indexsize = 2;
	Add(data, length);
}

//EData16: destructor
EData16::~EData16(void)
{
}

//Overloaded subscript operator
unsigned short &EData16::operator[](int i)
{
	dummy = 0;
	if (i < 0)
		return((unsigned short &)(dummy));
	if (arrayptr == NULL || i >= (long)arraysize)
	{
		if (!allowexpansion || !indexexpansion)
			return((unsigned short &)(dummy));
		if (!HandleExpansion(i))
			return((unsigned short &)(dummy));
	}
	return(((unsigned short *)(arrayptr))[i]);
}

//EData32: constructor
EData32::EData32(void)
{
	indexsize = 4;
}

//EData32: constructor
EData32::EData32(void *data, unsigned long length)
{
	indexsize = 4;
	Add(data, length);
}

//EData32: destructor
EData32::~EData32(void)
{
}

//Overloaded subscript operator
unsigned long &EData32::operator[](int i)
{
	dummy = 0;
	if (i < 0)
		return((unsigned long &)(dummy));
	if (arrayptr == NULL || i >= (long)arraysize)
	{
		if (!allowexpansion || !indexexpansion)
			return((unsigned long &)(dummy));
		if (!HandleExpansion(i))
			return((unsigned long &)(dummy));
	}
	return(((unsigned long *)(arrayptr))[i]);
}

///////////////////////////////////////////////////////////////////
// String class
///////////////////////////////////////////////////////////////////

//EString: constructor
EString::EString(void)
{
}

//EString: constructor
EString::EString(const char *str)
{
	Copy(str);
}

//EString: constructor
EString::EString(const WCHAR *str)
{
	Copy(str);
}

//EString: copy constructor
EString::EString(const EString &str)
{
	Copy(str);
}

//EString: destructor
EString::~EString(void)
{
}

//String copy
void EString::Copy(const char *str)
{
	unsigned long len = ClassEStd::StrLen(str);
	if (len)
	{
		bool result;
		unsigned long oldstepsize = allocstep;
		allocstep = indexsize;
		result = HandleExpansion(len);
		allocstep = oldstepsize;
		if (!result)
			return;
		for (unsigned long i = 0; i < len + 1; i++)
			((unsigned short *)arrayptr)[i] = str[i];
		datasize = len + 1;
	}
	else
		Trunc(0);
}

//String copy
void EString::Copy(const WCHAR *str)
{
	unsigned long len = ClassEStd::StrLen(str);
	if (len)
	{
		bool result;
		unsigned long oldstepsize = allocstep;
		allocstep = indexsize;
		result = HandleExpansion(len);
		allocstep = oldstepsize;
		if (!result)
			return;
		for (unsigned long i = 0; i < len + 1; i++)
			((unsigned short *)arrayptr)[i] = str[i];
		datasize = len + 1;
	}
	else
		Trunc(0);
}

//String copy
void EString::Copy(const EString &str)
{
	if (str.arrayptr == NULL)
		return;
	unsigned long len = str.datasize - 1;
	if (len)
	{
		bool result;
		unsigned long oldstepsize = allocstep;
		allocstep = indexsize;
		result = HandleExpansion(len);
		allocstep = oldstepsize;
		if (!result)
			return;
		for (unsigned long i = 0; i < len; i++)
			((unsigned short *)arrayptr)[i] = ((unsigned short *)str.arrayptr)[i];
		((unsigned short *)arrayptr)[len] = 0;
		datasize = len + 1;
	}
	else
		Trunc(0);
}

//Concatenate string
void EString::Copy(int value, unsigned char base, bool signedvalue)
{
	char tmpstr[20];
	ClassEStd::IntToStr(tmpstr, value, base, signedvalue);
	Copy(tmpstr);
}

//Get string data
WCHAR *EString::Get(void)
{
	if (arrayptr)
	{
		WCHAR *wptr = (WCHAR *)arrayptr;
		if (wptr[datasize - 1] != 0)
			wptr[datasize - 1] = 0;
		return(wptr);
	}
	dummy = 0;
	return((WCHAR *)&dummy);
}

//Return string character
WCHAR EString::Chr(unsigned long index)
{
	if (index + 1 < datasize)
		return(((WCHAR *)arrayptr)[index]);
	return(0);
}

//Return string character
void EString::SetChr(unsigned long index, WCHAR chr)
{
	if (index < datasize)
	{
		if (chr == 0)
		{
			((WCHAR *)arrayptr)[index] = chr;
			datasize = index + 1;
			return;
		}
		if (index + 1 >= datasize)
		{
			if (HandleExpansion(index + 1))
			{
				((WCHAR *)arrayptr)[index] = chr;
				((WCHAR *)arrayptr)[index + 1] = 0;
				datasize++;
			}
		}
		else
			((WCHAR *)arrayptr)[index] = chr;
	}
}

//Copy a sub string
void EString::SubStr(EString &str, long start, long length)
{
	if (start < 0)
		start = datasize + start - 1;
	if (length <= 0)
		length = datasize + length - 1;
	else
		length += start;
	if (start < 0)
		start = 0;
	if (length <= start)
		return;
	str.Copy(&Get()[start]);
	str.Trunc(length - start);
}

//Copy a sub string
EString &EString::SubStr(long start, long length)
{
	if (start < 0)
		start = datasize + start - 1;
	if (length <= 0)
		length = datasize + length - 1;
	else
		length += start;
	if (start < 0)
		start = 0;
	if (length <= start)
	{
		Trunc(0);
		return(*this);
	}
	Copy(&Get()[start]);
	Trunc(length - start);
	return(*this);
}

//Truncate string
void EString::Trunc(unsigned long length)
{
	SetChr(length, 0);
}

//Return string length
unsigned long EString::Length(void)
{
	if (datasize == 0)
		return(0);
	return(datasize - 1);
}

//Concatenate string
bool EString::Cat(const char *str)
{
	unsigned long len = ClassEStd::StrLen(str);
	if (len == 0)
		return(false);
	if (HandleExpansion(datasize + len - 1))
	{
		if (datasize == 0)
			datasize++;
		for (unsigned long i = 0; i < len + 1; i++)
			((unsigned short *)arrayptr)[datasize - 1 + i] = str[i];
		datasize += len;
		return(true);
	}
	return(false);
}

//Concatenate string
bool EString::Cat(const WCHAR *str)
{
	unsigned long len = ClassEStd::StrLen(str);
	if (len == 0)
		return(false);
	if (HandleExpansion(datasize + len - 1))
	{
		if (datasize == 0)
			datasize++;
		for (unsigned long i = 0; i < len + 1; i++)
			((unsigned short *)arrayptr)[datasize - 1 + i] = str[i];
		datasize += len;
		return(true);
	}
	return(false);
}

//Concatenate string
bool EString::Cat(const EString &str)
{
	if (str.arrayptr == NULL)
		return(false);
	unsigned long len = str.datasize - 1;
	if (len == 0)
		return(false);
	if (HandleExpansion(datasize + len - 1))
	{
		if (datasize == 0)
			datasize++;
		for (unsigned long i = 0; i < len; i++)
			((unsigned short *)arrayptr)[datasize - 1 + i] = ((unsigned short *)str.arrayptr)[i];
		((unsigned short *)arrayptr)[datasize + len] = 0;
		datasize += len;
		return(true);
	}
	return(false);
}

//Concatenate string
bool EString::Cat(int value, unsigned char base, bool signedvalue)
{
	char tmpstr[20];
	ClassEStd::IntToStr(tmpstr, value, base, signedvalue);
	return(Cat(tmpstr));
}

//Concatenate string
bool EString::CatChr(WCHAR chr)
{
	if (HandleExpansion(datasize))
	{
		if (datasize == 0)
			datasize++;
		((unsigned short *)arrayptr)[datasize - 1] = chr;
		((unsigned short *)arrayptr)[datasize++] = 0;
		return(true);
	}
	return(false);
}

//Plus operator overload
EString &EString::operator+(const char *str)
{
	Cat(str);
	return(*this);
}

//Plus operator overload
EString &EString::operator+(const WCHAR *str)
{
	Cat(str);
	return(*this);
}

//Plus operator overload
EString &EString::operator+(const EString &str)
{
	Cat(str);
	return(*this);
}

//Plus operator overload
EString &EString::operator+(int value)
{
	Cat(value);
	return(*this);
}

//Assignment operator overload
EString &EString::operator=(const char *str)
{
	Copy(str);
	return(*this);
}

//Assignment operator overload
EString &EString::operator=(const WCHAR *str)
{
	Copy(str);
	return(*this);
}

//Assignment operator overload
EString &EString::operator=(const EString &str)
{
	if (&str != this)
		Copy(str);
	return(*this);
}

//Assignment operator overload
EString &EString::operator=(int value)
{
	Copy(value);
	return(*this);
}

//Subscript operator overload
unsigned short &EString::operator[](int i)
{
	dummy = 0;
	if (i < 0)
		return((unsigned short &)(dummy));
	if (arrayptr == NULL || i >= (long)datasize)
		return((unsigned short &)(dummy));
	return(((unsigned short *)(arrayptr))[i]);
}

//Extract operator overload
EString::operator WCHAR*()
{
	return(Get());
}
