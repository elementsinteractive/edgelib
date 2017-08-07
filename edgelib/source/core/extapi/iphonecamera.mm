// iphonecamera.mm
// iPhone camera class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#import "iphonecamera.h"

#import "edgedevice.h"
#import "edgedef.h"
#define EDGE_INCLUDED

#import "eui.h"
#import "estdc.h"
#import "eloader.h"
#import "edisplay.h"
#import "esound.h"
#import "einput.h"
#import "ememory.h"
#import "efile.h"
#import "eini.h"
#import "emath.h"
#import "ereg.h"
#import "edevice.h"
#import "econsole.h"
#import "evtimer.h"
#import "edgeframe.h"
#import "edge2.h"
#import "ecamera.h"

// Due to the nature of the camera interface, only one camera can theoretically be supported
// this is currently not a problem since the max camera's found on one device is one (the rear cam of an iPhone)

// declare static since repeated startups are known to crash iPhoneOS. Shutdown at end of application can't be prevented however
// causing a necessary reboot of the iphone
static UIImagePickerController * iphonepicker;
// the one camera class
static IPhoneCamera * iphonecamerabackend;
static ClassECamera * curfrontend;
static ClassEdgeII * curframework;
static UIView * hiddenview;
static NSTimer * bootstraptimer;
static NSThread * capturethread;
static NSThread * dispatchthread;
static int bootstraptries;

static int camera_xres = 0;
static int camera_yres = 0;
static int camera_status = 0;

static bool initialized = false;

bool initializebackend(void);

bool initializebackend(void)
{
	if (!initialized)
	{
		// Singleton instantiation
		
		iphonecamerabackend = [IPhoneCamera alloc];
		if (iphonecamerabackend == nil) return false;
		
		iphonepicker = nil;
		initialized = true;
		
		bootstraptimer = nil;
		capturethread = nil;
	}
	
	return true;
}

ECOREOBJ ecCamCreate(ClassEdgeII * framework, ClassECamera * frontend)
{
	if (curfrontend == NULL)
	{		
		if (!initializebackend()) return NULL;
				
		curfrontend = frontend;
		curframework = framework;
		
		return iphonecamerabackend;
	}
	else return NULL;
}

void ecCamDelete(ECOREOBJ camera)
{
	if (camera == iphonecamerabackend)
	{
		ecCamStopCapture(camera);
		curfrontend = NULL;
	}
}

int ecCamDeviceCount()
{
	UIDevice *device = [UIDevice currentDevice];
	if ([device.model isEqualToString:@"iPhone"] == NO)
		return 0; // ipod touches do not have cameras
	if ([device.systemVersion compare: @"3.1"] == NSOrderedAscending)
		return 0; // version 3.1 required
	
	if ([UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera] == NO) return 0;
	
	if (!initializebackend()) return 0;
	
	if (iphonepicker == nil)
	{
		iphonepicker = [[UIImagePickerController alloc] init];
		if (iphonepicker == nil) return 0;
		
		[iphonepicker setDelegate:iphonecamerabackend];		
		[iphonepicker setSourceType:UIImagePickerControllerSourceTypeCamera];
		[iphonepicker setShowsCameraControls:NO];
		
		camera_status = 0;
		bootstraptries = 0;
		
		CGRect rc;
		rc.size.width = 0;
		rc.size.height = 0;
		rc.origin.x = 0;
		rc.origin.y = 0;
		hiddenview = [[UIView alloc] initWithFrame:rc];
		
		UIWindow * mywindow = [[UIApplication sharedApplication] keyWindow];
		[mywindow addSubview:hiddenview];
		
		UIViewController * tempvc = [[UIViewController alloc] init];
		[hiddenview addSubview:tempvc.view];
		[tempvc presentModalViewController:iphonepicker animated:YES];
		
	}
	
	return 1;
}

ERESULT ecCamRequestCamera(ECOREOBJ camera, int index)
{
	if (camera != iphonecamerabackend) return E_ERROR;
	if (index > 0) return E_UNSUPPORTED; // addressing by location will always yield the one camera used
	if (ecCamDeviceCount() == 0) return E_UNSUPPORTED;
	
	// picker was partially started by ecCamDeviceCount
	
	if (camera_status == 1)
	{
		curframework->OnCameraChange(curfrontend, E_OK);
	} 
	else
	{
		camera_status = 0;
		// start camera probe
		
		if (bootstraptimer == nil)
		{
			bootstraptimer = [NSTimer scheduledTimerWithTimeInterval:3.0 target:iphonecamerabackend selector:@selector(timerCallback:) userInfo:nil repeats:YES];
		}
		else
		{
			return E_ABORTED;
		}
		
	}
	return E_OK;
}

int ecCamGetOrientation(ECOREOBJ camera)
{
	if (camera == iphonecamerabackend) return ELOC_REAR;
	return ELOC_UNKNOWN;
}

int ecCamGetIndex(ECOREOBJ camera)
{
	if (camera == iphonecamerabackend)
		return 0;
	return -1;
}

int ecCamGetState(ECOREOBJ camera)
{
	if (camera != iphonecamerabackend) return ECS_INVALID;
	if (camera_status == 1) return ECS_READY;
	if (camera_status == 2) return ECS_RECORDING;
	return ECS_INVALID;
}

int ecCamGetResolutions(ECOREOBJ camera)
{
	if (ecCamGetState(camera) == ECS_INVALID) return 0;
	return 1;
}

void ecCamGetResolution(ECOREOBJ camera, int modeindex, E_CAMERAMODE & mode)
{
	if (camera != iphonecamerabackend) return;
	if (modeindex != 0) return;
	mode.x = camera_xres;
	mode.y = camera_yres;
	mode.format = EDSP_TRUE32;
	mode.expectedfps = 1; // *really* slow
}

ERESULT ecCamSetResolution(ECOREOBJ camera, int index)
{
	if (camera != iphonecamerabackend) return E_INVALIDPARAM;
	if (index != 0) return E_INVALIDPARAM;
	if (camera_status < 1) return E_INVALIDPARAM;
	curframework->OnCameraChange(curfrontend, E_OK);
	return E_OK;
}

int ecCamGetCurrentResolution(ECOREOBJ camera)
{
	return 0;
}

ERESULT ecCamStartCapture(ECOREOBJ camera)
{
	if (camera != iphonecamerabackend) return E_INVALIDPARAM;
	if (camera_status < 1) return E_INVALIDPARAM;
	if (camera_status == 1)
	{
		camera_status = 2;
		[iphonecamerabackend performSelector:@selector(captureThreadStartCapture:) onThread:capturethread withObject:nil waitUntilDone:YES];
		//[iphonepicker takePicture];
	}
	return E_OK;
}
ERESULT ecCamStopCapture(ECOREOBJ camera)
{
	if (camera != iphonecamerabackend) return E_INVALIDPARAM;
	if (camera_status < 1) return E_INVALIDPARAM;
	camera_status = 1;
	return E_OK;	
}


void navigator(UIView * thisview, int level)
{
	if ([thisview subviews] == nil) return;
	for (int i = 0; i < [[thisview subviews] count]; i++)
	{
		UIView * object = [[thisview subviews] objectAtIndex:i];
		if (object != nil)
		{
			navigator(object, level+1);
		}
	}
}

// this is the class where messages return to
@implementation IPhoneCamera

- (void)imagePickerController:(UIImagePickerController *)thispicker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
	UIImage * capturedImage = [info objectForKey:@"UIImagePickerControllerOriginalImage"];
	CGImageRef quartzData = capturedImage.CGImage;
	
	if (quartzData == nil) 
	{
		if (camera_status == 2) [iphonepicker takePicture];
		return;
	}
	
	size_t iwidth = CGImageGetWidth(quartzData);
	size_t iheight = CGImageGetHeight(quartzData);
	
	if(camera_status == 0)
	{
		camera_xres = iwidth;
		camera_yres = iheight;
		
		if (capturethread == nil)
		{
			dispatchthread = [NSThread currentThread];
			capturethread = [[NSThread alloc] initWithTarget:self selector:@selector(captureThreadMain:) object:nil];
			[capturethread start];
		}
		
		camera_status = 1;
	
		// push camera ok to application
		curframework->OnCameraChange(curfrontend, E_OK);		
		
	} 
	else if (camera_status == 2)
	{
		
		CFDataRef dataptr = CGDataProviderCopyData(CGImageGetDataProvider(quartzData));
		const unsigned char * data = CFDataGetBytePtr(dataptr);
		E_CAMERAFRAME frame;
		frame.data = data;
		frame.width = iwidth;
		frame.height = iheight;
		frame.format = EDSP_TRUE32;
		frame.pitch = iwidth * 4;
		frame.orientation = ECO_SWAPAXIS | ECO_XFLIP;
		
		curframework->OnCameraFrame(curfrontend, &frame);
		
		// push data to application - sync issue!
		
		CFRelease(dataptr);	
		if ([NSThread currentThread] != capturethread) 
		{
			[iphonecamerabackend performSelector:@selector(captureThreadStartCapture:) onThread:capturethread withObject:nil waitUntilDone:YES];
			return;
		}	
		else
		{
			[iphonepicker takePicture];
		}
		
	}	
	
	return;
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)thispicker
{
	return;
}

- (void)navigationController:(UINavigationController *)navigationController didShowViewController:(UIViewController *)viewController animated:(BOOL)animated
{
	hiddenview.hidden = YES;
	return;
}

- (void)navigationController:(UINavigationController *)navigationController willShowViewController:(UIViewController *)viewController animated:(BOOL)animated
{
	return;
}

- (void)timerCallback:(NSTimer*)timer
{
	if (camera_status >= 1)
	{
		[timer invalidate];
	}
	else if (bootstraptries == 5)
	{
		camera_status = -1;
		[timer invalidate];		
		curframework->OnCameraChange(curfrontend, E_ABORTED);		
	}
	else
	{
		bootstraptries++;
		[iphonepicker takePicture];
	}
	
	return;
}

- (void)captureThreadMain: (id)object
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	NSRunLoop * loop = [NSRunLoop currentRunLoop];
	
	while(1)
	{
		[loop runUntilDate:[NSDate dateWithTimeIntervalSinceNow:1]];
	}
	
	[pool release];
}

-(void)captureThreadStartCapture: (id)object
{
	[iphonepicker takePicture];
}

@end