# Microsoft Developer Studio Project File - Name="eventcore" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=eventcore - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "eventcore.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "eventcore.mak" CFG="eventcore - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "eventcore - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "eventcore - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "eventcore - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../../devlib/include" /I "../../../devlib/source" /I "../../../extsdk/zlib/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "NDEBUG"
# ADD RSC /l 0x413 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /libpath:"../../lib/pc-x86" /libpath:"../../lib/pc-x86/opengl"

!ELSEIF  "$(CFG)" == "eventcore - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../devlib/source" /I "../../../extsdk/zlib/include" /I "../../../devlib/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "_DEBUG"
# ADD RSC /l 0x413 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"../../../dist/lib/pc-x86" /libpath:"../../../dist/lib/pc-x86/opengl"

!ENDIF 

# Begin Target

# Name "eventcore - Win32 Release"
# Name "eventcore - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "edgecore c"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\devlib\source\core\cdisplay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\cevent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\cfile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\chelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\cmemory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\cnet.cpp
# End Source File
# End Group
# Begin Group "edgelib c"

# PROP Default_Filter ""
# Begin Group "extapi c"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\devlib\source\extapi_data.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\extapi_net.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\extapi_xml.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\devlib\source\edgeframe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_connect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_console.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_device.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_dsp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_file.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_ini.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_input.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_math.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_memory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_raw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_register.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_sound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_std.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_ui.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\emod_vtimer.cpp
# End Source File
# End Group
# Begin Group "loader c"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\devlib\source\loader\eldr_3ds.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\loader\eldr_base.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\loader\eldr_bmp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\loader\eldr_dae.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\loader\eldr_e3d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\loader\eldr_gif.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\loader\eldr_jpg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\loader\eldr_ms3d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\loader\eldr_png.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\loader\eldr_tga.cpp
# End Source File
# End Group
# Begin Group "surface c"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\devlib\source\surface\esurf_2d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\surface\esurf_2dbase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\surface\esurf_2drgba.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\surface\esurf_3d.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\cmain.cpp
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "edgecore h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\devlib\source\core\edgecore.h
# End Source File
# End Group
# Begin Group "extapi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\devlib\source\core\extapi\appleframe.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\extapi\appleinstanceobj.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\extapi\imp_bluetooth.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\extapi\imp_ddraw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\extapi\imp_dspchange.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\extapi\imp_gx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\extapi\imp_powerapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\extapi\imp_symbiantilt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\extapi\parsesdp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\extapi\symbianbtapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\extapi\symbiandsa.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\core\extapi\symbianframe.h
# End Source File
# End Group
# Begin Group "edgelib h"

# PROP Default_Filter ""
# Begin Group "extapi h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\devlib\include\extapi\dataapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\extapi\netapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\extapi\xmlapi.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\devlib\include\econnect.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\econsole.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\edevice.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\edgeframe.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\edisplay.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\efile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\eini.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\einput.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\eloader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\emath.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\ememory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\eraw.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\ereg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\esound.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\estdc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\esurface.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\eui.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\evtimer.h
# End Source File
# End Group
# Begin Group "extra"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\devlib\source\extra\exblit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\extra\exblitfast.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\extra\excoll.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\extra\exmacro.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\extra\exrect.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\extra\keynames.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\source\extra\verinfo.h
# End Source File
# End Group
# Begin Group "glplugin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\devlib\include\extapi\gldummy.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\extlib\opengl_plugin\wrap_gl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\extlib\opengl_plugin\wrap_gles.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\extlib\opengl_plugin\wrap_glesmac.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\extlib\opengl_plugin\wrap_gpu940.h
# End Source File
# End Group
# Begin Group "esound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\devlib\include\esound\hekkus.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\esound\openal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\esound\silent.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\devlib\include\edge.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\edgedef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\edgedevice.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\edgemain.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\edgerender.h
# End Source File
# Begin Source File

SOURCE=..\..\..\devlib\include\edgesdkdef.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# End Group
# End Target
# End Project
