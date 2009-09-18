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

#ifndef __RCUTP_H
#define __RCUTP_H

#ifndef __GLOBUS_H
#include "globus.h"
#endif
/*
#ifndef __EVNDEFS_H
#include "evndefs.h"
#endif
*/
#ifndef H_ccomdef_h
#include "ccomdefs.h"
#endif

//#ifndef __EXCDEFS_H
//#include "excdefs.h"
//#endif

#include "roots.h"

#ifdef __RCUTP__
  #define RCUTP_FUNC  FUN_EXPO
#else
  #define RCUTP_FUNC  FUN_IMPO
#endif

#ifdef __cplusplus
	extern "C" {
#endif

#pragma pack (push,8)

#define RCUTP_MAXNAME 260

RCUTP_FUNC(Bool32) RCUTP_Init(uint16_t wHeightCode,Handle hStorage);
RCUTP_FUNC(Bool32) RCUTP_Done();
RCUTP_FUNC(uint32_t) RCUTP_GetReturnCode();
RCUTP_FUNC(char *) RCUTP_GetReturnString(uint32_t dwError);
RCUTP_FUNC(Bool32) RCUTP_GetExportData(uint32_t dwType, void * pData);
RCUTP_FUNC(Bool32) RCUTP_SetImportData(uint32_t dwType, void * pData);

/////////////////////////////////////////////////////////////
typedef enum
{
 		RCUTP_FNRCUTP_CutPoints = 1,
   RCUTP_FNRCUTP_SetBL_for_CutPoints
} RCUTP_EXPORT_ENTRIES;

#define DEC_FUN(a,b,c) typedef a (*FN##b)c; RCUTP_FUNC(a) b c

DEC_FUN(Bool32,  RCUTP_CutPoints, (uchar *raster,struct own_cut *ans, int16_t w,int16_t h,int16_t row));
DEC_FUN(Bool32,  RCUTP_SetBL_for_CutPoints, (int16_t  minrow0,int16_t  bbs10,int16_t  bbs20,
        int16_t  bbs30,int16_t  bbs40, int16_t  Nb10,int16_t  Nb20,int16_t  Nb30,uchar language0));
//DEC_FUN(Bool32, RCUTP_RExc_CP, (void* m, BM* W, ExtComponent* wcomp, uint16_t actual_resolution));
DEC_FUN(int, RCUTP_RSelStr_CP, (CCOM_comp* comp, Bool* type_let, uchar* pmasp));
DEC_FUN(Bool32, RCUTP_RSelStr_SetEnv, (/*char *szImageName,*/ int _medium_h, int _inf_let_w, int _inf_dust_h, int _inf_let_h, Handle hCCOM, int _sup_dust_w, int _min_cut_down_let_w, int _sup_prob_w, const uint16_t biBit));
DEC_FUN(void, RCUTP_RSelStr_UnsetEnv, ());
#undef DEC_FUN

#pragma pack (pop)

#ifdef __cplusplus
            }
#endif

#endif
