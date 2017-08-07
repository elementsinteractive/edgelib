// iphonecamera.h
// iPhone camera class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

@interface IPhoneCamera : NSObject < UIImagePickerControllerDelegate, UINavigationControllerDelegate > {
}

- (void)timerCallback:(NSTimer*)timer;

@end
