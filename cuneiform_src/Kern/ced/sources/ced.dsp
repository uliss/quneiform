# Microsoft Developer Studio Project File - Name="ced" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ced - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ced.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ced.mak" CFG="ced - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ced - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ced - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ced - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin\Lang"
# PROP Intermediate_Dir "..\..\release\ced"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /WX /GX /O2 /I "..\h" /I "..\..\hhh" /I ".\h" /I "..\..\h" /I "..\..\hrk" /I "..\..\hdebug" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "__CED__" /FD /c
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
# ADD LINK32 user32.lib gdi32.lib /nologo /subsystem:windows /dll /machine:I386 /implib:"..\..\bin/ced.lib" /libpath:"..\..\bin"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ced - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bind"
# PROP Intermediate_Dir "..\..\debug\ced"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I "..\h" /I "..\..\hhh" /I ".\h" /I "..\..\h" /I "..\..\hrk" /I "..\..\hdebug" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__CED__" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 user32.lib gdi32.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /libpath:"..\..\bind"

!ENDIF 

# Begin Target

# Name "ced - Win32 Release"
# Name "ced - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\h\ced_struct.h
# End Source File
# Begin Source File

SOURCE=.\H\cedint.h
# End Source File
# Begin Source File

SOURCE=.\main\StdAfx.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\main\ced.cpp
# End Source File
# Begin Source File

SOURCE=.\main\Ced_Eng.rc

!IF  "$(CFG)" == "ced - Win32 Release"

# ADD BASE RSC /l 0x419 /i "main"
# ADD RSC /l 0x409 /i "main"

!ELSEIF  "$(CFG)" == "ced - Win32 Debug"

# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x419 /i "main"
# ADD RSC /l 0x409 /fo"..\..\debug\ced/Ced_Eng.res" /i "main"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\main\ced_func.cpp
# End Source File
# Begin Source File

SOURCE=.\main\ced_func_old.cpp
# End Source File
# Begin Source File

SOURCE=.\main\ced_func_rtf.cpp
# End Source File
# Begin Source File

SOURCE=.\main\Ced_Rus.rc

!IF  "$(CFG)" == "ced - Win32 Release"

# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x419 /i "main"
# ADD RSC /l 0x419 /fo"..\..\release\ced/Ced_Rus.res" /i "main"

!ELSEIF  "$(CFG)" == "ced - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\main\ced_struct_implem.cpp
# End Source File
# Begin Source File

SOURCE=.\main\dll.cpp
# End Source File
# Begin Source File

SOURCE=.\main\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\main\wrapper.cpp
# End Source File
# End Group
# End Target
# End Project
