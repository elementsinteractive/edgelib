// ini.h
// Handles reading/writing of Edge ini files
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

#define MAX_FILEBUF 1024
#define MAX_INILINE  512

//Class definition
class ClassIni
{
	private:
		ClassEFile ef;
		unsigned char filebuf[MAX_FILEBUF];
		unsigned long filebufpos, filesize, filepos;
		bool opened;
		bool inline IsWhiteSpace(char chr);
		bool inline IsNewline(char chr);
		bool inline IsComment(char chr);
		bool inline IsQuote(char chr);
		bool inline IsSeperator(char chr);
		bool HandleFilebuf(void);
	public:
		ClassIni(void);
		~ClassIni(void);
		bool Open(char *name, unsigned long flags);
		void Close(void);
		bool ReadLine(char *key, char *value);
		bool WriteLine(char *key, char *value);
};

/////////////////////////////////////////////////////////////////////
// ClassIni: private
/////////////////////////////////////////////////////////////////////

//Return if the character is a space seperator
bool ClassIni::IsWhiteSpace(char chr)
{
	return(chr == ' ' || chr == '\t' || chr == '\n' || chr == '\r' || chr == '\f' || chr == '\x0b');
}

//Return if the character is a new line
bool ClassIni::IsNewline(char chr)
{
	return(chr == '\r' || chr == '\n');
}

//Return if the character is a comment tag
bool ClassIni::IsComment(char chr)
{
	return(chr == ';' || chr == '#');
}

//Return if the character is a quote
bool ClassIni::IsQuote(char chr)
{
	return(chr == '\"');
}

//Return if the character is a seperator for assigning values
bool ClassIni::IsSeperator(char chr)
{
	return(chr == '=');
}

//Handle the file buffer
bool ClassIni::HandleFilebuf(void)
{
	if (filebufpos < MAX_FILEBUF)
		return(true);
	if (filepos == filesize)
		return(false);
	if (filepos + MAX_FILEBUF <= filesize)
	{
		if (ef.Read(filebuf, MAX_FILEBUF))
		{
			filepos += MAX_FILEBUF;
			filebufpos -= MAX_FILEBUF;
			return(true);
		}
	}
	else
	{
		if (ef.Read(&filebuf[MAX_FILEBUF - (filesize - filepos)], filesize - filepos))
		{
			filebufpos = MAX_FILEBUF - (filesize - filepos);
			filepos = filesize;
			return(true);
		}
	}
	return(false);
}

/////////////////////////////////////////////////////////////////////
// ClassIni: public
/////////////////////////////////////////////////////////////////////

//ClassMain: constructor
ClassIni::ClassIni(void)
{
	opened = false;
}

//ClassMain destructor
ClassIni::~ClassIni(void)
{
}

//Open an ini
bool ClassIni::Open(char *name, unsigned long flags)
{
	opened = ef.Open(name);
	filebufpos = MAX_FILEBUF;
	filesize = ef.Size();
	filepos = 0;
	return(opened);
}

//Close an ini
void ClassIni::Close(void)
{
	ef.Close();
}

//Read a line from the ini
bool ClassIni::ReadLine(char *key, char *value)
{
	key[0] = 0;
	value[0] = 0;
	if (opened && (filepos < filesize || filebufpos < MAX_FILEBUF))
	{
		bool isparsing = false, oncomment = false, onquote = false, onvalue = false;
		unsigned long lpos = 0;
		for (unsigned long ctr = 0; ctr < MAX_INILINE; ctr++)
		{
			if (filebufpos >= MAX_FILEBUF)
				if (!HandleFilebuf())
					break;
			if (IsNewline(filebuf[filebufpos]))
			{
				if (isparsing)
				{
					filebufpos++;
					break;
				}
				else
					oncomment = false;
			}
			if (IsComment(filebuf[filebufpos]))
				oncomment = true;
			if (!oncomment)
			{
				if (IsWhiteSpace(filebuf[filebufpos]))
					isparsing = true;
				if (IsQuote(filebuf[filebufpos]))
					onquote = !onquote;
				if (!onquote && IsSeperator(filebuf[filebufpos]))
				{
					lpos = 0;
					onvalue = true;
				}
				else
				{
					if (onvalue)
					{
						value[lpos] = filebuf[filebufpos];
						value[lpos + 1] = 0;
					}
					else
					{
						key[lpos] = filebuf[filebufpos];
						key[lpos + 1] = 0;
					}
					lpos++;
				}
			}
			filebufpos++;
		}
		ClassEStd::StrTrim(key);
		ClassEStd::StrTrim(value);
		if (IsQuote(key[0]) && IsQuote(key[ClassEStd::StrLen(key) - 1]))
		{
			unsigned long slen = ClassEStd::StrLen(key);
			for (ctr = 0; ctr < slen - 1; ctr++)
				key[ctr] = key[ctr + 1];
			key[slen - 2] = 0;
		}
		if (IsQuote(value[0]) && IsQuote(value[ClassEStd::StrLen(value) - 1]))
		{
			unsigned long slen = ClassEStd::StrLen(value);
			for (ctr = 0; ctr < slen - 1; ctr++)
				value[ctr] = value[ctr + 1];
			value[slen - 2] = 0;
		}
		if (isparsing)
			return(true);
	}
	return(false);
}

//Read a line to the ini
bool ClassIni::WriteLine(char *key, char *value)
{
	if (opened)
	{
		char seperator[] = " = ";
		char quote[] = "\"";
		char newline[] = "\r\n";
		if (filesize >= 2)
		{
			char tmp[2];
			ef.Seek(filesize - 2);
			ef.Read(tmp, 2);
			ef.Seek(filesize);
			if (tmp[0] != '\n' && tmp[1] != '\n')
				ef.Write(newline, 2);
		}
		else
			ef.Seek(filesize);
		ef.Write(key, ClassEStd::StrLen(key));
		ef.Write(seperator, 3);
		ef.Write(quote, 1);
		ef.Write(value, ClassEStd::StrLen(value));
		ef.Write(quote, 1);
		ef.Write(newline, 2);
	}
	return(false);
}
