// edgeframe.h
// The EDGELIB main framework class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED     //Can only be included by the main engine
#ifndef EFRAME_INCLUDED   //Make sure the header isn't included multiple times
#define EFRAME_INCLUDED

///////////////////////////////////////////////////////////////////
// Structures
///////////////////////////////////////////////////////////////////

//Edgemain entry info
typedef struct
{
	ENATIVETYPE instance;
	char commandline[MAX_CMDLINE];
	long returnvalue;
}EDGESTARTUP;

typedef struct
{
	unsigned long blightout_half;
	unsigned long blightout_acpower;
	unsigned long blightout_battery;
	unsigned long blightout_screensaver;
	unsigned long keyclick;
	bool restorebacklight;
	bool restorekeyclick;
}EKEEPDEVICESETTINGS;

typedef struct
{
	unsigned long *eventdata;
	unsigned long recordsize;
	unsigned long recordindex;
	unsigned long randomseed;
}EAPPEVENTRECORD;

///////////////////////////////////////////////////////////////////
// Class definition
///////////////////////////////////////////////////////////////////

class ClassEdge
{
	protected:
		virtual ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config);
		virtual ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
		virtual ERESULT OnInit(ENATIVETYPE instance);
		virtual ERESULT OnMinimize(void);
		virtual ERESULT OnRestore(void);
		virtual ERESULT OnPluginLoad(ClassEDisplay *display);
		virtual ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display);
        virtual ERESULT OnContextLost(ClassEDisplay *display);
		virtual ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
		virtual void OnExit(ENATIVETYPE instance);
		virtual void OnButtonDown(unsigned long bnr, EBUTTONLIST *blist);
		virtual void OnButtonUp(unsigned long bnr, EBUTTONLIST *blist);
		virtual void OnWheel(long direction);
		virtual void OnStylusUp(POINT pnt);
		virtual void OnStylusDown(POINT pnt);
		virtual void OnStylusDouble(POINT pnt);
		virtual void OnStylusMove(POINT pnt);
		virtual void OnDeviceTilt(long x, long y, long z);
		virtual bool OnNativeEvent(ENATIVETYPE winhandle, ENATIVETYPE msg, ENATIVETYPE param1, ENATIVETYPE param2);
		virtual bool OnPreNativeEvent(ENATIVETYPE param);		
	private:
		ECOREOBJ curinst;
		ECOREOBJ curwinstyle;
		EDGEDATA e_data;
		ENATIVETYPE winhandle;
		ENATIVETYPE powerhandle;
		EFRAMEWORKCONFIG e_frameworkconfig;
		EDISPLAYCONFIG e_displayconfig;
		EDGETILTEMUSETTINGS e_tiltemu;
		EKEEPDEVICESETTINGS e_keepdevicesettings;
		ClassEStd      e_std;
		ClassEDisplay  e_display;
		ClassESound    e_sound;
		ClassEInput    e_input;
		ClassEMemory   e_memory;
		ClassEVTimer   e_vtimer;
		ClassEFile     e_file;
		ClassEIni      e_ini;
		ClassEMath     e_math;
		ClassERegister e_reg;
		ClassEDevice   e_device;
		ClassEConsole  e_console;
		WCHAR i_errormsg[MAX_ESTR];
		unsigned char *e_eventrecord;
		unsigned char *e_eventrecordplay;
		bool e_nextminimize;
		bool hold_slomo;
		bool hold_ffwd;
		bool recording;
		bool sustainbacklight;
		bool waitflipredraw;
		unsigned long fps;
		unsigned long fpspassed;
		unsigned long fpsstamp;
		unsigned long lockframetmr;
		unsigned long lockframetmrex;
		unsigned long realframetmr;
		unsigned long recframetmr;
		unsigned char appstatus;
		void InitConfig(void);
		void InitVars(void);
		bool CheckConfig(void);
		ERESULT StartFramework(void);
		ERESULT StartDisplay(void);
		void ChangeAppStatus(unsigned char newstatus);
		void CalculateFPS(void);
		void LockFPS(void);
		void PlayRecordingLoop(unsigned long timedelta);
		void MainLoop(void);
		ECOREOBJ validcams[4];

	public:
		ECD ecd;
		EBUTTONLIST e_buttons;
		ClassEdge(void);
		virtual ~ClassEdge(void);
		virtual void OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer);
		virtual void OnDisplayChange(ClassEDisplay *display);
		virtual void OnCameraChange(ClassECamera * camera, ERESULT result);
        virtual void OnCameraFrame(ClassECamera * camera, const E_CAMERAFRAME * result);
		bool StartRecording(unsigned long recordsize);
		void StopRecording(void);
		void FreeRecording(void);
		unsigned long GetRecordingDataSize(void);
		void *GetRecordingData(void);
		void PlayRecording(void *recorddata);
		unsigned char GetSoftlabelPosition(unsigned char *firstkeyposition = NULL);
		void SetErrorMsg(const char *msg);
		void SetErrorMsg(const WCHAR *msg);
		void SetStatusMsg(const char *msg);
		void SetStatusMsg(const WCHAR *msg);
		void StartTiltEmulator(short centeranglex = 0, short centerangley = 675, bool emulatestylus = true, unsigned long stylussensitivity = 7000, bool emulatedpad = false, unsigned long dpadsensitivity = 3000);
		void GetFPS(unsigned long &full, unsigned long &decimal);
		unsigned long GetFPS(void);
		ENATIVETYPE GetWindowHandle(void);
		void static NewLogFile(const char *text);
		void static Log(const char *text, bool newline = true);
		void static Log(const WCHAR *text, bool newline = true);
		void static Log(long value, bool newline = true);
		void Minimize(void);
		void Quit(void);
		bool OnCoreEvent(ECOREOBJ instance, int type, int param1, int param2, int param3);	
        virtual void RegisterCam(ECOREOBJ); // virtual because of dll dependencies
		virtual void UnregisterCam(ECOREOBJ);	
};

#if defined(DEVICE_WIN32)         
    #define WM_CAMERAFRAME (WM_USER + 0)
#endif

#endif
#endif
