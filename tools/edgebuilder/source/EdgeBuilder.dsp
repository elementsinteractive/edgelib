# Microsoft Developer Studio Project File - Name="EdgeBuilder" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=EdgeBuilder - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EdgeBuilder.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EdgeBuilder.mak" CFG="EdgeBuilder - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EdgeBuilder - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "EdgeBuilder - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EdgeBuilder - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "." /I "../../../dist/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "NDEBUG"
# ADD RSC /l 0x413 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /libpath:"../../../dist/lib/pc-x86"

!ELSEIF  "$(CFG)" == "EdgeBuilder - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "." /I "../../../dist/include" /I "../../../libini/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "_DEBUG"
# ADD RSC /l 0x413 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"../../../dist/lib/pc-x86" /libpath:"../../../libini/lib/vc"

!ENDIF 

# Begin Target

# Name "EdgeBuilder - Win32 Release"
# Name "EdgeBuilder - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\app.cpp
# End Source File
# Begin Source File

SOURCE=.\resource.rc

!IF  "$(CFG)" == "EdgeBuilder - Win32 Release"

# ADD BASE RSC /l 0x413
# ADD RSC /l 0x409

!ELSEIF  "$(CFG)" == "EdgeBuilder - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "edge"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\econnect.h
# End Source File
# Begin Source File

SOURCE=..\..\include\econsole.h
# End Source File
# Begin Source File

SOURCE=..\..\include\edevice.h
# End Source File
# Begin Source File

SOURCE=..\..\include\edge.h
# End Source File
# Begin Source File

SOURCE=..\..\include\edgedef.h
# End Source File
# Begin Source File

SOURCE=..\..\include\edgemain.h
# End Source File
# Begin Source File

SOURCE=..\..\include\edisplay.h
# End Source File
# Begin Source File

SOURCE=..\..\include\efile.h
# End Source File
# Begin Source File

SOURCE=..\..\include\eini.h
# End Source File
# Begin Source File

SOURCE=..\..\include\emath.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ememory.h
# End Source File
# Begin Source File

SOURCE=..\..\include\eraw.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ereg.h
# End Source File
# Begin Source File

SOURCE=..\..\include\estdc.h
# End Source File
# Begin Source File

SOURCE=..\..\include\esurface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\evtimer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\compiler.h
# End Source File
# Begin Source File

SOURCE=.\def_main.h
# End Source File
# Begin Source File

SOURCE=.\env.h
# End Source File
# Begin Source File

SOURCE=.\filetree.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\folderclose.bmp
# End Source File
# Begin Source File

SOURCE=.\res\folderopen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\folderroot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\headerfile.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\main.ico
# End Source File
# Begin Source File

SOURCE=.\res\project.bmp
# End Source File
# Begin Source File

SOURCE=.\res\resfolderclose.bmp
# End Source File
# Begin Source File

SOURCE=.\res\resfolderopen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\resourcefile.bmp
# End Source File
# Begin Source File

SOURCE=.\res\resourceroot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sourcefile.bmp
# End Source File
# End Group
# End Target
# End Project
