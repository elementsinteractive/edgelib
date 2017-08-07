// dataapi.h
// Extended API's related to data and strings
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED     //Can only be included by the main engine
#ifndef EDATAPI_INCLUDED  //Make sure the API definition file isn't included multiple times
#define EDATAPI_INCLUDED


///////////////////////////////////////////////////////////////////
// Classes
///////////////////////////////////////////////////////////////////

class EData
{
	protected:
		void *arrayptr;
		unsigned char indexsize;
		unsigned long allocstep;
		unsigned long arraysize;
		unsigned long datasize;
		unsigned long dummy;
		bool allowexpansion;
		bool indexexpansion;
		bool HandleExpansion(unsigned long lastindex);
		void Init(void);
	public:
		EData(void);
		EData(void *data, unsigned long length);
		virtual ~EData(void);
		bool Add(void *data, unsigned long length);
		unsigned long GetDataSize(void); // typo
		unsigned long GetDatasize(void) { return GetDataSize(); } // Backwards compatibility with typo
		void *Get(unsigned long index = 0);
		void Configure(bool allowexpansion = true, bool indexexpansion = false, unsigned long allocstep = 256);
		void Trunc(unsigned long length);
		unsigned char &operator[](int i);
};//EDGELIB data class

class EData16 : public EData
{
	public:
		EData16(void);
		EData16(void *data, unsigned long length);
		~EData16(void);
		virtual unsigned short &operator[](int i);
};//EDGELIB data class (16-bit indexing)

class EData32 : public EData
{
	public:
		EData32(void);
		EData32(void *data, unsigned long length);
		~EData32(void);
		virtual unsigned long &operator[](int i);
};//EDGELIB data class (32-bit indexing)

class EString : public EData16
{
	public:
		EString(void);
		EString(const char *str);
		EString(const WCHAR *str);
		EString(const EString &str);
		~EString(void);
		void Copy(const char *str);
		void Copy(const WCHAR *str);
		void Copy(const EString &str);
		void Copy(int value, unsigned char base = 10, bool signedvalue = true);
		WCHAR *Get(void);
		WCHAR Chr(unsigned long index);
		void SetChr(unsigned long index, WCHAR chr);
		void SubStr(EString &str, long start, long length = 0);
		EString &SubStr(long start, long length = 0);
		void Trunc(unsigned long length);
		unsigned long Length(void);
		bool Cat(const char *str);
		bool Cat(const WCHAR *str);
		bool Cat(const EString &str);
		bool Cat(int value, unsigned char base = 10, bool signedvalue = true);
		bool CatChr(WCHAR chr);
		EString &operator+(const char *str);
		EString &operator+(const WCHAR *str);
		EString &operator+(const EString &str);
		EString &operator+(int value);
		EString &operator=(const char *str);
		EString &operator=(const WCHAR *str);
		EString &operator=(const EString &str);
		EString &operator=(int value);
		unsigned short &operator[](int i);
		operator WCHAR*();
};//EDGELIB string class (unicode compatible)

#endif
#endif
