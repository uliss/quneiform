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
static WORD 	wStatus = ST_OK ;
//////////////////////////////////////////////////////////////////////////////
static void Tiger_SetStatus(WORD wStat)
{
	wStatus =  wStat;
}
//////////////////////////////////////////////////////////////////////////////
static LPSTR Tiger_Message(WORD wStat)
{
return strErrorMessage[wStat];
}
//////////////////////////////////////////////////////////////////////////////
// Tiger entries
FNBOOL16  __Tiger_Load( fpTiger_Callback fpCallback,LPSTR lpPath)
{
typedef BOOL16 (TIGERFUN(*)fp__Tiger_Load)( fpTiger_Callback ,LPSTR );
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
FNBOOL16 __BCR_Load( fpTiger_Callback fpCallback,LPSTR lpPath)
{
typedef BOOL16  (TIGERFUN(*)fp__BCR_Load)( fpTiger_Callback ,LPSTR );
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
    LPSTR   lpPath,           // Path name
    LPDWORD lpParams,         // Parameters pass buffer
    LPSTR   lpBuffer          // Image pass buffer
)
{
typedef BOOL16  (TIGERFUN(*)lpCards_Init)
 (    HANDLE  , LPSTR  ,    LPDWORD ,    LPSTR   );
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
FNBOOL16 Tiger_SetRecognitionOptions (WORD wOptions)
{
typedef BOOL16  (TIGERFUN(*)fpTiger_SetRecognitionOptions) (WORD);
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
FNBOOL16 Tiger_Recognition (LPSTR lpEdFileName)
{
typedef BOOL16  (TIGERFUN(*)lpTiger_Recognition)(LPSTR );
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
FNBOOL16 Tiger_FinalRecognition (LPSTR lpEdFileName)
{
typedef BOOL16  (TIGERFUN(*)lpTiger_FinalRecognition) (LPSTR);
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
typedef WORD  (TIGERFUN(*)fpTiger_Status) (void);
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
FNLPSTR Tiger_ErrorMessage (WORD wErrorNumber)
{
typedef LPSTR  (TIGERFUN(*)lpTiger_ErrorMessage) (WORD);
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
    LPSTR lpEdFileName,
    LPSTR lpTextFileName,
    WORD wOptions,
    WORD wBadCharacter
)
{
typedef BOOL16  (TIGERFUN(*)fpTiger_EdToText)(LPSTR ,LPSTR, WORD ,WORD );
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
    LPSTR lpEdFileName,
    LPSTR lpTextFileName,
    WORD wOptions,
    WORD wCodePage,
    WORD wBadCharacter
)
{
typedef BOOL16  (TIGERFUN(*)fpTiger_ExtEdToText)(LPSTR ,LPSTR, WORD, WORD ,WORD );
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
FNWORD Tiger_ProgressList (LPWORD lpList, WORD wMode)
{
typedef WORD  (TIGERFUN(*)fpTiger_ProgressList) (LPWORD, WORD );
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
typedef BOOL16   (TIGERFUN(*)fpTigerLayout_GetPageIncline)(LPSTR , LPSTR );
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

return (*proc)((LPSTR)lpnNumerator, (LPSTR)lpnDenominator);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 TigerLayout_UpdateBlocks (LPINT16 lpNumber,LPLPTIGER_LAYOUTBLOCK lplpBlocks)
{
typedef BOOL16  (TIGERFUN(*)fpTigerLayout_UpdateBlocks) (LPSTR , LPSTR );
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

return (*proc)((LPSTR)lpNumber,(LPSTR)lplpBlocks);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 TigerLayout_HorzSplitBlocks
(    POINT w1, POINT w2, LPBOOL16 lpSuccess)
{
typedef BOOL16  (TIGERFUN(*)lpTigerLayout_HorzSplitBlocks)
				(WORD ,WORD ,WORD ,WORD ,LPSTR );
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

return (*proc)((WORD)w1.x,(WORD)w1.y,(WORD)w2.x,(WORD)w2.y,(LPSTR)lpSuccess);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
FNBOOL16 TigerLayout_VertSplitBlocks
(    POINT w1,POINT w2, LPBOOL16 lpSuccess)
{
typedef BOOL16  (TIGERFUN(*)lpTigerLayout_VertSplitBlocks)
				(WORD ,WORD ,WORD ,WORD ,LPSTR );
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


return (*proc)((WORD)w1.x,(WORD)w1.y,(WORD)w2.x,(WORD)w2.y,(LPSTR)lpSuccess);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_ExtractRectBlock(RECT rect, LPBOOL16 lpSuccess)
{
   WORD l,t,r,b;
   typedef BOOL16    (TIGERFUN(*)fpTigerLayout_ExtractRectBlock)
                                      (WORD ,WORD ,WORD ,WORD ,LPSTR );
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
   l=(WORD)rect.left;t=(WORD)rect.top;r=(WORD)rect.right;b=(WORD)rect.bottom;
/**
{ char buf[256];
sprintf(buf, "ERB: %u %u %u %u", l,t,r,b);
OutputDebugString(buf);
}
**/
return (*proc)( l,t,r,b, (LPSTR)lpSuccess);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_ExtractPicture (RECT rect,LPBOOL16 lpSuccess)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_ExtractPicture)
                                      (WORD ,WORD ,WORD ,WORD ,LPSTR );
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


return (*proc)((WORD)rect.left,(WORD)rect.top,(WORD)rect.right,
               (WORD)rect.bottom,(LPSTR)lpSuccess);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_GlueBlocks (WORD iBlock1, WORD iBlock2,
 LPBOOL16 lpSuccess)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_GlueBlocks) (WORD, WORD , LPSTR );
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

return (*proc)((WORD)iBlock1,(WORD)iBlock2,(LPSTR)lpSuccess);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_DeleteBlock (WORD iBlock, LPBOOL16 lpSuccess)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_DeleteBlock) (WORD , LPSTR );
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

return (*proc)((WORD)iBlock,(LPSTR)lpSuccess);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_EnglishBlock (WORD iBlock, LPBOOL16 lpSuccess)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_EnglishBlock) (WORD , LPSTR );
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

return (*proc)((WORD)iBlock,(LPSTR)lpSuccess);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_Backup (LPBOOL16 lpbError)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_Backup) (LPSTR );
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

return (*proc)((LPSTR)lpbError);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_Undo (LPBOOL16 lpbError)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_Undo) (LPSTR );
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

return (*proc)((LPSTR)lpbError);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 TigerLayout_Redo (LPBOOL16 lpbError)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_Redo) (LPSTR );
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

return (*proc)((LPSTR)lpbError);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_SetLanguage(WORD lng)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_SetLanguage)(WORD);
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
FNBOOL16 Tiger_OpenEdFile (LPSTR lpFilename)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_OpenEdFile) (LPSTR );
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
typedef BOOL16    (TIGERFUN(*)fpTiger_GetEdPage) (LPSTR);
fpTiger_GetEdPage proc;

if(!hTiger || !Init)
    return FALSE;

proc = (fpTiger_GetEdPage)GetProcAddress(hTiger,TIGER_GET_ED_PAGE_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)((LPSTR)lpEdPage);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_GetEdFragment (WORD wFragmentID,
LPED_FRAGMENT  lpEdFragment)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_GetEdFragment) (WORD, LPSTR );
fpTiger_GetEdFragment proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_GetEdFragment)GetProcAddress(hTiger,TIGER_GET_ED_FRAG_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)(wFragmentID,(LPSTR)lpEdFragment);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16  Tiger_GetEdString (WORD wStringID,
LPED_STRING  lpEdString)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_GetEdString) (WORD, LPSTR );
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


return (*proc)(wStringID,(LPSTR) lpEdString);
}

FNBOOL16 Tiger_GetEdWord (WORD wWordID,LPED_WORD  lpEdWord)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_GetEdWord) (WORD, LPSTR );
fpTiger_GetEdWord proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_GetEdWord)GetProcAddress(hTiger,TIGER_GET_ED_WORD_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)(wWordID,(LPSTR)lpEdWord);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_GetEdCharacter (WORD wCharacterID,
LPED_CHARACTER  lpEdCharacter)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_GetEdCharacter) (WORD , LPSTR );
fpTiger_GetEdCharacter proc;

if(!hTiger || !Init)
    return FALSE;

 proc = (fpTiger_GetEdCharacter)GetProcAddress(hTiger,TIGER_GET_ED_CHAR_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
	return FALSE;
   	}

return (*proc)(wCharacterID,(LPSTR)lpEdCharacter);
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
	WORD wFrame,
	LPSTR lpNameSerif,
	LPSTR lpNameNonSerif,
	LPSTR lpNameMono,
	LPSTR lpSerif,
	LPSTR lpNonSerif,
	LPSTR lpMono
	)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_SetEdSaveRtfOptions) (
			WORD ,LPSTR ,LPSTR ,LPSTR ,LPSTR ,LPSTR ,LPSTR);
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
FNBOOL16 TigerLayout_UpdateSheets (WORD FuncNumber, LPINT16 * lplpSheets)
{
typedef BOOL16    (TIGERFUN(*)fpTigerLayout_UpdateSheets) (WORD, LPSTR );
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

return (*proc)(FuncNumber,(LPSTR)lplpSheets);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL16 Tiger_RW_Options (WORD FuncNumber, LPSTR lpData)
{
typedef BOOL16    (TIGERFUN(*)fpTiger_RW_Options) (WORD, LPSTR );
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
FNBOOL16 Tiger_Load (HANDLE hApplicationInstance, LPSTR lpTigerPathName)
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
FNBOOL16 BCR_Load (HANDLE hApplicationInstance, LPSTR lpTigerPathName)
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
CBDWORD Tiger_CallBack (WORD wFuncNumber, LPDWORD lpParams)
{
    switch (wFuncNumber)
    {
        default:
            return ((uint32_t) 0);

        case TCB_IMAGE_OPEN_XY:
            return ((uint32_t) Tiger_Callback_ImageOpen_xy
                            (
                                (LPTIGER_IMAGEINFO) lpParams [0],
                                (WORD)              lpParams [1],
                                (WORD)              lpParams [2],
                                (WORD)              lpParams [3],
                                (WORD)              lpParams [4]
                            ));
        case TCB_PROCESS_PICTURES: // Valdemar
            return ((uint32_t) Tiger_Callback_ProcessPictures
                            (
                                (LPSTR)  lpParams [0],
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
                                (LPSTR)  lpParams [0],
                                (WORD)   lpParams [1]
                            ));

        case TCB_IMAGE_CLOSE:
            return ((uint32_t) Tiger_Callback_ImageClose ());

        case TCB_REPORT_ERROR:
            Tiger_Callback_ReportError ((WORD) lpParams [0], (LPSTR) lpParams [1]);
            return ((uint32_t) TRUE);

        case TCB_PROGRESS_START:
            return ((uint32_t) Tiger_Callback_ProgressStart ());

        case TCB_PROGRESS_FINISH:
            return ((uint32_t) Tiger_Callback_ProgressFinish ());

        case TCB_PROGRESS_SET_PERCENTAGE:
            return ((uint32_t) Tiger_Callback_ProgressSetPerc ((WORD) lpParams [0]));

        case TCB_PROGRESS_SET_STAGE:
            return ((uint32_t) Tiger_Callback_ProgressSetStage
                            (
                                (WORD)  lpParams [0],
                                (LPSTR) lpParams [1],
                                (WORD)  lpParams [2]
                            ));
		case TCB_TIGER_GET_WORD:
			return ((uint32_t) Tiger_Callback_Get_One_Word( (LPSTR) lpParams[0] ));
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNWORD Tiger_GetLanguage()
{
typedef WORD (TIGERFUN(*)fpTiger_GetLanguage)();
fpTiger_GetLanguage proc;

if(!hTiger || !Init)
    return (WORD)-1;

 proc = (fpTiger_GetLanguage)GetProcAddress(hTiger,
                                TIGER_GET_LANG_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
        return (WORD)-1;
   	}

return (*proc)();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNWORD Tiger_EnumLanguage(WORD prev)
{
typedef WORD (TIGERFUN(*)fpTiger_EnumLanguage)(WORD);
fpTiger_EnumLanguage proc;

if(!hTiger || !Init)
    return (WORD)-1;

 proc = (fpTiger_EnumLanguage)GetProcAddress(hTiger,
                                TIGER_ENUM_LANG_FN);
 if(proc==NULL)
	{
  	Tiger_SetStatus(ST_ENTRY_NOT_FOUND);
        return (WORD)-1;
   	}

return (*proc)(prev);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
FNBOOL Tiger_SpellerExist(WORD language)
{
typedef BOOL (TIGERFUN(*)fpTiger_SpellerExist)(WORD);
fpTiger_SpellerExist proc;

if(!hTiger || !Init)
    return (WORD)-1;

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
	typedef BOOL (TIGERFUN(*)fpTigerRunWithDIB)(void *, void *);
	fpTigerRunWithDIB proc;

	if ( !hTiger || !Init )
		return (WORD) -1;

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
