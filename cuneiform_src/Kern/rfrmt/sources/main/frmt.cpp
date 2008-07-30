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

// ============================================================================
// Written by Shahverdiev  Alik
// This file cration date: 10.18.98
//
// Frmt.cpp
//
// ============================================================================

//#include <afxtempl.h>
//#include <afxwin.h>
//#include <windows.h>
#include <stdlib.h>
#include "stdafx.h"
#include "resource.h"
#define __RFRMT__
#include "rfrmt.h"
#include "mymem.h"
#include "rtffragrect.h"
#include "dpuma.h"
#include "aldebug.h"
#include "rfrmt_prot.h"
#include <cstring>

#define  USE_NONE             0x0040   // no formatting
#define  USE_FRAME_AND_COLUMN 0x0001   // use columns & frames
#define  USE_FRAME            0x0002   // use only frames

#define  NOBOLD               0x0004
#define  NOCURSIV             0x0008
#define  NOSIZE               0x0020

Word32   FlagMode;
Word32   ExFlagMode;

Word32   RtfWriteMode;
CHAR     RtfFileName[MAX_PATH];
CHAR     WriteRtfImageName[MAX_PATH];
CHAR     lpMyNameSerif[MAX_PATH];
CHAR     lpMyNameNonSerif[MAX_PATH];
CHAR     lpMyNameMono[MAX_PATH];

Bool32   FlagChangeSizeKegl;
Bool32   FlagLineTransfer;
Bool32   FlagDebugAlign;

extern   Bool32   gbBold		;
extern   Bool32   gbItalic;
extern   Bool32   gbSize		;
extern   Word32   gnFormat;
extern   char*    gpSerifName;
extern   char*    gpSansSerifName;
extern   char*    gpCourierName;
extern   Word32   CountTable;

// extern functions
void    SetReturnCode_rfrmt(Word16 rc);
Word16  GetReturnCode_rfrmt();

#ifdef alDebug
std::vector <tagRECT>   *pInputArray;
std::vector <tagRECT>   *pTheGeomStep1;
std::vector <tagRECT>   *pTheGeomStep2;
std::vector <tagRECT>   *pTheGeomTemp ;
vectorWord             *pFragRectColor;
WORD                   *CountRect;
WORD                    Draw_Step;
WORD                    Draw_Cycle;
#endif
///////////////////////////////////////////////////////////////

Handle hDbgWnd = NULL;
extern Handle hDebugMy;
extern Handle hDebugKegl;
extern Handle hDebugFrame;
extern Handle hDebugLineTransfer;
extern Handle hDebugAlign;

//###########################################
#define New On
RFRMT_FUNC(Bool32)  RFRMT_Formatter(char* lpInputImageName , Handle* PtrEdTree )
{
#ifdef New
 char* lpInternalFileName = "internal.vit";
 LDPUMA_Skip(hDebugProfStart);

 FlagMode = 0;
 ExFlagMode = FALSE;

 RtfWriteMode = FALSE;
 strcpy((char*)WriteRtfImageName,lpInputImageName);

#ifdef alDebug
CRtfFragRect  RtfFragRect;
RtfFragRect.m_arInputFragRect.clear()    ;
RtfFragRect.m_arGeomFragRectStep1.clear();
RtfFragRect.m_arGeomFragRectStep2.clear();
RtfFragRect.m_arGeomFragRectTemp.clear() ;
RtfFragRect.m_arGeomFragRectColor.clear();
RtfFragRect.m_GeomFragCountRect           = 0;
RtfFragRect.m_Cycle                       = 0;
RtfFragRect.m_Step                        = 0;

#ifdef GLOBAL_DEBUG_AND_DRAW
 RtfFragRect.m_Cycle = 1;
#else
 RtfFragRect.m_Cycle = 0;
#endif

 pInputArray    = &RtfFragRect.m_arInputFragRect     ;
 pTheGeomStep1  = &RtfFragRect.m_arGeomFragRectStep1 ;
 pTheGeomStep2  = &RtfFragRect.m_arGeomFragRectStep2 ;
 pTheGeomTemp   = &RtfFragRect.m_arGeomFragRectTemp  ;
 pFragRectColor = &RtfFragRect.m_arGeomFragRectColor ;
 CountRect      = &RtfFragRect.m_GeomFragCountRect   ;
 Draw_Step      =  RtfFragRect.m_Step                ;
 Draw_Cycle     =  RtfFragRect.m_Cycle               ;
#endif

 SetReturnCode_rfrmt(IDS_ERR_NO);
 if(CreateInternalFileForFormatter() == FALSE)
   return FALSE;

 if( !gbBold )      FlagMode |= NOBOLD;
 if( !gbItalic )    FlagMode |= NOCURSIV;
 if( !gbSize )      FlagMode |= NOSIZE;

 if(!LDPUMA_Skip(hDebugFrame))
	 FlagMode |= USE_FRAME;
 else
   if( gnFormat==1 && ExFlagMode==FALSE)
     FlagMode |= USE_FRAME_AND_COLUMN;
   else
     FlagMode |= USE_NONE;

 strcpy( (char*)lpMyNameSerif	   , gpSerifName     );
 strcpy( (char*)lpMyNameNonSerif	, gpSansSerifName );
 strcpy( (char*)lpMyNameMono 	   , gpCourierName   );

 FlagChangeSizeKegl=TRUE;
 if(!LDPUMA_Skip(hDebugKegl))
	FlagChangeSizeKegl=FALSE;
 else
    FlagChangeSizeKegl=TRUE;

 FlagLineTransfer=FALSE;
 if(!LDPUMA_Skip(hDebugLineTransfer))
	FlagLineTransfer=TRUE;
 else
    FlagLineTransfer=FALSE;

 FlagDebugAlign=FALSE;
 if(!LDPUMA_Skip(hDebugAlign))
	FlagDebugAlign=TRUE;
 else
    FlagDebugAlign=FALSE;

 if( !FullRtf(lpInternalFileName, NULL, PtrEdTree) )
	return FALSE;

 if (!unlink(lpInternalFileName))
 {
	char ch[500];
	sprintf(ch,"File %s cannot be removed\n",lpInternalFileName);
	assert (ch);
 }

 #ifdef alDebug
  if(!LDPUMA_Skip(hDebugMy)){
    int i;
	for( i=0;i<pInputArray->size();i++)
    {
	    RECT rect = (*pInputArray)[i];
	    Rect16 rect16 = {(Int16)rect.left,(Int16)rect.top,(Int16)rect.right,(Int16)rect.bottom};
	    LDPUMA_DrawRect(hDbgWnd,&rect16, 0, RGB(0,127,0),1,(Word32)hDbgWnd);
    }

	LDPUMA_Console("Press any key...pInputArray");
	LDPUMA_WaitUserInput(hDebugMy,hDbgWnd);
	LDPUMA_DeleteRects(hDbgWnd,(Word32)hDbgWnd);

	for( i=0;i<pTheGeomStep1->size();i++)
    {
		RECT rect = (*pTheGeomStep1)[i];
		Rect16 rect16 = {(Int16)rect.left,(Int16)rect.top,(Int16)rect.right,(Int16)rect.bottom};
		LDPUMA_DrawRect(hDbgWnd,&rect16, 0, RGB(0,127,0),1,(Word32)hDbgWnd);
    }

 	LDPUMA_Console("Press any key...pTheGeomStep1");
	LDPUMA_WaitUserInput(hDebugMy,hDbgWnd);
    LDPUMA_DeleteRects(hDbgWnd,(Word32)hDbgWnd);

    for( i=0;i<pTheGeomStep2->size();i++)
    {
	    RECT rect = (*pTheGeomStep2)[i];
	    Rect16 rect16 = {(Int16)rect.left,(Int16)rect.top,(Int16)rect.right,(Int16)rect.bottom};
	    LDPUMA_DrawRect(hDbgWnd,&rect16, 0, RGB(0,127,0),1,(Word32)hDbgWnd);
    }

	LDPUMA_Console("Press any key...pTheGeomStep2");
	LDPUMA_WaitUserInput(hDebugMy,hDbgWnd);
    LDPUMA_DeleteRects(hDbgWnd,(Word32)hDbgWnd);

  }
#endif

  LDPUMA_Skip(hDebugProfEnd);
  return TRUE;
#else
  strcpy(WriteRtfImageName,lpInputImageName);
  return TRUE;
#endif
}

//###########################################
RFRMT_FUNC(Bool32)  RFRMT_SaveRtf(char* lpOutputFileName,Word32 code)
{
 char* lpInternalFileName = "internal.vit";
 LDPUMA_Skip(hDebugProfStart);

 FlagMode = 0;
 ExFlagMode = FALSE;
 RtfWriteMode = TRUE;
 if(!LDPUMA_Skip(hDebugMy)){
   hDbgWnd = LDPUMA_CreateWindow("Форматирование",LDPUMA_GetDIBptr(NULL));
 }

 strcpy( (char*)RtfFileName, lpOutputFileName );

#ifdef alDebug
 CRtfFragRect  RtfFragRect;
 RtfFragRect.m_arInputFragRect.clear()    ;
 RtfFragRect.m_arGeomFragRectStep1.clear();
 RtfFragRect.m_arGeomFragRectStep2.clear();
 RtfFragRect.m_arGeomFragRectTemp.clear() ;
 RtfFragRect.m_arGeomFragRectColor.clear();
 RtfFragRect.m_GeomFragCountRect           = 0;
 RtfFragRect.m_Cycle                       = 0;
 RtfFragRect.m_Step                        = 0;

#ifdef GLOBAL_DEBUG_AND_DRAW
 RtfFragRect.m_Cycle = 1;
#else
 RtfFragRect.m_Cycle = 0;
#endif

 pInputArray    = &RtfFragRect.m_arInputFragRect     ;
 pTheGeomStep1  = &RtfFragRect.m_arGeomFragRectStep1 ;
 pTheGeomStep2  = &RtfFragRect.m_arGeomFragRectStep2 ;
 pTheGeomTemp   = &RtfFragRect.m_arGeomFragRectTemp  ;
 pFragRectColor = &RtfFragRect.m_arGeomFragRectColor ;
 CountRect      = &RtfFragRect.m_GeomFragCountRect   ;
 Draw_Step      =  RtfFragRect.m_Step                ;
 Draw_Cycle     =  RtfFragRect.m_Cycle               ;
#endif

 SetReturnCode_rfrmt(IDS_ERR_NO);
 if(CreateInternalFileForFormatter() == FALSE){
 	LDPUMA_Skip(hDebugProfEnd);
	return FALSE;
 }

 if( !gbBold )      FlagMode |= NOBOLD;
 if( !gbItalic )    FlagMode |= NOCURSIV;
 if( !gbSize )      FlagMode |= NOSIZE;

 if(!LDPUMA_Skip(hDebugFrame))
   FlagMode |= USE_FRAME;
 else
   if( gnFormat==1 && ExFlagMode==FALSE)
      FlagMode |= USE_FRAME_AND_COLUMN;
   else
      FlagMode |= USE_NONE;

 strcpy( (char*)lpMyNameSerif	   , gpSerifName     );
 strcpy( (char*)lpMyNameNonSerif	, gpSansSerifName );
 strcpy( (char*)lpMyNameMono 	   , gpCourierName   );

 FlagChangeSizeKegl=TRUE;
 if(!LDPUMA_Skip(hDebugKegl))
	FlagChangeSizeKegl=FALSE;
 else
    FlagChangeSizeKegl=TRUE;

 FlagLineTransfer=FALSE;
 if(!LDPUMA_Skip(hDebugLineTransfer))
	FlagLineTransfer=TRUE;
 else
    FlagLineTransfer=FALSE;

 FlagDebugAlign=FALSE;
 if(!LDPUMA_Skip(hDebugAlign))
	FlagDebugAlign=TRUE;
 else
    FlagDebugAlign=FALSE;

 if(!FullRtf(lpInternalFileName,lpOutputFileName,NULL) ){
	LDPUMA_Skip(hDebugProfEnd);
	return FALSE;
 }

 if (!unlink(lpInternalFileName))
 {
	char ch[500];
	sprintf(ch,"File %s cannot be removed\n",lpInternalFileName);
	assert (ch);
 }

#ifdef alDebug
 if(!LDPUMA_Skip(hDebugMy)){
   int i;
   for( i=0;i<pInputArray->size();i++)
   {
	    RECT rect = (*pInputArray)[i];
	    Rect16 rect16 = {(Int16)rect.left,(Int16)rect.top,(Int16)rect.right,(Int16)rect.bottom};
	    LDPUMA_DrawRect(hDbgWnd,&rect16, 0, RGB(0,127,0),1,(Word32)hDbgWnd);
   }

   LDPUMA_Console("Press any key...pInputArray");
   LDPUMA_WaitUserInput(hDebugMy,hDbgWnd);
   LDPUMA_DeleteRects(hDbgWnd,(Word32)hDbgWnd);

   for( i=0;i<pTheGeomStep1->size();i++)
   {
		RECT rect = (*pTheGeomStep1)[i];
		Rect16 rect16 = {(Int16)rect.left,(Int16)rect.top,(Int16)rect.right,(Int16)rect.bottom};
		LDPUMA_DrawRect(hDbgWnd,&rect16, 0, RGB(0,127,0),1,(Word32)hDbgWnd);
   }

   LDPUMA_Console("Press any key...pTheGeomStep1");
   LDPUMA_WaitUserInput(hDebugMy,hDbgWnd);
   LDPUMA_DeleteRects(hDbgWnd,(Word32)hDbgWnd);

   for( i=0;i<pTheGeomStep2->size();i++)
   {
		RECT rect = (*pTheGeomStep2)[i];
	    Rect16 rect16 = {(Int16)rect.left,(Int16)rect.top,(Int16)rect.right,(Int16)rect.bottom};
	    LDPUMA_DrawRect(hDbgWnd,&rect16, 0, RGB(0,127,0),1,(Word32)hDbgWnd);
   }

   LDPUMA_Console("Press any key...pTheGeomStep2");
   LDPUMA_WaitUserInput(hDebugMy,hDbgWnd);
   LDPUMA_DeleteRects(hDbgWnd,(Word32)hDbgWnd);

 }
#endif

 LDPUMA_Skip(hDebugProfEnd);
 return TRUE;
}


#ifdef alDebug
void MyDrawForDebug(void)
{
 Draw_Step=3;
 Draw_Cycle=0;
}
#endif
