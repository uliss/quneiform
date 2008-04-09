# Microsoft Developer Studio Project File - Name="puma" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=puma - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "puma.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "puma.mak" CFG="puma - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "puma - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "puma - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "puma - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin\Lang"
# PROP Intermediate_Dir "..\Release\puma"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "h" /I "..\h" /I "..\hhh" /I "..\hrk" /I "..\hdebug" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__NT__" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 user32.lib ccom.lib cstr.lib rstr.lib rpstr.lib cline.lib /nologo /subsystem:windows /dll /debug /machine:I386 /implib:"..\bin/puma.lib" /libpath:"..\bin" /OPT:REF
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "puma - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bind"
# PROP Intermediate_Dir "..\Debug\puma"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I "..\h" /I "..\hhh" /I "h" /I "..\hrstr" /I "..\hrk" /I "..\hdebug" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__NT__" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 user32.lib ccom.lib cstr.lib rstr.lib rpstr.lib cline.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /libpath:"..\bind"

!ENDIF 

# Begin Target

# Name "puma - Win32 Release"
# Name "puma - Win32 Debug"
# Begin Group "SourcesFiles"

# PROP Default_Filter "*.cpp *.c"
# Begin Source File

SOURCE=..\Hdebug\__Snp.c
# End Source File
# Begin Source File

SOURCE=.\c\common.cpp
# End Source File
# Begin Source File

SOURCE=.\c\Debug.cpp
# End Source File
# Begin Source File

SOURCE=.\main\dll.cpp
# End Source File
# Begin Source File

SOURCE=.\c\Enum.cpp
# End Source File
# Begin Source File

SOURCE=.\c\InitDone.cpp
# End Source File
# Begin Source File

SOURCE=.\main\mymem.cpp
# End Source File
# Begin Source File

SOURCE=.\c\out.cpp
# End Source File
# Begin Source File

SOURCE=.\c\PartLayout.cpp
# End Source File
# Begin Source File

SOURCE=.\c\PartRecog.cpp
# End Source File
# Begin Source File

SOURCE=.\main\puma.cpp
# End Source File
# Begin Source File

SOURCE=.\c\snp_sjtl.c
# End Source File
# End Group
# Begin Group "HeadersFiles"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\h\mpuma.h
# End Source File
# Begin Source File

SOURCE=.\h\mymem.h
# End Source File
# End Group
# Begin Group "resources"

# PROP Default_Filter "*.rc"
# Begin Source File

SOURCE=.\h\Puma_Eng.rc

!IF  "$(CFG)" == "puma - Win32 Release"

# ADD BASE RSC /l 0x419 /i "h"
# ADD RSC /l 0x409 /i "h"

!ELSEIF  "$(CFG)" == "puma - Win32 Debug"

# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x419 /i "h"
# ADD RSC /l 0x409 /fo"..\Debug\puma/Puma_Eng.res" /i "h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\h\Puma_Rus.rc

!IF  "$(CFG)" == "puma - Win32 Release"

# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x419 /i "h"
# ADD RSC /l 0x419 /fo"..\Release\puma/Puma_Rus.res" /i "h"

!ELSEIF  "$(CFG)" == "puma - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
