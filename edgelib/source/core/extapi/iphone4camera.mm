// iphone4camera.mm
// iPhone 4 camera class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED

#include "estdc.h"
#include "esound.h"
#include "eloader.h"
#include "edisplay.h"
#include "einput.h"
#include "ememory.h"
#include "evtimer.h"
#include "efile.h"
#include "emath.h"
#include "eini.h"
#include "ereg.h"
#include "econsole.h"
#include "edevice.h"
#include "edgeframe.h"
#include "ecamera.h"
#include "edgecore.h"

#if !defined(DEVICE_IPHONEEMU) && !defined(DEVICE_DESKTOP)
// the camera API is device-only(!), no testing elsewhere :(

#import "iphone4camera.h"

//
// Support code
//

static NSArray * camdevices = nil;
static int * camdevused = NULL;

#define FORMATCOUNT 1
static int formatlut_e[FORMATCOUNT] = {
	EDSP_TRUE32,
//	EDSP_UYVY
};
static int formatlut_i[FORMATCOUNT] = {
	kCVPixelFormatType_32BGRA,
//	kCVPixelFormatType_422YpCbCr8
};

int PosToEdge(AVCaptureDevice * dev)
{
	if (dev.position == AVCaptureDevicePositionBack) return ELOC_REAR;
	if (dev.position == AVCaptureDevicePositionFront) return ELOC_FRONT;
	return ELOC_UNKNOWN;
}

const char * PosToString(AVCaptureDevice * dev)
{
	if (dev.position == AVCaptureDevicePositionBack) return "Rear";
	if (dev.position == AVCaptureDevicePositionFront) return "Front";
	return "Unknown";	
}

// Do all the stuff that needs to be done only once
void AVInit()
{
	NSArray * devices = [AVCaptureDevice devices];
	NSMutableArray * buildup = [[NSMutableArray alloc] init];

	for (int i = 0; i < devices.count; i++)
	{
		AVCaptureDevice * dev = [devices objectAtIndex:i];
	
		if ([dev hasMediaType:AVMediaTypeVideo] && dev.connected)
		{
			[buildup addObject:dev];
		}
		
	}
	
	camdevices = buildup;
	camdevused = new int[ecCamDeviceCount()];
	
	for (int i = 0; i < ecCamDeviceCount(); i++)
		camdevused[i] = 0;
		
}

//
// core functions
//

ECOREOBJ ecCamCreate(ECOREOBJ framework, ECOREOBJ sourceobject)
{
    IPhone4Camera * camera = [[IPhone4Camera alloc] initWithEdge:(ClassEdge*)framework withCamera:(ClassECamera*)sourceobject];
	if (camera == nil) return NULL;
	return camera;
}

void ecCamDelete(ECOREOBJ cam)
{
	IPhone4Camera * camera = (IPhone4Camera *) cam;
	[camera release];
}

int ecCamDeviceCount(void)
{
	if (camdevices == nil)
	{
		AVInit();
	}
	return camdevices.count;
}

unsigned long ecCamRequestCamera(ECOREOBJ cam, int index)
{
	IPhone4Camera * camera = (IPhone4Camera *) cam;
	return [camera requestCamera:index];
}


// every external interface not done below this line
unsigned long ecCamStartCapture(ECOREOBJ cam)
{
	IPhone4Camera * camera = (IPhone4Camera *) cam;
	return [camera requeststartplay];
}

unsigned long ecCamStopCapture(ECOREOBJ cam)
{
	IPhone4Camera * camera = (IPhone4Camera *) cam;
	return [camera requeststopplay];
}

int ecCamGetResolutions(ECOREOBJ camera)
{
	return FORMATCOUNT;
}

void ecCamGetResolution(ECOREOBJ cam, int index, ECOREOBJ mode)
{
	IPhone4Camera * camera = (IPhone4Camera *) cam;
	if (index < 0) return;
	if (index >= ecCamGetResolutions(camera)) return;
	
	*((E_CAMERAMODE *)mode) = [camera getbasemode:index];
}

unsigned long ecCamSetResolution(ECOREOBJ cam, int index)
{
	if (index < 0) return E_INVALIDPARAM;
	if (index >= FORMATCOUNT) return E_INVALIDPARAM;
	
	IPhone4Camera * camera = (IPhone4Camera *) cam;
	return [camera changeformat:index];
}

int ecCamGetOrientation(ECOREOBJ cam)
{
	IPhone4Camera * camera = (IPhone4Camera *) cam;
	return [camera getorientation];
}

int ecCamGetState(ECOREOBJ cam)
{
	IPhone4Camera * camera = (IPhone4Camera *) cam;
	return [camera getstate];
}

int ecCamGetCurrentResolution(ECOREOBJ cam)
{
	IPhone4Camera * camera = (IPhone4Camera *) cam;
	return [camera getformat];
}

int ecCamGetIndex(ECOREOBJ cam)
{	
	IPhone4Camera * camera = (IPhone4Camera *) cam;
	return [camera getindex];
}


//
// Camera classes
//


@implementation IPhone4Camera

-(id)initWithEdge:(ClassEdge *)framework withCamera:(ClassECamera *)sourceobject
{
	self = [super init];
	if (self)
	{
		dequeuelock = [[NSConditionLock alloc] initWithCondition:1];
		edge = framework;
		edgecam = sourceobject;
		currentcam = -1;
		queuelock = [[NSLock alloc] init];
		framequeue = dispatch_queue_create(NULL, NULL);
		restartlater = 0;
		currentstate = ECS_INVALID;

	}
	return self;
}

-(long)requestCamera:(int) camera
{
	if (camera < 0)
	{
		int maxcam = ecCamDeviceCount();
		for (int i = 0; i < maxcam; i++)
		{
			if (PosToEdge([camdevices objectAtIndex:i]) == camera && (camdevused[i] == 0 || i == currentcam))
				return [self requestCamera: i];
		}
		for (int i = 0; i < maxcam; i++)
		{
			if (PosToEdge([camdevices objectAtIndex:i]) == ELOC_UNKNOWN && (camdevused[i] == 0 || i == currentcam))
				return [self requestCamera: i];
		}
		for (int i = 0; i < maxcam; i++)
		{
			if (camdevused[i] == 0 || i == currentcam)
				return [self requestCamera: i];
		}
		
		return [self requestCamera: 0];
	}
	
	[self releaseCamera];
	
	if (camera >= ecCamDeviceCount())
	{
		edge->OnCameraChange(edgecam, E_NOTEXISTS);
		return E_NOTEXISTS;
	}
	if (camdevused[camera] == 1)
	{
		edge->OnCameraChange(edgecam, E_NOTEXISTS);
		return E_ERROR;
	}
	
	currentcam = camera;
	camdevused[camera] = 1;
	
	NSError * myerror = nil;
	avsession = [[AVCaptureSession alloc] init];
	avinput = [[AVCaptureDeviceInput alloc] initWithDevice:[camdevices objectAtIndex:camera] error:&myerror];
	avoutput = [[AVCaptureVideoDataOutput alloc] init];
	if (avsession == nil || avinput == nil || avoutput == nil || myerror != nil)
	{
		if (myerror != nil)
			[myerror release];
		[self releaseCamera];
		edge->OnCameraChange(edgecam, E_NOMEMORY);
		return E_NOMEMORY;
	}
	[avsession addInput:avinput];
	[avsession addOutput:avoutput];
	
	[avoutput setSampleBufferDelegate:self queue:framequeue];
	[avoutput setAlwaysDiscardsLateVideoFrames:YES];
	currentformat = 0;
	NSString * key = [NSString stringWithString: (NSString *)kCVPixelBufferPixelFormatTypeKey];
	NSNumber * value = [NSNumber numberWithUnsignedInt:formatlut_i[currentformat]];
	[avoutput setVideoSettings:[NSDictionary dictionaryWithObject:value forKey:key]];
	
	// start resolution test
	currentstate = ECS_PROBE;
	camheight = 0;
	camwidth = 0;
	restartlater = 0;
	printf("[probe start]\n");
	[avsession startRunning];
	
	// done for now
	return E_OK;
}

-(void)probedone: (id)object
{
	if (currentstate != ECS_PROBE) return;
	
	[self stopplay];
	//currentstate = ECS_READY;
	//edge->OnCameraChange(edgecam, E_OK);
}

-(void)releaseCamera
{
	if (currentcam >= 0)
	{
		if (currentstate == ECS_RECORDING) [self stopplay];
		if (avsession != nil) [avsession release];
		if (avinput != nil) [avinput release];
		if (avoutput != nil) [avoutput release];
		avsession = nil;
		avinput = nil;
		avoutput = nil;
		camdevused[currentcam] = 0;
	}
	currentcam = -1;
}

-(unsigned long)changeformat: (int)format
{
	[self stopplay];
	currentformat = format;
	NSString * key = [NSString stringWithString: (NSString *)kCVPixelBufferPixelFormatTypeKey];
	NSNumber * value = [NSNumber numberWithUnsignedInt:formatlut_i[format]];
	[avoutput setVideoSettings:[NSDictionary dictionaryWithObject:value forKey:key]];
	
	edge->OnCameraChange(edgecam, E_OK);
	return E_OK;
}

-(void)dealloc;
{
	[self releaseCamera];
	dispatch_release(framequeue);
	if (queuelock != nil)  [queuelock release];
	if (dequeuelock != nil) [dequeuelock release];
	[super dealloc];
}

-(void) captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
	if (currentstate == ECS_RECORDING)
	{
		[queuelock lock];
			if (currentstate != ECS_RECORDING) 
			{
				[queuelock unlock];
				return;
			}
			
			CVImageBufferRef ibuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
			CVPixelBufferLockBaseAddress(ibuffer, 0);
			
			cacheimage = ibuffer;
			[self performSelectorOnMainThread:@selector(dispatchOutput:) withObject:nil waitUntilDone:NO];
		[queuelock unlock];
		

		[dequeuelock lockWhenCondition:0];
			CVPixelBufferUnlockBaseAddress(ibuffer, 0);
		[dequeuelock unlockWithCondition:1];
	}
	else if (currentstate == ECS_PROBE)
	{
		[queuelock lock];
			if (currentstate != ECS_PROBE) 
			{
				[queuelock unlock];
				return;
			}
			CVImageBufferRef ibuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
			camwidth = CVPixelBufferGetWidth(ibuffer);
			camheight = CVPixelBufferGetHeight(ibuffer);
			[self performSelectorOnMainThread:@selector(probedone:) withObject:nil waitUntilDone:NO];
		[queuelock unlock];
	}
}

-(void) dispatchOutput:(id)object
{
	E_CAMERAFRAME frame;
	
	CVImageBufferRef ibuffer = [self grabimage];
	if (ibuffer == NULL) return;
	
	frame.data = (unsigned char *) CVPixelBufferGetBaseAddress(ibuffer);
	frame.pitch = CVPixelBufferGetBytesPerRow(ibuffer);
	frame.width = CVPixelBufferGetWidth(ibuffer);
	frame.height = CVPixelBufferGetHeight(ibuffer);
	
	frame.format = EDSP_TRUE32;
	frame.orientation = ECO_SWAPAXIS | ECO_XFLIP;
	
	edge->OnCameraFrame(edgecam, &frame);
	
	[dequeuelock lock];
	[dequeuelock unlockWithCondition:0];
	
}

-(void) stopplay
{
	if (restartlater > 0)
	{
		printf("[queue stop]\n");
		restartlater = 1;
	}
	else if (currentstate == ECS_RECORDING || currentstate == ECS_PROBE)
	{
		[queuelock lock];
		currentstate = ECS_READY;
		printf("[stop]\n");
		[avsession stopRunning];
		[queuelock unlock];
		
		if ([self grabimage])
		{
			// there was a pending frame, discard it
			[dequeuelock lock];
			[dequeuelock unlockWithCondition:0];
		}
		restartlater = 1;
		[self performSelector:@selector(aftershutdown:) withObject:nil afterDelay:2.0f];
	}
}

-(void) startplay
{
	if (restartlater > 0)
	{
		printf("[queue start]\n");
		restartlater = 2;
	}
	else if (currentstate == ECS_READY)
	{
		[queuelock lock];
		currentstate = ECS_RECORDING;
		printf("[start]\n");
		[avsession startRunning];
		[queuelock unlock];
	}
}

-(void) aftershutdown:(id)object
{
	printf("[shutdown fix]\n");
	if (restartlater == 2)
	{
		restartlater = 0;
		[self startplay];
	}
	else restartlater = 0;
}

-(CVImageBufferRef) grabimage
{
	CVImageBufferRef ibuffer;
	[queuelock lock];
	ibuffer = cacheimage;
	cacheimage = NULL;
	[queuelock unlock];
	return ibuffer;
}

-(unsigned long)requeststartplay
{
	if (currentstate != ECS_READY) return E_ERROR;
    [self startplay];
	unsigned long result = E_OK;
	if (currentstate != ECS_RECORDING) 
		result = E_ERROR;
	
	edge->OnCameraChange(edgecam, result);
	return result;
}
-(unsigned long)requeststopplay
{
	if (currentstate != ECS_RECORDING) return E_OK;
	[self stopplay];
	unsigned long result = E_OK;
	if (currentstate != ECS_READY) 
		result = E_ERROR;
		
		edge->OnCameraChange(edgecam, result);
	return result;
}

-(unsigned long)getstate
{
	if (currentstate == ECS_PROBE) return ECS_INVALID;
	return currentstate;
}

-(unsigned long)getindex
{
	return currentcam;
}

-(int)getformat
{
	return currentformat;
}

-(unsigned long)getorientation
{
	return PosToEdge([camdevices objectAtIndex:currentcam]);
}

-(E_CAMERAMODE)getbasemode:(int)index
{
	E_CAMERAMODE mode;
	mode.x = camwidth;
	mode.y = camheight;
	mode.expectedfps = 0;
	mode.format = formatlut_e[index];
	return mode;
}

@end

#endif