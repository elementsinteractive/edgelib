// iphoneframe.h
// iPhoneEdgeCoreTest
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_IPHONE)

#define MAX_IPHONETOUCH 5

typedef struct {
	ecEvent event;
	long prevX;
	long prevY;
	bool cancelAll;
}TOUCHEVENT;

typedef struct {
	long x;
	long y;
	bool isInUse;
	bool isUpdated;
}BUFFERTOUCH;


#if defined(IPHONE_SINGLETHREAD)
	@interface OCEvent : NSObject
	{
		ecEvent event;
	}

	- (ecEvent *)getEvent;

	@end
#endif

@interface IPhoneView : UIView <UIAlertViewDelegate> {
	//Vars/definitions
	#define MAX_IEVENTQUEUE   64
	#define MAX_MULTITOUCH    16
	TOUCHEVENT eventqueue[MAX_IEVENTQUEUE];
	BUFFERTOUCH touchBuffer[MAX_MULTITOUCH];
	long event_ptridx;
	long event_pushidx;
	
	NSThread *thread;
	NSConditionLock *threadlock;
	CGRect screenRect;
	CGRect rectImg;
	CGImageRef img;
	CGFloat pixelscale;
	EC_INSTANCEOBJ *inst;
	bool isRunning;
	bool isThreadExited;
	bool isDrawn;
	bool boolThreadLock;
}

- (id)initWithFrame:(CGRect)frame :(EC_INSTANCEOBJ *)instobj;
- (TOUCHEVENT *)getEventQueue;
- (long *)getEventPushIdx;
- (long *)getEventPtrIdx;
- (void)startThread:(bool)executeThread;
- (void)stopThread:(bool)forcedQuit;
- (void)throwEvent:(id)event;
- (void)runloop;
- (void)flipSurface:(CGImageRef)imgref :(CGRect *)rect;
- (void)emptyTouchBuffer;
- (void)setPixelScale:(CGFloat)scale;
- (CGFloat)getPixelScale;
- (EC_INSTANCEOBJ *)getInstanceObject;
- (void)drawSoftwareRect:(CGRect)rect;
@end

@interface IPhoneViewSoftrast : IPhoneView
{	
}
- (void)drawRect:(CGRect)rect;

@end

@interface IPhoneFrame : NSObject <UIApplicationDelegate, UIAccelerometerDelegate> {
	UIWindow *window;
	IPhoneView *view;
	CGRect screenRect;
	EC_INSTANCEOBJ *inst;
	bool isRunning;
}

@end

#elif defined(DEVICE_MAC)

@interface MacWindow : NSWindow
{
	
}

-(BOOL)canBecomeKeyWindow;

@end


@interface MacView : NSView
{
	EC_INSTANCEOBJ *inst;
}

- (EC_INSTANCEOBJ *)getInstance;
- (id)initWithFrame:(NSRect)frame :(EC_INSTANCEOBJ *)instobj;
- (void)translateMouseX:(ecParam *)x Y:(ecParam *)y;

@end

@interface MacViewGL : NSOpenGLView
{
	EC_INSTANCEOBJ *inst;	
	MacView * delegate;
}

- (EC_INSTANCEOBJ *)getInstance;
- (id)initWithFrame:(NSRect)screenRect pixelFormat:(NSOpenGLPixelFormat *)_format instance:(EC_INSTANCEOBJ *)instobj delegate:(MacView *)originalview;


@end


@interface MacFrame : NSResponder <NSWindowDelegate> {
	NSWindow *window;
	MacView *view;
	CGImageRef img;
	NSTimer *timer;
	ECOREOBJ glcontext;
	
	EC_INSTANCEOBJ *inst;
}

@property (assign) EC_INSTANCEOBJ * inst;

- (CGContextRef) GetContext;
- (void)windowWillClose:(NSNotification *)notification;
- (BOOL)windowShouldClose:(id)windows;

@end

#endif

bool ecOnEvent(ECOREOBJ instance, ecEvent *event);

//loops and window
int StartMacWindow(int argc, char *argv[], EC_INSTANCEOBJ *obj);
void KillMacWindow(EC_INSTANCEOBJ *obj);
void StartMacLoop(EC_INSTANCEOBJ *obj);
void StopMacLoop(EC_INSTANCEOBJ *obj);
void MacGetCurrentPath(char *path);
void MacGetReadOnlyPath(char *path);
void MacGetNativeResolution(ECOREOBJ winhandle, long &xres, long &yres);
void MacGetDeviceID(char *deviceId);
void MacGetDeviceModel(char *deviceModel);
void MacGetDeviceOS(char *deviceOS);
void MacGetDeviceName(char *deviceName);
void MacMsgWindow(char *msg, char *caption = NULL);
void MacVibrate(void);
void MacGetBattery(unsigned char * level, unsigned char * status);
//push event function
#if defined(DEVICE_IPHONE)
	void InitEventBuffer(BUFFERTOUCH *buffer);
	void PushEvent(TOUCHEVENT *qarray, TOUCHEVENT *pushevt, long &pushidx, long &ptridx);
	void MacGetLanguage(char *language);
	void MacGetLocale(char *locale);
#endif

//2D software
bool MacOpenDisplay(ECOREOBJ winhandle, long &xres, long &yres);
bool MacAllocSurface(ECOREOBJ winhandle, ECOREOBJ &surf, void *buffer, long xres, long yres, long lpitch, long bitdepth);
bool MacDeallocSurface(ECOREOBJ surf);
bool MacFlipSurface(ECOREOBJ winhandle, ECOREOBJ surf, long *pos, long *rect);

//OpenGL
void MacCreateOpenGLContext(void *window, void *&context, GLuint &depthformat, void *format);
bool MacCreateOpenGLSurface(void *window, void *&windowsurface, void *context, GLuint &renderbuffer, GLuint &framebuffer, GLuint &depthbuffer, GLuint &depthformat, unsigned long &width, unsigned long &height);
void MacCloseOpenGL(void *context, GLuint &renderbuffer, GLuint &framebuffer, GLuint &depthbuffer, GLuint &depthformat);
void MacFlipOpenGL(void *windowsurface, void *&contextOpenGL, GLuint &renderbuffer);
