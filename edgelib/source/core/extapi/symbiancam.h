// symbiancam.h
// Symbian camera class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include <ecam.h>

// resolution enumerator structure
typedef struct
{
    E_CAMERAMODE modeinfo;
    int formatindex;
    int resolutionindex;
    int framerateindex;
} E_SYMCAMERAMODE;

// fixme: MCameraObserver2, Autofocus callbacks, 5th.ed features?
class SymbianCameraInterface : public MCameraObserver
{

    public:

        SymbianCameraInterface(ClassEdge *, ClassECamera *);
        virtual ~SymbianCameraInterface();

        int GetCameraCount();
        ERESULT RequestCamera(int);
        ERESULT RequestStart();
        ERESULT RequestStop();

        int GetModeCount();
        void GetModeInformation(int, E_CAMERAMODE &);
        ERESULT RequestResolution(int);
        ERESULT RequestResolution(E_CAMERAMODE &);

        int GetOrientation(void);
        int GetIndex(void);
        int GetStatus(void);
        int GetSelectedResolution(void);

        // MCameraObserver
        virtual void ReserveComplete(TInt aError);
        virtual void PowerOnComplete(TInt aError);
        virtual void ViewFinderFrameReady(CFbsBitmap &aFrame);
        virtual void ImageReady(CFbsBitmap *aBitmap, HBufC8 *aData, TInt aError);
        virtual void FrameBufferReady(MFrameBuffer *aFrameBuffer, TInt aError);

    private:
        E_SYMCAMERAMODE * modedefinitions;
        int supportedmodes;
        int currentmode;
        int statusflags;
        int cameraindex; 
        int orientationflags;

        ClassEdge * parent;
        CCamera * camera;
        ClassECamera * frontend;

};

/////////////////////////////////////////////////////////////////////
// C functions
/////////////////////////////////////////////////////////////////////

ECOREOBJ ecCamCreate(ECOREOBJ framework, ECOREOBJ frontend)
{
    SymbianCameraInterface * object = new SymbianCameraInterface((ClassEdge*)framework, (ClassECamera *)frontend);
    return object;
}

void ecCamDelete(ECOREOBJ camera)
{
    delete ((SymbianCameraInterface *)camera);
}

int ecCamDeviceCount(void)
{
    return CCamera::CamerasAvailable();
}

unsigned long ecCamRequestCamera(ECOREOBJ camera, int index)
{
    return ((SymbianCameraInterface *)camera)->RequestCamera(index);
}

unsigned long ecCamStartCapture(ECOREOBJ camera)
{
    return ((SymbianCameraInterface *)camera)->RequestStart();
}

unsigned long ecCamStopCapture(ECOREOBJ camera)
{
    return ((SymbianCameraInterface *)camera)->RequestStop();
}

int ecCamGetResolutions(ECOREOBJ camera)
{
    return ((SymbianCameraInterface *)camera)->GetModeCount();
}

void ecCamGetResolution(ECOREOBJ camera, int index, ECOREOBJ mode)
{
	E_CAMERAMODE * modein = (E_CAMERAMODE *) mode;
    return ((SymbianCameraInterface *)camera)->GetModeInformation(index, *modein);
}

unsigned long ecCamSetResolution(ECOREOBJ camera, int index)
{
    return ((SymbianCameraInterface *)camera)->RequestResolution(index);
}

int ecCamGetOrientation(ECOREOBJ camera)
{
    return ((SymbianCameraInterface *)camera)->GetOrientation();
}

int ecCamGetState(ECOREOBJ camera)
{
    return ((SymbianCameraInterface *)camera)->GetStatus();
}

int ecCamGetCurrentResolution(ECOREOBJ camera)
{
    return ((SymbianCameraInterface *)camera)->GetSelectedResolution();
}

int ecCamGetIndex(ECOREOBJ camera)
{
    return ((SymbianCameraInterface *)camera)->GetIndex();
}


/////////////////////////////////////////////////////////////////////
// Symbian helpers
/////////////////////////////////////////////////////////////////////

static int FormatToEdge(TInt32 format)
{
    switch(format)
    {
//          case CCamera::EFormatMonochrome:
//              return "I8";
//          case CCamera::EFormat16bitRGB444:
//              return "R4G4B4X4";
        case CCamera::EFormat16BitRGB565:
            return EDSP_TRUE16;                     //
        case CCamera::EFormat32BitRGB888:
            return EDSP_TRUE32;                     // or TRUE32_INV? untested
        case CCamera::EFormatJpeg:
            return EDSP_JPEG;
        case CCamera::EFormatExif:
            return EDSP_EXIF;
//          case CCamera::EFormatFbsBitmapColor4K:
//              return "EColor4K";
//          case CCamera::EFormatFbsBitmapColor64K:
//              return "EColor64K";
//          case CCamera::EFormatFbsBitmapColor16M:
//              return "EColor16M";
//          case CCamera::EFormatUserDefined:
//              return "User";
        case CCamera::EFormatYUV420Interleaved:
            return EDSP_YUV420;
        case CCamera::EFormatYUV420Planar:
            return EDSP_YUV12;
        case CCamera::EFormatYUV422:
            return EDSP_UYVY;
        case CCamera::EFormatYUV422Reversed:
            return EDSP_YVYU;
        case CCamera::EFormatYUV444:
            return EDSP_YUV444;
        case CCamera::EFormatYUV420SemiPlanar:
            return EDSP_YUV444s;
//          case CCamera::EFormatFbsBitmapColor16MU:
//              return "EColor16MU";
        default:
            return -1;
    }
}

/////////////////////////////////////////////////////////////////////
// Symbian ClassECamera::Public
/////////////////////////////////////////////////////////////////////

SymbianCameraInterface::SymbianCameraInterface(ClassEdge * framework, ClassECamera * frontend)
    : modedefinitions(NULL),
      supportedmodes(0),
      camera(NULL)
{
    currentmode = -1;
    cameraindex = -1;
    statusflags = 0;
    orientationflags = 0;
    this->parent = framework;
    this->frontend = frontend;
}

SymbianCameraInterface::~SymbianCameraInterface()
{
    if (camera) delete camera;
}

ERESULT SymbianCameraInterface::RequestCamera(int camindex)
{

    if (camindex >= GetCameraCount()) return E_UNSUPPORTED;
    if (GetCameraCount() == 0) return E_UNSUPPORTED;

    if (camindex < 0)
    {
        for (int i = 0; i < GetCameraCount(); i++)
        {
            CCamera * trialcam = NULL;
            TInt error;
            TRAP(error, trialcam = CCamera::NewL(*this, i));
            if (error == KErrNone)
            {
                TCameraInfo info;
                trialcam->CameraInfo(info);
                if ( (  ((info.iOrientation == TCameraInfo::EOrientationOutwards) && (camindex == ELOC_REAR)) ||
                        ((info.iOrientation == TCameraInfo::EOrientationInwards) && (camindex == ELOC_FRONT)) ||
                        ((info.iOrientation == TCameraInfo::EOrientationMobile) && (camindex == ELOC_UNKNOWN)) ||
                        ((info.iOrientation == TCameraInfo::EOrientationUnknown) && (camindex == ELOC_UNKNOWN))
						) && (info.iOptionsSupported & TCameraInfo::EVideoCaptureSupported))
                {
                    if (camera)
                    {
                        CCamera * oldcam = camera;
                        oldcam->PowerOff();
                        oldcam->Release();
                        delete oldcam;
                        camera = NULL;
                    }

                    camera = trialcam;
                    trialcam->Reserve();
                    return E_OK;
                }
                delete trialcam;
            }
        }

        if (camindex != ELOC_UNKNOWN)
            return RequestCamera(ELOC_UNKNOWN);
        else
            return RequestCamera(0);

    }
    else
    {
        TInt error;
        CCamera * thiscam = NULL;
        TRAP(error, thiscam = CCamera::NewL(*this, camindex));
        if (error != KErrNone)
            return E_ERROR;

        cameraindex = -1;
        statusflags = ECS_INVALID;

        if (camera)
        {
            CCamera * oldcam = camera;
            oldcam->PowerOff();
            oldcam->Release();
            delete oldcam;
            camera = NULL;
        }

        cameraindex = camindex;
        camera = thiscam;

        thiscam->Reserve();
        return E_OK;
    }
}

int SymbianCameraInterface::GetCameraCount()
{
    return CCamera::CamerasAvailable();
}

ERESULT SymbianCameraInterface::RequestStart()
{
    if (!camera) return E_ERROR;
    ((CCamera *)camera)->StartVideoCapture();
    if (statusflags != ECS_INVALID) statusflags = ECS_RECORDING;
    return E_OK;
}

ERESULT SymbianCameraInterface::RequestStop()
{
    if (!camera) return E_ERROR;
    ((CCamera *)camera)->StopVideoCapture();
    if (statusflags != ECS_INVALID) statusflags = ECS_READY;
    return E_OK;
}

ERESULT SymbianCameraInterface::RequestResolution(int mode)
{
    if (statusflags == ECS_RECORDING) RequestStop();
    if (!camera) return E_ERROR;
    if (mode < 0) return E_INVALIDPARAM;
    if (mode >= supportedmodes) return E_INVALIDPARAM;

    TCameraInfo info;
    CCamera * cam = ((CCamera *)camera);
    cam->CameraInfo(info);

    unsigned char difforient = (4 + parent->ecd.dsp->rotation - parent->ecd.dsp->sysorientation) % 4;
	if (difforient == DOR_90DEG)
	    orientationflags = ECO_YFLIP | ECO_SWAPAXIS;
	else if (difforient == DOR_180DEG)
	    orientationflags = ECO_XFLIP | ECO_YFLIP;
    else if (difforient == DOR_270DEG)
	    orientationflags = ECO_XFLIP | ECO_SWAPAXIS;
	else
	    orientationflags = 0;

    int resolutions = info.iNumVideoFrameSizesSupported;
    int framerates = info.iNumVideoFrameRatesSupported;

    for (int floop = 0; floop < 32; floop++)
    {
        if (info.iVideoFrameFormatsSupported & (1 << floop))
        {
            for (int sloop = 0; sloop < resolutions; sloop++)
            {
                TSize resolution;
                resolution.iWidth = 0;
                resolution.iHeight = 0;
                cam->EnumerateVideoFrameSizes(resolution, sloop, (CCamera::TFormat)(1 << floop));
                if ((resolution.iWidth != 0) && (resolution.iHeight != 0))
                {
                    // supported pair
                    for (int rloop = 0; rloop < framerates; rloop++)
                    {
                        TReal32 rate;
                        cam->EnumerateVideoFrameRates(rate, rloop, (CCamera::TFormat)(1 << floop), sloop);
                        if (rate != 0)
                        {
                            E_CAMERAMODE thismode;
                            thismode.x = resolution.iWidth;
                            thismode.y = resolution.iHeight;
                            thismode.format = FormatToEdge(1 << floop);
                            thismode.expectedfps = (long)(rate * 65536.0f);

                            E_CAMERAMODE reqmode = modedefinitions[mode].modeinfo;
                            if (thismode.x == reqmode.x &&
                                thismode.y == reqmode.y &&
                                thismode.format == reqmode.format &&
                                thismode.expectedfps == reqmode.expectedfps)
                            {
                                TRAPD(ecode, cam->PrepareVideoCaptureL( (CCamera::TFormat)(1 << floop), sloop, rloop, 2, 1));
                                if (!ecode)
                                {
                                    statusflags = ECS_READY;
                                    currentmode = mode;

                                    parent->OnCameraChange(frontend, E_OK);
                                    ClassEdge::Log("new mode: ", false); ClassEdge::Log(currentmode);
                                    return E_OK;
                                }
                                currentmode = -1;
                                statusflags = ECS_INVALID;
                                ClassEdge::Log("[ClassECamera] Prepare failed");
                            }
                        }
                    }
                }
            }
        }
    }
    ClassEdge::Log("[ClassECamera] Resolution mismatch");
    return E_ERROR;
}

int SymbianCameraInterface::GetModeCount()
{
    return supportedmodes;
}

void SymbianCameraInterface::GetModeInformation(int index, E_CAMERAMODE & mode)
{
    if (index < 0) return;
    if (index >= supportedmodes) return;
    mode = modedefinitions[index].modeinfo;
}

int SymbianCameraInterface::GetOrientation(void)
{
    if (!camera) return ELOC_UNKNOWN;
    TCameraInfo ci;
    camera->CameraInfo(ci);
    if (ci.iOrientation == TCameraInfo::EOrientationOutwards) return ELOC_REAR;
    if (ci.iOrientation == TCameraInfo::EOrientationInwards) return ELOC_FRONT;
    return ELOC_UNKNOWN;
}

int SymbianCameraInterface::GetIndex()
{
    return cameraindex;
}

int SymbianCameraInterface::GetStatus()
{
    return statusflags;
}

int SymbianCameraInterface::GetSelectedResolution()
{
    return currentmode;
}

/////////////////////////////////////////////////////////////////////
// Symbian MCameraObserver callbacks
/////////////////////////////////////////////////////////////////////

void SymbianCameraInterface::ReserveComplete(TInt aError)
{
    if (aError)
    {
        ClassEdge::Log("[ClassECamera] Reserve failed");

        if (aError == KErrAccessDenied)
            parent->OnCameraChange(frontend, E_PERMISSION);
        else
            parent->OnCameraChange(frontend, E_ERROR);

        return;
    }

    camera->PowerOn();
}

void SymbianCameraInterface::PowerOnComplete(TInt aError)
{
    if (aError)
    {
        ClassEdge::Log("[ClassECamera] PowerOn failed");
        parent->OnCameraChange(frontend, E_ERROR);
        return;
    }

    CCamera * cam = camera;
    TCameraInfo info;
    cam->CameraInfo(info);

    int resolutions = info.iNumVideoFrameSizesSupported;
    int framerates = info.iNumVideoFrameRatesSupported;
    int formats = 0;
    for (unsigned long i = 0; i < 32; i++)
        if (info.iVideoFrameFormatsSupported & (1 << i)) formats++;

    modedefinitions = new E_SYMCAMERAMODE[formats * framerates * resolutions];
    int modecount = 0;
    for (int floop = 0; floop < 32; floop++)
    {
        if ((info.iVideoFrameFormatsSupported & (1 << floop)) && (FormatToEdge(1 << floop) >= 0))
        {
            for (int sloop = 0; sloop < resolutions; sloop++)
            {
                TSize resolution;
                    resolution.iWidth = 0;
                    resolution.iHeight = 0;
                    cam->EnumerateVideoFrameSizes(resolution, sloop, (CCamera::TFormat)(1 << floop));
                    if ((resolution.iWidth != 0) && (resolution.iHeight != 0))
                    {
                    // supported pair
                    for (int rloop = 0; rloop < framerates; rloop++)
                    {
                        TReal32 rate;
                            cam->EnumerateVideoFrameRates(rate, rloop, (CCamera::TFormat)(1 << floop), sloop);
                            if (rate != 0)
                            {
                            modedefinitions[modecount].modeinfo.x = resolution.iWidth;
                            modedefinitions[modecount].modeinfo.y = resolution.iHeight;
                            modedefinitions[modecount].modeinfo.format = FormatToEdge(1 << floop);
                            modedefinitions[modecount].modeinfo.expectedfps = (long)(rate * 65536.0f);
                            modedefinitions[modecount].formatindex = floop;
                            modedefinitions[modecount].framerateindex = rloop;
                            modedefinitions[modecount].resolutionindex = sloop;
                            modecount++;
                        }
                    }
                }
            }
        }
    }
    supportedmodes = modecount;
    
    if (RequestResolution(0) != E_OK)
        parent->OnCameraChange(frontend, E_ERROR);
}

void SymbianCameraInterface::ViewFinderFrameReady(CFbsBitmap &aFrame)
{
}

void SymbianCameraInterface::ImageReady(CFbsBitmap *aBitmap, HBufC8 *aData, TInt aError)
{
}

void SymbianCameraInterface::FrameBufferReady(MFrameBuffer *aFrameBuffer, TInt aError)
{
    if (aError) return; /* fixme, when does this happen */
    if (!aFrameBuffer) return;

    TInt error;
    TDesC8 * data = NULL;
    TRAP(error, data = aFrameBuffer->DataL(0));
    if (error != KErrNone) return;
    if (data == NULL) return;

    E_CAMERAFRAME frame;
    frame.data = (unsigned char *) data->Ptr();
    if (frame.data == NULL) return;

    frame.width = modedefinitions[currentmode].modeinfo.x;
    frame.height = modedefinitions[currentmode].modeinfo.y;
    frame.format = modedefinitions[currentmode].modeinfo.format;
    frame.pitch = 0;
    if (frame.format == EDSP_TRUE32) frame.pitch = 4;
    if (frame.format == EDSP_TRUE32_INV) frame.pitch = 4;
    if (frame.format == EDSP_TRUE16) frame.pitch = 2;
    if (frame.format == EDSP_TRUE15) frame.pitch = 2;
    frame.orientation = orientationflags;
    
    parent->OnCameraFrame(frontend, &frame);

    aFrameBuffer->Release();
}
