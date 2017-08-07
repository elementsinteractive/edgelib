// code/main.h
// Contains the main framework class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

//The main class
class ClassMain : public ClassEdge
{
	private:
		ClassEXmlRpc xmlrpc;
		char *xmlrpc_string;
		char show_string[MAX_ESTR];
	public:
		ClassMain(void);
		~ClassMain(void);
		ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
		ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display);
		ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
		void OnButtonDown(unsigned long bnr, EBUTTONLIST *blist);
		void OnStylusDown(POINT pnt);
};


/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////

//ClassMain: constructor
ClassMain::ClassMain(void)
{
	xmlrpc_string = NULL;
}

//ClassMain: destructor
ClassMain::~ClassMain(void)
{
	if (xmlrpc_string)
		ClassEMemory::DeAlloc(xmlrpc_string);
}

//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
	#if defined(IDI_MAIN)
		config->icon = IDI_MAIN;
	#endif
	ClassEStd::StrCpy(config->caption, "Sample: XML-RPC");
	return(E_OK);
}

//Initialize framework
ERESULT ClassMain::OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)
{
	ClassEFile ef;
	EXmlRpcNode node;
	unsigned long ctr;
	bool parsedok = false;
	if (ef.Open("file.xml", EFOF_READ | EFOF_READONLYPATH))
	{
		unsigned long filesize = ef.Size();
		if (filesize)
		{
			xmlrpc_string = (char *)ClassEMemory::Alloc(filesize + 1);
			if (xmlrpc_string != NULL)
			{
				ef.Read(xmlrpc_string, filesize);
				xmlrpc_string[filesize] = 0;
			}
		}
		ef.Close();
	}
	if (xmlrpc_string == NULL)
	{
		SetErrorMsg("Can't read XML-RPC file");
		return(E_ERROR);
	}
	if (xmlrpc.Open(xmlrpc_string))
		if (xmlrpc.Parse())
			parsedok = true;
	if (!parsedok)
	{
		SetErrorMsg("Can't parse XML-RPC file");
		return(E_ERROR);
	}
	ClassEStd::StrCpy(show_string, "Supported platforms:\n");
	node = xmlrpc.GetRoot()["platform"];
	if (!ClassEStd::StrEqual(node.GetName(), "#Error", false))
	{
		for (ctr = 0; ctr < node.CountChildNodes(); ctr++)
		{
			ClassEStd::StrCat(show_string, "  ");
			ClassEStd::StrCat(show_string, node[ctr].GetValue());
			ClassEStd::StrCat(show_string, "\n");
		}
	}
	else
		ClassEStd::StrCat(show_string, "Error reading XML-RPC data!");
	return(E_OK);
}

//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
	display->buffer.DrawFont(2, 2, &display->fontinternal, show_string);
	display->buffer.DrawFont(2, display->buffer.GetHeight() - 2, &display->fontinternal, "Press key or tap screen to quit", EFX_COLORKEY | EFO_VBOTTOM);
	return(E_OK);
}

//Callback: Called when the user pressed a key or button
void ClassMain::OnButtonDown(unsigned long bnr, EBUTTONLIST *blist)
{
	Quit();
}

//Callback: Called when the user points the stylus down or clicks the left mouse button
void ClassMain::OnStylusDown(POINT pnt)
{
	Quit();
}
