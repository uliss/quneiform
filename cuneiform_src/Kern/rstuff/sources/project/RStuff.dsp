# Microsoft Developer Studio Project File - Name="RStuff" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RStuff - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RStuff.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RStuff.mak" CFG="RStuff - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RStuff - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RStuff - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RStuff - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\bin"
# PROP Intermediate_Dir "..\..\..\Release\RStuff"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RSTUFF_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\..\..\h" /I "..\..\..\hhh" /I "..\..\..\usage" /I "..\..\..\usage\almi" /I "..\..\..\hrk" /I "..\..\..\hdebug" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RSTUFF_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 user32.lib ccom.lib cline.lib /nologo /dll /machine:I386 /libpath:"..\..\..\bin"

!ELSEIF  "$(CFG)" == "RStuff - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\bind"
# PROP Intermediate_Dir "..\..\..\Debug\RStuff"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RSTUFF_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I "..\..\..\h" /I "..\..\..\hhh" /I "..\..\..\usage" /I "..\..\..\usage\almi" /I "..\..\..\hrk" /I "..\..\..\hdebug" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__NT__" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 user32.lib ccom.lib cline.lib /nologo /dll /incremental:no /debug /machine:I386 /libpath:"..\..\..\bind"

!ENDIF 

# Begin Target

# Name "RStuff - Win32 Release"
# Name "RStuff - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\Hdebug\__Snp.c
# End Source File
# Begin Source File

SOURCE=..\Main\AboutLines.AboutLin.CPP
# End Source File
# Begin Source File

SOURCE=..\Main\AboutLines.Buffer.CPP
# End Source File
# Begin Source File

SOURCE=..\..\..\USAGE\AMU_Geom.CPP
# End Source File
# Begin Source File

SOURCE=..\Main\AutoTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\Main\Binarise.cpp
# End Source File
# Begin Source File

SOURCE=..\Main\DeleteLines.cpp
# End Source File
# Begin Source File

SOURCE=..\Main\dll.cpp
# End Source File
# Begin Source File

SOURCE=..\Main\Layout.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\USAGE\Line_AM_Util.CPP
# End Source File
# Begin Source File

SOURCE=..\..\..\USAGE\Line_VP_2_AM.CPP
# End Source File
# Begin Source File

SOURCE=..\..\..\USAGE\Line_VP_Util.CPP
# End Source File
# Begin Source File

SOURCE=..\Main\LineKiller.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\USAGE\MarkDataOper.CPP
# End Source File
# Begin Source File

SOURCE=..\Main\Normalise.cpp
# End Source File
# Begin Source File

SOURCE=..\Main\Ocomp.cpp
# End Source File
# Begin Source File

SOURCE=..\Main\OrtoMove.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\USAGE\Puma_Err.CPP
# End Source File
# Begin Source File

SOURCE=..\Main\RSMemory.cpp
# End Source File
# Begin Source File

SOURCE=..\Main\RSTimer.cpp
# End Source File
# Begin Source File

SOURCE=..\Main\RStuff.cpp
# End Source File
# Begin Source File

SOURCE=..\Main\RStuff.rc
# End Source File
# Begin Source File

SOURCE=..\..\..\USAGE\UN_Buff.CPP
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Main\AboutLines.Buffer.H
# End Source File
# Begin Source File

SOURCE=..\Main\AboutLines.Linedesc.h
# End Source File
# Begin Source File

SOURCE=..\Main\AutoTemplate.h
# End Source File
# Begin Source File

SOURCE=..\Main\resource.h
# End Source File
# Begin Source File

SOURCE=..\Main\RSDefines.h
# End Source File
# Begin Source File

SOURCE=..\Main\RSFunc.h
# End Source File
# Begin Source File

SOURCE=..\Main\RSGlobalData.h
# End Source File
# Begin Source File

SOURCE=..\Main\RSMemory.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
