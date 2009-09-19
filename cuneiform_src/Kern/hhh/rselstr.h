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

#ifndef __RSELSTR_H
#define __RSELSTR_H

#include "ccomdefs.h"
#include "cpage.h"

#ifndef __GLOBUS_H
#include "globus.h"
#endif

#ifdef __RSELSTR__
  #define RSELSTR_FUNC  FUN_EXPO
#else
  #define RSELSTR_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
	extern "C" {
#endif

#pragma pack (push,8)

#define RSELSTR_MAXNAME 260




RSELSTR_FUNC(Bool32) RSELSTR_Init(uint16_t wHeightCode,Handle hStorage);
RSELSTR_FUNC(Bool32) RSELSTR_Done();
RSELSTR_FUNC(uint32_t) RSELSTR_GetReturnCode();
RSELSTR_FUNC(char *) RSELSTR_GetReturnString(uint32_t dwError);
RSELSTR_FUNC(Bool32) RSELSTR_GetExportData(uint32_t dwType, void * pData);
RSELSTR_FUNC(Bool32) RSELSTR_SetImportData(uint32_t dwType, void * pData);
/////////////////////////////////////////////////////////////
typedef struct struct_CHSTR_Objects
{
	uchar ImageName[CPAGE_MAXNAME];
	Rect16 Rc;
	Bool32 type_vert;
	Bool32 type_neg;
	int32_t block_number;
} CHSTR_Objects;



typedef enum
{
		RSELSTR_FNRSELSTR_ExtractTextStrings = 1,
		RSELSTR_FNRSELSTR_ProgressStart,
		RSELSTR_FNRSELSTR_ProgressStep,
		RSELSTR_FNRSELSTR_ProgressFinish,
		RSELSTR_FNRSELSTR_PutObjects,
		RSELSTR_FNRSELSTR_CutStr,
        RSELSTR_FNRSELSTR_RotateRaster,
        RSELSTR_FNRSELSTR_UnRotateRect,
	    RSELSTR_FNRSELSTR_CleanStr,
		RSELSTR_FNRSELSTR_TestForVertical,
		RSELSTR_FNRSELSTR_CutCompInTableZones

} RSELSTR_EXPORT_ENTRIES;

/*  Описание функций  */

#define DEC_FUN(a,b,c) typedef a (*FN##b)c; RSELSTR_FUNC(a) b c

DEC_FUN(Bool32, RSELSTR_ExtractTextStrings,(Handle hCCOM,Handle hCPAGE));
DEC_FUN(Bool32, RSELSTR_PutObjects, (Handle hCPage,Handle hCCOM,CHSTR_Objects* pObjects,int nObjects));
DEC_FUN(Bool32, RSELSTR_CutStr, (Rect16** ppRc,int& nRc,int& len_mas_Rc,uchar* pRast,int DPIX,int DPIY,int str_w));
DEC_FUN(Bool32, RSELSTR_RotateRaster, (uchar* pmasp,int skew,Rect16* Rc,int16_t* begx,int16_t* movey,uchar* flmovey,int betw_str));
DEC_FUN(Bool32, RSELSTR_UnRotateRect, (int skew,Rect16* pRc,int nRc,Rect16 Rc,int16_t* begx,int16_t* movey,uchar* flmovey,int* hi));
DEC_FUN(void, RSELSTR_CleanStr, (Rect16* pN,CCOM_comp** pC,int& nN,int top,int left,int h,int w,int skew,Bool32 vertical));
DEC_FUN(Bool32, RSELSTR_TestForVertical, (CCOM_handle hCCOM,Handle hCPage,Rect16 RC,Bool32 neg,int param));
DEC_FUN(void, RSELSTR_CutCompInTableZones, (Handle hCPAGE,CCOM_handle hCCOM));

#undef DEC_FUN

typedef void   (*FNRSELSTR_ProgressStart)( void );
typedef	Bool32 (*FNRSELSTR_ProgressStep)(uint32_t perc);
typedef	void   (*FNRSELSTR_ProgressFinish)( void );


#pragma pack (pop)

#ifdef __cplusplus
            }
#endif

#endif
