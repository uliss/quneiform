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

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                   T I G E R   O C R   L I B R A R Y                      //
//                                                                          //
//           Copyright (C) 1996 Cognitive Technology Ltd.                   //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
#define FILELIB_TIGERAPI
//////////////////////////////////////////////////////////////////////////////
# include <windows.h>
//////////////////////////////////////////////////////////////////////////////
# include "dlltypes.h"
# include "tgcall.h"
# include "tgcallbk.h"
# include "tgerrors.h"
# include "_tgcall.h"
//////////////////////////////////////////////////////////////////////////////
# define MESSAGE(s)
#define ST_OK		        0
#define ST_LIBRARY_NOT_LOAD	1
#define ST_ENTRY_NOT_FOUND	2
#define TIGERNAME     "Tiger32.dll"
//////////////////////////////////////////////////////////////////////////////
static char * strErrorMessage[] =
{
	"Ok!",
	"Library TIGER32.DLL not load.",
	"Entry point not found."
};
//////////////////////////////////////////////////////////////////////////////
static BOOL16 	Init   = FALSE;
static HMODULE  hTiger = NULL;
static uint16_t 	wStatus = ST_OK ;
//////////////////////////////////////////////////////////////////////////////
static void Tiger_SetStatus(uint16_t wStat)
{
	wStatus =  wStat;
}
//////////////////////////////////////////////////////////////////////////////
static char* Tiger_Message(uint16_t wStat)
{
return strErrorMessage[wStat];
}
//////////////////////////////////////////////////////////////////////////////
// Tiger entries
FNBOOL16  __Tiger_Load( fpTiger_Callback fpCallback,char* lpPath)
{
typedef BOOL16 (TIGERFUN(*)fp__Tiger_Load)( fpTiger_Callback ,char* );
fp__Tiger_Load proc;

  if(hTiger==NULL)
     return FALSE;

  proc= (fp__Tiger_Load)GetProcAddress(hTiger,TIGER_LOAD_FN);
  if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

 return (*proc)(fpCallback,lpPath);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 __BCR_Load( fpTiger_Callback fpCallback,char* lpPath)
{
typedef BOOL16  (TIGERFUN(*)fp__BCR_Load)( fpTiger_Callback ,char* );
fp__BCR_Load proc;

  if(hTiger==NULL)
     return FALSE;

  proc = (fp__BCR_Load)GetProcAddress(hTiger,TIGER_BCR_LOAD_FN);
  if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

 return (*proc)(fpCallback,lpPath);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 Cards_Init
(
    HANDLE    wTGCALL_DLL,      // Instance of TGCALL for callback
    char*   lpPath,           // Path name
    LPDWORD lpParams,         // Parameters pass buffer
    char*   lpBuffer          // Image pass buffer
)
{
typedef BOOL16  (TIGERFUN(*)lpCards_Init)
 (    HANDLE  , char*  ,    LPDWORD ,    char*   );
lpCards_Init proc;

 Init = FALSE;

 if(hTiger==NULL)
    return FALSE;

  proc = (lpCards_Init)GetProcAddress(hTiger,TIGER_CARDS_INIT_FN);

 if(proc)
    Init = (*proc)(wTGCALL_DLL,lpPath,lpParams,lpBuffer);
   else
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}
return Init;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 Tiger_Reset (void)
{
typedef BOOL16  (TIGERFUN(*)fpTiger_Reset) (void);
fpTiger_Reset proc;

if(!hTiger || !Init)
    return FALSE;

proc  = (fpTiger_Reset)GetProcAddress(hTiger,TIGER_RESET_FN);

  if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 Tiger_SetRecognitionOptions (uint16_t wOptions)
{
typedef BOOL16  (TIGERFUN(*)fpTiger_SetRecognitionOptions) (uint16_t);
fpTiger_SetRecognitionOptions proc;

if(!hTiger || !Init)
    return FALSE;

  proc  = (fpTiger_SetRecognitionOptions)GetProcAddress(hTiger,TIGER_SET_REC_OPT_FN);

  if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)(wOptions);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 Tiger_Recognition (char* lpEdFileName)
{
typedef BOOL16  (TIGERFUN(*)lpTiger_Recognition)(char* );
lpTiger_Recognition proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (lpTiger_Recognition)GetProcAddress(hTiger,TIGER_RECOG_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc) (lpEdFileName);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 Tiger_PageAnalysis (void)
{
typedef BOOL16  (TIGERFUN(*)fpTiger_PageAnalysis) (void);
fpTiger_PageAnalysis proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_PageAnalysis)GetProcAddress(hTiger,TIGER_PAGE_AN_FN);

 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc) ();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 Tiger_FinalRecognition (char* lpEdFileName)
{
typedef BOOL16  (TIGERFUN(*)lpTiger_FinalRecognition) (char*);
lpTiger_FinalRecognition proc;

if(!hTiger || !Init)
    return FALSE;

 proc  = (lpTiger_FinalRecognition)GetProcAddress(hTiger,
				 TIGER_FIN_RECOG_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc) (lpEdFileName);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNWORD Tiger_Status (void)
{
typedef uint16_t  (TIGERFUN(*)fpTiger_Status) (void);
fpTiger_Status proc;

if(!hTiger)
    return FALSE;

 proc = (fpTiger_Status)GetProcAddress(hTiger,TIGER_STATUS_FN);

 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNLPSTR Tiger_ErrorMessage (uint16_t wErrorNumber)
{
typedef char*  (TIGERFUN(*)lpTiger_ErrorMessage) (uint16_t);
lpTiger_ErrorMessage proc;

if(!hTiger)
 {
  Tiger_SetStatus(ST_LIBRARY_NOT_LOAD);
  return Tiger_Message(wStatus);
 }

 proc = (lpTiger_ErrorMessage)GetProcAddress(hTiger,
			    TIGER_ERR_MESSAGE_FN);

 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return Tiger_Message(wStatus);
   	}

return (*proc)(wErrorNumber);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 Tiger_EdToText
(
    char* lpEdFileName,
    char* lpTextFileName,
    uint16_t wOptions,
    uint16_t wBadCharacter
)
{
typedef BOOL16  (TIGERFUN(*)fpTiger_EdToText)(char* ,char*, uint16_t ,uint16_t );
fpTiger_EdToText proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_EdToText)GetProcAddress(hTiger,TIGER_ED_2_TEXT_FN);

 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)(lpEdFileName,lpTextFileName,wOptions,wBadCharacter);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 Tiger_ExtEdToText
(
    char* lpEdFileName,
    char* lpTextFileName,
    uint16_t wOptions,
    uint16_t wCodePage,
    uint16_t wBadCharacter
)
{
typedef BOOL16  (TIGERFUN(*)fpTiger_ExtEdToText)(char* ,char*, uint16_t, uint16_t ,uint16_t );
fpTiger_ExtEdToText proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_ExtEdToText)GetProcAddress(hTiger,TIGER_EXT_ED_2_TEXT_FN);

 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)(lpEdFileName,lpTextFileName,wOptions,wCodePage,wBadCharacter);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNWORD Tiger_ProgressList (LPWORD lpList, uint16_t wMode)
{
typedef uint16_t  (TIGERFUN(*)fpTiger_ProgressList) (LPWORD, uint16_t );
fpTiger_ProgressList proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_ProgressList)GetProcAddress(hTiger,
                              TIGER_PROGRESS_LIST_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)(lpList,wMode);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 TigerLayout_GetPageIncline (LPINT16 lpnNumerator, LPINT16 lpnDenominator)
{
typedef BOOL16   (TIGERFUN(*)fpTigerLayout_GetPageIncline)(char* , char* );
fpTigerLayout_GetPageIncline proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTigerLayout_GetPageIncline)GetProcAddress(
				hTiger,TIGER_LAY_GET_P_INCL_L_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)((char*)lpnNumerator, (char*)lpnDenominator);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 TigerLayout_UpdateBlocks (LPINT16 lpNumber,LPLPTIGER_LAYOUTBLOCK lplpBlocks)
{
typedef BOOL16  (TIGERFUN(*)fpTigerLayout_UpdateBlocks) (char* , char* );
fpTigerLayout_UpdateBlocks proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTigerLayout_UpdateBlocks)GetProcAddress(
				hTiger,TIGER_LAY_UPD_BL_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)((char*)lpNumber,(char*)lplpBlocks);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 TigerLayout_HorzSplitBlocks
(    POINT w1, POINT w2, LPBOOL16 lpSuccess)
{
typedef BOOL16  (TIGERFUN(*)lpTigerLayout_HorzSplitBlocks)
				(uint16_t ,uint16_t ,uint16_t ,uint16_t ,char* );
lpTigerLayout_HorzSplitBlocks  proc;

if(!hTiger || !Init)
    return FALSE;

  proc = (lpTigerLayout_HorzSplitBlocks)GetProcAddress(
					hTiger,TIGER_LAY_H_SP_BL_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)((uint16_t)w1.x,(uint16_t)w1.y,(uint16_t)w2.x,(uint16_t)w2.y,(char*)lpSuccess);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 TigerLayout_VertSplitBlocks
(    POINT w1,POINT w2, LPBOOL16 lpSuccess)
{
typedef BOOL16  (TIGERFUN(*)lpTigerLayout_VertSplitBlocks)
				(uint16_t ,uint16_t ,uint16_t ,uint16_t ,char* );
lpTigerLayout_VertSplitBlocks  proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (lpTigerLayout_VertSplitBlocks)GetProcAddress(
					hTiger,TIGER_LAY_V_SP_BL_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}


return (*proc)((uint16_t)w1.x,(uint16_t)w1.y,(uint16_t)w2.x,(uint16_t)w2.y,(char*)lpSuccess);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_ExtractRectBlock(RECT rect, LPBOOL16 lpSuccess)
{
   uint16_t l,t,r,b;
   typedef BOOL16    (TIGERFUN(*)fpTigerLayout_ExtractRectBlock)
                                      (uint16_t ,uint16_t ,uint16_t ,uint16_t ,char* );
fpTigerLayout_ExtractRectBlock proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTigerLayout_ExtractRectBlock)GetProcAddress(
				     hTiger,TIGER_LAY_EXT_RECT_BL_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}
   l=(uint16_t)rect.left;t=(uint16_t)rect.top;r=(uint16_t)rect.right;b=(uint16_t)rect.bottom;
/**
{ char buf[256];
sprintf(buf, "ERB: %u %u %u %u", l,t,r,b);
OutputDebugString(buf);
}
**/
return (*proc)( l,t,r,b, (char*)lpSuccess);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_ExtractPicture (RECT rect,LPBOOL16 lpSuccess)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_ExtractPicture)
                                      (uint16_t ,uint16_t ,uint16_t ,uint16_t ,char* );
fpTigerLayout_ExtractPicture proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTigerLayout_ExtractPicture)GetProcAddress(
				     hTiger,TIGER_LAY_EXT_PIC_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}


return (*proc)((uint16_t)rect.left,(uint16_t)rect.top,(uint16_t)rect.right,
               (uint16_t)rect.bottom,(char*)lpSuccess);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_GlueBlocks (uint16_t iBlock1, uint16_t iBlock2,
 LPBOOL16 lpSuccess)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_GlueBlocks) (uint16_t, uint16_t , char* );
fpTigerLayout_GlueBlocks proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTigerLayout_GlueBlocks)GetProcAddress(
			       hTiger,TIGER_LAY_GLUE_BL_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)((uint16_t)iBlock1,(uint16_t)iBlock2,(char*)lpSuccess);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_DeleteBlock (uint16_t iBlock, LPBOOL16 lpSuccess)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_DeleteBlock) (uint16_t , char* );
fpTigerLayout_DeleteBlock proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTigerLayout_DeleteBlock)GetProcAddress(
				hTiger,TIGER_LAY_DEL_BL_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)((uint16_t)iBlock,(char*)lpSuccess);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_EnglishBlock (uint16_t iBlock, LPBOOL16 lpSuccess)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_EnglishBlock) (uint16_t , char* );
fpTigerLayout_EnglishBlock proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTigerLayout_EnglishBlock)GetProcAddress(
				hTiger,TIGER_LAY_ENGL_BL_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)((uint16_t)iBlock,(char*)lpSuccess);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_Backup (LPBOOL16 lpbError)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_Backup) (char* );
fpTigerLayout_Backup proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTigerLayout_Backup)GetProcAddress(
				hTiger,TIGER_LAY_BACKUP_FN);

 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)((char*)lpbError);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_Undo (LPBOOL16 lpbError)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_Undo) (char* );
fpTigerLayout_Undo proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTigerLayout_Undo)GetProcAddress(
				hTiger,TIGER_LAY_UNDO_FN);

 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)((char*)lpbError);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_Redo (LPBOOL16 lpbError)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_Redo) (char* );
fpTigerLayout_Redo proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTigerLayout_Redo)GetProcAddress(
				hTiger,TIGER_LAY_REDO_FN);

 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)((char*)lpbError);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_SetLanguage(uint16_t lng)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_SetLanguage)(uint16_t);
fpTiger_SetLanguage proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_SetLanguage)GetProcAddress(hTiger,TIGER_SET_LANG_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}


return (*proc)(lng);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_OpenEdFile (char* lpFilename)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_OpenEdFile) (char* );
fpTiger_OpenEdFile proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_OpenEdFile)GetProcAddress(hTiger,
                                          TIGER_OPEN_ED_FILE_FN);
return (*proc)(lpFilename);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_GetEdPage (LPED_PAGE lpEdPage)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_GetEdPage) (char*);
fpTiger_GetEdPage proc;

if(!hTiger || !Init)
    return FALSE;

proc = (fpTiger_GetEdPage)GetProcAddress(hTiger,TIGER_GET_ED_PAGE_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)((char*)lpEdPage);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_GetEdFragment (uint16_t wFragmentID,
LPED_FRAGMENT  lpEdFragment)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_GetEdFragment) (uint16_t, char* );
fpTiger_GetEdFragment proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_GetEdFragment)GetProcAddress(hTiger,TIGER_GET_ED_FRAG_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)(wFragmentID,(char*)lpEdFragment);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16  Tiger_GetEdString (uint16_t wStringID,
LPED_STRING  lpEdString)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_GetEdString) (uint16_t, char* );
fpTiger_GetEdString proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_GetEdString)GetProcAddress(hTiger,
				TIGER_GET_ED_STR_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}


return (*proc)(wStringID,(char*) lpEdString);
}

FNBOOL16 Tiger_GetEdWord (uint16_t wWordID,LPED_WORD  lpEdWord)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_GetEdWord) (uint16_t, char* );
fpTiger_GetEdWord proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_GetEdWord)GetProcAddress(hTiger,TIGER_GET_ED_WORD_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)(wWordID,(char*)lpEdWord);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_GetEdCharacter (uint16_t wCharacterID,
LPED_CHARACTER  lpEdCharacter)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_GetEdCharacter) (uint16_t , char* );
fpTiger_GetEdCharacter proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_GetEdCharacter)GetProcAddress(hTiger,TIGER_GET_ED_CHAR_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)(wCharacterID,(char*)lpEdCharacter);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_CloseEdFile (void)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_CloseEdFile) (void);
fpTiger_CloseEdFile proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_CloseEdFile)GetProcAddress(hTiger,
				TIGER_CLOSE_ED_FILE_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_SetEdSaveRtfOptions (
	uint16_t wFrame,
	char* lpNameSerif,
	char* lpNameNonSerif,
	char* lpNameMono,
	char* lpSerif,
	char* lpNonSerif,
	char* lpMono
	)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_SetEdSaveRtfOptions) (
			uint16_t ,char* ,char* ,char* ,char* ,char* ,char*);
fpTiger_SetEdSaveRtfOptions proc;
if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_SetEdSaveRtfOptions)GetProcAddress(
				hTiger,TIGER_SET_ED_SAV_RTF_OPT_FN);

 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)(wFrame,lpNameSerif,lpNameNonSerif,
	lpNameMono,lpSerif,lpNonSerif,lpMono);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_UpdateSheets (uint16_t FuncNumber, LPINT16 * lplpSheets)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_UpdateSheets) (uint16_t, char* );
fpTigerLayout_UpdateSheets proc;

if(!hTiger || !Init)
    return FALSE;

proc = (fpTigerLayout_UpdateSheets)GetProcAddress(
				hTiger,TIGER_LAY_UPD_SHEETS_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)(FuncNumber,(char*)lplpSheets);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_RW_Options (uint16_t FuncNumber, char* lpData)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_RW_Options) (uint16_t, char* );
fpTiger_RW_Options proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_RW_Options)GetProcAddress(hTiger,
				TIGER_RW_OPTIONS_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}


return (*proc)(FuncNumber,lpData);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_Load (HANDLE hApplicationInstance, char* lpTigerPathName)
{
char TigerFileName[256];

    MESSAGE ("_Tiger_Load");

    hApplicationInstance=hApplicationInstance;

    if(Init)
         return Init;

    if((strlen(lpTigerPathName)+strlen(TIGERNAME))>=256)
	return FALSE;

    if(lpTigerPathName)
      {
     strcpy(TigerFileName,lpTigerPathName);
     strcat(TigerFileName,TIGERNAME);
      }
    else
     strcpy(TigerFileName,TIGERNAME);

    hTiger = LoadLibrary(TigerFileName);

    if(hTiger==NULL)
	 return FALSE;

    Init = __Tiger_Load (Tiger_CallBack, lpTigerPathName);

     return Init;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 BCR_Load (HANDLE hApplicationInstance, char* lpTigerPathName)
{
char TigerFileName[256];

    MESSAGE ("BCR_Load");
    hApplicationInstance=hApplicationInstance;

    if(Init)
	return Init;

    if((strlen(lpTigerPathName)+strlen(TIGERNAME))>=256)
	return FALSE;

    if(lpTigerPathName)
      {
     strcpy(TigerFileName,lpTigerPathName);
     strcat(TigerFileName,TIGERNAME);
      }
    else
     strcpy(TigerFileName,TIGERNAME);

    hTiger = LoadLibrary(TigerFileName);

    if(hTiger==NULL)
	 return FALSE;

    Init = __BCR_Load (Tiger_CallBack, lpTigerPathName);

    return Init;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_Unload (void)
{
BOOL16 ret = FALSE;

    MESSAGE ("Tiger_Unload");

    if(hTiger)
    	{
	ret = FreeLibrary(hTiger);
	if(hTiger) hTiger = NULL;
	if(ret) Init = FALSE;
	}

return ret;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Call Back entries
CBDWORD Tiger_CallBack (uint16_t wFuncNumber, LPDWORD lpParams)
{
    switch (wFuncNumber)
    {
        default:
            return ((uint32_t) 0);

        case TCB_IMAGE_OPEN_XY:
            return ((uint32_t) Tiger_Callback_ImageOpen_xy
                            (
                                (LPTIGER_IMAGEINFO) lpParams [0],
                                (uint16_t)              lpParams [1],
                                (uint16_t)              lpParams [2],
                                (uint16_t)              lpParams [3],
                                (uint16_t)              lpParams [4]
                            ));
        case TCB_PROCESS_PICTURES: // Valdemar
            return ((uint32_t) Tiger_Callback_ProcessPictures
                            (
                                (char*)  lpParams [0],
                                (uint32_t)   lpParams [1]
                            ));

        case TCB_IMAGE_OPEN:
            return ((uint32_t) Tiger_Callback_ImageOpen
                            (
                                (LPTIGER_IMAGEINFO) lpParams [0]
                            ));

        case TCB_IMAGE_READ:
            return ((uint32_t) Tiger_Callback_ImageRead
                            (
                                (char*)  lpParams [0],
                                (uint16_t)   lpParams [1]
                            ));

        case TCB_IMAGE_CLOSE:
            return ((uint32_t) Tiger_Callback_ImageClose ());

        case TCB_REPORT_ERROR:
            Tiger_Callback_ReportError ((uint16_t) lpParams [0], (char*) lpParams [1]);
            return ((uint32_t) TRUE);

        case TCB_PROGRESS_START:
            return ((uint32_t) Tiger_Callback_ProgressStart ());

        case TCB_PROGRESS_FINISH:
            return ((uint32_t) Tiger_Callback_ProgressFinish ());

        case TCB_PROGRESS_SET_PERCENTAGE:
            return ((uint32_t) Tiger_Callback_ProgressSetPerc ((uint16_t) lpParams [0]));

        case TCB_PROGRESS_SET_STAGE:
            return ((uint32_t) Tiger_Callback_ProgressSetStage
                            (
                                (uint16_t)  lpParams [0],
                                (char*) lpParams [1],
                                (uint16_t)  lpParams [2]
                            ));
		case TCB_TIGER_GET_WORD:
			return ((uint32_t) Tiger_Callback_Get_One_Word( (char*) lpParams[0] ));
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNWORD Tiger_GetLanguage()
{
typedef uint16_t (TIGERFUN(*)fpTiger_GetLanguage)();
fpTiger_GetLanguage proc;

if(!hTiger || !Init)
    return (uint16_t)-1;

 proc = (fpTiger_GetLanguage)GetProcAddress(hTiger,
                                TIGER_GET_LANG_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
        return (uint16_t)-1;
   	}

return (*proc)();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNWORD Tiger_EnumLanguage(uint16_t prev)
{
typedef uint16_t (TIGERFUN(*)fpTiger_EnumLanguage)(uint16_t);
fpTiger_EnumLanguage proc;

if(!hTiger || !Init)
    return (uint16_t)-1;

 proc = (fpTiger_EnumLanguage)GetProcAddress(hTiger,
                                TIGER_ENUM_LANG_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
        return (uint16_t)-1;
   	}

return (*proc)(prev);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL Tiger_SpellerExist(uint16_t language)
{
typedef Bool (TIGERFUN(*)fpTiger_SpellerExist)(uint16_t);
fpTiger_SpellerExist proc;

if(!hTiger || !Init)
    return (uint16_t)-1;

 proc = (fpTiger_SpellerExist)GetProcAddress(hTiger,
                                TIGER_SPELL_EXIST_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
        return FALSE;
   	}

return (*proc)(language);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_RunWithDIB( void * lpDIB, void * lpRes )
{
	typedef Bool (TIGERFUN(*)fpTigerRunWithDIB)(void *, void *);
	fpTigerRunWithDIB proc;

	if ( !hTiger || !Init )
		return (uint16_t) -1;

	proc = (fpTigerRunWithDIB)GetProcAddress(hTiger,
		                                     TIGER_RUN_WITH_DIB_FN);

	if ( proc == NULL )
	{
		Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
		return FALSE;
	}

	return (*proc)(lpDIB, lpRes);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//end of file
