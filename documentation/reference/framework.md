:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Framework

EDGELIB uses a special program entry function and a main wrapper class. These must be created in order to use most of the EDGELIB modules and functionality. The program entry function replaces any OS-specific program entry function (like `WinMain`).

## Using the EDGELIB framework
Games and applications powered by EDGELIB should try to communicate exclusively with the engine as much as possible to ease multi-platform development. The game also should contain a wrapper class to inherit the functionality and gameloop from EDGELIB. The instance to this class should be created as a pointer and allocated using the `new` operator. After that, return it.

## The main entry function and framework class
* [Main entry function: EdgeMain](framework_edgemain.md)
* [ClassMain: Public internal variable list](framework_classmain_publicinternalvariablelist.md)
* [ClassMain (constructor)](framework_classmain_constructor.md)
* [FreeRecording](framework_freerecording.md)
* [GetFPS](framework_getfps.md)
* [GetRecordingData](framework_getrecordingdata.md)
* [GetRecordingDataSize](framework_getrecordingdatasize.md)
* [GetSoftlabelPosition](framework_getsoftlabelposition.md)
* [GetWindowHandle](framework_getwindowhandle.md)
* [GetWnd (deprecated)](framework_getwnd.md)
* [Log](framework_log.md)
* [Minimize](framework_minimize.md)
* [OnButtonDown](framework_onbuttondown.md)
* [OnButtonUp](framework_onbuttonup.md)
* [OnCameraChange](framework_oncamerachange.md)
* [OnCameraFrame](framework_oncameraframe.md)
* [OnCreate (deprecated)](framework_oncreate.md)
* [OnDeviceTilt](framework_ondevicetilt.md)
* [OnDisplayChange](framework_ondisplaychange.md)
* [OnContextLost](framework_oncontextlost.md)
* [OnDisplayConfig](framework_ondisplayconfig.md)
* [OnDisplayInit](framework_ondisplayinit.md)
* [OnExit](framework_onexit.md)
* [OnFrameworkConfig](framework_onframeworkconfig.md)
* [OnInit](framework_oninit.md)
* [OnMessage (deprecated)](framework_onmessage.md)
* [OnMinimize](framework_onminimize.md)
* [OnNativeEvent](framework_onnativeevent.md)
* [OnNetMessage](framework_onnetmessage.md)
* [OnNextFrame](framework_onnextframe.md)
* [OnPluginLoad](framework_onpluginload.md)
* [OnRestore](framework_onresume.md)
* [OnStylusDouble](framework_onstylusdouble.md)
* [OnStylusDown](framework_onstylusdown.md)
* [OnStylusMove](framework_onstylusmove.md)
* [OnStylusUp](framework_onstylusup.md)
* [OnWheel](framework_onwheel.md)
* [PlayRecording](framework_playrecording.md)
* [Quit](framework_quit.md)
* [SetErrorMsg](framework_seterrormsg.md)
* [SetStatusMsg](framework_setstatusmsg.md)
* [Start](framework_start.md)
* [StartRecording](framework_startrecording.md)
* [StartTiltEmulator](framework_starttiltemulator.md)
* [StopRecording](framework_stoprecording.md)
* [~ClassMain (destructor)](framework_classmain_destructor.md)

