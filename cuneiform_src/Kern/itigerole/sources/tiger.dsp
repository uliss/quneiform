# Microsoft Developer Studio Project File - Name="Tiger" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Tiger - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tiger.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tiger.mak" CFG="Tiger - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Tiger - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Tiger - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Tiger - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Tiger___Win32_Debug"
# PROP BASE Intermediate_Dir "Tiger___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bind"
# PROP Intermediate_Dir "..\..\Debug\PumaOLE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I ".\\" /I "..\..\h" /I "..\..\hhh" /I ".\tiger\h" /I "..\..\hrk" /I "..\..\hdebug" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "__PUMA__" /FR /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I ".\\" /I "..\..\h" /I "..\..\hhh" /I ".\tiger\h" /I "..\..\hrk" /I "..\..\hdebug" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "__PUMA__" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "__PUMA__"
# ADD MTL /nologo /D "__PUMA__"
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\..\bind/APuma.dll" /pdbtype:sept /libpath:"TIGER\lib" /libpath:"..\..\bind"
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 CrashRpt.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"..\..\bind/APuma.dll" /libpath:"TIGER\lib" /libpath:"..\..\bind"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "Tiger - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Tiger___Win32_Release"
# PROP BASE Intermediate_Dir "Tiger___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin\Lang"
# PROP Intermediate_Dir "..\..\Release\PumaOLE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /GX /O2 /I ".\\" /I "..\..\h" /I "..\..\hhh" /I ".\tiger\h" /I "..\..\hrk" /I "..\..\hdebug" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "__PUMA__" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /WX /GX /Zi /O2 /I ".\\" /I "..\..\h" /I "..\..\hhh" /I ".\tiger\h" /I "..\..\hrk" /I "..\..\hdebug" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "__PUMA__" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "__PUMA__"
# ADD MTL /nologo /D "__PUMA__"
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# SUBTRACT BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\bin\Lang/APuma.dll" /implib:"..\..\bin/APuma.lib" /pdbtype:sept /libpath:"TIGER\lib" /libpath:"..\..\bin"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /def:".\Tiger.def" /out:"..\..\bin\Lang/APuma.dll" /implib:"..\..\bin/APuma.lib" /libpath:"TIGER\lib" /libpath:"..\..\bin" /OPT:REF
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Tiger - Win32 Debug"
# Name "Tiger - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\HHH\__puma.c
# End Source File
# Begin Source File

SOURCE=..\..\hdebug\__Snp.c
# End Source File
# Begin Source File

SOURCE=.\TIGER\H\Lib_main.c
# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\TIGER\Recog.cpp
# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Recognition.cpp
# ADD BASE CPP /Z7
# SUBTRACT BASE CPP /YX
# ADD CPP /Z7
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\PUMA\recogpuma.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# SUBTRACT BASE CPP /YX /Yc
# SUBTRACT CPP /YX /Yc
# End Source File
# Begin Source File

SOURCE=.\TIGER\tgcallbk.cpp
# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Tiger.cpp
# SUBTRACT BASE CPP /YX
# SUBTRACT CPP /YX
# End Source File
# Begin Source File

SOURCE=.\Tiger.def

!IF  "$(CFG)" == "Tiger - Win32 Debug"

!ELSEIF  "$(CFG)" == "Tiger - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tiger.idl
# ADD BASE MTL /tlb ".\Tiger.tlb" /h "Tiger.h" /iid "Tiger_i.c" /Oicf
# ADD MTL /tlb ".\Tiger.tlb" /h "Tiger.h" /iid "Tiger_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\Tiger_Eng.rc

!IF  "$(CFG)" == "Tiger - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x409 /fo"..\..\Debug\PumaOLE/Tiger_Eng.res"
# ADD RSC /l 0x409 /fo"..\..\Debug\PumaOLE/Tiger_Eng.res"

!ELSEIF  "$(CFG)" == "Tiger - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tiger_i.c
# End Source File
# Begin Source File

SOURCE=.\Tiger_Rus.rc

!IF  "$(CFG)" == "Tiger - Win32 Debug"

!ELSEIF  "$(CFG)" == "Tiger - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x419 /fo"..\..\Release\PumaOLE/Tiger_Rus.res"
# ADD RSC /l 0x419 /fo"..\..\Release\PumaOLE/Tiger_Rus.res"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TIGER\tomono.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Tiger\H\_tgcall.h
# End Source File
# Begin Source File

SOURCE="..\..\..\..\Модули Микрософт\Microsoft Visual Studio\VC98\Include\BASETSD.H"
# End Source File
# Begin Source File

SOURCE=..\..\HHH\ced.h
# End Source File
# Begin Source File

SOURCE=..\..\HHH\cpage.h
# End Source File
# Begin Source File

SOURCE=..\..\HHH\cpagetyps.h
# End Source File
# Begin Source File

SOURCE=..\..\HHH\CTP.H
# End Source File
# Begin Source File

SOURCE=..\..\H\CTTYPES.H
# End Source File
# Begin Source File

SOURCE=.\Tiger\H\Cttypes.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Cttypext.h
# End Source File
# Begin Source File

SOURCE=.\Tiger\H\Dlltypes.h
# End Source File
# Begin Source File

SOURCE=..\..\Hdebug\dpuma.h
# End Source File
# Begin Source File

SOURCE=..\..\HHH\EDEXT.H
# End Source File
# Begin Source File

SOURCE=..\..\HHH\EDFILE.H
# End Source File
# Begin Source File

SOURCE=..\..\H\ERR16.H
# End Source File
# Begin Source File

SOURCE=.\Tiger\H\Err16.h
# End Source File
# Begin Source File

SOURCE=..\..\HHH\G2BW.H
# End Source File
# Begin Source File

SOURCE=..\..\H\GLOBUS.H
# End Source File
# Begin Source File

SOURCE=.\Tiger\H\Globus.h
# End Source File
# Begin Source File

SOURCE=.\Tiger\H\Imgaccs.h
# End Source File
# Begin Source File

SOURCE=..\..\H\imgaccs_gbw.h
# End Source File
# Begin Source File

SOURCE=..\..\HRK\LANG_DEF.H
# End Source File
# Begin Source File

SOURCE=..\..\HHH\polyblock.h
# End Source File
# Begin Source File

SOURCE=..\..\HHH\puma.h
# End Source File
# Begin Source File

SOURCE=.\Recognition.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\HHH\Table.h
# End Source File
# Begin Source File

SOURCE=.\Tiger\H\Tgcall.h
# End Source File
# Begin Source File

SOURCE=.\Tiger\H\Tgcallbk.h
# End Source File
# Begin Source File

SOURCE=.\Tiger\H\Tgerrors.h
# End Source File
# Begin Source File

SOURCE=.\TigerCP.h
# End Source File
# Begin Source File

SOURCE=.\tigerdef.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Recog_puma.rgs
# End Source File
# Begin Source File

SOURCE=.\recog_tiger.rgs
# End Source File
# Begin Source File

SOURCE=.\recognit.bin
# End Source File
# End Group
# End Target
# End Project
