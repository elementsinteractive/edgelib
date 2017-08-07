// xmlapi.h
// Extended API's related to XML parsing and processing
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED     //Can only be included by the main engine
#ifndef EXMLAPI_INCLUDED  //Make sure the API definition file isn't included multiple times
#define EXMLAPI_INCLUDED

///////////////////////////////////////////////////////////////////
// Data definitions
///////////////////////////////////////////////////////////////////

//Callback events
#define EXML_CLOSETAG         0
#define EXML_ADDATTRIBUTE     1
#define EXML_CLOSEATTRIBUTES  2
#define EXML_ADDCONTENT       3
#define EXML_OPENTAG          4

//Parser states
#define EXML_NOTPARSED        0
#define EXML_PARSING          1
#define EXML_ENDOFFILE        2
#define EXML_NODOCTAG         3
#define EXML_ILLEGALAMP       4
#define EXML_LONGCHAR         5
#define EXML_NOMEMORY         6
#define EXML_USERSTOP         7
#define EXML_INVALIDCLOSETAG  8
#define EXML_NOEQUALSIGN      9
#define EXML_NOQUOTE         10
#define EXML_NOATTRIBUTE     11
#define EXML_TAGMISMATCH     12

//XML RPC data types
#define XMLRPC_NONE			0
#define XMLRPC_ARRAY		1
#define XMLRPC_BASE64		2
#define XMLRPC_BOOLEAN		3
#define XMLRPC_DATETIME		4
#define XMLRPC_DOUBLE		5
#define XMLRPC_INTEGER		6
#define XMLRPC_STRING		7
#define XMLRPC_STRUCT		8
#define XMLRPC_NIL			9
#define XMLRPC_MEMBER		10
#define XMLRPC_VALUE		11

//Callback function for parsing the document
class ClassEXml;
class ClassEXmlRpc;
typedef bool (*EXml_Callback)(ClassEXml *parser, unsigned char event, const WCHAR *name, const WCHAR *attribute, const WCHAR *value, void *userdata);

///////////////////////////////////////////////////////////////////
// Classes
///////////////////////////////////////////////////////////////////

class EXmlAttribute
{
	friend class ClassEXml;
	friend class EXmlNode;
	private:
		ClassEXml *xmlParent;
		unsigned long dataIndex;
		unsigned long nameLength;
		unsigned long valueLength;
		unsigned long nextattribute;
		EXmlAttribute &Get(const char *key);
	public:
		EXmlAttribute(void);
		~EXmlAttribute(void);
		WCHAR *GetName(void);
		WCHAR *GetValue(void);
		EXmlAttribute &GetNextAttribute(void);
};//XML single attribute

class EXmlNode
{
	friend class ClassEXml;
	private:
		ClassEXml *xmlParent;
		unsigned long dataIndex;
		unsigned long valueIndex;
		unsigned long attributeIndex;
		unsigned long nameLength;
		unsigned long valueLength;
		unsigned long parent;
		unsigned long child;
		unsigned long prvsibling;
		unsigned long nxtsibling;
		EXmlNode &Get(int i);
		EXmlNode &Get(const char *key);
		EXmlAttribute &GetLastAttribute(void);
		EXmlAttribute &GetAttribute(const char *name);
		EXmlAttribute &GetAttribute(int id);
	public:
		EXmlNode(void);
		~EXmlNode(void);
		WCHAR *GetName(void);
		WCHAR *GetValue(void);
		EXmlNode &GetParent(void);
		EXmlNode &GetChild(void);
		EXmlNode &GetPreviousSibling(void);
		EXmlNode &GetNextSibling(void);
		EXmlNode &operator[](int i);
		EXmlNode &operator[](const char *key);
		unsigned long CountChildNodes(void);
		unsigned long CountAttributes(void);
		WCHAR *GetAttributeName(int id);
		WCHAR *GetAttributeValue(int id);
		WCHAR *GetAttributeValue(const char *name);
};//XML single node

class ClassEXml
{
	friend class EXmlNode;
	friend class EXmlAttribute;
	private:
		EData data;
		EXmlNode rootnode;
		EXmlNode dummynode;
		EXmlAttribute dummyattribute;
		EString *attrbuf;
		EString *valuebuf;
		EXml_Callback usecallback;
		void *userptr;
		char *loadedtext;
		unsigned long pos;
		unsigned long curline;
		unsigned long textlength;
		unsigned long prevdataidx;
		unsigned char state;
		unsigned char tokenstate;
		signed char direction;
		bool trimwhitespace;
		bool parsenodes;
		bool isParsing;
		bool isFault;
		unsigned long inline PeekInputCharAt(unsigned long offset);
		unsigned long inline PeekInputChar(void);
		bool inline SkipWhiteSpace(void);
		void inline SkipInputChars(unsigned long count);
		void inline SkipInputChar(void);
		unsigned long inline ReadInputChar(void);
		bool ReadChar(void);
		bool ReadNextContentToken(void);
		bool ParseOneTag(void);
		bool ReadNextTagToken(void);
		bool CallHandler(unsigned char event, const WCHAR *name, const WCHAR *attribute, const WCHAR *value);
		void Init(void);
		bool NodeCallback(ClassEXml *parser, unsigned char event, const WCHAR *name, const WCHAR *attribute, const WCHAR *value, void *userdata);
		void InitializeNodes(void);
		void AddNode(EXmlNode *node, WCHAR *name, signed char direction);
		void SetNodeValue(EXmlNode *node, WCHAR *value);
		void AddAttribute(EXmlAttribute *attribute, WCHAR *name, WCHAR *value);
	public:
		ClassEXml(void);
		~ClassEXml(void);
		unsigned long emptyString;
		bool Open(const char *str);
		bool Open(const WCHAR *str);
		bool Parse(bool storenodes = true, EXml_Callback callback = NULL, void *userdata = NULL);
		void Close(void);
		void IgnoreWhiteSpace(bool set);
		EXmlNode &GetRoot(void);
};//EDGELIB XML parser implementation

class EXmlRpcNode
{
	friend class ClassEXmlRpc;
	private:
		ClassEXmlRpc *xmlrpcParent;
		unsigned long dataIndex;
		unsigned long valueLength;
		unsigned long parent;
		unsigned long child;
		unsigned long prvsibling;
		unsigned long nxtsibling;
		EXmlRpcNode &Get(int i);
		EXmlRpcNode &Get(const char *key);
	public:
		EXmlRpcNode(void);
		~EXmlRpcNode(void);
		WCHAR *GetName(void);
		WCHAR *GetValue(void);
		EXmlRpcNode &GetParent(void);
		EXmlRpcNode &GetChild(void);
		EXmlRpcNode &GetPreviousSibling(void);
		EXmlRpcNode &GetNextSibling(void);
		EXmlRpcNode &operator[](int i);
		EXmlRpcNode &operator[](const char *key);
		unsigned long CountChildNodes(void);
};//XML RPC single node

class ClassEXmlRpc
{
	friend class EXmlRpcNode;
	private:
		ClassEXml xml;
		EData data;
		EXmlRpcNode rootnode;
		EXmlRpcNode dummynode;
		WCHAR xmlNodeName[64];
		long arrayCtr;
		long arraydepth;
		long prevdataidx;
		long xmlNodeType;
		long xmlNodeLastType;
		signed char xmlNodeDirection;
		bool isParsing;
		bool isFault;
		bool hasContent;
		void Initialize();
		void AddNode(EXmlRpcNode *node, WCHAR *name, WCHAR *value, signed char direction);
	public:
		ClassEXmlRpc(void);
		~ClassEXmlRpc(void);
		bool Open(const char *str);
		bool Open(const WCHAR *str);
		bool Parse(void);
		void Close(void);
		EXmlRpcNode &GetRoot(void);
		bool IsFault();
		bool Callback(ClassEXml *parser, unsigned char event, const WCHAR *name, const WCHAR *attribute, const WCHAR *value);
};//XML RPC parser

#endif
#endif
