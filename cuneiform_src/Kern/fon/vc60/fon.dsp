# Microsoft Developer Studio Project File - Name="fon" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=fon - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fon.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fon.mak" CFG="fon - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fon - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "fon - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fon - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "..\..\Release\fon"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /WX /GX /Zi /O2 /I "..\..\h" /I "..\..\hhh" /I "..\..\hrk" /I "..\..\hrkint" /I "..\..\hdebug" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "__FON__" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 user32.lib gdi32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\bin/fon32.dll" /libpath:"..\..\bind" /opt:ref
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "fon - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bind"
# PROP Intermediate_Dir "..\..\debug\fon"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I "..\..\h" /I "..\..\hhh" /I "..\..\hrk" /I "..\..\hrkint" /I "..\..\hdebug" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__FON__" /FR /FD /c
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
# ADD LINK32 user32.lib gdi32.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"..\..\bind/fon32.dll" /libpath:"..\..\bind"
# SUBTRACT LINK32 /profile /map

!ENDIF 

# Begin Target

# Name "fon - Win32 Release"
# Name "fon - Win32 Debug"
# Begin Group "Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Src\Clu_best.c
# End Source File
# Begin Source File

SOURCE=..\src\Clu_div.c
# End Source File
# Begin Source File

SOURCE=..\src\Clu_lang.c
# End Source File
# Begin Source File

SOURCE=..\src\Clu_test.c
# End Source File
# Begin Source File

SOURCE=..\Src\Cutoff.c
# End Source File
# Begin Source File

SOURCE=..\Src\Dist_bou.c
# End Source File
# Begin Source File

SOURCE=..\Src\Distance.c
# End Source File
# Begin Source File

SOURCE=..\Src\Fon_snap.c
# End Source File
# Begin Source File

SOURCE=..\Src\Fonrec.c
# End Source File
# Begin Source File

SOURCE=..\Src\P2_addcl.c
# End Source File
# Begin Source File

SOURCE=..\Src\P2_clu.c
# End Source File
# Begin Source File

SOURCE=..\Src\P2_dsbit.c
# End Source File
# Begin Source File

SOURCE=..\src\P2_mem.c
# End Source File
# Begin Source File

SOURCE=..\Src\P2_thick.c
# End Source File
# End Group
# Begin Group "Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\Array1.h
# End Source File
# Begin Source File

SOURCE=..\src\C_types.h
# End Source File
# Begin Source File

SOURCE=..\src\CLU_LANG.H
# End Source File
# Begin Source File

SOURCE=..\src\Fonrec.h
# End Source File
# Begin Source File

SOURCE=..\src\Sfont.h
# End Source File
# Begin Source File

SOURCE=..\Src\Utilctb.c
# End Source File
# End Group
# End Target
# End Project
