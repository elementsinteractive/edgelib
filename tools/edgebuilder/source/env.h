// env.h
// Handles environment strings
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

#define MAX_ENV     1024

#define SU_NONE        0
#define SU_ALL         1

//Class definition
class ClassEnv : ClassEStd, ClassEMemory
{
	private:
		char empty[1];
		char *str_key[MAX_ENV];
		char *str_value[MAX_ENV];
		unsigned char str_usage[MAX_ENV];
		unsigned long numstring, stringpos;
	public:
		ClassEnv(void);
		~ClassEnv(void);
		void Clear(void);
		void Restart(void);
		void Dump(char *filename);
		long GetKeyIndex(char *key, unsigned long occ, bool wrapper = true);
		bool OTAdjustKeyValue(char *key, unsigned long occ, char *newvalue, bool wrapper = true);
		char *OTGetKeyValue(char *key, unsigned long occ, bool wrapper = true);
		bool OTSwapKeyValues(char *key, unsigned long occ1, unsigned long occ2, bool wrapper = true);
		bool OTRemoveKey(char *key, unsigned long occ, bool reorder = false, bool wrapper = true);
		bool ITAdjustKeyValue(long index, char *newvalue);
		char *ITGetKeyValue(long index);
		bool ITSwapKeyValues(long index1, long index2);
		bool ITRemoveKey(long index, bool reorder = false);
		unsigned long GetKeyCount(char *key = NULL, bool wrapper = true);
		long AddString(char *key, char *value);
		char *GetString(char *key, bool wrapper = true, bool restart = false);
		long SearchString(char *key, char *value = NULL, bool wrapper = true);
		void RemoveString(long nr);
		unsigned long CountKey(char *key = NULL, bool frompos = false, bool wrapper = false);
		void GetNextString(char *key, char *value);
		void CopyFrom(ClassEnv *env);
		void ScanString(char *str);
		void ScanFunction(char *str);
};

/////////////////////////////////////////////////////////////////////
// ClassEnv: private
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// ClassEnv: public
/////////////////////////////////////////////////////////////////////

//ClassEnv: constructor
ClassEnv::ClassEnv(void)
{
	unsigned long ctr;
	for (ctr = 0; ctr < MAX_ENV; ctr++)
	{
		str_key[ctr] = NULL;
		str_value[ctr] = NULL;
		str_usage[ctr] = SU_NONE;
	}
	empty[0] = 0;
	stringpos = 0;
	numstring = 0;
}

//ClassEnv destructor
ClassEnv::~ClassEnv(void)
{
	Clear();
}

//Clean resources
void ClassEnv::Clear(void)
{
	unsigned long ctr;
	for (ctr = 0; ctr < MAX_ENV; ctr++)
	{
		DeAlloc(str_key[ctr]);
		str_key[ctr] = NULL;
		DeAlloc(str_value[ctr]);
		str_value[ctr] = NULL;
		str_usage[ctr] = SU_NONE;
	}
	numstring = 0;
}

//Restart searching
void ClassEnv::Restart(void)
{
	stringpos = 0;
}

//Dump environment to file
void ClassEnv::Dump(char *filename)
{
	ClassEFile ef;
	if (ef.New(filename))
	{
		ef.Write("# Dumped environment:\r\n", StrLen("# Dumped environment:\r\n"));
		unsigned long ctr;
		for (ctr = 0; ctr < numstring; ctr++)
			if (str_usage[ctr] != SU_NONE)
			{
				ef.Write(str_key[ctr], StrLen(str_key[ctr]));
				ef.Write(": ", StrLen(": "));
				ef.Write(str_value[ctr], StrLen(str_value[ctr]));
				ef.Write("\r\n", StrLen("\r\n"));
			}
		ef.Close();
	}
}

//Convert an occurring key to an index
long ClassEnv::GetKeyIndex(char *key, unsigned long occ, bool wrapper)
{
	long ctr;
	char tmp_key[MAX_ESTR];
	StrCpy(tmp_key, key);
	if (wrapper)
	{
		tmp_key[0] = ' ';
		tmp_key[StrLen(tmp_key) - 1] = ' ';
		StrTrim(tmp_key);
	}
	for (ctr = 0; ctr < (long)numstring; ctr++)
		if (str_usage[ctr] == SU_ALL)
			if (StrEqual(str_key[ctr], tmp_key, false))
			{
				if (occ == 0)
					return(ctr);
				occ--;
			}
	return(-1);
}

//Occurrence type: Set a new key value string
bool ClassEnv::OTAdjustKeyValue(char *key, unsigned long occ, char *newvalue, bool wrapper)
{
	return(ITAdjustKeyValue(GetKeyIndex(key, occ, wrapper), newvalue));
}

//Occurrence type: Get a key value string
char *ClassEnv::OTGetKeyValue(char *key, unsigned long occ, bool wrapper)
{
	return(ITGetKeyValue(GetKeyIndex(key, occ, wrapper)));
}

//Occurrence type: swap 2 key values
bool ClassEnv::OTSwapKeyValues(char *key, unsigned long occ1, unsigned long occ2, bool wrapper)
{
	return(ITSwapKeyValues(GetKeyIndex(key, occ1, wrapper), GetKeyIndex(key, occ2, wrapper)));
}

//Occurrence type: remove a key
bool ClassEnv::OTRemoveKey(char *key, unsigned long occ, bool reorder, bool wrapper)
{
	return(ITRemoveKey(GetKeyIndex(key, occ, wrapper), reorder));
}

//Index type: Set a new key value string
bool ClassEnv::ITAdjustKeyValue(long index, char *newvalue)
{
	if (index >= 0 && index < (long)numstring)
		if (str_usage[index] == SU_ALL)
		{
			if (str_value[index] != NULL)
			{
				DeAlloc(str_value[index]);
				str_value[index] = NULL;
			}
			str_value[index] = (char *)Alloc(StrLen(newvalue) + 2);
			if (str_value[index] == NULL)
				return(false);
			StrCpy(str_value[index], newvalue);
			return(true);
		}
	return(false);
}

//Index type: Get a key value string
char *ClassEnv::ITGetKeyValue(long index)
{
	if (index >= 0 && index < (long)numstring)
		if (str_usage[index] == SU_ALL)
			return(str_value[index]);
	return(empty);
}

//Index type: Swap 2 key values
bool ClassEnv::ITSwapKeyValues(long index1, long index2)
{
	if (index1 != index2)
		if (index1 >= 0 && index1 < (long)numstring && index2 >= 0 && index2 < (long)numstring)
			if (str_usage[index1] == SU_ALL && str_usage[index2] == SU_ALL)
			{
				char tmpval1[MAX_ESTR], tmpval2[MAX_ESTR];
				StrCpy(tmpval1, str_value[index1]);
				StrCpy(tmpval2, str_value[index2]);
				if (!ITAdjustKeyValue(index1, tmpval2))
					str_usage[index1] = SU_NONE;
				if (!ITAdjustKeyValue(index2, tmpval1))
					str_usage[index2] = SU_NONE;
				if (str_usage[index1] == SU_NONE || str_usage[index2] == SU_NONE)
					return(false);
				return(true);
			}
	return(false);
}

//Index type: remove a key
bool ClassEnv::ITRemoveKey(long index, bool reorder)
{
	if (index >= 0 && index < MAX_ENV)
		str_usage[index] = SU_NONE;
	else
		return(false);
	if (reorder)
	{
		unsigned long ctr, ctr2, nextkey, lastkey = 0;
		for (ctr = 0; ctr < numstring; ctr++)
		{
			if (str_usage[ctr] == SU_NONE)
			{
				nextkey = numstring;
				for (ctr2 = ctr + 1; ctr2 < numstring; ctr2++)
					if (str_usage[ctr2] != SU_NONE)
					{
						nextkey = ctr2;
						break;
					}
				if (nextkey < numstring)
				{
					char *tmpstr;
					unsigned char tmpusage;
					tmpstr = str_key[ctr];
					str_key[ctr] = str_key[nextkey];
					str_key[nextkey] = tmpstr;
					tmpstr = str_value[ctr];
					str_value[ctr] = str_value[nextkey];
					str_value[nextkey] = tmpstr;
					tmpusage = str_usage[ctr];
					str_usage[ctr] = str_usage[nextkey];
					str_usage[nextkey] = tmpusage;
					lastkey = ctr + 1;
				}
				else
					break;
			}
			else
				lastkey = ctr + 1;
		}
		numstring = lastkey;
	}
	return(true);
}

//Count the number of keys
unsigned long ClassEnv::GetKeyCount(char *key, bool wrapper)
{
	char tmp_key[MAX_ESTR];
	unsigned long ctr, count = 0;
	if (key != NULL)
	{
		StrCpy(tmp_key, key);
		if (wrapper)
		{
			tmp_key[0] = ' ';
			tmp_key[StrLen(tmp_key) - 1] = ' ';
			StrTrim(tmp_key);
		}
	}
	for (ctr = 0; ctr < numstring; ctr++)
		if (str_usage[ctr] != SU_NONE)
		{
			if (key == NULL)
				count++;
			else if (StrEqual(str_key[ctr], tmp_key, false))
				count++;
		}
	return(count);
}

//Add a new string
long ClassEnv::AddString(char *key, char *value)
{
	unsigned long ctr, target = numstring;
	for (ctr = 0; ctr < numstring; ctr++)
		if (str_usage[ctr] == SU_NONE)
		{
			target = ctr;
			break;
		}
	if (target < MAX_ENV)
	{
		if (str_key[target] != NULL)
		{
			DeAlloc(str_key[target]);
			str_key[target] = NULL;
		}
		if (str_value[target] != NULL)
		{
			DeAlloc(str_value[target]);
			str_value[target] = NULL;
		}
		str_key[target]   = (char *)Alloc(StrLen(key) + 2);
		str_value[target] = (char *)Alloc(StrLen(value) + 2);
		if (str_key[target] == NULL || str_value[target] == NULL)
		{
			if (str_key[target] != NULL)
			{
				DeAlloc(str_key[target]);
				str_key[target] = NULL;
			}
			if (str_value[target] != NULL)
			{
				DeAlloc(str_value[target]);
				str_value[target] = NULL;
			}
			return(-1);
		}
		StrCpy(str_key[target], key);
		StrCpy(str_value[target], value);
		str_usage[target] = SU_ALL;
		if (target >= numstring)
			numstring = target + 1;
		return(target);
	}
	return(-1);
}

//Search for a key string
char *ClassEnv::GetString(char *key, bool wrapper, bool restart)
{
	char tmp_key[MAX_ESTR];
	unsigned long ctr;
	if (restart)
		Restart();
	StrCpy(tmp_key, key);
	if (wrapper)
	{
		tmp_key[0] = ' ';
		tmp_key[StrLen(tmp_key) - 1] = ' ';
		StrTrim(tmp_key);
	}
	for (ctr = stringpos; ctr < numstring; ctr++)
		if (str_usage[ctr] == SU_ALL)
			if (StrEqual(str_key[ctr], tmp_key, false))
			{
				stringpos = ctr + 1;
				return(str_value[ctr]);
			}
	return(empty);
}

//Search for an environment string
long ClassEnv::SearchString(char *key, char *value, bool wrapper)
{
	char tmp_key[MAX_ESTR];
	unsigned long ctr;
	StrCpy(tmp_key, key);
	if (wrapper)
	{
		tmp_key[0] = ' ';
		tmp_key[StrLen(tmp_key) - 1] = ' ';
		StrTrim(tmp_key);
	}
	for (ctr = 0; ctr < numstring; ctr++)
		if (str_usage[ctr] == SU_ALL)
			if (StrEqual(str_key[ctr], tmp_key, false))
			{
				if (value == NULL)
					return(ctr);
				else if (StrEqual(str_value[ctr], value, false))
					return(ctr);
			}
	return(-1);
}

//Remove a string
void ClassEnv::RemoveString(long nr)
{
	if (nr >= 0 && nr < MAX_ENV)
		str_usage[nr] = SU_NONE;
}

//Copy everything from another environment
void ClassEnv::CopyFrom(ClassEnv *env)
{
	unsigned long ctr, ctr2, index = 0;
	for (ctr = 0; ctr < MAX_ENV; ctr++)
		if (ctr >= numstring || str_usage[ctr] == SU_NONE)
		{
			for (ctr2 = index; ctr2 < MAX_ENV; ctr2++)
				if (env->str_usage[ctr2] == SU_ALL)
				{
					if (str_key[ctr] != NULL)
					{
						DeAlloc(str_key[ctr]);
						str_key[ctr] = NULL;
					}
					if (str_value[ctr] != NULL)
					{
						DeAlloc(str_value[ctr]);
						str_value[ctr] = NULL;
					}
					str_key[ctr]   = (char *)Alloc(StrLen(env->str_key[ctr2]) + 2);
					str_value[ctr] = (char *)Alloc(StrLen(env->str_value[ctr2]) + 2);
					if (str_key[ctr] == NULL || str_value[ctr] == NULL)
					{
						if (str_key[ctr] != NULL)
						{
							DeAlloc(str_key[ctr]);
							str_key[ctr] = NULL;
						}
						if (str_value[ctr] != NULL)
						{
							DeAlloc(str_value[ctr]);
							str_value[ctr] = NULL;
						}
						break;
					}
					StrCpy(str_key[ctr], env->str_key[ctr2]);
					StrCpy(str_value[ctr], env->str_value[ctr2]);
					str_usage[ctr] = SU_ALL;
					if (ctr >= numstring)
						numstring = ctr + 1;
					index = ctr2 + 1;
					break;
				}
		}
}

//Scan for amount of duplicate keys
unsigned long ClassEnv::CountKey(char *key, bool frompos, bool wrapper)
{
	char tmp_key[MAX_ESTR];
	unsigned long ctr, count = 0, start = 0;
	if (key != NULL)
	{
		StrCpy(tmp_key, key);
		if (wrapper)
		{
			tmp_key[0] = ' ';
			tmp_key[StrLen(tmp_key) - 1] = ' ';
			StrTrim(tmp_key);
		}
	}
	if (frompos)
		start = stringpos;
	for (ctr = start; ctr < numstring; ctr++)
		if (str_usage[ctr] != SU_NONE)
		{
			if (key == NULL)
				count++;
			else if (StrEqual(str_key[ctr], tmp_key, false))
				count++;
		}
	return(count);
}

//Get the next string value from the environment
void ClassEnv::GetNextString(char *key, char *value)
{
	unsigned long ctr;
	for (ctr = stringpos; ctr < numstring; ctr++)
		if (str_usage[ctr] == SU_ALL)
		{
			StrCpy(key, str_key[ctr]);
			StrCpy(value, str_value[ctr]);
			stringpos = ctr + 1;
			return;
		}
}

//Scan a string for environment variables and replace them
void ClassEnv::ScanString(char *str)
{
	char *replacestr;
	char searchstr[MAX_ESTR];
	unsigned long curindexctr = 0;
	unsigned char symbolfound;
	long symbolstart, ctr, len;
	bool envfound;
	do
	{
		envfound = false;
		symbolfound = 0;
		len = StrLen(str);
		for (ctr = 0; ctr < len; ctr++)
		{
			if (str[ctr] == '%')
			{
				symbolfound++;
				if (symbolfound == 1)
					symbolstart = ctr;
				if (symbolfound == 2 && symbolstart == ctr - 1)
				{
					symbolfound = 0;
					ctr++;
				}
				else if (symbolfound >= 2)
				{
					bool nextreplace;
					Copy(searchstr, &str[symbolstart], ctr - symbolstart + 1);
					searchstr[ctr - symbolstart + 1] = 0;
					Restart();
					do
					{
						bool specialfound = false;
						nextreplace = false;
						replacestr = GetString(searchstr);
						if (StrEqual(replacestr, empty, false))
						{
							unsigned long keyfound = CountKey(searchstr, false, true);
							if (keyfound == 0)
							{
								if (StrLen(searchstr) > 6)
									if (searchstr[2] == '_' && searchstr[StrLen(searchstr) - 3] == '_' && searchstr[1] == searchstr[StrLen(searchstr) - 2])
									{
										char basekey[MAX_ESTR], segkey[MAX_ESTR];
										char newreplace[MAX_ESTR * 32];
										switch(searchstr[1])
										{
											case 'l':
												unsigned long listcount;
												StrCpy(basekey, &searchstr[3]);
												basekey[StrLen(basekey) - 3] = 0;
												listcount = CountKey(basekey);
												if (listcount > 0)
												{
													StrCpy(newreplace, "");
													for (unsigned long listctr = 0; listctr < listcount; listctr++)
													{
														StrCpy(segkey, basekey);
														if (listctr == 0)
															StrCat(segkey, "_begin");
														else
															StrCat(segkey, "_next");
														StrCat(newreplace, GetString(segkey, false, true));
													}
													StrCpy(segkey, basekey);
													StrCat(segkey, "_end");
													StrCat(newreplace, GetString(segkey, false, true));
													if (StrReplace(str, searchstr, newreplace, false, 1) > 0)
														nextreplace = true;
													specialfound = true;
												}
												break;
											case 'i':
												long index;
												StrCpy(segkey, &searchstr[3]);
												StrCpy(basekey, &searchstr[5]);
												basekey[StrLen(basekey) - 3] = 0;
												segkey[2] = 0;
												index = StrToInt(segkey);
												Restart();
												for (long indexctr = 0; indexctr <= index; indexctr++)
													replacestr = GetString(basekey, false);
												break;
										}
									}
								if (StrEqual(searchstr, "%indexcounter%"))
								{
									char replacectr[16];
									IntToStr(replacectr, curindexctr);
									if (StrReplace(str, searchstr, replacectr, false, 1) > 0)
									{
										curindexctr++;
										nextreplace = true;
									}
									specialfound = true;
								}
							}
							else if (keyfound == 1)
								replacestr = GetString(searchstr, true, true);
						}
						if (!specialfound)
						{
							if (StrReplace(str, searchstr, replacestr, false, 1) > 0)
								nextreplace = true;
						}
					}while(nextreplace);
					symbolfound = 0;
					ctr = symbolstart - 1;
					len = StrLen(str);
					envfound = true;
				}
			}
		}
	}while(envfound);
	StrReplace(str, "%%", "%");
	ScanFunction(str);
}

//Scan for functions
void ClassEnv::ScanFunction(char *str)
{
	char tmpstr[8192], replacestr[8192];
	char *func_str;
	bool busy;
	long ctr, ctr2, pos, end, len;
	do
	{
		char func_name[MAX_ESTR];
		busy = false;
		pos = -1;
		len = ClassEStd::StrLen(str);
		for (ctr = 0; ctr < len; ctr++)
			if (str[ctr] == '[')
			{
				for (ctr2 = ctr + 1; ctr2 < len; ctr2++)
				{
					if (str[ctr2] == ' ' || str[ctr2] == '[' || str[ctr2] == ']')
						break;
					if (str[ctr2] == '(')
					{
						pos = ctr;
						end = ctr2;
						ctr = ctr2;
						break;
					}
				}
			}
		if (pos >= 0)
		{
			ClassEStd::StrCpy(func_name, &str[pos], end - pos + 1);
			func_str = &str[pos + StrLen(func_name) + 1];
			end = StrFind(func_str, ")]");
			if (end >= 0)
			{
				if (ClassEStd::StrEqual(func_name, "[file_linux"))
				{
					if (func_str[1] == ':')
					{
						StrCpy(tmpstr, "%mountprepend%");
						ScanString(tmpstr);
					}
					else
						StrCpy(tmpstr, "");
					StrCpy(replacestr, func_str, end + 1);
					StrCat(tmpstr, replacestr);
					StrReplace(tmpstr, "\\", "/");
					StrReplace(tmpstr, " ", "\\ ");
				}
				else if (ClassEStd::StrEqual(func_name, "[lowercase"))
				{
					ClassEStd::StrCpy(tmpstr, func_str, end + 1);
					ClassEStd::StrLower(tmpstr);
				}
				else if (ClassEStd::StrEqual(func_name, "[remove"))
				{
					char removechar[16];
					removechar[0] = 0;
					ClassEStd::StrCpy(tmpstr, func_str, end + 1);
					len = ClassEStd::StrLen(tmpstr);
					for (ctr = 0; ctr < len; ctr++)
					{
						if (ctr > 14)
							break;
						removechar[ctr] = tmpstr[ctr];
						removechar[ctr + 1] = 0;
						if (tmpstr[ctr] == '@')
							break;
					}
					if (removechar[ClassEStd::StrLen(removechar) - 1] == '@')
					{
						ClassEStd::StrCpy(tmpstr, &tmpstr[ClassEStd::StrLen(removechar)]);
						len = ClassEStd::StrLen(removechar);
						for (ctr = 0; ctr < len - 1; ctr++)
						{
							replacestr[0] = removechar[ctr];
							replacestr[1] = 0;
							ClassEStd::StrReplace(tmpstr, replacestr, "");
						}
					}
				}
				else
					ClassEStd::StrCpy(tmpstr, func_str, end + 1);
			}
			StrCpy(replacestr, &str[pos], StrLen(func_name) + end + 4);
			StrReplace(str, replacestr, tmpstr);
			busy = true;
		}
	}
	while (busy);
}
