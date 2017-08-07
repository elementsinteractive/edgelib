// imp_symbiantilt.h
// Symbian TiltControl interface class (Symbian 9+)
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Maximum name for sensor
const TInt KMaxSensorName = 128;

//Sensor info
class TRRSensorInfo
{
	public:
		TInt iSensorCategory;
		TInt iSensorId;
		TBuf<KMaxSensorName> iSensorName;
};

//Sensor event structure
class TRRSensorEvent
{
	public:
		TInt iSensorData1;
		TInt iSensorData2;
		TInt iSensorData3;
};

//Sensor listener parent class
class MRRSensorDataListener
{
	public:
		virtual void HandleDataEventL( TRRSensorInfo aSensor, TRRSensorEvent aEvent ) = 0;
};

//Sensor API class
class CRRSensorApi : public CBase
{
	public:
		IMPORT_C static CRRSensorApi *NewL(TRRSensorInfo aSensor);
		IMPORT_C static void FindSensorsL(RArray<TRRSensorInfo>& aSensorInfoArray);
		virtual void AddDataListener(MRRSensorDataListener *aListener) = 0;
		virtual void RemoveDataListener() = 0;
		virtual TInt SensorCommand(TInt& aCommand, TInt& aValue) = 0;
};

//Imports
typedef void (*FUNC_SENSORFINDSENSORSL)(RArray<TRRSensorInfo>&aSensorInfoArray);
typedef CRRSensorApi* (*FUNC_SENSORNEWL)(TRRSensorInfo aSensor);

//Sensor container class
class CSymSensorTilt : public MRRSensorDataListener
{
	private:
		CRRSensorApi* iAccSensor;
		RLibrary rlib;
		ECOREOBJ curinst;
		bool dllloaded;
	public:
		CSymSensorTilt(ECOREOBJ inst);
		virtual ~CSymSensorTilt(void);
		bool StartMonitoring(void);
		void StopMonitoring(void);
		void HandleDataEventL(TRRSensorInfo aSensor, TRRSensorEvent aEvent);
};

///////////////////////////////////////////////////////////////////
// CSymServiceDiscoverer implementation
///////////////////////////////////////////////////////////////////

//Callback
bool ecOnEvent(ECOREOBJ instance, ecEvent *event);

//CSymSensorTilt: constructor
CSymSensorTilt::CSymSensorTilt(ECOREOBJ inst)
{
	curinst = inst;
	iAccSensor = NULL;
	dllloaded = false;
}

//CSymSensorTilt: destructor
CSymSensorTilt::~CSymSensorTilt(void)
{
	StopMonitoring();
}

//Start monitoring of tilt events
bool CSymSensorTilt::StartMonitoring(void)
{
	const TInt Kacc = 0x10273024;
	StopMonitoring();
	if (!dllloaded)
	{
		_LIT(kdllname, "RRSensorApi.dll");
		if (rlib.Load(kdllname, KNullUid) == KErrNone)
			dllloaded = true;
		if (!dllloaded)
			return(false);
	}
	RArray <TRRSensorInfo> sensorList;
	FUNC_SENSORFINDSENSORSL func_findsensorsl = (FUNC_SENSORFINDSENSORSL)rlib.Lookup(1);
	FUNC_SENSORNEWL func_newl = (FUNC_SENSORNEWL)rlib.Lookup(2);
	if (func_findsensorsl == NULL || func_newl == NULL)
		return(false);
	func_findsensorsl(sensorList);
	TInt sensorCount = sensorList.Count();
	if (sensorCount == 0)
		return(false);
	for (TInt i = 0; i != sensorCount; i++)
	{
		if (sensorList[i].iSensorId == Kacc)
    	{
			iAccSensor = func_newl(sensorList[i]);
			if (iAccSensor)
			{
				ecEvent event;
				event.type = ECEV_CHANGETILT;
				event.param1 = 0;
				event.param2 = 0;
				event.param3 = 0xFFFFFF;
				ecOnEvent(curinst, &event);
				iAccSensor->AddDataListener(this);
				return(true);
			}
		}
	}
	return(false);
}

//Stop monitoring of tilt events
void CSymSensorTilt::StopMonitoring(void)
{
	if (iAccSensor)
	{
		iAccSensor->RemoveDataListener();    
		delete iAccSensor;
		iAccSensor = NULL;
	}
}

//Event handler
void CSymSensorTilt::HandleDataEventL(TRRSensorInfo aSensor, TRRSensorEvent aEvent)
{
	ecEvent event;
	event.type = ECEV_CHANGETILT;
	event.param1 = aEvent.iSensorData2 * -32768 / 300;
	event.param2 = aEvent.iSensorData1 * -32768 / 300;
	event.param3 = aEvent.iSensorData3 * -32768 / 300;
	ecOnEvent(curinst, &event);
}
