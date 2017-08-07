// einput.h
// The smart input module
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED    //Can only be included by the main engine
#ifndef EINPUT_INCLUDED  //Make sure the module isn't included multiple times
#define EINPUT_INCLUDED

#define MAX_KEYACTIONS      224 //Maximum amount of mappable actions
#define MAX_KEYMAPPINGS     128 //Maximum amount of key mappings
#define MAX_INPUTEVENTS      32 //Maximum amount of input events
#define MAX_CHAREVENTS       32 //Maximum amount of characters to reserve for the typed character buffer
#define MAX_KEYDESCCOUNT    160 //Maximum different characters with key descriptions
#define MAX_CLICKID           8 //Maximum number of mouse buttons

//Input events
#define EIE_NONE              0
#define EIE_PRESSED           1
#define EIE_DOUBLEPRESSED     2
#define EIE_REPEAT            3
#define EIE_RELEASED          4

//Click device buttons
#define EIC_ALL               0
#define EIC_LEFT              1
#define EIC_MIDDLE            3
#define EIC_RIGHT             2
#define EIC_TOUCH1            1
#define EIC_TOUCH2            2
#define EIC_TOUCH3            3
#define EIC_TOUCH4            4
#define EIC_TOUCH5            5

//Default actions
#define EIM_CLICK             (MAX_KEYACTIONS)
#define EIM_WHEEL             (MAX_KEYACTIONS + MAX_CLICKID)

//Default input action mappings
#define DIAM_ARROWUP        160
#define DIAM_ARROWDOWN      161
#define DIAM_ARROWLEFT      162
#define DIAM_ARROWRIGHT     163
#define DIAM_VOLUMEUP       164
#define DIAM_VOLUMEDOWN     165
#define DIAM_SOFTKEY1       166
#define DIAM_SOFTKEY2       167
#define DIAM_SOFTKEY3       168
#define DIAM_SOFTKEY4       169
#define DIAM_MENUUP         192
#define DIAM_MENUDOWN       193
#define DIAM_MENULEFT       194
#define DIAM_MENURIGHT      195
#define DIAM_MENUCONFIRM    196
#define DIAM_MENUCANCEL     197
#define DIAM_MENUSOFT1      198
#define DIAM_MENUSOFT2      199
#define DIAM_KBTYPEKEY      200
#define DIAM_MENUCONFIRM_SP 201
#define DIAM_GAMEUP         208
#define DIAM_GAMEDOWN       209
#define DIAM_GAMELEFT       210
#define DIAM_GAMERIGHT      211
#define DIAM_GAMEACTION1    212
#define DIAM_GAMEACTION2    213
#define DIAM_GAMEPAUSE      214

///////////////////////////////////////////////////////////////////
// Structures                                                    //
///////////////////////////////////////////////////////////////////

typedef struct
{
	long clickx;
	long clicky;
	unsigned char type;
	unsigned char action;
}E_INPUTEVENT;
//An event from an input device

typedef struct
{
	double dHorizontalAccerleration;
	double dVerticalAccerleration;
	double dZAxisAccerleration;
	double dHorizontalAngleOfTilt;
	double dVerticalAngleOfTilt;
	double dZAxisAngleOfTilt;
	double dCompassHeading;
}TILTSENSORDATA;
//Tilt sensor data returned from PocketMotion's TiltControl

///////////////////////////////////////////////////////////////////
// Classes                                                       //
///////////////////////////////////////////////////////////////////

class ClassEInput
{
	private:
		#if defined(DEVICE_WINMOBILE)
			HINSTANCE dll_tilt;
			HANDLE htilt;
			int (*GetCurrentTiltControlDataFunc)(HANDLE, TILTSENSORDATA *, int);
			bool tiltinitialized;
			bool dlltiltnotfound;
		#endif
		E_INPUTEVENT inputevent[MAX_INPUTEVENTS];
		unsigned long keymapping[MAX_KEYMAPPINGS];
		unsigned long inputstate[MAX_KEYACTIONS + 16];
		unsigned long charevent_scancode[MAX_CHAREVENTS];
		unsigned long keydescindex[MAX_KEYDESCCOUNT];
		unsigned char ignorerelease[MAX_KEYACTIONS + 16];
		WCHAR charevent[MAX_CHAREVENTS];
		WCHAR *keydescriptor;
		long clickx[MAX_CLICKID];
		long clicky[MAX_CLICKID];
		long tiltx;
		long tilty;
		long tiltz;
		long wheely;
		unsigned long firstrepeat;
		unsigned long nextrepeat;
		unsigned long lastkey;
		unsigned char nummappings;
		unsigned char numevents;
		unsigned char numchar;
		bool tilteventfound;
	public:
		ClassEInput(void);
		~ClassEInput(void);
		bool AddKeyMapping(unsigned char action, unsigned long nr);
		bool AddKeyMapping(unsigned char action, unsigned long count, int nr, ...);
		bool RemoveKeyMapping(unsigned long nr);
		bool RemoveActionMapping(unsigned char action);
		bool MappedTo(unsigned char action, unsigned long nr);
		bool KeyHold(unsigned char action);
		bool ClickHold(unsigned char button = EIC_ALL);
		unsigned char GetKeyEvent(unsigned char action);
		bool GetKeyEvent(E_INPUTEVENT *event);
		bool GetClickEvent(E_INPUTEVENT *event);
		void GetClickPosition(long &x, long &y, unsigned char button = EIC_ALL);
		WCHAR GetChar(unsigned long *scancode = NULL);
		void PutChar(WCHAR chr, unsigned long scancode = 0);
		unsigned long GetLastKey(void);
		long GetWheelScroll(void);
		void GetTiltAngle(long &x, long &y, long &z);
		unsigned long GetKeyPressCount(unsigned char action, bool includerepeat = true);
		unsigned long GetClickPressCount(long &x, long &y, unsigned char button = EIC_ALL, bool includerepeat = false);
		void ResetEvents(void);
		void ResetState(void);
		void SetRepeatRates(unsigned long first, unsigned long next);
		ERESULT GetTiltData(ECD *ecd, long *x = NULL, long *y = NULL, long *z = NULL);
		void GetKeyName(long keynr, char *name);
		void GetKeyName(long keynr, WCHAR *name);
		void LoadKeyDescriptor(const char *keylist);
		void LoadKeyDescriptor(const WCHAR *keylist);
		void SetDefaultMappings(void);
		void GetButtonList(EBUTTONLIST *blist);
		void CheckRepeat(unsigned long timedelta);
		void OnKeyDown(unsigned long nr);
		void OnKeyUp(unsigned long nr);
		void OnClickDown(long x, long y, unsigned char button = EIC_ALL);
		void OnClickUp(long x, long y, unsigned char button = EIC_ALL);
		void OnClickDouble(long x, long y, unsigned char button = EIC_ALL);
		void OnClickMove(long x, long y, unsigned char button = EIC_ALL);
		void OnWheelScroll(long direction);
		void OnTilt(long x, long y, long z);
};//The input class

#endif
#endif
