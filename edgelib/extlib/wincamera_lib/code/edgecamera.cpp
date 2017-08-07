#define EDGE_WINCAMLIB
#define _CRT_SECURE_NO_WARNINGS
#include "edge.h"
#include "edgecore.h"

// ATL and DirectShow fail with default compiler settings
// make sure built-in wchar_t is disabled during compilation
// and that the necessary include/lib directories are included
// VC does not do this by default

// there is a PPC2005 experimental config file which has this
// fixed, as well as a desktop version with the better settings

// Desktop is even nastier - While on PocketPC, the DirectShow
// base classes are provided for you, this is not the case on
// Desktop machines.
// Currently, the needed headers from BaseClasses are added
// to edge, and BaseClasses itself is linked into a separate 
// .lib
// The source for BaseClasses is part of the platform SDK
// in Samples/Multimedia/DirectShow/BaseClasses

#if defined(DEVICE_DESKTOP)
#pragma warning(disable:4995)
#include <tchar.h>
#include "streams.h"
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "msdmo.lib")
#pragma comment(lib, "winmm.lib")
#else
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "strmbase.lib")
#pragma comment(lib, "mmtimer")
#include <dshow.h>
#include <winbase.h>
#include <atlbase.h>
#endif

#include <vector>

static void Log(const char *);

#if 0
	#include <stdio.h>
	void Log(const char * arg)
	{
		FILE * flog = fopen("log.txt", "a");
		fputs(arg, flog);
		fclose(flog);
	}
#else
	void Log(const char * arg)
	{
	}
#endif


// Specified by MS documentation, not defined in standard headers
const GUID MEDIASUBTYPE_I420_local = {0x30323449, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71};

static char modelName[256];
static HWND messagewnd;
static char currotation;

typedef struct tFRAMEGRAB
{
	char * data;
	E_CAMERAFRAME edgeframe;
	IMediaSample * volatile container; // pointer is volatile, data is not
	volatile long timestamp;
	volatile bool queued;

} FRAMEGRAB;

///////////////////////////////////////////////////////////////////////////////
// TapFilter - DirectShow TAP class
///////////////////////////////////////////////////////////////////////////////
class TapFilter : public CBaseRenderer, public CPersistStream
{
    friend class ClassEdge;

	public:
		TapFilter(LPUNKNOWN, HRESULT *, ClassEdge *, ClassECamera *);
		virtual ~TapFilter();

		// IUnknown
		DECLARE_IUNKNOWN;

		// CBaseRenderer
		virtual HRESULT DoRenderSample(IMediaSample *);
		virtual HRESULT CheckMediaType(const CMediaType *);

		// CPersistStream
		#if defined(DEVICE_DESKTOP)
			STDMETHODIMP GetClassID(CLSID *);
		#else
			virtual HRESULT GetClassID(CLSID *);
		#endif

		// IFrameGrabber
		E_CAMERAFRAME * GetBuffer(long);
        void ReleaseBuffer(long);
		void GetDimensions(int *, int *);
		void SetIssues(int);


	protected:
		ClassEdge * framework;
		ClassECamera * frontend;
        HWND destination;

		int GetRGB(const CMediaType * ms) const;

		int iBpp, iWidth, iHeight;
		long iFrameNo;
		int iKnownIssues;

#define BUFFER_COUNT 3
		FRAMEGRAB buffers[BUFFER_COUNT];

		long lasttick;
		int framesec;
		volatile bool synchronize;
		bool getbuffersynced;

};


///////////////////////////////////////////////////////////////////////////////
// DirectShowCamera class
///////////////////////////////////////////////////////////////////////////////
class DirectShowCamera
{
	public:
		DirectShowCamera(ClassEdge *, ClassECamera *);
		virtual ~DirectShowCamera();

		// IFrameGrabber
		E_CAMERAFRAME * GetBuffer(long);
		void ReleaseBuffer(long);
		void GetDimensions(int *, int *);

		// CameraControl
		ERESULT init(int camera);
		ERESULT start(void);
		ERESULT stop(void);

		int GetCameraCount();
		int GetCameraResolutions();
		void GetSupportedResolution(int, int *, int *, int *, int *);
		ERESULT SetResolution(int, int);
		ERESULT SetResolution(int);
		int GetSelectedCamera();
		int GetSelectedResolution();
		int GetCurrentState();
		int GetCameraOrientation();

		const char * GetErrorMessage();
		
		static int FormatToEdge(REFGUID format);
		static int AccumulateIssue(int issues, int moreissues);

	protected:
		#if defined(DEVICE_WINMOBILE)
			std::vector<DEVMGR_DEVICE_INFORMATION> vCameras;
		#else
			std::vector<IMoniker *> vCameras;
		#endif
		std::vector<POINT> vDimensions;
		std::vector<int> vFormats;
		std::vector<int> vIssues;
		std::vector<int> vRates;
		std::vector<int> vIndices;
		void DetectCameras(void);

		const char * lasterror; // may be useful later

		int currentstate;
		int currentcam;
		int selectedresolution;

		TapFilter * framegrabber;
		IBaseFilter * capturefilter;
		IPersistPropertyBag * captureproperties;
		IGraphBuilder * filtergraph;
		ICaptureGraphBuilder2 * graphbuilder;
		IMediaControl * mediacontrol;
		IAMStreamConfig * streamconfig;

        ClassEdge * framework;
        ClassECamera * frontend;
};


///////////////////////////////////////////////////////////////////////////////
// CPropertyBag class
///////////////////////////////////////////////////////////////////////////////

struct VAR_LIST
{
    VARIANT var;
    VAR_LIST *pNext;
    BSTR pBSTRName;
};

class CPropertyBag : public IPropertyBag
{
    public:
        CPropertyBag();
        ~CPropertyBag(); 
         
        HRESULT STDMETHODCALLTYPE 
        Read( 
            LPCOLESTR pszPropName,  
            VARIANT *pVar,  
            IErrorLog *pErrorLog
            );
         
         
        HRESULT STDMETHODCALLTYPE 
        Write( 
            LPCOLESTR pszPropName,  
            VARIANT *pVar 
            );

        ULONG STDMETHODCALLTYPE AddRef();
        ULONG STDMETHODCALLTYPE Release();
        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);

    private:
         ULONG _refCount;
         VAR_LIST *pVar;
};

///////////////////////////////////////////////////////////////////////////////
// Devicelist
///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    char * devname;
    int frontcam;
    int rearcam;
    int issues;
} WMDEVINFO;

static const WMDEVINFO * DeviceLookup(const char *);


///////////////////////////////////////////////////////////////////////////////
// TapFilter implementation
///////////////////////////////////////////////////////////////////////////////

const CLSID tapguid = {0x04e76950,0xada4,0x11de,0x8a,0x39,0x08,0x00,0x20,0x0c,0x9a,0x66};

TapFilter::TapFilter(LPUNKNOWN punk, HRESULT *phr, ClassEdge * ce, ClassECamera * cam) : CBaseRenderer(tapguid, NAME("TapFilter"), punk, phr ), CPersistStream(punk, phr), framework(ce), frontend(cam)
{


	lasttick = GetTickCount();
	framesec = 0;

	destination = messagewnd;

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
        buffers[i].data = NULL;
    }
    
    iFrameNo = 0;
}

TapFilter::~TapFilter()
{
    for (int i = 0; i < BUFFER_COUNT; i++)
	{
        if (buffers[i].data) delete[] buffers[i].data;
    }
}

HRESULT TapFilter::GetClassID(CLSID * clsid)
{
	return CBaseFilter::GetClassID(clsid);
}

HRESULT TapFilter::CheckMediaType(const CMediaType * mt)
{
	int bits = GetRGB(mt);
	if (bits == 0) return E_FAIL;

	iBpp = bits;

	VIDEOINFOHEADER * vi = (VIDEOINFOHEADER *) mt->Format();

	iHeight = vi->bmiHeader.biHeight;
	iWidth = vi->bmiHeader.biWidth;

    for (int i = 0; i < BUFFER_COUNT; i++)
    {
        if (buffers[i].data) delete[] buffers[i].data;
	    buffers[i].data = new char[iHeight * iWidth * iBpp / 8];

	    buffers[i].edgeframe.data = (const unsigned char *) buffers[i].data;
        buffers[i].edgeframe.width = iWidth;
        buffers[i].edgeframe.height = iHeight;
        buffers[i].edgeframe.orientation = iKnownIssues & (ECO_XFLIP | ECO_YFLIP | ECO_SWAPAXIS);

        buffers[i].edgeframe.pitch = iWidth * iBpp / 8;
		buffers[i].edgeframe.format = DirectShowCamera::FormatToEdge(*mt->Subtype());

		buffers[i].queued = false;

        // correct flipped images
        if (iHeight > 0 && iBpp != 12) // RGB bitmaps are bottom-up
            buffers[i].edgeframe.orientation = DirectShowCamera::AccumulateIssue(buffers[i].edgeframe.orientation, ECO_YFLIP);
        else if (iBpp != 12)
            iHeight = -iHeight;

	}

	return S_OK;
}

HRESULT TapFilter::DoRenderSample(IMediaSample * pIn)
{

	const unsigned char * src = NULL;	
	pIn->GetPointer((BYTE **) &src);

	int i;
    for (i = 0; i < BUFFER_COUNT; i++)
    if (buffers[i].queued == false)
    {
        buffers[i].queued = true;
        break;
    }

   	if (i == BUFFER_COUNT)
   	{
        // buffer overrun
        iFrameNo++;
    	framesec++;
        return S_OK;
    }

    buffers[i].timestamp = iFrameNo;
	memcpy(buffers[i].data, src, (iWidth * iHeight * iBpp) >> 3);

    PostMessage(destination, WM_CAMERAFRAME, (WPARAM)frontend, (LPARAM)iFrameNo);

    iFrameNo++;
	framesec++;

	/*long curtick = GetTickCount();
	if (lasttick + 1000 < curtick)
	{
		//log->Log("Grabber FPS: ", false); log->Log(framesec);
		framesec = 0;
		lasttick += 1000;
		if (lasttick + 1000 < curtick) lasttick = curtick;
	}*/



	return S_OK;
}

int TapFilter::GetRGB(const CMediaType * ms) const
{
	if (!IsEqualGUID(*ms->Type(), MEDIATYPE_Video)) return 0;
	if (ms->FormatLength() < sizeof(VIDEOINFOHEADER)) return 0;
	VIDEOINFOHEADER * vh = (VIDEOINFOHEADER*) ms->Format();

	if (IsEqualGUID(*ms->Subtype(), MEDIASUBTYPE_RGB24) && vh->bmiHeader.biBitCount == 24) return 24;
	if (IsEqualGUID(*ms->Subtype(), MEDIASUBTYPE_RGB32) && vh->bmiHeader.biBitCount == 32) return 32;
	if (IsEqualGUID(*ms->Subtype(), MEDIASUBTYPE_RGB565) && vh->bmiHeader.biBitCount == 16) return 16;
	if (IsEqualGUID(*ms->Subtype(), MEDIASUBTYPE_YV12) && vh->bmiHeader.biBitCount == 12) return 12;
	if (IsEqualGUID(*ms->Subtype(), MEDIASUBTYPE_I420_local) && vh->bmiHeader.biBitCount == 12) return 12;

	return 0;
}

void TapFilter::ReleaseBuffer(long frame)
{
    for (int i = 0; i < 3; i++)
    {
        if (buffers[i].timestamp == frame)
            buffers[i].queued = false;
    }
}

E_CAMERAFRAME * TapFilter::GetBuffer(long frame)
{
    for (int i = 0; i < 3; i++)
    {
        if (buffers[i].timestamp == frame)
            return &(buffers[i].edgeframe);
    }
    return NULL;
}

void TapFilter::GetDimensions(int * x, int * y)
{
	if (x) *x = iWidth;
	if (y) *y = iHeight;
}

void TapFilter::SetIssues(int issue)
{
    iKnownIssues = issue;
    //ClassEdge::Log("Accumulated camera issues registered: ", false); ClassEdge::Log(issue);
}


///////////////////////////////////////////////////////////////////////////////
// DirectShowCamera implementation
///////////////////////////////////////////////////////////////////////////////
static const char * dsc_noerror = "";

DirectShowCamera::DirectShowCamera(ClassEdge * ce, ClassECamera * cam)
    : framework(ce), frontend(cam)
{
	framegrabber = NULL;
	capturefilter = NULL;
	captureproperties = NULL;
	filtergraph = NULL;
	graphbuilder = NULL;
	mediacontrol = NULL;
	streamconfig = NULL;

	lasterror = dsc_noerror;

	// find camera's first
	vCameras.clear();
	DetectCameras();
	
	currentcam = -1;
	currentstate = ECS_INVALID;

	framework->RegisterCam(frontend);	
}

DirectShowCamera::~DirectShowCamera()
{

    stop();

	if (streamconfig) streamconfig->Release();
	if (mediacontrol) mediacontrol->Release();
	if (graphbuilder) graphbuilder->Release();
	if (filtergraph) filtergraph->Release();
	if (framegrabber) framegrabber->Release();
	if (captureproperties) captureproperties->Release();
	if (capturefilter) capturefilter->Release();
    framework->UnregisterCam(frontend);

	#if defined(DEVICE_DESKTOP)
		for (unsigned int i = 0; i < vCameras.size(); i++)
		{
			vCameras[i]->Release();
			vCameras[i] = NULL;
		}
	#endif
	vCameras.clear();

}

#if defined(DEVICE_WINMOBILE)
void DirectShowCamera::DetectCameras(void)
{
	const GUID cameraguid = { 0xCB998A05, 0x122C, 0x4166, 0x84, 0x6A, 0x93, 0x3E, 0x4D, 0x7E, 0x3C, 0x86};
	DEVMGR_DEVICE_INFORMATION thisdev;
	
	thisdev.dwSize = sizeof(thisdev);

	vCameras.clear();

	HANDLE hrSearchHandle = FindFirstDevice( DeviceSearchByGuid, &cameraguid, &thisdev);
	if (hrSearchHandle != INVALID_HANDLE_VALUE)
	{
		vCameras.push_back(thisdev);		

		while(FindNextDevice(hrSearchHandle, &thisdev))
		{
			vCameras.push_back(thisdev);
		}
		FindClose(hrSearchHandle);
	}
}
#else
void DirectShowCamera::DetectCameras(void)
{
	HRESULT hr;

	for (unsigned int i = 0; i < vCameras.size(); i++)
	{
		vCameras[i]->Release();
		vCameras[i] = NULL;
	}
	vCameras.clear();

	hr = CoInitialize(NULL);
	if (FAILED(hr)) return;

    ICreateDevEnum *pCreateDevEnum=0;
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if(hr != NOERROR)
        return;    

    IEnumMoniker *pEnumMoniker=0;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumMoniker, 0);
    if(hr != NOERROR)
    {
		// no devices
		pCreateDevEnum->Release();
        return;
    }

	// enumerate all devices
    pEnumMoniker->Reset();
    ULONG fetchcount;
    IMoniker *pMoniker;

	while(1)
	{
		hr = pEnumMoniker->Next(1, &pMoniker, &fetchcount);
		if (hr == S_OK)				
			vCameras.push_back(pMoniker);				
		else break;		
	}
	pEnumMoniker->Release();
	pCreateDevEnum->Release();

}
#endif

E_CAMERAFRAME * DirectShowCamera::GetBuffer(long frame)
{
	if (framegrabber) return framegrabber->GetBuffer(frame);
	return NULL;
}

void DirectShowCamera::ReleaseBuffer(long frame)
{
	if (framegrabber) framegrabber->ReleaseBuffer(frame);
}

void DirectShowCamera::GetDimensions(int * pX, int * pY)
{
	*pX = 0;
	*pY = 0;
	if (framegrabber) framegrabber->GetDimensions(pX, pY);
}

ERESULT DirectShowCamera::init(int camera)
{

	// wrapper
#define INIT_FAIL(m) { this->lasterror = m; framework->OnCameraChange(frontend, E_ERROR); Log(m); return E_ERROR; }

	HRESULT hResult;

    int issues = 0;
    
    const WMDEVINFO * listedinfo = DeviceLookup(modelName);
    if (listedinfo != NULL)
    {
        issues = listedinfo->issues;
    }

    //ClassEdge::Log("Known issues: ", false); ClassEdge::Log(issues);

	if (vCameras.size() == 0)
		INIT_FAIL("No camera's detected");

	if (camera == ELOC_REAR && listedinfo)
	    camera = listedinfo->rearcam;
	if (camera == ELOC_FRONT && listedinfo)
	    camera = listedinfo->frontcam;
	if (camera < 0)
        camera = 0;

	if (camera >= (int)vCameras.size())
		INIT_FAIL("Invalid camera number");

    currentcam = -1;
    selectedresolution = 0;
    currentstate = ECS_INVALID;

    stop();
    framework->UnregisterCam(frontend);
	if (streamconfig) streamconfig->Release();
	if (mediacontrol) mediacontrol->Release();
	if (graphbuilder) graphbuilder->Release();
	if (filtergraph) filtergraph->Release();
	if (framegrabber) framegrabber->Release();
	if (captureproperties) captureproperties->Release();
	if (capturefilter) capturefilter->Release();
	streamconfig = NULL;
	mediacontrol = NULL;
	graphbuilder = NULL;
	filtergraph = NULL;
	framegrabber = NULL;
	captureproperties = NULL;
	capturefilter = NULL;
		
	vDimensions.clear();
	vIndices.clear();
	vFormats.clear();
	vIssues.clear();
	vRates.clear();
	
	DetectCameras();

    framework->RegisterCam(frontend);

	// Initialize COM (required for directshow)
	hResult = CoInitialize(NULL);
	if ((hResult != S_OK) && (hResult != S_FALSE))
		INIT_FAIL("CoInitialize failed");

	#if defined (DEVICE_WINMOBILE)
		// create capture device
		hResult = CoCreateInstance(CLSID_VideoCapture, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&capturefilter);
		if (!SUCCEEDED(hResult))
			INIT_FAIL("Couldn't create capture device");

		// query capture device
		hResult = capturefilter->QueryInterface( &captureproperties );
		if (!SUCCEEDED(hResult))
			INIT_FAIL("Couldn't access capture device");

		// Set camera device
		CComVariant varString;
		CPropertyBag tempbag;
		varString = vCameras[camera].szLegacyName;
		tempbag.Write(_T("VCapName"), &varString);
		hResult = captureproperties->Load(&tempbag, NULL);
		captureproperties->Release();
		captureproperties = NULL;		
		if (!SUCCEEDED(hResult))
			INIT_FAIL("Couldn't select camera");
	#else
		// create capture device
		hResult = vCameras[camera]->BindToObject(0, 0, IID_IBaseFilter, (void**)&capturefilter);
		if (!SUCCEEDED(hResult))
			INIT_FAIL("Couldn't create capture device");
		
	#endif

	// create destination
	framegrabber = new TapFilter(NULL, &hResult, framework, frontend);
	if (!SUCCEEDED(hResult))
		INIT_FAIL("Couldn't create destination filter");
	
	#if defined(DEVICE_DESKTOP)
		framegrabber->AddRef();
	#endif

	unsigned char difforient = currotation;
	/*
	ClassEdge::Log("Relative Orientation: ", false); ClassEdge::Log(difforient);
	ClassEdge::Log("Absolute Orientation: ", false); ClassEdge::Log(framework->ecd.dsp->rotation);
	ClassEdge::Log("System Orientation: ", false); ClassEdge::Log(framework->ecd.dsp->GetSystemRotation());
	 */
	if (difforient == DOR_90DEG)
	   issues = AccumulateIssue(ECO_YFLIP | ECO_SWAPAXIS, issues);
	else if (difforient == DOR_180DEG)
	   issues = AccumulateIssue(ECO_XFLIP | ECO_YFLIP, issues);
    else if (difforient == DOR_270DEG)
	   issues = AccumulateIssue(ECO_XFLIP | ECO_SWAPAXIS, issues);

    framegrabber->SetIssues(issues);


	hResult=CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder,(void**)&filtergraph);
	if (!SUCCEEDED(hResult))
		INIT_FAIL("Couldn't create DirectShow filter graph");
	
	#ifdef DEVICE_DESKTOP
		hResult=CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2,(void**)&graphbuilder);
	#else
		hResult=CoCreateInstance(CLSID_CaptureGraphBuilder, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2,(void**)&graphbuilder);
	#endif
	if (!SUCCEEDED(hResult))
		INIT_FAIL("Couldn't create graph builder");

	hResult = graphbuilder->SetFiltergraph(filtergraph);
	if (!SUCCEEDED(hResult))
		INIT_FAIL("Couldn't attach graph builder");

	hResult = filtergraph->AddFilter(capturefilter, L"Camera");
	if (!SUCCEEDED(hResult))
		INIT_FAIL("Couldn't attach capture filter");

	hResult = filtergraph->AddFilter(framegrabber, L"Tap");
	if (!SUCCEEDED(hResult))
		INIT_FAIL("Couldn't attach output filter");
 
	hResult = graphbuilder->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, capturefilter, IID_IAMStreamConfig, (void**) &streamconfig);
	if (!SUCCEEDED(hResult))
		INIT_FAIL("Couldn't get stream configuration control");

	int sccount, scsize;
	hResult = streamconfig->GetNumberOfCapabilities(&sccount, &scsize);
	if (!SUCCEEDED(hResult))
		INIT_FAIL("Couldn't retrieve camera properties");
	if (scsize != sizeof(VIDEO_STREAM_CONFIG_CAPS))
		INIT_FAIL("Requested camera is not a streaming device");

	for (int i = 0; i < sccount; i++)
	{
		VIDEO_STREAM_CONFIG_CAPS vscc;
		AM_MEDIA_TYPE * mt;

		hResult = streamconfig->GetStreamCaps(i, &mt, (BYTE *) &vscc);

		if (SUCCEEDED(hResult))
		{
			if (IsEqualGUID(mt->formattype, FORMAT_VideoInfo))
			//if (mt->cbFormat >= sizeof(VIDEOINFOHEADER))
			{
				VIDEOINFOHEADER * vh = (VIDEOINFOHEADER*) mt->pbFormat;
				POINT p;
				p.x = vh->bmiHeader.biWidth;
				p.y = -vh->bmiHeader.biHeight; // bitmap defined as bottom-up
				int issue = 0;
				if (p.y < 0)
				{
                    issue ^= ECO_YFLIP;        // set the Y flip bit
                    p.y = -p.y;
                }
                
                /* this is the predefined issue...
				if (currotation != DOR_NONE)
                {
                    issue ^= (ECO_SWAPAXIS | ECO_XFLIP);
                }*/

				vDimensions.push_back(p);
				vIndices.push_back(i);
				vFormats.push_back(FormatToEdge(mt->subtype));
				vIssues.push_back(issue);
				vRates.push_back((int)((65536ull * 1000ull * 1000ull * 10ull) / vh->AvgTimePerFrame)); // 100ns->us->ms
				
				/*
				ClassEdge::Log("   SampleSize: ", false); ClassEdge::Log(mt->lSampleSize);
            	ClassEdge::Log("   FormatSize: ", false); ClassEdge::Log(mt->cbFormat);
            	ClassEdge::Log("   Temporal: ", false); ClassEdge::Log(mt->bTemporalCompression);
            	ClassEdge::Log("   FixedSize: ", false); ClassEdge::Log(mt->bFixedSizeSamples);
            	ClassEdge::Log("   Bit count: ", false); ClassEdge::Log(vh->bmiHeader.biBitCount);
            	ClassEdge::Log("   biClrImportant: ", false); ClassEdge::Log(vh->bmiHeader.biClrImportant);
            	ClassEdge::Log("   biClrUsed: ", false); ClassEdge::Log(vh->bmiHeader.biClrUsed);
            	ClassEdge::Log("   biCompression: ", false); ClassEdge::Log(vh->bmiHeader.biCompression);
            	ClassEdge::Log("   biHeight: ", false); ClassEdge::Log(vh->bmiHeader.biHeight);
            	ClassEdge::Log("   biPlanes: ", false); ClassEdge::Log(vh->bmiHeader.biPlanes);
            	ClassEdge::Log("   biSize: ", false); ClassEdge::Log(vh->bmiHeader.biSize);
            	ClassEdge::Log("   biSizeImage: ", false); ClassEdge::Log(vh->bmiHeader.biSizeImage);
            	ClassEdge::Log("   biWidth: ", false); ClassEdge::Log(vh->bmiHeader.biWidth);
            	ClassEdge::Log("   biXPelsPerMeter: ", false); ClassEdge::Log(vh->bmiHeader.biXPelsPerMeter);
            	ClassEdge::Log("   biYPelsPerMeter: ", false); ClassEdge::Log(vh->bmiHeader.biYPelsPerMeter);
            	*/
			}
			DeleteMediaType(mt);
		}
	}

	//if (sccount == 0)
	if (vDimensions.size() == 0)
		INIT_FAIL("No resolutions reported");

	hResult = graphbuilder->RenderStream( &PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, capturefilter, NULL, framegrabber);
	if (!SUCCEEDED(hResult))
		INIT_FAIL("Couldn't configure stream");

	hResult = filtergraph->QueryInterface(&mediacontrol);
	if (!SUCCEEDED(hResult))
		INIT_FAIL("Couldn't retrieve media control");

    currentcam = camera;
    currentstate = ECS_READY;

    framework->OnCameraChange(frontend, E_OK);

	return E_OK;
}

ERESULT DirectShowCamera::start()
{
	if (mediacontrol == NULL) return E_ERROR;	
	HRESULT hResult = mediacontrol->Run();
	if (SUCCEEDED(hResult))
    {
        currentstate = ECS_RECORDING;
        return E_OK;
    }
	return E_ERROR;
}

ERESULT DirectShowCamera::stop()
{
	if (mediacontrol == NULL) return E_ERROR;	
	HRESULT hResult = mediacontrol->Stop();
	if (SUCCEEDED(hResult)) 
    {
        currentstate = ECS_READY;
        return E_OK;
    }
	return E_ERROR;
}

const char * DirectShowCamera::GetErrorMessage()
{
	return this->lasterror;
}

int DirectShowCamera::GetCameraCount()
{
	return (int)vCameras.size();
}
int DirectShowCamera::GetCameraResolutions()
{
	return (int)vDimensions.size();
}
void DirectShowCamera::GetSupportedResolution(int index, int * pX, int * pY, int * pF, int * pO)
{
	if (index >= (int)vDimensions.size()) return;
	if (index < 0) return;
	if (pX) *pX = vDimensions[index].x;
	if (pY) *pY = vDimensions[index].y;
	if (pF) *pF = vFormats[index];
	if (pO) *pO = vRates[index];
	//if (pO) *pO = vIssues[index];
}

ERESULT DirectShowCamera::SetResolution(int x, int y)
{
	lasterror = dsc_noerror;
	if (mediacontrol == NULL) return E_ERROR;

	for (int i = 0; i < (int)vDimensions.size(); i++)
	{
		if(vDimensions[i].x == x && vDimensions[i].y == y)
		{
			VIDEO_STREAM_CONFIG_CAPS vscc;
			AM_MEDIA_TYPE * mt;
			HRESULT hResult;
			hResult = streamconfig->GetStreamCaps(vIndices[i], &mt, (BYTE*) &vscc);
			if (!SUCCEEDED(hResult))
			{
				this->lasterror = "Couldn't retrieve format descriptor";
				DeleteMediaType(mt);
				continue;
			}

			int laststate = currentstate;
			if (laststate == ECS_RECORDING) stop();
			hResult = streamconfig->SetFormat(mt);
			if (laststate == ECS_RECORDING) start();

			if (!SUCCEEDED(hResult))
			{
				DeleteMediaType(mt);
				this->lasterror = "Couldn't set format";
				continue;
			}
			selectedresolution = i;
			framework->OnCameraChange(frontend, E_OK);
			return true;
		}
	}
	
	framework->OnCameraChange(frontend, E_ERROR);

	if (lasterror == dsc_noerror)
		this->lasterror = "Resolution unavailable";

	return false;
}


ERESULT DirectShowCamera::SetResolution(int index)
{

    if (index < 0) return E_INVALIDPARAM;
    if (index >= (int)vDimensions.size()) return E_INVALIDPARAM;
	if (mediacontrol == NULL) return E_ERROR;
    
    stop();

	VIDEO_STREAM_CONFIG_CAPS vscc;
	AM_MEDIA_TYPE * mt;
	HRESULT hResult;
	hResult = streamconfig->GetStreamCaps(vIndices[index], &mt, (BYTE*) &vscc);
	if (!SUCCEEDED(hResult))
	{
		this->lasterror = "Couldn't retrieve format descriptor";
		DeleteMediaType(mt);
		framework->OnCameraChange(frontend, E_ERROR);
		return E_ERROR;
	}
    hResult = streamconfig->SetFormat(mt);
    DeleteMediaType(mt);
	if (!SUCCEEDED(hResult))
	{
		this->lasterror = "Couldn't set format";
		framework->OnCameraChange(frontend, E_ERROR);
		return E_ERROR;
	}
    selectedresolution = index;
	framework->OnCameraChange(frontend, E_OK);
	return E_OK;
}


int DirectShowCamera::FormatToEdge(REFGUID format)
{
    if (IsEqualGUID(format, MEDIASUBTYPE_RGB24)) return EDSP_TRUE24;
    if (IsEqualGUID(format, MEDIASUBTYPE_RGB32)) return EDSP_TRUE32;
    if (IsEqualGUID(format, MEDIASUBTYPE_RGB565)) return EDSP_TRUE16;
    if (IsEqualGUID(format, MEDIASUBTYPE_YV12)) return EDSP_YVU12;
	if (IsEqualGUID(format, MEDIASUBTYPE_I420_local)) return EDSP_YUV12;
	//ClassEdge::Log("unknown camera format: ", false);
	wchar_t guid[128];
	StringFromGUID2(format, guid, 127);
	//ClassEdge::Log(guid);
    return EDSP_UNKNOWN;
}

int DirectShowCamera::AccumulateIssue(int issues, int moreissues)
{
    if (issues & ECO_SWAPAXIS)
    {
        moreissues = moreissues & 0xfffc | ((moreissues & 2) >> 1) | ((moreissues & 1) << 1);
    }
    return ((issues | moreissues) & 0xfff8) | ((issues ^ moreissues) & 0x7);
}

int DirectShowCamera::GetSelectedCamera()
{
    return currentcam;
}
int DirectShowCamera::GetCurrentState()
{
    return currentstate;
}

int DirectShowCamera::GetCameraOrientation()
{
    if (currentcam < 0) return ELOC_UNKNOWN;
    
    const WMDEVINFO * listedinfo = DeviceLookup(modelName);

    if (listedinfo == NULL)
        return ELOC_UNKNOWN;

    if (listedinfo->frontcam == currentcam) return ELOC_FRONT;
    if (listedinfo->rearcam == currentcam) return ELOC_REAR;

    return ELOC_UNKNOWN;
}

int DirectShowCamera::GetSelectedResolution()
{
    return selectedresolution;
}


///////////////////////////////////////////////////////////////////////////////
// CPropertyBag implementation
///////////////////////////////////////////////////////////////////////////////

CPropertyBag::CPropertyBag() : _refCount(1), pVar(0)
{
}

CPropertyBag::~CPropertyBag()
{
    VAR_LIST *pTemp = pVar;
    HRESULT hr = S_OK;

    while(pTemp)
    {
        VAR_LIST *pDel = pTemp;
        VariantClear(&pTemp->var);
        SysFreeString(pTemp->pBSTRName);
        pTemp = pTemp->pNext;
        delete pDel;
    }

}

HRESULT STDMETHODCALLTYPE
CPropertyBag::Read(LPCOLESTR pszPropName,
                       VARIANT *_pVar,
                       IErrorLog *pErrorLog)
{
    VAR_LIST *pTemp = pVar;
    HRESULT hr = S_OK;

    while(pTemp)
    {
        if(0 == wcscmp(pszPropName, pTemp->pBSTRName))
        {
            hr = VariantCopy(_pVar, &pTemp->var);
            break;   
        }   
        pTemp = pTemp->pNext;   
    }   
    return hr;   
}   

   
HRESULT STDMETHODCALLTYPE   
CPropertyBag::Write(LPCOLESTR pszPropName,    
                            VARIANT *_pVar)   
{   
    HRESULT hr = S_OK;   
    VAR_LIST *pTemp = new VAR_LIST();   
    ASSERT(pTemp);
   
    if( !pTemp )   
    {   
        return E_OUTOFMEMORY;
    }   
   
    VariantInit(&pTemp->var);   
    pTemp->pBSTRName = SysAllocString(pszPropName);   
    pTemp->pNext = pVar;   
    pVar = pTemp;   
    return VariantCopy(&pTemp->var, _pVar);   
}   
   
ULONG STDMETHODCALLTYPE    
CPropertyBag::AddRef()    
{   
    return InterlockedIncrement((LONG *)&_refCount);   
}   
   
ULONG STDMETHODCALLTYPE    
CPropertyBag::Release()    
{   
    ASSERT(_refCount != 0xFFFFFFFF);   
    ULONG ret = InterlockedDecrement((LONG *)&_refCount);       
    if(!ret)
        delete this;    
    return ret;   
}   
   
HRESULT STDMETHODCALLTYPE    
CPropertyBag::QueryInterface(REFIID riid, void** ppv)    
{
    if(!ppv)    
        return E_POINTER;   
    if(riid == IID_IPropertyBag)    
        *ppv = static_cast<IPropertyBag*>(this);       
    else
        return *ppv = 0, E_NOINTERFACE;   
       
    return AddRef(), S_OK;     
}

/////////////////////////////////////////////////////////////////////
// C functions
/////////////////////////////////////////////////////////////////////

ECOREOBJ ecCamCreate(ECOREOBJ framework, ECOREOBJ frontend)
{
    DirectShowCamera * object = new DirectShowCamera((ClassEdge *) framework, (ClassECamera *) frontend);
    return object;
}

void ecCamDelete(ECOREOBJ camera)
{
    delete ((DirectShowCamera *)camera);
}

#if defined(DEVICE_WINMOBILE)
int ecCamDeviceCount(void)
{
    int count = 0;

	const GUID cameraguid = { 0xCB998A05, 0x122C, 0x4166, 0x84, 0x6A, 0x93, 0x3E, 0x4D, 0x7E, 0x3C, 0x86};
	DEVMGR_DEVICE_INFORMATION thisdev;

	thisdev.dwSize = sizeof(thisdev);

	HANDLE hrSearchHandle = FindFirstDevice( DeviceSearchByGuid, &cameraguid, &thisdev);
	if (hrSearchHandle != INVALID_HANDLE_VALUE)
	{
		count++;

		while(FindNextDevice(hrSearchHandle, &thisdev))
		{
			count++;
		}
		FindClose(hrSearchHandle);
	}

	return count;
}
#else
int ecCamDeviceCount(void)
{
	int count = 0;
	HRESULT hr;

	hr = CoInitialize(NULL);
	if (FAILED(hr)) return 0;

	// create enumeration object
    ICreateDevEnum *pCreateDevEnum=0;
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if(hr != NOERROR)
        return 0;    

    IEnumMoniker *pEnumMoniker=0;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumMoniker, 0);
    if(hr != NOERROR)
    {
		// no devices
		pCreateDevEnum->Release();
        return 0;
    }

	// enumerate all devices
    pEnumMoniker->Reset();
    ULONG fetchcount;
    IMoniker *pMoniker;

	while(1)
	{
		hr = pEnumMoniker->Next(1, &pMoniker, &fetchcount);		
		if (hr == S_OK)
		{
			count++;
			pMoniker->Release();
		}
		else break;
	}
	pEnumMoniker->Release();
	pCreateDevEnum->Release();

	return count;
}
#endif

unsigned long ecCamRequestCamera(ECOREOBJ camera, int index)
{
	if (!camera) return E_ERROR;
    return ((DirectShowCamera *)camera)->init(index);
}

unsigned long ecCamStartCapture(ECOREOBJ camera)
{
    return ((DirectShowCamera *)camera)->start();
}

unsigned long ecCamStopCapture(ECOREOBJ camera)
{
    return ((DirectShowCamera *)camera)->stop();
}

int ecCamGetResolutions(ECOREOBJ camera)
{
    return ((DirectShowCamera *)camera)->GetCameraResolutions();
}

void ecCamGetResolution(ECOREOBJ camera, int index, ECOREOBJ modein)
{
	E_CAMERAMODE * mode = (E_CAMERAMODE *) modein;
    return ((DirectShowCamera *)camera)->GetSupportedResolution(index, (int *) &(mode->x), (int *) &(mode->y), (int *) &(mode->format), (int *) &(mode->expectedfps));
}

unsigned long ecCamSetResolution(ECOREOBJ camera, int index)
{
    return ((DirectShowCamera *)camera)->SetResolution(index);
}

ECOREOBJ ecCamAsyncFrameRetrieve(ECOREOBJ camera, long returnedframe)
{
    return ((DirectShowCamera *)camera)->GetBuffer(returnedframe);
}

void ecCamAsyncFrameComplete(ECOREOBJ camera, long returnedframe)
{
    ((DirectShowCamera *)camera)->ReleaseBuffer(returnedframe);
}

int ecCamGetOrientation(ECOREOBJ camera)
{
    return ((DirectShowCamera *)camera)->GetCameraOrientation();;
}

int ecCamGetState(ECOREOBJ camera)
{
    return ((DirectShowCamera *)camera)->GetCurrentState();
}

int ecCamGetCurrentResolution(ECOREOBJ camera)
{
    return ((DirectShowCamera *)camera)->GetSelectedResolution();
}

int ecCamGetIndex(ECOREOBJ camera)
{
    return ((DirectShowCamera *)camera)->GetSelectedCamera();
}

void ecwCamInit(ECOREOBJ destwnd, char * modelname)
{
	messagewnd = (HWND)destwnd;
	strncpy(modelName, modelname, 255);
	modelName[255] = 0;
}

void ecwCamOrientation(char orientation)
{
	currotation = orientation;
	// = (4 + framework->ecd.dsp->rotation - framework->ecd.dsp->sysorientation) % 4;
}

/////////////////////////////////////////////////////////////////////
// Devicelist
/////////////////////////////////////////////////////////////////////

#define devicelist_count 5
static const WMDEVINFO devices[devicelist_count] =
    {
        //  ----- Name ------           F#  R#  -- Fixable <- Issues -> Unfixable --
		{"Desktop PC",                  -1, -1, 0},			    // Random Desktop - always assume separate camera
        {"HTC Touch",                   -1,  0, 0},
        {"HTC Touch Diamond P3700",     -1,  0, 0},             // no access to front camera
        {"O2 Xda Flame",                 0, -1, 0},             // no access to rear camera, corrupt data returned
        {"PU10",                        -1,  0, ECO_XFLIP }     // no access to front camera, has unidentified bug: will not restart after resolution was changed
    };

static const WMDEVINFO * DeviceLookup(const char * devname)
{
    int p = (devicelist_count >> 1);
    int min = 0;
    int max = devicelist_count - 1;

    bool done = false;

    while (!done)
    {        
        int order = strcmp(devname, devices[p].devname);
        if (order == 0)
            return devices+p;
        else if (order < 0)
        {
            max = p-1;
            p = (min+max) >> 1;
        }
        else
        {
            min = p+1;
            p = (min+max) >> 1;
        }     
        if (min > max) done = true;
    }

	//ClassEdge::Log("Known camera issue list: ", false);
	//ClassEdge::Log(devname, false);
    //ClassEdge::Log(" not found. Camera data may be corrupt or inaccurate.");
    return NULL;
}