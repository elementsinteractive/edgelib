// emod_register.cpp
// Register class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "estdc.h"
#include "ereg.h"
#include "ememory.h"
#include "evtimer.h"
#include "edevice.h"

#include "extra/verinfo.h"

/////////////////////////////////////////////////////////////////////
// ClassERegister::Private                                         //
/////////////////////////////////////////////////////////////////////

//Set the rpn string to lowercase
char *ClassERegister::LowerCase(char *str)
{
	for (unsigned short ctr = 0; ctr < 60000; ctr++)
	{
		if (str[ctr] >= 'A' && str[ctr] <= 'Z')
			str[ctr] += 32;
		if (str[ctr] == 0)
			break;
	}
	return(str);
}

//Parse the RPN script, return a value for every character
unsigned short ClassERegister::ParseRpn(char *rpnstr, unsigned char c, unsigned short i, unsigned short key)
{
	char subrpn[32];
	unsigned long operand[256];
	unsigned long rpnlen, parsepos = 0, stackpos = 0;
	rpnlen = ClassEStd::StrLen(rpnstr);
	while(parsepos < rpnlen && stackpos < 255)
	{
		if (rpnstr[parsepos] == '+')
		{
			if (stackpos > 1){ operand[stackpos - 2] += operand[stackpos - 1]; stackpos--; } else stackpos = 0;
		}
		else if (rpnstr[parsepos] == '-')
		{
			if (stackpos > 1){ operand[stackpos - 2] -= operand[stackpos - 1]; stackpos--; } else stackpos = 0;
		}
		else if (rpnstr[parsepos] == '*')
		{
			if (stackpos > 1){ operand[stackpos - 2] *= operand[stackpos - 1]; stackpos--; } else stackpos = 0;
		}
		else if (rpnstr[parsepos] == '/')
		{
			if (stackpos > 1){ operand[stackpos - 2] /= operand[stackpos - 1]; stackpos--; } else stackpos = 0;
		}
		else if (rpnstr[parsepos] == '%')
		{
			if (stackpos > 1){ operand[stackpos - 2] %= operand[stackpos - 1]; stackpos--; } else stackpos = 0;
		}
		else if (rpnstr[parsepos] == '&')
		{
			if (stackpos > 1){ operand[stackpos - 2] &= operand[stackpos - 1]; stackpos--; } else stackpos = 0;
		}
		else if (rpnstr[parsepos] == '|')
		{
			if (stackpos > 1){ operand[stackpos - 2] |= operand[stackpos - 1]; stackpos--; } else stackpos = 0;
		}
		else if (rpnstr[parsepos] == '~')
		{
			if (stackpos > 0)
				operand[stackpos - 1] = ~operand[stackpos - 1];
		}
		else if (rpnstr[parsepos] == '<' && parsepos < rpnlen - 1)
		{
			if (rpnstr[parsepos + 1] == '<' && stackpos > 1)
			{
				operand[stackpos - 2] <<= operand[stackpos - 1];
				stackpos--;
			}
		}
		else if (rpnstr[parsepos] == '>' && parsepos < rpnlen - 1)
		{
			if (rpnstr[parsepos + 1] == '>' && stackpos > 1)
			{
				operand[stackpos - 2] >>= operand[stackpos - 1];
				stackpos--;
			}
		}
		else if (rpnstr[parsepos] == '=' && parsepos < rpnlen - 1)
		{
			if (rpnstr[parsepos + 1] == '=' && stackpos > 1)
			{
				if (operand[stackpos - 2] == operand[stackpos - 1])
					operand[stackpos - 2] = 1;
				else
					operand[stackpos - 2] = 0;
				stackpos--;
			}
		}
		else if (rpnstr[parsepos] == '!' && parsepos < rpnlen - 1)
		{
			if (rpnstr[parsepos + 1] == '=' && stackpos > 1)
			{
				if (operand[stackpos - 2] != operand[stackpos - 1])
					operand[stackpos - 2] = 1;
				else
					operand[stackpos - 2] = 0;
				stackpos--;
			}
		}
		else if (rpnstr[parsepos] == 'c')
		{
			operand[stackpos] = c;
			stackpos++;
		}
		else if (rpnstr[parsepos] == 'i')
		{
			operand[stackpos] = i;
			stackpos++;
		}
		else
		{
			ClassEMemory::Set(subrpn, 0, 32);
			if (rpnlen - parsepos >= 3)
				ClassEMemory::Copy(subrpn, &rpnstr[parsepos], 3);
			if (ClassEStd::StrEqual(subrpn, "key", false))
			{
				operand[stackpos] = key;
				stackpos++;
				parsepos += 2;
			}
			else if (rpnstr[parsepos] >= '0' && rpnstr[parsepos] <= '9')
			{
				operand[stackpos] = rpnstr[parsepos] - 48;
				while(rpnstr[parsepos + 1] >= '0' && rpnstr[parsepos + 1] <= '9' && parsepos < rpnlen - 1)
				{
					operand[stackpos] *= 10;
					operand[stackpos] += rpnstr[parsepos + 1] - 48;
					parsepos++;
				}
				stackpos++;
			}
		}
		parsepos++;
	}
	if (stackpos > 0)
		stackpos--;
	return((unsigned short)operand[stackpos]);
}

/////////////////////////////////////////////////////////////////////
// ClassERegister::Public                                          //
/////////////////////////////////////////////////////////////////////

//ClassERegister: constructor
ClassERegister::ClassERegister(void)
{
	cdevice = NULL;
}

//ClassERegister: destructor
ClassERegister::~ClassERegister(void)
{
	if (cdevice != NULL)
	{
		delete cdevice;
		cdevice = NULL;
	}
}

//Calculate an unlock key based on the rpn string and the identifier
unsigned short ClassERegister::CalculateKey(const char *rpn, const char *id)
{
	char testrpn[512];
	unsigned short ctr, len, max, desiredkey = 0;
	ClassEStd::StrCpy(testrpn, rpn);
	LowerCase(testrpn);
	len = (unsigned short)ClassEStd::StrLen(id);
	if (len == 0)
		return(0);
	if (len < 5)
		max = len;
	else
		max = 5;
	for (ctr = 0; ctr < max; ctr++)
		desiredkey = ParseRpn(testrpn, id[ctr], ctr, desiredkey);
	if (len >= 5 && len < 10)
		max = len - 5;
	else if (len >= 10)
		max = 5;
	else
		max = 0;
	for (ctr = 0; ctr < max; ctr++)
		desiredkey = ParseRpn(testrpn, id[len - max + ctr], 5 + ctr, desiredkey);
	return(desiredkey);
}

//Calculate an unlock key based on the rpn string and the identifier
unsigned short ClassERegister::CalculateKey(const WCHAR *rpn, const WCHAR *id)
{
	unsigned short rval;
	CSTR_CHECK_BEGIN(rpn);
	CSTR_CHECK_BEGIN(id);
	rval = CalculateKey(cstr_rpn, cstr_id);
	CSTR_CHECK_END(rpn);
	CSTR_CHECK_END(id);
	return(rval);
}

//Encrypt an id to an unrecognizable string (12 characters); when the id is empty or NULL, the new id will be random
char *ClassERegister::EncryptId(char *newid, const char *id)
{
	char valstr[16];
	unsigned long ctr, len, value;
	ClassEStd::StrCpy(newid, "000000000000");
	if (id == NULL || id[0] == 0)
	{
		ClassERTimer tmr;
		ClassEStd::IntToStr(valstr, tmr.Count(), 10);
	}
	else
	{
		value = 1;
		len = ClassEStd::StrLen(id);
		for (ctr = 0; ctr < len; ctr++)
			value *= (id[ctr] + 2);
		value &= 0x2FFFFFFF;
		ClassEStd::IntToStr(valstr, value, 10);
	}
	len = ClassEStd::StrLen(valstr);
	if (len > 12)
		len = 12;
	ClassEMemory::Copy(&newid[12 - len], valstr, len);
	newid[12] = 0;
	return(newid);
}

//Encrypt an id to an unrecognizable string (12 characters); when the id is empty or NULL, the new id will be random
WCHAR *ClassERegister::EncryptId(WCHAR *newid, const WCHAR *id)
{
	char cstr_newid[MAX_ESTR];
	CSTR_CHECK_BEGIN(id);
	EncryptId(cstr_newid, cstr_id);
	ClassEStd::StrCpy(newid, cstr_newid);
	CSTR_CHECK_END(id);
	return(newid);
}

//Get the unique registration ID for selling portals (handango, etc.)
char *ClassERegister::GetRegistrationId(char *registrationid)
{
	WCHAR ustr_registrationid[MAX_ESTR];
	GetRegistrationId(ustr_registrationid);
	ClassEStd::StrCpy(registrationid, ustr_registrationid);
	return(registrationid);
}

//Get the unique registration ID for selling portals (handango, etc.)
WCHAR *ClassERegister::GetRegistrationId(WCHAR *registrationid)
{
	if (cdevice == NULL)
		cdevice = new ClassEDevice;
	if (cdevice != NULL)
	{
		#if defined(DEVICE_SYMBIAN)
			cdevice->GetIMEI(registrationid);
		#elif defined(DEVICE_GP2X)
			cdevice->GetSerialNumber(registrationid);
		#elif defined(DEVICE_IPHONE)
			cdevice->GetSerialNumber(registrationid);
		#else
			cdevice->GetOwnerName(registrationid);
		#endif
	}
	return(registrationid);
}
