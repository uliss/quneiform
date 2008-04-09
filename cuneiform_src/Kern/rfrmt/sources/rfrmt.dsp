# Microsoft Developer Studio Project File - Name="rfrmt" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=rfrmt - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rfrmt.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rfrmt.mak" CFG="rfrmt - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rfrmt - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "rfrmt - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rfrmt - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "..\..\Release\Rfrmt"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\h" /I "..\..\hhh" /I "..\h" /I "..\..\hrk" /I "..\..\hdebug" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 User32.lib Gdi32.lib cline.lib cstr.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\..\bin"

!ELSEIF  "$(CFG)" == "rfrmt - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bind"
# PROP Intermediate_Dir "..\..\Debug\rfrmt"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I "..\..\h" /I "..\..\hhh" /I "..\h" /I "..\..\hrk" /I "..\..\hdebug" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 User32.lib Gdi32.lib cline.lib cstr.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /force /libpath:"..\..\bind"
# SUBTRACT LINK32 /profile /map

!ENDIF 

# Begin Target

# Name "rfrmt - Win32 Release"
# Name "rfrmt - Win32 Debug"
# Begin Group "SourceFiles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Hdebug\__Snp.c
# End Source File
# Begin Source File

SOURCE=.\main\CreateRtf.cpp
# End Source File
# Begin Source File

SOURCE=.\main\frmt.cpp
# End Source File
# Begin Source File

SOURCE=.\main\frmtallignment.cpp
# End Source File
# Begin Source File

SOURCE=.\main\frmtdibapi.cpp
# End Source File
# Begin Source File

SOURCE=.\main\frmtdll.cpp
# End Source File
# Begin Source File

SOURCE=.\main\frmtfile.cpp
# End Source File
# Begin Source File

SOURCE=.\main\FrmtPict.cpp
# End Source File
# Begin Source File

SOURCE=.\main\Int_file.cpp
# End Source File
# Begin Source File

SOURCE=.\main\mymem.cpp
# End Source File
# Begin Source File

SOURCE=.\main\PageTree.cpp
# End Source File
# Begin Source File

SOURCE=.\main\Sort_int.cpp
# End Source File
# Begin Source File

SOURCE=.\main\Util.cpp
# End Source File
# Begin Source File

SOURCE=.\main\Util_lst.cpp
# End Source File
# Begin Source File

SOURCE=.\main\Util_spl.cpp
# End Source File
# End Group
# Begin Group "HeaderFiles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\h\alDebug.h
# End Source File
# Begin Source File

SOURCE=..\h\Consmess.h
# End Source File
# Begin Source File

SOURCE=..\h\CreateRtf.h
# End Source File
# Begin Source File

SOURCE=..\h\frmtdibapi.h
# End Source File
# Begin Source File

SOURCE=..\h\frmtpict.h
# End Source File
# Begin Source File

SOURCE=..\h\frmttabl.h
# End Source File
# Begin Source File

SOURCE=..\h\Ful_txt.h
# End Source File
# Begin Source File

SOURCE=.\main\mymem.h
# End Source File
# Begin Source File

SOURCE=..\h\rfrmt_prot.h
# End Source File
# Begin Source File

SOURCE=..\h\rfrmtfile.h
# End Source File
# Begin Source File

SOURCE=..\h\RtfEdWrite.h
# End Source File
# Begin Source File

SOURCE=..\h\RtfFragRect.h
# End Source File
# Begin Source File

SOURCE=.\main\StdAfx.h
# End Source File
# End Group
# End Target
# End Project
