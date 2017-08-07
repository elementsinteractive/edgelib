// emod_input.cpp
// Input class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#if defined(DEVICE_SYMBIAN)
	#include <e32keys.h>
#elif defined(DEVICE_LINUX)
	#include <sys/stat.h>
	#include <stdio.h>
#endif

#define EDGE_INCLUDED
#include "eloader.h"
#include "estdc.h"
#include "ememory.h"
#include "edisplay.h"
#include "einput.h"
#include "econsole.h"

#include "extra/verinfo.h"

#define EIF_MAPPINGHOLD      256

#if defined(DEVICE_SYMBIAN)
	#if (DEVICE_SYMBIAN < 900) || defined(DEVICE_SYMBIANEMU)
		#define va_start VA_START
		#define va_arg VA_ARG
		#define va_end VA_END
		#define va_list VA_LIST
	#endif
#elif defined(DEVICE_LINUX)
	#include <stdarg.h>
#elif defined(DEVICE_OPENKODE)
	#include <KD/ATX_specialkeys.h>
	#include <KD/ATX_tvremote.h>
	#include <KD/KHR_formatted.h>
	#define va_start KD_VA_START_KHR
	#define va_arg(l, t) KD_VA_ARG_INT_KHR(l)
	#define va_end KD_VA_END_KHR
	#define va_list KDVaListKHR
#endif

#define MAX_KEYNAMECOUNT 256 //Keys with a name description
#include "extra/keynames.h"

/////////////////////////////////////////////////////////////////////
// ClassEInput::Static data                                        //
/////////////////////////////////////////////////////////////////////

const char ekeydescriptorlist[] = "\
Up\n\
Down\n\
Left\n\
Right\n\
F1\n\
F2\n\
F3\n\
F4\n\
F5\n\
F6\n\
F7\n\
F8\n\
F9\n\
F10\n\
F11\n\
F12\n\
Tab\n\
Space\n\
Enter\n\
Shift\n\
Control\n\
Alt\n\
Esc\n\
Insert\n\
Delete\n\
Home\n\
End\n\
Pageup\n\
Pagedown\n\
Capslock\n\
Numlock\n\
Scroll lock\n\
Printscreen\n\
Pause\n\
Backspace\n\
Softkey 1\n\
Softkey 2\n\
Softkey 3\n\
Softkey 4\n\
Menu\n\
Start\n\
Select\n\
Jog up\n\
Jog down\n\
Jog\n\
Volume up\n\
Volume down\n\
Power\n\
Play\n\
Rewind\n\
Forward\n\
Stop\n\
";

/////////////////////////////////////////////////////////////////////
// ClassEInput::Pure functions                                     //
/////////////////////////////////////////////////////////////////////

//Check if the input hold state should still be set
bool EInput_CheckHoldState(unsigned long *keymapping, unsigned long *inputstate, unsigned char checkmapping, unsigned char nummappings)
{
	bool justreleased = false;
	if (checkmapping < MAX_KEYMAPPINGS)
	{
		unsigned char action = (unsigned char)(keymapping[checkmapping] & 255);
		if (action < MAX_KEYACTIONS)
		{
			unsigned long oldstate = inputstate[action];
			unsigned char ctr;
			if (inputstate[action] > 0)
				justreleased = true;
			inputstate[action] = 0;
			for (ctr = 0; ctr < nummappings; ctr++)
				if ((keymapping[ctr] & 255) == action && (keymapping[ctr] & EIF_MAPPINGHOLD))
				{
					inputstate[action] = oldstate;
					justreleased = false;
					break;
				}
		}
	}
	return(justreleased);
}

/////////////////////////////////////////////////////////////////////
// ClassEInput::Private                                            //
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// ClassEInput::Public                                             //
/////////////////////////////////////////////////////////////////////

//ClassEInput: constructor
ClassEInput::ClassEInput(void)
{
	#if defined(DEVICE_WINMOBILE)
		dll_tilt = NULL;
		tiltinitialized = false;
		dlltiltnotfound = false;
	#endif
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_KEYACTIONS + 16; ctr++)
	{
		inputstate[ctr] = 0;
		ignorerelease[ctr] = 1;
	}
	tilteventfound = false;
	keydescriptor = NULL;
	LoadKeyDescriptor((char *)ekeydescriptorlist);
	nummappings = 0;
	numevents   = 0;
	numchar     = 0;
	firstrepeat = 300;
	nextrepeat  = 100;
	lastkey     = 0;
	for (ctr = 0; ctr < MAX_CLICKID; ctr++)
	{
		clickx[ctr] = 0;
		clicky[ctr] = 0;
	}
	tiltx = 0;
	tilty = 0;
	tiltz = 0;
	wheely = 0;
}

//ClassEInput: destructor
ClassEInput::~ClassEInput(void)
{
	if (keydescriptor != NULL)
		ClassEMemory::DeAlloc(keydescriptor);
	#if defined(DEVICE_WINMOBILE)
		if (tiltinitialized)
		{
			typedef HANDLE (*DestroyTiltControlFunc)(HANDLE);
			DestroyTiltControlFunc Func;
			Func = (DestroyTiltControlFunc)GetProcAddress(dll_tilt, TEXT("?DestroyTiltControl@@YAPAXPAX@Z"));
			if (Func != NULL)
				(Func)(htilt);
		}
		if (dll_tilt != NULL)
			FreeLibrary(dll_tilt);
	#endif
}

//Add an input mapping
bool ClassEInput::AddKeyMapping(unsigned char action, unsigned long nr)
{
	if (nummappings < MAX_KEYMAPPINGS && action < MAX_KEYACTIONS)
	{
		keymapping[nummappings] = (nr << 16) | action;
		nummappings++;
		return(true);
	}
	return(false);
}

//Add an input mapping, multiple keys to 1 action
bool ClassEInput::AddKeyMapping(unsigned char action, unsigned long count, int nr, ...)
{
	if (count == 0)
		return(false);
	bool result = AddKeyMapping(action, nr);
	if (count > 1 && result)
	{
		unsigned long ctr;
		va_list argptr;
		va_start(argptr, nr);
		for (ctr = 1; ctr < count; ctr++)
		{
			int extnr = va_arg(argptr, int);
			if (result)
				result = AddKeyMapping(action, extnr);
			else
				break;
		}
		va_end(argptr);
	}
	return(result);
}

//Remove an input mapping
bool ClassEInput::RemoveKeyMapping(unsigned long nr)
{
	bool foundmapping = false;
	unsigned char ctr;
	for (ctr = 0; ctr < nummappings; ctr++)
		if ((keymapping[ctr] >> 16) == nr)
		{
			unsigned char subctr;
			keymapping[ctr] -= keymapping[ctr] & EIF_MAPPINGHOLD;
			EInput_CheckHoldState(keymapping, inputstate, ctr, nummappings);
			for (subctr = ctr; subctr < nummappings - 1; subctr++)
				keymapping[subctr] = keymapping[subctr + 1];
			nummappings--;
			foundmapping = true;
		}
	return(foundmapping);
}

//Remove an action mapping
bool ClassEInput::RemoveActionMapping(unsigned char action)
{
	bool foundmapping = false;
	unsigned char ctr;
	for (ctr = 0; ctr < nummappings; ctr++)
		if ((keymapping[ctr] & 0xFF) == action)
		{
			unsigned char subctr;
			keymapping[ctr] -= keymapping[ctr] & EIF_MAPPINGHOLD;
			EInput_CheckHoldState(keymapping, inputstate, ctr, nummappings);
			for (subctr = ctr; subctr < nummappings - 1; subctr++)
				keymapping[subctr] = keymapping[subctr + 1];
			nummappings--;
			foundmapping = true;
		}
	return(foundmapping);
}

//Return if the button is mapped to the specified action
bool ClassEInput::MappedTo(unsigned char action, unsigned long nr)
{
	unsigned char ctr;
	for (ctr = 0; ctr < nummappings; ctr++)
		if ((keymapping[ctr] & 0xFF) == action)
			if ((keymapping[ctr] >> 16) == nr)
				return(true);
	return(false);
}

//Return if a key is held
bool ClassEInput::KeyHold(unsigned char action)
{
	if (action < MAX_KEYACTIONS)
		if (inputstate[action] & 255)
			return(true);
	return(false);
}

//Return if the click device is held
bool ClassEInput::ClickHold(unsigned char button)
{
	if (button < MAX_CLICKID)
	{
		unsigned char ctr;
		if (inputstate[EIM_CLICK + EIC_ALL] & 255)
			return(true);
		for (ctr = 0; ctr < MAX_CLICKID; ctr++)
			if (ctr == button || button == EIC_ALL)
				if (inputstate[EIM_CLICK + ctr] & 255)
					return(true);
	}
	return(false);
}

//Check if there is an input event for the specified action
unsigned char ClassEInput::GetKeyEvent(unsigned char action)
{
	unsigned char ctr, event = EIE_NONE;
	if (action < MAX_KEYACTIONS)
		for (ctr = 0; ctr < numevents; ctr++)
			if (inputevent[ctr].action == action)
			{
				unsigned char subctr;
				event = inputevent[ctr].type;
				for (subctr = ctr; subctr < MAX_INPUTEVENTS - 1; subctr++)
					inputevent[subctr] = inputevent[subctr + 1];
				numevents--;
				break;
			}
	return(event);
}

//Check if there is an input event for the specified action
bool ClassEInput::GetKeyEvent(E_INPUTEVENT *event)
{
	unsigned char ctr, subctr;
	for (ctr = 0; ctr < numevents; ctr++)
		if (inputevent[ctr].action < MAX_KEYACTIONS)
		{
			ClassEMemory::Copy(event, &inputevent[ctr], sizeof(E_INPUTEVENT));
			for (subctr = ctr; subctr < MAX_INPUTEVENTS - 1; subctr++)
				inputevent[subctr] = inputevent[subctr + 1];
			numevents--;
			return(true);
		}
	return(false);
}

//Check if there is a click event for the specified action
bool ClassEInput::GetClickEvent(E_INPUTEVENT *event)
{
	unsigned char ctr, subctr;
	for (ctr = 0; ctr < numevents; ctr++)
		if (inputevent[ctr].action >= MAX_KEYACTIONS && inputevent[ctr].action < MAX_KEYACTIONS + MAX_CLICKID)
		{
			ClassEMemory::Copy(event, &inputevent[ctr], sizeof(E_INPUTEVENT));
			event->action -= MAX_KEYACTIONS;
			for (subctr = ctr; subctr < MAX_INPUTEVENTS - 1; subctr++)
				inputevent[subctr] = inputevent[subctr + 1];
			numevents--;
			return(true);
		}
	return(false);
}

//Retrieve a character from the typed character buffer
WCHAR ClassEInput::GetChar(unsigned long *scancode)
{
	if (numchar)
	{
		WCHAR chr = charevent[0];
		unsigned char ctr;
		if (scancode != NULL)
			*scancode = charevent_scancode[0];
		for (ctr = 0; ctr < numchar - 1; ctr++)
		{
			charevent[ctr] = charevent[ctr + 1];
			charevent_scancode[ctr] = charevent_scancode[ctr + 1];
		}
		numchar--;
		return(chr);
	}
	return(0);
}

//Put a character to the typed character buffer
void ClassEInput::PutChar(WCHAR chr, unsigned long scancode)
{
	#if defined(DEVICE_SMARTPHONE)
		if (chr == 13)
			return;
	#endif
	if (numchar < MAX_CHAREVENTS && !MappedTo(DIAM_KBTYPEKEY, scancode))
	{
		charevent[numchar] = chr;
		charevent_scancode[numchar] = scancode;
		numchar++;
	}
}

//Get the last pressed key
unsigned long ClassEInput::GetLastKey(void)
{
	return(lastkey);
}

//Get the position of the click device
void ClassEInput::GetClickPosition(long &x, long &y, unsigned char button)
{
	if (button < MAX_CLICKID)
	{
		x = clickx[button];
		y = clicky[button];
	}
	else
	{
		x = 0;
		y = 0;
	}
}

//Get the amount a wheel device has scrolled
long ClassEInput::GetWheelScroll(void)
{
	long wheelscroll = wheely;
	wheely = 0;
	return(wheelscroll);
}

//Get the angle of the tilt device
void ClassEInput::GetTiltAngle(long &x, long &y, long &z)
{
	x = tiltx;
	y = tilty;
	z = tiltz;
}

//Return the number of key events for the specified action
unsigned long ClassEInput::GetKeyPressCount(unsigned char action, bool includerepeat)
{
	unsigned long ctr, count = 0;
	for (ctr = 0; ctr < numevents; ctr++)
		if (inputevent[ctr].action == action)
			if (inputevent[ctr].type == EIE_PRESSED || (inputevent[ctr].type == EIE_REPEAT && includerepeat))
				count++;
	return(count);
}

//Return the number of click events for the specified button
unsigned long ClassEInput::GetClickPressCount(long &x, long &y, unsigned char button, bool includerepeat)
{
	unsigned long ctr, count = 0;
	GetClickPosition(x, y, button);
	for (ctr = 0; ctr < numevents; ctr++)
		if ((inputevent[ctr].action - MAX_KEYACTIONS == button && button != EIC_ALL) || (button == EIC_ALL && (inputevent[ctr].action - MAX_KEYACTIONS >= EIC_LEFT && inputevent[ctr].action - MAX_KEYACTIONS < MAX_CLICKID)))
			if (inputevent[ctr].type == EIE_PRESSED || (inputevent[ctr].type == EIE_REPEAT && includerepeat))
			{
				if (inputevent[ctr].type == EIE_PRESSED)
				{
					x = inputevent[ctr].clickx;
					y = inputevent[ctr].clicky;
				}
				count++;
			}
	return(count);
}

//Remove all events
void ClassEInput::ResetEvents(void)
{
	lastkey = 0;
	numevents = 0;
	numchar   = 0;
}

//Reset the input state of all devices
void ClassEInput::ResetState(void)
{
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_KEYACTIONS + 16; ctr++)
	{
		inputstate[ctr] = 0;
		ignorerelease[ctr] = 1;
	}
	for (ctr = 0; ctr < MAX_KEYMAPPINGS; ctr++)
		keymapping[ctr] -= keymapping[ctr] & EIF_MAPPINGHOLD;
	ResetEvents();
	wheely = 0;
}

//Change the rate of repeated key events
void ClassEInput::SetRepeatRates(unsigned long first, unsigned long next)
{
	firstrepeat = first;
	nextrepeat = next;
}

//Return the amount of tilt for a tiltdevice
ERESULT ClassEInput::GetTiltData(ECD *ecd, long *x, long *y, long *z)
{
	#if (EDITION_SDK < 500)
		return(E_NOFEATURE);
	#else
		#if defined(DEVICE_WINMOBILE)
			if (dll_tilt == NULL)
			{
				if (dlltiltnotfound)
					return(E_NOTEXISTS);
				dll_tilt = LoadLibrary(TEXT("TiltCONTROLDD.dll"));
				if (dll_tilt == NULL)
				{
					dlltiltnotfound = true;
					return(E_NOTEXISTS);
				}
			}
			if (!tiltinitialized)
			{
				typedef HANDLE (*InitializeTiltControlFunc)();
				InitializeTiltControlFunc Func;
				Func = (InitializeTiltControlFunc)GetProcAddress(dll_tilt, TEXT("?InitializeTiltControl@@YAPAXXZ"));
				if (Func != NULL)
				{
					htilt = (Func)();
					if (htilt == 0) //CANNOT_CONNECT_TO_SERIAL_PORT
						return(E_NOENTRY);
				}
				else
					return(E_TYPEMISMATCH);
				GetCurrentTiltControlDataFunc = (int(*)(HANDLE, TILTSENSORDATA *, int))GetProcAddress(dll_tilt, TEXT("?GetCurrentTiltControlData@@YAHPAXPAUTILTSENSORDATA@@H@Z"));
				if (Func == NULL)
					return(E_TYPEMISMATCH);
				tiltinitialized = true;
			}
			int result;
			TILTSENSORDATA tiltdata;
			result = (GetCurrentTiltControlDataFunc)(htilt, &tiltdata, 10);
			if (result != 2) //TILTCONTROL_INITIALISED_AND_CONNECTED
				return(E_NOTCONNECTED);
			if (x != NULL || y != NULL || z != NULL)
			{
				if (tiltdata.dHorizontalAngleOfTilt < -90)
					tiltdata.dHorizontalAngleOfTilt = -90;
				if (tiltdata.dHorizontalAngleOfTilt > 90)
					tiltdata.dHorizontalAngleOfTilt = 90;
				if (tiltdata.dVerticalAngleOfTilt < -90)
					tiltdata.dVerticalAngleOfTilt = -90;
				if (tiltdata.dVerticalAngleOfTilt > 90)
					tiltdata.dVerticalAngleOfTilt = 90;
				if (tiltdata.dZAxisAngleOfTilt < -90)
					tiltdata.dZAxisAngleOfTilt = -90;
				if (tiltdata.dZAxisAngleOfTilt > 90)
					tiltdata.dZAxisAngleOfTilt = 90;
				tiltx = (long)(tiltdata.dHorizontalAngleOfTilt * 65536 / 180);
				tilty = -(long)(tiltdata.dVerticalAngleOfTilt * 65536 / 180);
				tiltz = (long)(tiltdata.dZAxisAngleOfTilt * 65536 / 180);
				ecd->dsp->VirtualTilt(tiltx, tilty);
				tiltx >>= 5;
				tilty >>= 5;
				tiltz >>= 5;
				if (x != NULL)
					*x = tiltx;
				if (y != NULL)
					*y = tilty;
				if (z != NULL)
					*z = tiltz;
			}
			return(E_OK);
		#else
			#if (DEVICE_SYMBIAN >= 900) && defined(DEVICE_S60)
				if (tilteventfound)
				{
					if (x != NULL)
						*x = tiltx;
					if (y != NULL)
						*y = tilty;
					if (z != NULL)
						*z = tiltz;
					return(E_OK);
				}
				return(E_NOTEXISTS);
			#elif defined(DEVICE_IPHONE)
				if (x != NULL)
					*x = tiltx;
				if (y != NULL)
					*y = tilty;
				if (z != NULL)
					*z = tiltz;
				return(E_OK);
			#elif defined(DEVICE_LINUX)
				FILE *f;
				static const char devname[] = "/sys/class/i2c-adapter/i2c-3/3-001d/coord";
				struct stat sbuf;
				if (stat(devname, &sbuf) != 0)
					return(E_NOTEXISTS);
				f = fopen(devname, "rb");
				if (f)
				{
					char accelbuf[128];
					long angle[3] = { 0, 0, 0 };
					long actr, curpos = 0;
					if (sbuf.st_size > 127)
						sbuf.st_size = 127;
					fread(accelbuf, sbuf.st_size, 1, f);
					fclose(f);
					accelbuf[sbuf.st_size] = 0;
					for (actr = 0; actr < 3; actr++)
					{
						long spacepos = ClassEStd::StrFind(&accelbuf[curpos], " ");
						if (spacepos == -1)
							spacepos = ClassEStd::StrFind(&accelbuf[curpos], "\n");
						if (spacepos >= 0)
							accelbuf[curpos + spacepos] = 0;
						angle[actr] = ClassEStd::StrToInt(&accelbuf[curpos]);
						curpos += spacepos + 1;
						if (spacepos < 0)
							break;
					}
					tiltx = -angle[0];// * 1024 / 1024;
					tilty = -angle[1];// * 1024 / 1024;
					tiltz = -angle[2];// * 1024 / 1024;
					if (x != NULL)
						*x = tiltx;
					if (y != NULL)
						*y = tilty;
					if (z != NULL)
						*z = tiltz;
					return(E_OK);
				}
				return(E_ERROR);
			#endif
			return(E_UNSUPPORTED);
		#endif
	#endif
}

//Get a key name
void ClassEInput::GetKeyName(long keynr, char *name)
{
	WCHAR ustr_keyname[64];
	GetKeyName(keynr, ustr_keyname);
	ClassEStd::StrCpy(name, ustr_keyname);
}

//Get a key name
void ClassEInput::GetKeyName(long keynr, WCHAR *name)
{
	if (keynr == 0)
	{
		ClassEStd::StrCpy(name, "None");
		return;
	}
	ClassEStd::StrCpy(name, "#");
	ClassEStd::StrCat(name, keynr, 10, false);
	#if defined(DEVICE_OPENKODE)
		int keyid[] =
		{
			KD_INPUT_DPAD_UP, KD_INPUT_DPAD_DOWN, KD_INPUT_DPAD_LEFT, KD_INPUT_DPAD_RIGHT, KD_INPUT_DPAD_SELECT,
			KD_INPUT_GAMEKEYS_UP, KD_INPUT_GAMEKEYS_DOWN, KD_INPUT_GAMEKEYS_LEFT, KD_INPUT_GAMEKEYS_RIGHT,
			KD_INPUT_GAMEKEYS_FIRE, KD_INPUT_GAMEKEYS_A, KD_INPUT_GAMEKEYS_B, KD_INPUT_GAMEKEYS_C, KD_INPUT_GAMEKEYS_D,
			KD_INPUT_PHONEKEYPAD_0, KD_INPUT_PHONEKEYPAD_1, KD_INPUT_PHONEKEYPAD_2, KD_INPUT_PHONEKEYPAD_3, KD_INPUT_PHONEKEYPAD_4,
			KD_INPUT_PHONEKEYPAD_5, KD_INPUT_PHONEKEYPAD_6, KD_INPUT_PHONEKEYPAD_7, KD_INPUT_PHONEKEYPAD_8, KD_INPUT_PHONEKEYPAD_9,
			KD_INPUT_PHONEKEYPAD_STAR, KD_INPUT_PHONEKEYPAD_HASH, KD_INPUT_PHONEKEYPAD_LEFTSOFT, KD_INPUT_PHONEKEYPAD_RIGHTSOFT,
			KD_INPUT_TVREMOTE_RED_ATX, KD_INPUT_TVREMOTE_GREEN_ATX, KD_INPUT_TVREMOTE_YELLOW_ATX, KD_INPUT_TVREMOTE_BLUE_ATX,
			KD_INPUT_TVREMOTE_0_ATX, KD_INPUT_TVREMOTE_1_ATX, KD_INPUT_TVREMOTE_2_ATX, KD_INPUT_TVREMOTE_3_ATX, KD_INPUT_TVREMOTE_4_ATX,
			KD_INPUT_TVREMOTE_5_ATX, KD_INPUT_TVREMOTE_6_ATX, KD_INPUT_TVREMOTE_7_ATX, KD_INPUT_TVREMOTE_8_ATX, KD_INPUT_TVREMOTE_9_ATX,
			KD_INPUT_SPECIALKEYS_MENU_ATX, KD_INPUT_SPECIALKEYS_BACK_ATX
		};
		char keyidname[][32] =
		{
			"Up", "Down", "Left", "Right", "Enter",
			"Up", "Down", "Left", "Right",
			"Fire", "A", "B", "C", "D",
			"0", "1", "2", "3", "4",
			"5", "6", "7", "8", "9",
			"*", "#", "Softkey 1", "Softkey2",
			"Red", "Green", "Yellow", "Blue",
			"0", "1", "2", "3", "4",
			"5", "6", "7", "8", "9",
			"Menu", "Back"
		};
		int idctr;
		for (idctr = 0; idctr < (int)(sizeof(keyid) / sizeof(int)); idctr++)
			if (keyid[idctr] == keynr)
				ClassEStd::StrCpy(name, keyidname[idctr]);
		return;
	#endif
	if (keynr < MAX_KEYNAMECOUNT)
	{
		#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
			long tmp_prevkey = keynr;
		#endif
		keynr = ekeynamelist[keynr];
		if (keynr > 0 && keynr < MAX_KEYDESCCOUNT)
		{
			ClassEStd::StrCpy(name, &keydescriptor[keydescindex[keynr]]);
			#if defined(DEVICE_S60)
				if (ClassEStd::StrEqual(name, "Backspace", false))
					ClassEStd::StrCpy(name, "C");
			#endif
			#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
				if (tmp_prevkey >= 96 && tmp_prevkey < 106)
				{
					ClassEStd::StrCpy(name, "Num ");
					ClassEStd::StrCat(name, &keydescriptor[keydescindex[keynr]]);
				}
			#endif
		}
	}
}

//Load the key name descriptor list
void ClassEInput::LoadKeyDescriptor(const char *keylist)
{
	USTR_CHECK_BEGIN(keylist);
	LoadKeyDescriptor(ustr_keylist);
	USTR_CHECK_END(keylist);
}

//Load the key name descriptor list
void ClassEInput::LoadKeyDescriptor(const WCHAR *keylist)
{
	if (keydescriptor != NULL)
	{
		ClassEMemory::DeAlloc(keydescriptor);
		keydescriptor = NULL;
	}
	keydescriptor = (WCHAR *)ClassEMemory::Alloc((96 * 2) * sizeof(WCHAR) + (ClassEStd::StrLen(keylist) + 1) * sizeof(WCHAR));
	if (keydescriptor != NULL)
	{
		bool onparse;
		unsigned long ctr, len = ClassEStd::StrLen(keylist);
		unsigned char onkeydesc = 96;
		for (ctr = 0; ctr < 96; ctr++)
		{
			keydescriptor[ctr * 2] = (WCHAR)(ctr + 32);
			keydescriptor[ctr * 2 + 1] = 0;
			keydescindex[ctr] = ctr * 2;
		}
		for (ctr = 0; ctr < 64; ctr++)
			keydescindex[96 + ctr] = 0;
		onparse = false;
		for (ctr = 0; ctr < len; ctr++)
		{
			keydescriptor[192 + ctr] = keylist[ctr];
			if (keydescriptor[192 + ctr] == '\r' || keydescriptor[192 + ctr] == '\n')
			{
				keydescriptor[192 + ctr] = 0;
				if (onparse)
					onkeydesc++;
				onparse = false;
			}
			else
			{
				if (!onparse)
					if (onkeydesc < MAX_KEYDESCCOUNT)
						keydescindex[onkeydesc] = 192 + ctr;
				onparse = true;
			}
			keydescriptor[192 + ctr + 1] = 0;
		}
	}
}

//Set recommended action mappings
void ClassEInput::SetDefaultMappings(void)
{
	//Menu and miscellanious mappings
	#if defined(DEVICE_WIN32)
		AddKeyMapping(DIAM_ARROWUP,    VK_UP);
		AddKeyMapping(DIAM_ARROWDOWN,  VK_DOWN);
		AddKeyMapping(DIAM_ARROWLEFT,  VK_LEFT);
		AddKeyMapping(DIAM_ARROWRIGHT, VK_RIGHT);
		#if defined(DEVICE_DESKTOP)
			AddKeyMapping(DIAM_MENUUP,         2, VK_UP, 104);
			AddKeyMapping(DIAM_MENUDOWN,       2, VK_DOWN, 98);
			AddKeyMapping(DIAM_MENULEFT,       2, VK_LEFT, 100);
			AddKeyMapping(DIAM_MENURIGHT,      2, VK_RIGHT, 102);
			AddKeyMapping(DIAM_MENUCONFIRM,    3, VK_RETURN, VK_SPACE, 'Z');
			AddKeyMapping(DIAM_MENUCANCEL,     3, VK_ESCAPE, VK_BACK, 'X');
			AddKeyMapping(DIAM_MENUCONFIRM_SP, 2, VK_RETURN, VK_SPACE);
			AddKeyMapping(DIAM_MENUSOFT1,      7, VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6);
			AddKeyMapping(DIAM_MENUSOFT2,      7, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12);
		#elif defined(DEVICE_WINMOBILE)
			AddKeyMapping(DIAM_VOLUMEUP,       VK_F6);
			AddKeyMapping(DIAM_VOLUMEDOWN,     VK_F7);
			AddKeyMapping(DIAM_MENUUP,         VK_UP);
			AddKeyMapping(DIAM_MENUDOWN,       VK_DOWN);
			AddKeyMapping(DIAM_MENULEFT,       VK_LEFT);
			AddKeyMapping(DIAM_MENURIGHT,      VK_RIGHT);
			AddKeyMapping(DIAM_MENUCONFIRM,    4, VK_RETURN, VK_SPACE, 195, 196);
			AddKeyMapping(DIAM_MENUCANCEL,     4, VK_ESCAPE, VK_BACK, 193, 194);
			AddKeyMapping(DIAM_MENUCONFIRM_SP, 2, VK_RETURN, VK_SPACE);
			AddKeyMapping(DIAM_MENUSOFT1,      3, VK_F1, 193, 194);
			AddKeyMapping(DIAM_MENUSOFT2,      3, VK_F2, 195, 196);
			AddKeyMapping(DIAM_KBTYPEKEY,      VK_F23);
			#if defined(DEVICE_SMARTPHONE)
				AddKeyMapping(DIAM_SOFTKEY1,       VK_F1);
				AddKeyMapping(DIAM_SOFTKEY2,       VK_F2);
				AddKeyMapping(DIAM_MENUUP,         '2');
				AddKeyMapping(DIAM_MENUDOWN,       '8');
				AddKeyMapping(DIAM_MENULEFT,       '4');
				AddKeyMapping(DIAM_MENURIGHT,      '6');
				AddKeyMapping(DIAM_MENUCONFIRM,    '5');
				AddKeyMapping(DIAM_MENUCANCEL,     2, VK_F8, VK_F9);
				AddKeyMapping(DIAM_MENUCONFIRM_SP, '5');
				AddKeyMapping(DIAM_KBTYPEKEY,      '\r');
			#else
				AddKeyMapping(DIAM_SOFTKEY1,       193);
				AddKeyMapping(DIAM_SOFTKEY2,       194);
				AddKeyMapping(DIAM_SOFTKEY3,       195);
				AddKeyMapping(DIAM_SOFTKEY4,       196);
				AddKeyMapping(DIAM_MENUCONFIRM,    'Z');
				AddKeyMapping(DIAM_MENUCANCEL,     'X');
				AddKeyMapping(DIAM_MENUCONFIRM_SP, 'Z');
			#endif
		#elif defined(DEVICE_GIZMONDO)
			AddKeyMapping(DIAM_MENUUP,         VK_UP);
			AddKeyMapping(DIAM_MENUDOWN,       VK_DOWN);
			AddKeyMapping(DIAM_MENULEFT,       VK_LEFT);
			AddKeyMapping(DIAM_MENURIGHT,      VK_RIGHT);
			AddKeyMapping(DIAM_MENUCONFIRM,    2, VK_RETURN, VK_SPACE);
			AddKeyMapping(DIAM_MENUCANCEL,     2, VK_SHIFT, VK_CONTROL);
			AddKeyMapping(DIAM_MENUCONFIRM_SP, VK_RETURN);
			AddKeyMapping(DIAM_MENUSOFT1,      VK_TAB);
			AddKeyMapping(DIAM_MENUSOFT2,      VK_ESCAPE);
			AddKeyMapping(DIAM_KBTYPEKEY,      VK_RETURN);
		#endif
	#elif defined(DEVICE_SYMBIAN)
		AddKeyMapping(DIAM_ARROWUP,        EStdKeyUpArrow);
		AddKeyMapping(DIAM_ARROWDOWN,      EStdKeyDownArrow);
		AddKeyMapping(DIAM_ARROWLEFT,      EStdKeyLeftArrow);
		AddKeyMapping(DIAM_ARROWRIGHT,     EStdKeyRightArrow);
		AddKeyMapping(DIAM_MENUUP,         EStdKeyUpArrow);
		AddKeyMapping(DIAM_MENUDOWN,       EStdKeyDownArrow);
		AddKeyMapping(DIAM_MENULEFT,       EStdKeyLeftArrow);
		AddKeyMapping(DIAM_MENURIGHT,      EStdKeyRightArrow);
		AddKeyMapping(DIAM_MENUCONFIRM,    2, EStdKeyEnter, EStdKeySpace);
		AddKeyMapping(DIAM_MENUCANCEL,     2, EStdKeyBackspace, EStdKeyEscape);
		AddKeyMapping(DIAM_MENUCONFIRM_SP, 2, EStdKeyEnter, EStdKeySpace);
		#if defined(DEVICE_S60)
			AddKeyMapping(DIAM_SOFTKEY1,       EStdKeyDevice0);
			AddKeyMapping(DIAM_SOFTKEY2,       EStdKeyDevice1);
			AddKeyMapping(DIAM_MENUSOFT1,      EStdKeyDevice0);
			AddKeyMapping(DIAM_MENUSOFT2,      EStdKeyDevice1);
			AddKeyMapping(DIAM_MENUCONFIRM,    EStdKeyDevice3); //D-Pad
			AddKeyMapping(DIAM_MENUCANCEL,     2, '*', EStdKeyHash); //*, #
			AddKeyMapping(DIAM_MENUCONFIRM_SP, EStdKeyDevice3); //D-Pad
			AddKeyMapping(DIAM_KBTYPEKEY,      EStdKeyDevice3); //D-Pad
		#elif defined(DEVICE_S80)
			AddKeyMapping(DIAM_SOFTKEY1,       EStdKeyDevice0);
			AddKeyMapping(DIAM_SOFTKEY2,       EStdKeyDevice1);
			AddKeyMapping(DIAM_SOFTKEY3,       EStdKeyDevice2);
			AddKeyMapping(DIAM_SOFTKEY4,       EStdKeyDevice3);
			AddKeyMapping(DIAM_MENUSOFT1,      2, EStdKeyDevice0, EStdKeyDevice1);
			AddKeyMapping(DIAM_MENUSOFT2,      2, EStdKeyDevice2, EStdKeyDevice3);
			AddKeyMapping(DIAM_MENUCONFIRM,    2, EStdKeyDeviceA, 'Z'); //D-Pad, Z
			AddKeyMapping(DIAM_MENUCANCEL,     'X');
			AddKeyMapping(DIAM_MENUCONFIRM_SP, 2, EStdKeyDeviceA, 'Z'); //D-Pad, Z
			AddKeyMapping(DIAM_KBTYPEKEY,      EStdKeyDeviceA);         //D-Pad
		#elif defined(DEVICE_S90)
			AddKeyMapping(DIAM_SOFTKEY1,       EStdKeyDevice4);
			AddKeyMapping(DIAM_SOFTKEY2,       EStdKeyDevice5);
			AddKeyMapping(DIAM_SOFTKEY3,       EStdKeyEscape);
			AddKeyMapping(DIAM_MENUSOFT1,      2, EStdKeyDevice4, EStdKeyDevice5);
			AddKeyMapping(DIAM_MENUSOFT2,      EStdKeyEscape);
			AddKeyMapping(DIAM_MENUCONFIRM,    EStdKeyDevice7);         //D-Pad
			AddKeyMapping(DIAM_MENUCANCEL,     EStdKeyMenu);
			AddKeyMapping(DIAM_MENUCONFIRM_SP, EStdKeyDevice7);         //D-Pad
			AddKeyMapping(DIAM_KBTYPEKEY,      EStdKeyDevice7);         //D-Pad
		#elif defined(DEVICE_UIQ)
			AddKeyMapping(DIAM_MENULEFT,       178);                    //Jog down
			AddKeyMapping(DIAM_MENURIGHT,      177);                    //Jog up
			AddKeyMapping(DIAM_SOFTKEY1,       2, EStdKeyApplication1, EStdKeyF13); //Camera, Softkey
			AddKeyMapping(DIAM_SOFTKEY2,       2, EStdKeyApplication0, EStdKeyF14); //Internet, Softkey
			AddKeyMapping(DIAM_MENUSOFT1,      2, EStdKeyApplication1, EStdKeyF13); //Camera, Softkey
			AddKeyMapping(DIAM_MENUSOFT2,      2, EStdKeyApplication0, EStdKeyF14); //Internet, Softkey
			AddKeyMapping(DIAM_MENUCONFIRM,    2, EStdKeyDevice8, 212); //Jog
			AddKeyMapping(DIAM_MENUCANCEL,     2, EStdKeyApplication0, EStdKeyApplication1);
			AddKeyMapping(DIAM_MENUCONFIRM_SP, 2, EStdKeyDevice8, 212); //Jog
			AddKeyMapping(DIAM_KBTYPEKEY,      2, EStdKeyDevice8, 212); //Jog
		#endif
		#if !defined(DEVICE_S80)
			AddKeyMapping(DIAM_MENUUP,      '2');
			AddKeyMapping(DIAM_MENUDOWN,    '8');
			AddKeyMapping(DIAM_MENULEFT,    '4');
			AddKeyMapping(DIAM_MENURIGHT,   '6');
			AddKeyMapping(DIAM_MENUCONFIRM, '5');
			AddKeyMapping(DIAM_MENUCONFIRM_SP, '5');
		#endif
	#elif defined(DEVICE_LINUX)
		#if defined(DEVICE_GP2X) //9 = start, 10 = select
			AddKeyMapping(DIAM_ARROWUP,        1);
			AddKeyMapping(DIAM_ARROWDOWN,      5);
			AddKeyMapping(DIAM_ARROWLEFT,      3);
			AddKeyMapping(DIAM_ARROWRIGHT,     7);
			AddKeyMapping(DIAM_VOLUMEUP,       24);
			AddKeyMapping(DIAM_VOLUMEDOWN,     23);
			AddKeyMapping(DIAM_SOFTKEY1,       11);
			AddKeyMapping(DIAM_SOFTKEY2,       12);
			AddKeyMapping(DIAM_MENUUP,         1);
			AddKeyMapping(DIAM_MENUDOWN,       5);
			AddKeyMapping(DIAM_MENULEFT,       3);
			AddKeyMapping(DIAM_MENURIGHT,      7);
			AddKeyMapping(DIAM_MENUCONFIRM,    3, 28, 14, 16); //D-Pad, B, Y
			AddKeyMapping(DIAM_MENUCANCEL,     2, 13, 15);     //A, X
			AddKeyMapping(DIAM_MENUCONFIRM_SP, 3, 28, 14, 16); //D-Pad, B, Y
			AddKeyMapping(DIAM_MENUSOFT1,      11);            //L
			AddKeyMapping(DIAM_MENUSOFT2,      12);            //R
			AddKeyMapping(DIAM_KBTYPEKEY,      2, 28, 14);     //D-Pad + B
		#elif defined(DEVICE_EMBEDDEDLINUX)
			#if defined(DEVICE_MAEMO)
				AddKeyMapping(DIAM_ARROWUP,    37);
				AddKeyMapping(DIAM_ARROWDOWN,  40);
				AddKeyMapping(DIAM_ARROWLEFT,  38);
				AddKeyMapping(DIAM_ARROWRIGHT, 39);
				AddKeyMapping(DIAM_MENUUP,     37);
				AddKeyMapping(DIAM_MENUDOWN,   40);
				AddKeyMapping(DIAM_MENULEFT,   38);
				AddKeyMapping(DIAM_MENURIGHT,  39);
				AddKeyMapping(DIAM_MENUCONFIRM, 3, 13, 32, 'Z');
				AddKeyMapping(DIAM_MENUCANCEL,  3, 255, 8, 'X');
				AddKeyMapping(DIAM_MENUCONFIRM_SP, 2, 13, 32);
			#elif defined(DEVICE_ANDROID)
				// dpad up - 19
				// dpad down - 20
				// dpad left - 21
				// dpad right - 22
				// dpad ok - 23
				
				// backbutton - 4
				// call - 5
				// inbox/menu - 82
				// globe/search - 84
				
				// plus  - 24/25
				// minus - 25/24				
				// photo  - 27
				// menu
								
				// keypad menu - ?59
				// keypad search - ?127
				
				AddKeyMapping(DIAM_VOLUMEUP,       24);
				AddKeyMapping(DIAM_VOLUMEDOWN,     25);
				AddKeyMapping(DIAM_ARROWUP,        19);
				AddKeyMapping(DIAM_ARROWDOWN,      20);
				AddKeyMapping(DIAM_ARROWLEFT,      21);
				AddKeyMapping(DIAM_ARROWRIGHT,     22);
				AddKeyMapping(DIAM_MENUUP,         19);
				AddKeyMapping(DIAM_MENUDOWN,       20);
				AddKeyMapping(DIAM_MENULEFT,       21);
				AddKeyMapping(DIAM_MENURIGHT,      22);
				AddKeyMapping(DIAM_SOFTKEY1, 	   82);
				AddKeyMapping(DIAM_SOFTKEY2,       4);
				AddKeyMapping(DIAM_MENUCONFIRM,    23);
				AddKeyMapping(DIAM_MENUCONFIRM_SP, 23);
				AddKeyMapping(DIAM_MENUCANCEL,     4);
			#else
				#if defined(DEVICE_ALP)
					AddKeyMapping(DIAM_SOFTKEY1, 112);
					AddKeyMapping(DIAM_SOFTKEY2, 114);
					AddKeyMapping(DIAM_MENUCONFIRM, 13);
					AddKeyMapping(DIAM_MENUCANCEL, 8);
					AddKeyMapping(DIAM_MENUCONFIRM_SP, 13);
					AddKeyMapping(DIAM_MENUSOFT1, 112);
					AddKeyMapping(DIAM_MENUSOFT2, 114);
					AddKeyMapping(DIAM_KBTYPEKEY, 13);
					AddKeyMapping(DIAM_MENUUP,    '2'); //'2'
					AddKeyMapping(DIAM_MENUDOWN,  '8'); //'8'
					AddKeyMapping(DIAM_MENULEFT,  '3'); //'3'
					AddKeyMapping(DIAM_MENURIGHT, '5'); //'5'
				#else //temporary below, more devices needed (Android, OpenMoko, etc)
					AddKeyMapping(DIAM_SOFTKEY1,   229);
					AddKeyMapping(DIAM_SOFTKEY2,   102);
					//AddKeyMapping(DIAM_MENUCONFIRM,    2, KEY_ENTER, 232);
					//AddKeyMapping(DIAM_MENUCANCEL,     KEY_ESC);
					//AddKeyMapping(DIAM_MENUCONFIRM_SP, 2, KEY_ENTER, 232);
					AddKeyMapping(DIAM_MENUSOFT1,      229);
					AddKeyMapping(DIAM_MENUSOFT2,      102);
					AddKeyMapping(DIAM_KBTYPEKEY,      232);
				#endif
				AddKeyMapping(DIAM_ARROWUP,    111);
				AddKeyMapping(DIAM_ARROWDOWN,  42);
				AddKeyMapping(DIAM_ARROWLEFT,  14);
				AddKeyMapping(DIAM_ARROWRIGHT, 26);
				AddKeyMapping(DIAM_MENUUP,     111);
				AddKeyMapping(DIAM_MENUDOWN,   42);
				AddKeyMapping(DIAM_MENULEFT,   14);
				AddKeyMapping(DIAM_MENURIGHT,  26);
			#endif
		#else
			AddKeyMapping(DIAM_ARROWUP,    98);
			AddKeyMapping(DIAM_ARROWDOWN,  104);
			AddKeyMapping(DIAM_ARROWLEFT,  100);
			AddKeyMapping(DIAM_ARROWRIGHT, 102);
			AddKeyMapping(DIAM_MENUUP,     2, 98, 37);
			AddKeyMapping(DIAM_MENUDOWN,   2, 104, 40);
			AddKeyMapping(DIAM_MENULEFT,   2, 100, 38);
			AddKeyMapping(DIAM_MENURIGHT,  2, 102, 39);
			AddKeyMapping(DIAM_MENUCONFIRM, 3, 13, 32, 'Z');
			AddKeyMapping(DIAM_MENUCANCEL,  3, 27, 8, 'X');
			AddKeyMapping(DIAM_MENUCONFIRM_SP, 2, 13, 32);
			AddKeyMapping(DIAM_MENUSOFT1, 6, 112, 113, 114, 115, 116, 117);
			AddKeyMapping(DIAM_MENUSOFT2, 6, 118, 119, 120, 121, 122, 123);
		#endif
	#elif defined(DEVICE_OPENKODE)
		AddKeyMapping(DIAM_ARROWUP,        KD_INPUT_DPAD_UP);
		AddKeyMapping(DIAM_ARROWDOWN,      KD_INPUT_DPAD_DOWN);
		AddKeyMapping(DIAM_ARROWLEFT,      KD_INPUT_DPAD_LEFT);
		AddKeyMapping(DIAM_ARROWRIGHT,     KD_INPUT_DPAD_RIGHT);
		AddKeyMapping(DIAM_SOFTKEY1,       KD_INPUT_PHONEKEYPAD_LEFTSOFT);
		AddKeyMapping(DIAM_SOFTKEY2,       KD_INPUT_PHONEKEYPAD_RIGHTSOFT);
		AddKeyMapping(DIAM_MENUUP,         4, KD_INPUT_GAMEKEYS_UP, KD_INPUT_PHONEKEYPAD_2, KD_INPUT_TVREMOTE_2_ATX, KD_INPUT_DPAD_UP);
		AddKeyMapping(DIAM_MENUDOWN,       4, KD_INPUT_GAMEKEYS_DOWN, KD_INPUT_PHONEKEYPAD_8, KD_INPUT_TVREMOTE_8_ATX, KD_INPUT_DPAD_DOWN);
		AddKeyMapping(DIAM_MENULEFT,       4, KD_INPUT_GAMEKEYS_LEFT, KD_INPUT_PHONEKEYPAD_4, KD_INPUT_TVREMOTE_4_ATX, KD_INPUT_DPAD_LEFT);
		AddKeyMapping(DIAM_MENURIGHT,      4, KD_INPUT_GAMEKEYS_RIGHT, KD_INPUT_PHONEKEYPAD_6, KD_INPUT_TVREMOTE_6_ATX, KD_INPUT_DPAD_RIGHT);
		AddKeyMapping(DIAM_MENUCONFIRM,    4, KD_INPUT_GAMEKEYS_FIRE, KD_INPUT_PHONEKEYPAD_5, KD_INPUT_TVREMOTE_5_ATX, KD_INPUT_DPAD_SELECT);
		AddKeyMapping(DIAM_MENUCANCEL,     KD_INPUT_GAMEKEYS_A);
		AddKeyMapping(DIAM_MENUCONFIRM_SP, 3, KD_INPUT_GAMEKEYS_FIRE, KD_INPUT_PHONEKEYPAD_5, KD_INPUT_DPAD_SELECT);
		AddKeyMapping(DIAM_MENUSOFT1,      4, KD_INPUT_PHONEKEYPAD_LEFTSOFT, KD_INPUT_SPECIALKEYS_MENU_ATX, KD_INPUT_TVREMOTE_RED_ATX, KD_INPUT_TVREMOTE_GREEN_ATX);
		AddKeyMapping(DIAM_MENUSOFT2,      4, KD_INPUT_PHONEKEYPAD_RIGHTSOFT, KD_INPUT_SPECIALKEYS_BACK_ATX, KD_INPUT_TVREMOTE_YELLOW_ATX, KD_INPUT_TVREMOTE_BLUE_ATX);
		AddKeyMapping(DIAM_KBTYPEKEY,      KD_INPUT_DPAD_SELECT);
	#elif defined(DEVICE_MAC) && defined(DEVICE_DESKTOP)
		/*
		 AddKeyMapping(DIAM_MENUUP,         2, VK_UP, 104);
		 AddKeyMapping(DIAM_MENUDOWN,       2, VK_DOWN, 98);
		 AddKeyMapping(DIAM_MENULEFT,       2, VK_LEFT, 100);
		 AddKeyMapping(DIAM_MENURIGHT,      2, VK_RIGHT, 102);
		 AddKeyMapping(DIAM_MENUCONFIRM,    3, VK_RETURN, VK_SPACE, 'Z');
		 AddKeyMapping(DIAM_MENUCANCEL,     3, VK_ESCAPE, VK_BACK, 'X');
		 AddKeyMapping(DIAM_MENUCONFIRM_SP, 2, VK_RETURN, VK_SPACE);
		 AddKeyMapping(DIAM_MENUSOFT1,      7, VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6);
		 AddKeyMapping(DIAM_MENUSOFT2,      7, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12);
		*/

		AddKeyMapping(DIAM_ARROWUP,        30); //
		AddKeyMapping(DIAM_ARROWDOWN,      31); //
		AddKeyMapping(DIAM_ARROWLEFT,      28); //
		AddKeyMapping(DIAM_ARROWRIGHT,     29); //
		//AddKeyMapping(DIAM_VOLUMEUP,       24);
		//AddKeyMapping(DIAM_VOLUMEDOWN,     23);
		AddKeyMapping(DIAM_SOFTKEY1,       6, 112, 113, 114, 115, 116, 117);
		AddKeyMapping(DIAM_SOFTKEY2,       6, 118, 119, 120, 121, 122, 123);
		AddKeyMapping(DIAM_MENUUP,         30); //
		AddKeyMapping(DIAM_MENUDOWN,       31); //
		AddKeyMapping(DIAM_MENULEFT,       28); //
		AddKeyMapping(DIAM_MENURIGHT,      29); //
		AddKeyMapping(DIAM_MENUCONFIRM,    3, 13, 32, 90);
		AddKeyMapping(DIAM_MENUCANCEL,     3, 8, 53, 88);
		AddKeyMapping(DIAM_MENUCONFIRM_SP, 2, 13, 32);
		AddKeyMapping(DIAM_MENUSOFT1,      6, 112, 113, 114, 115, 116, 117);
		AddKeyMapping(DIAM_MENUSOFT2,      6, 118, 119, 120, 121, 122, 123);
	#endif

	//Game mappings
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		AddKeyMapping(DIAM_GAMEUP,      2, VK_UP, 104);
		AddKeyMapping(DIAM_GAMEDOWN,    2, VK_DOWN, 98);
		AddKeyMapping(DIAM_GAMELEFT,    2, VK_LEFT, 100);
		AddKeyMapping(DIAM_GAMERIGHT,   2, VK_RIGHT, 102);
		AddKeyMapping(DIAM_GAMEACTION1, 2, VK_SPACE, 'Z');
		AddKeyMapping(DIAM_GAMEACTION2, 'X');
		AddKeyMapping(DIAM_GAMEPAUSE,   3, VK_ESCAPE, VK_BACK, VK_RETURN);
	#elif defined(DEVICE_POCKETPC)
		AddKeyMapping(DIAM_GAMEUP,      VK_UP);
		AddKeyMapping(DIAM_GAMEDOWN,    VK_DOWN);
		AddKeyMapping(DIAM_GAMELEFT,    VK_LEFT);
		AddKeyMapping(DIAM_GAMERIGHT,   VK_RIGHT);
		AddKeyMapping(DIAM_GAMEACTION1, 3, 196, VK_SPACE, 'Z');
		AddKeyMapping(DIAM_GAMEACTION2, 2, 195, 'X');
		AddKeyMapping(DIAM_GAMEPAUSE,   6, VK_ESCAPE, VK_BACK, VK_F1, VK_F2, 193, 194);
	#elif defined(DEVICE_SMARTPHONE)
		AddKeyMapping(DIAM_GAMEUP,      2, VK_UP, '2');
		AddKeyMapping(DIAM_GAMEDOWN,    2, VK_DOWN, '8');
		AddKeyMapping(DIAM_GAMELEFT,    2, VK_LEFT, '4');
		AddKeyMapping(DIAM_GAMERIGHT,   2, VK_RIGHT, '6');
		AddKeyMapping(DIAM_GAMEACTION1, 2, VK_RETURN, '5');
		AddKeyMapping(DIAM_GAMEACTION2, 4, VK_F1, VK_F2, '1', '3');
		AddKeyMapping(DIAM_GAMEPAUSE,   4, VK_ESCAPE, VK_BACK, VK_F8, VK_F9);
	#elif defined(DEVICE_S60)
		AddKeyMapping(DIAM_GAMEUP,      2, EStdKeyUpArrow, '2');
		AddKeyMapping(DIAM_GAMEDOWN,    2, EStdKeyDownArrow, '8');
		AddKeyMapping(DIAM_GAMELEFT,    2, EStdKeyLeftArrow, '4');
		AddKeyMapping(DIAM_GAMERIGHT,   2, EStdKeyRightArrow, '6');
		AddKeyMapping(DIAM_GAMEACTION1, 2, EStdKeyDevice3, '5');
		AddKeyMapping(DIAM_GAMEACTION2, 4, EStdKeyDevice0, EStdKeyDevice1, '1', '3');
		AddKeyMapping(DIAM_GAMEPAUSE,   4, EStdKeyEscape, EStdKeyBackspace, '*', EStdKeyHash);
	#elif defined(DEVICE_S80)
		AddKeyMapping(DIAM_GAMEUP,      EStdKeyUpArrow);
		AddKeyMapping(DIAM_GAMEDOWN,    EStdKeyDownArrow);
		AddKeyMapping(DIAM_GAMELEFT,    EStdKeyLeftArrow);
		AddKeyMapping(DIAM_GAMERIGHT,   EStdKeyRightArrow);
		AddKeyMapping(DIAM_GAMEACTION1, 2, EStdKeySpace, 'Z');
		AddKeyMapping(DIAM_GAMEACTION2, 'X');
		AddKeyMapping(DIAM_GAMEPAUSE,   3, EStdKeyEnter, EStdKeyEscape, EStdKeyBackspace);
	#elif defined(DEVICE_S90)
		AddKeyMapping(DIAM_GAMEUP,      2, EStdKeyUpArrow, '2');
		AddKeyMapping(DIAM_GAMEDOWN,    2, EStdKeyDownArrow, '8');
		AddKeyMapping(DIAM_GAMELEFT,    2, EStdKeyLeftArrow, '4');
		AddKeyMapping(DIAM_GAMERIGHT,   2, EStdKeyRightArrow, '6');
		AddKeyMapping(DIAM_GAMEACTION1, EStdKeyDevice4);
		AddKeyMapping(DIAM_GAMEACTION2, EStdKeyDevice5);
		AddKeyMapping(DIAM_GAMEPAUSE,   EStdKeyMenu);
	#elif defined(DEVICE_UIQ)
		AddKeyMapping(DIAM_GAMEUP,      2, EStdKeyUpArrow, '2');
		AddKeyMapping(DIAM_GAMEDOWN,    2, EStdKeyDownArrow, '8');
		AddKeyMapping(DIAM_GAMELEFT,    3, EStdKeyLeftArrow, '4', 178);
		AddKeyMapping(DIAM_GAMERIGHT,   3, EStdKeyRightArrow, '6', 177);
		AddKeyMapping(DIAM_GAMEACTION1, 2, EStdKeyDevice8, '5');
		AddKeyMapping(DIAM_GAMEACTION2, 3, EStdKeyApplication1, '1', '3');
		AddKeyMapping(DIAM_GAMEPAUSE,   5, EStdKeyEscape, EStdKeyBackspace, EStdKeyApplication0, '*', EStdKeyHash);
	#elif defined(DEVICE_GP2X)
		AddKeyMapping(DIAM_GAMEUP,      1);
		AddKeyMapping(DIAM_GAMEDOWN,    5);
		AddKeyMapping(DIAM_GAMELEFT,    3);
		AddKeyMapping(DIAM_GAMERIGHT,   7);
		AddKeyMapping(DIAM_GAMEACTION1, 3, 14, 15, 11);
		AddKeyMapping(DIAM_GAMEACTION2, 4, 13, 16, 12, 10);
		AddKeyMapping(DIAM_GAMEPAUSE,   9);
	#elif defined(DEVICE_ALP)
		AddKeyMapping(DIAM_GAMEUP,      2, 111, 11);
		AddKeyMapping(DIAM_GAMEDOWN,    2, 116, 17);
		AddKeyMapping(DIAM_GAMELEFT,    2, 113, 13);
		AddKeyMapping(DIAM_GAMERIGHT,   2, 114, 15);
		AddKeyMapping(DIAM_GAMEACTION1, 3, 36, 67, 10);
		AddKeyMapping(DIAM_GAMEACTION2, 2, 69, 12);
		AddKeyMapping(DIAM_GAMEPAUSE,   2, 68, 22);
	#elif defined(DEVICE_MAEMO)
		AddKeyMapping(DIAM_GAMEUP,      37);
		AddKeyMapping(DIAM_GAMEDOWN,    40);
		AddKeyMapping(DIAM_GAMELEFT,    38);
		AddKeyMapping(DIAM_GAMERIGHT,   39);
		AddKeyMapping(DIAM_GAMEACTION1, 2, 13, 'Z');
		AddKeyMapping(DIAM_GAMEACTION2, 2, 32, 'X');
		AddKeyMapping(DIAM_GAMEPAUSE,   2, 27, 255);
	#elif defined(DEVICE_OPENKODE)
		AddKeyMapping(DIAM_GAMEUP,      2, KD_INPUT_GAMEKEYS_UP, KD_INPUT_PHONEKEYPAD_2);
		AddKeyMapping(DIAM_GAMEDOWN,    2, KD_INPUT_GAMEKEYS_DOWN, KD_INPUT_PHONEKEYPAD_8);
		AddKeyMapping(DIAM_GAMELEFT,    2, KD_INPUT_GAMEKEYS_LEFT, KD_INPUT_PHONEKEYPAD_4);
		AddKeyMapping(DIAM_GAMERIGHT,   2, KD_INPUT_GAMEKEYS_RIGHT, KD_INPUT_PHONEKEYPAD_6);
		AddKeyMapping(DIAM_GAMEACTION1, 2, KD_INPUT_GAMEKEYS_FIRE, KD_INPUT_PHONEKEYPAD_5);
		AddKeyMapping(DIAM_GAMEACTION2, 3, KD_INPUT_GAMEKEYS_A, KD_INPUT_PHONEKEYPAD_1, KD_INPUT_PHONEKEYPAD_3);
		AddKeyMapping(DIAM_GAMEPAUSE,   5, KD_INPUT_GAMEKEYS_B, KD_INPUT_PHONEKEYPAD_LEFTSOFT, KD_INPUT_PHONEKEYPAD_RIGHTSOFT, KD_INPUT_PHONEKEYPAD_STAR, KD_INPUT_PHONEKEYPAD_HASH);
	#elif defined(DEVICE_GIZMONDO)
		AddKeyMapping(DIAM_GAMEUP,      VK_UP);
		AddKeyMapping(DIAM_GAMEDOWN,    VK_DOWN);
		AddKeyMapping(DIAM_GAMELEFT,    VK_LEFT);
		AddKeyMapping(DIAM_GAMERIGHT,   VK_RIGHT);
		AddKeyMapping(DIAM_GAMEACTION1, 2, VK_SHIFT, VK_SPACE);
		AddKeyMapping(DIAM_GAMEACTION2, VK_RETURN);
		AddKeyMapping(DIAM_GAMEPAUSE,   VK_CONTROL);
	#endif
}

//Get the button mapping list
void ClassEInput::GetButtonList(EBUTTONLIST *blist)
{
	ClassEMemory::Set(blist, 0, sizeof(EBUTTONLIST));
	#if defined(DEVICE_SYMBIAN)
		blist->Home         = 0;
		blist->KeyEnter     = EStdKeyEnter;
		blist->KeyShift     = EStdKeyLeftShift;
		blist->KeyShift2    = EStdKeyRightShift;
		blist->KeyControl   = EStdKeyLeftCtrl;
		blist->KeyControl2  = EStdKeyRightCtrl;
		blist->KeyEscape    = EStdKeyEscape;
		blist->KeySpace     = EStdKeySpace;
		blist->KeyBackspace = EStdKeyBackspace;
		blist->KeyTab       = EStdKeyTab;
		blist->KeyPageup    = EStdKeyPageUp;
		blist->KeyPagedown  = EStdKeyPageDown;
		blist->KeyInsert    = EStdKeyInsert;
		blist->KeyDelete    = EStdKeyDelete;
		blist->KeyHome      = EStdKeyHome;
		blist->KeyEnd       = EStdKeyEnd;
		blist->KeyMenu      = EStdKeyMenu;
		blist->DPadUp       = EStdKeyUpArrow;
		blist->DPadDown     = EStdKeyDownArrow;
		blist->DPadLeft     = EStdKeyLeftArrow;
		blist->DPadRight    = EStdKeyRightArrow;
	#elif defined(DEVICE_LINUX)
		#if !defined(DEVICE_GP2X)
			blist->Home         = 0;
			blist->KeyEnter     = 13;
			blist->KeyShift     = 16;
			blist->KeyShift2    = 0;
			blist->KeyControl   = 17;
			blist->KeyControl2  = 0;
			blist->KeyEscape    = 27;
			blist->KeySpace     = 32;
			blist->KeyBackspace = 8;
			blist->KeyTab       = 0;
			blist->KeyPageup    = 0;
			blist->KeyPagedown  = 0;
			blist->KeyInsert    = 0;
			blist->KeyDelete    = 0;
			blist->KeyHome      = 0;
			blist->KeyEnd       = 0;
			blist->KeyMenu      = 0;
			blist->DPadUp       = 37;
			blist->DPadDown     = 40;
			blist->DPadLeft     = 38;
			blist->DPadRight    = 39;
		#else
			blist->DPadUp       = 1;
			blist->DPadDown     = 5;
			blist->DPadLeft     = 3;
			blist->DPadRight    = 7;
		#endif
	#elif defined(DEVICE_OPENKODE)
		blist->Home         = 0;
		blist->KeyEnter     = KD_INPUT_DPAD_SELECT;
		blist->KeyShift     = 0;
		blist->KeyShift2    = 0;
		blist->KeyControl   = 0;
		blist->KeyControl2  = 0;
		blist->KeyEscape    = 0;
		blist->KeySpace     = 0;
		blist->KeyBackspace = 0;
		blist->KeyTab       = 0;
		blist->KeyPageup    = 0;
		blist->KeyPagedown  = 0;
		blist->KeyInsert    = 0;
		blist->KeyDelete    = 0;
		blist->KeyHome      = 0;
		blist->KeyEnd       = 0;
		blist->KeyMenu      = 0;
		blist->DPadUp       = KD_INPUT_DPAD_UP;
		blist->DPadDown     = KD_INPUT_DPAD_DOWN;
		blist->DPadLeft     = KD_INPUT_DPAD_LEFT;
		blist->DPadRight    = KD_INPUT_DPAD_RIGHT;
	#elif defined(DEVICE_MAC) && defined(DEVICE_DESKTOP)
		blist->Home         = 5;
		blist->KeyEnter     = 13;
		blist->KeyShift     = 0;
		blist->KeyShift2    = 0;
		blist->KeyControl   = 0;
		blist->KeyControl2  = 0;
		blist->KeyEscape    = 27;
		blist->KeySpace     = 32;
		blist->KeyBackspace = 8;
		blist->KeyTab       = 9;
		blist->KeyPageup    = 18;
		blist->KeyPagedown  = 19;
		blist->KeyInsert    = 0;
		blist->KeyDelete    = 11;
		blist->KeyHome      = 5;
		blist->KeyEnd       = 17;
		blist->KeyMenu      = 0;
		blist->DPadUp       = 30;
		blist->DPadDown     = 31;
		blist->DPadLeft     = 28;
		blist->DPadRight    = 29;
	#elif defined(DEVICE_IPHONE)
		blist->Home         = 0;
		blist->KeyEnter     = 0;
		blist->KeyShift     = 0;
		blist->KeyShift2    = 0;
		blist->KeyControl   = 0;
		blist->KeyControl2  = 0;
		blist->KeyEscape    = 0;
		blist->KeySpace     = 0;
		blist->KeyBackspace = 0;
		blist->KeyTab       = 0;
		blist->KeyPageup    = 0;
		blist->KeyPagedown  = 0;
		blist->KeyInsert    = 0;
		blist->KeyDelete    = 0;
		blist->KeyHome      = 0;
		blist->KeyEnd       = 0;
		blist->KeyMenu      = 0;
		blist->DPadUp       = 0;
		blist->DPadDown     = 0;
		blist->DPadLeft     = 0;
		blist->DPadRight    = 0;
	#elif defined(DEVICE_WIN32)
		#if defined(DEVICE_WINMOBILE)
			blist->Home = VK_LWIN;
		#elif defined(DEVICE_GIZMONDO)
			blist->Home = VK_F1;
		#else
			blist->Home = 0;
		#endif
		blist->KeyEnter     = VK_RETURN;
		blist->KeyShift     = VK_SHIFT;
		blist->KeyShift2    = VK_SHIFT;
		blist->KeyControl   = VK_CONTROL;
		blist->KeyControl2  = VK_CONTROL;
		blist->KeyEscape    = VK_ESCAPE;
		blist->KeySpace     = VK_SPACE;
		blist->KeyBackspace = VK_BACK;
		blist->KeyTab       = VK_TAB;
		blist->KeyPageup    = VK_PRIOR;
		blist->KeyPagedown  = VK_NEXT;
		blist->KeyInsert    = VK_INSERT;
		blist->KeyDelete    = VK_DELETE;
		blist->KeyHome      = VK_HOME;
		blist->KeyEnd       = VK_END;
		blist->KeyMenu      = 0;
		blist->DPadUp       = VK_UP;
		blist->DPadDown     = VK_DOWN;
		blist->DPadLeft     = VK_LEFT;
		blist->DPadRight    = VK_RIGHT;
	#endif
	#if defined(DEVICE_UIQ)
		blist->JogLeft  = EStdKeyDeviceE;
		blist->JogRight = EStdKeyDeviceD;
	#else
		blist->JogLeft  = 0;
		blist->JogRight = 0;
	#endif
	#if defined(DEVICE_SERIES60)
		blist->ButtonA     = '5';               //5 number key
		blist->ButtonB     = '6';               //6 number key
		blist->ButtonC     = '7';               //7 number key
		blist->ButtonStart = EStdKeyDevice3;    //The button near or inside the d-pad
		blist->Soft1       = EStdKeyDevice0;    //Softbutton left
		blist->Soft2       = EStdKeyDevice1;    //Softbutton right
		blist->Soft3       = 0;
		blist->Soft4       = 0;
	#elif defined(DEVICE_SERIES80)
		blist->ButtonA     = 'Z';               //Z ascii key
		blist->ButtonB     = 'X';               //X ascii key
		blist->ButtonC     = 'C';               //C ascii key
		blist->ButtonStart = EStdKeyDeviceA;    //The button inside the d-pad
		blist->Soft1       = EStdKeyDevice0;    //Softbutton 1st
		blist->Soft2       = EStdKeyDevice1;    //Softbutton 2nd
		blist->Soft3       = EStdKeyDevice2;    //Softbutton 3rd
		blist->Soft4       = EStdKeyDevice3;    //Softbutton 4th
	#elif defined(DEVICE_SERIES90)
		blist->ButtonA     = EStdKeyDevice7;    //The button inside the d-pad
		blist->ButtonB     = EStdKeyDevice4;    //Right button (1st)
		blist->ButtonC     = EStdKeyDevice5;    //Right button (2nd)
		blist->ButtonStart = EStdKeyMenu;       //The .= button
		blist->Soft1       = EStdKeyDevice4;    //Right button (1st)
		blist->Soft2       = EStdKeyDevice5;    //Right button (2nd)
		blist->Soft3       = 0;
		blist->Soft4       = 0;
	#elif defined(DEVICE_UIQ)
		#if (DEVICE_SYMBIAN < 900)
			blist->ButtonA     = EStdKeyDevice8;      //Jog inwards
			blist->ButtonB     = EStdKeyApplication1; //Camera shortcut key
			blist->ButtonC     = EStdKeyApplication0; //Internet shortcut key
			blist->ButtonStart = EStdKeyDevice9;      //Menu
			blist->Soft1       = EStdKeyApplication1; //Camera shortcut key
			blist->Soft2       = EStdKeyApplication0; //Internet shortcut key
		#else
			blist->ButtonA     = EStdKeyF13;          //Softkey left
			blist->ButtonB     = EStdKeyF14;          //Softkey right
			blist->ButtonC     = EStdKeyHash;         //#
			blist->ButtonStart = EStdKeyDevice8;      //Jog/CSK inwards
			blist->Soft1       = EStdKeyF13;          //Camera shortcut key
			blist->Soft2       = EStdKeyF14;          //Internet shortcut key
		#endif
		blist->Soft3       = 0;
		blist->Soft4       = 0;
	#elif defined(DEVICE_LINUX)
		#if defined(DEVICE_GP2X)
			blist->ButtonA     = 13;                  //A key
			blist->ButtonB     = 14;                  //B key
			blist->ButtonC     = 15;                  //X key
			blist->ButtonStart = 9;                   //Start key
			blist->Soft1       = 11;                  //L button
			blist->Soft2       = 12;                  //R button
			blist->Soft3       = 0;
			blist->Soft4       = 0;
		#else
			blist->ButtonA     = 'Z';
			blist->ButtonB     = 'X';
			blist->ButtonC     = 'C';
			#if defined(DEVICE_DESKTOP)
				blist->Soft1       = '1';
				blist->Soft2       = '2';
				blist->Soft3       = '3';
				blist->Soft4       = '4';
			#else
				blist->Soft1       = '0';
				blist->Soft2       = '0';
				blist->Soft3       = '0';
				blist->Soft4       = '0';
			#endif
			blist->ButtonStart = 13;
		#endif
	#elif defined(DEVICE_MAC) && defined(DEVICE_DESKTOP)
		blist->ButtonA     = 13;                  //A key
		blist->ButtonB     = 14;                  //B key
		blist->ButtonC     = 15;                  //X key
		blist->ButtonStart = 9;                   //Start key
		blist->Soft1       = 11;                  //L button
		blist->Soft2       = 12;                  //R button
		blist->Soft3       = 0;
		blist->Soft4       = 0;
	#elif defined(DEVICE_IPHONE)
		blist->ButtonA     = 0;
		blist->ButtonB     = 0;
		blist->ButtonC     = 0;
		blist->ButtonStart = 0;
		blist->Soft1       = 0;
		blist->Soft2       = 0;
		blist->Soft3       = 0;
		blist->Soft4       = 0;
	#elif defined(DEVICE_WINMOBILE)
		#if defined(DEVICE_POCKETPC)
			blist->ButtonA     = 193;
			blist->ButtonB     = 194;
			blist->ButtonC     = 195;
			blist->ButtonStart = VK_RETURN;
			blist->Soft1       = 193;
			blist->Soft2       = 194;
			blist->Soft3       = 195;
			blist->Soft4       = 196;
		#else
			blist->ButtonA     = 112;
			blist->ButtonB     = 113;
			blist->ButtonC     = '*';
			blist->ButtonStart = VK_RETURN;
			blist->Soft1       = 112;
			blist->Soft2       = 113;
			blist->Soft3       = 0;
			blist->Soft4       = 0;
		#endif
	#else
		#if defined(DEVICE_GIZMONDO)
			blist->ButtonA     = VK_CONTROL;
			blist->ButtonB     = VK_SPACE;
			blist->ButtonC     = VK_SHIFT;
			blist->Soft1       = VK_TAB;
			blist->Soft2       = VK_ESCAPE;
			blist->Soft3       = 0;
			blist->Soft4       = 0;
			blist->ButtonStart = VK_RETURN;
		#elif defined(DEVICE_WIN32)
			blist->ButtonA     = 'Z';
			blist->ButtonB     = 'X';
			blist->ButtonC     = 'C';
			blist->Soft1       = '1';
			blist->Soft2       = '2';
			blist->Soft3       = '3';
			blist->Soft4       = '4';
			blist->ButtonStart = VK_RETURN;
		#endif
	#endif
}

//Check input repeat rates
void ClassEInput::CheckRepeat(unsigned long timedelta)
{
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_KEYACTIONS + 16; ctr++)
		if (inputstate[ctr] & 255)
		{
			unsigned long ktime;
			if (65535 - (inputstate[ctr] >> 16) < timedelta)
				inputstate[ctr] |= 0xFFFF0000;
			else
				inputstate[ctr] += (timedelta << 16);
			ktime = inputstate[ctr] >> 16;
			if ((ktime >= firstrepeat && ((inputstate[ctr] & 0xFF00) >> 8) < 1) || (ktime >= nextrepeat && ((inputstate[ctr] & 0xFF00) >> 8) >= 1))
			{
				inputstate[ctr] = (inputstate[ctr] & 255) + (inputstate[ctr] & 0xFF00) + 256;
				if (numevents < MAX_INPUTEVENTS)
				{
					inputevent[numevents].action = ctr;
					inputevent[numevents].type = EIE_REPEAT;
					inputevent[numevents].clickx = clickx[0];
					inputevent[numevents].clicky = clicky[0];
					numevents++;
				}
			}
		}
}

//Pass button down event
void ClassEInput::OnKeyDown(unsigned long nr)
{
	if (nr == 0)
		return;
	unsigned char ctr;
	lastkey = nr;
	for (ctr = 0; ctr < nummappings; ctr++)
	{
		if ((keymapping[ctr] >> 16) == nr)
		{
			bool justpress = false;
			unsigned char action = (unsigned char)(keymapping[ctr] & 255);
			keymapping[ctr] |= EIF_MAPPINGHOLD;
			if (action < MAX_KEYACTIONS)
			{
				if ((inputstate[action] & 255) == 0)
				{
					inputstate[action] = 1;
					justpress = true;
				}
				ignorerelease[action] = 0;
			}
			if (action < MAX_KEYACTIONS && numevents < MAX_INPUTEVENTS && justpress)
			{
				inputevent[numevents].action = action;
				inputevent[numevents].type = EIE_PRESSED;
				inputevent[numevents].clickx = clickx[0];
				inputevent[numevents].clicky = clicky[0];
				numevents++;
			}
		}
	}
}

//Pass button up event
void ClassEInput::OnKeyUp(unsigned long nr)
{
	if (nr == 0)
		return;
	unsigned char ctr;
	for (ctr = 0; ctr < nummappings; ctr++)
		if ((keymapping[ctr] >> 16) == nr)
		{
			bool justreleased = false;
			unsigned char action = (unsigned char)(keymapping[ctr] & 255);
			if (action < MAX_KEYACTIONS)
			{
				if (!ignorerelease[action])
				{
					keymapping[ctr] -= keymapping[ctr] & EIF_MAPPINGHOLD;
					justreleased = EInput_CheckHoldState(keymapping, inputstate, ctr, nummappings);
				}
			}
			if (action < MAX_KEYACTIONS && numevents < MAX_INPUTEVENTS && justreleased)
			{
				if (!ignorerelease[action])
				{
					inputevent[numevents].action = action;
					inputevent[numevents].type = EIE_RELEASED;
					inputevent[numevents].clickx = clickx[0];
					inputevent[numevents].clicky = clicky[0];
					numevents++;
				}
			}
		}
}

//Pass click down event
void ClassEInput::OnClickDown(long x, long y, unsigned char button)
{
	clickx[0] = x;
	clicky[0] = y;
	if (button < MAX_CLICKID)
	{
		clickx[button] = x;
		clicky[button] = y;
		if ((inputstate[EIM_CLICK + button] & 255) == 0)
			inputstate[EIM_CLICK + button] = 1;
		ignorerelease[EIM_CLICK + button] = 0;
		if (numevents < MAX_INPUTEVENTS)
		{
			inputevent[numevents].action = EIM_CLICK + button;
			inputevent[numevents].type = EIE_PRESSED;
			inputevent[numevents].clickx = clickx[button];
			inputevent[numevents].clicky = clicky[button];
			numevents++;
		}
	}
}

//Pass click up event
void ClassEInput::OnClickUp(long x, long y, unsigned char button)
{
	clickx[0] = x;
	clicky[0] = y;
	if (button < MAX_CLICKID)
	{
		clickx[button] = x;
		clicky[button] = y;
		inputstate[EIM_CLICK + button] = 0;
		if (numevents < MAX_INPUTEVENTS && !ignorerelease[EIM_CLICK + button])
		{
			inputevent[numevents].action = EIM_CLICK + button;
			inputevent[numevents].type = EIE_RELEASED;
			inputevent[numevents].clickx = clickx[button];
			inputevent[numevents].clicky = clicky[button];
			numevents++;
		}
	}
}

//Pass doubleclick event
void ClassEInput::OnClickDouble(long x, long y, unsigned char button)
{
	clickx[0] = x;
	clicky[0] = y;
	if (button < MAX_CLICKID)
	{
		clickx[button] = x;
		clicky[button] = y;
		if ((inputstate[EIM_CLICK + button] & 255) == 0)
			inputstate[EIM_CLICK + button] = 1;
		if (numevents < MAX_INPUTEVENTS)
		{
			inputevent[numevents].action = EIM_CLICK + button;
			inputevent[numevents].type = EIE_DOUBLEPRESSED;
			inputevent[numevents].clickx = clickx[button];
			inputevent[numevents].clicky = clicky[button];
			numevents++;
		}
	}
}

//Pass clickdevice move event
void ClassEInput::OnClickMove(long x, long y, unsigned char button)
{
	clickx[0] = x;
	clicky[0] = y;
	if (button < MAX_CLICKID)
	{
		clickx[button] = x;
		clicky[button] = y;
	}
}

//Pass wheeldevice scroll event
void ClassEInput::OnWheelScroll(long direction)
{
	if (direction < 0)
		wheely--;
	else if (direction > 0)
		wheely++;
}

//Pass tiltdevice event
void ClassEInput::OnTilt(long x, long y, long z)
{
	tiltx = x;
	tilty = y;
	tiltz = z;
	tilteventfound = true;
}
