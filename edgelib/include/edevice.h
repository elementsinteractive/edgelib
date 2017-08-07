// edevice.h
// A module for retrieving device specific information
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED     //Can only be included by the main engine
#ifndef EDEVICE_INCLUDED  //Make sure the module isn't included multiple times
#define EDEVICE_INCLUDED

///////////////////////////////////////////////////////////////////
// Type definitions                                              //
///////////////////////////////////////////////////////////////////

#define BATT_ERROR            0
#define BATT_UNSUPPORTED      1
#define BATT_NOTFOUND         2
#define BATT_CHARGING         3
#define BATT_BATTERYPOWERED   4
#define BATT_ACPOWERED        5

///////////////////////////////////////////////////////////////////
// Classes                                                       //
///////////////////////////////////////////////////////////////////

class ClassEDevice
{
	private:
		char cachedid[MAX_ESTR];
		unsigned long mVibrateTimer;
		bool iscachedid;
		void static GetBatteryInfo(unsigned char &level, unsigned char &status);
	public:
		ClassEDevice(void);
		~ClassEDevice(void);
		void static CurrentTime(unsigned char &hour, unsigned char &minute, unsigned char &second, bool islocal = true);
		void static CurrentDate(unsigned long &year, unsigned char &month, unsigned char &day, bool islocal = true);
		ETYPE_TIMESTAMP static TimestampNow(bool islocal = true);
		long static LocalTimeOffset(void);
		unsigned long static GetModelID(void);
		void static GetModelName(char *modelname);
		void static GetModelName(WCHAR *modelname);
		void static GetOSName(char *osname);
		void static GetOSName(WCHAR *osname);
		unsigned char static GetBatteryLevel(void);
		unsigned char static GetBatteryStatus(void);
		char *GetOwnerName(char *owner);
		WCHAR *GetOwnerName(WCHAR *owner);
		char *GetIMEI(char *imeicode, bool includedashes = false);
		WCHAR *GetIMEI(WCHAR *imeicode, bool includedashes = false);
		char *GetSerialNumber(char *serialid);
		WCHAR *GetSerialNumber(WCHAR *serialid);
		void Vibrate(void);
		char *GetLanguage(char *language);
		char *GetLocale(char *locale);
		void Update(unsigned long timedelta);
};//The device class

#endif
#endif
