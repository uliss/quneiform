# Microsoft Developer Studio Project File - Name="RVERLINE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RVERLINE - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RVerLine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RVerLine.mak" CFG="RVERLINE - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RVERLINE - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RVERLINE - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RVERLINE - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "..\..\Release\RVERLINE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\src" /I "..\..\hhh" /I "..\..\hh" /I "..\..\h" /I "..\src\H" /I "..\src\USAGE" /I "..\..\hrk" /I "..\..\hdebug" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 cline.lib ccom.lib user32.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\..\bin"
# SUBTRACT LINK32 /incremental:yes /debug

!ELSEIF  "$(CFG)" == "RVERLINE - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bind"
# PROP Intermediate_Dir "..\..\debug\RVERLINE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I "..\src" /I "..\..\hhh" /I "..\..\hh" /I "..\..\h" /I "..\src\H" /I "..\src\USAGE" /I "..\..\hrk" /I "..\..\hdebug" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 user32.lib cline.lib ccom.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /libpath:"..\..\bind" /VERBOSE:LIB
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "RVERLINE - Win32 Release"
# Name "RVERLINE - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\..\Hdebug\__Snp.c
# End Source File
# Begin Source File

SOURCE=..\SRC\USAGE\AMU_Geom.CPP
# End Source File
# Begin Source File

SOURCE=..\SRC\ALGORITM\By_Image.CPP
# End Source File
# Begin Source File

SOURCE=..\SRC\USAGE_PUMA\Pum_Comm.CPP
# End Source File
# Begin Source File

SOURCE=..\SRC\ALGORITM\VERIFYLN.CPP
# End Source File
# Begin Source File

SOURCE=..\SRC\SERVICE\VL_Data.CPP
# End Source File
# Begin Source File

SOURCE=..\SRC\ROOT\VL_Kern.CPP
# End Source File
# Begin Source File

SOURCE=..\SRC\ROOT\VL_Resid.CPP
# End Source File
# Begin Source File

SOURCE=..\SRC\SERVICE\VL_Rule.CPP
# End Source File
# Begin Source File

SOURCE=..\SRC\SERVICE\VL_Snap.CPP
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\SRC\USAGE\AM_Buff.H
# End Source File
# Begin Source File

SOURCE=..\SRC\USAGE\AM_Comm.H
# End Source File
# Begin Source File

SOURCE=..\SRC\USAGE\AM_Err.H
# End Source File
# Begin Source File

SOURCE=..\SRC\USAGE\AMT_Geom.H
# End Source File
# Begin Source File

SOURCE=..\SRC\H\MyRaster.H
# End Source File
# Begin Source File

SOURCE=..\SRC\H\VL_Rule.H
# End Source File
# Begin Source File

SOURCE=..\SRC\H\VL_Snap.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
