// extapi_xml.cpp
// XML class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#include "extra/verinfo.h"

#define EDGE_INCLUDED
#include "estdc.h"
#include "ememory.h"
#include "econsole.h"

#include "extapi/dataapi.h"
#include "extapi/xmlapi.h"

///////////////////////////////////////////////////////////////////
// XML parser definitions
///////////////////////////////////////////////////////////////////

//Token states
#define EXMLTKN_PROCINSTRUCTION     0
#define EXMLTKN_COMMENT             1
#define EXMLTKN_DOCTYPE             2
#define EXMLTKN_TAGBEGINOPEN        3
#define EXMLTKN_TAGBEGINCLOSE       4
#define EXMLTKN_TAGEND              5
#define EXMLTKN_CONTENT             6
#define EXMLTKN_ATTRIBUTE           7
#define EXMLTKN_UNKNOWN             8

//Size of node, aligned per 4-byte boundary
#define ALIGN_RPCNODESIZE (sizeof(EXmlRpcNode) + (4 - sizeof(EXmlRpcNode) % 4))
#define ALIGN_NODESIZE (sizeof(EXmlNode) + (4 - sizeof(EXmlNode) % 4))
#define ALIGN_ATTRIBUTESIZE (sizeof(EXmlAttribute) + (4 - sizeof(EXmlAttribute) % 4))

//XMLRPC parser method
bool XmlRpcCallback(ClassEXml *parser, unsigned char event, const WCHAR *name, const WCHAR *attribute, const WCHAR *value, void *userdata);

/////////////////////////////////////////////////////////////////////
//EXmlAttribute
/////////////////////////////////////////////////////////////////////

//EXmlAttribute: constructor
EXmlAttribute::EXmlAttribute()
{
}

//EXmlAttribute: destructor
EXmlAttribute::~EXmlAttribute()
{
}

//returns the next attribute, if there isn't a next attribute it returns a dummyattribute
EXmlAttribute &EXmlAttribute::GetNextAttribute()
{
	if (nextattribute == 0xFFFFFFFF)
		return(xmlParent->dummyattribute);
	return(*(EXmlAttribute *)xmlParent->data.Get(nextattribute));
}

//returns the name of the attribute
WCHAR *EXmlAttribute::GetName()
{
	return((unsigned short*)xmlParent->data.Get(dataIndex + ALIGN_ATTRIBUTESIZE));
}

//returns the value of the attribute
WCHAR *EXmlAttribute::GetValue()
{
	return((unsigned short*)xmlParent->data.Get(dataIndex + ALIGN_ATTRIBUTESIZE + nameLength));
}

/////////////////////////////////////////////////////////////////////
//EXmlNode
/////////////////////////////////////////////////////////////////////

//EXmlNode: constructor
EXmlNode::EXmlNode()
{
}

//EXmlNode: destructor
EXmlNode::~EXmlNode()
{
}

//Returns the name of the node
WCHAR *EXmlNode::GetName(void)
{
	return((unsigned short*)xmlParent->data.Get(dataIndex + ALIGN_NODESIZE));
}

//Returns the value of the node
WCHAR *EXmlNode::GetValue(void)
{
	if (valueIndex >= 0 && valueIndex != 0xFFFFFFFF)
		return((unsigned short*)xmlParent->data.Get(valueIndex));
	return((unsigned short*)xmlParent->data.Get(xmlParent->emptyString));
}

//Returns the parent node of the node
EXmlNode &EXmlNode::GetParent(void)
{
	if (parent == 0xFFFFFFFF)
		return(xmlParent->dummynode);
	return(*(EXmlNode*) xmlParent->data.Get(parent));
}

//Returns the child node of the node if it has one, else it will return a dummynode
EXmlNode &EXmlNode::GetChild(void)
{
	if (child == 0xFFFFFFFF)
		return(xmlParent->dummynode);
	return(*(EXmlNode*) xmlParent->data.Get(child));
}

//Returns the previous node on the same level as the node, if it doesn't have one it will return a dummynode
EXmlNode &EXmlNode::GetPreviousSibling(void)
{
	if (prvsibling == 0xFFFFFFFF)
		return(xmlParent->dummynode);
	return(*(EXmlNode*)xmlParent->data.Get(prvsibling));
}

//Returns the next node on the same level as the node, if it doesn't have one it will return a dummynode
EXmlNode &EXmlNode::GetNextSibling(void)
{
	if (nxtsibling == 0xFFFFFFFF)
		return(xmlParent->dummynode);
	return(*(EXmlNode*) xmlParent->data.Get(nxtsibling));
}

//Gets the node by index
EXmlNode &EXmlNode::Get(int i)
{
	if (child != 0xFFFFFFFF)
	{
		EXmlNode *n = &this->GetChild();
		int ctr = 0;
		while (ctr <= i)
		{
			if (ctr == i)
				return(*n);
			if (n->nxtsibling != 0xFFFFFFFF)
				n = &n->GetNextSibling();
			else
				break;
			ctr++;
		}
	}
	return(xmlParent->dummynode);
}

//Gets the node by name
EXmlNode &EXmlNode::Get(const char *key)
{
	if (child != 0xFFFFFFFF)
	{
		EXmlNode *n = &this->GetChild();
		while (true)
		{
			if (ClassEStd::StrEqual(n->GetName(), key))
				return *n;
			if (n->nxtsibling != 0xFFFFFFFF && n->nxtsibling != n->dataIndex)
				n = &n->GetNextSibling();
			else
				break;
		}
	}
	return(xmlParent->dummynode);
}

//Overloaded subscript operator (index)
EXmlNode &EXmlNode::operator [](int i)
{
	return(Get(i));
}

//Overloaded subscript operator (string)
EXmlNode &EXmlNode::operator [](const char *key)
{
	return(Get(key));
}

//Returns the last attribute of the node, if it doesn't have any attributes it will return a dummyattribute
EXmlAttribute &EXmlNode::GetLastAttribute()
{
	EXmlAttribute *result = &xmlParent->dummyattribute;
	unsigned long id = 0;
	id = CountAttributes();
	if (id > 0)
		result = &GetAttribute(id - 1);
	return(*result);
}

//Returns the attribute with the given name, if it doesn't exist it will return a dummyattribute
EXmlAttribute &EXmlNode::GetAttribute(const char *name)
{
	EXmlAttribute *result;
	if (attributeIndex == 0xFFFFFFFF)
		result = &xmlParent->dummyattribute;
	else
	{
		bool ready = false;
		result = (EXmlAttribute*) xmlParent->data.Get(attributeIndex);
		while(!ready)
		{
			if (ClassEStd::StrEqual(name, result->GetName()))
			{
				ready = true;
			}
			else
			{
				result = &result->GetNextAttribute();
				if (result == &xmlParent->dummyattribute)
					ready = true;
			}
		}
	}
	return (*result);
}

//Returns the attribute with the given name, if it doesn't exist it will return a dummyattribute
EXmlAttribute &EXmlNode::GetAttribute(int id)
{
	EXmlAttribute *result;
	if (attributeIndex == 0xFFFFFFFF)
		result = &xmlParent->dummyattribute;
	else
	{
		bool ready = false;
		result = (EXmlAttribute*) xmlParent->data.Get(attributeIndex);
		int ctr = 0;
		while(!ready)
		{
			if (id == ctr)
			{
				ready = true;
			}
			else
			{
				ctr++;
				result = &result->GetNextAttribute();
				if (result == &xmlParent->dummyattribute)
					ready = true;
			}
		}
	}
	return (*result);
}

//Counts the number of child nodes the node has
unsigned long EXmlNode::CountChildNodes()
{
	unsigned long ctr = 0;
	EXmlNode n = GetChild();
	if (&GetChild() != &xmlParent->dummynode)
		ctr++;
	else
		return(0);
	while (n.nxtsibling != 0xFFFFFFFF)
	{
		n = n.GetNextSibling();
		ctr++;
	}
	return(ctr);
}

//Counts the number of attributes the node has
unsigned long EXmlNode::CountAttributes()
{
	unsigned long ctr = 0;
	EXmlAttribute *result;
	if (attributeIndex != 0xFFFFFFFF)
	{
		bool ready = false;
		result = (EXmlAttribute*) xmlParent->data.Get(attributeIndex);
		ctr++;
		while(!ready)
		{
			result = &result->GetNextAttribute();
			if (result == &xmlParent->dummyattribute)
				ready = true;
			else
				ctr++;
		}
	}
	return(ctr);
}
//Get the name of the attribute by id
WCHAR* EXmlNode::GetAttributeName(int id)
{
	EXmlAttribute attribute = GetAttribute(id);
	return(attribute.GetName());
}

//Get the value of the attribute by id
WCHAR* EXmlNode::GetAttributeValue(int id)
{
	EXmlAttribute attribute = GetAttribute(id);
	return(attribute.GetValue());
}

//Get the value of the attribute by name
WCHAR* EXmlNode::GetAttributeValue(const char *name)
{
	EXmlAttribute attribute = GetAttribute(name);
	return(attribute.GetValue());
}

///////////////////////////////////////////////////////////////////
// Main XML parser
///////////////////////////////////////////////////////////////////

//Peeks character at specific position
unsigned long ClassEXml::PeekInputCharAt(unsigned long offset)
{
	if (offset >= textlength)
		return(0);
	unsigned char unichar = (unsigned char)loadedtext[offset];
	if (unichar >= 0x80)
	{
		if ((unichar & 0xF0) == 0xF0)
		{
			if (offset + 3 >= textlength)
				return(0);
			return(((loadedtext[offset] & 0x7) << 18) | ((loadedtext[offset + 1] & 0x3F) << 12) | ((loadedtext[offset + 2] & 0x3F) << 6) | ((loadedtext[offset + 3] & 0x3F)));
		}
		else if ((unichar & 0xE0) == 0xE0)
		{
			if (offset + 2 >= textlength)
				return(0);
			return(((loadedtext[offset] & 0xF) << 12) | ((loadedtext[offset + 1] & 0x3F) << 6) | ((loadedtext[offset + 2] & 0x3F)));
		}
		else if ((unichar & 0xC0) == 0xC0)
		{
			if (offset + 1 >= textlength)
				return(0);
			return(((loadedtext[offset] & 0x1F) << 6) | ((loadedtext[offset + 1] & 0x3F)));
		}
	}
	return(loadedtext[offset]);
}

//Check input character at current position
unsigned long ClassEXml::PeekInputChar(void)
{
	return(PeekInputCharAt(pos));
}

//Parse: skip white spaces
bool ClassEXml::SkipWhiteSpace(void)
{
	while (PeekInputChar() <= ' ')
	{
		ReadInputChar();
		if (state != EXML_PARSING)
			return(false);
	}
	return(true);
}

//Skip a defined number of characters
void ClassEXml::SkipInputChars(unsigned long count)
{
	for (unsigned long ctr = 0; ctr < count; ctr++)
		SkipInputChar();
}

//Skip one character
void ClassEXml::SkipInputChar(void)
{
	if (pos < textlength)
	{
		if (loadedtext[pos] == '\r')
		{
			if (pos + 1 < textlength)
				if (loadedtext[pos + 1] != '\n')
					curline++;
		}
		else if (loadedtext[pos] == '\n')
		{
			curline++;
		}
		unsigned char unichar = (unsigned char)loadedtext[pos];
		if ((unichar & 0xF0) == 0xF0)
			pos += 3;
		else if ((unichar & 0xE0) == 0xE0)
			pos += 2;
		else if ((unichar & 0xC0) == 0xC0)
			pos++;
	}
	pos++;
}

//Read one character
unsigned long ClassEXml::ReadInputChar(void)
{
	if (pos >= textlength)
	{
		state = EXML_ENDOFFILE;
		return(0);
	}
	unsigned long cread = PeekInputCharAt(pos);
	SkipInputChar();
	return(cread);
}

//Read character into the valuebuffer
bool ClassEXml::ReadChar(void)
{
	unsigned long c = ReadInputChar();
	if (c != 0 || state == EXML_PARSING)
	{
		if (c == '&')
		{
			if (PeekInputCharAt(pos) == '#')
			{
				int nCode = 0;
				SkipInputChar();
				c = ReadInputChar();
				if (c == 'x')
				{
					c = ReadInputChar();
					while (c != ';')
					{
						if (c >= '0' && c <= '9')
							nCode = nCode * 16 + c - '0';
						else if (c >= 'A' && c <= 'F')
							nCode= nCode * 16 + c - 'A' + 10;
						else if (c >= 'a' && c <= 'f')
							nCode= nCode * 16 + c - 'a' + 10;
						else
						{
							state = EXML_ILLEGALAMP;
							return(false);
						}
						c = ReadInputChar();
					}
				}
				else if (c >= '0' && c <= '9')
				{
					c = ReadInputChar();
					while (c != ';')
					{
						if (c >= '0' && c <= '9')
							nCode = nCode * 16 + c - '0';
						else
						{
							state = EXML_ILLEGALAMP;
							return(false);
						}
						c = ReadInputChar();
					}
				}
				else
				{
					state = EXML_ILLEGALAMP;
					return(false);
				}
				if (nCode > 65535)
				{
					state = EXML_LONGCHAR;
					return(false);
				}
				return(valuebuf->CatChr((WCHAR)nCode));
			}
			else if (PeekInputCharAt(pos) == 'a' && PeekInputCharAt(pos + 1) == 'm' && PeekInputCharAt(pos + 2) == 'p' && PeekInputCharAt(pos + 3) == ';')
			{
				SkipInputChars(4);
				return(valuebuf->CatChr('&'));
			}
			else if (PeekInputCharAt(pos) == 'a' && PeekInputCharAt(pos + 1) == 'p' && PeekInputCharAt(pos + 2) == 'o' && PeekInputCharAt(pos + 3) == 's' && PeekInputCharAt(pos + 4) == ';')
			{
				SkipInputChars(5);
				return(valuebuf->CatChr('\''));
			}
			else if (PeekInputCharAt(pos) == 'q' && PeekInputCharAt(pos + 1) == 'u' && PeekInputCharAt(pos + 2) == 'o' && PeekInputCharAt(pos + 3) == 't' && PeekInputCharAt(pos + 4) == ';')
			{
				SkipInputChars(5);
				return(valuebuf->CatChr('"'));
			}
			else if (PeekInputCharAt(pos) == 'l' && PeekInputCharAt(pos + 1) == 't' && PeekInputCharAt(pos + 2) == ';')
			{
				SkipInputChars(3);
				return(valuebuf->CatChr('<'));
			}
			else if (PeekInputCharAt(pos) == 'g' && PeekInputCharAt(pos + 1) == 't' && PeekInputCharAt(pos + 2) == ';')
			{
				SkipInputChars(3);
				return(valuebuf->CatChr('>'));
			}
			else
			{
				return(valuebuf->CatChr('&'));
				//state = EXML_ILLEGALAMP;
			}
		}
		else
			return(valuebuf->CatChr((unsigned short)c));
	}
	return(false);
}

//Read next content token
bool ClassEXml::ReadNextContentToken(void)
{
	if (PeekInputChar() == '<')
	{
		SkipInputChar();
		if (PeekInputChar() == '/')
		{
			tokenstate = EXMLTKN_TAGEND;
			SkipInputChar();
			while(PeekInputChar() > ' ' && PeekInputChar() != '>')
			{
				if (!ReadChar())
					return(false);
			}
			while (PeekInputChar() != '>')
				SkipInputChar();
			if (PeekInputChar() != '>')
			{
				state = EXML_ENDOFFILE;
				return(false);
			}
			SkipInputChar();
		}
		else if (PeekInputChar() == '?')
		{
			tokenstate = EXMLTKN_PROCINSTRUCTION;
			SkipInputChar();
			while (PeekInputCharAt(pos) != '?' || PeekInputCharAt(pos + 1) != '>')
			{
				if (!ReadChar())
					return(false);
			}
			SkipInputChars(2);
		}
		else if (PeekInputChar() == '!')
		{
			SkipInputChar();
			if (PeekInputCharAt(pos) == '-' && PeekInputCharAt(pos + 1) == '-')
			{
				tokenstate = EXMLTKN_COMMENT;
				SkipInputChars(2);
				while (PeekInputCharAt(pos) != '-' || PeekInputCharAt(pos + 1) != '-' || PeekInputCharAt(pos + 2) != '>')
				{
					if (!ReadChar())
						return(false);
				}
				SkipInputChars(3);
			}
			else if (PeekInputCharAt(pos) == 'D' && PeekInputCharAt(pos + 1) == 'O' && PeekInputCharAt(pos + 2) == 'C' && PeekInputCharAt(pos + 3) == 'T' && PeekInputCharAt(pos + 4) == 'Y' && PeekInputCharAt(pos + 5) == 'P' && PeekInputCharAt(pos + 6) == 'E')
			{
				int nCount= 1;
				tokenstate = EXMLTKN_DOCTYPE;
				SkipInputChars(7);
				while (nCount > 0)
				{
					if (PeekInputChar() == '>')
						nCount--;
					else if (PeekInputChar() == '<')
						nCount++;
					if (nCount > 0 && !ReadChar())
						return(false);
				}
				SkipInputChar();
			}
			else
			{
				tokenstate = EXMLTKN_UNKNOWN;
				while (PeekInputChar() != '>')
					if (!ReadChar())
						return(false);
				SkipInputChar();
			}
		}
		else
		{
			tokenstate = EXMLTKN_TAGBEGINOPEN;
			while (PeekInputChar() > ' ' && PeekInputChar() != '/' && PeekInputChar() != '>')
			{
				if (!ReadChar())
					return(false);
			}
			if (!SkipWhiteSpace())
				return(false);
		}
	}
	else
	{
		tokenstate = EXMLTKN_CONTENT;
		if (trimwhitespace)
			if (!SkipWhiteSpace())
				return(false);
		while (PeekInputChar() != '<')
		{
			/*if (trimwhitespace && PeekInputChar() < ' ')
			{
				if (PeekInputChar() == 0)
					return(false);
				SkipInputChar();
			}
			else */if (!ReadChar())
				return(false);
		}
		if (trimwhitespace)
			if (!SkipWhiteSpace())
				return(false);
	}
	return(true);
}

//Parse one tag
bool ClassEXml::ParseOneTag(void)
{
	WCHAR *tagname;
	if (valuebuf->Length() == 0)
	{
		state = EXML_NOMEMORY;
		return(false);
	}
	tagname = (WCHAR *)ClassEMemory::Alloc((valuebuf->Length() + 1) * sizeof(WCHAR));
	if (tagname == NULL)
	{
		state = EXML_NOMEMORY;
		return(false);
	}
	ClassEStd::StrCpy(tagname, valuebuf->Get());
	valuebuf->Trunc(0);
	if (!CallHandler(EXML_OPENTAG, tagname, NULL, NULL))
	{
		ClassEMemory::DeAlloc(tagname);
		return(false);
	}
	if (!ReadNextTagToken())
	{
		ClassEMemory::DeAlloc(tagname);
		return(false);
	}
	while (tokenstate != EXMLTKN_TAGEND && tokenstate != EXMLTKN_TAGBEGINCLOSE)
	{
		if (tokenstate == EXMLTKN_ATTRIBUTE)
		{
			if (attrbuf->Length() == 0)
			{
				state = EXML_NOMEMORY;
				ClassEMemory::DeAlloc(tagname);
				return(false);
			}
			if (!CallHandler(EXML_ADDATTRIBUTE, tagname, attrbuf->Get(), valuebuf->Get()))
			{
				ClassEMemory::DeAlloc(tagname);
				return(false);
			}
			valuebuf->Trunc(0);
		}
		else
		{
			state = EXML_NOATTRIBUTE;
			ClassEMemory::DeAlloc(tagname);
			return(false);
		}
		if (!ReadNextTagToken())
		{
			ClassEMemory::DeAlloc(tagname);
			return(false);
		}
	}
	if (!CallHandler(EXML_CLOSEATTRIBUTES, tagname, NULL, NULL))
	{
		ClassEMemory::DeAlloc(tagname);
		return(false);
	}
	if (tokenstate == EXMLTKN_TAGBEGINCLOSE)
	{
		if (!ReadNextContentToken())
		{
			ClassEMemory::DeAlloc(tagname);
			return(false);
		}
		while (tokenstate != EXMLTKN_TAGEND)
		{
			if (tokenstate == EXMLTKN_TAGBEGINOPEN)
			{
				if (!ParseOneTag())
				{
					ClassEMemory::DeAlloc(tagname);
					return(false);
				}
			}
			else if (tokenstate == EXMLTKN_CONTENT)
			{
				if (valuebuf->Length() > 0)
					if (!CallHandler(EXML_ADDCONTENT, tagname, NULL, valuebuf->Get()))
					{
						ClassEMemory::DeAlloc(tagname);
						return(false);
					}
			}
			//else if (tokenstate == EXMLTKN_COMMENT) //Ignore comment
			//{
			//}
			valuebuf->Trunc(0);
			if (!ReadNextContentToken())
			{
				ClassEMemory::DeAlloc(tagname);
				return(false);
			}
		}
		if (!ClassEStd::StrEqual(tagname, valuebuf->Get()))
		{
			state = EXML_TAGMISMATCH;
			ClassEMemory::DeAlloc(tagname);
			return(false);
		}
	}
	valuebuf->Trunc(0);
	if (!CallHandler(EXML_CLOSETAG, tagname, NULL, NULL))
	{
		ClassEMemory::DeAlloc(tagname);
		return(false);
	}
	ClassEMemory::DeAlloc(tagname);
	return(true);
}

//Read next tag token
bool ClassEXml::ReadNextTagToken(void)
{
	if (PeekInputChar() == '/')
	{
		SkipInputChar();
		if (PeekInputChar() == '>')
		{
			tokenstate = EXMLTKN_TAGEND;
			SkipInputChar();
		}
		else
		{
			state = EXML_INVALIDCLOSETAG;
			return(false);
		}
	}
	else if (PeekInputChar() == '>')
	{
		tokenstate = EXMLTKN_TAGBEGINCLOSE;
		SkipInputChar();
	}
	else
	{
		unsigned long cQuote;
		tokenstate = EXMLTKN_ATTRIBUTE;
		while (PeekInputChar() != '=' && PeekInputChar() > ' ')
		{
			if (!ReadChar())
				return(false);
		}
		if (!SkipWhiteSpace())
			return(false);
		if (PeekInputChar() != '=')
		{
			state = EXML_NOEQUALSIGN;
			return(false);
		}
		SkipInputChar();
		attrbuf->Copy(*valuebuf);
		valuebuf->Trunc(0);
		if (!SkipWhiteSpace())
			return(false);
		cQuote = ReadInputChar();
		if (state != EXML_PARSING)
			return(false);
		if (cQuote != '\'' && cQuote != '"')
		{
			state = EXML_NOQUOTE;
			return(false);
		}
		while (PeekInputChar() != cQuote)
		{
			if (!ReadChar())
				return(false);
		}
		SkipInputChar();
		if (!SkipWhiteSpace())
			return(false);
	}
	return(true);
}

//Call handler and error handling
bool ClassEXml::CallHandler(unsigned char event, const WCHAR *name, const WCHAR *attribute, const WCHAR *value)
{
	if (parsenodes)
		NodeCallback(this, event, name, attribute, value, userptr);
	if (usecallback != NULL)
	{
		if (!usecallback(this, event, name, attribute, value, userptr))
		{
			state = EXML_USERSTOP;
			return(false);
		}
	}
	if (state != EXML_PARSING)
		return(false);
	return(true);
}

//(Re)Initialize parser
void ClassEXml::Init(void)
{
	state = EXML_PARSING;
	curline = 0;
	pos = 0;
	textlength = ClassEStd::StrLen(loadedtext);
}

//Create XML nodes
bool ClassEXml::NodeCallback(ClassEXml *parser, unsigned char event, const WCHAR *name, const WCHAR *attribute, const WCHAR *value, void *userdata)
{
	EXmlNode node;
	EXmlAttribute attributeXml;
	switch (event)
	{
		case EXML_CLOSETAG:
			direction--;
			//if (direction < -1)
			//	direction = -1;
			break;
		case EXML_ADDATTRIBUTE:
			AddAttribute(&attributeXml, (WCHAR *)attribute, (WCHAR *)value);
			break;
		case EXML_CLOSEATTRIBUTES:
			break;
		case EXML_ADDCONTENT:
			SetNodeValue(&node, (WCHAR *)value);
			break;
		case EXML_OPENTAG:
			AddNode(&node, (WCHAR *)name, direction);
			direction = 1;
			break;
	}
	return(true);
}

//Initialize XML nodes
void ClassEXml::InitializeNodes()
{
	WCHAR rootTitle[16], errorTitle[16];
	isParsing = false;
	isFault = false;
	data.Trunc(0);
	prevdataidx = 0xFFFFFFFF;
	direction = 1;


	char empty[2];
	ClassEStd::StrCpy(empty, "");
	long valueLength = sizeof(char) * (1 + ClassEStd::StrLen(empty));
	long dummy = 0;
	emptyString = data.GetDatasize();
	data.Add(empty, valueLength);
	if (valueLength % 4) //Byte padding to 4-byte boundary
		data.Add(&dummy, 4 - (valueLength) % 4);


	ClassEStd::StrCpy(rootTitle, "root");
	ClassEStd::StrCpy(errorTitle, "#ERROR#");
	AddNode(&dummynode, errorTitle, 0);
	AddAttribute(&dummyattribute, errorTitle, errorTitle);
	AddNode(&rootnode, rootTitle, 0);
	isParsing = true;
}

//Add node to data object
//- if the direction is 0 it will be added at the same level of the last node
//- if the direction is 1 it will be added one level deeper as the last node
//- if the direction is -1 it will be added one level higher as the last node
void ClassEXml::AddNode(EXmlNode *node, WCHAR *name, signed char direction)
{
	EXmlNode *prvnode = NULL;
	unsigned long dummy = 0;
	node->xmlParent = this;
	if (prevdataidx >= 1 && prevdataidx < 0xFFFFFFFF)
		prvnode = (EXmlNode *)data.Get(prevdataidx);
	if (node == &dummynode && !isParsing)
	{
		dummynode.child = 0;
		dummynode.nxtsibling = 0;
		dummynode.prvsibling = 0;
		dummynode.parent = 0;
		dummynode.attributeIndex = 0xFFFFFFFF;
		dummynode.valueIndex = 0xFFFFFFFF;
		dummynode.dataIndex = 0;
		dummynode.nameLength = 0;
		dummynode.valueLength = 0;
	}
	else
	{
		node->child = 0xFFFFFFFF;
		node->parent = 0xFFFFFFFF;
		node->nxtsibling = 0xFFFFFFFF;
		node->prvsibling = 0xFFFFFFFF;
		node->attributeIndex = 0xFFFFFFFF;
		node->dataIndex = 0xFFFFFFFF;
		node->nameLength = 0;
		node->valueLength = 0;
		node->valueIndex = 0xFFFFFFFF;
	}
	node->dataIndex = data.GetDatasize();
	node->nameLength = ClassEStd::StrLen(name) * 2 + 2;
	node->valueLength = 0;
	if (prvnode)
	{
		if (direction < 0) //parent
		{
			int ctr;
			for (ctr = direction; ctr < 0; ctr++)
			{
				node->prvsibling = prvnode->parent;
				prvnode = (EXmlNode *)data.Get(node->prvsibling);
			}
			prvnode->nxtsibling = node->dataIndex;
			node->parent = prvnode->parent;
		}
		else if (direction == 0) //sibling
		{
			node->prvsibling = prvnode->dataIndex;
			node->parent = prvnode->parent;
			prvnode->nxtsibling = node->dataIndex;
		}
		else if (direction == 1) //child
		{
			node->parent = prvnode->dataIndex;
			prvnode->child = node->dataIndex;
		}
	}
	prevdataidx = node->dataIndex;
	data.Add(node, sizeof(EXmlNode));
	data.Add(&dummy, 4 - sizeof(EXmlNode) % 4);
	data.Add(name, node->nameLength);
	if (node->nameLength % 4) //Byte padding to 4-byte boundary
		data.Add(&dummy, 4 - (node->nameLength) % 4);
}

//Sets the value of the last node added
void ClassEXml::SetNodeValue(EXmlNode *node, WCHAR *value)
{
	node = (EXmlNode *)data.Get(prevdataidx);
	if (node->valueLength == 0)
	{
		unsigned long dummy = 0;
		node->valueIndex = data.GetDatasize();
		node->valueLength = ClassEStd::StrLen(value) * 2 + 2;
		data.Add(value, node->valueLength);
		node = (EXmlNode *)data.Get(prevdataidx);
		if (node->valueLength % 4) //Byte padding to 4-byte boundary
			data.Add(&dummy, 4 - (node->valueLength) % 4);
	}
}

//Adds an attribute to the last added node
void ClassEXml::AddAttribute(EXmlAttribute *attribute, WCHAR *name, WCHAR *value)
{
	EXmlNode *node = (EXmlNode*)data.Get(prevdataidx);
	bool firstAttribute = false;
	unsigned long dummy = 0;
	EXmlAttribute *lastAttribute = &dummyattribute;
	attribute->dataIndex = 0xFFFFFFFF;
	attribute->nextattribute = 0xFFFFFFFF;
	attribute->nameLength = 0;
	attribute->valueLength = 0;
	if (!isParsing)
	{
		firstAttribute = true;
		node->attributeIndex = data.GetDatasize();
	}
	else
	{
		lastAttribute = (EXmlAttribute*)data.Get(node->GetLastAttribute().dataIndex);
		if (lastAttribute->dataIndex == dummyattribute.dataIndex)
			firstAttribute = true;
		if (!firstAttribute)
			lastAttribute->nextattribute = data.GetDatasize();
		else
			node->attributeIndex = data.GetDatasize();
	}
	attribute->nameLength = ClassEStd::StrLen(name) * 2 + 2;
	attribute->nameLength += 4 - (attribute->nameLength) % 4;
	attribute->valueLength = ClassEStd::StrLen(value) * 2 + 2;
	attribute->xmlParent = node->xmlParent;
	attribute->nextattribute = 0xFFFFFFFF;
	attribute->dataIndex = data.GetDatasize();
	data.Add(attribute, sizeof(EXmlAttribute));
	data.Add(&dummy, 4 - sizeof(EXmlAttribute) % 4);
	data.Add(name, attribute->nameLength);
	if (attribute->nameLength % 4) //Byte padding to 4-byte boundary
		data.Add(&dummy, 4 - (attribute->nameLength) % 4);
	data.Add(value, attribute->valueLength);
	if (attribute->valueLength % 4) //Byte padding to 4-byte boundary
		data.Add(&dummy, 4 - (attribute->valueLength) % 4);
}

//ClassEXml: Constructor
ClassEXml::ClassEXml(void)
{
	trimwhitespace = true;
	loadedtext = NULL;
	attrbuf = NULL;
	valuebuf = NULL;
}

//ClassEXml: Destructor
ClassEXml::~ClassEXml(void)
{
	Close();
}

//Open xml text
bool ClassEXml::Open(const char *str)
{
	#if (EDITION_SDK < 1000)
		return(false);
	#endif
	Close();
	if (str == NULL)
		return(false);
	attrbuf = new EString;
	valuebuf = new EString;
	loadedtext = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(str) + 1);
	if (attrbuf == NULL || valuebuf == NULL || loadedtext == NULL)
	{
		if (attrbuf != NULL)
		{
			delete attrbuf;
			attrbuf = NULL;
		}
		if (valuebuf != NULL)
		{
			delete valuebuf;
			valuebuf = NULL;
		}
		if (loadedtext != NULL)
		{
			ClassEMemory::DeAlloc(loadedtext);
			loadedtext = NULL;
		}
		return(false);
	}
	ClassEStd::StrCpy(loadedtext, str);
	state = EXML_NOTPARSED;
	attrbuf->Configure(true, false, 1024);
	valuebuf->Configure(true, false, 1024);
	return(true);
}

//Open xml text
bool ClassEXml::Open(const WCHAR *str)
{
	#if (EDITION_SDK < 1000)
		return(false);
	#endif
	Close();
	if (str == NULL)
		return(false);
	attrbuf = new EString;
	valuebuf = new EString;
	loadedtext = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(str) + 1);
	if (attrbuf == NULL || valuebuf == NULL || loadedtext == NULL)
	{
		if (attrbuf != NULL)
		{
			delete attrbuf;
			attrbuf = NULL;
		}
		if (valuebuf != NULL)
		{
			delete valuebuf;
			valuebuf = NULL;
		}
		if (loadedtext != NULL)
		{
			ClassEMemory::DeAlloc(loadedtext);
			loadedtext = NULL;
		}
		return(false);
	}
	ClassEStd::StrToUtf8(loadedtext, str);
	state = EXML_NOTPARSED;
	attrbuf->Configure(true, false, 1024);
	valuebuf->Configure(true, false, 1024);
	return(true);
}

//Parse xml text
bool ClassEXml::Parse(bool storenodes, EXml_Callback callback, void *userdata)
{
	if (loadedtext == NULL)
		return(false);
	Init();
	usecallback = callback;
	userptr = userdata;
	parsenodes = storenodes;
	if (storenodes)
		InitializeNodes();
	do
	{
		SkipWhiteSpace();
		valuebuf->Trunc(0);
		if (!ReadNextContentToken())
		{
			if (state == EXML_ENDOFFILE)
				state = EXML_NODOCTAG;
			return(false);
		}
	}
	while(tokenstate == EXMLTKN_PROCINSTRUCTION || tokenstate == EXMLTKN_COMMENT || tokenstate == EXMLTKN_DOCTYPE);
	if (tokenstate == EXMLTKN_TAGBEGINOPEN)
		return(ParseOneTag());
	state = EXML_NODOCTAG;
	return(false);
}

//Close xml text
void ClassEXml::Close(void)
{
	data.Trunc(0);
	if (attrbuf != NULL)
	{
		delete attrbuf;
		attrbuf = NULL;
	}
	if (valuebuf != NULL)
	{
		delete valuebuf;
		valuebuf = NULL;
	}
	if (loadedtext)
	{
		ClassEMemory::DeAlloc(loadedtext);
		loadedtext = NULL;
	}
}

//Enable or disable whitespace trimming from content
void ClassEXml::IgnoreWhiteSpace(bool set)
{
	trimwhitespace = set;
}

//returns the root of the xml
EXmlNode &ClassEXml::GetRoot()
{
	return(*((EXmlNode *)data.Get(rootnode.dataIndex)));
}

///////////////////////////////////////////////////////////////////
// XML-RPC node
///////////////////////////////////////////////////////////////////

//EXmlRpcNode: contructor
EXmlRpcNode::EXmlRpcNode()
{
}

//EXmlRpcNode: destructor
EXmlRpcNode::~EXmlRpcNode()
{
}

//Get node name
WCHAR *EXmlRpcNode::GetName()
{
	return((unsigned short*)xmlrpcParent->data.Get(dataIndex + ALIGN_RPCNODESIZE + valueLength));
}

//Get node value
WCHAR *EXmlRpcNode::GetValue()
{
	return((unsigned short*)xmlrpcParent->data.Get(dataIndex + ALIGN_RPCNODESIZE));
}

//Get parent
EXmlRpcNode &EXmlRpcNode::GetParent()
{
	if (parent == 0xFFFFFFFF)
		return(xmlrpcParent->dummynode);
	return(*(EXmlRpcNode *) xmlrpcParent->data.Get(parent));
}

//Get first child
EXmlRpcNode &EXmlRpcNode::GetChild()
{
	if (child == 0xFFFFFFFF)
		return(xmlrpcParent->dummynode);
	return(*(EXmlRpcNode *)xmlrpcParent->data.Get(child));
}

//Get previous sibling
EXmlRpcNode &EXmlRpcNode::GetPreviousSibling()
{
	if (prvsibling == 0xFFFFFFFF)
		return(xmlrpcParent->dummynode);
	return (*(EXmlRpcNode *)xmlrpcParent->data.Get(prvsibling));
}

//Get next sibling
EXmlRpcNode &EXmlRpcNode::GetNextSibling()
{
	if (nxtsibling == 0xFFFFFFFF)
		return(xmlrpcParent->dummynode);
	return(*(EXmlRpcNode *)xmlrpcParent->data.Get(nxtsibling));
}

//Overloaded subscript operator (index)
EXmlRpcNode &EXmlRpcNode::operator [](int i)
{
	return(Get(i));
}

//Overloaded subscript operator (string)
EXmlRpcNode &EXmlRpcNode::operator [](const char *key)
{
	return(Get(key));
}

//Get node instance by index
EXmlRpcNode &EXmlRpcNode::Get(int i)
{
	if (child != 0xFFFFFFFF)
	{
		EXmlRpcNode *n = &this->GetChild();
		int ctr = 0;
		while (ctr <= i)
		{
			if (ctr == i)
				return(*n);
			if (n->nxtsibling != 0xFFFFFFFF)
				n = &n->GetNextSibling();
			else
				break;
			ctr++;
		}
	}
	return(xmlrpcParent->dummynode);
}

//Get node instance by string
EXmlRpcNode &EXmlRpcNode::Get(const char *key)
{
	if (child != 0xFFFFFFFF)
	{
		EXmlRpcNode *n = &this->GetChild();
		while (true)
		{
			if (ClassEStd::StrEqual(n->GetName(), key))
				return *n;
			if (n->nxtsibling != 0xFFFFFFFF && n->nxtsibling != n->dataIndex)
				n = &n->GetNextSibling();
			else
				break;
		}
	}
	return(xmlrpcParent->dummynode);
}

//Get number of children
unsigned long EXmlRpcNode::CountChildNodes()
{
	unsigned long ctr = 0;
	EXmlRpcNode n = GetChild();
	if (&GetChild() != &xmlrpcParent->dummynode)
		ctr++;
	else
		return(0);
	while (n.nxtsibling != 0xFFFFFFFF)
	{
		n = n.GetNextSibling();
		ctr++;
	}
	return(ctr);
}

///////////////////////////////////////////////////////////////////
// Main XML-RPC parser
///////////////////////////////////////////////////////////////////

//ClassEXmlRpc: constructor
ClassEXmlRpc::ClassEXmlRpc()
{
}

//ClassEXmlRpc: destructor
ClassEXmlRpc::~ClassEXmlRpc()
{
}

//Initialize parser
void ClassEXmlRpc::Initialize()
{
	WCHAR rootTitle[16], errorTitle[16];
	arraydepth = 0;
	arrayCtr = -1;
	isParsing = false;
	isFault = false;
	data.Trunc(0);
	prevdataidx = -1;
	xmlNodeType = XMLRPC_NONE;
	xmlNodeLastType = XMLRPC_NONE;
	xmlNodeDirection = 1;
	
	ClassEStd::StrCpy(rootTitle, "root");
	ClassEStd::StrCpy(errorTitle, "#ERROR#");
	AddNode(&dummynode, errorTitle, errorTitle, 0);
	AddNode(&rootnode, rootTitle, rootTitle, 0);
}

//Open parser
bool ClassEXmlRpc::Open(const char *str)
{
	Initialize();
	return(xml.Open(str));
}

//Open parser
bool ClassEXmlRpc::Open(const WCHAR *str)
{
	Initialize();
	return(xml.Open(str));
}

//Close parser
void ClassEXmlRpc::Close()
{
	xml.Close();
	data.Trunc(0);
}

//Start parsing
bool ClassEXmlRpc::Parse()
{
	Initialize();
	return(xml.Parse(false, XmlRpcCallback, this));
}

//Get root node
EXmlRpcNode &ClassEXmlRpc::GetRoot()
{
	return(*((EXmlRpcNode *)data.Get(rootnode.dataIndex)));
}

//Add node to data structure
void ClassEXmlRpc::AddNode(EXmlRpcNode *node, WCHAR *name, WCHAR *value, signed char direction)
{
	EXmlRpcNode *prvnode = NULL;
	unsigned long nameLength, dummy = 0;
	node->xmlrpcParent = this;
	if (prevdataidx >= 1)
		prvnode = (EXmlRpcNode *)data.Get(prevdataidx);
	node->dataIndex = data.GetDatasize();
	if (value != NULL)
		node->valueLength = ClassEStd::StrLen(value) * 2 + 2;
	else
		node->valueLength = 2;
	nameLength = ClassEStd::StrLen(name) * 2 + 2;
	if (node == &dummynode && !isParsing)
	{
		dummynode.child = 0;
		dummynode.nxtsibling = 0;
		dummynode.prvsibling = 0;
		dummynode.parent = 0;
	}
	else
	{
		node->child = 0xFFFFFFFF;
		node->parent = 0xFFFFFFFF;
		node->nxtsibling = 0xFFFFFFFF;
		node->prvsibling = 0xFFFFFFFF;
	}
	if (prvnode)
	{
		if (direction < 0) //parent
		{
			int ctr;
			for (ctr = direction; ctr < 0; ctr++)
			{
				node->prvsibling = prvnode->parent;
				prvnode = (EXmlRpcNode *)data.Get(node->prvsibling);
			}
			prvnode->nxtsibling = node->dataIndex;
			node->parent = prvnode->parent;
		}
		else if (direction == 0) //sibling
		{
			node->prvsibling = prvnode->dataIndex;
			node->parent = prvnode->parent;
			prvnode->nxtsibling = node->dataIndex;
		}
		else if (direction == 1) //child
		{
			node->parent = prvnode->dataIndex;
			prvnode->child = node->dataIndex;
		}
	}
	prevdataidx = node->dataIndex;
	data.Add(node, sizeof(EXmlRpcNode));
	data.Add(&dummy, 4 - sizeof(EXmlRpcNode) % 4);
	if (value != NULL)
		data.Add(value, node->valueLength);
	else
		data.Add(&dummy, node->valueLength);
	data.Add(name, nameLength);
	if ((node->valueLength + nameLength) % 4) //Byte padding to 4-byte boundary
		data.Add(&dummy, 4 - (node->valueLength + nameLength) % 4);
}

//Return if the response is a fault
bool ClassEXmlRpc::IsFault()
{
	return(isFault);
}

//XML parser callback function
bool ClassEXmlRpc::Callback(ClassEXml *parser, unsigned char event, const WCHAR *name, const WCHAR *attribute, const WCHAR *value)
{
	WCHAR emptyValue[4];
	EXmlRpcNode node;
	ClassEStd::StrCpy(emptyValue, "");
	switch(event)
	{
		case EXML_CLOSETAG:
			if (isParsing)
			{
				if (ClassEStd::StrEqual(name, "array"))
				{
					if (arrayCtr == 0) //lege array
					{
						AddNode(&node, xmlNodeName, emptyValue, xmlNodeDirection);
						arrayCtr = -1;
					}
					if (xmlNodeDirection < 0)
						xmlNodeDirection--;
					else
						xmlNodeDirection = -1;
					arraydepth--;
				}
				else if (ClassEStd::StrEqual(name, "struct"))
				{
					arraydepth--;
					if (arraydepth > 0)
					{
						xmlNodeDirection--;
						//AddNode(&dummynode, (WCHAR *)L"", (WCHAR *)L"", xmlNodeDirection);
						//xmlNodeDirection = 1;
					}
					else if (xmlNodeDirection < 0)
						xmlNodeDirection--;
					else
						xmlNodeDirection = -1;
				}
				else if (!hasContent)
				{
					Callback(parser, EXML_ADDCONTENT, name, attribute, value);
				}
			}
			break;
		case EXML_ADDCONTENT:
			if (isParsing)
			{
				hasContent = true;
				if (ClassEStd::StrEqual(name, "name"))
				{
					ClassEStd::StrCpy(xmlNodeName, value, 512);
				}
				else
				{
					if (xmlNodeLastType != XMLRPC_ARRAY && xmlNodeLastType != XMLRPC_MEMBER && xmlNodeDirection > 0)
						xmlNodeDirection = 0;
					AddNode(&node, xmlNodeName, (unsigned short *)value, xmlNodeDirection);
					xmlNodeDirection = 0;
					ClassEStd::StrCpy(xmlNodeName, "", 512);
				}
			}
			break;
		case EXML_OPENTAG:
			xmlNodeLastType = xmlNodeType;
			if (ClassEStd::StrEqual(name, "fault") && !isParsing)
			{
				isParsing = true;
				isFault = true;
			}
			else if (ClassEStd::StrEqual(name, "param") && !isParsing)
			{
				isParsing = true;
				isFault = false;
			}
			else if (isParsing)
			{
				if (ClassEStd::StrEqual(name, "array"))
				{
					arraydepth++;
					if (arrayCtr >= 0)
						arrayCtr++;
					arrayCtr = 0;
					if (xmlNodeLastType == XMLRPC_MEMBER)
					{
						WCHAR t[64];
						ClassEStd::StrCpy(t, "");
						AddNode(&node, xmlNodeName, t, xmlNodeDirection);
						ClassEStd::StrCpy(xmlNodeName, "", 512);
					}
					else if (xmlNodeLastType != XMLRPC_MEMBER && xmlNodeDirection == 0)
					{
						WCHAR t[64];
						ClassEStd::StrCpy(t, "");
						AddNode(&node, t, t, xmlNodeDirection);
					}
					xmlNodeDirection = 1;
					xmlNodeType = XMLRPC_ARRAY;
				}
				else if (ClassEStd::StrEqual(name, "base64"))
				{
					hasContent = false;
					if (arrayCtr >= 0)
						arrayCtr++;
					xmlNodeType = XMLRPC_BASE64;
				}
				else if (ClassEStd::StrEqual(name, "boolean"))
				{
					hasContent = false;
					if (arrayCtr >= 0)
						arrayCtr++;
					xmlNodeType = XMLRPC_BOOLEAN;
				}
				else if (ClassEStd::StrEqual(name, "dateTime.iso8601"))
				{
					hasContent = false;
					if (arrayCtr >= 0)
						arrayCtr++;
					xmlNodeType = XMLRPC_DATETIME;
				}
				else if (ClassEStd::StrEqual(name, "double"))
				{
					hasContent = false;
					if (arrayCtr >= 0)
						arrayCtr++;
					xmlNodeType = XMLRPC_DOUBLE;
				}
				else if (ClassEStd::StrEqual(name, "int") || ClassEStd::StrEqual(name, "i4"))
				{
					hasContent = false;
					if (arrayCtr >= 0)
						arrayCtr++;
					xmlNodeType = XMLRPC_INTEGER;
				}
				else if (ClassEStd::StrEqual(name, "string"))
				{
					hasContent = false;
					if (arrayCtr >= 0)
						arrayCtr++;
					xmlNodeType = XMLRPC_STRING;
				}
				else if (ClassEStd::StrEqual(name, "struct"))
				{
					hasContent = false;
					xmlNodeType = XMLRPC_STRUCT;
					if (xmlNodeLastType == XMLRPC_ARRAY && xmlNodeType == XMLRPC_STRUCT)
					{
						AddNode(&dummynode, emptyValue, emptyValue, xmlNodeDirection);
						xmlNodeDirection = 1;
					}
					else if (xmlNodeLastType != XMLRPC_ARRAY && xmlNodeType == XMLRPC_STRUCT && arraydepth > 0)
					{
						AddNode(&dummynode, emptyValue, emptyValue, -1);
						xmlNodeDirection = 1;
					}
					if (arrayCtr >= 0)
						arrayCtr++;
					if (ClassEStd::StrLen(xmlNodeName) > 0 && xmlNodeLastType == XMLRPC_MEMBER)
					{
						WCHAR t[64];
						ClassEStd::StrCpy(t, "");
						AddNode(&node, xmlNodeName, t, xmlNodeDirection);
					}
					if (xmlNodeDirection < 0 && (xmlNodeLastType == XMLRPC_MEMBER || xmlNodeLastType == XMLRPC_ARRAY))
						xmlNodeDirection = 1;
					if (arraydepth > 0)
						arraydepth++;
				}
				else if (ClassEStd::StrEqual(name, "member"))
				{
					hasContent = false;
					if (arrayCtr >= 0)
						arrayCtr++;
					xmlNodeType = XMLRPC_MEMBER;
				}
				else if (ClassEStd::StrEqual(name, "nil"))
				{
					hasContent = false;
					if (arrayCtr >= 0)
						arrayCtr++;
					xmlNodeType = XMLRPC_NIL;
				}
				/*else if (ClassEStd::StrEqual(name, "value"))
				{
					xmlNodeType = XMLRPC_VALUE;
				}*/
			}
		break;
	}
	return(true);
}

//Callback wrapper for the main XML parser
bool XmlRpcCallback(ClassEXml *parser, unsigned char event, const WCHAR *name, const WCHAR *attribute, const WCHAR *value, void *userdata)
{
	ClassEXmlRpc *xmlrpc = (ClassEXmlRpc *)userdata;
	return(xmlrpc->Callback(parser, event, name, attribute, value));
}
