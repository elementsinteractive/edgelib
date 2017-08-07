// symbianbtapi.h
// Symbian Bluetooth interface classes
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Class definitions
///////////////////////////////////////////////////////////////////

//Forward declarations
class CSymDeviceDiscoverer;

class CSymServiceDiscoverer : public CBase, public MSdpAgentNotifier, public MSdpAttributeValueVisitor
{
	friend class CSymDeviceDiscoverer;
	private:
		TBTDevAddr btaddr;
		CSymDeviceDiscoverer *discoverer;
		CSdpAgent *iAgent;
		CSdpSearchPattern *iSpat;
		TUUID iLastUUID;
		TUint iPort;
		char foundservice[256];
		unsigned long basicarrsize;
		unsigned char foundport;
		unsigned char readstate;
		void ConstructL();
		void NextRecordRequestComplete(TInt aError, TSdpServRecordHandle aHandle, TInt aTotalRecordsCount);
		void AttributeRequestResult(TSdpServRecordHandle aHandle, TSdpAttributeID aAttrID, CSdpAttrValue* aAttrValue);
		void AttributeRequestComplete(TSdpServRecordHandle aHandle, TInt aError);
		void VisitAttributeValueL(CSdpAttrValue &aValue, TSdpElementType aType);
		void StartListL(CSdpAttrValueList &aList);
		void EndListL();
	public:
		static CSymServiceDiscoverer *NewL(CSymDeviceDiscoverer *observer);
		static CSymServiceDiscoverer *NewLC(CSymDeviceDiscoverer *observer);
		CSymServiceDiscoverer(CSymDeviceDiscoverer *observer);
		~CSymServiceDiscoverer();
		bool DiscoverServices(TBTDevAddr usebtaddr);
		void CancelProcess(void);
};

class CSymDeviceDiscoverer : public CActive
{
	friend class CSymServiceDiscoverer;
	private:
		EC_NETOBJ *neto;
		CSymServiceDiscoverer *servicediscoverer;
		RHostResolver resolver;
		TInquirySockAddr inquiriaddr;
		TNameEntry nameentry;
		unsigned char arraydata[512];
		bool enddevicequeue;
		bool servicebusy;
		bool searchinprogress;
		bool requestrestart;
		void ConstructL();
		void RunL();
		void DoCancel();
		void BTAtoA(char *btstr, TBTSockAddr address);
	public:
		static CSymDeviceDiscoverer *NewL(void *userdata);
		static CSymDeviceDiscoverer *NewLC(void *userdata);
		CSymDeviceDiscoverer(void *userdata);
		~CSymDeviceDiscoverer();
		bool DiscoverDevices(void);
		void ServiceComplete(bool abort);
};

class CSymBTAdvertiser : public CBase
{
	private:
		bool isconnected;
		RSdp iSdpSession;
		RSdpDatabase iSdpDatabase;
		TSdpServRecordHandle rechandle;
		void BuildProtocolDescriptionL(CSdpAttrValueDES *aProtocolDescriptor, TInt aPort);
	public:
		static CSymBTAdvertiser *NewL(void);
		static CSymBTAdvertiser *NewLC(void);
		CSymBTAdvertiser(void);
		~CSymBTAdvertiser(void);
		bool StartAdvertising(char *servicename, unsigned char port);
		void StopAdvertising(void);
};

///////////////////////////////////////////////////////////////////
// CSymServiceDiscoverer implementation
///////////////////////////////////////////////////////////////////

//Construct object
void CSymServiceDiscoverer::ConstructL(){ }

//Found a service SDP record on device
void CSymServiceDiscoverer::NextRecordRequestComplete(TInt aError, TSdpServRecordHandle aHandle, TInt aTotalRecordsCount)
{
	if (discoverer->neto->abortsearch)
	{
		CancelProcess();
		discoverer->ServiceComplete(true);
		return;
	}
	if (aError == KErrNone && aTotalRecordsCount > 0)
	{
		ecStrCpy(foundservice, "");
		foundport = 0;
		readstate = 0; //Get protocol descriptor
		iAgent->AttributeRequestL(aHandle, KSdpAttrIdProtocolDescriptorList);
	}
	else
	{
		CancelProcess();
		discoverer->ServiceComplete(false);
	}
}

//Attribute found, send to parser
void CSymServiceDiscoverer::AttributeRequestResult(TSdpServRecordHandle /*aHandle*/, TSdpAttributeID /*aAttrID*/, CSdpAttrValue* aAttrValue)
{
	aAttrValue->AcceptVisitorL(*this);
}

//Parsing of SDP record is complete
void CSymServiceDiscoverer::AttributeRequestComplete(TSdpServRecordHandle aHandle, TInt aError)
{
	if (aError == KErrNone)
	{
		if (readstate >= 2) //Try to get service or service has been found
		{
			unsigned long arraydatasize = basicarrsize;
			discoverer->arraydata[20] = foundport;
			discoverer->arraydata[arraydatasize] = ecStrLen(foundservice) + 1;;
			ecStrCpy((char *)&discoverer->arraydata[arraydatasize + 1], foundservice);
			arraydatasize += discoverer->arraydata[arraydatasize] + 1;
			if (foundport > 0)
				if (!eciNetDeviceCallback(0, discoverer->arraydata, arraydatasize, discoverer->neto))
				{
					CancelProcess();
					discoverer->ServiceComplete(false);
					return;
				}
		}
		if (readstate == 1) //Next call to get service name
		{
			readstate = 2; //Get service name
			iAgent->AttributeRequestL(aHandle, KSdpAttrIdBasePrimaryLanguage + KSdpAttrIdOffsetServiceName);
		}
		else
			iAgent->NextRecordRequestL();
	}
	else
		iAgent->NextRecordRequestL();
}

//Service record parser
void CSymServiceDiscoverer::VisitAttributeValueL(CSdpAttrValue &aValue, TSdpElementType aType)
{
	switch (aType)
	{
		case ETypeUUID:
		{
			if (readstate == 0) //Get protocol descriptor
			{
				TPtrC8 uuid(aValue.UUID().ShortestForm());
				iLastUUID.SetL(uuid);
			}
			break;
		}
		case ETypeUint:
			if (readstate == 0) //Get protocol descriptor
			{
				if (iLastUUID == KRFCOMM)
					foundport = (unsigned char)aValue.Uint();
				readstate = 1; //Next call to get service name
			}
			break;
		case ETypeString:
			if (readstate == 2) //Get service name
			{
				readstate = 3; //Service found
				for (unsigned char ctr = 0; ctr < 255 && ctr < aValue.Des().Length(); ctr++)
				{
					foundservice[ctr] = aValue.Des()[ctr];
					foundservice[ctr + 1] = 0;
				}
			}
			break;
		default:
			break;
	}
}

//List functions (not needed)
void CSymServiceDiscoverer::StartListL(CSdpAttrValueList& /*aList*/){ }
void CSymServiceDiscoverer::EndListL(){ }

//Construction phase 1
CSymServiceDiscoverer *CSymServiceDiscoverer::NewL(CSymDeviceDiscoverer *observer)
{
	CSymServiceDiscoverer *self = CSymServiceDiscoverer::NewLC(observer);
	CleanupStack::Pop(self);
	return(self);
}

//Construction phase 2
CSymServiceDiscoverer *CSymServiceDiscoverer::NewLC(CSymDeviceDiscoverer *observer)
{
	CSymServiceDiscoverer *self = new(ELeave)CSymServiceDiscoverer(observer);
	CleanupStack::PushL(self);
	self->ConstructL();
	return(self);
}

//CSymServiceDiscoverer: constructor
CSymServiceDiscoverer::CSymServiceDiscoverer(CSymDeviceDiscoverer *observer)
{
	discoverer = observer;
}

//CSymServiceDiscoverer: destructor
CSymServiceDiscoverer::~CSymServiceDiscoverer()
{
	CancelProcess();
}

//Start discovering services
bool CSymServiceDiscoverer::DiscoverServices(TBTDevAddr usebtaddr)
{
	btaddr = usebtaddr;
	CancelProcess();
	iAgent = CSdpAgent::NewL(*this, btaddr);
	if (iAgent == NULL)
		return(false);
	iSpat = CSdpSearchPattern::NewL();
	if (iSpat == NULL)
		return(false);
	TUUID serviceUUID(0x0100);
	iSpat->AddL(serviceUUID);
	iAgent->SetRecordFilterL(*iSpat);
	iAgent->NextRecordRequestL();
	return(true);
}

//Cancel discovery process
void CSymServiceDiscoverer::CancelProcess(void)
{
	if (iAgent)
		iAgent->Cancel();
	delete iAgent;
	iAgent = NULL;
	if(iSpat)
		iSpat->Reset();
	delete iSpat;
	iSpat = NULL;
}

///////////////////////////////////////////////////////////////////
// CSymDeviceDiscoverer implementation
///////////////////////////////////////////////////////////////////

//Class construction
void CSymDeviceDiscoverer::ConstructL(){ }

//Active object run process
void CSymDeviceDiscoverer::RunL()
{
	if (iStatus == KErrNone)
	{
		TBTSockAddr sockaddr;
		unsigned long ctr, arraydatasize = 21;
		sockaddr.SetBTAddr(static_cast<TBTSockAddr>(nameentry().iAddr).BTAddr());
		arraydata[0] = 18;
		BTAtoA((char *)&arraydata[1], sockaddr);
		arraydata[19] = 1;
		arraydata[20] = 0;
		arraydata[arraydatasize] = nameentry().iName.Length() + 1;
		arraydata[arraydatasize + 1] = 0;
		for (ctr = 0; ctr < (unsigned long)(arraydata[arraydatasize] - 1); ctr++)
		{
			arraydata[arraydatasize + 1 + ctr] = nameentry().iName[ctr];
			arraydata[arraydatasize + 1 + ctr + 1] = 0;
		}
		arraydatasize += arraydata[arraydatasize] + 1;
		arraydata[arraydatasize] = 1;
		arraydata[arraydatasize + 1] = 0;
		arraydatasize += arraydata[arraydatasize] + 1;
		if (!eciNetDeviceCallback(0, arraydata, arraydatasize, neto))
		{
			neto->searchcomplete = true;
			searchinprogress = false;
			Cancel();
			return;
		}
		if (neto->abortsearch)
		{
			neto->searchcomplete = true;
			searchinprogress = false;
			Cancel();
			return;
		}
		if (neto->servicesearch)
		{
			if (servicediscoverer == NULL)
				servicediscoverer = CSymServiceDiscoverer::NewL(this);
			if (servicediscoverer != NULL)
			{
				servicebusy = true;
				servicediscoverer->basicarrsize = arraydatasize - 2;
				if (!servicediscoverer->DiscoverServices(static_cast<TBTSockAddr>(nameentry().iAddr).BTAddr()))
					neto->servicesearch = false;
			}
			else
				neto->servicesearch = false;
		}
		if (neto->abortsearch)
		{
			neto->searchcomplete = true;
			searchinprogress = false;
			Cancel();
			return;
		}
		if (!neto->servicesearch)
		{
			if (requestrestart)
			{
				searchinprogress = false;
				DiscoverDevices();
			}
			else
			{
				resolver.Next(nameentry, iStatus);
				SetActive();
			}
		}
	}
	else
	{
		if (!neto->servicesearch || !servicebusy)
		{
			if (requestrestart)
			{
				searchinprogress = false;
				DiscoverDevices();
				return;
			}
			searchinprogress = false;
			neto->searchcomplete = true;
			Cancel();
		}
		else
			enddevicequeue = true;
	}
}

//On cancellation of the active object
void CSymDeviceDiscoverer::DoCancel()
{
	servicebusy = false;
	searchinprogress = false;
	resolver.Cancel();
	resolver.Close();
	if (servicediscoverer)
		servicediscoverer->CancelProcess();
}

//Convert native bluetooth address to ascii
void CSymDeviceDiscoverer::BTAtoA(char *btstr, TBTSockAddr address)
{
	unsigned char nv;
	for (nv = 0; nv < 6; nv++)
	{
		btstr[nv * 3] = (unsigned char)((address.BTAddr()[nv] >> 4) & 15);
		btstr[nv * 3 + 1] = (unsigned char)(address.BTAddr()[nv] & 15);
	}
	for (nv = 0; nv < 17; nv++)
		if (btstr[nv] > 9)
			btstr[nv] = btstr[nv] - 10 + 'a';
		else
			btstr[nv] = btstr[nv] + '0';
	for (nv = 0; nv < 6; nv++)
		btstr[nv * 3 + 2] = ':';
	btstr[17] = 0;
}

//Construction phase 1
CSymDeviceDiscoverer *CSymDeviceDiscoverer::NewL(void *userdata)
{
	CSymDeviceDiscoverer *self = CSymDeviceDiscoverer::NewLC(userdata);
	CleanupStack::Pop(self);
	return(self);
}

//Construction phase 2
CSymDeviceDiscoverer *CSymDeviceDiscoverer::NewLC(void *userdata)
{
	CSymDeviceDiscoverer *self = new(ELeave)CSymDeviceDiscoverer(userdata);
	CleanupStack::PushL(self);
	self->ConstructL();
	return(self);
}

//CEdgeDeviceDiscoverer: constructor
CSymDeviceDiscoverer::CSymDeviceDiscoverer(void *userdata) : CActive(CActive::EPriorityStandard)
{
	neto = (EC_NETOBJ *)userdata;
	servicediscoverer = NULL;
	searchinprogress = false;
	requestrestart = false;
	CActiveScheduler::Add(this);
}

//CEdgeDeviceDiscoverer: destructor
CSymDeviceDiscoverer::~CSymDeviceDiscoverer()
{
	if (servicediscoverer != NULL)
		delete servicediscoverer;
	Cancel();
}

//Start discovering devices
bool CSymDeviceDiscoverer::DiscoverDevices(void)
{
	if (searchinprogress)
	{
		requestrestart = true;
		return(true);
	}
	requestrestart = false;
	TProtocolDesc pdesc;
	Cancel();
	if (neto->sockserv.FindProtocol(_L("BTLinkManager"), pdesc) != KErrNone)
		return(false);
	if (resolver.Open(neto->sockserv, pdesc.iAddrFamily, pdesc.iProtocol) != KErrNone)
		return(false);
	enddevicequeue = false;
	servicebusy = false;
	inquiriaddr.SetIAC(KGIAC);
	inquiriaddr.SetAction(KHostResInquiry | KHostResName | KHostResIgnoreCache);
	resolver.GetByAddress(inquiriaddr, nameentry, iStatus);
	searchinprogress = true;
	SetActive();
	return(true);
}

//Callback when the service searcher has completed the device
void CSymDeviceDiscoverer::ServiceComplete(bool abort)
{
	if (servicebusy)
	{
		servicebusy = false;
		if (requestrestart && !abort)
		{
			searchinprogress = false;
			DiscoverDevices();
			return;
		}
		if (abort || enddevicequeue)
		{
			enddevicequeue = true;
			neto->searchcomplete = true;
			searchinprogress = false;
			Cancel();
		}
		else
		{
			resolver.Next(nameentry, iStatus);
			SetActive();
		}
	}
}

///////////////////////////////////////////////////////////////////
// CSymBTAdvertiser implementation
///////////////////////////////////////////////////////////////////

//Protocol descriptor builder
void CSymBTAdvertiser::BuildProtocolDescriptionL(CSdpAttrValueDES *aProtocolDescriptor, TInt aPort)
{
	TBuf8<1> channel;
	channel.Append((TChar)aPort);
	aProtocolDescriptor->StartListL()->BuildDESL()->StartListL()->BuildUUIDL(KL2CAP)->EndListL()->BuildDESL()->StartListL()->BuildUUIDL(KRFCOMM)->BuildUintL(channel)->EndListL()->EndListL();
}

//Construction phase 1
CSymBTAdvertiser *CSymBTAdvertiser::NewL(void)
{
	CSymBTAdvertiser *self = CSymBTAdvertiser::NewLC();
	CleanupStack::Pop(self);
	return(self);
}

//Construction phase 2
CSymBTAdvertiser *CSymBTAdvertiser::NewLC(void)
{
	CSymBTAdvertiser *self = new(ELeave)CSymBTAdvertiser();
	CleanupStack::PushL(self);
	return(self);
}

//CSymBTAdvertiser: constructor
CSymBTAdvertiser::CSymBTAdvertiser(void)
{
	isconnected = false;
	rechandle = 0;
}

//CSymBTAdvertiser: destructor
CSymBTAdvertiser::~CSymBTAdvertiser(void)
{
	StopAdvertising();
}

//Start advertising service
bool CSymBTAdvertiser::StartAdvertising(char *servicename, unsigned char port)
{
	StopAdvertising();
	if (!isconnected)
	{
		if (iSdpSession.Connect() == KErrNone)
		{
			if (iSdpDatabase.Open(iSdpSession) != KErrNone)
			{
				iSdpSession.Close();
				return(false);
			}
			else
				isconnected = true;
		}
		else
			return(false);
	}
	CSdpAttrValueDES *UUIDlist = CSdpAttrValueDES::NewDESL(NULL);
	CleanupStack::PushL(UUIDlist);
	UUIDlist
		->StartListL()
			->BuildUUIDL(TUUID(TUint32(0x1101)))
			->BuildUUIDL(TUUID(TUint32(0x1002)))
		->EndListL();
	iSdpDatabase.CreateServiceRecordL(*UUIDlist, rechandle);
	CleanupStack::PopAndDestroy();
	CSdpAttrValueDES *vProtocolDescriptor = CSdpAttrValueDES::NewDESL(NULL);
	CleanupStack::PushL(vProtocolDescriptor);
	BuildProtocolDescriptionL(vProtocolDescriptor, port);
	iSdpDatabase.UpdateAttributeL(rechandle, KSdpAttrIdProtocolDescriptorList, *vProtocolDescriptor);
	CleanupStack::PopAndDestroy(vProtocolDescriptor);
	_LIT(KServiceDescription, "EDGELIB Bluetooth Service");
	TBuf8<256> snamedes;
	snamedes.Copy((const TUint8 *)servicename);
	iSdpDatabase.UpdateAttributeL(rechandle, KSdpAttrIdBasePrimaryLanguage + KSdpAttrIdOffsetServiceName, snamedes);
	iSdpDatabase.UpdateAttributeL(rechandle, KSdpAttrIdBasePrimaryLanguage + KSdpAttrIdOffsetServiceDescription, KServiceDescription);
	//** Update availability (useless?)
	//TUint state;
	//if (true) //true to set availability to true
	//	state = 0xFF;
	//else
	//	state = 0x00;
	//iSdpDatabase.UpdateAttributeL(iRecord, KSdpAttrIdServiceAvailability, state);
	//iSdpDatabase.UpdateAttributeL(iRecord, KSdpAttrIdServiceRecordState, ++iRecordState);
	return(true);
}

//Stop advertising
void CSymBTAdvertiser::StopAdvertising(void)
{
	if (rechandle != 0)
	{
		iSdpDatabase.DeleteRecordL(rechandle);
		rechandle = 0;
	}
	if (isconnected)
	{
		iSdpDatabase.Close();
		iSdpSession.Close();
		isconnected = false;
	}
}
