# Microsoft Developer Studio Project File - Name="CFIO" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CFIO - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CFIO.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CFIO.mak" CFG="CFIO - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CFIO - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CFIO - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CFIO - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Bin\Lang"
# PROP Intermediate_Dir "..\..\..\Release\CFIO"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\h" /I "..\..\..\h" /I "..\..\..\hhh" /I "..\..\..\hrk" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_WINDLL" /D "_AFXDLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /i ".\H" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /machine:I386 /implib:"..\..\..\Bin/CFIO.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CFIO - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\Bind"
# PROP Intermediate_Dir "..\..\..\Debug\CFIO"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I "..\h" /I "..\..\..\h" /I "..\..\..\hhh" /I "..\..\..\hrk" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /i "..\H" /i "..\..\H" /i "..\..\..\H" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386

!ENDIF 

# Begin Target

# Name "CFIO - Win32 Release"
# Name "CFIO - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\cpp\CTCBase.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\CTCControl.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\CTCFile.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\CTCMemory.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\CTCStorage.cpp
# End Source File
# Begin Source File

SOURCE=..\cpp\Dll_Main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\H\CTCBaseClasses.h
# End Source File
# Begin Source File

SOURCE=..\H\CTCClasses.h
# End Source File
# Begin Source File

SOURCE=..\H\CTCControl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\cpp\Cfio_Eng.rc

!IF  "$(CFG)" == "CFIO - Win32 Release"

# ADD BASE RSC /l 0x419 /i "\—борка\Puma\CFIO\SOURCES\cpp" /i "\projects\puma\vss_rk\CFIO\SOURCES\cpp" /i "\Puma_VSS\CFIO\SOURCES\cpp"
# SUBTRACT BASE RSC /i ".\H"
# ADD RSC /l 0x409 /i "\—борка\Puma\CFIO\SOURCES\cpp" /i "\projects\puma\vss_rk\CFIO\SOURCES\cpp" /i "\Puma_VSS\CFIO\SOURCES\cpp"
# SUBTRACT RSC /i ".\H"

!ELSEIF  "$(CFG)" == "CFIO - Win32 Debug"

# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x419 /i "\—борка\Puma\CFIO\SOURCES\cpp" /i "\projects\puma\vss_rk\CFIO\SOURCES\cpp" /i "\Puma_VSS\CFIO\SOURCES\cpp"
# SUBTRACT BASE RSC /i "..\H" /i "..\..\H" /i "..\..\..\H"
# ADD RSC /l 0x409 /fo"..\..\..\Debug\CFIO/Cfio_Eng.res" /i "\—борка\Puma\CFIO\SOURCES\cpp" /i "\projects\puma\vss_rk\CFIO\SOURCES\cpp" /i "\Puma_VSS\CFIO\SOURCES\cpp"
# SUBTRACT RSC /i "..\H" /i "..\..\H" /i "..\..\..\H"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\cpp\Cfio_Rus.rc

!IF  "$(CFG)" == "CFIO - Win32 Release"

# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x419 /i "\—борка\Puma\CFIO\SOURCES\cpp" /i "\projects\puma\vss_rk\CFIO\SOURCES\cpp" /i "\Puma_VSS\CFIO\SOURCES\cpp"
# SUBTRACT BASE RSC /i ".\H"
# ADD RSC /l 0x419 /fo"..\..\..\Release\CFIO/Cfio_Rus.res" /i "\—борка\Puma\CFIO\SOURCES\cpp" /i "\projects\puma\vss_rk\CFIO\SOURCES\cpp" /i "\Puma_VSS\CFIO\SOURCES\cpp"
# SUBTRACT RSC /i ".\H"

!ELSEIF  "$(CFG)" == "CFIO - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
