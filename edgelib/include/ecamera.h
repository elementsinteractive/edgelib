// ecamera.h
// A module for using the video camera
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED     //Can only be included by the main engine
#ifndef ECAMERA_INCLUDED  //Make sure the module isn't included multiple times
#define ECAMERA_INCLUDED

// Camera locations
#define ELOC_REAR -1
#define ELOC_FRONT -2
#define ELOC_UNKNOWN -3

// Camera orientations
#define ECO_XFLIP 1
#define ECO_YFLIP 2
#define ECO_SWAPAXIS 4

// Camera statuses
#define ECS_INVALID 0
#define ECS_READY 1
#define ECS_RECORDING 2

class ClassECamera
{
    friend class ClassEdge;
    public:

        ClassECamera(ClassEdge *);
        ~ClassECamera();

        int GetCameraCount();
        ERESULT RequestCamera(int);
        ERESULT RequestStart();
        ERESULT RequestStop();

        int GetModeCount();
        void GetModeInformation(int, E_CAMERAMODE &);
        ERESULT RequestResolution(int);
        ERESULT RequestResolution(E_CAMERAMODE &);

        void GetCurrentMode(E_CAMERAMODE &);
        int GetCurrentModeIndex();
        int GetCameraOrientation();
        int GetCameraState();
        int GetCameraIndex();

        static void FrameToSurface(const E_CAMERAFRAME *, E2DSurface *);
        static void FrameToSurfaceAspect(const E_CAMERAFRAME *, E2DSurface *);


    private:
        ClassEdge * framework;
        ECOREOBJ cameraclass;

}; // The camera class



#endif
#endif
