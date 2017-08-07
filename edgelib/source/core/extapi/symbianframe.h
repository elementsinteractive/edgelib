// symbianframe.h
// Symbian framework classes
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_SERIES60)
	#include <aknappui.h>
	#include <aknapp.h>
	#include <akndoc.h>
	#include <aknsoundsystem.h>
#elif defined(DEVICE_UIQ)
	#include <qikappui.h>
	#include <qikapplication.h>
	#include <qikdocument.h>
#else
	#include <eikappui.h>
	#include <eikapp.h>
	#include <eikdoc.h>
	#include <eikmenup.h>
#endif

#if defined(SYMBIAN_ETEL3RDPARTY)
	#include <Etel3rdParty.h>
#else
	#include <etel.h>
#endif

#if defined(DEVICE_SERIES60) && (DEVICE_SYMBIAN >= 900)
	#include <hal.h>
#endif

#include <apgtask.h>

///////////////////////////////////////////////////////////////////
// Symbian framework class type definitions
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_UIQ)
	#define CSymDocument CQikDocument
	#define CSymApplication CQikApplication
	#define CSymAppUi CQikAppUi
	#if (DEVICE_SYMBIAN >= 900)
		#include <QikViewBase.h>
		#define CSymView CQikViewBase
	#else
		#define CSymView CCoeControl
	#endif
#elif defined(DEVICE_S60)
	#define CSymDocument CAknDocument
	#define CSymView CCoeControl
	#define CSymApplication CAknApplication
	#define CSymAppUi CAknAppUi
#else
	#define CSymDocument CEikDocument
	#define CSymView CCoeControl
	#define CSymApplication CEikApplication
	#define CSymAppUi CEikAppUi
#endif

///////////////////////////////////////////////////////////////////
// Class definitions
///////////////////////////////////////////////////////////////////

//Keys to capture
#define MAX_CAPTUREKEY    2

//Forward declaration
class CSymCoreNotifyCall;

class CSymCoreView : public CSymView
{
	private:
		ECOREOBJ curinst;
		void Draw(const TRect &aRect) const;
		bool fromorient;
	public:
		#if defined(DEVICE_UIQ) && (DEVICE_SYMBIAN >= 900)
			CSymCoreView(ECOREOBJ inst, CQikAppUi &aAppUi);
			~CSymCoreView();
			static CSymCoreView* NewL(const TRect &aRect, ECOREOBJ inst, CQikAppUi &aAppUi);
			static CSymCoreView* NewLC(const TRect &aRect, ECOREOBJ inst, CQikAppUi &aAppUi);
		#else
			CSymCoreView(ECOREOBJ inst);
			~CSymCoreView();
			static CSymCoreView* NewL(const TRect &aRect, ECOREOBJ inst);
			static CSymCoreView* NewLC(const TRect &aRect, ECOREOBJ inst);
		#endif
		void HandleResourceChange(TInt aType);
		void ConstructL(const TRect &aRect);
		//TCoeInputCapabilities InputCapabilities(void);
		//TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType);
		RWindow& RetrieveWindow();
		#if defined(DEVICE_UIQ) && (DEVICE_SYMBIAN >= 900)
			TVwsViewId ViewId() const;
			void ViewConstructL();
			void ViewDeactivated();
			TVwsViewIdAndMessage ViewScreenDeviceChangedL();
			void HandleUiConfigChangedL(void);
		#endif
};

class CSymCoreUi : public CSymAppUi
{
	private:
		#if defined(DEVICE_UIQ) || defined(DEVICE_S90)
			#if defined(DEVICE_UIQ)
				enum EnumCaptKey
				{
					EInternetKey = 0x0F852,
					ECameraKeyCode = 0x0F853
				};
			#endif
			TInt32 capturedkey[MAX_CAPTUREKEY];
		#endif
		ECOREOBJ curinst;
		CSymCoreNotifyCall *callnotifier;
		bool isminimized;
		bool inconstruct;
		bool nextexit;
		void HandleCommandL(TInt aCommand);
		void HandleWsEventL(const TWsEvent &aEvent, CCoeControl *aDestination);
		void HandleResourceChangeL(TInt aType);
		void ConstructL();
	public:
		CSymCoreView *iAppView;
		void RequestExit();
		void RequestMinimize();
		void NotifyCall();
		CSymCoreUi(ECOREOBJ inst);
		virtual ~CSymCoreUi(void);
};

class CSymCoreDocument : public CSymDocument
{
	private:
		ECOREOBJ curinst;
		void ConstructL();
	public:
		CSymCoreDocument(CEikApplication &aApp, ECOREOBJ inst);
		~CSymCoreDocument();
		static CSymCoreDocument* NewL(CEikApplication &aApp, ECOREOBJ inst);
		static CSymCoreDocument* NewLC(CEikApplication &aApp, ECOREOBJ inst);
		CEikAppUi* CreateAppUiL();
};

class CSymCoreApp : public CSymApplication
{
	private:
		ECOREOBJ curinst;
		CApaDocument* CreateDocumentL();
		TUid AppDllUid() const;
	public:
		CSymCoreApp(ECOREOBJ inst);
		~CSymCoreApp(void);
};

#if !defined(DEVICE_SYMBIANEMU) && (!defined(DEVICE_UIQ) || (DEVICE_SYMBIAN < 900))
	class CSymCoreNotifyCall : public CActive
#else
	class CSymCoreNotifyCall
#endif
{
	#if !defined(DEVICE_SYMBIANEMU) && (!defined(DEVICE_UIQ) || (DEVICE_SYMBIAN < 900))
		private:
			CSymCoreUi *curappui;
			void RunL();
			void DoCancel();
			#if (DEVICE_SYMBIAN >= 900)
				CTelephony *iTelephony;
				CTelephony::TCallStatusV1 iCallStatus;
				CTelephony::TCallStatusV1Pckg iCallStatusPckg;
			#else
				RTelServer iTelserver;
				RTelServer::TPhoneInfo iPhoneinfo;
				RPhone::TLineInfo iLineinfo;
				RPhone iPhone;
				RLine iLine;
				TInt iLines;
			#endif
	#endif
	public:
		CSymCoreNotifyCall();
		~CSymCoreNotifyCall();
		void ConstructL(CSymCoreUi *appui);
		void RequestNotify();
		void CancelRequestNotify();
};

///////////////////////////////////////////////////////////////////
// CSymCoreView
///////////////////////////////////////////////////////////////////

//Draw the view
void CSymCoreView::Draw(const TRect &aRect) const { }

#if defined(DEVICE_UIQ) && (DEVICE_SYMBIAN >= 900)

//View: constructor/destructor
CSymCoreView::CSymCoreView(ECOREOBJ inst, CQikAppUi &aAppUi) : CQikViewBase(aAppUi, KNullViewId){ curinst = inst; }
CSymCoreView::~CSymCoreView(){ }

//View construction phase 1
CSymCoreView* CSymCoreView::NewL(const TRect &aRect, ECOREOBJ inst, CQikAppUi &aAppUi)
{
	CSymCoreView *self = CSymCoreView::NewLC(aRect, inst, aAppUi);
	CleanupStack::Pop();
	return(self);
}

//View construction phase 2
CSymCoreView* CSymCoreView::NewLC(const TRect &aRect, ECOREOBJ inst, CQikAppUi &aAppUi)
{
	CSymCoreView *self = new(ELeave)CSymCoreView(inst, aAppUi);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return(self);
}

#else

//View: constructor and destructor
CSymCoreView::CSymCoreView(ECOREOBJ inst){ curinst = inst; fromorient = false; }
CSymCoreView::~CSymCoreView(){ }

//Prepare the view
CSymCoreView* CSymCoreView::NewL(const TRect &aRect, ECOREOBJ inst)
{
	CSymCoreView *self = CSymCoreView::NewLC(aRect, inst);
	CleanupStack::Pop();
	return(self);
}

//Prepare the view 2
CSymCoreView* CSymCoreView::NewLC(const TRect &aRect, ECOREOBJ inst)
{
	CSymCoreView *self = new(ELeave)CSymCoreView(inst);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return(self);
}

#endif

//Callback for change in screen orientation 
void CSymCoreView::HandleResourceChange(TInt aType)
{
	#if (DEVICE_SYMBIAN >= 900)
		CCoeControl::HandleResourceChange(aType);
		#if defined(DEVICE_SERIES60)
			if (aType == KEikDynamicLayoutVariantSwitch)
			{
				EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)curinst;
				if (io->curwinstyle.fullscreen == 2)
					SetExtentToWholeScreen();
			}
		#endif
	#endif
}

//View main construction
void CSymCoreView::ConstructL(const TRect &aRect)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)curinst;
	#if defined(DEVICE_UIQ) && (DEVICE_SYMBIAN >= 900)
		BaseConstructL();
	#endif
	CreateWindowL();
	if (io->curwinstyle.fullscreen)
		SetExtentToWholeScreen();
	EnableDragEvents();
	#if defined(DEVICE_UIQ) && (DEVICE_SYMBIAN >= 900)
		//SetLayoutManagerL(NULL); //???
	#else
		ActivateL();
	#endif
}

//Retrieve the window
RWindow& CSymCoreView::RetrieveWindow(){ return(Window()); }

//tmp: callback functions for SPB
//TCoeInputCapabilities inputcapability_callback(void);
//void inputkeyevent_callback(TInt keycode);

//Return input capabilities
//TCoeInputCapabilities CSymCoreView::InputCapabilities(void)
//{
	//tmp: use callback for SPB
	//return(inputcapability_callback());
	//return(TCoeInputCapabilities::EAllText); //Set all text
	//return(TCoeInputCapabilities::ENone); //Set none
//}

//Handle key event
//TKeyResponse CSymCoreView::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
//{
	//tmp: use callback for SPB
	//TInt key = aKeyEvent.iCode;
	//inputkeyevent_callback(key);
	//return not consumed key
	//return(EKeyWasNotConsumed);
//}

#if defined(DEVICE_UIQ) && (DEVICE_SYMBIAN >= 900)

//Return the view ID
TVwsViewId CSymCoreView::ViewId() const
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)curinst;
	const TUid KUidSymCoreView = { 0x00000001 }, KUidSymCoreUid = { io->appid };
	return(TVwsViewId(KUidSymCoreUid, KUidSymCoreView));
}

//View extra construction
void CSymCoreView::ViewConstructL()
{
	TQikViewMode viewmode;
	viewmode.SetFullscreen();
	SetViewModeL(viewmode);
	//viewmode.SetAppTitleBar(false);
	//viewmode.SetButtonOrSoftkeyBar(false);
	//viewmode.SetStatusBar(false);
	//viewmode.SetToolbar(false);
	//CQUiConfigClient::Static().SetCurrentConfigL(EQikOrientationNormal);
	//CQUiConfigClient::Static().SetCurrentConfigL(KQikPenStyleTouchPortrait);
	
	//CQUiConfigClient::Static().SetCurrentConfigL(CQUiConfigClient::Static().SystemDefaultUiConfigMode());
}

//View deactivate callback
void CSymCoreView::ViewDeactivated(){ }

//Callback for screen device change
TVwsViewIdAndMessage CSymCoreView::ViewScreenDeviceChangedL()
{
	return(TVwsViewIdAndMessage(ViewId()));
}

//Callback when UI configuration/layout changes
void CSymCoreView::HandleUiConfigChangedL(void)
{
	ecEvent event;
	CQikViewBase::HandleUiConfigChangedL();
	TQikUiConfig config = CQUiConfigClient::Static().CurrentConfig();
	event.type = ECEV_CHANGEORIENTATION;
	event.param1 = 0;
	switch(config.ScreenMode())
	{
		case EQikUiConfigPortrait:
			event.param1 = 0;
			break;
		case EQikUiConfigSmallPortrait:
			event.param1 = 0;
			break;
		case EQikUiConfigLandscape:
			event.param1 = 90;
			break;
		case EQikUiConfigSmallLandscape:
			event.param1 = 90;
			break;
		default:
			break;
	}
	ecOnEvent(curinst, &event);
}

#endif

///////////////////////////////////////////////////////////////////
// User interface class
///////////////////////////////////////////////////////////////////

//Command event procedure
void CSymCoreUi::HandleCommandL(TInt aCommand)
{
	switch(aCommand)
	{
		case EEikCmdExit:
			EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)curinst;
			io->runidletimer = false;
			ecInstExit(curinst);
			break;
	}
}

//Message event loop
void CSymCoreUi::HandleWsEventL(const TWsEvent &aEvent, CCoeControl *aDestination)
{
	switch(aEvent.Type())
	{
		#if defined(DEVICE_UIQ)
			case EEventKey:
				if (aEvent.Key()->iCode == ECameraKeyCode || aEvent.Key()->iCode == EInternetKey)
					return;
				break;
		#endif
		//case EEventScreenDeviceChanged:
		case EEventSwitchOff:
		case EEventCaseClosed:
		case EEventFocusLost:
			if (isminimized)
			{
				if (iAppView != NULL)
				{
					iAppView->SetFocus(EFalse);
					#if !defined(DEVICE_UIQ) || (DEVICE_SYMBIAN < 900)
						iAppView->SetExtent(TPoint(0, 0), TSize(0, 0));
					#endif
				}
				isminimized = true;
			}
			break;
		case EEventFocusGained:
			if (!isminimized)
			{
				#if defined(DEVICE_SERIES60)
					EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)curinst;
					CAknAppUi::SetKeyBlockMode(ENoKeyBlock);
					if (!io->curwinstyle.keysounds)
					{
						KeySounds()->BringToForeground();
						KeySounds()->PushContextL(R_AVKON_SILENT_SKEY_LIST);
					}
				#endif
				if (iAppView != NULL)
				{
					iAppView->SetExtentToWholeScreen();
					iAppView->SetFocus(ETrue);
				}
				if (callnotifier != NULL)
					callnotifier->RequestNotify();
				isminimized = false;
			}
			break;
	}
	if (ecInstNativeCallback(curinst, (ecParam)&aEvent, (ecParam)aDestination, 0) == 0)
		CSymAppUi::HandleWsEventL(aEvent, aDestination);
}

//Called when layout changes between portrait and landscape
void CSymCoreUi::HandleResourceChangeL(TInt aType)
{
	#if (DEVICE_SYMBIAN >= 900)
		CSymAppUi::HandleResourceChangeL(aType);
		#if defined(DEVICE_SERIES60)
			if (aType == KEikDynamicLayoutVariantSwitch && iAppView)
			{
				iAppView->HandleResourceChange(aType);
				ecEvent event;
				event.type = ECEV_CHANGEORIENTATION;
				event.param1 = 0;
				TPixelsAndRotation sizerotation;
				CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(sizerotation);
				switch(sizerotation.iRotation)
				{
					case CFbsBitGc::EGraphicsOrientationNormal:
						event.param1 = 0;
						break;
					case CFbsBitGc::EGraphicsOrientationRotated90:
						event.param1 = 90;
						break;
					case CFbsBitGc::EGraphicsOrientationRotated180:
						event.param1 = 180;
						break;
					case CFbsBitGc::EGraphicsOrientationRotated270:
						event.param1 = 270;
						break;
				}
				ecOnEvent(curinst, &event);
			}
		#endif
	#endif
}

//Construct the application framework
void CSymCoreUi::ConstructL()
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)curinst;
	inconstruct = true;
	nextexit = false;
	BaseConstructL(0x00800000); //EAknTouchCompatibleFlag
	RWindowGroup& rwg = iCoeEnv->RootWin();
	rwg.EnableScreenChangeEvents();
	rwg.AutoForeground(ETrue);
	#if defined(DEVICE_SERIES60)
		#if (DEVICE_SYMBIAN >= 900)
			if (io->curwinstyle.forceorientation > 0)
			{
				TInt xpixels, ypixels;
				TPixelsAndRotation sizerot_r;
				CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(sizerot_r);
				switch(sizerot_r.iRotation)
				{
					case CFbsBitGc::EGraphicsOrientationRotated90:
					case CFbsBitGc::EGraphicsOrientationRotated270:
						xpixels = sizerot_r.iPixelSize.iHeight;
						ypixels = sizerot_r.iPixelSize.iWidth;
						break;
					default:
						xpixels = sizerot_r.iPixelSize.iWidth;
						ypixels = sizerot_r.iPixelSize.iHeight;
						break;
				}
				if (io->curwinstyle.forceorientation == 1)
					CAknAppUiBase::SetOrientationL(CAknAppUiBase::EAppUiOrientationPortrait);
				else if (io->curwinstyle.forceorientation == 2)
					CAknAppUiBase::SetOrientationL(CAknAppUiBase::EAppUiOrientationLandscape);
				else if (xpixels < ypixels)
					CAknAppUiBase::SetOrientationL(CAknAppUiBase::EAppUiOrientationPortrait);
				else
					CAknAppUiBase::SetOrientationL(CAknAppUiBase::EAppUiOrientationLandscape);
			}
		#endif
	#endif
	#if defined(DEVICE_UIQ) && (DEVICE_SYMBIAN >= 900)
		iAppView = CSymCoreView::NewLC(ClientRect(), curinst, *this);
		AddViewL(*iAppView);
		CleanupStack::Pop(iAppView);
	#else
		iAppView = CSymCoreView::NewL(ClientRect(), curinst);
		AddToStackL(iAppView);
	#endif
	rwg = iCoeEnv->RootWin();
	rwg.EnableScreenChangeEvents();
	rwg.AutoForeground(ETrue);
	#if defined(DEVICE_SERIES90)
		if (io->curwinstyle.lockkeys)
		{
			if (capturedkey[0] == 0)
				capturedkey[0] = rwg.CaptureKeyUpAndDowns(EStdKeyMenu, 0, 0);
			if (capturedkey[1] == 0)
				capturedkey[1] = rwg.CaptureKeyUpAndDowns(EStdKeyEscape, 0, 0, 0xFFFFFF);
		}
	#elif defined(DEVICE_UIQ)
		if (io->curwinstyle.lockkeys)
		{
			if (capturedkey[0] == 0)
				capturedkey[0] = rwg.CaptureKey(ECameraKeyCode, 0, 0, 2);
			if (capturedkey[1] == 0)
				capturedkey[1] = rwg.CaptureKey(EInternetKey, 0, 0, 2);
		}
	#endif
	io->wnd = &iAppView->RetrieveWindow();
	ecEvent event;
	event.type = ECEV_CHANGEORIENTATION;
	event.param1 = 0;
	TPixelsAndRotation sizerotation;
	CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(sizerotation);
	switch(sizerotation.iRotation)
	{
		case CFbsBitGc::EGraphicsOrientationNormal:
			event.param1 = 0;
			break;
		case CFbsBitGc::EGraphicsOrientationRotated90:
			event.param1 = 90;
			break;
		case CFbsBitGc::EGraphicsOrientationRotated180:
			event.param1 = 180;
			break;
		case CFbsBitGc::EGraphicsOrientationRotated270:
			event.param1 = 270;
			break;
	}
	if (event.param1 != 0)
		ecOnEvent(curinst, &event);
	event.type = ECEV_CREATEWINDOW;
	event.param1 = 0;
	ecOnEvent(curinst, &event);
	if (!nextexit)
	{
		callnotifier = new CSymCoreNotifyCall();
		callnotifier->ConstructL(this);
	}
	else
		Exit();
	inconstruct = false;
}

//Request application to exit
void CSymCoreUi::RequestExit()
{
	#if defined(DEVICE_SERIES60)
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)curinst;
		if (!io->curwinstyle.keysounds)
			KeySounds()->PopContext();
	#endif
	if (inconstruct)
		nextexit = true;
	else
		Exit();
}

//Request application minimize
void CSymCoreUi::RequestMinimize()
{
	TApaTask task(iEikonEnv->WsSession());
	task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
	task.SendToBackground();
}

//Called when the user receives a call
void CSymCoreUi::NotifyCall()
{
	/*TWsEvent aEvent;
	aEvent.SetType(EEventFocusLost);
	HandleWsEventL(aEvent, NULL);
	TApaTask task(iEikonEnv->WsSession());
	task.SetWgId(CEikonEnv::Static()->RootWin().Identifier());
	task.SendToBackground();*/
	ecEvent event;
	event.type = ECEV_PHONECALL;
	ecOnEvent(curinst, &event);
}

//CSymCoreUi: constructor
CSymCoreUi::CSymCoreUi(ECOREOBJ inst)
{
	#if defined(DEVICE_UIQ) || defined(DEVICE_S90)
		for (unsigned char ctr = 0; ctr < MAX_CAPTUREKEY; ctr++)
			capturedkey[ctr] = 0;
	#endif
	curinst = inst;
	iAppView = NULL;
	callnotifier = NULL;
	isminimized = false;
}

//CSymCoreUi: destructor
CSymCoreUi::~CSymCoreUi(void)
{
	#if defined(DEVICE_UIQ) || defined(DEVICE_S90)
		RWindowGroup &rwg = iCoeEnv->RootWin();
		for (unsigned char ctr = 0; ctr < MAX_CAPTUREKEY; ctr++)
			#ifdef DEVICE_UIQ
				if (capturedkey[ctr] > 0)
				{
					rwg.CancelCaptureKey(capturedkey[ctr]);
					capturedkey[ctr] = 0;
				}
			#else
				if (capturedkey[ctr] > 0)
				{
					rwg.CancelCaptureKeyUpAndDowns(capturedkey[ctr]);
					capturedkey[ctr] = 0;
				}
			#endif
	#endif
	if (callnotifier != NULL)
		delete callnotifier;
	if (iAppView)
	{
		#if !defined(DEVICE_UIQ) || (DEVICE_SYMBIAN < 900)
			RemoveFromStack(iAppView);
			delete iAppView;
			iAppView = NULL;
		#endif
	}
}

///////////////////////////////////////////////////////////////////
// CSymCoreDocument
///////////////////////////////////////////////////////////////////

//Document construction phase 1
CSymCoreDocument* CSymCoreDocument::NewL(CEikApplication &aApp, ECOREOBJ inst)
{
	CSymCoreDocument *self = NewLC(aApp, inst);
	CleanupStack::Pop(self);
	return(self);
}

//Document construction phase 2
CSymCoreDocument* CSymCoreDocument::NewLC(CEikApplication &aApp, ECOREOBJ inst)
{
	CSymCoreDocument *self = new(ELeave)CSymCoreDocument(aApp, inst);
	CleanupStack::PushL(self);
	self->ConstructL();
	return(self);
}

//Document main construction
void CSymCoreDocument::ConstructL(){ }

//Document: constructor/destructor
CSymCoreDocument::CSymCoreDocument(CEikApplication &aApp, ECOREOBJ inst) : CSymDocument(aApp){ curinst = inst; }
CSymCoreDocument::~CSymCoreDocument(){ }

//Document: create user interface
CEikAppUi* CSymCoreDocument::CreateAppUiL()
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)curinst;
	ecEvent event;
	event.type = ECEV_CREATEINSTANCE;
	event.param1 = 0;
	ecOnEvent(curinst, &event);
	return((CSymCoreUi *)io->frameui);
}

///////////////////////////////////////////////////////////////////
// CSymCoreApp
///////////////////////////////////////////////////////////////////

//Create a new document
CApaDocument* CSymCoreApp::CreateDocumentL(){ return(CSymCoreDocument::NewL(*this, curinst)); }

//Inform Symbian about the application ID
TUid CSymCoreApp::AppDllUid() const
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)curinst;
	const TUid KUidApp = { io->appid };
	return(KUidApp);
}

//Constructor/Destructor
CSymCoreApp::CSymCoreApp(ECOREOBJ inst)
{
	ecEvent event;
	curinst = inst;
	event.type = ECEV_REQUESTUID;
	ecOnEvent(curinst, &event);
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)curinst;
	io->appid = event.param1;
}
CSymCoreApp::~CSymCoreApp(void)
{
	ecEvent event;
	event.type = ECEV_DESTROYINSTANCE;
	ecOnEvent(curinst, &event);
	ecInstDelete(curinst);
}

///////////////////////////////////////////////////////////////////
// CSymCoreNotifyCall
///////////////////////////////////////////////////////////////////

//CSymCoreNotifyCall: Constructor
#if !defined(DEVICE_SYMBIANEMU) && (!defined(DEVICE_UIQ) || (DEVICE_SYMBIAN < 900))
	#if (DEVICE_SYMBIAN >= 900)
		CSymCoreNotifyCall::CSymCoreNotifyCall() : CActive(CActive::EPriorityIdle), iCallStatusPckg(iCallStatus){ iTelephony = NULL; }
	#else
		CSymCoreNotifyCall::CSymCoreNotifyCall() : CActive(CActive::EPriorityIdle){ }
	#endif
#else
	CSymCoreNotifyCall::CSymCoreNotifyCall(){ }
#endif

//CSymCoreNotifyCall: Destructor
CSymCoreNotifyCall::~CSymCoreNotifyCall()
{
	#if !defined(DEVICE_SYMBIANEMU) && (!defined(DEVICE_UIQ) || (DEVICE_SYMBIAN < 900))
		#if (DEVICE_SYMBIAN >= 900)
			if (iTelephony != NULL)
			{
				Cancel();
				delete iTelephony;
				iTelephony = NULL;
			}
		#else
			Cancel();
			iLine.NotifyIncomingCallCancel();
			iLine.Close();
			iPhone.Close();
			iTelserver.Close();
		#endif
	#endif
}

//Object construction
void CSymCoreNotifyCall::ConstructL(CSymCoreUi *appui)
{
	#if !defined(DEVICE_SYMBIANEMU) && (!defined(DEVICE_UIQ) || (DEVICE_SYMBIAN < 900))
		#if (DEVICE_SYMBIAN >= 900)
			curappui = appui;
			iTelephony = CTelephony::NewL();
			if (iTelephony != NULL)
				CActiveScheduler::Add(this);
		#else
			iTelserver.Connect();
			iTelserver.GetPhoneInfo(0, iPhoneinfo);
			iPhone.Open(iTelserver, iPhoneinfo.iName);
			iPhone.EnumerateLines(iLines);
			iPhone.GetLineInfo(0, iLineinfo);
			iLine.Open(iPhone, iLineinfo.iName);
			curappui = appui;
			CActiveScheduler::Add(this);
		#endif
	#endif
}

//Start requesting notifications
void CSymCoreNotifyCall::RequestNotify()
{
	#if !defined(DEVICE_SYMBIANEMU) && (!defined(DEVICE_UIQ) || (DEVICE_SYMBIAN < 900))
		if (IsActive()){ return; }
		#if (DEVICE_SYMBIAN >= 900)
			if (iTelephony != NULL)
			{
				iTelephony->NotifyChange(iStatus, CTelephony::EVoiceLineStatusChange, iCallStatusPckg);
				SetActive();
			}
		#else
			iLine.NotifyIncomingCall(iStatus, iLineinfo.iName);
			SetActive();
		#endif
	#endif
}

//Cancel request for notifications
void CSymCoreNotifyCall::CancelRequestNotify()
{
	#if !defined(DEVICE_SYMBIANEMU) && (!defined(DEVICE_UIQ) || (DEVICE_SYMBIAN < 900))
		Cancel();
	#endif
}

#if !defined(DEVICE_SYMBIANEMU) && (!defined(DEVICE_UIQ) || (DEVICE_SYMBIAN < 900))

//Run the call notifier
void CSymCoreNotifyCall::RunL()
{
	#if (DEVICE_SYMBIAN >= 900)
		if (iCallStatus.iStatus == CTelephony::EStatusRinging)
			curappui->NotifyCall();
		iTelephony->NotifyChange(iStatus, CTelephony::EVoiceLineStatusChange, iCallStatusPckg);
		SetActive();
	#else
		curappui->NotifyCall();
	#endif
}

//Cancel the call notifier
void CSymCoreNotifyCall::DoCancel()
{
	#if (DEVICE_SYMBIAN >= 900)
		if (iTelephony != NULL)
			iTelephony->CancelAsync(CTelephony::EVoiceLineStatusChangeCancel);
	#else
		iLine.NotifyIncomingCallCancel();
	#endif
}

#endif
