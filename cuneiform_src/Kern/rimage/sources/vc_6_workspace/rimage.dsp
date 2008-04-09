# Microsoft Developer Studio Project File - Name="RImage" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RImage - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rimage.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rimage.mak" CFG="RImage - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RImage - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RImage - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RImage - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\bin"
# PROP Intermediate_Dir "..\..\..\Release\RImage"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /WX /GX /Zi /O2 /I "..\..\H" /I "..\..\..\H" /I "..\..\..\HHH" /I "..\main\Deza" /I "..\main\kronrod" /I "..\main" /I "..\..\..\hrk" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG" /d "..\..\H" /d "..\..\..\H"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /libpath:"..\..\..\bin" /opt:ref
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RImage - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\bind"
# PROP Intermediate_Dir "..\..\..\Debug\RImage"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I "..\..\..\H" /I "..\..\..\HHH" /I "..\..\H" /I "..\main\Deza" /I "..\main\kronrod" /I "..\main" /I "..\..\..\hrk" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /i "..\..\H" /i "..\..\..\H" /i "..\main\Deza" /i "..\main\kronrod" /i "..\main" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 user32.lib cfio.lib rdib.lib cimage.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /libpath:"..\..\..\bind"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "RImage - Win32 Release"
# Name "RImage - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\main\Deza\CDezaBinarizator.cpp
# End Source File
# Begin Source File

SOURCE=..\main\Kronrod\CKronrodBinarizator.cpp
# End Source File
# Begin Source File

SOURCE=..\main\CRIBinarizator.cpp
# End Source File
# Begin Source File

SOURCE=..\main\CRIControl.cpp
# End Source File
# Begin Source File

SOURCE=..\main\CRIImage.cpp
# End Source File
# Begin Source File

SOURCE=..\main\CRImage.rc
# End Source File
# Begin Source File

SOURCE=..\main\CRIMemory.cpp
# End Source File
# Begin Source File

SOURCE=..\main\CRInvertor.cpp
# End Source File
# Begin Source File

SOURCE=..\main\CRRotator.cpp
# End Source File
# Begin Source File

SOURCE=..\main\CRTurner.cpp
# End Source File
# Begin Source File

SOURCE=..\main\dll.cpp
# End Source File
# Begin Source File

SOURCE=..\main\Kronrod\GDATA.C
# End Source File
# Begin Source File

SOURCE="..\main\Kronrod\GREY-WND.C"
# End Source File
# Begin Source File

SOURCE=..\main\RProgressor.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\main\Deza\CDezaBinarizator.h
# End Source File
# Begin Source File

SOURCE=..\main\Kronrod\CKronrodBinarizator.h
# End Source File
# Begin Source File

SOURCE=..\main\CRIBinarizator.h
# End Source File
# Begin Source File

SOURCE=..\main\CRIControl.h
# End Source File
# Begin Source File

SOURCE=..\main\CRIDefines.h
# End Source File
# Begin Source File

SOURCE=..\main\CRIMemory.h
# End Source File
# Begin Source File

SOURCE=..\main\CRInvertor.h
# End Source File
# Begin Source File

SOURCE=..\main\CRRotator.h
# End Source File
# Begin Source File

SOURCE=..\main\CRTurner.h
# End Source File
# Begin Source File

SOURCE=..\main\Kronrod\Gdata.h
# End Source File
# Begin Source File

SOURCE=..\main\RProgressor.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
