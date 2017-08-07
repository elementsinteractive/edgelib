# Microsoft Developer Studio Project File - Name="edge" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=edge - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "edge.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "edge.mak" CFG="edge - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "edge - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "edge - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "edge - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../../extsdk/zlib/include" /I "../../source" /I "../../include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x413 /d "NDEBUG"
# ADD RSC /l 0x413 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\dist\lib\pc-x86\edge.lib"

!ELSEIF  "$(CFG)" == "edge - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../" /I "../../_import/zlib/include" /I "../../include" /I "../../../extsdk/zlib/include" /I "../../source" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x413 /d "_DEBUG"
# ADD RSC /l 0x413 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "edge - Win32 Release"
# Name "edge - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\core\ccamera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\core\cdisplay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\core\cevent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\core\cfile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\core\chelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\core\cmemory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\core\cnet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\core\cthread.cpp
# End Source File
# End Group
# Begin Group "api"

# PROP Default_Filter ""
# Begin Group "ext"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\extapi_data.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\extapi_xml.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\source\edgeframe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_camera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_connect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_console.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_device.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_dsp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_file.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_ini.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_input.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_math.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_memory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_raw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_register.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_sound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_std.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\emod_vtimer.cpp
# End Source File
# End Group
# Begin Group "loader"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\loader\eldr_3ds.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\loader\eldr_base.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\loader\eldr_bmp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\loader\eldr_dae.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\loader\eldr_e3d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\loader\eldr_gif.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\loader\eldr_jpg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\loader\eldr_ms3d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\loader\eldr_png.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\loader\eldr_pvr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\loader\eldr_tga.cpp
# End Source File
# End Group
# Begin Group "surface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\source\surface\esurf_2d.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\surface\esurf_2dbase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\surface\esurf_2ddisplay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\surface\esurf_2drgba.cpp
# End Source File
# Begin Source File

SOURCE=..\..\source\surface\esurf_3d.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
