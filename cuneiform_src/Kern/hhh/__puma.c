/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Разрешается повторное распространение и использование как в виде исходного кода,
так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

      * При повторном распространении исходного кода должны оставаться указанное
        выше уведомление об авторском праве, этот список условий и последующий
        отказ от гарантий.
      * При повторном распространении двоичного кода в документации и/или в
        других материалах, поставляемых при распространении, должны сохраняться
        указанная выше информация об авторском праве, этот список условий и
        последующий отказ от гарантий.
      * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
        быть использованы в качестве средства поддержки и/или продвижения
        продуктов, основанных на этом ПО, без предварительного письменного
        разрешения.

ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <windows.h>
#include <crtdbg.h>
#include "puma.h"

#define ADD_CED // Поддержка FED формата
#ifdef  ADD_CED
	#include "ced.h"
#endif

static FNPUMA_Init                              Init = NULL;
static FNPUMA_Done                              Done = NULL;
static FNPUMA_GetReturnCode             GetReturnCode = NULL;
static FNPUMA_GetReturnString   GetReturnString = NULL;
static FNPUMA_GetExportData             GetExportData = NULL;
static FNPUMA_SetImportData             SetImportData = NULL;
/*
static FNPUMA_Recognition               Recognition = NULL;
static FNPUMA_PartLayout                PartLayout = NULL;
static FNPUMA_PartRecognition   PartRecognition = NULL;
static FNPUMA_GetLanguage               GetLanguage = NULL;
*/
static FNPUMA_XOpen                             XOpen = NULL;
static FNPUMA_XClose                    XClose = NULL;
static FNPUMA_XPageAnalysis             XPageAnalysis = NULL;
static FNPUMA_XFinalRecognition XFinalRecognition = NULL;
static FNPUMA_XSave                             XSave = NULL;
static FNPUMA_EnumLanguages             EnumLanguages = NULL;
static FNPUMA_EnumFormats               EnumFormats = NULL;
static FNPUMA_EnumCodes                 EnumCodes = NULL;
static FNPUMA_EnumFormatMode    EnumFormatMode= NULL;
static FNPUMA_EnumTable                 EnumTable = NULL;
static FNPUMA_EnumPicture               EnumPicture = NULL;
static FNPUMA_XGetRotateDIB             XGetRotateDIB = NULL;
static FNPUMA_RenameImageName   fnRenameImageName = NULL;
static FNPUMA_XSetTemplate              fnXSetTemplate = NULL;
static FNPUMA_XGetTemplate              fnXGetTemplate = NULL;
static FNPUMA_Save                      fnSave = NULL;
static FNPUMA_XOpenClbk                 fnXOpenClbk = NULL;
static PUMAENTRY						peCED;
static FNPUMA_SaveToMemory              fnSaveToMemory = NULL;
static FNPUMA_GetSpecialBuffer			fnGetSpecialBuffer = NULL;  // OLEG for Consist
static FNPUMA_SetSpecialProject			fnSetSpecialProject = NULL;     // OLEG for GiP

static HINSTANCE        s_hLib  = NULL;

Bool32 LPUMA_Load(char * lpPath)
{
        Bool32 rc = FALSE;
        char szCurrentDir[260];
        char szFullPathName[260];

//		if (!StdLoad())
//			return -1; //RETURN("StdLoad() failed...");

        GetCurrentDirectory(sizeof(szCurrentDir),szCurrentDir);
        SetCurrentDirectory(lpPath);

  _RPT1(_CRT_WARN, "APUMA.DLL: Current directory is: %s\n", lpPath);

        strcpy(szFullPathName,lpPath);
        strcat(szFullPathName,"PUMA.DLL");

  _RPT1(_CRT_WARN, "APUMA.DLL: Puma.dll is: %s\n", szFullPathName);
        //s_hLib = LoadLibraryEx(szFullPathName,NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
        s_hLib = LoadLibrary(szFullPathName);

  _RPT1(_CRT_WARN, "APUMA.DLL: Puma.dll LoadLibrary %s!\n", (s_hLib ? "succeded" : "failed"));
#ifdef _DEBUG
  if(!s_hLib)
  {
      LPTSTR lpMsgBuf;
      FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL
        );
      // Display the string.
      _RPT1(_CRT_WARN, "APUMA.DLL: Reason: %s\n", lpMsgBuf);
      MessageBox( NULL, lpMsgBuf, "Debug message", MB_OK|MB_ICONINFORMATION );
      // Free the buffer.
      LocalFree( lpMsgBuf );
    }
#endif //_DEBUG


  SetCurrentDirectory(szCurrentDir);

  _RPT1(_CRT_WARN, "APUMA.DLL: Current directory is: %s\n", szCurrentDir);

        if(s_hLib)
        {
                Init = (FNPUMA_Init)GetProcAddress(s_hLib,"PUMA_Init");
                Done = (FNPUMA_Done)GetProcAddress(s_hLib,"PUMA_Done");
                GetReturnCode = (FNPUMA_GetReturnCode)GetProcAddress(s_hLib,"PUMA_GetReturnCode");
                GetReturnString = (FNPUMA_GetReturnString)GetProcAddress(s_hLib,"PUMA_GetReturnString");
                GetExportData = (FNPUMA_GetExportData)GetProcAddress(s_hLib,"PUMA_GetExportData");
                SetImportData = (FNPUMA_SetImportData)GetProcAddress(s_hLib,"PUMA_SetImportData");

                if( Init && Done && GetReturnCode && GetReturnString &&
                        GetExportData && SetImportData &&
                        GetExportData(PUMA_FNPUMA_XOpen,&XOpen) &&
                        GetExportData(PUMA_FNPUMA_XClose,&XClose) &&
                        GetExportData(PUMA_FNPUMA_XPageAnalysis,&XPageAnalysis) &&
                        GetExportData(PUMA_FNPUMA_XFinalRecognition,&XFinalRecognition) &&
                        GetExportData(PUMA_FNPUMA_XSave,&XSave) &&
                        GetExportData(PUMA_FNPUMA_EnumLanguages,&EnumLanguages) &&
                        GetExportData(PUMA_FNPUMA_EnumFormats,&EnumFormats) &&
                        GetExportData(PUMA_FNPUMA_EnumFormatMode,&EnumFormatMode) &&
                        GetExportData(PUMA_FNPUMA_EnumTable,&EnumTable) &&
                        GetExportData(PUMA_FNPUMA_EnumPicture,&EnumPicture) &&
                        GetExportData(PUMA_FNPUMA_XGetRotateDIB,&XGetRotateDIB) &&
                        GetExportData(PUMA_FNPUMA_RenameImageName,&fnRenameImageName) &&
                        GetExportData(PUMA_FNPUMA_XSetTemplate,&fnXSetTemplate) &&
                        GetExportData(PUMA_FNPUMA_XGetTemplate,&fnXGetTemplate) &&
                        GetExportData(PUMA_FNPUMA_Save,&fnSave) &&
                        GetExportData(PUMA_FNPUMA_XOpenClbk ,&fnXOpenClbk) &&
						GetExportData(PUMA_LPPUMAENTRY_CED ,&peCED) &&
                        GetExportData(PUMA_FNPUMA_SaveToMemory,&fnSaveToMemory) &&
                        GetExportData(PUMA_FNPUMA_GetSpecialBuffer,&fnGetSpecialBuffer) &&  // OLEG for GiP
						GetExportData(PUMA_FNPUMA_SetSpecialProject,&fnSetSpecialProject) &&   // OLEG for Consist
                        GetExportData(PUMA_FNPUMA_EnumCodes,&EnumCodes))
                {
                        rc = Init(0,0);
    }else{
      _RPT0(_CRT_WARN, "APUMA.DLL: Importing data failed!\n");
    }
        }
  if(!rc){
    _RPT0(_CRT_WARN, "APUMA.DLL: Init failed!\n");
  }
  return rc;
}
//////////////////////////////////////////////////////////////////
void LPUMA_Unload()
{
        if(s_hLib && Done)
        {
                Done();
                FreeLibrary(s_hLib);
                s_hLib = NULL;
        }
//	StdUnload();
}
//////////////////////////////////////////////////////////////////
Word32 LPUMA_GetReturnCode()
{
        Word32 rc = -1;
        if(s_hLib && GetReturnCode)
                rc = GetReturnCode();
return rc;
}
//////////////////////////////////////////////////////////////////
char * LPUMA_GetReturnString(Word32 dwError)
{
        char * rc = "Library PUMA.DLL not load.";
        if(s_hLib && GetReturnString)
                rc = GetReturnString(dwError);
return rc;
}
//////////////////////////////////////////////////////////////////
Bool32 LPUMA_GetExportData(Word32 dwType, void * pData)
{
        Bool32 rc = FALSE;
        if(s_hLib && GetExportData)
                rc = GetExportData(dwType,pData);
return rc;
}
//////////////////////////////////////////////////////////////////
Bool32 LPUMA_SetImportData(Word32 dwType, void * pData)
{
        Bool32 rc = FALSE;
        if(s_hLib && SetImportData)
                SetImportData(dwType,pData);
return rc;
}
//////////////////////////////////////////////////////////////////
Bool32 LPUMA_XOpen(void * pDIB,const char * lpFileName)
{
        Bool32 rc = FALSE;
        if(s_hLib && XOpen)
                rc = XOpen(pDIB,lpFileName);
        return rc;
}
//////////////////////////////////////////////////////////////////
Bool32  LPUMA_XClose( void )
{
        Bool32 rc = FALSE;
        if(s_hLib && XClose)
                rc = XClose();
        return rc;
}
//////////////////////////////////////////////////////////////////
Bool32  LPUMA_XPageAnalysis( void )
{
        Bool32 rc = FALSE;
        if(s_hLib && XPageAnalysis)
                rc = XPageAnalysis();
        return rc;
}
//////////////////////////////////////////////////////////////////
Bool32 LPUMA_XFinalRecognition( void )
{
        Bool32 rc = FALSE;
        if(s_hLib && XFinalRecognition)
                rc = XFinalRecognition();
        return rc;
}
//////////////////////////////////////////////////////////////////
Bool32 LPUMA_XSave(const char * lpOutFileName, Int32 lnFormat, Int32 lnCode )
{
        Bool32 rc = FALSE;
        if(s_hLib && XSave)
                rc = XSave(lpOutFileName,lnFormat,lnCode);
        return rc;
}
//////////////////////////////////////////////////////////////////
Word32   LPUMA_GetLanguage()
{
        Word32 rc = (Word32)-1;
        if(s_hLib && GetExportData)
        {
          Word32 lang = 0;
          if(GetExportData(PUMA_Word32_Language,&lang))
                  rc = lang;
        }

return rc;
}
//////////////////////////////////////////////////////////////////
void LPUMA_SetLanguage(Word32 lang)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Word32_Language,&lang))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
Bool32  LPUMA_GetSpeller()
{
        Bool32 rc = FALSE;
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_Bool32_Speller,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetSpeller(Bool32 nNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Bool32_Speller,&nNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
Bool32  LPUMA_GetOneColumn()
{
        Bool32 rc = FALSE;
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_Bool32_OneColumn,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetOneColumn(Bool32 nNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Bool32_OneColumn,&nNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
Bool32  LPUMA_GetFax100()
{
        Bool32 rc = FALSE;
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_Bool32_Fax100,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetFax100(Bool32 nNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Bool32_Fax100,&nNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
Bool32  LPUMA_GetDotMatrix()
{
        Bool32 rc = FALSE;
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_Bool32_DotMatrix,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetDotMatrix(Bool32 nNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Bool32_DotMatrix,&nNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
char *  LPUMA_GetUserDictName()
{
        char * rc = "";
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_pchar_UserDictName,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetUserDictName(char * lpszNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_pchar_UserDictName,lpszNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
Bool32  LPUMA_GetBold()
{
        Bool32 rc = FALSE;
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_Bool32_Bold,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetBold(Bool32 nNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Bool32_Bold,&nNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
Bool32  LPUMA_GetItalic()
{
        Bool32 rc = FALSE;
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_Bool32_Italic,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetItalic(Bool32 nNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Bool32_Italic,&nNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
Bool32  LPUMA_GetSize()
{
        Bool32 rc = FALSE;
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_Bool32_Size,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetSize(Bool32 nNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Bool32_Size,&nNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
Bool32  LPUMA_GetFormat()
{
        Bool32 rc = FALSE;
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_Bool32_Format,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetFormat(Bool32 nNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Bool32_Format,&nNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
char *  LPUMA_GetSerifName()
{
        char * rc = "";
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_pchar_SerifName,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetSerifName(char * lpszNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_pchar_SerifName,lpszNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
char *  LPUMA_GetSansSerifName()
{
        char * rc = "";
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_pchar_SansSerifName,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetSansSerifName(char * lpszNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_pchar_SansSerifName,lpszNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
char *  LPUMA_GetCourierName()
{
        char * rc = "";
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_pchar_CourierName,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetCourierName(char * lpszNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_pchar_CourierName,lpszNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetPictures(Word32 nNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Word32_Pictures,&nNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
Word32  LPUMA_GetPictures( void )
{
        Word32 rc = FALSE;
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_Word32_Pictures,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetTables(Word32 nNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Word32_Tables,&nNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
Word32  LPUMA_GetTables( void )
{
        Word32 rc = FALSE;
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_Word32_Tables,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
char *  LPUMA_GetVersion()
{
        char * rc = "";
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_pchar_Version,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
Int32  LPUMA_EnumLanguages(Int32 nPrev )
{
        Int32 rc = -1;
        if(s_hLib && EnumLanguages)
                rc = EnumLanguages(nPrev);
        return rc;
}
//////////////////////////////////////////////////////////////////
Int32  LPUMA_EnumFormats(Int32 nPrev )
{
        Int32 rc = -1;
        if(s_hLib && EnumFormats)
                rc = EnumFormats(nPrev );
        return rc;
}
//////////////////////////////////////////////////////////////////
Int32  LPUMA_EnumCodes(Int32 format, Int32 nPrev )
{
        Int32 rc = -1;
        if(s_hLib && EnumCodes)
                rc = EnumCodes(format,nPrev);
        return rc;
}
//////////////////////////////////////////////////////////////////
Int32  LPUMA_EnumFormatMode(Int32 nPrev )
{
        Int32 rc = -1;
        if(s_hLib && EnumFormatMode)
                rc = EnumFormatMode(nPrev );
        return rc;
}
//////////////////////////////////////////////////////////////////
Int32  LPUMA_EnumTable(Int32 nPrev )
{
        Int32 rc = -1;
        if(s_hLib && EnumTable)
                rc = EnumTable(nPrev );
        return rc;
}
//////////////////////////////////////////////////////////////////
Int32  LPUMA_EnumPicture(Int32 nPrev )
{
        Int32 rc = -1;
        if(s_hLib && EnumPicture)
                rc = EnumPicture(nPrev );
        return rc;
}
//////////////////////////////////////////////////////////////////
Word32  LPUMA_GetFormatMode()
{
        Word32 rc = 0;
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_Word32_Format,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetFormatMode(Word32 nNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Word32_Format,&nNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetUnrecogChar(Word8 nNewValue)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Word8_Format,&nNewValue))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
Word8   LPUMA_GetUnrecogChar()
{
        Word8 rc = 0;
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_Word8_Format,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
Bool32  LPUMA_SetProgressFunction(FNPUMA_ProgressStart fnStart,FNPUMA_ProgressStep fnStep,FNPUMA_ProgressFinish fnFinish)
{
        Bool32 rc = FALSE;
        if(s_hLib && SetImportData &&
                SetImportData(PUMA_FNPUMA_ProgressStart,fnStart) &&
                SetImportData(PUMA_FNPUMA_ProgressStep,fnStep) &&
                SetImportData(PUMA_FNPUMA_ProgressFinish,fnFinish)
                )
        {
                rc = TRUE;      // Ок!
        }

        return rc;
}
//////////////////////////////////////////////////////////////////
Bool32  LPUMA_XGetRotateDIB(void ** lpDIB, Point32 * p)
{
        Bool32 rc = FALSE;
        if(s_hLib && XGetRotateDIB)
                rc = XGetRotateDIB(lpDIB,p);

        return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetAutoRotate(Bool32 b)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Bool32_AutoRotate,&b))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
Bool32  LPUMA_GetAutoRotate( void )
{
        Bool32 rc = 0;
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_Bool32_AutoRotate,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetPageSize(Word32 width, Word32 height)
{
        Point32 p = { width,height };
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Point32_PageSize,&p))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
void    LPUMA_RenameImageName(char * name)
{
        if(s_hLib && fnRenameImageName)
        {
                fnRenameImageName(name);
        }

}
//////////////////////////////////////////////////////////////////
Bool32    LPUMA_XSetTemplate(Rect32 rect)
{
        Bool32 rc = FALSE;
        if(s_hLib && fnXSetTemplate)
        {
                rc = fnXSetTemplate(rect);
        }
return rc;
}
//////////////////////////////////////////////////////////////////
Bool32    LPUMA_XGetTemplate(Rect32 *pRect)
{
        Bool32 rc = FALSE;
        if(s_hLib && fnXGetTemplate)
        {
                rc = fnXGetTemplate(pRect);
        }
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetCurrentEdPage(Handle hEdPage)
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Handle_CurrentEdPage,&hEdPage))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
Handle  LPUMA_GetCurrentEdPage()
{
        Handle rc = 0;
        if(s_hLib && GetExportData &&
                GetExportData(PUMA_Handle_CurrentEdPage,&rc))
        {
                // Ок!
        }
return rc;
}
//////////////////////////////////////////////////////////////////
Bool32 LPUMA_Save(Handle hEdPage, const char * lpOutFileName, Int32 lnFormat, Int32 lnCode, Bool32 bAppend )
{
        Bool32 rc = FALSE;
        if(s_hLib && fnSave)
                rc = fnSave(hEdPage, lpOutFileName, lnFormat, lnCode, bAppend);
return rc;
}
//////////////////////////////////////////////////////////////////
void    LPUMA_SetPreserveLineBreaks( Bool32 b )
{
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Bool32_PreserveLineBreaks,&b))
        {
                // Ок!
        }
}
//////////////////////////////////////////////////////////////////
Bool32  LPUMA_GetPreserveLineBreaks( void )
{
        Bool32 rc = FALSE;
        if(s_hLib && SetImportData &&
           SetImportData(PUMA_Bool32_PreserveLineBreaks,&rc))
        {
                // Ок!
        }
        return rc;
}
//////////////////////////////////////////////////////////////////
Bool32    LPUMA_XOpenClbk(PUMAIMAGECALLBACK CallBack,const char * lpFileName)
{
        Bool32 rc = FALSE;
        if(s_hLib && fnXOpenClbk)
        {
                rc = fnXOpenClbk(CallBack,lpFileName);
        }
return rc;
}
//////////////////////////////////////////////////////////////////
Word32 LPUMA_SaveToMemory(Handle hEdPage, Int32 lnFormat, Int32 lnCode, char * lpMem, Word32 size )
{
        Bool32 rc = FALSE;
        if(s_hLib && fnSaveToMemory)
                rc = fnSaveToMemory(hEdPage, lnFormat, lnCode, lpMem, size);
return rc;
}
//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
#ifdef  ADD_CED
Word32  LCED_DeletePage(Handle hEdPage)
{
	Word32 rc = 0;
	typedef void (*FNDeletePage)(Handle hEdPage);
	FNDeletePage fn;

	if(peCED.fnGetExportData(CED_FNCED_DeletePage,&fn))
		fn(hEdPage);
	else
		rc = peCED.fnGetReturnCode();

return rc;
}
#endif
///////////////////////////
void    LPUMA_GetSpecialBuffer(char * szResult,Int32 *nResultLength)
{
        if(s_hLib && fnGetSpecialBuffer)
        {
                fnGetSpecialBuffer(szResult,nResultLength);
        }

}

Bool32    LPUMA_SetSpecialProject(Word8 nSpecPrj)
{
        if(s_hLib && fnSetSpecialProject)
        {
                return fnSetSpecialProject(nSpecPrj);
        }

	return FALSE;
}
