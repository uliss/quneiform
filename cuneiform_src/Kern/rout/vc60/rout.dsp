# Microsoft Developer Studio Project File - Name="Rout" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Rout - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rout.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rout.mak" CFG="Rout - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Rout - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Rout - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Rout - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin\Lang"
# PROP Intermediate_Dir "..\..\release\rout"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COUT_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\h" /I "..\..\h" /I "..\..\hhh" /I "..\..\HRK" /I "..\..\hdebug" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COUT_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 user32.lib /nologo /dll /pdb:"..\..\Release/Rout.pdb" /machine:I386 /implib:"..\..\bin/Rout.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Rout - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bind"
# PROP Intermediate_Dir "..\..\debug\rout"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COUT_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I "..\H" /I "..\..\H" /I "..\..\HHH" /I "..\..\HRK" /I "..\..\hdebug" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COUT_EXPORTS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 user32.lib ced.lib /nologo /dll /incremental:no /debug /machine:I386 /pdbtype:sept /libpath:"..\..\bind"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Rout - Win32 Release"
# Name "Rout - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Src\Browse.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\CodeTables.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\HTML.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Pictures.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Rout.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Rout_DLL.cpp
# End Source File
# Begin Source File

SOURCE=..\src\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\Src\TableDBF.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\TableText.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Text.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Words.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\SRC\Dbf.h
# End Source File
# Begin Source File

SOURCE=..\Src\resource.h
# End Source File
# Begin Source File

SOURCE=..\Src\Rout_own.h
# End Source File
# Begin Source File

SOURCE=..\src\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Wtypes.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\SRC\Rout_Eng.rc

!IF  "$(CFG)" == "Rout - Win32 Release"

# ADD BASE RSC /l 0x419 /i "\cuneiform\OpenSource\Version1\rout\SRC" /i "\—борка\Puma\Rout\SRC" /i "\projects\puma\vss_cf\Rout\SRC" /i "\PUMA\Rout\SRC" /i "\Puma_VSS\Rout\SRC"
# ADD RSC /l 0x409 /i "\cuneiform\OpenSource\Version1\rout\SRC" /i "\—борка\Puma\Rout\SRC" /i "\projects\puma\vss_cf\Rout\SRC" /i "\PUMA\Rout\SRC" /i "\Puma_VSS\Rout\SRC"

!ELSEIF  "$(CFG)" == "Rout - Win32 Debug"

# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x419 /i "\cuneiform\OpenSource\Version1\rout\SRC" /i "\—борка\Puma\Rout\SRC" /i "\projects\puma\vss_cf\Rout\SRC" /i "\PUMA\Rout\SRC" /i "\Puma_VSS\Rout\SRC"
# ADD RSC /l 0x409 /fo"..\..\debug\rout/Rout_Eng.res" /i "\cuneiform\OpenSource\Version1\rout\SRC" /i "\—борка\Puma\Rout\SRC" /i "\projects\puma\vss_cf\Rout\SRC" /i "\PUMA\Rout\SRC" /i "\Puma_VSS\Rout\SRC"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SRC\Rout_Rus.rc

!IF  "$(CFG)" == "Rout - Win32 Release"

# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x419 /i "\cuneiform\OpenSource\Version1\rout\SRC" /i "\—борка\Puma\Rout\SRC" /i "\projects\puma\vss_cf\Rout\SRC" /i "\PUMA\Rout\SRC" /i "\Puma_VSS\Rout\SRC"
# ADD RSC /l 0x419 /fo"..\..\release\rout/Rout_Rus.res" /i "\cuneiform\OpenSource\Version1\rout\SRC" /i "\—борка\Puma\Rout\SRC" /i "\projects\puma\vss_cf\Rout\SRC" /i "\PUMA\Rout\SRC" /i "\Puma_VSS\Rout\SRC"

!ELSEIF  "$(CFG)" == "Rout - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
