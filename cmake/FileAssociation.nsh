/*
_____________________________________________________________________________
 
                       File Association
_____________________________________________________________________________
 
 Based on code taken from http://nsis.sourceforge.net/File_Association 
 
 Usage in script:
 1. !include "FileAssociation.nsh"
 2. [Section|Function]
      ${FileAssociationFunction} "Param1" "Param2" "..." $var
    [SectionEnd|FunctionEnd]
 
 FileAssociationFunction=[RegisterExtension|UnRegisterExtension]
 
_____________________________________________________________________________
 
 ${RegisterExtension} "[executable]" "[extension]" "[description]"
 
"[executable]"     ; executable which opens the file format
                   ;
"[extension]"      ; extension, which represents the file format to open
                   ;
"[description]"    ; description for the extension. This will be display in Windows Explorer.
                   ;
 
 
 ${UnRegisterExtension} "[extension]" "[description]"
 
"[extension]"      ; extension, which represents the file format to open
                   ;
"[description]"    ; description for the extension. This will be display in Windows Explorer.
                   ;
 
_____________________________________________________________________________
 
                         Macros
_____________________________________________________________________________
 
 Change log window verbosity (default: 3=no script)
 
 Example:
 !include "FileAssociation.nsh"
 !insertmacro RegisterExtension
 ${FileAssociation_VERBOSE} 4   # all verbosity
 !insertmacro UnRegisterExtension
 ${FileAssociation_VERBOSE} 3   # no script
*/
 
 
!ifndef FileAssociation_INCLUDED
!define FileAssociation_INCLUDED
 
!include Util.nsh
 
!verbose push
!verbose 3
!ifndef _FileAssociation_VERBOSE
    !define _FileAssociation_VERBOSE 3
!endif
!verbose ${_FileAssociation_VERBOSE}
!define FileAssociation_VERBOSE `!insertmacro FileAssociation_VERBOSE`
!verbose pop

!define SHCNE_ASSOCCHANGED 0x08000000
!define SHCNF_IDLIST 0
 
!macro FileAssociation_VERBOSE _VERBOSE
        !verbose push
        !verbose 3
        !undef _FileAssociation_VERBOSE
        !define _FileAssociation_VERBOSE ${_VERBOSE}
        !verbose pop
!macroend
 
 
!macro RegisterExtensionCall _EXECUTABLE _EXTENSION _DESCRIPTION
        !verbose push
        !verbose ${_FileAssociation_VERBOSE}
        Push `${_EXECUTABLE}`
        Push `${_EXTENSION}`
        Push `${_DESCRIPTION}`
        ${CallArtificialFunction} RegisterExtension_
        !verbose pop
!macroend
 
!macro UnRegisterExtensionCall _EXTENSION _DESCRIPTION
        !verbose push
        !verbose ${_FileAssociation_VERBOSE}
        Push `${_EXTENSION}`
        Push `${_DESCRIPTION}`
        ${CallArtificialFunction} UnRegisterExtension_
        !verbose pop
!macroend
 
!define RegisterExtension `!insertmacro RegisterExtensionCall`
!define un.RegisterExtension `!insertmacro RegisterExtensionCall`
 
!macro RegisterExtension
!macroend
 
!macro un.RegisterExtension
!macroend
 
!macro RegisterExtension_
  !verbose push
  !verbose ${_FileAssociation_VERBOSE}
 
  Exch $R2 ;desc
  Exch
  Exch $R1 ;ext
  Exch
  Exch 2
  Exch $R0 ;exe
  Exch 2
  Push $0
  Push $1
 
  WriteRegStr HKCR $R1 "" $R0
  WriteRegStr HKCR $R0 "" $R0
  WriteRegStr HKCR "$R0\shell" "" "open"
  WriteRegStr HKCR "$R0\DefaultIcon" "" "$R2,1"
  WriteRegStr HKCR "$R0\shell\open\command" "" '$R2 "%1"'
  
  System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v(${SHCNE_ASSOCCHANGED}, ${SHCNF_IDLIST}, 0, 0)'
 
  Pop $1
  Pop $0
  Pop $R2
  Pop $R1
  Pop $R0
 
  !verbose pop
!macroend
 
!define UnRegisterExtension `!insertmacro UnRegisterExtensionCall`
!define un.UnRegisterExtension `!insertmacro UnRegisterExtensionCall`
 
!macro UnRegisterExtension
!macroend
 
!macro un.UnRegisterExtension
!macroend
 
!macro UnRegisterExtension_
  !verbose push
  !verbose ${_FileAssociation_VERBOSE}
 
  Exch $R1 ;desc
  Exch
  Exch $R0 ;ext
  Exch
  Push $0
  Push $1
 
  DeleteRegKey HKCR $R0
  DeleteRegKey HKCR $R1 ;Delete key with association name settings
  
  System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) v(${SHCNE_ASSOCCHANGED}, ${SHCNF_IDLIST}, 0, 0)'
 
  Pop $1
  Pop $0
  Pop $R1
  Pop $R0
 
  !verbose pop
!macroend
 
!endif # !FileAssociation_INCLUDED
