// emod_console.cpp
// Debug console class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#if defined(DEVICE_SYMBIAN)
	#include <eikenv.h>
#endif

#define EDGE_INCLUDED
#include "estdc.h"
#include "ememory.h"
#include "econsole.h"

#include "core/edgecore.h"
#include "extra/verinfo.h"

/////////////////////////////////////////////////////////////////////
// ClassEConsole::Message handler
/////////////////////////////////////////////////////////////////////

#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)

BOOL CALLBACK ConsoleMsgProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	ClassEConsole *console;
	switch(wmsg)
	{
		case WM_CREATE:
			LPCREATESTRUCT lpcs;
			lpcs = (LPCREATESTRUCT)lparam;
			SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG_PTR)lpcs->lpCreateParams);
			break;
		case WM_SIZE:
			console = (ClassEConsole *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			console->OnResize(hwnd);
			break;
		case WM_CLOSE:
			console = (ClassEConsole *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (console)
				console->ActivateWindow(true);
			break;
	}
	return(DefWindowProc(hwnd, wmsg, wparam, lparam));
}

#endif

#if defined(DEVICE_IPHONE)
	void MacMsgWindow(char *msg, char *caption = NULL);
#endif

/////////////////////////////////////////////////////////////////////
// ClassEConsole::Private                                          //
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// ClassEConsole::Public                                           //
/////////////////////////////////////////////////////////////////////

//ClassEConsole: constructor
ClassEConsole::ClassEConsole(void)
{
	curinst = NULL;
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		regclass = false;
		cwnd = NULL;
	#endif
}

//ClassEConsole: destructor
ClassEConsole::~ClassEConsole(void)
{
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		if (cwnd)
		{
			DestroyWindow(cwnd);
			cwnd = NULL;
			ewnd = NULL;
		}
	#endif
}

//Link the message window
void ClassEConsole::LinkInstance(ECOREOBJ instance, ENATIVETYPE prginst)
{
	curinst = instance;
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		hinst = (HINSTANCE)prginst;
	#endif
}

#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)

//Activate/deactivate console window
void ClassEConsole::ActivateWindow(bool close)
{
	if (close)
	{
		if (cwnd)
		{
			ecEvent t_event;
			t_event.param1 = 0;
			DestroyWindow(cwnd);
			cwnd = NULL;
			ewnd = NULL;
			ecInstEventFromButton(curinst, &t_event, true);
		}
	}
	else
	{
		TCHAR classname[64];
		ClassEStd::StrCpy(classname, E_ENGINENAME " console");
		if (!regclass)
		{
			WNDCLASS wc;
			ClassEMemory::Set(&wc, 0, sizeof(WNDCLASS));
			wc.hIcon         = 0;
			wc.hCursor       = 0;
			wc.style         = CS_VREDRAW | CS_HREDRAW;
			wc.lpfnWndProc   = (WNDPROC)ConsoleMsgProc;
			wc.cbClsExtra    = 0;
			wc.cbWndExtra    = 0;
			wc.hInstance     = hinst;
			wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
			wc.lpszMenuName  = NULL;
			wc.lpszClassName = classname;
			if (RegisterClass(&wc) == NULL)
				return;
			regclass = true;
		}
		if (cwnd == NULL)
		{
			cwnd = CreateWindowEx(0, classname, E_ENGINENAME " console", WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_DLGFRAME | WS_SIZEBOX, 0, 0, GetSystemMetrics(SM_CXFULLSCREEN) / 2, GetSystemMetrics(SM_CYFULLSCREEN) / 2, NULL, NULL, hinst, this);
			if (cwnd)
			{
				ewnd = CreateWindowEx(WS_EX_STATICEDGE, TEXT("EDIT"), TEXT("Console Messages"), WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY, 10, 10, 200, 200, cwnd, NULL, hinst, NULL);
				if (ewnd != NULL)
				{
					RECT rc;
					GetWindowRect(cwnd, &rc);
					MoveWindow(ewnd, 0, 0, rc.right - rc.left - GetSystemMetrics(SM_CXFIXEDFRAME) * 2, rc.bottom - rc.top - GetSystemMetrics(SM_CYFIXEDFRAME) * 2 - GetSystemMetrics(SM_CYCAPTION), TRUE);
					Empty();
					ShowMsg("Console started...");
					ShowMsg("");
				}
				else
					SendMessage(cwnd, WM_CLOSE, 0, 0);
				SetForegroundWindow((HWND)ecInstGetWindow(curinst));
			}
		}
	}
}

//Resize the console edit window
void ClassEConsole::OnResize(HWND hwnd)
{
	if (ewnd != NULL)
	{
		RECT rc;
		GetWindowRect(hwnd, &rc);
		MoveWindow(ewnd, 0, 0, rc.right - rc.left - GetSystemMetrics(SM_CXFIXEDFRAME) * 2, rc.bottom - rc.top - GetSystemMetrics(SM_CYFIXEDFRAME) * 2 - GetSystemMetrics(SM_CYCAPTION), TRUE);
	}
}

#endif

//Empty the console
void ClassEConsole::Empty(void)
{
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		if (cwnd != NULL)
			SetWindowText(ewnd, TEXT(""));
	#endif
}

//Show a message on the console (+ newline option)
void ClassEConsole::ShowMsg(const char *msg, bool newline)
{
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		if (cwnd != NULL)
		{
			USTR_CHECK_BEGIN(msg);
			ShowMsg(ustr_msg, newline);
			USTR_CHECK_END(msg);
		}
	#endif
}

//Show a message on the console (+ newline option)
void ClassEConsole::ShowMsg(const WCHAR *msg, bool newline)
{
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		if (cwnd != NULL)
		{
			WCHAR cnewline[4];
			ClassEStd::StrCpy(cnewline, "\r\n");
			SendMessageW(ewnd, EM_SETSEL, 0xFFFFFFF, 0xFFFFFFF);
			SendMessageW(ewnd, EM_REPLACESEL, FALSE, (LPARAM)msg);
			if (newline)
			{
				SendMessageW(ewnd, EM_SETSEL, 0xFFFFFFF, 0xFFFFFFF);
				SendMessageW(ewnd, EM_REPLACESEL, FALSE, (LPARAM)cnewline);
			}
			SendMessage(ewnd, WM_VSCROLL, SB_BOTTOM, 0);
		}
	#endif
}

//Show a value on the console (+ newline option)
void ClassEConsole::ShowMsg(int value, bool newline)
{
	char valuestring[256];
	ClassEStd::IntToStr(valuestring, value, 10);
	ShowMsg(valuestring, newline);
}

//Show a message window
void ClassEConsole::MsgWindow(const char *msg, const char *caption)
{
	USTR_CHECK_BEGIN(msg);
	USTR_CHECK_BEGIN(caption);
	MsgWindow(ustr_msg, ustr_caption);
	USTR_CHECK_END(msg);
	USTR_CHECK_END(caption);
}

//Show a message window
void ClassEConsole::MsgWindow(const WCHAR *msg, const WCHAR *caption)
{
	#if defined(DEVICE_SYMBIAN)
		TBuf16<256> msgb = msg;
		CEikonEnv::Static()->AlertWin(msgb);
	#elif defined(DEVICE_WIN32)
		if (caption == NULL)
			MessageBoxW(NULL, (LPCWSTR)msg, L"Information", MB_OK);
		else
			MessageBoxW(NULL, (LPCWSTR)msg, (LPCWSTR)caption, MB_OK);
	#elif defined(DEVICE_IPHONE)
		char cMsg[MAX_ESTR], cCaption[MAX_ESTR];
		ClassEStd::StrToUtf8(cMsg, msg);
		if (caption == NULL)
			ClassEStd::StrCpy(cCaption, "Information");
		else
			ClassEStd::StrToUtf8(cCaption, caption);
		MacMsgWindow(cMsg, cCaption);
	#endif
}

//Show a message window
void ClassEConsole::MsgWindow(long value, const char *caption)
{
	WCHAR vstr[64];
	ClassEStd::IntToStr(vstr, value);
	USTR_CHECK_BEGIN(caption);
	MsgWindow(vstr, ustr_caption);
	USTR_CHECK_END(caption);
}
