// code/main.h
// Contains the main wrapper class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Include internal resources for native Win32 builds
#if defined(DEVICE_WIN32)
#include "resource.h"
#else
#define IDI_MAIN 0
#endif


/////////////////////////////////////////////////////////////////////
// Control key definitions (device specific)                       //
/////////////////////////////////////////////////////////////////////

#define CK_SOFT1_SIM 998
#define CK_SOFT2_SIM 999 //Simulated key presses


/////////////////////////////////////////////////////////////////////
// Other definitions                                               //
/////////////////////////////////////////////////////////////////////

#define PS_FAILED       0
#define PS_CTYPE        1
#define PS_SERVCLIENT   2
#define PS_TCPOPTION    3
#define PS_BTSEARCH1    4
#define PS_BTSEARCH2    5
#define PS_BTSEARCH3    6
#define PS_CHOOSEBTID   7
#define PS_CONNECT1     8
#define PS_CONNECT2     9
#define PS_WAITCONNECT 10
#define PS_IDLE        11
#define PS_FAILMSG     12 //Program states

#define PORT_TCP     4000 //Port to connect through TCP/IP

#define KI_ARROWUP      0
#define KI_ARROWDOWN    1
#define KI_WHEEL        4 //Keys tracked by this program

#define DIP_NR1       128
#define DIP_NR2         0
#define DIP_NR3         0
#define DIP_NR4         0 //Default IP address when connecting with TCP/IP

#define NM_MESSAGE      1 //Network message package type

#define MAX_BTDEVICES  16 //Search for a maximum of 16 bluetooth devices in the area
#define MAX_KEYINFO     3 //Number of tracked keys
#define MAX_MSG        32 //Maximum amount of messages for display
#define MAX_CLIENTS     8 //Maximum amount of client connections to a server
#define MAX_MSGLIST    10 //The size of the list of messages that can be sent over the network


/////////////////////////////////////////////////////////////////////
// Structure definitions                                           //
/////////////////////////////////////////////////////////////////////

//Key status information
typedef struct
{
	bool hold;
	bool simstylus;
	unsigned long timecount;
	unsigned long repeatrate;
}KEYINFO;

//Program configuration
typedef struct
{
	unsigned char last_ip[4];
}PRG_CFG;

//Bluetooth device information
typedef struct
{
	char name[128];
	char address[64];
	unsigned long port;
}BT_DEVICEINFO;

//Network message structure
typedef struct
{
	unsigned long uid;
	unsigned long msgtype;
	unsigned long fromclient;
	char data[64];
}NET_MESSAGE;


/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

//The main class
class ClassMain : public ClassEdge
{
	private:
		E2DSurface *curbuffer;
		E2DSurface font;
		ENetStack netstack[MAX_CLIENTS];
		KEYINFO keyinfo[MAX_KEYINFO];
		BT_DEVICEINFO btinfo[MAX_BTDEVICES];
		PRG_CFG cfg;
		char msgstr[MAX_MSG][64];
		char msglist[MAX_MSGLIST][64];
		char host_addr[128];
		char failmsg[128];
		unsigned char ip_addr[4];
		unsigned long waittimer;
		unsigned long failtimer;
		unsigned long host_port;
		unsigned char programstate;
		unsigned char useprotocol;
		unsigned char btfound;
		unsigned char focus;
		unsigned char onmsgline;
		bool isclient;
		void DrawBasicGUI(E2DSurface *buffer, char *caption, char *option1, char *option2);
		void DrawGUI(E2DSurface *buffer);
		void LoadCfg(void);
		void SaveCfg(void);
		void ClearMsg(void);
		void ShowMsg(char *msg, bool newline = true);
		void ShowMsg(long value, bool newline = true);
		void SendNetMessage(long client, char *msg, long fromclient);
	public:
		ClassMain(void);
		~ClassMain(void);
		ERESULT OnInit(ENATIVETYPE instance);
		ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config);
		ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
		ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display);
		ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
		void OnExit(ENATIVETYPE instance);
		void OnButtonDown(unsigned long bnr, EBUTTONLIST *blist);
		void OnButtonUp(unsigned long bnr, EBUTTONLIST *blist);
		void OnWheel(long direction);
		void OnStylusDown(POINT pnt);
		void OnStylusUp(POINT pnt);
		void OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer);
};


/////////////////////////////////////////////////////////////////////
// ClassMain: private                                              //
/////////////////////////////////////////////////////////////////////

//Draw the basic GUI interface with the program title and 2 options
void ClassMain::DrawBasicGUI(E2DSurface *buffer, char *caption, char *option1, char *option2)
{
	RECT rc;
	buffer->FillRect(NULL, E_RGBX(128, 192, 255));
	SetRect(&rc, 0, 0, buffer->GetWidth(), 14);
	buffer->FillRect(&rc, E_RGBX(64, 128, 255));
	SetRect(&rc, 0, buffer->GetHeight() - 14, buffer->GetWidth(), buffer->GetHeight());
	buffer->FillRect(&rc, E_RGBX(64, 192, 255));
	buffer->DrawLine(0, 14, buffer->GetWidth(), 14, ECOLOR_BLACK);
	SetRect(&rc, 0, buffer->GetHeight() - 16, buffer->GetWidth(), buffer->GetHeight() - 14);
	buffer->FillRect(&rc, ECOLOR_BLACK);
	buffer->DrawLine(buffer->GetWidth() / 2, buffer->GetHeight() - 16, buffer->GetWidth() / 2, buffer->GetHeight(), ECOLOR_BLACK);
	buffer->DrawFont(2, 2, &font, caption, EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
	buffer->DrawFont(buffer->GetWidth() / 4, buffer->GetHeight(), &font, option1, EFX_COLORKEY | EFX_COLORIZE | EFO_HCENTER | EFO_VBOTTOM, ECOLOR_BLACK);
	buffer->DrawFont(buffer->GetWidth() * 3 / 4, buffer->GetHeight(), &font, option2, EFX_COLORKEY | EFX_COLORIZE | EFO_HCENTER | EFO_VBOTTOM, ECOLOR_BLACK);
	buffer->DrawLine(buffer->GetWidth() - 14, 0, buffer->GetWidth() - 14, 14, ECOLOR_BLACK);
	buffer->DrawLine(buffer->GetWidth() - 12, 2, buffer->GetWidth() - 2, 12, ECOLOR_BLACK);
	buffer->DrawLine(buffer->GetWidth() - 2, 2, buffer->GetWidth() - 12, 12, ECOLOR_BLACK);
}

//Draw the GUI interface based on the current program state
void ClassMain::DrawGUI(E2DSurface *buffer)
{
	RECT rc;
	unsigned char ctr;
	switch(programstate)
	{
		case PS_FAILED:
			DrawBasicGUI(buffer, "Connect", "", "");
			buffer->DrawFont(2, 20, &font, failmsg, EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			break;
		case PS_CTYPE:
			DrawBasicGUI(buffer, "Connect", "TCP/IP", "Bluetooth");
			buffer->DrawFont(2, 20, &font, "Please choose your\nconnection type.", EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			break;
		case PS_SERVCLIENT:
			if (useprotocol == ECONN_SOCKET)
			{
				DrawBasicGUI(buffer, "Connect: TCP/IP", "Server", "Client");
				buffer->DrawFont(2, 20, &font, "Server or client?", EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
				buffer->DrawFont(2, 40, &font, "Using port:", EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
				buffer->DrawFont(2 + font.GetFontWidth("Using port: "), 40, &font, PORT_TCP, EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			}
			else
			{
				DrawBasicGUI(buffer, "Connect: Bluetooth", "Server", "Client");
				buffer->DrawFont(2, 20, &font, "Server or client?", EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			}
			break;
		case PS_TCPOPTION:
			DrawBasicGUI(buffer, "Client: TCP/IP", "Connect", "Exit");
			buffer->DrawFont(2, 20, &font, "IP Address:", EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			for (ctr = 0; ctr < 4; ctr++)
			{
				SetRect(&rc, 2 + ctr * 40, 56, 32 + ctr * 40, 72);
				if (ctr == focus)
				{
					buffer->DrawRect(&rc, ECOLOR_WHITE);
					buffer->DrawFont(rc.left + (rc.right - rc.left) / 2, rc.top - font.GetHeight() - 2, &font, "\x1", EFX_COLORKEY | EFO_HCENTER);
					buffer->DrawFont(rc.left + (rc.right - rc.left) / 2, rc.bottom + 2, &font, "\x2", EFX_COLORKEY | EFO_HCENTER);
				}
				else
					buffer->DrawRect(&rc, ECOLOR_BLACK);
				buffer->DrawFont(5 + ctr * 40, 58, &font, ip_addr[ctr], EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			}
			break;
		case PS_BTSEARCH1:
		case PS_BTSEARCH2:
		case PS_BTSEARCH3:
			DrawBasicGUI(buffer, "Client: Bluetooth", "Stop", "Exit");
			buffer->DrawFont(2, 20, &font, "Searching devices...", EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			buffer->DrawFont(2, 44, &font, "Found:", EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			buffer->DrawFont(2 + font.GetFontWidth("Found: "), 44, &font, btfound, EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			break;
		case PS_CHOOSEBTID:
			DrawBasicGUI(buffer, "Client: Bluetooth", "Connect", "Cancel");
			buffer->DrawFont(2, 20, &font, "Search end", EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			buffer->DrawFont(2, 44, &font, "Found:", EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			buffer->DrawFont(2 + font.GetFontWidth("Found: "), 44, &font, btfound, EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			SetRect(&rc, 0, buffer->GetHeight() / 2 - font.GetHeight() * btfound / 2 - 1, buffer->GetWidth(), buffer->GetHeight() / 2 + font.GetHeight() * btfound / 2);
			buffer->FillRect(&rc, ECOLOR_GRAY);
			for (ctr = 0; ctr < btfound; ctr++)
			{
				RECT focusrc;
				if (ctr == focus)
				{
					SetRect(&focusrc, rc.left, rc.top + ctr * font.GetHeight(), rc.right, rc.top + (ctr + 1) * font.GetHeight() + 1);
					buffer->FillRect(&focusrc, ECOLOR_WHITE);
				}
				buffer->DrawFont((rc.right - rc.left) / 2 - font.GetFontWidth(btinfo[ctr].name) / 2, rc.top + ctr * font.GetHeight(), &font, btinfo[ctr].name, EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			}
			buffer->DrawLine(rc.left, rc.top - 1, rc.right, rc.top - 1, ECOLOR_BLACK);
			buffer->DrawLine(rc.left, rc.bottom, rc.right, rc.bottom, ECOLOR_BLACK);
			break;
		case PS_CONNECT1:
		case PS_CONNECT2:
			if (isclient)
			{
				if (useprotocol == ECONN_SOCKET)
				{
					DrawBasicGUI(buffer, "Client: TCP/IP", "Message", "Exit");
					buffer->DrawFont(2, 20, &font, "Connecting...", EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
				}
				else
				{
					DrawBasicGUI(buffer, "Client: Bluetooth", "Message", "Exit");
					buffer->DrawFont(2, 20, &font, "Connecting...", EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
				}
			}
			else
			{
				if (useprotocol == ECONN_SOCKET)
					DrawBasicGUI(buffer, "Server: TCP/IP", "Message", "Exit");
				else
					DrawBasicGUI(buffer, "Server: Bluetooth", "Message", "Exit");
				buffer->DrawFont(2, 20, &font, "Initializing...", EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			}
			break;
		case PS_IDLE:
			if (isclient)
			{
				if (useprotocol == ECONN_SOCKET)
					DrawBasicGUI(buffer, "Client: TCP/IP", "Message", "Exit");
				else
					DrawBasicGUI(buffer, "Client: Bluetooth", "Message", "Exit");
			}
			else
			{
				if (useprotocol == ECONN_SOCKET)
					DrawBasicGUI(buffer, "Server: TCP/IP", "Message", "Exit");
				else
					DrawBasicGUI(buffer, "Server: Bluetooth", "Message", "Exit");
			}
			for (ctr = 0; ctr < MAX_MSG; ctr++)
				buffer->DrawFont(2, 20 + ctr * font.GetHeight(), &font, msgstr[ctr], EFX_COLORKEY | EFX_COLORIZE, ECOLOR_BLACK);
			break;
	}
}

//Load configuration
void ClassMain::LoadCfg(void)
{
	ClassEFile ef;
	cfg.last_ip[0] = DIP_NR1;
	cfg.last_ip[1] = DIP_NR2;
	cfg.last_ip[2] = DIP_NR3;
	cfg.last_ip[3] = DIP_NR4;
	if (ef.Open("connect.cfg", EFOF_READ))
	{
		ef.Read(&cfg, sizeof(PRG_CFG));
		ef.Close();
	}
}

//Save configuration
void ClassMain::SaveCfg(void)
{
	ClassEFile ef;
	if (ef.New("connect.cfg"))
	{
		ef.Write(&cfg, sizeof(PRG_CFG));
		ef.Close();
	}
}

//Clear the chat message window
void ClassMain::ClearMsg(void)
{
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_MSG; ctr++)
		ClassEStd::StrCpy(msgstr[ctr], "");
	onmsgline = 0;
}

//Show a message in the chat window
void ClassMain::ShowMsg(char *msg, bool newline)
{
	if (onmsgline >= MAX_MSG || onmsgline * font.GetHeight() >= curbuffer->GetHeight() - 48)
	{
		unsigned char ctr;
		for (ctr = 0; ctr < MAX_MSG - 1; ctr++)
			ClassEStd::StrCpy(msgstr[ctr], msgstr[ctr + 1]);
		ClassEStd::StrCpy(msgstr[MAX_MSG - 1], "");
		onmsgline--;
	}
	ClassEStd::StrCat(msgstr[onmsgline], msg);
	if (newline)
	{
		onmsgline++;
		if (onmsgline < MAX_MSG)
			ClassEStd::StrCpy(msgstr[onmsgline], "");
	}
}

//Show a value in the chat window
void ClassMain::ShowMsg(long value, bool newline)
{
	char valstr[32];
	ClassEStd::IntToStr(valstr, value, 10);
	ShowMsg(valstr, newline);
}

//Send a text message over the network
void ClassMain::SendNetMessage(long client, char *msg, long fromclient)
{
	NET_MESSAGE netmsg;
	netmsg.uid = APP_UID & 0xFFFFFFF;
	netmsg.msgtype = NM_MESSAGE;
	netmsg.fromclient = fromclient;
	ClassEStd::StrCpy(netmsg.data, msg);
	ecd.net[0]->Send((unsigned char *)&netmsg, sizeof(NET_MESSAGE), (unsigned char)client);
}


/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////

//ClassMain: constructor
ClassMain::ClassMain(void)
{
	unsigned short ctr;
	for (ctr = 0; ctr < MAX_KEYINFO; ctr++)
	{
		keyinfo[ctr].hold = false;
		keyinfo[ctr].simstylus = false;
	}
	ClassEStd::StrCpy(failmsg, "Unknown failure");
}

//ClassMain: destructor
ClassMain::~ClassMain(void)
{
}

//Configure framework
ERESULT ClassMain::OnFrameworkConfig(EFRAMEWORKCONFIG *config)
{
	config->maxfps = 20; //Limit the maximum frames per second by 20
	return(E_OK);
}

//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
	ClassEStd::StrCpy(config->caption, "Connect");
	config->icon            = IDI_MAIN;           //Application icon
	config->width           = 320;                //Desktop PC: Custom display width
	config->height          = 240;                //Desktop PC: Custom display height
	config->enginestatus    = true;               //Desktop PC: Enable the status bar and status information icon
	config->engineconsole   = false;              //Desktop PC: Disable the edge console
	return(E_OK);
}

//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_MSGLIST; ctr++)
		ClassEStd::StrCpy(msglist[ctr], "");
	LoadCfg();
	programstate = PS_CTYPE;
	ClassEStd::StrCpy(msglist[0], "Hello!");
	ClassEStd::StrCpy(msglist[1], "I like banana's");
	ClassEStd::StrCpy(msglist[2], "This program rules!");
	ClassEStd::StrCpy(msglist[3], "Check out my phone");
	ClassEStd::StrCpy(msglist[4], "How are you?");
	ClassEStd::StrCpy(msglist[5], "I'm fine");
	ClassEStd::StrCpy(msglist[6], "No comment");
	ClassEStd::StrCpy(msglist[7], "...");
	ClassEStd::StrCpy(msglist[8], "_O_");
	ClassEStd::StrCpy(msglist[9], ":-)");
	return(E_OK);
}

//Callback: Called when the display mode has changed, use this to initialize your surfaces, etc.
ERESULT ClassMain::OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)
{
	//Load a fixed font file
	if (display->CreateSurface(&font, "font_fixed.bmp", EST_READONLYPATH) != E_OK)
	{
		SetErrorMsg("Error loading font");
		return(E_ERROR);
	}
	else
	{
		font.SetFontCharacters(0, 127);
		font.BuildFont(false, ECOLOR_FUCHSIA);
	}
	curbuffer = &display->buffer;
	DrawGUI(curbuffer);
	return(E_OK);
}

//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
	if (programstate == PS_TCPOPTION) //Handling of IP address input fields
	{
		if (keyinfo[KI_ARROWUP].hold || keyinfo[KI_ARROWUP].simstylus)
		{
			keyinfo[KI_ARROWUP].timecount += timedelta;
			while (keyinfo[KI_ARROWUP].timecount >= keyinfo[KI_ARROWUP].repeatrate)
			{
				ip_addr[focus]++;
				keyinfo[KI_ARROWUP].timecount -= keyinfo[KI_ARROWUP].repeatrate;
				if (keyinfo[KI_ARROWUP].repeatrate > 20)
					keyinfo[KI_ARROWUP].repeatrate -= 10;
			}
		}
		else if (keyinfo[KI_ARROWDOWN].hold || keyinfo[KI_ARROWDOWN].simstylus)
		{
			keyinfo[KI_ARROWDOWN].timecount += timedelta;
			while (keyinfo[KI_ARROWDOWN].timecount >= keyinfo[KI_ARROWDOWN].repeatrate)
			{
				ip_addr[focus]--;
				keyinfo[KI_ARROWDOWN].timecount -= keyinfo[KI_ARROWDOWN].repeatrate;
				if (keyinfo[KI_ARROWDOWN].repeatrate > 20)
					keyinfo[KI_ARROWDOWN].repeatrate -= 10;
			}
		}
		DrawGUI(curbuffer);
	}
	else if (programstate == PS_BTSEARCH1) //Initiate a bluetooth device search
	{
		DrawGUI(curbuffer);
		programstate = PS_BTSEARCH2;
	}
	else if (programstate == PS_BTSEARCH2) //Initiate a bluetooth device search
	{
		if (ecd.net[0]->Init(useprotocol, this) == E_OK)
		{
			if (!ecd.net[0]->SearchWireless(MAX_BTDEVICES, true))
			{
				ClassEStd::StrCpy(failmsg, "No devices found");
				programstate = PS_FAILED;
			}
		}
		else
		{
			ClassEStd::StrCpy(failmsg, "Connection error");
			programstate = PS_FAILED;
		}
		if (programstate == PS_BTSEARCH2)
			programstate = PS_BTSEARCH3;
	}
	else if (programstate == PS_CONNECT1) //Inform the user of a connection in progress
	{
		programstate = PS_CONNECT2;
		DrawGUI(curbuffer);
	}
	else if (programstate == PS_CONNECT2) //Attempt to start a server, or connect to a server as a client
	{
		if (ecd.net[0]->Init(useprotocol, this) == E_OK)
		{
			if (isclient)
			{
				if (useprotocol == ECONN_SOCKET)
				{
					if (!ecd.net[0]->SearchHost(host_addr, PORT_TCP))
						programstate = PS_FAILED;
				}
				else
				{
					if (!ecd.net[0]->SearchHost(host_addr, host_port))
						programstate = PS_FAILED;
				}
				ClassEStd::StrCpy(failmsg, "Server not found");
			}
			else
			{
				if (useprotocol == ECONN_SOCKET)
				{
					if (!ecd.net[0]->SearchHost(NULL, PORT_TCP))
						programstate = PS_FAILED;
				}
				else
				{
					if (!ecd.net[0]->SearchHost(NULL, 0))
						programstate = PS_FAILED;
				}
				ClassEStd::StrCpy(failmsg, "Server failed to start");
			}
		}
		else
		{
			ClassEStd::StrCpy(failmsg, "Connection error");
			programstate = PS_FAILED;
		}
		if (programstate == PS_CONNECT2)
		{
			programstate = PS_WAITCONNECT;
			waittimer = 5000;
		}
		DrawGUI(curbuffer);
	}
	else if (programstate == PS_WAITCONNECT) //Wait for a connection to be established, connection failure on timeout
	{
		if (waittimer < timedelta)
		{
			ClassEStd::StrCpy(failmsg, "Connection timeout");
			programstate = PS_FAILED;
		}
		else
			waittimer -= timedelta;
		DrawGUI(curbuffer);
	}
	else if (programstate == PS_FAILED) //Prepare the failure message and set the message timer
	{
		failtimer = 3000;
		programstate = PS_FAILMSG;
	}
	else if (programstate == PS_FAILMSG) //Wait for the failure message to time out and return to the initial startup state
	{
		if (failtimer < timedelta)
		{
			programstate = PS_CTYPE;
			DrawGUI(curbuffer);
		}
		else
			failtimer -= timedelta;
	}
	if (keyinfo[KI_WHEEL].hold)
	{
		if (keyinfo[KI_WHEEL].timecount < timedelta)
			keyinfo[KI_WHEEL].timecount = 0;
		else
			keyinfo[KI_WHEEL].timecount -= timedelta;
	}
	return(E_OK);
}

//Callback: Called when the program exits
void ClassMain::OnExit(ENATIVETYPE instance)
{
	SaveCfg();
}

//Callback: Called when the user pressed a key or button
void ClassMain::OnButtonDown(unsigned long bnr, EBUTTONLIST *blist)
{
	switch(programstate)
	{
		case PS_CTYPE:
			if (ecd.input->MappedTo(DIAM_MENUSOFT1, bnr) || bnr == CK_SOFT1_SIM)
			{
				useprotocol = ECONN_SOCKET;
				programstate = PS_SERVCLIENT;
			}
			else if (ecd.input->MappedTo(DIAM_MENUSOFT2, bnr) || bnr == CK_SOFT2_SIM)
			{
				useprotocol = ECONN_BLUETOOTH;
				programstate = PS_SERVCLIENT;
			}
			else if (ecd.input->MappedTo(DIAM_MENUCONFIRM_SP, bnr))
				Quit();
			DrawGUI(curbuffer);
			break;
		case PS_SERVCLIENT:
			if (ecd.input->MappedTo(DIAM_MENUSOFT1, bnr) || bnr == CK_SOFT1_SIM)
			{
				isclient = false;
				programstate = PS_CONNECT1;
			}
			else if (ecd.input->MappedTo(DIAM_MENUSOFT2, bnr) || bnr == CK_SOFT2_SIM)
			{
				if (useprotocol == ECONN_SOCKET)
				{
					unsigned char ctr;
					for (ctr = 0; ctr < 4; ctr++)
						ip_addr[ctr] = cfg.last_ip[ctr];
					focus = 0;
					programstate = PS_TCPOPTION;
				}
				else
				{
					btfound = 0;
					programstate = PS_BTSEARCH1;
				}
				isclient = true;
			}
			else if (ecd.input->MappedTo(DIAM_MENUCONFIRM_SP, bnr))
				programstate = PS_CTYPE;
			DrawGUI(curbuffer);
			break;
		case PS_TCPOPTION:
			if (bnr == blist->KeyBackspace)
				ip_addr[focus] /= 10;
			else if (bnr >= '0' && bnr <= '9')
			{
				if (ip_addr[focus] * 10 + bnr - '0' < 256)
					ip_addr[focus] = (unsigned char)(ip_addr[focus] * 10 + bnr - '0');
			}
			else if (ecd.input->MappedTo(DIAM_ARROWUP, bnr))
				ip_addr[focus]++;
			else if (ecd.input->MappedTo(DIAM_ARROWDOWN, bnr))
				ip_addr[focus]--;
			else if (ecd.input->MappedTo(DIAM_ARROWLEFT, bnr))
				focus = (focus + 3) % 4;
			else if (ecd.input->MappedTo(DIAM_ARROWRIGHT, bnr))
				focus = (focus + 1) % 4;
			else if (ecd.input->MappedTo(DIAM_MENUSOFT1, bnr) || bnr == CK_SOFT1_SIM || ecd.input->MappedTo(DIAM_MENUCONFIRM_SP, bnr))
			{
				unsigned char ctr;
				for (ctr = 0; ctr < 4; ctr++)
					cfg.last_ip[ctr] = ip_addr[ctr];
				ClassEStd::IntToStr(host_addr, ip_addr[0], 10);
				ClassEStd::StrCat(host_addr, ".");
				ClassEStd::IntToStr(&host_addr[ClassEStd::StrLen(host_addr)], ip_addr[1], 10);
				ClassEStd::StrCat(host_addr, ".");
				ClassEStd::IntToStr(&host_addr[ClassEStd::StrLen(host_addr)], ip_addr[2], 10);
				ClassEStd::StrCat(host_addr, ".");
				ClassEStd::IntToStr(&host_addr[ClassEStd::StrLen(host_addr)], ip_addr[3], 10);
				programstate = PS_CONNECT1;
			}
			else if (ecd.input->MappedTo(DIAM_MENUSOFT2, bnr) || bnr == CK_SOFT2_SIM)
			{
				unsigned char ctr;
				for (ctr = 0; ctr < 4; ctr++)
					cfg.last_ip[ctr] = ip_addr[ctr];
				programstate = PS_CTYPE;
			}
			DrawGUI(curbuffer);
			break;
		case PS_BTSEARCH1:
		case PS_BTSEARCH2:
		case PS_BTSEARCH3:
			if (ecd.input->MappedTo(DIAM_MENUSOFT1, bnr) || bnr == CK_SOFT1_SIM || ecd.input->MappedTo(DIAM_MENUCONFIRM_SP, bnr))
				OnNetMessage(0, 0, ENET_ENDSEARCH, 0, NULL);
			else if (ecd.input->MappedTo(DIAM_MENUSOFT2, bnr) || bnr == CK_SOFT2_SIM)
				programstate = PS_CTYPE;
			DrawGUI(curbuffer);
			break;
		case PS_CHOOSEBTID:
			if (ecd.input->MappedTo(DIAM_MENUSOFT1, bnr) || bnr == CK_SOFT1_SIM || ecd.input->MappedTo(DIAM_MENUCONFIRM_SP, bnr))
			{
				ClassEStd::StrCpy(host_addr, btinfo[focus].address);
				host_port = btinfo[focus].port;
				programstate = PS_CONNECT1;
			}
			else if (ecd.input->MappedTo(DIAM_MENUSOFT2, bnr) || bnr == CK_SOFT2_SIM)
				programstate = PS_CTYPE;
			else if (ecd.input->MappedTo(DIAM_MENUUP, bnr))
				focus = (focus + btfound - 1) % btfound;
			else if (ecd.input->MappedTo(DIAM_MENUDOWN, bnr))
				focus = (focus + 1) % btfound;
			DrawGUI(curbuffer);
			break;
		case PS_CONNECT1:
		case PS_CONNECT2:
		case PS_WAITCONNECT:
			if (ecd.input->MappedTo(DIAM_MENUCONFIRM_SP, bnr))
				programstate = PS_CTYPE;
			break;
		case PS_IDLE:
			if (ecd.input->MappedTo(DIAM_MENUSOFT1, bnr) || bnr == CK_SOFT1_SIM)
			{
				if (isclient)
					SendNetMessage(0, msglist[ecd.math->Rand() % MAX_MSGLIST], 1);
				else
					SendNetMessage(0, msglist[ecd.math->Rand() % MAX_MSGLIST], 0);
			}
			else if (ecd.input->MappedTo(DIAM_MENUSOFT2, bnr) || bnr == CK_SOFT2_SIM || ecd.input->MappedTo(DIAM_MENUCONFIRM_SP, bnr))
			{
				ecd.net[0]->Disconnect();
				programstate = PS_CTYPE;
			}
			DrawGUI(curbuffer);
			break;
		case PS_FAILMSG:
			if (ecd.input->MappedTo(DIAM_MENUCONFIRM_SP, bnr))
				programstate = PS_CTYPE;
			DrawGUI(curbuffer);
			break;
	}

	//Handling of button repeat rates (for the IP address input fields)
	if (ecd.input->MappedTo(DIAM_ARROWUP, bnr))
	{
		keyinfo[KI_ARROWUP].hold = true;
		keyinfo[KI_ARROWUP].timecount = 0;
		keyinfo[KI_ARROWUP].repeatrate = 200;
	}
	else if (ecd.input->MappedTo(DIAM_ARROWDOWN, bnr))
	{
		keyinfo[KI_ARROWDOWN].hold = true;
		keyinfo[KI_ARROWDOWN].timecount = 0;
		keyinfo[KI_ARROWDOWN].repeatrate = 200;
	}
}

//Callback: Called when the user released a key or button
void ClassMain::OnButtonUp(unsigned long bnr, EBUTTONLIST *blist)
{
	if (ecd.input->MappedTo(DIAM_ARROWUP, bnr))
		keyinfo[KI_ARROWUP].hold = false;
	else if (ecd.input->MappedTo(DIAM_ARROWDOWN, bnr))
		keyinfo[KI_ARROWDOWN].hold = false;
}

//Callback: Called when the user scrolls a wheel device
void ClassMain::OnWheel(long direction)
{
	long add_modifier = 0;
	if (direction < 0)
		add_modifier = -1;
	else
		add_modifier = 1;
	if (keyinfo[KI_WHEEL].hold && keyinfo[KI_WHEEL].timecount > 0)
		keyinfo[KI_WHEEL].repeatrate++;
	else
		keyinfo[KI_WHEEL].repeatrate = 0;
	keyinfo[KI_WHEEL].hold = true;
	keyinfo[KI_WHEEL].timecount = 500;
	if (programstate == PS_TCPOPTION)
	{
		if (keyinfo[KI_WHEEL].repeatrate == 0)
			ip_addr[focus] += (unsigned char)(add_modifier * 1);
		else if (keyinfo[KI_WHEEL].repeatrate == 1)
			ip_addr[focus] += (unsigned char)(add_modifier * 2);
		else if (keyinfo[KI_WHEEL].repeatrate == 2)
			ip_addr[focus] += (unsigned char)(add_modifier * 3);
		else if (keyinfo[KI_WHEEL].repeatrate == 3)
			ip_addr[focus] += (unsigned char)(add_modifier * 4);
		else if (keyinfo[KI_WHEEL].repeatrate == 4)
			ip_addr[focus] += (unsigned char)(add_modifier * 5);
		else
			ip_addr[focus] += (unsigned char)(add_modifier * 10);
	}
	else if (programstate == PS_CHOOSEBTID)
	{
		if (add_modifier < 0)
			focus = (focus + 1) % btfound;
		else
			focus = (focus + btfound - 1) % btfound;
		DrawGUI(curbuffer);
	}
}

//Callback: Called when the user points the stylus down or clicks the left mouse button
void ClassMain::OnStylusDown(POINT pnt)
{
	if (pnt.x > (long)curbuffer->GetWidth() - 16 && pnt.y < 16)
	{
		Quit();
		return;
	}
	if (pnt.y >= (long)curbuffer->GetHeight() - 16)
	{
		if (pnt.x < (long)curbuffer->GetWidth() / 2)
			OnButtonDown(CK_SOFT1_SIM, &e_buttons);
		else
			OnButtonDown(CK_SOFT2_SIM, &e_buttons);
	}
	if (programstate == PS_TCPOPTION)
	{
		if (pnt.x % 40 >= 2 && pnt.x % 40 < 36)
			if (pnt.x / 40 < 4)
			{
				if (pnt.y >= 56 && pnt.y < 72)
					focus = (unsigned char)(pnt.x / 40);
				if (pnt.y >= 26 && pnt.y < 56)
				{
					ip_addr[focus]++;
					keyinfo[KI_ARROWUP].simstylus = true;
					keyinfo[KI_ARROWUP].timecount = 0;
					keyinfo[KI_ARROWUP].repeatrate = 200;
				}
				else if (pnt.y >= 72 && pnt.y < 102)
				{
					ip_addr[focus]--;
					keyinfo[KI_ARROWDOWN].simstylus = true;
					keyinfo[KI_ARROWDOWN].timecount = 0;
					keyinfo[KI_ARROWDOWN].repeatrate = 200;
				}
			}
	}
	if (programstate == PS_CHOOSEBTID)
		if (pnt.y >= (long)curbuffer->GetHeight() / 2 - btfound * (long)font.GetHeight() / 2 && pnt.y < (long)curbuffer->GetHeight() / 2 + btfound * (long)font.GetHeight() / 2)
		{
			focus = (unsigned char)((pnt.y - (curbuffer->GetHeight() / 2 - btfound * font.GetHeight() / 2)) / font.GetHeight());
			if (focus >= btfound)
				focus = btfound - 1;
			OnButtonDown(CK_SOFT1_SIM, &e_buttons);
		}
}

//Callback: Called when the user lifts the stylus up or releases the left mouse button
void ClassMain::OnStylusUp(POINT pnt)
{
	unsigned short ctr;
	for (ctr = 0; ctr < MAX_KEYINFO; ctr++)
		keyinfo[ctr].simstylus = false;
}

//Callback: Called when a network event occurs
void ClassMain::OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer)
{
	EBLUETOOTH_DEVICEID *deviceinfo;
	unsigned char ctr, stacknr;
	switch(msg)
	{
		case ENET_SERVERFOUND: //A server has been found, start the chat process
			if (programstate == PS_CONNECT1 || programstate == PS_CONNECT2 || programstate == PS_WAITCONNECT)
			{
				netstack[0].Alloc(1024);
				ClearMsg();
				ShowMsg("Connected to server:");
				ShowMsg((char *)buffer);
				ShowMsg("");
				programstate = PS_IDLE;
				DrawGUI(curbuffer);
			}
			break;
		case ENET_RUNNINGSERVER: //The server is running, accept clients
			if (programstate == PS_CONNECT1 || programstate == PS_CONNECT2 || programstate == PS_WAITCONNECT)
			{
				for (ctr = 0; ctr < MAX_CLIENTS; ctr++)
					netstack[ctr].Alloc(1024);
				ClearMsg();
				ShowMsg("Running server:");
				ShowMsg((char *)buffer);
				ShowMsg("");
				programstate = PS_IDLE;
				DrawGUI(curbuffer);
			}
			break;
		case ENET_ADDCLIENT: //A client has been added, accept and add this client or refuse if there are too many
			if (id == 0 || id > MAX_CLIENTS)
				ecd.net[0]->Disconnect((unsigned char)id);
			else if (programstate == PS_IDLE)
			{
				ShowMsg("Client(", false);
				ShowMsg(id, false);
				ShowMsg(") added:");
				ShowMsg((char *)buffer);
				ShowMsg("");
				DrawGUI(curbuffer);
			}
			break;
		case ENET_CLOSEDSOCKET: //The connection has been terminated
			if (programstate == PS_IDLE)
			{
				if (isclient)
				{
					ClassEStd::StrCpy(failmsg, "Lost server connection");
					programstate = PS_FAILED;
				}
				else
				{
					ShowMsg("Client(", false);
					ShowMsg(id, false);
					ShowMsg(") left");
					ShowMsg("");
				}
				DrawGUI(curbuffer);
			}
			break;
		case ENET_RECEIVEDATA: //Data is received, add the data to a stack and interpret messages
			stacknr = 0;
			if (!isclient)
			{
				if (id > 0 && id <= MAX_CLIENTS)
					stacknr = (unsigned char)id;
			}
			else
				stacknr = 1;
			if (stacknr)
				if (netstack[stacknr - 1].Push(buffer, bufsize))
				{
					unsigned long peeksize = netstack[stacknr - 1].Peek();
					if (peeksize == sizeof(NET_MESSAGE))
					{
						NET_MESSAGE msg;
						netstack[stacknr - 1].Pop(&msg, peeksize);
						if (msg.uid == (APP_UID & 0xFFFFFFF))
						{
							if (msg.msgtype == NM_MESSAGE && programstate == PS_IDLE)
							{
								if (!isclient)
								{
									for (ctr = 1; ctr <= MAX_CLIENTS; ctr++)
										if (ctr != stacknr)
											SendNetMessage(ctr, msg.data, stacknr);
									ShowMsg("Client(", false);
									ShowMsg(stacknr, false);
									ShowMsg(") says: ");
								}
								else
								{
									if (msg.fromclient != 0)
									{
										ShowMsg("Client(", false);
										ShowMsg(msg.fromclient, false);
										ShowMsg(") says: ");
									}
									else
										ShowMsg("Server says: ");
								}
								ShowMsg(msg.data);
								ShowMsg("");
							}
						}
					}
				}
			DrawGUI(curbuffer);
			break;
		case ENET_DEVICEFOUND: //A bluetooth device has been found, add it to the list
			if (programstate == PS_BTSEARCH1 || programstate == PS_BTSEARCH2 || programstate == PS_BTSEARCH3)
			{
				deviceinfo = (EBLUETOOTH_DEVICEID *)buffer;
				ClassEStd::StrCpy(btinfo[btfound].address, deviceinfo->address);
				if (ClassEStd::StrLen(deviceinfo->name))
					ClassEStd::StrCpy(btinfo[btfound].name,	deviceinfo->name);
				else
					ClassEStd::StrCpy(btinfo[btfound].name,	deviceinfo->address);
				btinfo[btfound].port = deviceinfo->port;
				btfound++;
				DrawGUI(curbuffer);
			}
			break;
		case ENET_ENDSEARCH: //The bluetooth search has ended, pick a device or show a message that no devices are found
			if (programstate == PS_BTSEARCH1 || programstate == PS_BTSEARCH2 || programstate == PS_BTSEARCH3)
			{
				if (btfound == 0)
				{
					ClassEStd::StrCpy(failmsg, "No devices found");
					programstate = PS_FAILED;
				}
				else
				{
					focus = 0;
					programstate = PS_CHOOSEBTID;
				}
				DrawGUI(curbuffer);
			}
			break;
	}
}
