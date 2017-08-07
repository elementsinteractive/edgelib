// iphone4camera.h
// iPhone 4 camera class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

extern "C" void AVInit(char *);

class ClassEdge;
class ClassECamera;

extern void * CamCreate(ClassEdge * framework, ClassECamera * sourceobject);
extern long CamRequest(void *, int);
extern void CamDelete(void *);

#define ECS_PROBE 32

@interface IPhone4Camera : NSObject < AVCaptureVideoDataOutputSampleBufferDelegate > {
	ClassEdge * edge;
	ClassECamera * edgecam;
	int currentcam;
	
	AVCaptureDeviceInput * avinput;
	AVCaptureVideoDataOutput * avoutput;
	AVCaptureSession * avsession;
	dispatch_queue_t framequeue;
	
	CVImageBufferRef cacheimage;
	
	NSLock * queuelock;
	NSConditionLock * dequeuelock;
	
	int camwidth;
	int camheight;
	int currentstate;
	int currentformat;
	int restartlater;
}

-(id)initWithEdge:(ClassEdge *)framework withCamera:(ClassECamera *)sourceobject;
-(long)requestCamera:(int) camera;
-(void)releaseCamera;
-(void)dealloc;
-(void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection;
-(void)dispatchOutput:(id)object;
-(void)startplay;
-(void)stopplay;
-(void)probedone:(id)object;
-(void)aftershutdown:(id)object;
-(unsigned long)changeformat: (int)format;

-(unsigned long)requeststartplay;
-(unsigned long)requeststopplay;

-(unsigned long)getindex;
-(unsigned long)getstate;
-(unsigned long)getorientation;
-(int)getformat;
-(E_CAMERAMODE)getbasemode:(int)mode;

-(CVImageBufferRef) grabimage;

@end
