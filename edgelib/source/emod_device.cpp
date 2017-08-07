// emod_device.cpp
// Device class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
	#include <stdlib.h>
#elif defined(DEVICE_SYMBIAN)
	#if defined(SYMBIAN_ETEL3RDPARTY)
		#include <Etel3rdParty.h>
	#endif
	#if !defined(SYMBIAN_ALTERNATIVE_IMEI)
		#include <plpvariant.h>
	#endif
	#include <hal.h>
#elif defined(DEVICE_LINUX)
	#include <sys/mman.h>
	#include <fcntl.h>
	#include <time.h>
	#if defined(DEVICE_GP2X)
		#define I2CCON   (0xD00>>1)
		#define I2CSTAT  (0xD02>>1)
		#define I2CADDR  (0xD04>>1)
		#define I2CDATA  (0xD06>>1)
		#define DEVICE   0xA0
	#elif defined(DEVICE_MAEMO) && !defined(DEVICE_MAEMOSIM)
		#include <dbus/dbus-glib.h>
		#include <glib.h>
	#elif defined(DEVICE_ANDROID)
		char * AndroidGetDeviceModel();
		char * AndroidGetDeviceID();
		int AndroidGetDeviceOS();
		char * AndroidGetDeviceName();
		void Call_EdgelibUtil_vibrate();
		int Call_EdgelibUtil_getBatteryLevel();
		int Call_EdgelibUtil_getBatteryState();
		void Call_EdgelibUtil_getIMEI(char * output);
		void Call_EdgelibUtil_getSerial(char * output);
		void Call_EdgelibUtil_getLocale(char * output);
		int Call_EdgelibUtil_getTimezoneOffset();
	#endif
	void ecCamPoll();
#elif defined(DEVICE_IPHONE)
	void MacGetDeviceID(char *deviceId);
	void MacGetDeviceModel(char *deviceModel);
	void MacGetDeviceOS(char *deviceOS);
	void MacGetDeviceName(char *deviceName);
	void MacVibrate(void);
	char MacGetLanguage(char *language);
	char MacGetLocale(char *locale);
	void MacGetBattery(unsigned char * level, unsigned char * status);
#elif defined(DEVICE_ANTIX)
	#include <cms.h>
#endif

#define EDGE_INCLUDED
#include "estdc.h"
#include "ememory.h"
#include "edevice.h"

#include "extra/verinfo.h"

/////////////////////////////////////////////////////////////////////
// ClassReqBattery Symbian interface                               //
/////////////////////////////////////////////////////////////////////

#if defined(SYMBIAN_ETEL3RDPARTY)

class ClassReqBattery : public CActive
{
	private:
		CTelephony *iTelephony;
		CTelephony::TBatteryInfoV1 iBatteryInfoV1;
		CTelephony::TBatteryInfoV1Pckg iBatteryInfoV1Pckg;
		void RunL(void);
		void DoCancel(void);
	public:
		unsigned char battlevel;
		unsigned char battstatus;
		ClassReqBattery(CTelephony* aTelephony);
		~ClassReqBattery(void);
		void StartRequest(void);
};//Battery status request class

//Active object callback
void ClassReqBattery::RunL(void)
{
	if (iStatus == KErrNone)
	{
		battlevel = iBatteryInfoV1.iChargeLevel;
		switch(iBatteryInfoV1.iStatus)
		{
			case CTelephony::EPowerStatusUnknown:
				battstatus = BATT_ERROR;
				break;
			case CTelephony::EPoweredByBattery:
				battstatus = BATT_BATTERYPOWERED;
				break;
			case CTelephony::EBatteryConnectedButExternallyPowered:
				battstatus = BATT_ACPOWERED;
				break;
			case CTelephony::ENoBatteryConnected:
				battstatus = BATT_ACPOWERED;
				break;
			case CTelephony::EPowerFault:
				battstatus = BATT_NOTFOUND;
				break;
			default:
				battstatus = BATT_ERROR;
				break;
		}
	}
	CActiveScheduler::Stop();
	TRequestStatus *s = &iStatus;
	User::RequestComplete(s, KErrNone);
}

//Active object cancel procedure
void ClassReqBattery::DoCancel(void)
{
	iTelephony->CancelAsync(CTelephony::EGetBatteryInfoCancel);
	Cancel();
}

//ClassReqBattery: constructor
ClassReqBattery::ClassReqBattery(CTelephony* aTelephony) : CActive(EPriorityStandard), iTelephony(aTelephony), iBatteryInfoV1Pckg(iBatteryInfoV1)
{
	CActiveScheduler::Add(this);
}

//ClassReqBattery: destructor
ClassReqBattery::~ClassReqBattery(void)
{
	Cancel();
}

//Initiate a request
void ClassReqBattery::StartRequest(void)
{
	iTelephony->GetBatteryInfo(iStatus, iBatteryInfoV1Pckg);
	SetActive();
	CActiveScheduler::Start();
	User::WaitForRequest(iStatus);
}

#endif

/////////////////////////////////////////////////////////////////////
// ClassReqIMEI Symbian interface                                  //
/////////////////////////////////////////////////////////////////////

#if defined(SYMBIAN_ALTERNATIVE_IMEI)

class ClassReqIMEI : public CActive
{
	private:
		CTelephony *iTelephony;
		CTelephony::TPhoneIdV1 iPhoneIdV1;
		CTelephony::TPhoneIdV1Pckg iPhoneIdV1Pckg;
		void RunL(void);
		void DoCancel(void);
	public:
		TBuf<50> retrievedIMEI;
		ClassReqIMEI(CTelephony* aTelephony);
		~ClassReqIMEI(void);
		void StartRequest(void);
};//IMEI status request class

//Active object callback
void ClassReqIMEI::RunL(void)
{
	if (iStatus == KErrNone)
		retrievedIMEI = iPhoneIdV1.iSerialNumber;
	CActiveScheduler::Stop();
	TRequestStatus *s = &iStatus;
	User::RequestComplete(s, KErrNone);
}

//Active object cancel procedure
void ClassReqIMEI::DoCancel(void)
{
	iTelephony->CancelAsync(CTelephony::EGetPhoneIdCancel);
	Cancel();
}

//ClassReqIMEI: constructor
ClassReqIMEI::ClassReqIMEI(CTelephony* aTelephony) : CActive(EPriorityStandard), iTelephony(aTelephony), iPhoneIdV1Pckg(iPhoneIdV1)
{
	CActiveScheduler::Add(this);
}

//ClassReqIMEI: destructor
ClassReqIMEI::~ClassReqIMEI(void)
{
	Cancel();
}

//Initiate a request
void ClassReqIMEI::StartRequest(void)
{
	iTelephony->GetPhoneId(iStatus, iPhoneIdV1Pckg);
	SetActive();
	CActiveScheduler::Start();
	User::WaitForRequest(iStatus);
}

#endif

/////////////////////////////////////////////////////////////////////
// GP2X serial number functions                                    //
/////////////////////////////////////////////////////////////////////

#if defined(DEVICE_GP2X)

//Wait for interrupt
int ClassEDevice_WaitOnI2CInterrupt(volatile unsigned short *pusRegs)
{
	int n = 10000000;
	while(n > 0)
	{
		if (pusRegs[I2CCON] & 0x10) // wait for interrupt pending flag
			return(1);
		n--;
	}
	return(0);
}

//Returns 1 on success, 0 on failure
int ClassEDevice_FetchSerial(volatile unsigned short *pusRegs, char *pszSerial)
{
	unsigned short usCount = 0;

	// initialize registers
	pusRegs[I2CCON] = 0xEF; // set clocks, enable interrupt, clear pending flag
	pusRegs[I2CADDR] = DEVICE; // set address
	pusRegs[I2CSTAT] = 0x10; // enable I2C

	*pszSerial = 0;

	// tell the eeprom what address we're interested in
	pusRegs[I2CDATA] = DEVICE;
	pusRegs[I2CSTAT] = 0xF0; // Master mode, data write
	if (ClassEDevice_WaitOnI2CInterrupt(pusRegs))
	{
		pusRegs[I2CDATA] = 0x00; // high byte of address
		pusRegs[I2CCON] = 0xEF;
		if (ClassEDevice_WaitOnI2CInterrupt(pusRegs))
		{
			pusRegs[I2CDATA] = 0x20; // address of serial number
			pusRegs[I2CCON] = 0xEF;
			if (ClassEDevice_WaitOnI2CInterrupt(pusRegs))
			{
				// issue the read command
				pusRegs[I2CDATA] = DEVICE;
				pusRegs[I2CSTAT] = 0xB0; // Master mode, data read
				pusRegs[I2CCON] = 0xEF;
				if (ClassEDevice_WaitOnI2CInterrupt(pusRegs))
				{
					// read the data bytes
					for (usCount = 0; usCount < 24; usCount++)
					{
						pusRegs[I2CCON] = 0xEF;
						if (!ClassEDevice_WaitOnI2CInterrupt(pusRegs))
							break;
						*pszSerial++ = (char)pusRegs[I2CDATA];
						*pszSerial = 0;
					}
				}
			}
		}
	}
	// shut down
	pusRegs[I2CSTAT] = 0x90;
	pusRegs[I2CCON] = 0xEF;
	usleep(10000);
	// leave the I2C in a quiet state
	pusRegs[I2CCON] = 0x40; // disable interrupt
	pusRegs[I2CSTAT] = 0x00; // disable I2C
	if(usCount == 24)
		return(1);
	return(0);
}

#endif

/////////////////////////////////////////////////////////////////////
// ClassEDevice::Private                                           //
/////////////////////////////////////////////////////////////////////

//Get battery information
void ClassEDevice::GetBatteryInfo(unsigned char &level, unsigned char &status)
{
	#if defined(DEVICE_GP2X)
		int devbatt;
		unsigned short currentval = 0;
		devbatt = open("/dev/batt", O_RDONLY);
		read(devbatt, &currentval, 2);
		close(devbatt);
		if (currentval >= 900)
		{
			status = BATT_ACPOWERED;
			level = 255;
		}
		else
		{
			if (currentval < 600)
				level = 0;
			else
				level = (unsigned char)((currentval - 600) * 255 / 300);
			status = BATT_BATTERYPOWERED;
		}
	#elif defined(DEVICE_WIN32)
		BOOL result;
		#if defined(DEVICE_WINCE)
			SYSTEM_POWER_STATUS_EX pstatus;
			#if !defined(DEVICE_WINMOBILE)
				ClassEMemory::Set(&pstatus, 0, sizeof(pstatus));
				result = FALSE;
			#else
				result = GetSystemPowerStatusEx(&pstatus, TRUE);
			#endif
		#else
			SYSTEM_POWER_STATUS pstatus;
			result = GetSystemPowerStatus(&pstatus);
		#endif
		if (result == TRUE)
		{
			if (pstatus.ACLineStatus == 1)
			{
				status = BATT_ACPOWERED;
				if (pstatus.BatteryFlag & 128) //No system battery
					level = 255;
			}
			else
			{
				if (pstatus.ACLineStatus == 0)
					status = BATT_BATTERYPOWERED;
				else
					status = BATT_ERROR;
				if (pstatus.BatteryFlag & 128)
				{
					level = 0;
					status = BATT_NOTFOUND;
				}
			}
			if (status == BATT_ACPOWERED || status == BATT_BATTERYPOWERED)
			{
				if (!(pstatus.BatteryFlag & 128))
				{
					if (pstatus.BatteryLifePercent > 100)
					{
						level = 0;
						status = BATT_ERROR;
					}
					else
					{
						level = pstatus.BatteryLifePercent * 255 / 100;
						if (pstatus.BatteryFlag & 8) //Is charging
							status = BATT_CHARGING;
					}
				}
				else
					level = 255;
			}
		}
		else
		{
			level = 255;
			status = BATT_ERROR;
		}
	#elif defined(DEVICE_SYMBIAN)
		#if defined(SYMBIAN_ETEL3RDPARTY)
			CTelephony *iTelephony;
			iTelephony = CTelephony::NewL();
			if (iTelephony != NULL)
			{
				ClassReqBattery *cbattery = NULL;
				cbattery = new ClassReqBattery(iTelephony);
				if (cbattery != NULL)
				{
					cbattery->StartRequest();
					status = cbattery->battstatus;
					level  = cbattery->battlevel;
					delete cbattery;
				}
				delete iTelephony;
			}
		#else
			TInt aValue;
			HAL::Get(HALData::EPowerBatteryStatus, aValue);
			switch (aValue)
			{
				case HALData::EPowerBatteryStatus_Zero:
					status = BATT_BATTERYPOWERED;
					level = 0;
					break;
				case HALData::EPowerBatteryStatus_Low:
					status = BATT_BATTERYPOWERED;
					level = 96;
					break;
				case HALData::EPowerBatteryStatus_Replace:
					status = BATT_BATTERYPOWERED;
					level = 0;
					break;
				case HALData::EPowerBatteryStatus_Good:
					status = BATT_BATTERYPOWERED;
					level = 192;
					break;
				default:
					status = BATT_ERROR;
					level = 0;
					break;
			}
		#endif
	#elif defined(DEVICE_MAEMO) && !defined(DEVICE_MAEMOSIM)
		GError *error = NULL;
		DBusGConnection *connection = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);
		if (connection)
		{
			DBusGProxy *proxy;
			gchar **devicearray = NULL;
			char batterydevice[256];
			proxy = dbus_g_proxy_new_for_name(connection, "org.freedesktop.Hal", "/org/freedesktop/Hal/Manager", "org.freedesktop.Hal.Manager");
			if (dbus_g_proxy_call(proxy, "FindDeviceByCapability", &error, G_TYPE_STRING, "battery", G_TYPE_INVALID, G_TYPE_STRV, &devicearray, G_TYPE_INVALID))
			{
				if (devicearray[0])
				{
					bool ischarging;
					guint32 currentlevel, maxlevel;
					ClassEStd::StrCpy(batterydevice, devicearray[0]);
					proxy = dbus_g_proxy_new_for_name(connection, "org.freedesktop.Hal", batterydevice, "org.freedesktop.Hal.Device");
					if (dbus_g_proxy_call(proxy, "GetProperty", &error, G_TYPE_STRING, "battery.charge_level.current", G_TYPE_INVALID, G_TYPE_INT, &currentlevel, G_TYPE_INVALID) == 0)
					{
						g_clear_error(&error);
						currentlevel = 0;
					}
					if (dbus_g_proxy_call(proxy, "GetProperty", &error, G_TYPE_STRING, "battery.charge_level.last_full", G_TYPE_INVALID, G_TYPE_INT, &maxlevel, G_TYPE_INVALID) == 0)
					{
						g_clear_error(&error);
						maxlevel = 0;
					}
					if (maxlevel == 0)
					{
						if (dbus_g_proxy_call(proxy, "GetProperty", &error, G_TYPE_STRING, "battery.charge_level.design", G_TYPE_INVALID, G_TYPE_INT, &maxlevel, G_TYPE_INVALID) == 0)
						{
							maxlevel = 0;
						}
					}
					if (maxlevel == 0)
					{
						maxlevel = 8;
						if (maxlevel < currentlevel)
							maxlevel = currentlevel;
					}
					if (dbus_g_proxy_call(proxy, "GetProperty", &error, G_TYPE_STRING, "battery.rechargeable.is_charging", G_TYPE_INVALID, G_TYPE_BOOLEAN, &ischarging, G_TYPE_INVALID) == 0)
					{
						g_clear_error(&error);
						ischarging = false;
					}
					if (ischarging)
						status = BATT_CHARGING;
					else
						status = BATT_BATTERYPOWERED;

					level = currentlevel * 255 / maxlevel;
				}
				else
				{
					status = BATT_NOTFOUND;
					level = 0;
				}
				g_free(devicearray);
			}
			else
			{
				g_clear_error(&error);
				status = BATT_NOTFOUND;
				level = 0;
			}
		}
		else
		{
			g_clear_error(&error);
			status = BATT_ERROR;
			level = 0;
		}
	#elif defined(DEVICE_ANDROID)
		level = (unsigned char) Call_EdgelibUtil_getBatteryLevel();
		status = (unsigned char) Call_EdgelibUtil_getBatteryState();
	#elif defined(DEVICE_IPHONE)
		MacGetBattery(&level, &status);
	#else
		level = 255;
		status = BATT_UNSUPPORTED;
	#endif
}

/////////////////////////////////////////////////////////////////////
// ClassEDevice::Public                                            //
/////////////////////////////////////////////////////////////////////

//ClassEDevice: constructor
ClassEDevice::ClassEDevice(void)
{
	iscachedid = false;
	mVibrateTimer = 0;
	cachedid[0] = 0;
}

//ClassEDevice: destructor
ClassEDevice::~ClassEDevice(void)
{
}

//Retrieve the current time
void ClassEDevice::CurrentTime(unsigned char &hour, unsigned char &minute, unsigned char &second, bool islocal)
{
	#if defined(DEVICE_WIN32)
		SYSTEMTIME newtime;
		if (islocal)
			GetLocalTime(&newtime);
		else
			GetSystemTime(&newtime);
		hour   = (unsigned char)newtime.wHour;
		minute = (unsigned char)newtime.wMinute;
		second = (unsigned char)newtime.wSecond;
	#elif defined(DEVICE_SYMBIAN)
		TTime ttime;
		TDateTime datetime;
		if (islocal)
			ttime.HomeTime();
		else
			ttime.UniversalTime();
		datetime = ttime.DateTime();
		hour = datetime.Hour();
		minute = datetime.Minute();
		second = datetime.Second();
	#elif defined(DEVICE_POSIX)
		tm *newtime;
		time_t ltime;
		time(&ltime);
		if (islocal)
			newtime = localtime(&ltime);
		else
			newtime = gmtime(&ltime);
		hour   = newtime->tm_hour;
		minute = newtime->tm_min;
		second = newtime->tm_sec;
	#elif defined(DEVICE_OPENKODE)
		KDtime ltime;
		KDTm newtime;
		kdTime(&ltime);
		if (islocal)
			kdLocaltime_r(&ltime, &newtime);
		else
			kdGmtime_r(&ltime, &newtime);
		hour   = newtime.tm_hour;
		minute = newtime.tm_min;
		second = newtime.tm_sec;
	#endif
}

//Retrieve the current date
void ClassEDevice::CurrentDate(unsigned long &year, unsigned char &month, unsigned char &day, bool islocal)
{
	#if defined(DEVICE_WIN32)
		SYSTEMTIME newtime;
		if (islocal)
			GetLocalTime(&newtime);
		else
			GetSystemTime(&newtime);
		year  = newtime.wYear;
		month = (unsigned char)newtime.wMonth;
		day   = (unsigned char)newtime.wDay;
	#elif defined(DEVICE_SYMBIAN)
		TTime ttime;
		TDateTime datetime;
		if (islocal)
			ttime.HomeTime();
		else
			ttime.UniversalTime();
		datetime = ttime.DateTime();
		year = datetime.Year();
		month = datetime.Month() + 1;
		day = datetime.Day() + 1;
	#elif defined(DEVICE_POSIX)
		tm *newtime;
		time_t ltime;
		time(&ltime);
		if (islocal)
			newtime = localtime(&ltime);
		else
			newtime = gmtime(&ltime);
		year  = newtime->tm_year + 1900;
		month = newtime->tm_mon + 1;
		day   = newtime->tm_mday;
	#elif defined(DEVICE_OPENKODE)
		KDtime ltime;
		KDTm newtime;
		kdTime(&ltime);
		if (islocal)
			kdLocaltime_r(&ltime, &newtime);
		else
			kdGmtime_r(&ltime, &newtime);
		year  = newtime.tm_year + 1900;
		month = newtime.tm_mon + 1;
		day   = newtime.tm_mday;
	#endif
}

//Retrieve the current timestamp
ETYPE_TIMESTAMP ClassEDevice::TimestampNow(bool islocal)
{
	E_DATETIME dt;
	ClassEDevice::CurrentDate(dt.year, dt.month, dt.day, islocal);
	ClassEDevice::CurrentTime(dt.hour, dt.minute, dt.second, islocal);
	return(ClassEStd::DateTimeToTimestamp(&dt));
}

//Retrieve the local time offset relative to GMT
long ClassEDevice::LocalTimeOffset(void)
{
	ETYPE_TIMESTAMP offset = 0;
	#if defined(DEVICE_WIN32)
		SYSTEMTIME timelocal, timenonlocal;
		GetLocalTime(&timelocal);
		GetSystemTime(&timenonlocal);
		if (timelocal.wDay != timenonlocal.wDay)
			offset = 24 * 3600 + timelocal.wHour * 3600 + timelocal.wMinute * 60 + timelocal.wSecond - timenonlocal.wHour * 3600 - timenonlocal.wMinute * 60 - timenonlocal.wSecond;
		else
			offset = timelocal.wHour * 3600 + timelocal.wMinute * 60 + timelocal.wSecond - timenonlocal.wHour * 3600 - timenonlocal.wMinute * 60 - timenonlocal.wSecond;
	#elif defined(DEVICE_SYMBIAN)
		TLocale locale;
		offset = locale.UniversalTimeOffset().Int();
		if (locale.QueryHomeHasDaylightSavingOn())
			offset += 3600;
	#elif defined(DEVICE_ANDROID)
		offset = Call_EdgelibUtil_getTimezoneOffset();
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		time_t ltime = 0;
		tm *newtime = localtime(&ltime);
		offset = newtime->tm_hour * 3600 + newtime->tm_min * 60 + newtime->tm_sec;
		if (daylight)
			offset += 3600;
	#elif defined(DEVICE_POSIX)
		TIME_ZONE_INFORMATION timeinfo;
		if (GetTimeZoneInformation(&timeinfo) != TIME_ZONE_ID_UNKNOWN)
			offset = (timeinfo.Bias * 60 + timeinfo.DaylightBias * 60) * -1;
	#elif defined(DEVICE_OPENKODE)
		KDtime ltime;
		KDTm timelocal, timenonlocal;
		kdTime(&ltime);
		kdLocaltime_r(&ltime, &timelocal);
		kdGmtime_r(&ltime, &timenonlocal);
		if (timelocal.tm_yday != timenonlocal.tm_yday)
			offset = 24 * 3600 + timelocal.tm_hour * 3600 + timelocal.tm_min * 60 + timelocal.tm_sec - timenonlocal.tm_hour * 3600 - timenonlocal.tm_min * 60 - timenonlocal.tm_sec;
		else
			offset = timelocal.tm_hour * 3600 + timelocal.tm_min * 60 + timelocal.tm_sec - timenonlocal.tm_hour * 3600 - timenonlocal.tm_min * 60 - timenonlocal.tm_sec;
	#endif
	return(offset);
}

//Retrieve the factory ID from the device
unsigned long ClassEDevice::GetModelID(void)
{
	#if defined(DEVICE_SYMBIAN)
		TInt aInt;
		HAL::Get(HALData::EMachineUid, aInt);
		return(aInt);
	#endif
	return(0);
}

//Retrieve the model name in a string
void ClassEDevice::GetModelName(char *modelname)
{
	WCHAR ustr_modelname[MAX_ESTR];
	GetModelName(ustr_modelname);
	ClassEStd::StrCpy(modelname, ustr_modelname);
}

//Retrieve the model name in a string
void ClassEDevice::GetModelName(WCHAR *modelname)
{
	unsigned long modelid = GetModelID();
	ClassEStd::StrCpy(modelname, "Device model unknown");
	#if defined(DEVICE_SYMBIAN)
		#if defined(DEVICE_S60)
			#if (DEVICE_SYMBIAN >= 910)
				ClassEStd::StrCpy(modelname, "S60 device");
			#else
				ClassEStd::StrCpy(modelname, "Series 60 device");
			#endif
		#elif defined(DEVICE_S80)
			ClassEStd::StrCpy(modelname, "Nokia Communicator");
		#elif defined(DEVICE_S90)
			ClassEStd::StrCpy(modelname, "Series 90 device");
		#elif defined(DEVICE_UIQ)
			ClassEStd::StrCpy(modelname, "UIQ device");
		#endif
		switch(modelid)
		{
			#if defined(DEVICE_S60)
				case 0x10005e33: ClassEStd::StrCpy(modelname, "Nokia 92xx"); break;
				case 0x10005f62: ClassEStd::StrCpy(modelname, "Symbian emulator"); break;
				case 0x101f3ee3: ClassEStd::StrCpy(modelname, "Nokia 6620"); break;
				case 0x101f408b: ClassEStd::StrCpy(modelname, "Sony-Ericsson P800"); break;
				case 0x101f466a: ClassEStd::StrCpy(modelname, "Nokia 3650/3660"); break;
				case 0x101f4fc3: ClassEStd::StrCpy(modelname, "Nokia 7650"); break;
				case 0x101f8c19: ClassEStd::StrCpy(modelname, "Nokia N-Gage"); break;
				case 0x101f8ddb: ClassEStd::StrCpy(modelname, "Nokia 9500"); break;
				case 0x101f9071: ClassEStd::StrCpy(modelname, "Siemens SX1"); break;
				case 0x101fa031: ClassEStd::StrCpy(modelname, "Sendo X"); break;
				case 0x101fb2b1: ClassEStd::StrCpy(modelname, "Nokia N-Gage QD"); break;
				case 0x101fb3dd: ClassEStd::StrCpy(modelname, "Nokia 6600"); break;
				case 0x101fb3f3: ClassEStd::StrCpy(modelname, "Nokia 7610"); break;
				case 0x101fb3f4: ClassEStd::StrCpy(modelname, "Nokia 6260"); break;
				case 0x101fbb55: ClassEStd::StrCpy(modelname, "Nokia 6630"); break;
				case 0x101fbe09: ClassEStd::StrCpy(modelname, "Nokia 7710"); break;
				case 0x10200f97: ClassEStd::StrCpy(modelname, "Nokia 3230"); break;
				case 0x10200f98: ClassEStd::StrCpy(modelname, "Nokia N90"); break;
				case 0x10200f99: ClassEStd::StrCpy(modelname, "Nokia 6680"); break;
				case 0x10200f9a: ClassEStd::StrCpy(modelname, "Nokia N70/N72"); break;				// Added v3.60: N70/N72 See http://www.glbenchmark.com/phonedetails.jsp?benchmark=pro&D=Nokia%20N72&testgroup=system
				case 0x10200f9b: ClassEStd::StrCpy(modelname, "Nokia 6682"); break;
				case 0x10200f9c: ClassEStd::StrCpy(modelname, "Nokia 6681"); break;
				case 0x200005f8: ClassEStd::StrCpy(modelname, "Nokia 3250"); break;
				case 0x200005f9: ClassEStd::StrCpy(modelname, "Nokia N80"); break;
				case 0x200005fa: ClassEStd::StrCpy(modelname, "Nokia N92"); break;
				case 0x200005fb: ClassEStd::StrCpy(modelname, "Nokia N73"); break;
				case 0x200005fc: ClassEStd::StrCpy(modelname, "Nokia N91"); break;
				case 0x200005fe: ClassEStd::StrCpy(modelname, "Nokia N75"); break;					// Added v3.60
				case 0x200005ff: ClassEStd::StrCpy(modelname, "Nokia N71"); break;
				case 0x20000600: ClassEStd::StrCpy(modelname, "Nokia N93"); break;
				case 0x20000602: ClassEStd::StrCpy(modelname, "Nokia 5500"); break;					// Added v3.60
				case 0x20000604: ClassEStd::StrCpy(modelname, "Nokia E65"); break;					// Added v3.60
				case 0x20000605: ClassEStd::StrCpy(modelname, "Nokia N93i"); break;					// Added v3.60
				case 0x20000606: ClassEStd::StrCpy(modelname, "Nokia 6290"); break;					// Added v3.60
				case 0x2000060b: ClassEStd::StrCpy(modelname, "Nokia N95"); break;
				case 0x20001856: ClassEStd::StrCpy(modelname, "Nokia E60"); break;
				case 0x20001857: ClassEStd::StrCpy(modelname, "Nokia E70"); break;
				case 0x20001858: ClassEStd::StrCpy(modelname, "Nokia E61"); break;
				case 0x20002495: ClassEStd::StrCpy(modelname, "Nokia E50"); break;					// Added v3.60
				case 0x20002496: ClassEStd::StrCpy(modelname, "Nokia E90"); break;
				case 0x20002498: ClassEStd::StrCpy(modelname, "Nokia E51"); break;					// Added v3.60
				case 0x20002d7b: ClassEStd::StrCpy(modelname, "Nokia 6110 Navigator"); break;		// Added v3.60
				case 0x20002d7c: ClassEStd::StrCpy(modelname, "Nokia 5700"); break;					// Added v3.60
				case 0x20002d7e: ClassEStd::StrCpy(modelname, "Nokia 6120 Classic"); break;			// Added v3.60
				case 0x20002d7f: ClassEStd::StrCpy(modelname, "Nokia E61i"); break;					// Added v3.60
				case 0x20002d83: ClassEStd::StrCpy(modelname, "Nokia N81"); break;					// Added v3.60
				case 0x20002d84: ClassEStd::StrCpy(modelname, "Nokia N95 8GB"); break;				// Added v3.60
				case 0x20002d85: ClassEStd::StrCpy(modelname, "Nokia N82"); break;					// Added v3.60
			#elif defined(DEVICE_UIQ)
				case 0x101f6b26: ClassEStd::StrCpy(modelname, "Motorola A9xx"); break;
				case 0x101fb2ae: ClassEStd::StrCpy(modelname, "Sony-Ericsson P900"); break;
				case 0x10200ac6: ClassEStd::StrCpy(modelname, "Sony-Ericsson P910"); break;
				case 0x1020e285: ClassEStd::StrCpy(modelname, "Sony-Ericsson P990i"); break;		// Added v3.60
				case 0x1027400d: ClassEStd::StrCpy(modelname, "Motorola RIZR Z8"); break;			// Added v3.60
				case 0x10274bf9: ClassEStd::StrCpy(modelname, "Sony-Ericsson M600i"); break;
				case 0x10274bfa: ClassEStd::StrCpy(modelname, "Sony-Ericsson W950i"); break;		// Added v3.60
				case 0x20002e69: ClassEStd::StrCpy(modelname, "Sony-Ericsson P1i"); break;			// Added v3.60
				case 0x20002e6a: ClassEStd::StrCpy(modelname, "Sony-Ericsson W960i"); break;		// Added v3.60
			#endif
		}
	#elif defined(DEVICE_WINCE)
		#if defined(DEVICE_POCKETPC)
			ClassEStd::StrCpy(modelname, "Pocket PC");
		#elif defined(DEVICE_SMARTPHONE)
			ClassEStd::StrCpy(modelname, "Smartphone");
		#elif defined(DEVICE_GIZMONDO)
			ClassEStd::StrCpy(modelname, "Gizmondo");
		#endif
		WCHAR aname[512];
		SystemParametersInfo(SPI_GETOEMINFO, sizeof(aname), &aname, 0);
		if (aname[0] == '<')
		{
			ClassEStd::StrCpy(modelname, &aname[1]);
			if (modelname[ClassEStd::StrLen(modelname) - 1] == '>')
				modelname[ClassEStd::StrLen(modelname) - 1] = 0;
			ClassEStd::StrTrim(modelname);
		}
		else
			ClassEStd::StrCpy(modelname, aname);
	#elif defined(DEVICE_GP2X)
		ClassEStd::StrCpy(modelname, "GP2X F-100");
	#elif defined(DEVICE_LINUX)
		#if defined(DEVICE_MAEMO)
			char *strptr = getenv("OSSO_PRODUCT_FULL_NAME");
			if (strptr)
				ClassEStd::StrCpy(modelname, strptr);
			else
				ClassEStd::StrCpy(modelname, "Maemo device");
		#elif defined(DEVICE_ANDROID)
			ClassEStd::StrCpy(modelname, AndroidGetDeviceModel());
		#else
			ClassEStd::StrCpy(modelname, "Linux device");
		#endif
	#elif defined(DEVICE_IPHONE)
		char name[MAX_ESTR];
		MacGetDeviceModel(name);
		ClassEStd::StrCpy(modelname, name);
	#elif defined(DEVICE_MAC)
		ClassEStd::StrCpy(modelname, "Macintosh");
	#else
		ClassEStd::StrCpy(modelname, "Desktop PC");
	#endif
}

//Retrieve the operating system name in a string
void ClassEDevice::GetOSName(char *osname)
{
	WCHAR ustr_osname[MAX_ESTR];
	GetOSName(ustr_osname);
	ClassEStd::StrCpy(osname, ustr_osname);
}

//Retrieve the operating system name in a string
void ClassEDevice::GetOSName(WCHAR *osname)
{
	#if defined(DEVICE_SYMBIAN)
		#if defined(DEVICE_S60)
			#if (DEVICE_SYMBIAN >= 910)
				ClassEStd::StrCpy(osname, "Symbian S60");
			#else
				ClassEStd::StrCpy(osname, "Symbian Series 60");
			#endif
		#elif defined(DEVICE_S80)
			ClassEStd::StrCpy(osname, "Symbian Series 80");
		#elif defined(DEVICE_S90)
			ClassEStd::StrCpy(osname, "Symbian Series 90");
		#elif defined(DEVICE_UIQ)
			ClassEStd::StrCpy(osname, "Symbian UIQ");
		#else
			ClassEStd::StrCpy(osname, "Symbian");
		#endif
	#elif defined(DEVICE_WINCE)
		#if defined(DEVICE_POCKETPC)
			ClassEStd::StrCpy(osname, "Windows Mobile Pocket PC");
		#elif defined(DEVICE_SMARTPHONE)
			ClassEStd::StrCpy(osname, "Windows Mobile Smartphone");
		#elif defined(DEVICE_WINMOBILE)
			ClassEStd::StrCpy(osname, "Windows Mobile");
		#else
			ClassEStd::StrCpy(osname, "Windows CE");
		#endif
	#elif defined(DEVICE_WINDOWS)
		OSVERSIONINFO verinfo;
		verinfo.dwOSVersionInfoSize = sizeof(verinfo);
		ClassEStd::StrCpy(osname, "Windows");
		if (GetVersionEx(&verinfo) == TRUE)
		{
			if (verinfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
			{
				if (verinfo.dwMajorVersion == 5)
				{
					if (verinfo.dwMinorVersion == 0)
						ClassEStd::StrCpy(osname, "Windows 2000");
					else if (verinfo.dwMinorVersion == 1)
						ClassEStd::StrCpy(osname, "Windows XP");
					else if (verinfo.dwMinorVersion == 2)
						ClassEStd::StrCpy(osname, "Windows 2003");
					else
						ClassEStd::StrCpy(osname, "Windows NT");
				}
				else
					ClassEStd::StrCpy(osname, "Windows NT");
			}
			else if (verinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
			{
				if (verinfo.dwMajorVersion == 4)
				{
					if (verinfo.dwMinorVersion == 0)
						ClassEStd::StrCpy(osname, "Windows 95");
					else if (verinfo.dwMinorVersion >= 1 && verinfo.dwMinorVersion <= 8)
						ClassEStd::StrCpy(osname, "Windows 98");
					else if (verinfo.dwMinorVersion == 9)
						ClassEStd::StrCpy(osname, "Windows ME");
				}
			}
		}
	#elif defined(DEVICE_LINUX)
		#if defined(DEVICE_MAEMO)
			char *strptr = getenv("OSSO_PRODUCT_RELEASE_NAME");
			if (strptr)
				ClassEStd::StrCpy(osname, strptr);
			else
				ClassEStd::StrCpy(osname, "Linux");
		#elif defined(DEVICE_ANDROID)
			ClassEStd::StrCpy(osname, "Linux/Android");
			switch(AndroidGetDeviceOS())
			{
				case 1:	ClassEStd::StrCat(osname, " 1.0"); break;
				case 2: ClassEStd::StrCat(osname, " 1.1"); break;
				case 3: ClassEStd::StrCat(osname, " 1.5"); break;
				case 4: ClassEStd::StrCat(osname, " 1.6"); break;
				case 5: ClassEStd::StrCat(osname, " 2.0"); break;
				case 6: ClassEStd::StrCat(osname, " 2.0.1"); break;
				case 7: ClassEStd::StrCat(osname, " 2.1"); break;
				case 8: ClassEStd::StrCat(osname, " 2.2"); break;
				case 9: ClassEStd::StrCat(osname, " 2.3"); break;
				case 10: ClassEStd::StrCat(osname, " 2.3.3"); break;
				case 11: ClassEStd::StrCat(osname, " 3.0"); break;

				default:
					break;
			}
		#else
			ClassEStd::StrCpy(osname, "Linux");
		#endif
	#elif defined(DEVICE_IPHONE)
		char name[MAX_ESTR];
		MacGetDeviceOS(name);
		ClassEStd::StrCpy(osname, name);
	#elif defined(DEVICE_ANTIX)
		static const char INTERFACE_NAME[] = "http://components.antixlabs.com/if/agp";
		CMSEnumeration *components = cmsStartEnumeration(INTERFACE_NAME, KD_NULL);
		const KDchar *component, *version = KD_NULL;
		ClassEStd::StrCpy(osname, "AGP");
		if (components)
		{
			cmsGetNext(components, &component, &version);
			if (version)
			{
				KDchar *delimiter;
				long major, minor;
				major = kdStrtol(version, &delimiter, 10);
				minor = kdStrtol(delimiter + 1, KD_NULL, 10);
				ClassEStd::StrCpy(osname, "AGP version: ");
				ClassEStd::StrCat(osname, major);
				ClassEStd::StrCat(osname, ".");
				ClassEStd::StrCat(osname, minor);
			}
			cmsEndEnumeration(components);
		}
	#else
		ClassEStd::StrCpy(osname, "Unknown");
	#endif
}

//Return the amount of battery charge left
unsigned char ClassEDevice::GetBatteryLevel(void)
{
	unsigned char level, info;
	GetBatteryInfo(level, info);
	return(level);
}

//Get the battery status
unsigned char ClassEDevice::GetBatteryStatus(void)
{
	unsigned char level, info;
	GetBatteryInfo(level, info);
	return(info);
}

//Retrieve the owner name
char *ClassEDevice::GetOwnerName(char *owner)
{
	WCHAR ustr_owner[MAX_ESTR];
	GetOwnerName(ustr_owner);
	ClassEStd::StrCpy(owner, ustr_owner);
	return(owner);
}

//Retrieve the owner name
WCHAR *ClassEDevice::GetOwnerName(WCHAR *owner)
{
	ClassEStd::StrCpy(owner, "");
	#if defined(DEVICE_WIN32)
		#if defined(DEVICE_DESKTOP)
			#if defined(_CRT_INSECURE_DEPRECATE)
				size_t dummy;
				_wgetenv_s(&dummy, (LPWSTR)owner, 64, L"username");
			#else
				ClassEStd::StrCpy(owner, (WCHAR *)_wgetenv(L"username"));
			#endif
		#else
			HKEY keyowner;
			WCHAR ownerw[0x200];
			unsigned long result, rsize, rtype, ka = KEY_ALL_ACCESS;
			memset(ownerw, 0, 0x300);
			rsize = 0x300;
			rtype = REG_BINARY;
			RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("\\ControlPanel\\Owner"), 0, NULL, 0, ka, NULL, &keyowner, &result);
			if (RegQueryValueEx(keyowner, TEXT("Owner"), NULL, &rtype, (PBYTE)ownerw, &rsize) == ERROR_SUCCESS)
				ClassEStd::StrCpy(owner, ownerw);
			if (RegQueryValueEx(keyowner, TEXT("Name"), NULL, &rtype, (PBYTE)ownerw, &rsize) == ERROR_SUCCESS)
				ClassEStd::StrCpy(owner, ownerw);
			RegCloseKey(keyowner);
		#endif
	#elif defined(DEVICE_IPHONE)
		char name[MAX_ESTR];
		MacGetDeviceName(name);
		ClassEStd::StrCpy(owner, name);
	#endif
	return(owner);
}

//Retrieve the owner name
char *ClassEDevice::GetIMEI(char *imeicode, bool includedashes)
{
	WCHAR ustr_imeicode[MAX_ESTR];
	GetIMEI(ustr_imeicode, includedashes);
	ClassEStd::StrCpy(imeicode, ustr_imeicode);
	return(imeicode);
}

//Retrieve the IMEI code
WCHAR *ClassEDevice::GetIMEI(WCHAR *imeicode, bool includedashes)
{
	ClassEStd::StrCpy(imeicode, "");
	#if defined(DEVICE_SYMBIAN)
		char imeitmpstr[MAX_ESTR];
		#if defined(SYMBIAN_ALTERNATIVE_IMEI)
			CTelephony *iTelephony;
			iTelephony = CTelephony::NewL();
			if (iTelephony != NULL)
			{
				ClassReqIMEI *cimei = NULL;
				cimei = new ClassReqIMEI(iTelephony);
				if (cimei != NULL)
				{
					cimei->StartRequest();
					TBuf<50> imeistr(cimei->retrievedIMEI);
					ClassEStd::StrCpy(imeitmpstr, imeistr.PtrZ());
					delete cimei;
				}
				delete iTelephony;
			}
		#else
			TPlpVariantMachineId machineid;
			PlpVariant::GetMachineIdL(machineid);
			ClassEStd::StrCpy(imeitmpstr, machineid.PtrZ());
		#endif
		if (includedashes)
		{
			unsigned short onchar = 0, ctr;
			for (ctr = 0; ctr < ClassEStd::StrLen(imeitmpstr); ctr++)
			{
				imeicode[onchar] = imeitmpstr[ctr];
				if ((onchar == 6 || onchar == 9 || onchar == 16) && imeicode[onchar] != '-')
				{
					imeicode[onchar + 1] = imeicode[onchar];
					imeicode[onchar] = '-';
					onchar++;
				}
				onchar++;
				imeicode[onchar] = 0;
				if (imeitmpstr[ctr] == 0)
				{
					imeicode[onchar] = 0;
					break;
				}
				if (ctr == 31)
					imeicode[onchar] = 0;
			}
		}
		else
			ClassEStd::StrCpy(imeicode, imeitmpstr);
	#elif defined(DEVICE_MAEMO) && !defined(DEVICE_MAEMOSIM)
		GError *error = NULL;
		DBusGConnection *connection = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);
		if (connection)
		{
			DBusGProxy *proxy;
			gchar *dbus_imei = NULL;
			guint32 tmp1;
			g_return_val_if_fail(connection, imeicode);
			proxy = dbus_g_proxy_new_for_name(connection, "com.nokia.phone.SIM", "/com/nokia/phone/SIM/security", "Phone.Sim.Security");//Name, path, interface
			if (dbus_g_proxy_call(proxy, "get_imei", &error, G_TYPE_INVALID, G_TYPE_STRING, &dbus_imei, G_TYPE_INT, &tmp1, G_TYPE_INVALID))
			{
				ClassEStd::StrCpy(imeicode, dbus_imei);
				g_free(dbus_imei);
			}
			else
				g_clear_error(&error);
			g_object_unref(proxy);
		}
		else
			g_clear_error(&error);
	#elif defined(DEVICE_ANDROID)
		char temp[80];
		Call_EdgelibUtil_getIMEI(temp);
		ClassEStd::StrCpy(imeicode, temp);
	#endif
	return(imeicode);
}

//Retrieve the device unique serial number
char *ClassEDevice::GetSerialNumber(char *serialid)
{
	ClassEStd::StrCpy(serialid, "");
	#if defined(DEVICE_GP2X)
		if (!iscachedid)
		{
			char tmp_id[32];
			int devicereg_ptr;
			volatile unsigned short *devicereg;
			devicereg_ptr = open("/dev/mem", O_RDWR);
			devicereg = (unsigned short *)mmap(0, 0x10000, PROT_READ | PROT_WRITE, MAP_SHARED, devicereg_ptr, 0xc0000000);
			if (ClassEDevice_FetchSerial(devicereg, tmp_id))
				ClassEStd::StrCpy(serialid, &tmp_id[16]);
			munmap((void *)devicereg, 0x10000);
			close(devicereg_ptr);
			ClassEStd::StrCpy(cachedid, serialid);
			iscachedid = true;
		}
		else
			ClassEStd::StrCpy(serialid, cachedid);
	#elif defined(DEVICE_IPHONE)
		MacGetDeviceID(serialid);
	#elif defined(DEVICE_ANDROID)
		Call_EdgelibUtil_getSerial(serialid);
	#endif
	return(serialid);
}

//Retrieve the device unique serial number
WCHAR *ClassEDevice::GetSerialNumber(WCHAR *serialid)
{
	char cstr_serialid[MAX_ESTR];
	GetSerialNumber(cstr_serialid);
	ClassEStd::StrCpy(serialid, cstr_serialid);
	return(serialid);
}

//Vibrate the device
void ClassEDevice::Vibrate(void)
{
	#if defined(DEVICE_IPHONE)
		MacVibrate();
	#elif defined(DEVICE_MAEMO) && !defined(DEVICE_MAEMOSIM)
		GError *error = NULL;
		DBusGConnection *connection = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);
		if (connection)
		{
			DBusGProxy *proxy;
			proxy = dbus_g_proxy_new_for_name(connection, "com.nokia.mce", "/com/nokia/mce/request", "com.nokia.mce.request");//Name, path, interface
			if (dbus_g_proxy_call(proxy, "req_vibrator_pattern_activate", &error, G_TYPE_STRING, "PatternChatAndEmail", G_TYPE_INVALID, G_TYPE_INVALID) == 0)
				g_clear_error(&error);
			g_object_unref(proxy);
		}
		else
			g_clear_error(&error);
	#elif defined(DEVICE_ANDROID)
		Call_EdgelibUtil_vibrate();
	#elif defined(DEVICE_OPENKODE)
		int vol = 1000;
		kdOutputSeti(KD_OUTPUT_VIBRATE_VOLUME, 1, &vol);
		mVibrateTimer = 500;
	#endif
}

//Retrieve the OS language
char *ClassEDevice::GetLanguage(char *language)
{
	char cstr_language[MAX_ESTR];
	ClassEStd::StrCpy(cstr_language, "");
	#if defined(DEVICE_IPHONE)
		MacGetLanguage(cstr_language);
	#elif defined(DEVICE_OPENKODE)
		ClassEStd::StrCpy(cstr_language, kdGetLocale());
		cstr_language[2] = 0;
	#elif defined(DEVICE_ANDROID)
		Call_EdgelibUtil_getLocale(cstr_language);
	#elif defined(DEVICE_LINUX)
		ClassEStd::StrCpy(cstr_language, getenv("LANG"));
		cstr_language[2] = 0;
	#endif
	ClassEStd::StrCpy(language, cstr_language);
	return(language);
}

//Retrieve the OS locale
char *ClassEDevice::GetLocale(char *locale)
{
	char cstr_locale[MAX_ESTR];
	ClassEStd::StrCpy(cstr_locale, "");
	#if defined(DEVICE_IPHONE)
		MacGetLocale(cstr_locale);
	#elif defined(DEVICE_OPENKODE)
		ClassEStd::StrCpy(locale, kdGetLocale());
		if (locale[2] == '_')
		{
			ClassEStd::StrCpy(cstr_locale, &locale[3]);
			ClassEStd::StrLower(cstr_locale);
		}
	#elif defined(DEVICE_ANDROID)
		Call_EdgelibUtil_getLocale(cstr_locale);
	#elif defined(DEVICE_LINUX)
		ClassEStd::StrCpy(locale, getenv("LANG"));
		if (locale[2] == '_')
		{
			ClassEStd::StrCpy(cstr_locale, &locale[3]);
			ClassEStd::StrLower(cstr_locale);
		}
	#endif
	ClassEStd::StrCpy(locale, cstr_locale);
	return(locale);
}

//Update internal state
void ClassEDevice::Update(unsigned long timedelta)
{
	if (mVibrateTimer)
	{
		if (mVibrateTimer <= timedelta)
		{
			mVibrateTimer = 0;
			#if defined(DEVICE_OPENKODE)
				int vol = 0;
				kdOutputSeti(KD_OUTPUT_VIBRATE_VOLUME, 1, &vol);
			#endif
		}
		else
			mVibrateTimer -= timedelta;
	}
	#if defined(DEVICE_LINUX)
		ecCamPoll();
	#endif
}
