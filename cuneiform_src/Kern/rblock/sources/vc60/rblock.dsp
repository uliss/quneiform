# Microsoft Developer Studio Project File - Name="RBLOCK" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RBLOCK - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rblock.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rblock.mak" CFG="RBLOCK - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RBLOCK - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RBLOCK - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RBLOCK - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\bin"
# PROP Intermediate_Dir "..\..\..\Release\RBlock"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /WX /GX /Zi /O2 /I "..\h" /I "..\..\..\h" /I "..\..\..\hhh" /I "..\..\..\hhh\tigerh\h" /I "..\..\..\hhh\tigerh" /I "..\..\..\hhh\tigerh\usercall" /I "..\main" /I "..\..\..\hrk" /I "..\..\..\hdebug" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 user32.lib ccom.lib cline.lib cstr.lib rstr.lib /nologo /subsystem:windows /dll /debug /machine:I386 /libpath:"..\..\..\bin" /opt:ref
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RBLOCK - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\bind"
# PROP Intermediate_Dir "..\..\..\Debug\RBlock"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I "..\..\..\h" /I "..\..\..\hhh" /I "..\h" /I "..\..\..\hhh\tigerh\h" /I "..\..\..\hhh\tigerh" /I "..\..\..\hhh\tigerh\usercall" /I "..\main" /I "..\..\..\hrk" /I "..\..\..\hdebug" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__NT__" /FR /FD /c
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
# ADD LINK32 user32.lib ccom.lib cline.lib cstr.lib rstr.lib cpage.lib exc.lib loc32.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /libpath:"..\..\..\bind"

!ENDIF 

# Begin Target

# Name "RBLOCK - Win32 Release"
# Name "RBLOCK - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\main\Rblock.rc
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Hdebug\__Snp.c
# End Source File
# Begin Source File

SOURCE=..\WinC\_adaptive.c
# End Source File
# Begin Source File

SOURCE=..\New_c\_Debug.c
# End Source File
# Begin Source File

SOURCE=..\main\_dll.cpp
# End Source File
# Begin Source File

SOURCE=..\WinC\_dlldummy.c
# End Source File
# Begin Source File

SOURCE=..\WinC\_graphics.c
# End Source File
# Begin Source File

SOURCE=..\New_c\_LoadRoots.c
# End Source File
# Begin Source File

SOURCE=..\main\_mymem.cpp
# End Source File
# Begin Source File

SOURCE=..\New_c\_PartLayout.c
# End Source File
# Begin Source File

SOURCE=..\New_c\_PartString.c
# End Source File
# Begin Source File

SOURCE="..\New_c\_pic&table.c"
# End Source File
# Begin Source File

SOURCE=..\main\_rblock.cpp
# End Source File
# Begin Source File

SOURCE=..\C\Compdata.c
# End Source File
# Begin Source File

SOURCE=..\C\Ltblglue.c
# End Source File
# Begin Source File

SOURCE=..\C\Ltblocks.c
# End Source File
# Begin Source File

SOURCE=..\C\Ltbreak.c
# End Source File
# Begin Source File

SOURCE=..\C\Lterrors.c
# End Source File
# Begin Source File

SOURCE=..\C\Ltexcomp.c
# End Source File
# Begin Source File

SOURCE=..\C\Lthand.c
# End Source File
# Begin Source File

SOURCE=..\C\Lthlundo.c
# End Source File
# Begin Source File

SOURCE=..\C\Lthyst.c
# End Source File
# Begin Source File

SOURCE=..\C\Ltibreak.c
# End Source File
# Begin Source File

SOURCE=..\C\Ltinclin.c
# End Source File
# Begin Source File

SOURCE=..\C\Ltmain.c
# End Source File
# Begin Source File

SOURCE=..\C\Ltpage.c
# End Source File
# Begin Source File

SOURCE=..\C\Ltremove.c
# End Source File
# Begin Source File

SOURCE=..\C\Ltroots.c
# End Source File
# Begin Source File

SOURCE=..\C\Ltseps.c
# End Source File
# Begin Source File

SOURCE=..\C\Ltsmart.c
# End Source File
# Begin Source File

SOURCE=..\C\Lttrees.c
# End Source File
# Begin Source File

SOURCE=..\C\Ltwstrip.c
# End Source File
# Begin Source File

SOURCE=..\C\Madebug.c
# End Source File
# Begin Source File

SOURCE=..\C\Msgerr.c
# End Source File
# Begin Source File

SOURCE=..\C\Qsort.c
# End Source File
# Begin Source File

SOURCE=..\C\Scalar.c
# End Source File
# Begin Source File

SOURCE=..\C\Seblocks.c
# End Source File
# Begin Source File

SOURCE=..\C\Seform.c
# End Source File
# Begin Source File

SOURCE=..\C\Sehbreak.c
# End Source File
# Begin Source File

SOURCE=..\C\Sehyst.c
# End Source File
# Begin Source File

SOURCE=..\C\Semain.c
# End Source File
# Begin Source File

SOURCE=..\C\Sestring.c
# End Source File
# Begin Source File

SOURCE=..\C\Sevbreak.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\main\mymem.h
# End Source File
# Begin Source File

SOURCE=..\h\new_c.h
# End Source File
# Begin Source File

SOURCE=..\h\NewFunc.h
# End Source File
# Begin Source File

SOURCE="..\h\pic&table.h"
# End Source File
# End Group
# End Target
# End Project
