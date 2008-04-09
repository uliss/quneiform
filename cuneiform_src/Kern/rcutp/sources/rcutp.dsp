# Microsoft Developer Studio Project File - Name="RCUTP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RCUTP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RCUTP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RCUTP.mak" CFG="RCUTP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RCUTP - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RCUTP - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RCUTP - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "..\..\Release\rcutp"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RCUTP_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /WX /GX /Zi /O2 /I "..\h" /I "..\..\h" /I "..\..\hh" /I "..\..\hhh" /I "..\..\hrstr" /I "..\..\hrk" /I "..\..\hdebug" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RCUTP_EXPORTS" /D "__RCUTP__" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 user32.lib /nologo /dll /debug /machine:I386 /opt:ref
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RCUTP - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bind"
# PROP Intermediate_Dir "..\..\Debug\rcutp"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RCUTP_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I "..\h" /I "..\..\h" /I "..\..\hh" /I "..\..\hhh" /I "..\..\hrstr" /I "..\..\hrk" /I "..\..\hdebug" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RCUTP_EXPORTS" /D "__RCUTP__" /FR /FD /GZ /c
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
# ADD LINK32 user32.lib /nologo /dll /incremental:no /debug /machine:I386 /libpath:"..\..\bind"
# SUBTRACT LINK32 /profile /map /nodefaultlib /force

!ENDIF 

# Begin Target

# Name "RCUTP - Win32 Release"
# Name "RCUTP - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\hdebug\__Snp.c
# End Source File
# Begin Source File

SOURCE=.\MAIN\rcut_4rselstr.cpp
# End Source File
# Begin Source File

SOURCE=.\MAIN\rcutp.rc
# End Source File
# Begin Source File

SOURCE=.\MAIN\rcutp_call.cpp
# End Source File
# Begin Source File

SOURCE=.\MAIN\rcutp_func.cpp
# End Source File
# Begin Source File

SOURCE=.\MAIN\rcutp_main.cpp
# End Source File
# Begin Source File

SOURCE=.\MAIN\rcutp_mainfunc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\HRstr\Alik_rec.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Cuthdr.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\CUTSTR.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\DISKRTAB.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\DMCONST.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\EMBBOX.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\INTERVAL.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Nt_types.h
# End Source File
# Begin Source File

SOURCE=.\MAIN\rcutp_prot.h
# End Source File
# Begin Source File

SOURCE=.\MAIN\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Struct.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Tuner.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\MAIN\resource.h
# End Source File
# End Group
# End Target
# End Project
