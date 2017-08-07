// windowscam.h
// Windows camera class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

// note: the camera orientation has been disabled due to friend class madness - please fix later.

//
// "null" implementation
//
static ECOREOBJ xecCamCreate(ECOREOBJ framework, ECOREOBJ frontend)
{
    return NULL;
}

static void xecCamDelete(ECOREOBJ camera)
{
}

static int xecCamDeviceCount(void)
{
    return 0;
}

unsigned long xecCamRequestCamera(ECOREOBJ camera, int index)
{
    return E_UNSUPPORTED;
}

static unsigned long xecCamStartCapture(ECOREOBJ camera)
{
    return E_ERROR;
}

static unsigned long xecCamStopCapture(ECOREOBJ camera)
{
    return E_ERROR;
}

static int xecCamGetResolutions(ECOREOBJ camera)
{
    return 0;
}

static void xecCamGetResolution(ECOREOBJ camera, int index, ECOREOBJ mode)
{
}

static unsigned long xecCamSetResolution(ECOREOBJ camera, int index)
{
    return E_ERROR;
}

static int xecCamGetOrientation(ECOREOBJ camera)
{
    return ELOC_UNKNOWN;
}

static int xecCamGetState(ECOREOBJ camera)
{
    return ECS_INVALID;
}

static int xecCamGetCurrentResolution(ECOREOBJ camera)
{
    return 0;
}

static int xecCamGetIndex(ECOREOBJ camera)
{	
	return 0;
}

static void xecCamAsyncFrameComplete(ECOREOBJ, long)
{
}

static ECOREOBJ xecCamAsyncFrameRetrieve(ECOREOBJ, long)
{
	return NULL;
}

// 
// bootstrap code
//

static ECOREOBJ (*temp_ecCamCreate)(ECOREOBJ framework, ECOREOBJ frontend) = &xecCamCreate;
static unsigned long (*temp_ecCamStartCapture)(ECOREOBJ camera) = &xecCamStartCapture;
static bool initialized = false;
static HMODULE hCameraModule = NULL;
static ClassEdge * hiddenframework;

static void CameraInit(void)
{
	hCameraModule = LoadLibrary(TEXT("edgecamera.dll"));
	if (hCameraModule)
	{
		ClassEdge::Log("Camera DLL found");		
		ecCamAsyncFrameComplete = (void (*)(ECOREOBJ, long)) GetProcAddress(hCameraModule, TEXT("?ecCamAsyncFrameComplete@@YAXPAXJ@Z"));
		ecCamAsyncFrameRetrieve = (ECOREOBJ(*)(ECOREOBJ, long)) GetProcAddress(hCameraModule, TEXT("?ecCamAsyncFrameRetrieve@@YAPAXPAXJ@Z"));
		temp_ecCamCreate = (void *(*)(ECOREOBJ, ECOREOBJ)) GetProcAddress(hCameraModule, TEXT("?ecCamCreate@@YAPAXPAX0@Z"));
		temp_ecCamStartCapture = (unsigned long (*)(void*)) GetProcAddress(hCameraModule, TEXT("?ecCamStartCapture@@YAKPAX@Z"));

		ecCamDelete = (void (*)(void*)) GetProcAddress(hCameraModule, TEXT("?ecCamDelete@@YAXPAX@Z"));
		ecCamDeviceCount = (int (*)(void)) GetProcAddress(hCameraModule, TEXT("?ecCamDeviceCount@@YAHXZ"));
		ecCamGetCurrentResolution = (int (*)(void*)) GetProcAddress(hCameraModule, TEXT("?ecCamGetCurrentResolution@@YAHPAX@Z"));
		ecCamGetIndex = (int (*)(void*)) GetProcAddress(hCameraModule, TEXT("?ecCamGetIndex@@YAHPAX@Z"));
		ecCamGetOrientation = (int (*)(void*))GetProcAddress(hCameraModule, TEXT("?ecCamGetOrientation@@YAHPAX@Z"));
		ecCamGetResolution = (void (*)(ECOREOBJ, int, ECOREOBJ)) GetProcAddress(hCameraModule, TEXT("?ecCamGetResolution@@YAXPAXH0@Z"));
		ecCamGetResolutions = (int (*)(void*)) GetProcAddress(hCameraModule, TEXT("?ecCamGetResolutions@@YAHPAX@Z"));
		ecCamGetState = (int (*)(void*)) GetProcAddress(hCameraModule, TEXT("?ecCamGetState@@YAHPAX@Z"));
		ecCamRequestCamera = (unsigned long (*)(void *, int)) GetProcAddress(hCameraModule, TEXT("?ecCamRequestCamera@@YAKPAXH@Z"));
		ecCamSetResolution = (unsigned long (*)(ECOREOBJ, int)) GetProcAddress(hCameraModule, TEXT("?ecCamSetResolution@@YAKPAXH@Z"));		
		ecCamStopCapture = (unsigned long (*)(void*))GetProcAddress(hCameraModule, TEXT("?ecCamStopCapture@@YAKPAX@Z"));
		ecwCamInit = (void (*)(ECOREOBJ, char *)) GetProcAddress(hCameraModule, TEXT("?ecwCamInit@@YAXPAXPAD@Z"));
		ecwCamOrientation = (void (*)(char)) GetProcAddress(hCameraModule, TEXT("?ecwCamOrientation@@YAXD@Z"));

		if (!ecCamAsyncFrameComplete || !ecCamAsyncFrameRetrieve || !ecCamDeviceCount || !ecCamDelete || !ecCamGetCurrentResolution || !ecCamGetIndex || !ecwCamInit || !ecwCamOrientation)
			ClassEdge::Log("Error: DLL does not match the expected interface");
	}

	initialized = true;
}

static int iecCamDeviceCount(void)
{
	ecCamDeviceCount = &xecCamDeviceCount;
	if (!initialized) CameraInit();
	return ecCamDeviceCount();
}

static ECOREOBJ iecCamCreate(ECOREOBJ framework, ECOREOBJ frontend)
{	
	if (!initialized) CameraInit();
	hiddenframework = (ClassEdge*) framework;
	
	if (ecwCamInit) 
	{
		char name[256];
		hiddenframework->ecd.device->GetModelName(name);
		ecwCamInit((ECOREOBJ) hiddenframework->GetWindowHandle(), name);		
		ECOREOBJ result = temp_ecCamCreate(hiddenframework, frontend);		
		return result;
	}	
	return NULL;
}

unsigned long iecCamStartCapture (ECOREOBJ camera)
{
	if (!initialized) CameraInit();
	/*
	if (ecwCamOrientation && framework)
		ecwCamOrientation((4+framework->getcurrentorientation() - getsystemorientation)%4);
	*/
	return temp_ecCamStartCapture(camera);
}

//
// pointers definitions
//
int (*ecCamDeviceCount)(void) = &iecCamDeviceCount;
ECOREOBJ (*ecCamCreate)(ECOREOBJ framework, ECOREOBJ frontend) = &iecCamCreate;
unsigned long (*ecCamStartCapture)(ECOREOBJ camera) = &iecCamStartCapture;

void (*ecCamDelete)(ECOREOBJ camera) = &xecCamDelete;
unsigned long (*ecCamRequestCamera)(ECOREOBJ camera, int index) = &xecCamRequestCamera;
int (*ecCamGetIndex)(ECOREOBJ camera) = &xecCamGetIndex;
int (*ecCamGetOrientation)(ECOREOBJ camera) = &xecCamGetOrientation;
unsigned long (*ecCamStopCapture)(ECOREOBJ camera) = &xecCamStopCapture;
int (*ecCamGetState)(ECOREOBJ camera) = &xecCamGetState;
int (*ecCamGetResolutions)(ECOREOBJ camera) = &xecCamGetResolutions;
int (*ecCamGetCurrentResolution)(ECOREOBJ camera) = &xecCamGetCurrentResolution;
void (*ecCamGetResolution)(ECOREOBJ camera, int index, ECOREOBJ mode) = &xecCamGetResolution;
unsigned long (*ecCamSetResolution)(ECOREOBJ camera, int index) = &xecCamSetResolution;
void (*ecCamAsyncFrameComplete)(ECOREOBJ camera, long) = &xecCamAsyncFrameComplete; 
ECOREOBJ (*ecCamAsyncFrameRetrieve)(ECOREOBJ camera, long) = &xecCamAsyncFrameRetrieve;
void (*ecwCamInit)(ECOREOBJ destwnd, char * modelname) = NULL;
void (*ecwCamOrientation)(char orientation) = NULL;
