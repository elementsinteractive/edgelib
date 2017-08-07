// edgecore.h
// Platform independent core functionality
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////

#if !defined(ECOREOBJ)
	#define ECOREOBJ void *
#endif

// pointer sizes
#if !defined(EINTPTR)
	#if defined(__amd64__) || defined(__x86_64__)
		#define EINTPTR long
		#define EINT64 long
		#define EINT32 int
		#define EINT16 short
	#else
		#define EINTPTR int
		#define EINT64 long long
		#define EINT32 int
		#define EINT16 short
	#endif
#endif

//Event types
#define ECEV_NATIVE                0
#define ECEV_IDLE                  1
#define ECEV_REQUESTUID            2
#define ECEV_SKIPIDLE              3
#define ECEV_PRENATIVE             4
#define ECEV_CREATEINSTANCE       16
#define ECEV_DESTROYINSTANCE      17
#define ECEV_CREATEWINDOW         18
#define ECEV_DESTROYWINDOW        19
#define ECEV_ENDMAINLOOP          20
#define ECEV_CHANGEORIENTATION    21
#define ECEV_CHANGEFOCUS          22
#define ECEV_REDRAW               23
#define ECEV_PHONECALL            24
#define ECEV_SETCOMMANDLINE       25
#define ECEV_CONTEXTLOST          26
#define ECEV_KEYDOWN             256
#define ECEV_KEYUP               257
#define ECEV_TYPECHAR            258
#define ECEV_CLICKUP             259
#define ECEV_CLICKDOWN           260
#define ECEV_CLICKDOUBLE         261
#define ECEV_CLICKMOVE           262
#define ECEV_WHEELSCROLL         263
#define ECEV_CHANGETILT          264
#define ECEV_NET_HOSTLOOKUP      512
#define ECEV_NET_HOSTCONNECT     513
#define ECEV_NET_ADDCLIENT       514
#define ECEV_NET_READABLE        515
#define ECEV_NET_FOUNDBTDEVICE   516
#define ECEV_NET_FOUNDBTSERVICE  517
#define ECEV_NET_BTDEVICENAME    518


///////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////

typedef char ecChar;
typedef ecChar* ecString;
typedef unsigned short ecUnichar;
typedef ecUnichar* ecUnistring;

typedef long ecErr;
typedef long ecParam;
typedef long ecNativetype;

typedef void (*FUNC_NETHANDLER)(unsigned short eventtype, void *eventdata, void *userdata);

///////////////////////////////////////////////////////////////////
// Structures
///////////////////////////////////////////////////////////////////

//A datetime structure
typedef struct
{
	unsigned long year;
	unsigned char dt[5];
}EC_DATETIME;

//A file info structure
typedef struct
{
	unsigned long mode;
	unsigned long size;
	EC_DATETIME timemodified;
	EC_DATETIME timecreated;
}EC_FILEINFO;

//Window create properties
typedef struct
{
	ecNativetype icon;
	ecNativetype menu;
	ecNativetype dialogbox;
	ecUnichar title[64];
	unsigned long width;
	unsigned long height;
	unsigned char fullscreen;
	unsigned char forceorientation;
	bool allowdoubleinst;
	bool statusbar;
	bool lockkeys;
	bool keysounds;
	bool showclickcursor;
	bool corethread;
	bool winborder;
	bool outwndclickcursor;
	bool hasopengl;
	bool focussuspend;
	unsigned char emulatelowres;
}EC_WINDOWSTYLE;

//Display configuration settings
typedef struct
{
	long xres;
	long yres;
	long xpitch;
	long ypitch;
	unsigned long colorchnl;
	unsigned char bitdepth;
	unsigned char prefermethod;
	bool fullscreen;
	bool hasmenu;
}EC_DISPLAYCONFIG;

//An event structure
typedef struct
{
	unsigned short type;
	ecParam param1;
	ecParam param2;
	ecParam param3;
}ecEvent;

///////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////

//Helper API
long ecExMapError(long idx, const long *errarray, long numarray);
void ecExScanFilename(ecUnistring fname, const ecUnistring pfname);
ECOREOBJ ecExCreateLogFile(void);
void ecExWriteLog(ECOREOBJ f, const char *txt, bool newline = true);
void ecExWriteLog(ECOREOBJ f, int v, bool newline = true);
long ecStrLen(const char *str, unsigned long max_str = 16384);
long ecStrLen(const ecUnistring str, unsigned long max_str = 16384);
void ecStrCpy(char *dst, const char *src, long maxlen = 16384);
void ecStrCpy(ecUnistring dst, const char *src, long maxlen = 16384);
void ecStrCpy(ecUnistring dst, const ecUnistring src, long maxlen = 16384);
void ecStrCpy(char *dst, const ecUnistring src, long maxlen = 16384);
void ecStrCat(char *dst, const char *src);
void ecStrCat(char *dst, const ecUnistring src);
void ecStrCat(ecUnistring dst, const char *src);
void ecStrCat(ecUnistring dst, const ecUnistring src);
void ecStrToUtf8(char *dst, const ecUnistring src, long maxlen = 16384);
void ecUtf8ToStr(ecUnistring dst, const char *src, long maxlen = 16384);
void ecIntToStr(char *dst, int i, unsigned char base = 10, bool signedvalue = true);
void ecIntToStr(ecUnistring dst, int i, unsigned char base = 10, bool signedvalue = true);
int ecStrToInt(char *str, unsigned char base = 10);
int ecStrToInt(ecUnistring str, unsigned char base = 10);

//Instance API
ECOREOBJ ecInstCreate(ecErr &errcode, ecParam param = 0);
void ecInstDelete(ECOREOBJ inst);
void ecInstSetCommandline(ECOREOBJ inst, const ecUnistring cmdline);
void ecInstGetCommandline(ECOREOBJ inst, ecUnistring cmdline);
void ecInstSetReturnvalue(ECOREOBJ inst, int value);
void ecInstSetErrorMessage(ECOREOBJ inst, const ecUnistring msg);
ecNativetype ecInstGetWindow(ECOREOBJ inst);
void ecInstExit(ECOREOBJ inst);
void ecInstSetLoopSleep(ECOREOBJ inst, unsigned long tosleep);
bool ecInstSetGlobal(ECOREOBJ inst, ecParam global, unsigned char index = 0);
ecParam ecInstGetGlobal(ECOREOBJ inst, unsigned char index = 0);
void ecInstSustainBacklight(ECOREOBJ inst, bool enable);
int ecInstNativeCallback(ECOREOBJ inst, ecParam param1, ecParam param2, ecParam param3);
ecErr ecInstCreateWindow(ECOREOBJ inst, EC_WINDOWSTYLE *winstyle);
void ecInstAdjustWindow(ECOREOBJ inst, EC_WINDOWSTYLE *winstyle);
void ecInstDestroyWindow(ECOREOBJ inst);
bool ecInstGetWindowRect(ECOREOBJ inst, long *rect, bool aligntoscreen);
ecErr ecInstStartLoop(ECOREOBJ inst);
unsigned char ecInstEventFromButton(ECOREOBJ inst, ecEvent *event, bool setvalue);
void ecInstStatusMsg(ECOREOBJ inst, ecUnistring msg);
void ecInstMinimize(ECOREOBJ inst);

//Display API
ECOREOBJ ecDspCreate(void);
void ecDspDestroy(ECOREOBJ dspobj);
ecErr ecDspOpen(ECOREOBJ dspobj, ecNativetype winhandle, EC_DISPLAYCONFIG *config);
void ecDspClose(ECOREOBJ dspobj);
void *ecDspLock(ECOREOBJ dspobj, long *lpitch, bool checkonly);
void ecDspUnlock(ECOREOBJ dspobj);
void ecDspWaitSync(ECOREOBJ dspobj);
unsigned char ecDspFlipNativeSurface(ECOREOBJ dspobj, ECOREOBJ surfobj, long *pos, long *rect);
void ecDspSuspend(ECOREOBJ dspobj);
void ecDspResume(ECOREOBJ dspobj);
void ecDspSetPalette(ECOREOBJ dspobj, unsigned long *colors);
ecErr ecDspGetVideoMemory(ECOREOBJ dspobj, unsigned long *vmem);
ECOREOBJ ecDspCreateNativeSurface(ECOREOBJ dspobj, ecErr &errcode, bool hardware, bool backbuffer, long width, long height);
void ecDspDestroyNativeSurface(ECOREOBJ dspobj, ECOREOBJ surfobj);
void *ecDspLockNativeSurface(ECOREOBJ dspobj, ECOREOBJ surfobj, long *lpitch);
void ecDspUnLockNativeSurface(ECOREOBJ dspobj, ECOREOBJ surfobj);
void ecDspNativeSetColorkey(ECOREOBJ dspobj, ECOREOBJ surfdst, unsigned long color);
bool ecDspNativeFillRect(ECOREOBJ dspobj, ECOREOBJ surfdst, long *rc, unsigned long color);
bool ecDspNativeBltFast(ECOREOBJ dspobj, ECOREOBJ surfdst, ECOREOBJ surfsrc, long *pos, long *rc, unsigned long flags);
bool ecDspNativeBlt(ECOREOBJ dspobj, ECOREOBJ surfdst, ECOREOBJ surfsrc, long *pos, long *rc, long *cliprc, long *scale, unsigned long rotate, unsigned long flags);
ecNativetype ecDspNativeSurfaceType(ECOREOBJ surfobj);
bool ecDspInfHandleOrientation(ECOREOBJ dspobj);
void ecDspInfNativeResolution(ECOREOBJ dspobj, long *resdata);
unsigned long ecDspInfGetOrientation(ecNativetype winhandle);

//Memory API
void *ecMemAlloc(int count);
void *ecMemReAlloc(void *ptr, int newcount);
void ecMemFree(void *src);
void ecMemCpy(void *dest, const void *src, int count);
void ecMemMove(void *dest, const void *src, int count);
void ecMemSet(void *dest, unsigned char byte, unsigned long count);

//File API
ECOREOBJ ecFOpen(ecErr &errcode, const ecUnistring path, unsigned char openmode);
void ecFClose(ECOREOBJ fileobj);
long ecFRead(ECOREOBJ fileobj, void *buffer, unsigned long bufsize);
long ecFWrite(ECOREOBJ fileobj, const void *buffer, unsigned long bufsize);
long ecFSeek(ECOREOBJ fileobj, long distance, unsigned char type);
long ecFTell(ECOREOBJ fileobj);
ecErr ecFTruncate(const ecUnistring path, unsigned long fsize);
ecErr ecFStat(const ecUnistring path, void *statbuf, bool plocaltime);
ecErr ecFFStat(ECOREOBJ fileobj, void *statbuf, bool localtime);
ecErr ecFRename(const ecUnistring dst, const ecUnistring src);
ecErr ecFRemove(const ecUnistring path);
void ecFGetReadOnlyPath(ecUnistring path);
void ecFGetBinaryPath(ecUnistring path);
ECOREOBJ ecDirOpen(ecErr &errcode, const ecUnistring path);
void ecDirClose(ECOREOBJ dirobj);
bool ecDirRead(ECOREOBJ dirobj, ecUnistring entry);
ecErr ecDirNew(const ecUnistring path);
ecErr ecDirRemove(const ecUnistring path);

//Network API
ECOREOBJ ecNetCreate(unsigned long uid);
void ecNetDestroy(ECOREOBJ netobj);
ecErr ecNetStartBluetoothSearch(ECOREOBJ netobj, bool services);
void ecNetStopBluetoothSearch(ECOREOBJ netobj);
ecErr ecNetFetchBluetoothID(ECOREOBJ netobj, unsigned char *&data, unsigned char &type);
ecErr ecNetStartBluetoothService(ECOREOBJ netobj, ECOREOBJ sockobj, char *servicename, unsigned char port);
void ecNetStopBluetoothService(ECOREOBJ netobj);
unsigned char ecNetBluetoothState(ECOREOBJ netobj);
void ecNetBluetoothSetState(ECOREOBJ netobj, unsigned char state);
bool ecNetOpenInet(ECOREOBJ netobj);
void ecNetCloseInet(ECOREOBJ netobj);
ECOREOBJ ecNetOpenSocket(ECOREOBJ netobj, unsigned char protocol);
void ecNetDestroySocket(ECOREOBJ netobj, ECOREOBJ sockobj);
unsigned long ecNetSockAvailablePort(ECOREOBJ sockobj);
bool ecNetSockBind(ECOREOBJ sockobj, char *address, unsigned long port);
bool ecNetSockListen(ECOREOBJ sockobj, unsigned char maxclients);
void ecNetSockShutdown(ECOREOBJ sockobj);
ECOREOBJ ecNetSockAccept(ECOREOBJ sockobj, char *address);
bool ecNetSockConnect(ECOREOBJ sockobj, const char *host, unsigned long port, char *address);
unsigned char ecNetSockState(ECOREOBJ sockobj);
bool ecNetSockRecv(ECOREOBJ sockobj, unsigned char *data, unsigned long &length);
bool ecNetSockSend(ECOREOBJ sockobj, const unsigned char *data, unsigned long length);
void ecNetRegisterHandler(ECOREOBJ netobj, FUNC_NETHANDLER handler, void *userdata);
void ecNetOnHandler(ECOREOBJ netobj, unsigned short eventtype, void *eventdata);

//Thread API
ECOREOBJ ecTCreate(ecErr &errcode, void *(*entry)(ECOREOBJ), void *param);
void ecTDestroy(ECOREOBJ t, bool waitstop = true);
void *ecTGetParam(ECOREOBJ t);
ECOREOBJ ecTCreateSignal(ecErr &errcode);
void ecTDestroySignal(ECOREOBJ s);
void ecTLockSignal(ECOREOBJ s);
void ecTUnlockSignal(ECOREOBJ s);
void ecTSetSignal(ECOREOBJ s);
void ecTResetSignal(ECOREOBJ s);
bool ecTWaitSignal(ECOREOBJ s, long timeout);

//Camera API
#if defined(DEVICE_WIN32) && defined(EDGE_WINCAMLIB)
#define EDGE_LINK(a, x) __declspec(dllexport) a x
#elif defined(DEVICE_WIN32)
#define EDGE_LINK(a, x) extern a (*x)
#else
#define EDGE_LINK(a, x) a x
#endif
EDGE_LINK(int, ecCamDeviceCount)(void);
EDGE_LINK(ECOREOBJ, ecCamCreate)(ECOREOBJ framework, ECOREOBJ frontend);
EDGE_LINK(void, ecCamDelete)(ECOREOBJ camera);
EDGE_LINK(unsigned long, ecCamRequestCamera)(ECOREOBJ camera, int index);
EDGE_LINK(int, ecCamGetIndex)(ECOREOBJ camera);
EDGE_LINK(int, ecCamGetOrientation)(ECOREOBJ camera);
EDGE_LINK(unsigned long, ecCamStartCapture)(ECOREOBJ camera);
EDGE_LINK(unsigned long, ecCamStopCapture)(ECOREOBJ camera);
EDGE_LINK(int, ecCamGetState)(ECOREOBJ camera);
EDGE_LINK(int, ecCamGetResolutions)(ECOREOBJ camera);
EDGE_LINK(int, ecCamGetCurrentResolution)(ECOREOBJ camera);
EDGE_LINK(void, ecCamGetResolution)(ECOREOBJ camera, int index, ECOREOBJ mode);
EDGE_LINK(unsigned long, ecCamSetResolution)(ECOREOBJ camera, int index);

#if defined(DEVICE_WIN32)
EDGE_LINK(void, ecCamAsyncFrameComplete)(ECOREOBJ camera, long); // for unqueueing an asynchronous camera event
EDGE_LINK(ECOREOBJ, ecCamAsyncFrameRetrieve)(ECOREOBJ camera, long);
EDGE_LINK(void, ecwCamInit)(ECOREOBJ destwnd, char * modelname);
EDGE_LINK(void, ecwCamOrientation)(char orientation);
#endif

#if defined(DEVICE_LINUX)
void ecCamPoll(void);
#endif
