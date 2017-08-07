// ereg.h
// A register module for handling game registration using the RPN method
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED  //Can only be included by the main engine
#ifndef EREG_INCLUDED  //Make sure the module isn't included multiple times
#define EREG_INCLUDED

///////////////////////////////////////////////////////////////////
// Classes                                                       //
///////////////////////////////////////////////////////////////////

class ClassERegister
{
	private:
		ClassEDevice *cdevice;
		char *LowerCase(char *str);
		unsigned short ParseRpn(char *rpnstr, unsigned char c, unsigned short i, unsigned short key);
	public:
		ClassERegister(void);
		~ClassERegister(void);
		unsigned short CalculateKey(const char *rpn, const char *id);
		unsigned short CalculateKey(const WCHAR *rpn, const WCHAR *id);
		char *EncryptId(char *newid, const char *id);
		WCHAR *EncryptId(WCHAR *newid, const WCHAR *id);
		char *GetRegistrationId(char *registrationid);
		WCHAR *GetRegistrationId(WCHAR *registrationid);
};//The registration class

#endif
#endif
