// iphoneframe.mm
// iPhoneEdgeCoreTest
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Enable this to have EDGELIB running in a single thread
#define IPHONE_SINGLETHREAD
//#define IPHONE_OPENGLTHREADFIX

//EDGELIB includes
#include "edgedevice.h"
#include "edgecore.h"
#define EDGE_INCLUDED
#include "edgedef.h"
#include "estdc.h"
#include "edevice.h"
#include "appleinstanceobj.h"

//Apple framework includes
#if defined(DEVICE_IPHONE)
	#include <OpenGLES/EAGL.h>
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
	#include <OpenGLES/ES2/gl.h>
	#include <QuartzCore/CAEAGLLayer.h>
	#include <UIKit/UIKit.h>
	#include <AudioToolbox/AudioServices.h>
	#include <sys/sysctl.h>
    #include <sys/socket.h>
    #include <net/if.h>
    #include <net/if_dl.h>
#elif defined(DEVICE_MAC)
	#include <Cocoa/Cocoa.h>
	#include <OpenGL/OpenGL.h>
#endif

//Include interface header
#include "appleframe.h"

//Imported function to determine if OpenGL is used
bool EdgeGL_Use(void);

//Global pointer to keep EDGELIB data when launching the UI application framework
ECOREOBJ global_userdata;

//Event storage for singlethread
#if defined(IPHONE_SINGLETHREAD) && !defined(DEVICE_DESKTOP)
	@implementation OCEvent

	- (ecEvent *)getEvent
	{
		return &event;
	}

	@end
#endif


///////////////////////////////////////////////////////////////////
// Message box interface and implementation
///////////////////////////////////////////////////////////////////

@interface MessageWindow : NSObject {
	char caption[256];
	char msg[256];
}

- (void) SetCaption:(char*) caption;
- (void) SetMessage:(char*) msg;

- (void) ShowWindow;

@end


@implementation MessageWindow

- (void) SetCaption:(char*) newcaption
{
	ecStrCpy(self->caption, newcaption);
}

- (void) SetMessage:(char*) newmsg
{
	ecStrCpy(self->msg, newmsg);
}

- (void) ShowWindow
{
	#if defined(DEVICE_IPHONE)
		NSString *oMsg = [[NSString alloc] initWithUTF8String:msg];
		NSString *oCaption = [[NSString alloc] initWithUTF8String:caption];
		UIWindow *window = [[UIApplication sharedApplication] keyWindow];
		UIView *delegate = [[window subviews] objectAtIndex:0];
		
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:oCaption message:oMsg
													   delegate:nil cancelButtonTitle:@"OK" otherButtonTitles: nil];
		
		[alert setDelegate:delegate];
		
		[alert show];	
		[alert release];
		[oMsg release];
		[oCaption release];
	#endif
}

@end


///////////////////////////////////////////////////////////////////
// Wrapper functions
///////////////////////////////////////////////////////////////////

//create an iPhone/Mac application
int StartMacWindow(int argc, char *argv[], EC_INSTANCEOBJ *obj)
{
	int retVal;
	global_userdata = (ECOREOBJ)obj;
	
	#if defined(DEVICE_IPHONE)
		NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
		retVal = UIApplicationMain(argc, argv, nil, @"IPhoneFrame");
		[pool release];
	#else
		NSAutoreleasePool *pool;
		MacFrame *delegate;
	
		pool = [[NSAutoreleasePool alloc] init];
		delegate = [[MacFrame alloc] init];
		[delegate setInst:obj];
	
		[NSApplication sharedApplication];
		[NSApp setDelegate: delegate];
		[NSApp run];
	
		retVal = NSApplicationMain(argc,  (const char **)argv);
	
		[pool release];
		[delegate release];
	#endif
	
	return retVal;
}

//stop the iPhone/Mac application
void KillMacWindow(EC_INSTANCEOBJ *obj)
{
	ecInstDelete(obj);
	#if defined(DEVICE_IPHONE)
		//[[UIApplication sharedApplication] terminate];
	#else
		[[NSApplication sharedApplication] terminate:NULL];
	#endif
}

void MacOnEvent(EC_INSTANCEOBJ * inst, ecEvent * event)
{
	ecOnEvent(inst, event);
}

//start the loop for the iPhone/Mac
void StartMacLoop(EC_INSTANCEOBJ *obj)
{
	#if defined(DEVICE_IPHONE)
		[(IPhoneView *)obj->view startThread:false];
	#else
	#endif
}

//stop the loop for the iPhone/Mac
void StopMacLoop(EC_INSTANCEOBJ *obj)
{
	#if defined(DEVICE_IPHONE)
		[(IPhoneView *)obj->view stopThread:false];
	#else
	#endif
}

//iphone return application path
void MacGetCurrentPath(char *path)
{
	#if defined(DEVICE_IPHONE)
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, true);
		NSString *documentsDirectory = [paths objectAtIndex:0];
		
		ecStrCpy(path, (char *)[documentsDirectory UTF8String]);
		ecStrCat(path, "/");
	#else
		NSString *documentsDirectory = [[NSBundle mainBundle] executablePath];
		
		ecStrCpy(path, (char *)[documentsDirectory UTF8String]);
		long ctr = ecStrLen(path) - 1;
		while(ctr > 0)
		{
			if (path[ctr] == '/')
			{
				path[ctr + 1] = 0;
				break;
			}
			ctr--;
		}
	#endif
}

//Get read-only path from bundle
void MacGetReadOnlyPath(char *path)
{
	#if defined(DEVICE_IPHONE)
		NSBundle *thisBundle = [NSBundle bundleForClass:NULL];
		NSString *currentPath = [thisBundle executablePath];
		ecStrCpy(path, (char *)[currentPath UTF8String]);
	#else
		NSBundle * thisBundle = [NSBundle mainBundle];
		NSString * resourcePath = [thisBundle resourcePath];
		ecStrCpy(path, (char *)[resourcePath UTF8String]);	
		ecStrCat(path, "/");
		//MacGetCurrentPath(path);
	#endif
}

//Get native resolution
void MacGetNativeResolution(ECOREOBJ winhandle, long &xres, long &yres)
{
	#if defined(DEVICE_IPHONE)
		UIWindow *win = (UIWindow *)winhandle;
		IPhoneView *view = (IPhoneView *)[[win subviews] objectAtIndex:0];
		CGRect screenRect = [view bounds];
		xres = screenRect.size.width;
		yres = screenRect.size.height;
		CGFloat scale = [view getPixelScale];
		xres *= scale;
		yres *= scale;
	#else
		NSWindow *win = (NSWindow *)winhandle;
		NSRect screenRect = [win frame];
		xres = screenRect.size.width;
		yres = screenRect.size.height;
	#endif
}

//Get device identifier
#if defined(DEVICE_IPHONE)
NSString * getMacAddress()
{
    int                 mgmtInfoBase[6];
    char                *msgBuffer = NULL;
    size_t              length;
    unsigned char       macAddress[6];
    struct if_msghdr    *interfaceMsgStruct;
    struct sockaddr_dl  *socketStruct;
    NSString            *errorFlag = NULL;
    
    // Setup the management Information Base (mib)
    mgmtInfoBase[0] = CTL_NET;        // Request network subsystem
    mgmtInfoBase[1] = AF_ROUTE;       // Routing table info
    mgmtInfoBase[2] = 0;              
    mgmtInfoBase[3] = AF_LINK;        // Request link layer information
    mgmtInfoBase[4] = NET_RT_IFLIST;  // Request all configured interfaces
    
    // With all configured interfaces requested, get handle index
    if ((mgmtInfoBase[5] = if_nametoindex("en0")) == 0) 
        errorFlag = @"if_nametoindex failure";
    else
    {
        // Get the size of the data available (store in len)
        if (sysctl(mgmtInfoBase, 6, NULL, &length, NULL, 0) < 0) 
            errorFlag = @"sysctl mgmtInfoBase failure";
        else
        {
            // Alloc memory based on above call
            if ((msgBuffer = (char*) malloc(length)) == NULL)
                errorFlag = @"buffer allocation failure";
            else
            {
                // Get system information, store in buffer
                if (sysctl(mgmtInfoBase, 6, msgBuffer, &length, NULL, 0) < 0)
                    errorFlag = @"sysctl msgBuffer failure";
            }
        }
    }
    
    // Befor going any further...
    if (errorFlag != NULL || msgBuffer == NULL)
    {
        NSLog(@"Error: %@", errorFlag);
        return @"";
    }
    
    // Map msgbuffer to interface message structure
    interfaceMsgStruct = (struct if_msghdr *) msgBuffer;
    
    // Map to link-level socket structure
    socketStruct = (struct sockaddr_dl *) (interfaceMsgStruct + 1);
    
    // Copy link layer address data in socket structure to an array
    memcpy(&macAddress, socketStruct->sdl_data + socketStruct->sdl_nlen, 6);
    
    // Read from char array into a string object, into traditional Mac address format
    NSString *macAddressString = [NSString stringWithFormat:@"%02X:%02X:%02X:%02X:%02X:%02X", 
                                  macAddress[0], macAddress[1], macAddress[2], 
                                  macAddress[3], macAddress[4], macAddress[5]];
    NSLog(@"Mac Address: %@", macAddressString);
    
    // Release the buffer memory
    free(msgBuffer);
    
    return macAddressString;
}
#endif


void MacGetDeviceID(char *deviceId)
{    
	#if defined(DEVICE_IPHONE)
		NSString *uniqueIdentifier = getMacAddress();
		ecStrCpy(deviceId, [uniqueIdentifier UTF8String]);
    #else
        ecStrCpy(deviceId, "");
	#endif
}

//Get device model (iPhone or iPod)
void MacGetDeviceModel(char *deviceModel)
{
	#if defined(DEVICE_IPHONE)
		size_t size;
		sysctlbyname("hw.machine", NULL, &size, NULL, 0);
		char * machine = (char*) malloc(size);	
		UIDevice *device = [UIDevice currentDevice];
		NSString *model = [device model];
		sysctlbyname("hw.machine", machine, &size, NULL, 0);
		ecStrCpy(deviceModel, [model UTF8String]);
		if (strcmp(machine, "i386"))
		{
			ecStrCat(deviceModel, " (");
			ecStrCat(deviceModel, machine);
			ecStrCat(deviceModel, ")");
		}
		free(machine);
	#endif
}

//Get operating system version
void MacGetDeviceOS(char *deviceOS)
{
	#if defined(DEVICE_IPHONE)
		UIDevice *device = [UIDevice currentDevice];
		NSString *systemName = [device systemName];
		ecStrCpy(deviceOS, [systemName UTF8String]);
		ecStrCat(deviceOS, " ");
		systemName = [device systemVersion];
		ecStrCat(deviceOS, [systemName UTF8String]);
	#endif
}

//Get name of the device
void MacGetDeviceName(char *deviceName)
{
	#if defined(DEVICE_IPHONE)
		UIDevice *device = [UIDevice currentDevice];
		NSString *name = [device name];
		ecStrCpy(deviceName, [name UTF8String]);
	#endif
}

//Open message window
void MacMsgWindow(char *msg, char *caption)
{
	MessageWindow *msgwindow = [MessageWindow alloc];
	[msgwindow SetMessage:msg];
	[msgwindow SetCaption:caption];
	[msgwindow performSelectorOnMainThread:@selector(ShowWindow) withObject:nil waitUntilDone:true];
	[msgwindow release];
}

//Vibrate for a short while
void MacVibrate()
{
	#if defined(DEVICE_IPHONE)
		AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
	#endif
}

//iphone open display
bool MacOpenDisplay(ECOREOBJ winhandle, long &xres, long &yres)
{
	#if defined(DEVICE_IPHONE)
		UIWindow *win = (UIWindow *)winhandle;
		UIView *view = (UIView *)[[win subviews] objectAtIndex:0];
		CGRect screenRect = [view bounds];
		xres = screenRect.size.width;
		yres = screenRect.size.height;
	#else
		NSWindow *win = (NSWindow *)winhandle;
		NSRect screenRect = [win frame];
	#endif
	return(true);
}

//iphone alloc surface
bool MacAllocSurface(ECOREOBJ winhandle, ECOREOBJ &surf, void *buffer, long xres, long yres, long lpitch, long bitdepth)
{
	CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceRGB();
	CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, buffer, yres * lpitch, NULL);
	surf = CGImageCreate(xres, yres, 8, bitdepth, lpitch, colorspace, kCGImageAlphaNoneSkipLast, provider, NULL, FALSE, kCGRenderingIntentDefault);
	CGColorSpaceRelease(colorspace);
	CGDataProviderRelease(provider);
	return(true);
}

//iphone dealloc surface
bool MacDeallocSurface(ECOREOBJ surf)
{
	CGImageRelease((CGImageRef)surf);
	return(true);
}

//iphone flip surface
bool MacFlipSurface(ECOREOBJ winhandle, ECOREOBJ surf, long *pos, long *rect)
{
	CGImageRef img = (CGImageRef)surf;
	CGRect rectImg = CGRectMake(pos[0] + rect[0], pos[1] + rect[1], rect[2] - rect[0], rect[3] - rect[1]);
	#if defined(DEVICE_IPHONE)
		UIWindow *win = (UIWindow *)winhandle;
		IPhoneView *view = (IPhoneView *)[[win subviews] objectAtIndex:0];
		[view flipSurface:img :&rectImg];
	#else
		NSWindow * window = (NSWindow *) winhandle;
		MacView * view = window.contentView;
		EC_INSTANCEOBJ * inst = [view getInstance];		
		CGContextRef ctx = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
		if (inst->curwinstyle.fullscreen)
		{			
			CGContextSaveGState(ctx);
			CGContextScaleCTM(ctx, (float)inst->screenx / (float)inst->curwinstyle.width , (float)inst->screeny / (float)inst->curwinstyle.height);
			CGContextDrawImage(ctx, rectImg, img);
			CGContextRestoreGState(ctx);
		}
		else {
			CGContextDrawImage(ctx, rectImg, img);
		}
		CGContextFlush(ctx);
	#endif
	return(true);
}


#if defined(DEVICE_DESKTOP)
// format selection for desktop
CGLPixelFormatObj MacFindConfig(unsigned char preferredmode, bool fullscreen, unsigned char & stencil)
{
	for (int featuremask = 0x7; featuremask >= 0; featuremask--) // incrementally strip requirements
	{
		
		int attributes[] = 
		{
			kCGLPFAAccelerated,
			kCGLPFANoRecovery,
			kCGLPFADoubleBuffer,
			kCGLPFADepthSize, 16,			
			0,
			0,
			0,
			0,
			0,
			0
		};
		int head = 5;
		if ((featuremask & 4) && fullscreen)
		{
			attributes[head++] = NSOpenGLPFAFullScreen;
		}
		if ((featuremask & 1) && stencil)
		{
			attributes[head++] = NSOpenGLPFAStencilSize;
			attributes[head++] = 8;
		}
		if ((featuremask & 2) && preferredmode)
		{
			attributes[head++] = NSOpenGLPFAColorSize;
			attributes[head++] = preferredmode;
		}
		attributes[head] = 0;
		
		CGLPixelFormatObj format = NULL;
		GLint numFormats;
		CGLChoosePixelFormat((CGLPixelFormatAttribute*) &(attributes[0]), &format, &numFormats);
		if (format) 
		{
			if (!(featuremask & 1))
				stencil = 0;
			
			return format;
		}
	}
		return NULL;
}
#endif

//create OpenGL context, both versions
void MacCreateOpenGLContext(void *window, void *&context, GLuint &depthformat, GLuint &stencilformat, bool fullscreen, void *format, int api)
{
	context = NULL;
	
	#if defined(DEVICE_IPHONE)
		UIWindow *_window = (UIWindow *)window;
		UIView *view = [[_window subviews] objectAtIndex:0];
		CAEAGLLayer *eaglLayer = (CAEAGLLayer*)[view layer];
		depthformat = GL_DEPTH_COMPONENT16_OES;
		stencilformat = 0;
		if (api <= 0x101)
		{
			[eaglLayer setDrawableProperties:[NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat, nil]];
			format = kEAGLColorFormatRGB565;
			context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		}
		else {
			eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
			format = kEAGLColorFormatRGBA8;
			eaglLayer.opaque = YES;
			EAGLContext * tempcontext;
			tempcontext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
			
			if (!tempcontext || ![EAGLContext setCurrentContext:tempcontext]) {
				return;
			}
			
			GLuint testProgram = glCreateProgram();
			if (testProgram == 0)
				return;
			glDeleteProgram(testProgram);
			
			context = tempcontext;
		}
	
		#if defined(DEVICE_IPHONEEMU)
			stencilformat = GL_STENCIL_INDEX8_OES;			
		#else
			char device[32];
			MacGetDeviceModel(device);
			if( strcmp(device, "iPhone (iPhone2,") > 0 && strcmp(device, "iPhone (iPhone9,") < 0 )
				stencilformat = GL_DEPTH24_STENCIL8_OES;
			if( strcmp(device, "iPod touch (iPod3,") > 0 && strcmp(device, "iPod touch (iPod9,") < 0 )
				stencilformat = GL_DEPTH24_STENCIL8_OES;
		#endif

	#else
		NSWindow *_window = (NSWindow *)window;
		NSRect screenRect = [_window frame];
		unsigned char stencil = stencilformat;
		CGLPixelFormatObj pixelformat = MacFindConfig(24, fullscreen, stencil);
		stencilformat = stencil;
		if (!pixelformat) return;
	
		MacView * oldview = (MacView*) _window.contentView;
		NSOpenGLPixelFormat *_format = [[NSOpenGLPixelFormat alloc] initWithCGLPixelFormatObj:pixelformat];
		NSOpenGLView *view = [[MacViewGL alloc] initWithFrame:screenRect pixelFormat: _format instance:[oldview getInstance] delegate:oldview];
		NSOpenGLContext * _context = [[NSOpenGLContext alloc] initWithFormat:_format shareContext:nil];
		context = _context;
		[_window setContentView:view];
		[_window makeFirstResponder:view];
		format = _format;
	#endif
}

//create OpenGL surface, both versions
bool MacCreateOpenGLSurface(void *window, void *&windowsurface, void *context, GLuint &renderbuffer, GLuint &framebuffer, GLuint &depthbuffer, GLuint &depthformat, GLuint &stencilbuffer, GLuint &stencilformat, unsigned long &width, unsigned long &height)
{
	#if defined(DEVICE_IPHONE)
		UIWindow *_window = (UIWindow *)window;
		UIView *view = [[_window subviews] objectAtIndex:0];
		IPhoneView *iview = (IPhoneView *) view;
		float scale = [iview getPixelScale];
		
		if (scale != 1.0f && [view respondsToSelector:@selector(contentScaleFactor)])
		{
			view.contentScaleFactor = scale;
		}
		
		CAEAGLLayer* eaglLayer = (CAEAGLLayer*)[view layer];
		windowsurface = eaglLayer;				
		 
		GLuint oldRenderbuffer;
		GLuint oldFramebuffer;
		if(![EAGLContext setCurrentContext:(EAGLContext *)context])
			return false;
		
		glGetIntegerv(GL_RENDERBUFFER_BINDING_OES, (GLint *) &oldRenderbuffer);
		glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, (GLint *) &oldFramebuffer);
		
		EAGLContext * tempcontext = (EAGLContext *)context;
		GLint actualw, actualh;
	
		glGenFramebuffersOES(1, &framebuffer);
		glGenRenderbuffersOES(1, &renderbuffer);
		
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, framebuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderbuffer);
		[tempcontext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:eaglLayer];
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, renderbuffer);
			
		glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &actualw);
		glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &actualh);
	
		width = actualw;
		height = actualh;
		stencilbuffer = 0;
	
		#if defined(DEVICE_IPHONEEMU)
			if (depthformat)
			{
				glGenRenderbuffersOES(1, &depthbuffer);
				glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthbuffer);
				glRenderbufferStorageOES(GL_RENDERBUFFER_OES, depthformat, width, height);
				glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthbuffer);
			}
			if (stencilformat)
			{
				glGenRenderbuffersOES(1, &stencilbuffer);
				glBindRenderbufferOES(GL_RENDERBUFFER_OES, stencilbuffer);
				glRenderbufferStorageOES(GL_RENDERBUFFER_OES, stencilformat, width, height);
				glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_STENCIL_ATTACHMENT_OES, GL_RENDERBUFFER_OES, stencilbuffer);
			}
		#else
			if (depthformat)
			{
				glGenRenderbuffersOES(1, &depthbuffer);
				glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthbuffer);
				if (stencilformat)
					depthformat = GL_DEPTH24_STENCIL8_OES;
				glRenderbufferStorageOES(GL_RENDERBUFFER_OES, depthformat, width, height);
				glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthbuffer);
				if (stencilformat)
					glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_STENCIL_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthbuffer);
			}
		#endif

	
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE)
		{
			return false;
		}
	
		glViewport(0, 0, width, height);
		glScissor(0, 0, width, height);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, oldRenderbuffer);
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	#else
		// Mac OS
		GLuint oldRenderbuffer;
		GLuint oldFramebuffer;
		NSSize newSize;
	
		NSWindow *_window = (NSWindow *)window;
		NSOpenGLView *view = [_window contentView];
	
		EC_INSTANCEOBJ *inst = [(MacViewGL *)view getInstance];
		//GLint sizes[2] = {inst->curwinstyle.width, inst->curwinstyle.height};
		//CGLContextObj cglcontext = (CGLContextObj)[(NSOpenGLContext *)context CGLContextObj]; 
		//CGLSetParameter(cglcontext, kCGLCPSurfaceBackingSize, sizes);
	
		newSize = [view bounds].size;
		/*
		if (inst->curwinstyle.fullscreen)
		{
			CGLEnable(cglcontext, kCGLCESurfaceBackingSize);
			CGLSetFullScreen(cglcontext);
			width = inst->curwinstyle.width;
			height = inst->curwinstyle.height;
		} else {		
			
		}*/
		width = roundf(newSize.width);
		height = roundf(newSize.height);
		if (inst->curwinstyle.fullscreen)
		{
			inst->curwinstyle.width = width;
			inst->curwinstyle.height = height;
		}
	
		[view setOpenGLContext:(NSOpenGLContext *)context];

	
		glGetIntegerv(GL_RENDERBUFFER_BINDING_EXT, (GLint *)&oldRenderbuffer);
		glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, (GLint *) &oldFramebuffer);
		
		glGenRenderbuffersEXT(1, &renderbuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderbuffer);
		
		//if(![(NSOpenGLContext *)context renderbufferStorage:GL_RENDERBUFFER_EXT fromDrawable:eaglLayer])
		//{
		//	glDeleteRenderbuffersEXT(1, &renderbuffer);
		//	glBindRenderbufferEXT(GL_RENDERBUFFER_BINDING_EXT, oldRenderbuffer);
		//	return false;
		//}
		
		glGenFramebuffersEXT(1, &framebuffer);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, renderbuffer);
		if (depthformat)
		{
			glGenRenderbuffersEXT(1, &depthbuffer);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthbuffer);
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, depthformat, width, height);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthbuffer);
		}
		
		glViewport(0, 0, width, height);
		glScissor(0, 0, width, height);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, oldRenderbuffer);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	#endif
	
	return true;
}

//iphone close OpenGL
void MacCloseOpenGL(void *context, GLuint &renderbuffer, GLuint &framebuffer, GLuint &depthbuffer, GLuint &depthformat, GLuint &stencilbuffer, GLuint &stencilformat)
{
	#if defined(DEVICE_IPHONE)
		EAGLContext *oldContext = [EAGLContext currentContext];
		if (oldContext != context)
			[EAGLContext setCurrentContext:(EAGLContext *)context];
		if(depthformat)
		{
			glDeleteRenderbuffersOES(1, &depthbuffer);
			depthbuffer = 0;
		}
		glDeleteRenderbuffersOES(1, &renderbuffer);
		renderbuffer = 0;
		glDeleteFramebuffersOES(1, &framebuffer);
		framebuffer = 0;
		if (oldContext != context)
			[EAGLContext setCurrentContext:oldContext];
	#else
	#endif
}

//iphone flip method for OpenGL
void MacFlipOpenGL(void *windowsurface, void *&contextOpenGL, GLuint &renderbuffer)
{
	#if defined(DEVICE_IPHONE)
		EAGLContext *_contextOpenGL = (EAGLContext *)contextOpenGL;
		EAGLContext *oldContext = [EAGLContext currentContext];
		GLuint oldRenderbuffer;
		
		if(oldContext != _contextOpenGL)
			[EAGLContext setCurrentContext:_contextOpenGL];
	
		glGetIntegerv(GL_RENDERBUFFER_BINDING_OES, (GLint *) &oldRenderbuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderbuffer);
		
		[_contextOpenGL presentRenderbuffer:GL_RENDERBUFFER_OES];
	#else
		NSOpenGLContext * context = (NSOpenGLContext *)contextOpenGL;
		[context makeCurrentContext];
		[context flushBuffer];
	#endif
}

#if defined(DEVICE_IPHONE)

void InitEventBuffer(BUFFERTOUCH *buffer)
{
	int ctr;
	for(ctr = 0; ctr < MAX_MULTITOUCH; ctr++)
	{
		buffer[ctr].x = -1000;
		buffer[ctr].y = -1000;
		buffer[ctr].isInUse = false;
		buffer[ctr].isUpdated = false;
	}
}

//push event function
void PushEvent(TOUCHEVENT *qarray, TOUCHEVENT *pushevt, long &pushidx, long &ptridx)
{
	#if defined(DEVICE_IPHONE)
		//if (pushidx < MAX_IEVENTQUEUE || pushidx % MAX_IEVENTQUEUE != ptridx)
		if (pushidx > ptridx && pushidx >= ptridx + MAX_IEVENTQUEUE - 1)
		{
			qarray[pushidx % MAX_IEVENTQUEUE].cancelAll = true;
			pushidx++;
		}
		else if (!(pushidx > ptridx && pushidx >= ptridx + MAX_IEVENTQUEUE))
		{
			ecMemCpy(&qarray[pushidx % MAX_IEVENTQUEUE], pushevt, sizeof(TOUCHEVENT));
			qarray[pushidx % MAX_IEVENTQUEUE].cancelAll = false;
			pushidx++;
		}
	#endif
}

void MacGetLanguage(char *language)
{
	NSUserDefaults* defs = [NSUserDefaults standardUserDefaults];
	NSArray* languages = [defs objectForKey:@"AppleLanguages"];
	ecStrCpy(language, [[languages objectAtIndex:0] UTF8String]);
}

void MacGetLocale(char *locale)
{
	NSLocale *currentLocale = [NSLocale autoupdatingCurrentLocale];
	NSString *ocLocale;

	ocLocale = [currentLocale objectForKey:NSLocaleLanguageCode];
	ecStrCpy(locale, [ocLocale UTF8String]);
	ecStrCat(locale, "-");
	ocLocale = [currentLocale objectForKey:NSLocaleCountryCode];
	ecStrCat(locale, [ocLocale UTF8String]);
}

#endif

void MacGetBattery(unsigned char * level, unsigned char * status)
{
	*level = 255;
	*status = BATT_UNSUPPORTED;
	
	#if !defined(DEVICE_DESKTOP)
		UIDevice * device = [UIDevice currentDevice];
		if ([device.systemVersion compare: @"3"] == NSOrderedAscending)
			return; // version 3.0 required

		if (device.batteryMonitoringEnabled == NO)
			[device setBatteryMonitoringEnabled: YES];
	
		float batlevel = device.batteryLevel;
		if (batlevel < 0 || device.batteryState == UIDeviceBatteryStateUnknown)
		{
			*status = BATT_ERROR;
			return;
		}
		
		*level = (unsigned char)(batlevel * 255.0f);
		if (device.batteryState == UIDeviceBatteryStateCharging)
			*status = BATT_CHARGING;
		else if (device.batteryState == UIDeviceBatteryStateFull)
			*status = BATT_ACPOWERED;
		else if (device.batteryState == UIDeviceBatteryStateUnplugged)
			*status = BATT_BATTERYPOWERED;
		else
			*status = BATT_ERROR;
	#endif
}

///////////////////////////////////////////////////////////////////
// UIApplication implementation
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_IPHONE)
@implementation IPhoneFrame

//asks the delegate to open a resource identifier by URL
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
	if (!url)
		return FALSE;
	
	//Send native event
	ecEvent event;
	event.type = ECEV_NATIVE;
	event.param1 = IM_OPENURL;
	event.param2 = (ecParam)url;
	if (ecOnEvent(inst, &event))
		return TRUE;

	//Convert commandline to UTF8 string
	ecUnichar tcmdline[EC_MAXCMDLINE];
	ecUtf8ToStr(tcmdline, [[url absoluteString] UTF8String]);
	
	//Pass commandline event
	event.type = ECEV_SETCOMMANDLINE;
	event.param1 = (ecParam) &tcmdline;
	ecOnEvent(inst, &event);
	
	//Pass native event
	return TRUE;
}

//delegate function which is called right before the application will start
- (void)applicationDidFinishLaunching:(UIApplication *)application
{
	inst = (EC_INSTANCEOBJ *)global_userdata;

	isRunning = false;
	
	[application setIdleTimerDisabled:true];
	
	//Set up the window
    screenRect = [[UIScreen mainScreen] bounds];
	window = [[UIWindow alloc] initWithFrame:screenRect];
	//[window retain];
	inst->wnd = window;
	
    //Set up the view (only overriding DrawRect when not using OpenGL or things will break)
	if (EdgeGL_Use())
		view = [[[IPhoneView alloc] initWithFrame:screenRect :inst] autorelease];
	else
		view = [[[IPhoneViewSoftrast alloc] initWithFrame:screenRect :inst] autorelease];
	
	inst->view = view;
	if ([view respondsToSelector:@selector(contentScaleFactor)] && inst->curwinstyle.emulatelowres != 1)
	{
		CGFloat scale = [UIScreen mainScreen].scale;
		view.contentScaleFactor = 1.0f;
		[view setPixelScale:scale];
		//[view setScale:[UIScreen mainScreen].scale];
	}
	[window addSubview:view];
	[view setExclusiveTouch:true];
	[view setMultipleTouchEnabled:true];
	
	//Show the window 
	[window makeKeyAndVisible];
	
	//Enable accelerometer
	[[UIAccelerometer sharedAccelerometer] setDelegate:self];
	//[[UIAccelerometer sharedAccelerometer] updateInterval];

	//Send native event
	ecEvent event;
	event.type = ECEV_NATIVE;
	event.param1 = IM_CREATE;
	event.param2 = (ecParam)application;
	ecOnEvent(inst, &event);

	//Send event, continue startup code
	#if defined(DEVICE_IPHONESIM) || defined(IPHONE_SINGLETHREAD) || !defined(IPHONE_OPENGLTHREADFIX)
		event.type = ECEV_CREATEWINDOW;
		ecOnEvent(inst, &event);
	#else
		[view startThread:true];
	#endif
}

//delegate function which is called right before the application will stop
- (void)applicationWillTerminate:(UIApplication *)application
{
	ecEvent event;
	
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	io->sustainloop = false;
	[view stopThread:true];
	event.type = ECEV_ENDMAINLOOP;
	ecOnEvent(inst, &event);
//	ecInstDestroyWindow(inst);

	event.type = ECEV_DESTROYWINDOW;
	ecOnEvent(inst, &event);
	event.type = ECEV_DESTROYINSTANCE;
	ecOnEvent(inst, &event);
	ecInstDelete(inst);
}

// accelerometer listener
- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration {
	ecEvent event;
	event.type = ECEV_CHANGETILT;
	event.param1 = acceleration.x * 32768;
	event.param2 = acceleration.y * -32768;
	event.param3 = acceleration.z * -32768;
	
	if (event.param1 > 32768)
		event.param1 = 32768;
	else if(event.param1 < -32768)
		event.param1 = -32768;
	
	if (event.param2 > 32768)
		event.param2 = 32768;
	else if(event.param2 < -32768)
		event.param2 = -32768;
	
	if (event.param3 > 32768)
		event.param3 = 32768;
	else if(event.param3 < -32768)
		event.param3 = -32768;
		
	ecOnEvent(inst, &event);	
}

//event is thrown after going back from a phonecall, sms, other messages or back from locked screen
- (void)applicationDidBecomeActive:(UIApplication *)application
{
	//Pause UI
	[application endIgnoringInteractionEvents];
	if (isRunning)
		[view emptyTouchBuffer];
	else
		isRunning = true;
	
	//Add event to queue
	TOUCHEVENT touchevent;
	touchevent.event.type = ECEV_CHANGEFOCUS;
	touchevent.event.param1 = 0;
	PushEvent([view getEventQueue], &touchevent, *[view getEventPushIdx], *[view getEventPtrIdx]);
	
	//Direct event call (old)
	//ecEvent event;
	//event.type = ECEV_CHANGEFOCUS;
	//event.param1 = 0;
	//ecOnEvent(inst, &event);
}

//event is thrown before going to a phonecall, sms, other messages or to locked screen
- (void)applicationWillResignActive:(UIApplication *)application
{
	//Resume UI
	[application beginIgnoringInteractionEvents];
	
	//Add event to queue
	TOUCHEVENT touchevent;
	touchevent.event.type = ECEV_CHANGEFOCUS;
	touchevent.event.param1 = 2;
	PushEvent([view getEventQueue], &touchevent, *[view getEventPushIdx], *[view getEventPtrIdx]);
	
	//Direct event call (old)
	//ecEvent event;
	//event.type = ECEV_CHANGEFOCUS;
	//event.param1 = 2;
	//ecOnEvent(inst, &event);
}

//
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	//Continue launching
	[self applicationDidFinishLaunching:application];
	
	//Send native event
	ecEvent event;
	event.type = ECEV_NATIVE;
	event.param1 = IM_LAUNCHOPTIONS;
	event.param2 = (ecParam)launchOptions;
	if (ecOnEvent(inst, &event))
		return(TRUE);
	
	//Check notification and send event
	if ([[[UIDevice currentDevice] systemVersion] floatValue] >= 3.0f)
	{
		//Forward compatibility, "nice" solution doesn't work, so go ahead with the dirty one
		#if (DEVICE_IPHONE < 300)
			//extern NSString *const UIApplicationLaunchOptionsRemoteNotificationKey __attribute__ ((weak_import, weak));
			NSString *UIApplicationLaunchOptionsRemoteNotificationKey = @"UIApplicationLaunchOptionsRemoteNotificationKey";
		#endif
		
		//Check for launch options
		if ([launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey])
		{
			NSDictionary *remoteNotification = [launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey];
			event.type = ECEV_NATIVE;
			event.param1 = IM_STARTNOTIFICATION;
			event.param2 = (ecParam)remoteNotification;
			ecOnEvent(inst, &event);
		}
	}
	return(TRUE);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)remoteNotification
{
	ecEvent event;
	event.type = ECEV_NATIVE;
	event.param1 = IM_NOTIFICATION;
	event.param2 = (ecParam)remoteNotification;
	ecOnEvent(inst, &event);
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
	ecEvent event;
	event.type = ECEV_NATIVE;
	event.param1 = IM_REGNOTIFTOKEN;
	event.param2 = (ecParam)deviceToken;
	ecOnEvent(inst, &event);
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
	ecEvent event;
	event.type = ECEV_NATIVE;
	event.param1 = IM_REGNOTIFERROR;
	event.param2 = (ecParam)error;
	ecOnEvent(inst, &event);
}


//deallocate variables
- (void)dealloc
{
	[view release];
	[window release];
	[super dealloc];
}

@end


///////////////////////////////////////////////////////////////////
// View implementation
///////////////////////////////////////////////////////////////////

//iPhone view implementation
@implementation IPhoneView

//return the correct layer for 2D software or OpenGLES
+ (Class) layerClass
{
	if (EdgeGL_Use())
		return [CAEAGLLayer class];
	return [super layerClass];
}

//init with frame and EC_INSTANCEOBJ
- (id)initWithFrame:(CGRect)frame :(EC_INSTANCEOBJ *)instobj
{
	isThreadExited = true;
	inst = instobj;
	InitEventBuffer(touchBuffer);
	[self initWithFrame:frame];
	
	return self;
}

//Return event queue
- (TOUCHEVENT *)getEventQueue
{
	return eventqueue;
}

//Return pointer to queue push index
- (long *)getEventPushIdx
{
	return &event_pushidx;
}

//Return pointer to queue ptr index
- (long *)getEventPtrIdx
{
	return &event_ptridx;
}

//init with frame
- (id)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame])
	{
		screenRect = [[UIScreen mainScreen] bounds];
	}
	event_ptridx = 0;
	event_pushidx = 0;
	pixelscale = 1.0f;
	[[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(orientationChanged:) name:UIDeviceOrientationDidChangeNotification object:nil];	
	[self orientationChanged:nil];
	return self;
}

//Get the EC_INSTANCEOBJ
- (EC_INSTANCEOBJ *)getInstanceObject
{
	return inst;
}

//deallocate variables
- (void)dealloc
{
	[thread release];
	[super dealloc];
}

//Start the thread
- (void)startThread:(bool)executeThread
{
	#if defined(DEVICE_IPHONESIM) || defined(IPHONE_SINGLETHREAD) || !defined(IPHONE_OPENGLTHREADFIX)
		isRunning = true;
		thread = [[NSThread alloc] initWithTarget:self selector:@selector(runloop) object:nil];
		[thread start];
	#else
		if (executeThread)
		{
			isRunning = false;
			thread = [[NSThread alloc] initWithTarget:self selector:@selector(runloop) object:nil];
			[thread start];
		}
		else
			isRunning = true;
	#endif
}

//Stop the thread
- (void)stopThread:(bool)forcedQuit
{
	if (isRunning)
	{
		isRunning = false;
		#if !defined(IPHONE_SINGLETHREAD) && !defined(DEVICE_IPHONEEMU)
			if (forcedQuit)
				while (!isThreadExited)
					usleep(20 * 1000);
		#endif
	}
}

- (void)throwIdleEvent
{
	ecEvent event;
	event.type = ECEV_IDLE;
	ecOnEvent(inst, &event);
	#if defined(IPHONE_SINGLETHREAD)
		boolThreadLock = false; //[threadlock unlockWithCondition:0];
	#endif
}

- (void)throwEvent:(id)event
{
	#if defined(IPHONE_SINGLETHREAD)
		ecOnEvent(inst, [event getEvent]);
		[event release];
	#endif
}

//Mainloop (worker) thread
- (void)runloop
{
	//Start thread
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	threadlock = [[NSConditionLock alloc] initWithCondition:0];
	#if !defined(DEVICE_IPHONESIM) && !defined(IPHONE_SINGLETHREAD) && defined(IPHONE_OPENGLTHREADFIX)
		ecEvent event;
		event.type = ECEV_CREATEWINDOW;
		ecOnEvent(inst, &event);
	#endif
	isThreadExited = false;
	while(isRunning)
	{
		//Process queued events
		while (event_ptridx < event_pushidx)
		{
			//Pop event from queue
			TOUCHEVENT *touchevent = &eventqueue[event_ptridx % MAX_IEVENTQUEUE];
			int ctr = MAX_MULTITOUCH;
			event_ptridx++;
			
			//Handle events not related to touches (minimize/restore)
			if (touchevent->event.type == ECEV_CHANGEFOCUS)
			{
				#if defined(IPHONE_SINGLETHREAD)
					OCEvent *ev = [OCEvent alloc];
					[ev getEvent]->type = touchevent->event.type;
					[ev getEvent]->param1 = touchevent->event.param1;
					[ev getEvent]->param2 = touchevent->event.param2;
					[ev getEvent]->param3 = touchevent->event.param3;
					[self performSelectorOnMainThread:@selector(throwEvent:) withObject:ev waitUntilDone:true];
				#else
					ecOnEvent(inst, &touchevent->event);
				#endif
				continue;
			}

			//Handle cancel all event
			if (touchevent->cancelAll)
			{
				for (ctr = 0; ctr < MAX_MULTITOUCH; ctr++)
				{
					if (touchBuffer[ctr].isInUse)
					{
						touchBuffer[ctr].isInUse = false;

						#if defined(IPHONE_SINGLETHREAD)
							OCEvent *ev = [OCEvent alloc];
							[ev getEvent]->type = ECEV_CLICKUP;
							[ev getEvent]->param1 = ctr;
							[ev getEvent]->param2 = touchBuffer[ctr].x;
							[ev getEvent]->param3 = touchBuffer[ctr].y;
							[self performSelectorOnMainThread:@selector(throwEvent:) withObject:ev waitUntilDone:true];
						#else
							touchevent->event.type = ECEV_CLICKUP;
							touchevent->event.param1 = ctr;
							touchevent->event.param2 = touchBuffer[ctr].x;
							touchevent->event.param3 = touchBuffer[ctr].y;
							ecOnEvent(inst, &touchevent->event);
						#endif
					}
				}
				continue;
			}
			
			//Handle click events
			if (touchevent->event.type == ECEV_CLICKDOWN)
			{
				for (ctr = 0; ctr < MAX_MULTITOUCH; ctr++)
				{
					if (!touchBuffer[ctr].isInUse)
					{
						touchBuffer[ctr].x = touchevent->event.param2;
						touchBuffer[ctr].y = touchevent->event.param3;
						touchBuffer[ctr].isInUse = true;
						touchBuffer[ctr].isUpdated = true;
						break;
					}
				}
			}
			else if (touchevent->event.type == ECEV_CLICKMOVE)
			{
				for (ctr = 0; ctr < MAX_MULTITOUCH; ctr++)
				{
					if (touchBuffer[ctr].isInUse && touchBuffer[ctr].x == touchevent->prevX && touchBuffer[ctr].y == touchevent->prevY)
					{
						touchBuffer[ctr].x = touchevent->event.param2;
						touchBuffer[ctr].y = touchevent->event.param3;
						touchBuffer[ctr].isUpdated = true;
						break;
					}
				}
			}
			else if (touchevent->event.type == ECEV_CLICKUP)
			{
				for (ctr = 0; ctr < MAX_MULTITOUCH; ctr++)
				{
					if (touchBuffer[ctr].isInUse && touchBuffer[ctr].x == touchevent->event.param2 && touchBuffer[ctr].y == touchevent->event.param3)
					{
						touchBuffer[ctr].x = -1000;
						touchBuffer[ctr].y = -1000;
						touchBuffer[ctr].isInUse = false;
						touchBuffer[ctr].isUpdated = true;
						break;
					}
				}
			}
			
			//Send click event
			if (ctr < MAX_MULTITOUCH)
			{
				#if defined(IPHONE_SINGLETHREAD)
					OCEvent *ev = [OCEvent alloc];
					[ev getEvent]->type = touchevent->event.type;
					[ev getEvent]->param1 = ctr;
					[ev getEvent]->param2 = touchevent->event.param2;
					[ev getEvent]->param3 = touchevent->event.param3;
					[self performSelectorOnMainThread:@selector(throwEvent:) withObject:ev waitUntilDone:true];
				#else
					touchevent->event.param1 = ctr;
					ecOnEvent(inst, &touchevent->event);
				#endif
			}
			else if (touchevent->event.type == ECEV_CLICKMOVE || touchevent->event.type == ECEV_CLICKUP)
			{
				for (ctr = 0; ctr < MAX_MULTITOUCH; ctr++)
				{
					//check all the touchBuffers which are in use
					if (touchBuffer[ctr].isInUse)
					{
						touchBuffer[ctr].isInUse = false;
						//if the touchBuffer has been updated reset the update status
						if (touchBuffer[ctr].isUpdated)
						{
							touchBuffer[ctr].isUpdated = false;
						}					
						//if the touchBuffer hasn't been updated then the user must have released the touchscreen
						else
						{
							touchevent->event.type = ECEV_CLICKUP;
						}
						
						#if defined(IPHONE_SINGLETHREAD)
							OCEvent *ev = [OCEvent alloc];
							[ev getEvent]->type = touchevent->event.type;
							[ev getEvent]->param1 = ctr;
							[ev getEvent]->param2 = touchBuffer[ctr].x;
							[ev getEvent]->param3 = touchBuffer[ctr].y;
							[self performSelectorOnMainThread:@selector(throwEvent:) withObject:ev waitUntilDone:true];
						#else
							touchevent->event.param1 = ctr;
							touchevent->event.param2 = touchBuffer[ctr].x;
							touchevent->event.param3 = touchBuffer[ctr].y;
							ecOnEvent(inst, &touchevent->event);
						#endif
					}
				}
			}
			//else
			//	touchevent->event.param1 = -1;
		}
		event_ptridx %= MAX_IEVENTQUEUE;
		event_pushidx %= MAX_IEVENTQUEUE;
		
		//Throw idle event
		if (isRunning)
		{
			#if defined(DEVICE_IPHONEEMU)
				[self performSelectorOnMainThread:@selector(throwIdleEvent) withObject:nil waitUntilDone:true];
			#else
				#if defined(IPHONE_SINGLETHREAD)
					boolThreadLock = true;//[threadlock tryLockWhenCondition:0];
					[self performSelectorOnMainThread:@selector(throwIdleEvent) withObject:nil waitUntilDone:true];
					//[threadlock lockWhenCondition:0];
					while(boolThreadLock)
						usleep(1 * 1000);
				#else
					[self throwIdleEvent];
				#endif
			#endif
			EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
			if (io->loopsleep)
			{
				usleep(io->loopsleep * 1000);
				io->loopsleep = 0;
			}
		}
		
		//Synchronize with CGImage refresh
		if (isRunning)
		{
			#if !defined(IPHONE_SINGLETHREAD)
				if (!EdgeGL_Use())
				{
					bool b = true;
					while(b)
					{
						@synchronized(self)
						{
							if (isDrawn)
								b = false;
						}
					}
				}
			#endif
		}
	}
	
	//Exit and clean thread
	isThreadExited = true;
	[NSThread exit];
	[pool release];
}

//flip the surface to the screen
- (void)flipSurface:(CGImageRef)imgref :(CGRect *)rect
{
	self->img = imgref;
	self->rectImg = *rect;
	
	isDrawn = false;
	[self performSelectorOnMainThread:@selector(setNeedsDisplay) withObject:nil waitUntilDone:true];
}

//empty the multitouch buffer
- (void)emptyTouchBuffer
{
	int ctr = 0;
	for (ctr = 0; ctr < MAX_MULTITOUCH; ctr++)
	{
		if (touchBuffer[ctr].isInUse)
		{
			TOUCHEVENT touchevent;
			touchevent.event.type = ECEV_CLICKUP;
			touchevent.event.param1 = 0;
			touchevent.event.param2 = touchBuffer[ctr].x;
			touchevent.event.param3 = touchBuffer[ctr].y;
			
			touchevent.prevX = touchBuffer[ctr].x;
			touchevent.prevY = touchBuffer[ctr].y;
			
			PushEvent(eventqueue, &touchevent, event_pushidx, event_ptridx);
		}
	}
}

//redraw the screen
- (void)drawSoftwareRect:(CGRect)rect
{
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	if (pixelscale != 1.0f)	CGContextScaleCTM(ctx, 1.0f/pixelscale, 1.0f/pixelscale);
	if (img)
	{
		CGContextDrawImage(ctx, rectImg, img);
	}
	
	isDrawn = true;
}

// Handles the start of a touch
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches)
	{
		CGPoint point = [touch locationInView:self];

		TOUCHEVENT touchevent;
		touchevent.event.type = ECEV_CLICKDOWN;
		touchevent.event.param1 = 0;
		touchevent.event.param2 = point.x * pixelscale;
		touchevent.event.param3 = point.y * pixelscale;
		
		touchevent.prevX = -1000;
		touchevent.prevX = -1000;
		
		PushEvent(eventqueue, &touchevent, event_pushidx, event_ptridx);
		//ecOnEvent(inst, &event);
	}
}

// Handles the continuation of a touch.
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{  
	for (UITouch *touch in touches)
	{		
		CGPoint point = [touch locationInView:self];
		
		TOUCHEVENT touchevent;
		touchevent.event.type = ECEV_CLICKMOVE;
		touchevent.event.param1 = 0;
		touchevent.event.param2 = point.x * pixelscale;
		touchevent.event.param3 = point.y * pixelscale;
		
		CGPoint prevpoint = [touch previousLocationInView:self];
		
		touchevent.prevX = prevpoint.x * pixelscale;
		touchevent.prevY = prevpoint.y * pixelscale;
		
		PushEvent(eventqueue, &touchevent, event_pushidx, event_ptridx);
		//ecOnEvent(inst, &event);
	}
}

// Handles the end of a touch event.
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches)
	{
		CGPoint point = [touch locationInView:self];
		
		TOUCHEVENT touchevent;
		touchevent.event.type = ECEV_CLICKUP;
		touchevent.event.param1 = 0;
		touchevent.event.param2 = point.x * pixelscale;
		touchevent.event.param3 = point.y * pixelscale;
		
		CGPoint prevpoint = [touch previousLocationInView:self];
		
		touchevent.prevX = prevpoint.x * pixelscale;
		touchevent.prevY = prevpoint.y * pixelscale;

		PushEvent(eventqueue, &touchevent, event_pushidx, event_ptridx);
		//ecOnEvent(inst, &event);
	}
}

// Handles if a touch is cancelled
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	for (UITouch *touch in touches)
	{
		CGPoint point = [touch locationInView:self];
		
		TOUCHEVENT touchevent;
		touchevent.event.type = ECEV_CLICKUP;
		touchevent.event.param1 = 0;
		touchevent.event.param2 = point.x;
		touchevent.event.param3 = point.y;
		
		CGPoint prevpoint = [touch previousLocationInView:self];
		
		touchevent.prevX = prevpoint.x;
		touchevent.prevY = prevpoint.y;
		
		PushEvent(eventqueue, &touchevent, event_pushidx, event_ptridx);
	}
}

//event is thrown when the alert view is closed
- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	[self emptyTouchBuffer];
}

- (void)setPixelScale:(CGFloat)scale
{
	pixelscale = scale;
}

- (CGFloat)getPixelScale
{
	return pixelscale;
}

- (void)orientationChanged:(id)notification
{
	UIDeviceOrientation newOrientation = [[UIDevice currentDevice] orientation];
	ecEvent event;
	event.type = ECEV_CHANGEORIENTATION;
	event.param1 = -1;
	switch (newOrientation) {
		case UIDeviceOrientationLandscapeLeft:
			event.param2 = 1; break;
		case UIDeviceOrientationLandscapeRight:
			event.param2 = 3; break;
		case UIDeviceOrientationPortrait:
			event.param2 = 0; break;
		case UIDeviceOrientationPortraitUpsideDown:
			event.param2 = 2; break;
			
		case UIDeviceOrientationFaceDown:
			return;
		case UIDeviceOrientationFaceUp:
			return;
		case UIDeviceOrientationUnknown:
			return;
		default:
			return;
	}
	ecOnEvent(inst, &event);
}	

@end


@implementation IPhoneViewSoftrast

-(void)drawRect:(CGRect)rect
{
	[self drawSoftwareRect:rect];
}

@end



///////////////////////////////////////////////////////////////////
// Implementation of MAC classes
///////////////////////////////////////////////////////////////////

#elif defined(DEVICE_MAC) && defined(DEVICE_DESKTOP)

@implementation MacViewGL

- (id)initWithFrame:(NSRect)screenRect pixelFormat:(NSOpenGLPixelFormat *)_format instance:(EC_INSTANCEOBJ *)instobj delegate:(MacView *)originalview
{
	delegate = originalview;
	inst = instobj;
	id retval = [super initWithFrame:screenRect pixelFormat:_format];
	return retval;
}

- (EC_INSTANCEOBJ *) getInstance
{
	return inst;
}


- (BOOL)acceptsFirstResponder:(NSEvent *)theEvent {	return YES; }
- (BOOL)acceptsFirstMouse:(NSEvent *)theEvent {	return YES; }

- (void)keyDown:(NSEvent *)theEvent { [delegate keyDown:theEvent]; }
- (void)keyUp:(NSEvent *)theEvent { [delegate keyUp:theEvent]; }
- (void)mouseMoved:(NSEvent *)theEvent { [delegate mouseMoved:theEvent]; }
- (void)mouseDown:(NSEvent *)theEvent { [delegate mouseDown:theEvent]; }
- (void)mouseUp:(NSEvent *)theEvent { [delegate mouseUp:theEvent]; }
- (void)mouseDragged:(NSEvent *)theEvent { [delegate mouseDragged:theEvent]; }
- (void)rightMouseDown:(NSEvent *)theEvent { [delegate rightMouseDown:theEvent]; }
- (void)rightMouseUp:(NSEvent *)theEvent { [delegate rightMouseUp:theEvent]; }
- (void)rightMouseDragged:(NSEvent *)theEvent { [delegate rightMouseDragged:theEvent]; }
- (void)otherMouseDown:(NSEvent *)theEvent { [delegate otherMouseDown:theEvent]; }
- (void)otherMouseUp:(NSEvent *)theEvent { [delegate otherMouseUp:theEvent]; }
- (void)otherMouseDragged:(NSEvent *)theEvent {	[delegate otherMouseDragged:theEvent]; }

@end


@implementation MacView

- (BOOL)acceptsFirstResponder:(NSEvent *)theEvent
{
	return YES;
}

- (BOOL)acceptsFirstMouse:(NSEvent *)theEvent
{
	return YES;
}

- (id)initWithFrame:(NSRect)frame :(EC_INSTANCEOBJ *)instobj
{
	inst = instobj;
	return [self initWithFrame:frame];
}

- (EC_INSTANCEOBJ *) getInstance
{
	return inst;
}

- (void) translateMouseX:(ecParam *)x Y:(ecParam *)y
{
	if (!inst->curwinstyle.fullscreen) return;
	NSRect window = [[NSScreen mainScreen] frame];
	*x = (*x * inst->curwinstyle.width) / window.size.width;
	*y = (*y * inst->curwinstyle.height) / window.size.height;
}

//KEYBOARD
int translateKeycode(int orgkey)
{
	unsigned char key_translater[] = {
		65,  83,  68,  70,  72,  71,  90,  88,  67,  86, 167,  66,  81,  87,  69,  82,
		89,  84,  49,  50,  51,  52,  54,  53,  43,  57,  55,  45,  56,  48,  93,  79,
		85,  91,  73,  80,  13,  76,  74,  39,  75,  59,  92,  44,  47,  78,  77,  46,
		9,  32,  96,   8,   3,  27,  22,  26,  28,  25,  58,  41,  60,  24,  62,  63,
		64,  46,  29,  42,   2,  43,  28,  27,  31,  34,  38,  47,   3,  30,  45,  31,
		35,  61,  48,  49,  50,  51,  52,  53,  54,  55,   6,  56,  57,  30,  94,  44,
		116,  117,  118,  114,  119,  120,  16, 122,  16,  0,  16,  16,  16,  121,  16, 123,
		16,  16,  16,   5,   18,  11, 115,  17, 113,  19, 112,  28,  29,  31,  30,  30,
		128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
		144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
		160, 161, 162, 163, 164, 165, 166,  10, 168, 169, 170, 171, 172, 173, 174, 175,
		176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
		192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
		208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
		224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
		240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
	};
	if (orgkey >= 0 && orgkey < 256)
		orgkey = key_translater[orgkey];
	return(orgkey);
}

- (void)keyDown:(NSEvent *)theEvent
{
	ecEvent event;
	event.param1 = translateKeycode([theEvent keyCode]);
	event.type = ECEV_KEYDOWN;
	ecOnEvent(inst, &event);	
}

- (void)keyUp:(NSEvent *)theEvent
{
	ecEvent event;
	event.param1 = translateKeycode([theEvent keyCode]);
	event.type = ECEV_KEYUP;
	ecOnEvent(inst, &event);	
}

//MOUSE
- (void)mouseMoved:(NSEvent *)theEvent
{
	ecEvent event;
	event.type = ECEV_CLICKMOVE;
	event.param2 = [theEvent locationInWindow].x;
	event.param3 = [self bounds].size.height - [theEvent locationInWindow].y - 1;
	[self translateMouseX:&event.param2 Y:&event.param3];
	ecOnEvent(inst, &event);
}

- (void)mouseDown:(NSEvent *)theEvent
{
	ecEvent event;
	event.type = ECEV_CLICKDOWN;
	event.param1 = 0;
	event.param2 = [theEvent locationInWindow].x;
	event.param3 = [self bounds].size.height - [theEvent locationInWindow].y - 1;
	[self translateMouseX:&event.param2 Y:&event.param3];
	ecOnEvent(inst, &event);
}

- (void)mouseUp:(NSEvent *)theEvent
{
	ecEvent event;
	event.type = ECEV_CLICKUP;
	event.param1 = 0;
	event.param2 = [theEvent locationInWindow].x;
	event.param3 = [self bounds].size.height - [theEvent locationInWindow].y - 1;
	[self translateMouseX:&event.param2 Y:&event.param3];
	ecOnEvent(inst, &event);
}

- (void)mouseDragged:(NSEvent *)theEvent
{
	ecEvent event;
	event.type = ECEV_CLICKMOVE;
	event.param1 = 0;
	event.param2 = [theEvent locationInWindow].x;
	event.param3 = [self bounds].size.height - [theEvent locationInWindow].y - 1;
	[self translateMouseX:&event.param2 Y:&event.param3];	
	ecOnEvent(inst, &event);
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
	ecEvent event;
	event.type = ECEV_CLICKDOWN;
	event.param1 = 1;
	event.param2 = [theEvent locationInWindow].x;
	event.param3 = [self bounds].size.height - [theEvent locationInWindow].y - 1;
	[self translateMouseX:&event.param2 Y:&event.param3];	
	ecOnEvent(inst, &event);
}

- (void)rightMouseUp:(NSEvent *)theEvent
{
	ecEvent event;
	event.type = ECEV_CLICKUP;
	event.param1 = 1;
	event.param2 = [theEvent locationInWindow].x;
	event.param3 = [self bounds].size.height - [theEvent locationInWindow].y - 1;
	[self translateMouseX:&event.param2 Y:&event.param3];	
	ecOnEvent(inst, &event);
}

- (void)rightMouseDragged:(NSEvent *)theEvent
{
	ecEvent event;
	event.type = ECEV_CLICKMOVE;
	event.param1 = 1;
	event.param2 = [theEvent locationInWindow].x;
	event.param3 = [self bounds].size.height - [theEvent locationInWindow].y - 1;
	[self translateMouseX:&event.param2 Y:&event.param3];	
	ecOnEvent(inst, &event);
}

- (void)otherMouseDown:(NSEvent *)theEvent
{
	ecEvent event;
	event.type = ECEV_CLICKDOWN;
	event.param1 = 2;
	event.param2 = [theEvent locationInWindow].x;
	event.param3 = [self bounds].size.height - [theEvent locationInWindow].y - 1;
	[self translateMouseX:&event.param2 Y:&event.param3];	
	ecOnEvent(inst, &event);
}

- (void)otherMouseUp:(NSEvent *)theEvent
{
	ecEvent event;
	event.type = ECEV_CLICKUP;
	event.param1 = 2;
	event.param2 = [theEvent locationInWindow].x;
	event.param3 = [self bounds].size.height - [theEvent locationInWindow].y - 1;
	[self translateMouseX:&event.param2 Y:&event.param3];	
	ecOnEvent(inst, &event);
}

- (void)otherMouseDragged:(NSEvent *)theEvent
{
	ecEvent event;
	event.type = ECEV_CLICKMOVE;
	event.param1 = 2;
	event.param2 = [theEvent locationInWindow].x;
	event.param3 = [self bounds].size.height - [theEvent locationInWindow].y - 1;
	[self translateMouseX:&event.param2 Y:&event.param3];	
	ecOnEvent(inst, &event);
}


@end


@implementation MacWindow

-(BOOL)canBecomeKeyWindow
{
	return YES;
}

@end


//Mac implementation
@implementation MacFrame

@synthesize inst;

//APPLICATION
- (void) applicationWillFinishLaunching: (NSNotification *) notification
{
	inst = (EC_INSTANCEOBJ *)global_userdata;
	NSRect screenRect = [[NSScreen mainScreen] frame];
	glcontext = NULL;
	
	inst->screenx = roundf(screenRect.size.width);
	inst->screeny = roundf(screenRect.size.height);
	
	//create window
	if(inst->curwinstyle.fullscreen)
	{
		window = [[MacWindow alloc] initWithContentRect: screenRect
											 styleMask: NSBorderlessWindowMask
											   backing: NSBackingStoreBuffered
												 defer: true];	
	}
	else 
	{
		window = [[MacWindow alloc] initWithContentRect: NSMakeRect(screenRect.size.width / 2 - inst->curwinstyle.width / 2, screenRect.size.height / 2 - inst->curwinstyle.height / 2, inst->curwinstyle.width, inst->curwinstyle.height)
								   styleMask: (NSTitledWindowMask |
											   NSMiniaturizableWindowMask |
											   NSClosableWindowMask)
								   backing: NSBackingStoreNonretained
								   defer: true];
	}
	view = [[MacView alloc] initWithFrame:screenRect :inst];
		
	inst->wnd = window;
	inst->app = self;
	
	char titlec[256];
	ecStrCpy(titlec, inst->curwinstyle.title, 256);
	NSString *title = [[NSString alloc] initWithCString:titlec];
	
	//set view
	//[view needsPanelToBecomeKey];
	
	//set window
	[window setTitle: title];
	[window contentRectForFrameRect:screenRect];
	[window makeKeyWindow];
	[window setContentView:view];
	[window makeFirstResponder:view];
	[window setAcceptsMouseMovedEvents:true];
	[window setDelegate:self];
	if(inst->curwinstyle.fullscreen)
	{
		[window setOpaque:YES];
		[window setHidesOnDeactivate:YES];
		[window setLevel:NSMainMenuWindowLevel+1];
		[window makeKeyAndOrderFront:self];
		[window makeKeyWindow];	
	}
}

- (void) applicationDidFinishLaunching: (NSNotification *) notification
{
	//show window
	[window makeKeyAndOrderFront:self];
	
	ecEvent event;
	event.type = ECEV_CREATEWINDOW;
	ecOnEvent(inst, &event);
	
	//create timer to keep application running
	timer = [NSTimer scheduledTimerWithTimeInterval:(1.0/60.0) target:self selector:@selector(tick) userInfo:nil repeats:true];
	
}


//WINDOW
- (BOOL)windowShouldClose:(id)windows
{
	ecEvent event;
	event.type = ECEV_DESTROYWINDOW;
	ecOnEvent(inst, &event);
	
	return true;
}

- (void)windowWillClose:(NSNotification *)notification
{
	ecEvent event;
	event.type = ECEV_DESTROYINSTANCE;
	ecOnEvent(inst, &event);
	
	[[NSApplication sharedApplication] terminate:NULL];
}

/*
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
    return YES;
}
*/

- (void)windowWillMiniaturize:(NSNotification *)notification
{
	ecEvent event;
	event.type = ECEV_CHANGEFOCUS;
	event.param1 = 2;
	ecOnEvent(inst, &event);
}

- (void)windowDidDeminiaturize:(NSNotification *)notification
{
	ecEvent event;
	event.type = ECEV_CHANGEFOCUS;
	event.param1 = 0;
	ecOnEvent(inst, &event);
}

- (void)tick {
	ecEvent event;
	event.type = ECEV_IDLE;
	ecOnEvent(inst, &event);	
}

- (CGContextRef) GetContext {
	return (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
}

- (void) dealloc
{
	CGImageRelease(img);
	[window release];
	[view release];
	[super dealloc];
}

@end
#endif