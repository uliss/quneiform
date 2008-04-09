# Microsoft Developer Studio Project File - Name="Rling" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Rling - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Rling.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Rling.mak" CFG="Rling - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Rling - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Rling - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Rling - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "..\..\Release\RLing"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RLING_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\..\h" /I "..\..\hhh" /I "h" /I ".\\" /I "..\..\hrk" /I "..\..\hrkint" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RLING_EXPORTS" /FD /c
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
# ADD LINK32 user32.lib /nologo /dll /machine:I386
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "Rling - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bind"
# PROP Intermediate_Dir "..\..\Debug\RLing"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RLING_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I "..\..\h" /I "..\..\hhh" /I "h" /I ".\\" /I "..\..\hrk" /I "..\..\hrkint" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RLING_EXPORTS" /FR /FD /GZ /c
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
# ADD LINK32 user32.lib /nologo /dll /incremental:no /debug /machine:I386
# SUBTRACT LINK32 /pdb:none /pdbtype:<none>

!ENDIF 

# Begin Target

# Name "Rling - Win32 Release"
# Name "Rling - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cpp\CRLEd.cpp
# End Source File
# Begin Source File

SOURCE=.\CPP\CRLing.cpp
# End Source File
# Begin Source File

SOURCE=.\CRLing.rc
# End Source File
# Begin Source File

SOURCE=.\CPP\CRLMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\CPP\dll.cpp
# End Source File
# Begin Source File

SOURCE=.\CPP\RLControl.cpp
# End Source File
# Begin Source File

SOURCE=.\C\Rling_Ma.c
# End Source File
# Begin Source File

SOURCE=.\C\SPEL2DIC.C
# End Source File
# Begin Source File

SOURCE=.\C\SPEL2VOC.C
# End Source File
# Begin Source File

SOURCE=.\C\SPELABC.C
# End Source File
# Begin Source File

SOURCE=.\C\SPELART.C
# End Source File
# Begin Source File

SOURCE=.\C\SPELBUF.C
# End Source File
# Begin Source File

SOURCE=.\C\SPELCHK.C
# End Source File
# Begin Source File

SOURCE=.\C\SPELDAT1.C
# End Source File
# Begin Source File

SOURCE=.\C\SPELDAT2.C
# End Source File
# Begin Source File

SOURCE=.\C\Speldici.c
# End Source File
# Begin Source File

SOURCE=.\C\SPELDICT.C
# End Source File
# Begin Source File

SOURCE=.\C\SPELDVOC.C
# End Source File
# Begin Source File

SOURCE=.\C\SPELEDF1.C
# End Source File
# Begin Source File

SOURCE=.\C\SPELEDF2.C
# End Source File
# Begin Source File

SOURCE=.\C\Speledio.c
# End Source File
# Begin Source File

SOURCE=.\C\SPELFUN.C
# End Source File
# Begin Source File

SOURCE=.\C\Spelloop.c
# End Source File
# Begin Source File

SOURCE=.\C\SPELOUT.C
# End Source File
# Begin Source File

SOURCE=.\C\SPELQ.C
# End Source File
# Begin Source File

SOURCE=.\C\SPELSET.C
# End Source File
# Begin Source File

SOURCE=.\C\SPELSPEC.C
# End Source File
# Begin Source File

SOURCE=.\c\Udictini.c
# End Source File
# Begin Source File

SOURCE=.\C\UDICTUTI.C
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\h\CRLDefines.h
# End Source File
# Begin Source File

SOURCE=.\h\CRLEd.h
# End Source File
# Begin Source File

SOURCE=.\h\CRLMemory.h
# End Source File
# Begin Source File

SOURCE=.\h\EDTABLE.H
# End Source File
# Begin Source File

SOURCE=.\h\LANG.H
# End Source File
# Begin Source File

SOURCE=.\h\NT_TYPES.H
# End Source File
# Begin Source File

SOURCE=.\h\RLControl.h
# End Source File
# Begin Source File

SOURCE=.\h\SPEL_SRC.H
# End Source File
# Begin Source File

SOURCE=.\h\Speldefs.h
# End Source File
# Begin Source File

SOURCE=.\h\SPELDICT.H
# End Source File
# Begin Source File

SOURCE=.\h\SPELFUNC.H
# End Source File
# Begin Source File

SOURCE=.\h\SPELMODE.H
# End Source File
# Begin Source File

SOURCE=.\h\SPELTAB.H
# End Source File
# Begin Source File

SOURCE=.\h\SPELWATC.H
# End Source File
# Begin Source File

SOURCE=.\h\TigerEmulate.h
# End Source File
# Begin Source File

SOURCE=.\h\TUNER.H
# End Source File
# Begin Source File

SOURCE=.\h\UDICFUNC.H
# End Source File
# Begin Source File

SOURCE=.\h\UDICTEST.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
