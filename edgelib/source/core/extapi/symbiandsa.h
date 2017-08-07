// symbiandsa.h
// Direct Access Framework for Symbian, dummy for Series 80 1st edition
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// DSA framework                                                 //
///////////////////////////////////////////////////////////////////

class ClassDSA : public CTimer, public MDirectScreenAccess
{
	friend class ClassEDisplay;
	private:
		RWsSession *iClient;
		RWindow *iWindow;
		CDirectScreenAccess *iDirectScreenAccess;
		CFbsBitGc *bitgc;
		RRegion *iRegion;
		bool dsaactive;
		void Restart(RDirectScreenAccess::TTerminationReasons aReason);
		void AbortNow(RDirectScreenAccess::TTerminationReasons aReason);
		void RunL();
		void DoCancel();
	public:
		CWsScreenDevice *iScreenDevice;
		ClassDSA(RWindow *aWindow);
		~ClassDSA();
		bool ConstructL();
		void StartDSA();
		void StopDSA();
		void StartFlip(const TPoint &aPoint, const CFbsBitmap *aBitmap, const TRect &aSourceRect, const TRegion &aRegion);
};

//Callbacks from DirectScreenAccess
void ClassDSA::Restart(RDirectScreenAccess::TTerminationReasons /*aReason*/){ StartDSA(); }
void ClassDSA::AbortNow(RDirectScreenAccess::TTerminationReasons /*aReason*/)
{
	Cancel();
	dsaactive = false;
}

//Active object loop
void ClassDSA::RunL()
{
	After(TTimeIntervalMicroSeconds32(10)); //After 10 ms
}

//Cancel active object
void ClassDSA::DoCancel()
{
	CTimer::DoCancel();
	if (iDirectScreenAccess != NULL)
		iDirectScreenAccess->Cancel();
}

//ClassDSA: Constructor
ClassDSA::ClassDSA(RWindow *aWindow) : CTimer(CActive::EPriorityStandard)
{
	iDirectScreenAccess = NULL;
	iClient = &CEikonEnv::Static()->WsSession();
	iWindow = aWindow;
	iScreenDevice = (CCoeEnv::Static()->ScreenDevice());
}

//ClassDSA: Destructor
ClassDSA::~ClassDSA()
{
	Cancel();
	if (iDirectScreenAccess != NULL)
	{
		delete iDirectScreenAccess;
		iDirectScreenAccess = NULL;
	}
}

//ClassDSA: Object construction
bool ClassDSA::ConstructL()
{
	dsaactive = false;
	CTimer::ConstructL();
	iDirectScreenAccess = CDirectScreenAccess::NewL(*iClient, *iScreenDevice, *iWindow, *this);
	if (iDirectScreenAccess == NULL)
		return(false);
	CActiveScheduler::Add(this);
	return(true);
}

//Start the active object
void ClassDSA::StartDSA()
{
	if (!dsaactive)
	{
		TRAPD(dsaErr, iDirectScreenAccess->StartL());
		if (dsaErr == KErrNone)
		{
			dsaactive = true;
			bitgc = iDirectScreenAccess->Gc();
			iRegion = iDirectScreenAccess->DrawingRegion();
			bitgc->SetClippingRegion(iRegion); 
			After(TTimeIntervalMicroSeconds32(10)); //After 10 ms
		}
	}
}

//Stop the active object
void ClassDSA::StopDSA()
{
	if (dsaactive)
	{
		Cancel();
		dsaactive = false;
	}
}

//Update the screen device with the display buffer
void ClassDSA::StartFlip(const TPoint &aPoint, const CFbsBitmap *aBitmap, const TRect &aSourceRect, const TRegion &aRegion)
{
	if (dsaactive)
	{
		bitgc->BitBlt(aPoint, aBitmap, aSourceRect);
		iDirectScreenAccess->ScreenDevice()->Update(/*aRegion*/);
	}
}
