# Microsoft Developer Studio Project File - Name="rpstr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=rpstr - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rpstr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rpstr.mak" CFG="rpstr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rpstr - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "rpstr - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rpstr - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "..\..\Release\rpstr"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I ".\\" /I "..\src" /I "..\..\h" /I "..\..\hhh" /I "..\src\incl" /I "..\src\capdrop" /I "..\..\hrk" /I "..\..\hdebug" /I "..\..\hrkint" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "__RPSTR__" /FD /c
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
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\..\bin"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "rpstr - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bind"
# PROP Intermediate_Dir "..\..\debug\rpstr"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I ".\\" /I "..\src" /I "..\..\h" /I "..\..\hhh" /I "..\src\incl" /I "..\src\capdrop" /I "..\..\hrk" /I "..\..\hdebug" /I "..\..\hrkint" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__RPSTR__" /FR /FD /c
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
# ADD LINK32 user32.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /libpath:"..\..\bind"

!ENDIF 

# Begin Target

# Name "rpstr - Win32 Release"
# Name "rpstr - Win32 Debug"
# Begin Group "SourcesFiles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\hdebug\__Snp.c
# End Source File
# Begin Source File

SOURCE=..\Src\capdrop\cap_drop.c
# End Source File
# Begin Source File

SOURCE=..\Src\Incl\cor_incl.c
# End Source File
# Begin Source File

SOURCE=..\Src\cor_spel.c
# End Source File
# Begin Source File

SOURCE=..\Src\Rpstr.c
# End Source File
# Begin Source File

SOURCE=..\SRC\vertstr.cpp
# End Source File
# End Group
# Begin Group "HeadersFiles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\SRC\capdrop\cap_drop.h
# End Source File
# Begin Source File

SOURCE=..\Src\Incl\cor_incl.h
# End Source File
# Begin Source File

SOURCE=..\SRC\cor_spel.h
# End Source File
# Begin Source File

SOURCE=..\Src\resource.h
# End Source File
# End Group
# End Target
# End Project
