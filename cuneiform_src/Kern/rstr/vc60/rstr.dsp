# Microsoft Developer Studio Project File - Name="Rstr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Rstr - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Rstr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Rstr.mak" CFG="Rstr - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Rstr - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Rstr - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Rstr - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "..\..\Release\rstr"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /WX /GX /Zi /O2 /I "..\..\hrstr" /I "..\..\h" /I "..\..\hh" /I "..\..\hhh" /I "..\..\hrk" /I "..\..\hrkint" /I "..\..\hdebug" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "__RSTR__" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"..\..\bin32\Rstr.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /opt:ref
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Rstr - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bind"
# PROP Intermediate_Dir "..\..\debug\rstr"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /WX /Gm /GX /Zi /Od /I "..\..\hrstr" /I "..\..\h" /I "..\..\hh" /I "..\..\hhh" /I "..\..\hrk" /I "..\..\hrkint" /I "..\..\hdebug" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "__RSTR__" /D "DPUMA_SNAP" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386

!ENDIF 

# Begin Target

# Name "Rstr - Win32 Release"
# Name "Rstr - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=..\..\hdebug\__Snp.c
# End Source File
# Begin Source File

SOURCE=..\src\Abris.c
# End Source File
# Begin Source File

SOURCE=..\src\Abrtools.c
# End Source File
# Begin Source File

SOURCE=..\src\Acc_tabs.c
# End Source File
# Begin Source File

SOURCE=..\src\Accents.c
# End Source File
# Begin Source File

SOURCE=..\src\add1_std.c
# End Source File
# Begin Source File

SOURCE=..\src\add2_std.c
# End Source File
# Begin Source File

SOURCE=..\src\Alcutfun.c
# End Source File
# Begin Source File

SOURCE=..\src\Alphabet.c
# End Source File
# Begin Source File

SOURCE=..\src\Atlant.c
# End Source File
# Begin Source File

SOURCE=..\src\Baton.c
# End Source File
# Begin Source File

SOURCE=..\src\Bold.c
# End Source File
# Begin Source File

SOURCE=..\src\Celldump.c
# End Source File
# Begin Source File

SOURCE=..\src\cg_hcut.c
# End Source File
# Begin Source File

SOURCE=..\src\Cg_main.c
# End Source File
# Begin Source File

SOURCE=..\src\Cg_smcut.c
# End Source File
# Begin Source File

SOURCE=..\src\Cg_tools.c
# End Source File
# Begin Source File

SOURCE=..\src\Compkit.c
# End Source File
# Begin Source File

SOURCE=..\src\Context.c
# End Source File
# Begin Source File

SOURCE=..\src\Contools.c
# End Source File
# Begin Source File

SOURCE=..\src\Corners.c
# End Source File
# Begin Source File

SOURCE=..\src\Cutgraph.c
# End Source File
# Begin Source File

SOURCE=..\src\Cutpoint.c
# End Source File
# Begin Source File

SOURCE=..\src\Defpoint.c
# End Source File
# Begin Source File

SOURCE=..\src\Diff.c
# End Source File
# Begin Source File

SOURCE=..\src\Differ.c
# End Source File
# Begin Source File

SOURCE=..\src\Diffr.c
# End Source File
# Begin Source File

SOURCE=..\src\Diffrang.c
# End Source File
# Begin Source File

SOURCE=..\src\Diffrb.c
# End Source File
# Begin Source File

SOURCE=..\src\Diffrv.c
# End Source File
# Begin Source File

SOURCE=..\src\Dm1.c
# End Source File
# Begin Source File

SOURCE=..\src\Dm2.c
# End Source File
# Begin Source File

SOURCE=..\src\Dmacc.c
# End Source File
# Begin Source File

SOURCE=..\src\Dmbox.c
# End Source File
# Begin Source File

SOURCE=..\src\Dms.c
# End Source File
# Begin Source File

SOURCE=..\src\Embbox.c
# End Source File
# Begin Source File

SOURCE=..\src\erect2.c
# End Source File
# Begin Source File

SOURCE=..\src\Erection.c
# End Source File
# Begin Source File

SOURCE=..\src\Findiff.c
# End Source File
# Begin Source File

SOURCE=..\src\Fontprop.c
# End Source File
# Begin Source File

SOURCE=..\src\fontser.c
# End Source File
# Begin Source File

SOURCE=..\src\Glue.c
# End Source File
# Begin Source File

SOURCE=..\src\Il1peak.c
# End Source File
# Begin Source File

SOURCE=..\src\Il1tg.c
# End Source File
# Begin Source File

SOURCE=..\src\Iot.c
# End Source File
# Begin Source File

SOURCE=..\src\Lang_fil.c
# End Source File
# Begin Source File

SOURCE=..\src\Lettab.c
# End Source File
# Begin Source File

SOURCE=..\src\Lineout.c
# End Source File
# Begin Source File

SOURCE=..\src\match_wd.c
# End Source File
# Begin Source File

SOURCE=..\src\Mmx_addr.c
# End Source File
# Begin Source File

SOURCE=..\src\P2_bold.c
# End Source File
# Begin Source File

SOURCE=..\src\p2_cour.c
# End Source File
# Begin Source File

SOURCE=..\src\Pass3.c
# End Source File
# Begin Source File

SOURCE=..\src\Passe.c
# End Source File
# Begin Source File

SOURCE=..\src\Perc.c
# End Source File
# Begin Source File

SOURCE=..\src\Prop.c
# End Source File
# Begin Source File

SOURCE=..\src\Proport.c
# End Source File
# Begin Source File

SOURCE=..\src\Proptab.c
# End Source File
# Begin Source File

SOURCE=..\src\Punct.c
# End Source File
# Begin Source File

SOURCE=..\src\Qsort.c
# End Source File
# Begin Source File

SOURCE=..\src\Rastror.c
# End Source File
# Begin Source File

SOURCE=..\src\Rcm.c
# End Source File
# Begin Source File

SOURCE=..\src\Rcontext.c
# End Source File
# Begin Source File

SOURCE=..\src\rstr_bl.c
# End Source File
# Begin Source File

SOURCE=..\src\rstr_con.c
# End Source File
# Begin Source File

SOURCE=..\src\rstr_lik.c
# End Source File
# Begin Source File

SOURCE=..\src\Rstr_p2.c
# End Source File
# Begin Source File

SOURCE=..\src\Scalar.c
# End Source File
# Begin Source File

SOURCE=..\src\Shaving.c
# End Source File
# Begin Source File

SOURCE=..\src\Shevron.c
# End Source File
# Begin Source File

SOURCE=..\src\Skew.c
# End Source File
# Begin Source File

SOURCE=..\src\snap.c
# End Source File
# Begin Source File

SOURCE=..\src\snap_fon.c
# End Source File
# Begin Source File

SOURCE=..\src\Space.c
# End Source File
# Begin Source File

SOURCE=..\src\Spec_sym.c
# End Source File
# Begin Source File

SOURCE=..\src\St_tools.c
# End Source File
# Begin Source File

SOURCE=..\src\Stic_big.c
# End Source File
# Begin Source File

SOURCE=..\src\Stic_cha.c
# End Source File
# Begin Source File

SOURCE=..\src\Stic_def.c
# End Source File
# Begin Source File

SOURCE=..\src\Stic_dis.c
# End Source File
# Begin Source File

SOURCE=..\src\Stic_frt.c
# End Source File
# Begin Source File

SOURCE=..\src\Stic_mak.c
# End Source File
# Begin Source File

SOURCE=..\src\Sweeper.c
# End Source File
# Begin Source File

SOURCE=..\src\Tm.c
# End Source File
# Begin Source File

SOURCE=..\src\Tools.c
# End Source File
# Begin Source File

SOURCE=..\src\Ukr.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=..\..\HRstr\Abris.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Alik_rec.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\ALPHABET.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\C_MEMORY.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Common.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Context.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\CTP.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Cut_glue.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Cuthdr.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\CUTSTR.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\DIFFRANG.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\DIFFRB.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\DISCRIM.H
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

SOURCE=..\..\HRstr\Extract.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\FUNC.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\IL1PEAK.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\INC_TAB.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\INTERVAL.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Lang.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Layout.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Linear.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\LINUTIL.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Loadtree.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\LTCONFIG.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Lterrors.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Madebug.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\match_wd.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\MSGERR.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Nt_types.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\P2LIBR.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\PICTURE.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\prop_nar.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\resource.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Roots.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\SHAVING.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\SHEET.H
# End Source File
# Begin Source File

SOURCE="..\..\HRstr\STIC-FRT.H"
# End Source File
# Begin Source File

SOURCE="..\..\HRstr\Stic-mac.h"
# End Source File
# Begin Source File

SOURCE="..\..\HRstr\STIC-TAB.H"
# End Source File
# Begin Source File

SOURCE="..\..\HRstr\Stic-tb.h"
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\STICK.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Strings.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Struct.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\TECH.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\TIGRUN.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\TM.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\TM_INT.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\Tuner.h
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\UKR.H
# End Source File
# Begin Source File

SOURCE=..\..\HRstr\user_voc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
