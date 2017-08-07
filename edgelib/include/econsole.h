// econsole.h
// A module for communicating with the edge message console
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED     //Can only be included by the main engine
#ifndef ECONSOLE_INCLUDED //Make sure the module isn't included multiple times
#define ECONSOLE_INCLUDED

///////////////////////////////////////////////////////////////////
// Classes                                                       //
///////////////////////////////////////////////////////////////////

class ClassEConsole
{
	private:
		ECOREOBJ curinst;
		#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
			HINSTANCE hinst;
			HWND cwnd;
			bool regclass;
		#endif
	public:
		#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
			HWND ewnd;
		#endif
		ClassEConsole(void);
		~ClassEConsole(void);
		void LinkInstance(ECOREOBJ instance, ENATIVETYPE prginst);
		#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
			void ActivateWindow(bool close = false);
			void OnResize(HWND hwnd);
		#endif
		void Empty(void);
		void ShowMsg(const char *msg, bool newline = true);
		void ShowMsg(const WCHAR *msg, bool newline = true);
		void ShowMsg(int value, bool newline = true);
		void static MsgWindow(const char *msg, const char *caption = NULL);
		void static MsgWindow(const WCHAR *msg, const WCHAR *caption = NULL);
		void static MsgWindow(long value, const char *caption = NULL);
};//The console class

#endif
#endif
